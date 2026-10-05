#!/usr/bin/env python3
"""Read the chunk archives (`*.PAC`) used on the disc.

Layout, established by parsing and checked against every file's size:
a little-endian u32 chunk count at offset 0, then one 32-byte entry per chunk
from offset 0x20 holding a u32 type, a u32 size in bytes and a copy of the
chunk's first four bytes. Chunk data follows from offset 0x800 in entry order,
each chunk padded to a 2,048-byte boundary.

The type word is two 16-bit fields. The resident loader reads the upper half
as the number of a destination table and the lower half as a slot in it (see
ps1/docs/overlays.md for the function that does so). They are reported here
as `table` and `slot`.

`list` prints the table. `extract` writes one chunk. `scan` inventories a
directory and estimates, for chunks that look like MIPS code, the address the
chunk is linked for. That estimate is static: it finds the base under which
the most absolute call targets land on function prologues.

`loadmap` reads the destination tables from the resident executable and
compares them with those estimates. `sides` compares the block of a
second-side archive with its first-side twin word by word. Nothing here was
observed in a running game.
"""

from __future__ import annotations

import argparse
import collections
import hashlib
import json
import re
import struct
import sys
from pathlib import Path

SECTOR = 2048
ENTRY_OFFSET = 0x20
ENTRY_SIZE = 32
JR_RA = 0x03E00008
MIN_RETURNS = 8  # fewer `jr ra` words than this is not treated as code


class FormatError(Exception):
    pass


def chunk_table(data: bytes) -> list[dict]:
    """Return the chunks of an archive. Raises FormatError if the layout does not hold."""
    if len(data) < SECTOR:
        raise FormatError("shorter than one sector")
    (count,) = struct.unpack_from("<I", data, 0)
    if count == 0 or ENTRY_OFFSET + count * ENTRY_SIZE > SECTOR:
        raise FormatError(f"implausible chunk count {count}")
    chunks, offset = [], SECTOR
    for index in range(count):
        kind, size, first = struct.unpack_from("<III", data, ENTRY_OFFSET + index * ENTRY_SIZE)
        if offset + size > len(data):
            raise FormatError(f"chunk {index} runs past the end of the file")
        if size >= 4 and struct.unpack_from("<I", data, offset)[0] != first:
            raise FormatError(f"chunk {index}: first word does not match its table entry")
        chunks.append(
            {"index": index, "type": kind, "table": kind >> 16, "slot": kind & 0xFFFF, "size": size, "offset": offset}
        )
        offset += -(-size // SECTOR) * SECTOR
    if offset != len(data):
        raise FormatError(f"chunks end at {offset:#x}, file size is {len(data):#x}")
    return chunks


def code_estimate(chunk: bytes) -> dict | None:
    """Estimate whether a chunk holds MIPS code and where it is linked."""
    words = struct.unpack_from(f"<{len(chunk) // 4}I", chunk)
    returns = [i * 4 for i, w in enumerate(words) if w == JR_RA]
    if len(returns) < MIN_RETURNS:
        return None
    prologues = [i * 4 for i, w in enumerate(words) if w >> 16 == 0x27BD and w & 0x8000]
    targets = {((w & 0x03FFFFFF) << 2) | 0x80000000 for w in words if w >> 26 == 3}
    votes = collections.Counter(t - p for t in targets for p in prologues)
    result = {"returns": len(returns), "first_return": returns[0], "last_return": returns[-1]}
    if votes:
        base, count = votes.most_common(1)[0]
        result.update({"base": base & 0xFFFFFFFF, "votes": count})
    return result


def cmd_list(args) -> int:
    status = 0
    for path in args.files:
        try:
            chunks = chunk_table(Path(path).read_bytes())
        except (OSError, FormatError) as exc:
            print(f"{path}: {exc}")
            status = 1
            continue
        print(f"{path}: {len(chunks)} chunks")
        for c in chunks:
            print(
                f"  {c['index']:2} type {c['type']:#8x} (table {c['table']}, slot {c['slot']:#x})"
                f" size {c['size']:#8x} at {c['offset']:#8x}"
            )
    return status


def cmd_extract(args) -> int:
    data = Path(args.file).read_bytes()
    try:
        chunks = chunk_table(data)
    except FormatError as exc:
        print(f"{args.file}: {exc}")
        return 1
    if not 0 <= args.index < len(chunks):
        print(f"no chunk {args.index}")
        return 1
    c = chunks[args.index]
    Path(args.out).write_bytes(data[c["offset"] : c["offset"] + c["size"]])
    print(f"wrote {c['size']} bytes, type {c['type']:#x}")
    return 0


def cmd_scan(args) -> int:
    files, bad = [], 0
    by_type: dict[int, dict] = {}
    for path in sorted(Path(args.directory).rglob("*.PAC")):
        data = path.read_bytes()
        try:
            chunks = chunk_table(data)
        except FormatError as exc:
            print(f"{path.name}: {exc}")
            bad += 1
            continue
        for c in chunks:
            code = code_estimate(data[c["offset"] : c["offset"] + c["size"]])
            if code:
                c["code"] = code
            entry = by_type.setdefault(c["type"], {"chunks": 0, "code_chunks": 0, "bases": collections.Counter()})
            entry["chunks"] += 1
            if code:
                entry["code_chunks"] += 1
                if "base" in code:
                    entry["bases"][code["base"]] += 1
        files.append({"path": path.name, "size": len(data), "chunks": chunks})
    print(f"{len(files)} archives parsed, {bad} rejected")
    print("type       chunks  with code  estimated link addresses (count)")
    for kind in sorted(by_type):
        entry = by_type[kind]
        bases = ", ".join(f"{b:#x} ({n})" for b, n in entry["bases"].most_common(4))
        print(f"{kind:#10x} {entry['chunks']:6} {entry['code_chunks']:10}  {bases}")
    if args.out:
        Path(args.out).parent.mkdir(parents=True, exist_ok=True)
        Path(args.out).write_text(json.dumps({"files": files}, indent=1) + "\n")
    return 1 if bad or not files else 0


EXE_MAGIC = b"PS-X EXE"
EXE_HEADER = 0x800
MAX_TABLES = 16


class Image:
    """The loaded part of a PS-X executable: bytes at their link addresses."""

    def __init__(self, data: bytes):
        if data[:8] != EXE_MAGIC or len(data) < EXE_HEADER:
            raise FormatError("not a PS-X executable")
        self.start, size = struct.unpack_from("<II", data, 0x18)
        self.payload = data[EXE_HEADER : EXE_HEADER + size]
        if len(self.payload) != size:
            raise FormatError("executable is shorter than its header says")
        self.end = self.start + size

    def word(self, address: int) -> int:
        if not (self.start <= address <= self.end - 4) or address % 4:
            raise FormatError(f"address {address:#x} is not a word inside the image")
        return struct.unpack_from("<I", self.payload, address - self.start)[0]


def destination_tables(image: Image, pointers: int) -> list[list[int] | None]:
    """The destination tables that the loader selects by an entry's table number.

    `pointers` is the address of the block of table addresses. The block ends
    at the first word that is not an address inside the image above the
    previous one. A table is read up to the start of the next one, which
    bounds its length from above. The last table has no such bound and is
    returned as None.
    """
    starts: list[int] = []
    for n in range(MAX_TABLES):
        try:
            value = image.word(pointers + 4 * n)
        except FormatError:
            break
        if not (image.start <= value < image.end) or value % 4 or (starts and value <= starts[-1]):
            break
        starts.append(value)
    if len(starts) < 2:
        raise FormatError(f"no block of table addresses at {pointers:#x}")
    tables: list[list[int] | None] = []
    for begin, end in zip(starts, starts[1:]):
        tables.append([image.word(a) for a in range(begin, end, 4)])
    tables.append(None)
    return tables


def read_archives(directory: str) -> tuple[list[tuple[str, bytes, list[dict]]], int]:
    """Every parseable archive under a directory, and the number rejected."""
    archives, bad = [], 0
    for path in sorted(Path(directory).rglob("*.PAC")):
        data = path.read_bytes()
        try:
            archives.append((path.name, data, chunk_table(data)))
        except FormatError as exc:
            print(f"{path.name}: {exc}")
            bad += 1
    return archives, bad


def load_tables(args) -> tuple[Image, list[list[int] | None]]:
    image = Image(Path(args.executable).read_bytes())
    return image, destination_tables(image, args.pointers)


def cmd_loadmap(args) -> int:
    try:
        image, tables = load_tables(args)
    except (OSError, FormatError) as exc:
        print(f"{args.executable}: {exc}")
        return 1
    archives, bad = read_archives(args.directory)
    if not archives:
        print("no archive parsed")
        return 1
    print(f"image {image.start:#x} to {image.end:#x}; {len(tables)} destination tables at {args.pointers:#x}:")
    for number, table in enumerate(tables):
        print(f"  table {number}: " + ("length not bounded" if table is None else f"at most {len(table)} entries"))
    pairs: dict[tuple[int, int], dict] = {}
    bodies: dict[tuple[int, int], list[bytes]] = {}
    for name, data, chunks in archives:
        for c in chunks:
            body = data[c["offset"] : c["offset"] + c["size"]]
            bodies.setdefault((c["table"], c["slot"]), []).append(body)
            entry = pairs.setdefault(
                (c["table"], c["slot"]), {"chunks": 0, "largest": 0, "contents": set(), "code": 0, "estimates": collections.Counter()}
            )
            entry["chunks"] += 1
            entry["largest"] = max(entry["largest"], c["size"])
            entry["contents"].add(hashlib.sha256(body).digest())
            code = code_estimate(body)
            if code:
                entry["code"] += 1
                entry["estimates"][code.get("base")] += 1
    print(f"{len(archives)} archives parsed, {bad} rejected, {sum(e['chunks'] for e in pairs.values())} chunks")
    print("table   slot  destination  chunks  distinct  largest  with code  verdict")
    agree = differ = unmapped = 0
    rows = []
    for (number, slot), entry in sorted(pairs.items()):
        table = tables[number] if number < len(tables) else None
        destination = table[slot] if table is not None and slot < len(table) else None
        verdict = ""
        if destination is None:
            unmapped += entry["chunks"]
            verdict = "no table entry"
        if entry["code"]:
            if destination is not None and set(entry["estimates"]) == {destination}:
                verdict = "estimate agrees"
                agree += 1
            else:
                found = ", ".join("none" if b is None else f"{b:#x}" for b in sorted(entry["estimates"], key=str))
                verdict = f"ESTIMATE DIFFERS: {found}"
                differ += 1
        where = f"{'-':>11}" if destination is None else f"{destination:#011x}".replace("0x0", " 0x", 1)
        print(
            f"{number:5} {slot:#6x}  {where}  {entry['chunks']:6}  {len(entry['contents']):8}  {entry['largest']:#7x}"
            f"  {entry['code']:9}  {verdict}"
        )
        rows.append(
            {"table": number, "slot": slot, "destination": destination, "chunks": entry["chunks"],
             "distinct": len(entry["contents"]), "largest": entry["largest"], "code_chunks": entry["code"],
             "estimates": {("none" if b is None else f"{b:#x}"): n for b, n in entry["estimates"].items()}}
        )
    print(f"pairs with code: {agree} agree with the table, {differ} differ; chunks without a table entry: {unmapped}")
    if args.symbols:
        report_symbols(Path(args.symbols).read_text(), image, rows, bodies)
    if args.out:
        Path(args.out).parent.mkdir(parents=True, exist_ok=True)
        Path(args.out).write_text(json.dumps({"pointers": args.pointers, "pairs": rows}, indent=1) + "\n")
    return 1 if bad or differ else 0


SYMBOL_LINE = re.compile(r"^\s*(\w+)\s*=\s*(0x[0-9a-fA-F]+)\s*;", re.M)


def report_symbols(text: str, image: Image, rows: list[dict], bodies: dict) -> None:
    """For each assigned symbol outside the image, the table 0 chunks loaded over its address.

    Per slot: how many chunks reach the address, in how many of them a stack
    frame is opened there (`addiu sp,sp,-N`), and how many different 64-byte
    contents start there. Symbols that no chunk reaches are counted per
    64 KiB region.
    """
    print("symbols outside the image: slot: chunks reaching it / opening a frame there / distinct contents")
    unreached: dict[int, list[int]] = {}
    for name, value in sorted(SYMBOL_LINE.findall(text), key=lambda pair: int(pair[1], 16)):
        address = int(value, 16)
        if image.start <= address < image.end:
            continue
        found = []
        for row in rows:
            base = row["destination"]
            if row["table"] != 0 or base is None or not (base <= address < base + row["largest"]):
                continue
            reach = [b for b in bodies[(0, row["slot"])] if address - base + 4 <= len(b)]
            frames = sum(1 for b in reach if struct.unpack_from("<I", b, address - base)[0] & 0xFFFF8000 == 0x27BD8000)
            distinct = len({b[address - base : address - base + 64] for b in reach})
            found.append(f"slot {row['slot']:#x}: {len(reach)}/{frames}/{distinct}")
        if found:
            print(f"  {name} {address:#x}  " + "; ".join(found))
        else:
            unreached.setdefault(address >> 16, []).append(address)
    for group in unreached.values():
        print(f"  no chunk reaches {len(group)} symbol(s) from {min(group):#x} to {max(group):#x}")


def classify_shift(first: int, second: int, delta: int, low: int, high: int) -> str:
    """Name the way two differing words relate when the second block sits `delta` above the first.

    The classes say what a difference is consistent with. They do not prove
    that the word is an address.
    """
    op = first >> 26
    if op == second >> 26 and op in (2, 3) and ((second - first) & 0x03FFFFFF) == (delta >> 2) & 0x03FFFFFF:
        return "jump target moved by the distance"
    if second - first == delta and low <= first < high:
        return "word pointing into the block moved by the distance"
    if first >> 16 == second >> 16:
        step = ((second & 0xFFFF) - (first & 0xFFFF)) & 0xFFFF
        if op == 0x0F and step in (delta >> 16, (delta >> 16) + 1):
            return "upper half of an address"
        if op != 0x0F and delta & 0xFFFF and step == delta & 0xFFFF:
            return "lower half of an address"
    return "other"


def cmd_sides(args) -> int:
    try:
        image, tables = load_tables(args)
        table = tables[0]
        if table is None or max(args.first, args.second) >= len(table):
            raise FormatError("table 0 has no such slots")
    except (OSError, FormatError) as exc:
        print(f"{args.executable}: {exc}")
        return 1
    low, delta = table[args.first], table[args.second] - table[args.first]
    archives, bad = read_archives(args.directory)

    def block(chunks, data, slot):
        found = [c for c in chunks if c["table"] == 0 and c["slot"] == slot]
        return data[found[0]["offset"] : found[0]["offset"] + found[0]["size"]] if len(found) == 1 else None

    firsts = {name: block(chunks, data, args.first) for name, data, chunks in archives}
    print(f"slot {args.first:#x} at {low:#x}, slot {args.second:#x} at {low + delta:#x}, distance {delta:#x}")
    totals: collections.Counter = collections.Counter()
    compared = skipped = 0
    for name, data, chunks in archives:
        stem = Path(name).stem
        twin = stem[:-1] + ".PAC"
        second = block(chunks, data, args.second)
        if not stem.endswith("X") or second is None or firsts.get(twin) is None:
            continue
        first = firsts[twin]
        if len(first) != len(second):
            print(f"{twin} and {name}: sizes differ ({len(first):#x}, {len(second):#x}), not compared")
            skipped += 1
            continue
        counts: collections.Counter = collections.Counter()
        others = []
        for offset in range(0, len(first) - 3, 4):
            a, b = struct.unpack_from("<I", first, offset)[0], struct.unpack_from("<I", second, offset)[0]
            if a == b:
                counts["identical"] += 1
                continue
            kind = classify_shift(a, b, delta, low, low + len(first))
            counts[kind] += 1
            if kind == "other":
                others.append(f"{offset:#x}: {a:08x} {b:08x}")
        compared += 1
        totals.update(counts)
        print(f"{twin} and {name}: {counts['identical']} words identical, {sum(counts.values()) - counts['identical']} differ, {counts['other']} other")
        for line in others[: args.show]:
            print(f"    {line}")
    print(f"{compared} pairs compared, {skipped} skipped for size")
    for kind, count in totals.most_common():
        print(f"{count:9}  {kind}")
    return 1 if bad or not compared else 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("list", help="print the chunk table of archives")
    p.add_argument("files", nargs="+")
    p.set_defaults(fn=cmd_list)
    p = sub.add_parser("extract", help="write one chunk to a file")
    p.add_argument("file")
    p.add_argument("index", type=int)
    p.add_argument("out")
    p.set_defaults(fn=cmd_extract)
    p = sub.add_parser("scan", help="inventory every archive under a directory")
    p.add_argument("directory")
    p.add_argument("--out", help="write the full inventory as JSON")
    p.set_defaults(fn=cmd_scan)

    def address(text: str) -> int:
        return int(text, 0)

    for name, fn, text in (
        ("loadmap", cmd_loadmap, "compare the loader's destination tables with the code estimates"),
        ("sides", cmd_sides, "compare second-side blocks with their first-side twins"),
    ):
        p = sub.add_parser(name, help=text)
        p.add_argument("executable", help="the resident PS-X executable")
        p.add_argument("directory", help="directory holding the archives")
        p.add_argument("--pointers", type=address, required=True, help="address of the block of table addresses")
        if name == "loadmap":
            p.add_argument("--out", help="write the rows as JSON")
            p.add_argument("--symbols", help="a file of `name = 0xADDRESS;` lines: report those outside the image")
        else:
            p.add_argument("--first", type=address, default=4, help="slot of the first-side block (default 4)")
            p.add_argument("--second", type=address, default=5, help="slot of the second-side block (default 5)")
            p.add_argument("--show", type=int, default=0, help="print up to this many unexplained words per pair")
        p.set_defaults(fn=fn)
    args = parser.parse_args()
    return args.fn(args)


if __name__ == "__main__":
    sys.exit(main())
