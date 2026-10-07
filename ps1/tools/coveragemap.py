#!/usr/bin/env python3
"""Draw the function coverage of the PS1 reconstruction as a map of squares.

One square per function of the static sweep, coloured by what the matching
build owns at its bytes: green when a C unit declares them, blue when an
assembly unit does, grey when no unit does. The squares stand in address
order inside blocks. The resident executable has one block for the game
code and one per family of the Sony library, as `families.py` labels them.
The modules have one block per distinct content of a slot, as `pac.py
functions` lists them. Blocks are laid out as a squarified treemap, each
block's area in proportion to its functions.

Two measures, kept apart as the completion map keeps them: the resident
executable counts functions, the modules count function placements. A
content that the build declares as a second link of another image, `like`
in the build configuration, counts its placements again and is marked so.
Above the two panels one bar gives the overall share of distinct functions:
each function of the resident executable once, and each function of the
modules once per address-blind form, the hash that `pac.py functions`
writes, so that code the character modules share is counted once. A form is
exact when any placement of it is. The share of all placements and of all
code bytes stand beside it.

What the sweep lists is an estimate (see the overlay map). The build is the
authority on bytes: a swept function is exact when every byte of it is
owned by a unit of its image, functions and data together. That is how the
first function of slots `0x0` and `0x8` counts, which the sweep begins 16
bytes early at a table that a unit owns as data. A swept function owned
only as data is set aside and counted separately: the sweep read data as
code there. A swept function owned in part is not exact and is counted
separately too, as a boundary to look at. A declared function that touches
no swept function of its image is an error. That is the one guard on the
inventory: it catches a declared function that the inventory does not
represent at all, not an inventory stale within a row. A row shorter or
longer than the function it holds passes, and the sweep's boundaries stay
estimates beside the declared ranges; the JSON gives the declared bytes
next to the bytes exact within swept rows, and they differ where the two
disagree.

Two commands. `sweep` runs the two inventory tools with the project's
settings and writes their tables into a directory, `ps1/inventory/` unless
another is given. It needs the executable and the extracted archives, so it
runs locally only; its tables are published, by the owner's decision of
2026-10-06, and hold addresses and sizes, no bytes. `render` reads that
directory and the build configuration and writes the SVG, a JSON of the
counts and, with `--html`, a page that shows them. It needs no game file,
so the repository's workflow runs it on every push. Nothing of the game, not
an address, enters any of its outputs.

usage:
  coveragemap.py sweep EXECUTABLE PAC_DIRECTORY [--out DIR] [--config FILE] [--families FILE]
                 [--symbols FILE] [--library ADDRESS] [--end ADDRESS] [--pointers ADDRESS]
  coveragemap.py render [DIR] [--config FILE] --svg FILE --json FILE [--html FILE] [--date YYYY-MM-DD]

DIR holds `game.tsv` and `library.tsv` from `families.py` and `modules.tsv`
from `pac.py functions`, in the columns those tools document, and
`contents.tsv`, which `sweep` writes itself: for every content of
`modules.tsv`, its first archive, slot, number of archives and the archives
that carry it. From it a block gets its name: the archive family that
carries the content, `END`, `CONT`, `CDEMO`, the family being the archive
stem without its two-digit number; the stems themselves when three
archives or fewer carry it, `PL11+PL13`; the first family and how many
others when more than two families do, `BOSS+2`. Without `contents.tsv`
a content of a slot with one content is named by the slot.
"""

from __future__ import annotations

import argparse
import datetime
import hashlib
import json
import math
import os
import re
import subprocess
import sys
import tomllib
from bisect import bisect_right
from dataclasses import dataclass, field
from html import escape
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))
import pac  # noqa: E402

INVENTORY = TOOLS.parent / "inventory"
REPOSITORY = "https://github.com/fliperama86/sfa2-decomp"
LIBRARY_BOUNDARY, CODE_END = 0x80157000, 0x8016D920
RESIDENT = "resident"
GAME = "game"
C, ASM, DATA, NONE, PARTIAL = "c", "asm", "data", "none", "partial"
EXACT = (C, ASM)

WIDTH, GAP, PANEL_WIDTHS = 1000, 10, (380, 610)
TOP, HEADER, MAP_HEIGHT, LEGEND = 50, 44, 300, 24
COLORS = {C: "#2ea043", ASM: "#388bfd", NONE: "#6e7681", PARTIAL: "#bb8009"}
SECOND_COLORS = {C: "#1a6b2f", ASM: "#1f5fb0"}  # exact, but a second link of the same objects
LEGEND_TEXT = {C: "exact from C", ASM: "exact from assembly", "second": "exact, linked a second time",
               NONE: "not in the build", PARTIAL: "owned in part"}
BACKGROUND, TEXT = "#0d1117", "#ffffff"
LABEL_SIZE, LABEL_CHAR = 12, 0.56  # font size, and the width of a lowercase character in ems
WIDE_CHAR = 0.74  # capitals and digits, which the archive names are made of


class Problem(Exception):
    pass


@dataclass
class Swept:
    address: int
    size: int
    form: str = ""  # the address-blind hash of a module function; empty for the resident executable
    state: str = NONE


@dataclass
class Block:
    key: str
    label: str
    title: str
    functions: list[Swept]
    second_link: bool = False
    image: str | None = None
    archives: int = 1

    def count(self, *states: str) -> int:
        return sum(f.state in states for f in self.functions)


@dataclass
class Panel:
    name: str
    unit: str
    blocks: list[Block]
    data_rows: int = 0
    bytes_total: int = 0
    bytes_exact: int = 0
    declared_functions: int = 0
    declared_bytes: int = 0
    distinct_total: int = 0
    distinct_exact: int = 0
    overruns: list[tuple[str, int, int, int]] = field(default_factory=list)  # block key, address, size, overrun bytes

    @property
    def functions(self) -> int:
        return sum(len(b.functions) for b in self.blocks)

    def count(self, *states: str) -> int:
        return sum(b.count(*states) for b in self.blocks)


@dataclass
class Owner:
    """What the units of one image own: sorted, non-overlapping ranges."""

    starts: list[int]
    ends: list[int]
    kinds: list[str]
    touched: list[bool]

    def pieces(self, start: int, end: int) -> list[tuple[int, int, str]]:
        """The owned ranges inside [start, end), clipped to it, in address order."""
        out = []
        i = bisect_right(self.ends, start)
        while i < len(self.starts) and self.starts[i] < end:
            s, e = max(start, self.starts[i]), min(end, self.ends[i])
            if e > s:
                out.append((s, e, self.kinds[i]))
                self.touched[i] = True
            i += 1
        return out

    def declared(self) -> tuple[int, int]:
        """Functions declared, and their bytes."""
        items = [e - s for s, e, k in zip(self.starts, self.ends, self.kinds) if k in EXACT]
        return len(items), sum(items)


def anchored(pieces: list[tuple[int, int, str]], start: int, end: int) -> tuple[list[tuple[int, int, str]], int]:
    """The owned pieces that run without a gap from `start`, or else to `end`, when a function is among them.

    The rest of the range is the sweep's overrun: bytes it took into the function that no unit
    owns. The sweep does that where a function ends without a return (the program entry
    routine) and where instruction-shaped data lies before a function that nothing calls.
    Returns no pieces when neither run holds a function.
    """
    for ordered, edge, step in ((pieces, start, 0), (pieces[::-1], end, 1)):
        run = []
        for s, e, kind in ordered:
            if (s, e)[step] != edge:
                break
            run.append((s, e, kind))
            edge = (e, s)[step]
        if any(kind in EXACT for _, _, kind in run):
            return run, (end - start) - sum(e - s for s, e, _ in pieces)
    return [], 0


def read_rows(path: Path, columns: int) -> list[list[str]]:
    rows = []
    for number, line in enumerate(path.read_text().splitlines(), 1):
        if not line.strip():
            continue
        fields = line.split("\t")
        if len(fields) < columns:
            raise Problem(f"{path}:{number}: expected at least {columns} columns, found {len(fields)}")
        rows.append(fields)
    return rows


def read_resident(directory: Path) -> list[Block]:
    """The game block and one block per library family, in the order of the sweep."""
    game = [Swept(int(r[0], 16), int(r[1])) for r in read_rows(directory / "game.tsv", 3)]
    families: dict[str, list[Swept]] = {}
    for r in read_rows(directory / "library.tsv", 4):
        families.setdefault(r[3], []).append(Swept(int(r[0], 16), int(r[1])))
    blocks = [Block(GAME, "Game", "game code", game)]
    blocks += [Block(f"library/{name}", name, f"library: {name}", functions) for name, functions in families.items()]
    for block in blocks:
        block.functions.sort(key=lambda f: f.address)
    return blocks


FAMILY = re.compile(r"^(.*?)(?:[0-9A-F]{2})?X?$")


def family(archive: str) -> str:
    """The archive's family: its stem without a two-digit number and a side mark, `CONT00X` to `CONT`."""
    stem = archive.rsplit(".", 1)[0]
    return FAMILY.match(stem).group(1) or stem


def content_label(archives: list[str]) -> str:
    """The name of a content from the archives that carry it, as the module docstring says."""
    stems = [a.rsplit(".", 1)[0] for a in archives]
    if len(stems) <= 3:
        return "+".join(stems)
    families = list(dict.fromkeys(family(a) for a in archives))
    if len(families) <= 2:
        return "+".join(families)
    return f"{families[0]}+{len(families) - 1}"


def content_title(slot: int, archives: list[str]) -> str:
    if len(archives) <= 3:
        return f"slot {slot:#x}, " + ", ".join(archives)
    return f"slot {slot:#x}, {archives[0]} and {len(archives) - 1} more archives"


def read_contents(directory: Path) -> dict[tuple[int, str], list[str]] | None:
    """The archives of every content, by slot and first archive, or None when the table is absent."""
    path = directory / "contents.tsv"
    if not path.is_file():
        return None
    out = {}
    for r in read_rows(path, 4):
        archives = r[3].split(",")
        if archives[0] != r[0] or len(archives) != int(r[2]):
            raise Problem(f"{path}: the row of {r[0]} slot {r[1]} does not list its archives as it counts them")
        out[(int(r[1], 16), r[0])] = archives
    return out


def read_modules(directory: Path) -> list[Block]:
    """One block per distinct content of a slot, keyed by slot and first archive."""
    contents: dict[tuple[int, str], list[Swept]] = {}
    for r in read_rows(directory / "modules.tsv", 5):
        contents.setdefault((int(r[1], 16), r[0]), []).append(Swept(int(r[2], 16), int(r[3]), r[4]))
    carriers = read_contents(directory)
    if carriers is not None:
        missing = sorted(f"{slot:#x}/{archive}" for slot, archive in set(contents) ^ set(carriers))
        if missing:
            raise Problem("modules.tsv and contents.tsv disagree about the contents, run `sweep` again: " + ", ".join(missing[:8]))
    per_slot: dict[int, int] = {}
    for slot, _ in contents:
        per_slot[slot] = per_slot.get(slot, 0) + 1
    blocks = []
    for (slot, archive), functions in contents.items():
        functions.sort(key=lambda f: f.address)
        if carriers is not None:
            archives = carriers[(slot, archive)]
            block = Block(f"{slot:#x}/{archive}", content_label(archives), content_title(slot, archives), functions, archives=len(archives))
        else:
            stem = archive.rsplit(".", 1)[0]
            label = f"{slot:#x}" if per_slot[slot] == 1 else stem
            block = Block(f"{slot:#x}/{archive}", label, f"slot {slot:#x}, {archive}", functions)
        blocks.append(block)
    return blocks


def contents_rows(executable: str, directory: str, pointers: int) -> list[str]:
    """One row per content of a code-bearing chunk of table 0, as `pac.py functions` takes them: first archive,
    slot, number of archives, the archives in the order `pac.py` reads them."""
    image = pac.Image(Path(executable).read_bytes())
    table = pac.destination_tables(image, pointers)[0]
    archives, _ = pac.read_archives(directory)
    placements: dict[tuple[int, bytes], list[str]] = {}
    for name, data, chunks in archives:
        for c in chunks:
            if c["table"] != 0 or c["slot"] >= len(table):
                continue
            body = data[c["offset"] : c["offset"] + c["size"]]
            key = (c["slot"], hashlib.sha256(body).digest())
            if key in placements:
                placements[key].append(name)
            elif pac.code_estimate(body):
                placements[key] = [name]
    return [f"{names[0]}\t{slot:#x}\t{len(names)}\t{','.join(names)}" for (slot, _), names in placements.items()]


def load_config(path: Path) -> dict:
    try:
        return tomllib.loads(path.read_text())
    except (OSError, tomllib.TOMLDecodeError) as exc:
        raise Problem(f"cannot read {path}: {exc}") from exc


def owners(config: dict) -> dict[str, Owner]:
    """The ranges that units own, per image. A `like` image gets its base's ranges, shifted."""
    ranges: dict[str, list[tuple[int, int, str]]] = {RESIDENT: []}
    images = {image["name"]: image for image in config.get("image", [])}
    for name in images:
        ranges[name] = []
    for unit in config.get("unit", []):
        image = unit.get("image", RESIDENT)
        if image not in ranges:
            raise Problem(f"unit {unit['name']!r} names an image {image!r} that the configuration does not declare")
        kind = ASM if unit.get("kind") == "asm" else C
        for function in unit.get("functions", []):
            ranges[image].append((function["address"], function["address"] + function["size"], kind))
        for key in ("rodata", "data"):
            if key in unit:
                ranges[image].append((unit[key]["address"], unit[key]["address"] + unit[key]["size"], DATA))
    for name, image in images.items():
        if "like" in image:
            base = images.get(image["like"])
            if base is None:
                raise Problem(f"image {name!r} is like {image['like']!r}, which the configuration does not declare")
            if ranges[name]:
                raise Problem(f"image {name!r} is like {image['like']!r} and has units of its own")
            shift = image["address"] - base["address"]
            ranges[name] = [(s + shift, e + shift, kind) for s, e, kind in ranges[base["name"]]]
    result = {}
    for name, items in ranges.items():
        items.sort()
        for (s1, e1, _), (s2, _, _) in zip(items, items[1:]):
            if s2 < e1:
                raise Problem(f"image {name!r}: owned ranges overlap at {s2:#x}")
        result[name] = Owner([s for s, _, _ in items], [e for _, e, _ in items], [k for _, _, k in items], [False] * len(items))
    return result


def assign_images(blocks: list[Block], config: dict) -> None:
    """Which image of the build each content is. Fails when an image fits no content."""
    images = config.get("image", [])
    per_slot: dict[int, int] = {}
    for block in blocks:
        slot = int(block.key.split("/")[0], 16)
        per_slot[slot] = per_slot.get(slot, 0) + 1
    taken: dict[str, str] = {}
    for block in blocks:
        slot_text, archive = block.key.split("/", 1)
        slot = int(slot_text, 16)
        candidates = [image for image in images if image["slot"] == slot]
        if per_slot[slot] > 1 or len(candidates) > 1:
            candidates = [image for image in candidates if Path(image["archive"]).name == archive]
        if len(candidates) > 1:
            raise Problem(f"content {block.key}: several images of the build fit it: " + ", ".join(i["name"] for i in candidates))
        if candidates:
            image = candidates[0]
            block.image = image["name"]
            block.second_link = "like" in image
            taken[image["name"]] = block.key
    for image in images:
        if image["name"] not in taken:
            raise Problem(
                f"image {image['name']!r} (slot {image['slot']:#x}, {Path(image['archive']).name}) fits no content of the inventory"
            )


def classify(panel: Panel, owned: dict[str, Owner], image_of: dict[str, str | None]) -> None:
    """Set the state of every swept function from the bytes the build owns, and total the bytes."""
    for block in panel.blocks:
        image = image_of[block.key]
        kept = []
        for function in block.functions:
            start, end = function.address, function.address + function.size
            panel.bytes_total += function.size
            pieces = owned[image].pieces(start, end) if image else []
            panel.bytes_exact += sum(e - s for s, e, kind in pieces if kind in EXACT)
            owned_bytes = sum(e - s for s, e, _ in pieces)
            if owned_bytes < function.size and pieces:
                pieces, overrun = anchored(pieces, start, end)
                if pieces:
                    panel.overruns.append((block.key, start, function.size, overrun))
                else:
                    function.state = PARTIAL
            if pieces and function.state != PARTIAL:
                kinds = {kind for _, _, kind in pieces}
                function.state = C if C in kinds else ASM if ASM in kinds else DATA
            if function.state == DATA:
                panel.data_rows += 1
                panel.bytes_total -= function.size
            else:
                kept.append(function)
        block.functions = kept
    for name in sorted({image for image in image_of.values() if image}):
        owner = owned[name]
        missed = [f"{owner.starts[i]:#x}" for i, (kind, touched) in enumerate(zip(owner.kinds, owner.touched)) if kind in EXACT and not touched]
        if missed:
            raise Problem(f"image {name!r} declares {len(missed)} function(s) that touch no swept function; the inventory does not represent them, run `sweep` again: " + ", ".join(missed[:8]))
        functions, size = owner.declared()
        panel.declared_functions += functions
        panel.declared_bytes += size
    forms: dict[object, bool] = {}
    for block in panel.blocks:
        for function in block.functions:
            key = function.form or (block.key, function.address)
            forms[key] = forms.get(key, False) or function.state in EXACT
    panel.distinct_total, panel.distinct_exact = len(forms), sum(forms.values())


def overall(panels: list[Panel]) -> dict:
    """The three whole-program shares: distinct functions, placements, code bytes."""
    distinct_exact, distinct_total = sum(p.distinct_exact for p in panels), sum(p.distinct_total for p in panels)
    exact, total = sum(p.count(*EXACT) for p in panels), sum(p.functions for p in panels)
    bytes_exact, bytes_total = sum(p.bytes_exact for p in panels), sum(p.bytes_total for p in panels)
    return {
        "distinct_exact": distinct_exact, "distinct_total": distinct_total, "percent": percent(distinct_exact, distinct_total),
        "placements_exact": exact, "placements_total": total, "placements_percent": percent(exact, total),
        "bytes_exact": bytes_exact, "bytes_total": bytes_total, "bytes_percent": percent(bytes_exact, bytes_total),
    }


def overall_lines(whole: dict) -> tuple[str, str]:
    title = f"Overall: {whole['percent']}% of distinct functions ({whole['distinct_exact']:,}/{whole['distinct_total']:,})"
    detail = (f"resident functions once each, module functions once per address-blind form · "
              f"{whole['placements_percent']}% of all placements ({whole['placements_exact']:,}/{whole['placements_total']:,}) · "
              f"{whole['bytes_percent']}% of the code bytes")
    return title, detail


def build_panels(directory: Path, config: dict) -> list[Panel]:
    owned = owners(config)
    resident = Panel(RESIDENT, "functions", read_resident(directory))
    classify(resident, owned, {block.key: RESIDENT for block in resident.blocks})
    modules = Panel("modules", "function placements", read_modules(directory))
    assign_images(modules.blocks, config)
    classify(modules, owned, {block.key: block.image for block in modules.blocks})
    unused = sorted(name for name in owned if name != RESIDENT and name not in {b.image for b in modules.blocks})
    if unused:
        raise Problem("images with no content in the inventory: " + ", ".join(unused))
    return [resident, modules]


# Layout: a squarified treemap (Bruls, Huizing and van Wijk, 2000), written here.


def worst(row: list[float], side: float) -> float:
    total = sum(row)
    return max(max(side * side * r / (total * total), total * total / (side * side * r)) for r in row)


def lay_row(row: list[float], x: float, y: float, w: float, h: float, out: list) -> tuple[float, float, float, float]:
    total = sum(row)
    if w >= h:
        strip = total / h
        offset = y
        for area in row:
            out.append((x, offset, strip, area / strip))
            offset += area / strip
        return x + strip, y, w - strip, h
    strip = total / w
    offset = x
    for area in row:
        out.append((offset, y, area / strip, strip))
        offset += area / strip
    return x, y + strip, w, h - strip


def squarify(values: list[float], x: float, y: float, w: float, h: float) -> list[tuple[float, float, float, float]]:
    """Rectangles for values given in descending order, areas in proportion, filling the rectangle."""
    if not values:
        return []
    scale = w * h / sum(values)
    areas = [v * scale for v in values]
    out: list = []
    row: list[float] = []
    for area in areas:
        side = min(w, h)
        if row and worst(row + [area], side) > worst(row, side):
            x, y, w, h = lay_row(row, x, y, w, h, out)
            row = []
        row.append(area)
    if row:
        lay_row(row, x, y, w, h, out)
    return out


def grid(count: int, x: float, y: float, w: float, h: float) -> list[tuple[float, float, float, float]]:
    """`count` cells filling the rectangle in rows of near-square cells, in reading order.

    The first rows hold one cell more where the count does not divide evenly, so no row is
    left short and the cells of a row share one width.
    """
    if count <= 0 or w <= 0 or h <= 0:
        return []
    rows = max(1, min(count, round(math.sqrt(count * h / w))))
    cells = []
    ch = h / rows
    for r in range(rows):
        in_row = count // rows + (r < count % rows)
        cw = w / in_row
        cells += [(x + c * cw, y + r * ch, cw, ch) for c in range(in_row)]
    return cells


def rect(x: float, y: float, w: float, h: float, fill: str, stroke: float = 0.5) -> str:
    return (f'<rect x="{x:.1f}" y="{y:.1f}" width="{w:.1f}" height="{h:.1f}" fill="{fill}" '
            f'stroke="{BACKGROUND}" stroke-width="{stroke}"/>')


def text(x: float, y: float, value: str, size: int, weight: str = "normal") -> str:
    return (f'<text x="{x:.2f}" y="{y:.2f}" font-family="sans-serif" font-size="{size}" font-weight="{weight}" '
            f'fill="{TEXT}" stroke="{BACKGROUND}" stroke-width="3" paint-order="stroke">{escape(value)}</text>')


def percent(part: int, whole: int) -> float:
    return round(100 * part / whole, 1) if whole else 0.0


def headline(panel: Panel) -> tuple[str, str]:
    exact, total = panel.count(*EXACT), panel.functions
    if panel.name == RESIDENT:
        title = f"Resident executable: {percent(exact, total)}% ({exact:,}/{total:,})"
        detail = f"functions · {panel.count(C):,} from C, {panel.count(ASM):,} from assembly"
    else:
        again = sum(b.count(*EXACT) for b in panel.blocks if b.second_link)
        title = f"Overlay modules: {percent(exact, total)}% ({exact:,}/{total:,})"
        detail = f"function placements · {exact - again:,} from source, {again:,} linked a second time"
    detail += f" · {percent(panel.bytes_exact, panel.bytes_total)}% of the code bytes"
    return title, detail


def label_width(label: str, size: int) -> float:
    return sum(WIDE_CHAR if c.isupper() or c.isdigit() else LABEL_CHAR for c in label) * size


def fit_label(label: str, w: float, h: float) -> int:
    """The font size at which the whole label fits the block, or 0. Labels are never cut short:
    a prefix can read as another block's name, `0x2` for `0x2b`."""
    for size in (LABEL_SIZE, 10, 9):
        if h >= size + 8 and label_width(label, size) + 8 <= w:
            return size
    return 0


def cells_path(cells: list[tuple[float, float, float, float]], fill: str) -> str:
    d = "".join(f"M{x:.1f} {y:.1f}h{w:.1f}v{h:.1f}h{-w:.1f}Z" for x, y, w, h in cells)
    return f'<path fill="{fill}" stroke="{BACKGROUND}" stroke-width="0.5" d="{d}"/>'


def draw_panel(panel: Panel, left: float, width: float, top: float) -> list[str]:
    title, detail = headline(panel)
    parts = [text(left + 4, top + 18, title, 16, "bold"), text(left + 4, top + 36, detail, 11)]
    blocks = sorted((b for b in panel.blocks if b.functions), key=lambda b: (-len(b.functions), b.key))
    rects = squarify([float(len(b.functions)) for b in blocks], left, top + HEADER, width, MAP_HEIGHT)
    for block, (x, y, w, h) in zip(blocks, rects):
        exact, total = block.count(*EXACT), len(block.functions)
        note = " (second link)" if block.second_link else ""
        parts.append(f"<g><title>{escape(block.title)}{note}: {exact}/{total} ({percent(exact, total)}%)</title>")
        colors = {**COLORS, **SECOND_COLORS} if block.second_link else COLORS
        by_state: dict[str, list] = {}
        for function, cell in zip(block.functions, grid(total, x + 1, y + 1, w - 2, h - 2)):
            by_state.setdefault(function.state, []).append(cell)
        for state, cells in by_state.items():
            parts.append(cells_path(cells, colors[state]))
        parts.append(rect(x, y, w, h, "none", 2))
        size = fit_label(block.label, w, h)
        if size:
            parts.append(text(x + 4, y + size + 3, block.label, size))
        parts.append("</g>")
    return parts


def render(panels: list[Panel], date: str) -> str:
    height = TOP + HEADER + MAP_HEIGHT + LEGEND
    parts = [
        f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {WIDTH} {height}" width="{WIDTH}" height="{height}">',
        rect(0, 0, WIDTH, height, BACKGROUND, 0),
    ]
    whole = overall(panels)
    title, detail = overall_lines(whole)
    bar = WIDTH - 8
    parts += [
        text(4, 18, title, 16, "bold"),
        rect(4, 24, bar, 8, COLORS[NONE], 0),
        rect(4, 24, bar * whole["distinct_exact"] / whole["distinct_total"] if whole["distinct_total"] else 0, 8, COLORS[C], 0),
        text(4, 45, detail, 11),
    ]
    left = 0.0
    for panel, width in zip(panels, PANEL_WIDTHS):
        parts += draw_panel(panel, left, width, TOP)
        left += width + GAP
    y = TOP + HEADER + MAP_HEIGHT + 8
    x = 4.0
    present = {f.state for p in panels for b in p.blocks for f in b.functions}
    if any(b.second_link and b.count(*EXACT) for p in panels for b in p.blocks):
        present.add("second")
    for state in (C, ASM, "second", NONE, PARTIAL):
        if state in present:
            parts.append(rect(x, y, 10, 10, SECOND_COLORS[C] if state == "second" else COLORS[state]))
            parts.append(text(x + 14, y + 9, LEGEND_TEXT[state], 11))
            x += 14 + 11 * LABEL_CHAR * len(LEGEND_TEXT[state]) + 16
    parts.append(text(x, y + 9, f"one square per function of the static sweep, in address order · {date}", 11))
    parts.append("</svg>")
    return "\n".join(parts) + "\n"


def summary(panels: list[Panel], date: str, config: Path) -> dict:
    def counts(blocks: list[Block]) -> dict:
        total = sum(len(b.functions) for b in blocks)
        exact = sum(b.count(*EXACT) for b in blocks)
        return {
            "total": total,
            "exact": exact,
            "percent": percent(exact, total),
            "exact_c": sum(b.count(C) for b in blocks),
            "exact_asm": sum(b.count(ASM) for b in blocks),
            "partial": sum(b.count(PARTIAL) for b in blocks),
        }

    out = {"date": date, "config": config.name, "overall": overall(panels), "panels": {}}
    for panel in panels:
        entry = counts(panel.blocks)
        entry.update(unit=panel.unit, bytes_total=panel.bytes_total, bytes_exact=panel.bytes_exact,
                     bytes_percent=percent(panel.bytes_exact, panel.bytes_total),
                     distinct_total=panel.distinct_total, distinct_exact=panel.distinct_exact,
                     declared_functions=panel.declared_functions, declared_bytes=panel.declared_bytes,
                     overrun_functions=len(panel.overruns), overrun_bytes=sum(o[3] for o in panel.overruns),
                     swept_as_data=panel.data_rows)
        if panel.name != RESIDENT:
            entry["second_links"] = sum(b.count(*EXACT) for b in panel.blocks if b.second_link)
        blocks = []
        for block in sorted(panel.blocks, key=lambda b: b.key):
            item = {"key": block.key, "label": block.label, **counts([block])}
            if panel.name != RESIDENT:
                item.update(image=block.image, second_link=block.second_link, archives=block.archives)
            blocks.append(item)
        entry["blocks"] = blocks
        out["panels"][panel.name] = entry
    return out


def page(panels: list[Panel], date: str, svg: str, counts: str) -> str:
    """A page that shows the map, the counts of every block and where the numbers come from.

    `svg` and `counts` are the links to the picture and the JSON, relative to the page.
    """
    style = ("body{font-family:sans-serif;max-width:1000px;margin:auto;padding:16px;background:#0d1117;color:#e6edf3}"
             "img{max-width:100%}table{border-collapse:collapse}th,td{border:1px solid #30363d;padding:2px 8px}"
             "td+td,th+th{text-align:right}a{color:#58a6ff}")
    lines = [
        "<!DOCTYPE html>",
        '<html lang="en"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width, initial-scale=1">',
        "<title>SFA2 PS1 reconstruction: coverage map</title>", f"<style>{style}</style></head><body>",
        "<h1>SFA2 PS1 reconstruction: coverage map</h1>",
        f'<img src="{escape(svg)}" alt="one square per function of the static sweep, green where the build owns it">',
        f'<p>One square per function of the static sweep, in address order, drawn on {date} by '
        f'<a href="{REPOSITORY}/blob/main/ps1/tools/coveragemap.py">coveragemap.py</a> from '
        f'<a href="{REPOSITORY}/tree/main/ps1/inventory">the inventory</a> and '
        f'<a href="{REPOSITORY}/blob/main/ps1/src/build.toml">the build configuration</a>. '
        f'Counts: <a href="{escape(counts)}">{escape(counts)}</a>. What the measures mean and what remains: '
        f'<a href="{REPOSITORY}/blob/main/docs/completion-map.md">the completion map</a>.</p>',
    ]
    whole = overall(panels)
    title, detail = overall_lines(whole)
    lines += [
        f"<h2>{escape(title)}</h2>", f"<p>{escape(detail)}</p>",
        "<table><tr><th>Measure</th><th>Exact</th><th>Total</th><th>%</th></tr>",
        f"<tr><td>distinct functions</td><td>{whole['distinct_exact']}</td><td>{whole['distinct_total']}</td><td>{whole['percent']}</td></tr>",
        f"<tr><td>function placements</td><td>{whole['placements_exact']}</td><td>{whole['placements_total']}</td><td>{whole['placements_percent']}</td></tr>",
        f"<tr><td>code bytes</td><td>{whole['bytes_exact']}</td><td>{whole['bytes_total']}</td><td>{whole['bytes_percent']}</td></tr>",
        "</table>",
    ]
    for panel in panels:
        title, detail = headline(panel)
        lines += [f"<h2>{escape(title)}</h2>", f"<p>{escape(detail)}</p>", "<table>"]
        second = panel.name != RESIDENT
        lines.append("<tr><th>Block</th><th>Exact</th><th>Total</th><th>%</th>" + ("<th>Image</th>" if second else "") + "</tr>")
        for block in sorted(panel.blocks, key=lambda b: b.key):
            exact, total = block.count(*EXACT), len(block.functions)
            image = (escape(block.image or "-") + (", second link" if block.second_link else "")) if second else ""
            lines.append(f"<tr><td>{escape(block.title)}</td><td>{exact}</td><td>{total}</td><td>{percent(exact, total)}</td>"
                         + (f"<td>{image}</td>" if second else "") + "</tr>")
        exact, total = panel.count(*EXACT), panel.functions
        lines.append(f"<tr><th>Total</th><th>{exact}</th><th>{total}</th><th>{percent(exact, total)}</th>" + ("<th></th>" if second else "") + "</tr>")
        lines.append("</table>")
    lines.append("</body></html>")
    return "\n".join(lines) + "\n"


def cmd_render(args) -> int:
    directory, config_path = Path(args.directory), Path(args.config)
    for name in ("game.tsv", "library.tsv", "modules.tsv"):
        if not (directory / name).is_file():
            raise Problem(f"{directory / name} is missing; run `coveragemap.py sweep` first")
    panels = build_panels(directory, load_config(config_path))
    date = args.date or datetime.date.today().isoformat()
    Path(args.svg).parent.mkdir(parents=True, exist_ok=True)
    Path(args.svg).write_text(render(panels, date))
    Path(args.json).parent.mkdir(parents=True, exist_ok=True)
    Path(args.json).write_text(json.dumps(summary(panels, date, config_path), indent=1) + "\n")
    if args.html:
        html = Path(args.html).resolve()
        html.parent.mkdir(parents=True, exist_ok=True)
        links = [os.path.relpath(Path(target).resolve(), html.parent) for target in (args.svg, args.json)]
        html.write_text(page(panels, date, *links))
    for panel in panels:
        exact, total = panel.count(*EXACT), panel.functions
        line = f"{panel.name}: {exact}/{total} {panel.unit} exact ({percent(exact, total)}%), {panel.count(C)} from C, {panel.count(ASM)} from assembly"
        line += f"; {panel.bytes_exact}/{panel.bytes_total} code bytes ({percent(panel.bytes_exact, panel.bytes_total)}%)"
        line += f"; the build declares {panel.declared_functions} functions, {panel.declared_bytes} bytes"
        if panel.count(PARTIAL):
            line += f"; owned in part: {panel.count(PARTIAL)}"
        if panel.data_rows:
            line += f"; swept as code but owned as data: {panel.data_rows}"
        print(line)
        for key, address, size, overrun in panel.overruns:
            print(f"  sweep overrun in {key}: the function at {address:#x} is {size} bytes to the sweep, {overrun} of them owned by no unit")
    whole = overall(panels)
    print(f"overall: {whole['distinct_exact']}/{whole['distinct_total']} distinct functions exact ({whole['percent']}%);"
          f" {whole['placements_exact']}/{whole['placements_total']} placements ({whole['placements_percent']}%);"
          f" {whole['bytes_exact']}/{whole['bytes_total']} code bytes ({whole['bytes_percent']}%)")
    return 0


def cmd_sweep(args) -> int:
    out = Path(args.out)
    out.mkdir(parents=True, exist_ok=True)
    config = load_config(Path(args.config))
    pointers = args.pointers if args.pointers is not None else config.get("overlays", {}).get("table_pointers")
    if pointers is None:
        raise Problem("no --pointers given and the configuration has no [overlays] table_pointers")
    commands = [
        [sys.executable, str(TOOLS / "families.py"), args.executable, "--config", args.config, "--families", args.families,
         "--library", f"{args.library:#x}", "--end", f"{args.end:#x}", "--symbols", args.symbols,
         "--out", str(out / "game.tsv"), "--library-out", str(out / "library.tsv")],
        [sys.executable, str(TOOLS / "pac.py"), "functions", args.executable, args.pac_directory, "--pointers", f"{pointers:#x}",
         "--symbols", args.symbols, "--out", str(out / "modules.tsv")],
    ]
    for command in commands:
        proc = subprocess.run(command, capture_output=True, text=True)
        sys.stdout.write(proc.stdout)
        sys.stderr.write(proc.stderr)
        if proc.returncode:
            raise Problem(f"{Path(command[1]).name} exited with {proc.returncode}")
    try:
        rows = contents_rows(args.executable, args.pac_directory, pointers)
    except (OSError, pac.FormatError) as exc:
        raise Problem(f"contents: {exc}") from exc
    (out / "contents.tsv").write_text("".join(row + "\n" for row in rows))
    print(f"inventory written to {out}")
    return 0


def number(value: str) -> int:
    return int(value, 0)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0], formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="command", required=True)
    root = TOOLS.parent
    sweep = sub.add_parser("sweep", help="run the inventory tools and write their tables into a directory")
    sweep.add_argument("executable")
    sweep.add_argument("pac_directory")
    sweep.add_argument("--out", default=str(INVENTORY), help="directory for game.tsv, library.tsv and modules.tsv")
    sweep.add_argument("--config", default=str(root / "src" / "build.toml"))
    sweep.add_argument("--families", default=str(root / "src" / "library-families.toml"))
    sweep.add_argument("--symbols", default=str(root / "src" / "symbols.ld"))
    sweep.add_argument("--library", type=number, default=LIBRARY_BOUNDARY, help="first address of the library area")
    sweep.add_argument("--end", type=number, default=CODE_END, help="end of the resident code")
    sweep.add_argument("--pointers", type=number, help="address of the loader's table pointers; default: the configuration's")
    sweep.set_defaults(run=cmd_sweep)
    render_ = sub.add_parser("render", help="draw the map from an inventory directory and the build configuration")
    render_.add_argument("directory", nargs="?", default=str(INVENTORY), help="the inventory directory")
    render_.add_argument("--config", default=str(root / "src" / "build.toml"))
    render_.add_argument("--svg", required=True)
    render_.add_argument("--json", required=True)
    render_.add_argument("--html", help="also write a page that shows the map and the counts")
    render_.add_argument("--date", help="date written into the map; default: today")
    render_.set_defaults(run=cmd_render)
    args = parser.parse_args(argv)
    try:
        return args.run(args)
    except Problem as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
