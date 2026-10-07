#!/usr/bin/env python3
"""Controls for coveragemap.py using synthetic inputs.

Builds small inventories, build configurations and, for the `sweep` command, a
fake PS-X executable with one archive in a temporary directory. No game data is
involved. The expected counts are worked out here from each layout, not read
from the tool.
"""

from __future__ import annotations

import json
import random
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

from test_disc_tools import make_archive, make_program

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))
import coveragemap  # noqa: E402

OPEN, CLOSE, RETURN, ONE = 0x27BDFFE8, 0x27BD0018, 0x03E00008, 0x24020001
START = 0x80010000


def tool(*args) -> subprocess.CompletedProcess:
    return subprocess.run([sys.executable, str(TOOLS / "coveragemap.py"), *map(str, args)], capture_output=True, text=True)


def verdict(ok: bool, detail: str) -> subprocess.CompletedProcess:
    return subprocess.CompletedProcess([], 0 if ok else 1, "as required" if ok else "", "" if ok else detail)


def game_row(address: int, size: int) -> str:
    return f"{address:08x}\t{size}\tfunc_{address:08x}\t-\t-\t0\t0\tclosed\n"


def library_row(address: int, size: int, family: str) -> str:
    return f"{address:08x}\t{size}\t-\t{family}\t0\tfolder\n"


def module_row(archive: str, slot: int, address: int, size: int, form: str | None = None) -> str:
    """A row of `pac.py functions`. The address-blind form defaults to the low address bits and the size,
    so that the same code linked at another address shares it, as a second link does."""
    return f"{archive}\t{slot:#x}\t{address:08x}\t{size}\t{form or f'{address & 0xFFFF:04x}-{size}'}\n"


def inventory(root: Path, name: str, game: str = "", library: str = "", modules: str = "", contents: str | None = None) -> Path:
    directory = root / name
    directory.mkdir(parents=True, exist_ok=True)
    (directory / "game.tsv").write_text(game)
    (directory / "library.tsv").write_text(library)
    (directory / "modules.tsv").write_text(modules)
    if contents is not None:
        (directory / "contents.tsv").write_text(contents)
    return directory


def contents_row(first: str, slot: int, *others: str) -> str:
    archives = [first, *others]
    return f"{first}\t{slot:#x}\t{len(archives)}\t{','.join(archives)}\n"


def unit(name: str, functions: list[tuple[int, int]], image: str | None = None, kind: str | None = None, **data) -> str:
    text = f'[[unit]]\nname = "{name}"\nsource = "{name}.c"\n'
    if image:
        text += f'image = "{image}"\n'
    if kind:
        text += f'kind = "{kind}"\n'
    text += "functions = [" + ", ".join(f'{{ name = "f_{a:x}", address = {a:#x}, size = {s} }}' for a, s in functions) + "]\n"
    for key, (address, size) in data.items():
        text += f"{key} = {{ address = {address:#x}, size = {size} }}\n"
    return text + "\n"


def image(name: str, slot: int, archive: str, address: int, like: str | None = None) -> str:
    text = f'[[image]]\nname = "{name}"\n'
    if like:
        text += f'like = "{like}"\n'
    return text + f'archive = "../x/{archive}"\nslot = {slot:#x}\nsha256 = "00"\naddress = {address:#x}\n\n'


def render(root: Path, label: str, directory: Path, config_text: str):
    config = root / f"{label}.toml"
    config.write_text(config_text)
    svg, out = root / f"{label}.svg", root / f"{label}.json"
    proc = tool("render", directory, "--config", config, "--svg", svg, "--json", out, "--date", "2026-01-01")
    data = json.loads(out.read_text()) if out.exists() else None
    text = svg.read_text() if svg.exists() else ""
    return proc, data, text


def main_cases(root: Path):
    """A resident executable of two areas and modules of four contents, one of them a second link."""
    game = "".join(game_row(START + 32 * k, 32) for k in range(5))
    library = library_row(0x80011000, 16, "sound") + library_row(0x80011010, 16, "sound") + library_row(0x80011020, 16, "disc")
    # Forms: PL01 shares its first function with PL00; the second link shares both of its base's.
    modules = "".join(module_row("PL00.PAC", 4, 0x801B0000 + 32 * k, 32, form) for k, form in enumerate("abc"))
    modules += "".join(module_row("PL01.PAC", 4, 0x801B0000 + 32 * k, 32, form) for k, form in enumerate("ad"))
    modules += module_row("CONT00.PAC", 0x12, 0x80010000, 64, "e") + module_row("CONT00.PAC", 0x12, 0x80010040, 32, "f")
    modules += module_row("CONT00X.PAC", 0x13, 0x80020000, 64, "e") + module_row("CONT00X.PAC", 0x13, 0x80020040, 32, "f")
    contents = (contents_row("PL00.PAC", 4) + contents_row("PL01.PAC", 4)
                + contents_row("CONT00.PAC", 0x12, "CONT00X.PAC", "CONT01.PAC", "CONT01X.PAC", "CONT02.PAC") + contents_row("CONT00X.PAC", 0x13))
    directory = inventory(root, "main", game, library, modules, contents)
    config = (
        unit("a", [(START, 32), (START + 96, 32)])
        + unit("b", [(START + 64, 32)], kind="asm")
        + unit("l", [(0x80011000, 16)])
        + image("slot12", 0x12, "CONT00.PAC", 0x80010000)
        + image("slot13", 0x13, "CONT00X.PAC", 0x80020000, like="slot12")
        + image("pl01", 0x4, "PL01.PAC", 0x801B0000)
        + unit("m12", [(0x80010000, 64)], image="slot12")
        + unit("m01", [(0x801B0020, 32)], image="pl01")
    )
    proc, data, svg = render(root, "main", directory, config)
    yield "main-resident-line", proc, 0, (
        "resident: 4/8 functions exact (50.0%), 3 from C, 1 from assembly; 112/208 code bytes (53.8%); the build declares 4 functions, 112 bytes\n"
    )
    yield "main-modules-line", proc, 0, (
        "modules: 3/9 function placements exact (33.3%), 3 from C, 0 from assembly; 160/352 code bytes (45.5%); the build declares 3 functions, 160 bytes\n"
    )
    yield "main-no-overrun", verdict("overrun" not in proc.stdout, proc.stdout), 0, "as required"
    # Distinct: 8 resident functions once each; module forms a..f, of which d (PL01) and e (0x12 and its second link) are exact.
    yield "main-overall-line", proc, 0, "overall: 6/14 distinct functions exact (42.9%); 7/17 placements (41.2%); 272/560 code bytes (48.6%)\n"
    yield "main-json-overall", verdict(
        data["overall"] == {"distinct_exact": 6, "distinct_total": 14, "percent": 42.9, "placements_exact": 7, "placements_total": 17,
                            "placements_percent": 41.2, "bytes_exact": 272, "bytes_total": 560, "bytes_percent": 48.6},
        json.dumps(data["overall"]),
    ), 0, "as required"
    yield "main-json-distinct-per-panel", verdict(
        (data["panels"]["resident"]["distinct_total"], data["panels"]["resident"]["distinct_exact"],
         data["panels"]["modules"]["distinct_total"], data["panels"]["modules"]["distinct_exact"]) == (8, 4, 6, 2),
        json.dumps(data["panels"]["modules"]),
    ), 0, "as required"
    resident, modules_ = data["panels"]["resident"], data["panels"]["modules"]
    yield "main-json-resident", verdict(
        (resident["total"], resident["exact"], resident["exact_c"], resident["exact_asm"], resident["partial"], resident["percent"]) == (8, 4, 3, 1, 0, 50.0)
        and (resident["bytes_total"], resident["bytes_exact"], resident["declared_functions"], resident["declared_bytes"]) == (208, 112, 4, 112)
        and resident["overrun_functions"] == 0 and resident["swept_as_data"] == 0 and "second_links" not in resident,
        json.dumps(resident),
    ), 0, "as required"
    blocks = {b["key"]: (b["total"], b["exact"], b["exact_c"], b["exact_asm"]) for b in resident["blocks"]}
    yield "main-json-resident-blocks", verdict(
        blocks == {"game": (5, 3, 2, 1), "library/sound": (2, 1, 1, 0), "library/disc": (1, 0, 0, 0)}, json.dumps(blocks)
    ), 0, "as required"
    yield "main-json-modules", verdict(
        (modules_["total"], modules_["exact"], modules_["second_links"], modules_["bytes_total"], modules_["bytes_exact"]) == (9, 3, 1, 352, 160)
        and (modules_["declared_functions"], modules_["declared_bytes"], modules_["unit"]) == (3, 160, "function placements"),
        json.dumps(modules_),
    ), 0, "as required"
    mblocks = {b["key"]: (b["label"], b["total"], b["exact"], b["image"], b["second_link"], b["archives"]) for b in modules_["blocks"]}
    yield "main-json-module-blocks", verdict(
        mblocks == {
            "0x4/PL00.PAC": ("PL00", 3, 0, None, False, 1),
            "0x4/PL01.PAC": ("PL01", 2, 1, "pl01", False, 1),
            "0x12/CONT00.PAC": ("CONT", 2, 1, "slot12", False, 5),
            "0x13/CONT00X.PAC": ("CONT00X", 2, 1, "slot13", True, 1),
        },
        json.dumps(mblocks),
    ), 0, "as required"
    wanted = [
        "Overall: 42.9% of distinct functions (6/14)",
        "resident functions once each, module functions once per address-blind form · 41.2% of all placements (7/17) · 48.6% of the code bytes",
        "Resident executable: 50.0% (4/8)", "functions · 3 from C, 1 from assembly · 53.8% of the code bytes",
        "Overlay modules: 33.3% (3/9)", "function placements · 2 from source, 1 linked a second time · 45.5% of the code bytes",
        "<title>game code: 3/5 (60.0%)</title>", "<title>library: sound: 1/2 (50.0%)</title>", "<title>library: disc: 0/1 (0.0%)</title>",
        "<title>slot 0x4, PL00.PAC: 0/3 (0.0%)</title>", "<title>slot 0x13, CONT00X.PAC (second link): 1/2 (50.0%)</title>",
        "<title>slot 0x12, CONT00.PAC and 4 more archives: 1/2 (50.0%)</title>",
        ">Game</text>", ">PL00</text>", ">CONT</text>",
        "exact from C", "exact from assembly", "exact, linked a second time", "not in the build", "2026-01-01",
    ]
    for item in wanted:
        yield f"main-svg-has {item[:40]!r}", verdict(item in svg, svg[:3000]), 0, "as required"
    yield "main-svg-no-partial-legend", verdict("owned in part" not in svg, ""), 0, "as required"
    colors = {state: svg.count(f'fill="{color}"') for state, color in coveragemap.COLORS.items()}
    yield "main-svg-colors", verdict(colors[coveragemap.C] > 0 and colors[coveragemap.ASM] > 0 and colors[coveragemap.PARTIAL] == 0, json.dumps(colors)), 0, "as required"
    yield "main-svg-second-link-color", verdict(svg.count(f'<path fill="{coveragemap.SECOND_COLORS[coveragemap.C]}"') == 1, svg[-2000:]), 0, "as required"
    cells = svg.count("Z")  # every cell closes a subpath; nothing else in the file holds the letter
    yield "main-svg-one-cell-per-function", verdict(cells == 8 + 9, str(cells)), 0, "as required"
    # No address of the inventory enters the published outputs.
    addresses = [START + 32 * k for k in range(5)] + [0x80011000, 0x80011010, 0x80011020, 0x801B0000, 0x80010040, 0x80020000]
    leaked = [f"{a:x}" for a in addresses if f"{a:x}" in svg or f"{a:x}" in json.dumps(data)]
    yield "main-no-address-in-outputs", verdict(not leaked, str(leaked)), 0, "as required"
    # The page: the headings, a row per block, a total row, and no address either.
    html = root / "main.html"
    proc = tool("render", directory, "--config", root / "main.toml", "--svg", root / "main-page.svg", "--json", root / "main-page.json", "--html", html, "--date", "2026-01-01")
    text = html.read_text() if html.exists() else ""
    yield "page-written", proc, 0, "resident: 4/8 functions exact"
    for item in [
        "<h2>Overall: 42.9% of distinct functions (6/14)</h2>", "<tr><td>distinct functions</td><td>6</td><td>14</td><td>42.9</td></tr>",
        "<tr><td>function placements</td><td>7</td><td>17</td><td>41.2</td></tr>", "<tr><td>code bytes</td><td>272</td><td>560</td><td>48.6</td></tr>",
        "<h2>Resident executable: 50.0% (4/8)</h2>", "<h2>Overlay modules: 33.3% (3/9)</h2>",
        "<tr><td>game code</td><td>3</td><td>5</td><td>60.0</td></tr>",
        "<tr><td>slot 0x13, CONT00X.PAC</td><td>1</td><td>2</td><td>50.0</td><td>slot13, second link</td></tr>",
        "<tr><td>slot 0x4, PL00.PAC</td><td>0</td><td>3</td><td>0.0</td><td>-</td></tr>",
        "<tr><th>Total</th><th>3</th><th>9</th><th>33.3</th><th></th></tr>", 'src="main-page.svg"', 'href="main-page.json"', "drawn on 2026-01-01",
    ]:
        yield f"page-has {item[:40]!r}", verdict(item in text, text[:3000]), 0, "as required"
    yield "page-no-address", verdict(not [a for a in addresses if f"{a:x}" in text], text[:3000]), 0, "as required"
    # The page links the picture and the counts wherever they were written, relative to itself.
    deep = root / "pages" / "site"
    proc = tool("render", directory, "--config", root / "main.toml", "--svg", root / "pages" / "img" / "map.svg", "--json", deep / "counts.json",
                "--html", deep / "index.html", "--date", "2026-01-01")
    text = (deep / "index.html").read_text() if (deep / "index.html").exists() else ""
    links = re.findall(r'(?:src|href)="([^"]+)"', text)
    local = [link for link in links if not link.startswith("http")]
    yield "page-links-relative", verdict(
        proc.returncode == 0 and sorted(local) == ["../img/map.svg", "counts.json"] and all((deep / link).is_file() for link in local),
        f"{proc.stderr}{local}",
    ), 0, "as required"
    # Without contents.tsv a content of a slot with one content is named by the slot, and the title names its first archive.
    bare = inventory(root, "main-bare", game, library, modules)
    proc, data, svg = render(root, "main-bare", bare, config)
    labels = {b["key"]: b["label"] for b in data["panels"]["modules"]["blocks"]}
    yield "labels-without-contents", verdict(
        proc.returncode == 0 and labels == {"0x4/PL00.PAC": "PL00", "0x4/PL01.PAC": "PL01", "0x12/CONT00.PAC": "0x12", "0x13/CONT00X.PAC": "0x13"}
        and "<title>slot 0x12, CONT00.PAC: 1/2 (50.0%)</title>" in svg,
        json.dumps(labels),
    ), 0, "as required"
    # Without the table the number of archives is not known, and the JSON says so rather than guessing one.
    yield "archives-unknown-without-contents", verdict(
        all(b["archives"] is None for b in data["panels"]["modules"]["blocks"]), json.dumps(data["panels"]["modules"]["blocks"])
    ), 0, "as required"
    # contents.tsv and modules.tsv must name the same contents.
    drift = inventory(root, "main-drift", game, library, modules, contents.replace(contents_row("CONT00X.PAC", 0x13), ""))
    proc, _, _ = render(root, "main-drift", drift, config)
    yield "contents-drift", proc, 1, "error: modules.tsv and contents.tsv disagree about the contents, run `sweep` again: 0x13/CONT00X.PAC\n"
    bad = inventory(root, "main-bad-contents", game, library, modules, contents.replace("CONT00.PAC\t0x12\t5\t", "CONT00.PAC\t0x12\t4\t"))
    proc, _, _ = render(root, "main-bad-contents", bad, config)
    yield "contents-count-mismatch", proc, 1, "the row of CONT00.PAC slot 0x12 does not list its archives as it counts them\n"
    # The same inventory with a wider configuration: an image whose archive is not the content's first archive.
    proc, _, _ = render(root, "main-wrong-archive", directory, config + image("pl07", 0x4, "PL07.PAC", 0x801B0000))
    yield "image-fits-no-content", proc, 1, "image 'pl07' (slot 0x4, PL07.PAC) fits no content of the inventory"
    proc, _, _ = render(root, "main-two-images", directory, config + image("slot12b", 0x12, "CONT00.PAC", 0x80010000))
    yield "two-images-one-content", proc, 1, "content 0x12/CONT00.PAC: several images of the build fit it: slot12, slot12b"
    # A content of a slot with one content is matched by slot alone, whatever archive the image names.
    other = config.replace('archive = "../x/CONT00.PAC"', 'archive = "../x/DEMO07.PAC"')
    proc, data, _ = render(root, "main-other-archive", directory, other)
    yield "single-content-slot-by-slot", proc, 0, "modules: 3/9 function placements exact"


def boundary_cases(root: Path):
    """Where the sweep's boundary and the build's differ."""
    # The table before the first function of a module, owned as data: exact, no overrun.
    modules = module_row("PL09.PAC", 0, 0x80075E00, 80) + module_row("PL09.PAC", 0, 0x80075E50, 64)
    directory = inventory(root, "table", modules=modules)
    config = image("slot00", 0, "PL09.PAC", 0x80075E00) + unit("t", [(0x80075E10, 64), (0x80075E50, 64)], image="slot00", rodata=(0x80075E00, 16))
    proc, data, _ = render(root, "table", directory, config)
    yield "table-exact", proc, 0, "modules: 2/2 function placements exact (100.0%), 2 from C, 0 from assembly; 128/144 code bytes (88.9%); the build declares 2 functions, 128 bytes\n"
    yield "table-no-overrun", verdict("overrun" not in proc.stdout and data["panels"]["modules"]["overrun_functions"] == 0, proc.stdout), 0, "as required"
    # The program entry routine: the sweep takes in the table after it. Exact, with an overrun.
    directory = inventory(root, "entry", game=game_row(START, 188))
    proc, data, svg = render(root, "entry", directory, unit("e", [(START, 172)], kind="asm"))
    yield "entry-exact", proc, 0, "resident: 1/1 functions exact (100.0%), 0 from C, 1 from assembly; 172/188 code bytes (91.5%); the build declares 1 functions, 172 bytes\n"
    yield "entry-overrun-line", proc, 0, f"  sweep overrun in game: the function at {START:#x} is 188 bytes to the sweep, 16 of them owned by no unit\n"
    yield "entry-json", verdict((data["panels"]["resident"]["overrun_functions"], data["panels"]["resident"]["overrun_bytes"]) == (1, 16), json.dumps(data)), 0, "as required"
    yield "entry-legend", verdict("owned in part" not in svg and "exact from assembly" in svg, ""), 0, "as required"
    # Instruction-shaped data before a function that nothing calls: the function runs to the end. Exact, overrun at the head.
    directory = inventory(root, "head", game=game_row(START, 100))
    proc, data, _ = render(root, "head", directory, unit("h", [(START + 32, 68)]))
    yield "head-overrun", proc, 0, f"  sweep overrun in game: the function at {START:#x} is 100 bytes to the sweep, 32 of them owned by no unit\n"
    yield "head-exact", proc, 0, "resident: 1/1 functions exact (100.0%), 1 from C"
    # Data at the head, a gap, then a function to the end: the run to the end holds a function.
    proc, data, _ = render(root, "head-data", directory, unit("hd", [(START + 60, 40)], rodata=(START, 16)))
    yield "head-data-exact", proc, 0, "resident: 1/1 functions exact (100.0%), 1 from C, 0 from assembly; 40/100 code bytes (40.0%)"
    yield "head-data-overrun", proc, 0, "44 of them owned by no unit"
    # A function floating inside, owned bytes on neither edge: owned in part, not exact.
    proc, data, svg = render(root, "floating", directory, unit("fl", [(START + 32, 40)]))
    yield "floating-partial", proc, 0, "resident: 0/1 functions exact (0.0%), 0 from C, 0 from assembly; 40/100 code bytes (40.0%); the build declares 1 functions, 40 bytes; owned in part: 1\n"
    yield "floating-legend", verdict("owned in part" in svg and f'fill="{coveragemap.COLORS[coveragemap.PARTIAL]}"' in svg, svg[-1500:]), 0, "as required"
    yield "floating-json", verdict(data["panels"]["resident"]["partial"] == 1 and data["panels"]["resident"]["blocks"][0]["partial"] == 1, json.dumps(data)), 0, "as required"
    # Only data at the head and nothing else: no function in the run, owned in part.
    directory2 = inventory(root, "data-head", game=game_row(START, 100) + game_row(0x80030000, 8))
    proc, _, _ = render(root, "data-head", directory2, unit("dh", [(0x80030000, 8)], rodata=(START, 16)))
    yield "data-head-partial", proc, 0, "resident: 1/2 functions exact (50.0%), 1 from C, 0 from assembly; 8/108 code bytes (7.4%); the build declares 1 functions, 8 bytes; owned in part: 1\n"
    # A swept function owned as data only: set aside, counted apart, out of the totals and the bytes.
    directory = inventory(root, "data-only", game=game_row(START, 32) + game_row(START + 32, 32))
    proc, data, _ = render(root, "data-only", directory, unit("d", [(START + 32, 32)], rodata=(START, 32)))
    yield "data-only-line", proc, 0, "resident: 1/1 functions exact (100.0%), 1 from C, 0 from assembly; 32/32 code bytes (100.0%); the build declares 1 functions, 32 bytes; swept as code but owned as data: 1\n"
    yield "data-only-json", verdict(data["panels"]["resident"]["swept_as_data"] == 1 and data["panels"]["resident"]["total"] == 1, json.dumps(data)), 0, "as required"
    # One declared function over two swept functions (a call into its middle): both exact.
    directory = inventory(root, "split", game=game_row(START, 40) + game_row(START + 40, 24))
    proc, _, _ = render(root, "split", directory, unit("s", [(START, 64)]))
    yield "split-both-exact", proc, 0, "resident: 2/2 functions exact (100.0%), 2 from C, 0 from assembly; 64/64 code bytes (100.0%); the build declares 1 functions, 64 bytes\n"
    # Two declared functions inside one swept function: exact.
    directory = inventory(root, "merged", game=game_row(START, 64))
    proc, _, _ = render(root, "merged", directory, unit("m", [(START, 24), (START + 24, 40)]))
    yield "merged-exact", proc, 0, "resident: 1/1 functions exact (100.0%), 1 from C, 0 from assembly; 64/64 code bytes (100.0%); the build declares 2 functions, 64 bytes\n"
    # Second link: the functions of the base image count at the shifted addresses, and a stale base is caught there too.
    modules = module_row("A.PAC", 0x16, 0x80070000, 16) + module_row("B.PAC", 0x17, 0x80080000, 16) + module_row("B.PAC", 0x17, 0x80080010, 16)
    directory = inventory(root, "like", modules=modules)
    config = image("s16", 0x16, "A.PAC", 0x80070000) + image("s17", 0x17, "B.PAC", 0x80080000, like="s16") + unit("u16", [(0x80070000, 16)], image="s16")
    proc, data, _ = render(root, "like", directory, config)
    yield "like-shifted", proc, 0, "modules: 2/3 function placements exact (66.7%), 2 from C, 0 from assembly; 32/48 code bytes (66.7%); the build declares 2 functions, 32 bytes\n"
    yield "like-second-links", verdict(data["panels"]["modules"]["second_links"] == 1, json.dumps(data)), 0, "as required"
    # The second link's placements share their forms with the base: two forms, one of them exact.
    yield "like-distinct", proc, 0, "overall: 1/2 distinct functions exact (50.0%); 2/3 placements (66.7%); 32/48 code bytes (66.7%)\n"


def refusal_cases(root: Path):
    directory = inventory(root, "refuse", game=game_row(START, 32), modules=module_row("A.PAC", 1, 0x80020000, 32))
    proc, _, _ = render(root, "stale", directory, unit("x", [(START, 32), (START + 0x1000, 32)]))
    yield "stale-inventory", proc, 1, f"error: image 'resident' declares 1 function(s) that touch no swept function; the inventory does not represent them, run `sweep` again: {START + 0x1000:#x}\n"
    # The limit of that guard, recorded: a row stale within its range passes. A 16-byte function over an 8-byte row
    # is exact, 8 of 8 bytes, while the declared bytes say 16. The guard finds unrepresented functions, nothing more.
    short_row = inventory(root, "stale-within", game=game_row(START, 8))
    proc, data, _ = render(root, "stale-within", short_row, unit("w", [(START, 16)]))
    yield "stale-within-a-row-passes", proc, 0, "resident: 1/1 functions exact (100.0%), 1 from C, 0 from assembly; 8/8 code bytes (100.0%); the build declares 1 functions, 16 bytes\n"
    yield "stale-within-a-row-shows-in-declared-bytes", verdict(
        (data["panels"]["resident"]["bytes_exact"], data["panels"]["resident"]["declared_bytes"]) == (8, 16), json.dumps(data["panels"]["resident"])
    ), 0, "as required"
    proc, _, _ = render(root, "unknown-image", directory, unit("x", [(START, 32)], image="nowhere"))
    yield "unknown-image", proc, 1, "error: unit 'x' names an image 'nowhere' that the configuration does not declare\n"
    proc, _, _ = render(root, "like-missing", directory, image("s1", 1, "A.PAC", 0x80020000, like="gone"))
    yield "like-of-undeclared", proc, 1, "error: image 's1' is like 'gone', which the configuration does not declare\n"
    config = image("s0", 2, "Z.PAC", 0x80030000) + image("s1", 1, "A.PAC", 0x80020000, like="s0") + unit("own", [(0x80020000, 32)], image="s1")
    proc, _, _ = render(root, "like-own-units", directory, config)
    yield "like-with-own-units", proc, 1, "error: image 's1' is like 's0' and has units of its own\n"
    proc, _, _ = render(root, "overlap", directory, unit("p", [(START, 32)]) + unit("q", [(START + 16, 16)]))
    yield "overlap", proc, 1, f"error: image 'resident': owned ranges overlap at {START + 16:#x}\n"
    proc, _, _ = render(root, "unused-image", directory, image("s9", 9, "Q.PAC", 0x80090000))
    yield "image-of-no-content", proc, 1, "error: image 's9' (slot 0x9, Q.PAC) fits no content of the inventory\n"
    empty = root / "empty"
    empty.mkdir()
    proc = tool("render", empty, "--config", root / "stale.toml", "--svg", root / "e.svg", "--json", root / "e.json")
    yield "missing-tsv", proc, 1, "game.tsv is missing; run `coveragemap.py sweep` first\n"
    short = inventory(root, "short", game="80010000\t32\n")
    proc, _, _ = render(root, "short", short, "")
    yield "short-row", proc, 1, "game.tsv:1: expected at least 3 columns, found 2\n"
    short = inventory(root, "short-module", modules="A.PAC\t0x1\t80020000\t32\n")
    proc, _, _ = render(root, "short-module", short, "")
    yield "short-module-row", proc, 1, "modules.tsv:1: expected at least 5 columns, found 4\n"
    proc = tool("render", directory, "--config", root / "nothing.toml", "--svg", root / "n.svg", "--json", root / "n.json")
    yield "no-config", proc, 1, "error: cannot read"
    proc = tool("sweep", root / "no.exe", root / "nopac", "--out", root / "sweep-fail", "--config", root / "stale.toml",
                "--families", root / "nothing.toml", "--symbols", root / "nothing.ld", "--pointers", f"{START:#x}")
    yield "sweep-failing-tool", proc, 1, "error: families.py exited with"


def layout_cases(root: Path):
    """The treemap and the grid, checked in process: counts, bounds, areas, no overlaps."""
    rng = random.Random(7)
    for n, (w, h) in [(1, (100, 50)), (2, (50, 100)), (5, (380, 300)), (40, (610, 300)), (77, (610, 300)), (12, (30, 400))]:
        values = sorted((rng.random() * 100 + 1 for _ in range(n)), reverse=True)
        rects = coveragemap.squarify(values, 10, 20, w, h)
        total = sum(values)
        ok = len(rects) == n
        for value, (x, y, rw, rh) in zip(values, rects):
            ok &= x >= 10 - 1e-6 and y >= 20 - 1e-6 and x + rw <= 10 + w + 1e-6 and y + rh <= 20 + h + 1e-6
            ok &= abs(rw * rh - value * w * h / total) < 1e-6
        for i, a in enumerate(rects):
            for b in rects[i + 1:]:
                overlap = max(0.0, min(a[0] + a[2], b[0] + b[2]) - max(a[0], b[0])) * max(0.0, min(a[1] + a[3], b[1] + b[3]) - max(a[1], b[1]))
                ok &= overlap < 1e-6
        ok &= abs(sum(r[2] * r[3] for r in rects) - w * h) < 1e-6
        yield f"squarify-{n}-in-{w}x{h}", verdict(ok, str(rects)), 0, "as required"
    yield "squarify-empty", verdict(coveragemap.squarify([], 0, 0, 10, 10) == [], ""), 0, "as required"
    for n, (w, h) in [(1, (10, 10)), (2, (20, 10)), (3, (10, 30)), (7, (50, 20)), (10, (33, 33)), (97, (120, 70)), (1436, (300, 220))]:
        cells = coveragemap.grid(n, 5, 6, w, h)
        ok = len(cells) == n and abs(sum(c[2] * c[3] for c in cells) - w * h) < 1e-6
        ok &= all(c[0] >= 5 - 1e-6 and c[1] >= 6 - 1e-6 and c[0] + c[2] <= 5 + w + 1e-6 and c[1] + c[3] <= 6 + h + 1e-6 for c in cells)
        rows = sorted({round(c[1], 6) for c in cells})
        counts = [sum(1 for c in cells if round(c[1], 6) == r) for r in rows]
        ok &= max(counts) - min(counts) <= 1 and counts == sorted(counts, reverse=True)
        yield f"grid-{n}-in-{w}x{h}", verdict(ok, str(cells[:5])), 0, "as required"
    yield "grid-none", verdict(coveragemap.grid(0, 0, 0, 10, 10) == [] and coveragemap.grid(3, 0, 0, 0, 10) == [], ""), 0, "as required"
    labels = {
        ("0x2b", 20, 30): 0, ("Game", 60, 30): 12, ("system", 44, 30): 10, ("system", 39, 30): 9, ("system", 30, 30): 0,
        ("x", 100, 10): 0, ("PL00", 44, 21): 12, ("PL00", 40, 21): 10, ("PL00", 40, 17): 9, ("CDEMO", 37, 30): 0,
    }
    got = {key: coveragemap.fit_label(*key) for key in labels}
    yield "fit-label", verdict(got == labels, str(got)), 0, "as required"
    names = {
        ("DEMO.PAC",): "DEMO", ("SELECTA.PAC",): "SELECTA", ("PL09.PAC",): "PL09", ("PL0EX.PAC",): "PL0EX", ("A1.PAC",): "A1",
        ("PL11.PAC", "PL13.PAC"): "PL11+PL13", ("PL11X.PAC", "PL13X.PAC"): "PL11X+PL13X",
        tuple(f"CDEMO{n:02X}.PAC" for n in range(21)): "CDEMO",
        tuple(f"END{n:02X}.PAC" for n in range(21)): "END",
        tuple(f"CONT{n:02X}{x}.PAC" for n in range(21) for x in ("", "X")): "CONT",
        tuple(f"{f}{n:02X}.PAC" for f in ("BOSS", "GDEMO", "RDM") for n in range(21)): "BOSS+2",
        tuple(f"PL{n:02X}X.PAC" for n in range(21)) + ("PL12.PAC", "PL13.PAC", "PL14.PAC"): "PL",
        ("STAGE0E.PAC",): "STAGE0E",
    }
    got = {key: coveragemap.content_label(list(key)) for key in names}
    yield "content-label", verdict(got == names, str({k[:2]: v for k, v in got.items() if names[k] != v})), 0, "as required"
    titles = {
        (0xF, ("DEMO.PAC",)): "slot 0xf, DEMO.PAC",
        (0x16, ("PL11.PAC", "PL13.PAC")): "slot 0x16, PL11.PAC, PL13.PAC",
        (0x28, tuple(f"END{n:02X}.PAC" for n in range(21))): "slot 0x28, END00.PAC and 20 more archives",
    }
    got_titles = {key: coveragemap.content_title(key[0], list(key[1])) for key in titles}
    yield "content-title", verdict(got_titles == titles, str(got_titles)), 0, "as required"
    pieces = [(0, 16, "data"), (16, 80, "c")]
    yield "anchored-full-from-start", verdict(coveragemap.anchored(pieces, 0, 100) == (pieces, 20), ""), 0, "as required"
    yield "anchored-to-end", verdict(coveragemap.anchored([(60, 100, "c")], 0, 100) == ([(60, 100, "c")], 60), ""), 0, "as required"
    yield "anchored-data-only-head", verdict(coveragemap.anchored([(0, 16, "data")], 0, 100) == ([], 0), ""), 0, "as required"
    yield "anchored-floating", verdict(coveragemap.anchored([(20, 60, "asm")], 0, 100) == ([], 0), ""), 0, "as required"
    yield "anchored-gap-then-function", verdict(coveragemap.anchored([(0, 8, "c"), (16, 40, "c")], 0, 100) == ([(0, 8, "c")], 68), ""), 0, "as required"


def sweep_cases(root: Path):
    """`sweep` on a fake executable and archive, then `render` on what it wrote."""
    XB = 0x80200000
    plain = [OPEN, ONE, RETURN, CLOSE]
    stub = [0x240A00A0, 0x01400008, 0x24090005]  # li t2,0xa0; jr t2; li t1,5
    code = plain + plain + stub
    library, end = START + 32, START + 4 * len(code)
    exe, pointers = make_program(START, code, [[XB], [0x80300000]])  # the loader's block holds at least two tables
    exe_path = root / "sweep.exe"
    exe_path.write_bytes(exe)
    pac = root / "pac"
    pac.mkdir()
    (pac / "A1.PAC").write_bytes(make_archive([(0, struct.pack("<32I", *(plain * 8)))]))  # eight returns: what pac.py takes as code
    config = root / "sweep.toml"
    config.write_text(f"[overlays]\ntable_pointers = {pointers:#x}\n\n" + unit("g", [(START, 16)]) + image("m0", 0, "A1.PAC", XB) + unit("m", [(XB, 16)], image="m0"))
    families = root / "sweep-families.toml"
    families.write_text('[bios]\n"a0:05" = "alpha"\n')
    symbols = root / "sweep.ld"
    symbols.write_text("")
    out = root / "sweep-out"
    proc = tool("sweep", exe_path, pac, "--out", out, "--config", config, "--families", families, "--symbols", symbols,
                "--library", f"{library:#x}", "--end", f"{end:#x}")
    yield "sweep-runs", proc, 0, f"inventory written to {out}\n"
    yield "sweep-swept-the-executable", proc, 0, f"swept {START:#x} to {end:#x}: 3 functions, 2 game and 1 library"
    rows = {name: (out / name).read_text() if (out / name).exists() else None for name in ("game.tsv", "library.tsv", "modules.tsv")}
    yield "sweep-game-rows", verdict(rows["game.tsv"] is not None and rows["game.tsv"].count("\n") == 2 and rows["game.tsv"].startswith(f"{START:08x}\t16\t"), str(rows)), 0, "as required"
    yield "sweep-library-rows", verdict(rows["library.tsv"] is not None and rows["library.tsv"].count("\n") == 1 and "\talpha\t" in rows["library.tsv"], str(rows)), 0, "as required"
    yield "sweep-module-rows", verdict(rows["modules.tsv"] is not None and rows["modules.tsv"].count("\n") == 8 and rows["modules.tsv"].startswith(f"A1.PAC\t0x0\t{XB:08x}\t16\t"), str(rows)), 0, "as required"
    contents = (out / "contents.tsv").read_text() if (out / "contents.tsv").exists() else None
    yield "sweep-contents-rows", verdict(contents == "A1.PAC\t0x0\t1\tA1.PAC\n", repr(contents)), 0, "as required"
    proc = tool("render", out, "--config", config, "--svg", root / "sweep.svg", "--json", root / "sweep.json", "--date", "2026-01-01")
    yield "sweep-then-render-resident", proc, 0, "resident: 1/3 functions exact (33.3%), 1 from C, 0 from assembly; 16/44 code bytes (36.4%); the build declares 1 functions, 16 bytes\n"
    yield "sweep-then-render-modules", proc, 0, "modules: 1/8 function placements exact (12.5%), 1 from C, 0 from assembly; 16/128 code bytes (12.5%); the build declares 1 functions, 16 bytes\n"
    sweep_json = json.loads((root / "sweep.json").read_text()) if (root / "sweep.json").exists() else {}
    yield "sweep-then-render-label", verdict(
        [(b["label"], b["archives"]) for b in sweep_json.get("panels", {}).get("modules", {}).get("blocks", [])] == [("A1", 1)], json.dumps(sweep_json)[:500]
    ), 0, "as required"
    proc = tool("sweep", exe_path, pac, "--out", root / "sweep-nop", "--config", families, "--families", families, "--symbols", symbols)
    yield "sweep-needs-pointers", proc, 1, "error: no --pointers given and the configuration has no [overlays] table_pointers\n"


def cases(root: Path):
    yield from main_cases(root)
    yield from boundary_cases(root)
    yield from refusal_cases(root)
    yield from layout_cases(root)
    yield from sweep_cases(root)


def main() -> int:
    failed = 0
    with tempfile.TemporaryDirectory() as tmp:
        for name, proc, want_status, want_text in cases(Path(tmp)):
            output = proc.stdout + proc.stderr
            ok = proc.returncode == want_status and want_text in output
            print(f"{'ok  ' if ok else 'FAIL'} {name}: exit {proc.returncode}, wanted {want_status} with {want_text!r}")
            if not ok:
                print(output)
                failed += 1
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
