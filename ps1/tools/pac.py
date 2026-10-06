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
second-side archive with its first-side twin word by word. `functions`
sweeps every code-bearing chunk for function boundaries with funcscan.py and
counts how many functions are distinct and which slots have them in common,
with funcscan.py's four counts that a wrong boundary can disturb.
`unlisted` sweeps the resident executable, takes the functions that an
inventory of it does not list, and counts how the executable and the modules
refer to each: see `cmd_unlisted`.
Nothing here was observed in a running game.
"""

from __future__ import annotations

import argparse
import collections
import hashlib
import json
import re
import struct
import sys
import tomllib
from pathlib import Path

import funcscan

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


SAVED = {16, 17, 18, 19, 20, 21, 22, 23, 28, 29, 30}  # s0 to s7, gp, sp, fp: a call leaves them as they were


def address_blind(words: list[int]) -> bytes:
    """The words of a function with the fields that depend on where things are linked set to zero.

    Zeroed: the target of every `j` and `jal`; the 16-bit field of every
    `lui`; and the 16-bit field of a later instruction that uses a register
    loaded by `lui` as its base, until that register is overwritten. A `jal`
    or `jalr` is taken to overwrite every register but `s0` to `s7`, `gp`,
    `sp` and `fp`, after its delay slot. The registers are followed in
    address order, not along the paths of the function. Two functions that
    agree after this may still differ in what they address; two that
    disagree may be one source with other constants. It gives an estimate
    of how much code is shared, nothing more.
    """
    out, upper, returned = [], set(), -1
    for position, word in enumerate(words):
        if position == returned:
            upper &= SAVED
        op, rs, rt = word >> 26, (word >> 21) & 0x1F, (word >> 16) & 0x1F
        writes = 0x08 <= op <= 0x0E or 0x20 <= op <= 0x26
        if op == 0x03 or (op == 0 and word & 0x3F == 0x09):
            returned = position + 2
        if op in (0x02, 0x03):
            word &= 0xFC000000
        elif op == 0x0F:
            upper.add(rt)
            word &= 0xFFFF0000
        elif (writes or 0x28 <= op <= 0x3A) and rs in upper:
            word &= 0xFFFF0000
            if writes:
                upper.discard(rt)  # the full address, a loaded value, or another register's new value
        elif writes:
            upper.discard(rt)
        elif op == 0:
            rd = (word >> 11) & 0x1F
            if rs not in upper and rt not in upper:
                upper.discard(rd)  # `addu at,at,v0` keeps the upper half in `at`
        out.append(word)
    return struct.pack(f"<{len(out)}I", *out)


def starts_function(words: list[int], index: int) -> bool:
    """Whether the words from `index` begin the way a compiled function with a stack frame can.

    Either the frame is opened there (`addiu sp,sp,-N`), or one or two pairs
    of a `lui` and a load through the same register come first and the frame
    is opened directly after them. A function without a frame, or one whose
    frame opens later in another way, is not recognised.
    """
    for _ in range(3):
        if index >= len(words):
            return False
        if words[index] & 0xFFFF8000 == 0x27BD8000:
            return True
        upper, load = words[index], words[index + 1] if index + 1 < len(words) else 0
        if upper >> 26 != 0x0F or not 0x20 <= load >> 26 <= 0x25 or (load >> 21) & 0x1F != (upper >> 16) & 0x1F:
            return False
        index += 2
    return False


def swept_chunks(archives: list, table: list[int], symbols: list[int]):
    """Every distinct content of a code-bearing chunk of table 0, once per slot, with the functions swept in it.

    Yields the first archive with that content, the slot, its destination,
    the size of the chunk in bytes, its whole words, the functions as
    (address, size), the number of symbols whose address lies in the chunk
    and how many of those were taken as a function start. A destination that
    is not a multiple of four raises FormatError.
    """
    seen: set[tuple[int, bytes]] = set()
    for name, data, chunks in archives:
        for c in chunks:
            body = data[c["offset"] : c["offset"] + c["size"]]
            key = (c["slot"], hashlib.sha256(body).digest())
            if c["table"] != 0 or c["slot"] >= len(table) or key in seen or not code_estimate(body):
                continue
            seen.add(key)
            base = table[c["slot"]]
            if base % 4:
                raise FormatError(f"slot {c['slot']:#x}: the destination {base:#x} is not a multiple of four")
            words = list(struct.unpack_from(f"<{len(body) // 4}I", body))
            # A symbol belongs to one module and its address lies in others too: see starts_function.
            inside = [a for a in symbols if base <= a < base + 4 * len(words)]
            entries = [a for a in inside if starts_function(words, (a - base) // 4)]
            found = funcscan.scan(words, base, 0, len(words), funcscan.reader(words, base), entries)
            yield name, c["slot"], base, len(body), words, found, len(inside), len(entries)


def cmd_functions(args) -> int:
    try:
        table = load_tables(args)[1][0]  # the last table is the one without a bound, and table 0 is never the last
    except (OSError, FormatError) as exc:
        print(f"{args.executable}: {exc}")
        return 1
    symbols = sorted({a for a in funcscan.read_entries(args.symbols) if a % 4 == 0})
    offered = taken = contents = 0
    archives, bad = read_archives(args.directory)
    slots: dict[int, dict] = {}
    distinct_bytes: set[bytes] = set()
    sizes: dict[bytes, int] = {}
    counts = dict.fromkeys((key for key, _ in funcscan.CHECKS), 0)
    rows = []
    try:
        for name, slot, base, length, words, found, inside, entries in swept_chunks(archives, table, symbols):
            contents += 1
            offered += inside
            taken += entries
            for check, count in funcscan.census(words, base, 0, len(words), found).items():
                counts[check] += count
            entry = slots.setdefault(slot, {"contents": [], "size": 0, "functions": 0, "bytes": 0, "exact": set()})
            entry["size"] += length
            entry["functions"] += len(found)
            entry["bytes"] += sum(size for _, size in found)
            here = set()
            for address, size in found:
                part = words[(address - base) // 4 : (address - base + size) // 4]
                exact = hashlib.sha256(struct.pack(f"<{len(part)}I", *part)).digest()
                blind = hashlib.sha256(address_blind(part)).digest()
                entry["exact"].add(exact)
                here.add(blind)
                distinct_bytes.add(exact)
                sizes[blind] = size
                rows.append(f"{name}\t{slot:#x}\t{address:08x}\t{size}\t{blind.hex()[:16]}")
            entry["contents"].append(here)
    except FormatError as exc:
        print(exc)
        return 1
    if not slots:
        print("no code-bearing chunk found")
        return 1
    blind = {slot: set().union(*entry["contents"]) for slot, entry in slots.items()}
    print(f"{len(archives)} archives parsed, {bad} rejected; code-bearing chunks with distinct contents: {contents}")
    print(
        " slot  destination  contents  content bytes  functions  function bytes"
        "  distinct by bytes  distinct address-blind  in no other slot"
    )
    for slot, entry in sorted(slots.items()):
        elsewhere = set().union(*(found for other, found in blind.items() if other != slot))
        print(
            f"{slot:#5x}   {table[slot]:#010x}  {len(entry['contents']):8}  {entry['size']:13}  {entry['functions']:9}"
            f"  {entry['bytes']:14}  {len(entry['exact']):17}  {len(blind[slot]):22}  {len(blind[slot] - elsewhere):16}"
        )
    print(
        f"all slots: {sum(e['functions'] for e in slots.values())} functions,"
        f" {sum(e['bytes'] for e in slots.values())} bytes;"
        f" distinct by bytes: {len(distinct_bytes)};"
        f" distinct address-blind: {len(sizes)} functions, {sum(sizes.values())} bytes"
    )
    print(funcscan.census_text(counts))
    for slot, entry in sorted(slots.items()):
        if len(entry["contents"]) > 1:
            held = [sum(1 for found in entry["contents"] if h in found) for h in blind[slot]]
            print(
                f"slot {slot:#x}: of its {len(held)} address-blind distinct functions, {held.count(1)} are in one of its"
                f" {len(entry['contents'])} contents only and {held.count(len(entry['contents']))} in all of them"
            )
    order = sorted(slots)
    for index, slot in enumerate(order):
        for other in order[index + 1 :]:
            common = len(blind[slot] & blind[other])
            if blind[slot] == blind[other]:
                print(f"slots {slot:#x} and {other:#x}: the same {common} address-blind distinct functions")
            elif common >= args.common:
                print(f"slots {slot:#x} and {other:#x}: {common} address-blind distinct functions in common")
    if symbols:
        print(f"symbols whose address lies in a module: {offered} cases; taken as a function start there: {taken}")
    if args.out:
        Path(args.out).parent.mkdir(parents=True, exist_ok=True)
        Path(args.out).write_text("".join(row + "\n" for row in rows))
    return 1 if bad else 0


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


PAIR_REACH = 8  # words after a `lui` in which the instruction that completes an address is looked for
REFERENCES = (
    ("called by the executable", ("call", "executable")),
    ("called by modules only", ("call", "modules")),
    ("address in a data word", ("word", "executable"), ("word", "modules")),
    ("address formed in code", ("pair", "executable"), ("pair", "modules")),
)
NO_REFERENCE = "no reference found"


def code_references(words: list[int], base: int, functions: list[tuple[int, int]], wanted: dict[int, int]) -> collections.Counter:
    """Count what the code of `functions` and the words outside them hold of the addresses in `wanted`.

    `wanted` maps the start of a function to its size. The result counts
    (start, kind) with three kinds:

    - `call`: a `jal` or a `j` to the start, inside one of `functions`. A
      jump from inside the wanted function itself is not counted.
    - `pair`: inside one of `functions`, a `lui` and the first `addiu` or
      `ori` after it that reads the register it loaded, at most PAIR_REACH
      words later and before another `lui` into that register, which
      together form the start.
    - `word`: a word outside every one of `functions` that equals the start.

    None of the three proves a reference. A data word may equal an address
    by chance, and the two halves of a pair need not belong together.
    """
    found: collections.Counter = collections.Counter()
    code = [False] * len(words)
    for address, size in functions:
        low, high = (address - base) // 4, (address - base + size) // 4
        code[low:high] = [True] * (high - low)
        for index in range(low, high):
            word = words[index]
            op = word >> 26
            here = base + 4 * index
            if op in (0x02, 0x03):
                target = here & 0xF0000000 | (word & 0x03FFFFFF) << 2
                if target in wanted and not target <= here < target + wanted[target]:
                    found[target, "call"] += 1
            elif op == 0x0F:
                register = (word >> 16) & 0x1F
                for later in words[index + 1 : min(index + 1 + PAIR_REACH, high)]:
                    if later >> 26 == 0x0F and (later >> 16) & 0x1F == register:
                        break
                    if later >> 26 in (0x09, 0x0D) and (later >> 21) & 0x1F == register:
                        half = later & 0xFFFF
                        target = ((word & 0xFFFF) << 16) + (funcscan.signed16(half) if later >> 26 == 0x09 else half)
                        if target & 0xFFFFFFFF in wanted:
                            found[target & 0xFFFFFFFF, "pair"] += 1
                        break
    for index, word in enumerate(words):
        if not code[index] and word in wanted:
            found[word, "word"] += 1
    return found


def build_functions(path: str) -> set[int]:
    """The addresses of the functions that the units of the resident image declare in a build configuration.

    A function without an integer address raises FormatError.
    """
    with open(path, "rb") as handle:
        units = tomllib.load(handle).get("unit", [])
    try:
        found = {f["address"] for unit in units if "image" not in unit for f in unit.get("functions", [])}
    except (KeyError, TypeError, AttributeError):
        found = {None}
    if not all(type(address) is int for address in found):
        raise FormatError(f"{path}: a function of a unit has no integer address")
    return found


def cmd_unlisted(args) -> int:
    """Sort the functions of the resident executable that an inventory of it does not list.

    The executable is swept from the first address of the inventory to
    `--end`, or to the end of the inventory's last function. A function of
    the sweep whose start the inventory does not have is unlisted. Each is
    given:

    - an area: game code if it starts below `--library`, library code
      otherwise;
    - whether a unit of the resident image declares a function at its start
      in the build configuration;
    - one way it is referred to, the first that applies of REFERENCES. The
      references come from `code_references`, over the functions that the
      sweep finds in the executable and the words of the image outside
      them, and over every distinct content of a code-bearing chunk of
      table 0 in the same way.

    The classes say what was counted. A function without a counted
    reference may still be reached in a way this does not look for.
    """
    try:
        image, tables = load_tables(args)
        table = tables[0]
        wanted = funcscan.read_inventory(args.inventory)
        if not wanted:
            raise FormatError("the inventory is empty")
        owned = build_functions(args.config) if args.config else set()
    except (OSError, FormatError, tomllib.TOMLDecodeError) as exc:
        print(exc)
        return 1
    words = list(struct.unpack_from(f"<{len(image.payload) // 4}I", image.payload))
    start, end = min(wanted), args.end or max(address + size for address, size in wanted.items())
    if not image.start <= start < end <= image.start + 4 * len(words) or start % 4 or end % 4:
        print(f"the range to sweep, {start:#x} to {end:#x}, is not a range of words inside the image")
        return 1
    low, high = (start - image.start) // 4, (end - image.start) // 4
    swept = funcscan.scan(words, image.start, low, high, funcscan.reader(words, image.start))
    found = dict(swept)
    unlisted = {address: size for address, size in swept if address not in wanted}
    missing = sorted(a for a in wanted if a not in found)
    outside = sorted(a for a in owned if a not in found)

    counts: dict[str, collections.Counter] = {"executable": code_references(words, image.start, swept, unlisted)}
    counts["modules"] = collections.Counter()
    archives, bad = read_archives(args.directory)
    symbols = sorted({a for a in funcscan.read_entries(args.symbols) if a % 4 == 0})
    contents = 0
    try:
        for _, _, base, _, body, functions, _, _ in swept_chunks(archives, table, symbols):
            contents += 1
            counts["modules"].update(code_references(body, base, functions, unlisted))
    except FormatError as exc:
        print(exc)
        return 1

    def reference(address: int) -> str:
        for name, *sources in REFERENCES:
            if any(counts[source][address, kind] for kind, source in sources):
                return name
        return NO_REFERENCE

    def area(address: int) -> str:
        return "game" if address < args.library else "library"

    inside = {a for address, size in swept for a in range(address, address + size, 4)}
    between = [words[(a - image.start) // 4] for a in range(start, end, 4) if a not in inside]
    print(f"{len(archives)} archives parsed, {bad} rejected; code-bearing chunks with distinct contents: {contents}")
    print(
        f"swept {start:#x} to {end:#x}, {end - start} bytes: {len(swept)} functions, {4 * len(inside)} bytes;"
        f" outside them {between.count(0)} zero words and {len(between) - between.count(0)} other words"
    )
    print(f"inventory: {len(wanted)} functions; starts that the sweep does not find: {len(missing)}")
    if args.config:
        print(f"functions of the build that are no start of the sweep: {len(outside)}")
        for address in outside[: args.show]:
            print(f"  {address:#x}")
    print(f"not in the inventory: {len(unlisted)} functions, {sum(unlisted.values())} bytes")
    classes: collections.Counter = collections.Counter()
    sizes: collections.Counter = collections.Counter()
    rows = []
    for address, size in sorted(unlisted.items()):
        key = (area(address), "yes" if address in owned else "no", reference(address))
        classes[key] += 1
        sizes[key] += size
        numbers = "\t".join(str(counts[source][address, kind]) for kind in ("call", "word", "pair") for source in counts)
        rows.append(f"{address:08x}\t{size}\t" + "\t".join(key) + f"\t{numbers}")
    print(" area     in the build  reference                 functions   bytes")
    order = [name for name, *_ in REFERENCES] + [NO_REFERENCE]
    for key in sorted(classes, key=lambda k: (k[0], k[1] == "yes", order.index(k[2]))):
        print(f" {key[0]:8} {key[1]:13} {key[2]:25} {classes[key]:9} {sizes[key]:7}")
    for name in ("game", "library"):
        listed = sum(1 for a in wanted if area(a) == name)
        more = sum(1 for a in unlisted if area(a) == name)
        built = sum(1 for a in found if area(a) == name and a in owned)
        tail = f"; in the build: {built}" if args.config else ""
        print(f"{name}: {listed} in the inventory and {more} not, {listed + more} together{tail}")
    if args.out:
        Path(args.out).parent.mkdir(parents=True, exist_ok=True)
        Path(args.out).write_text("".join(row + "\n" for row in rows))
    return 1 if bad or missing else 0


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
        ("functions", cmd_functions, "sweep the code-bearing chunks for functions and count distinct ones"),
        ("unlisted", cmd_unlisted, "sort the functions of the executable that an inventory does not list"),
    ):
        p = sub.add_parser(name, help=text)
        p.add_argument("executable", help="the resident PS-X executable")
        p.add_argument("directory", help="directory holding the archives")
        p.add_argument("--pointers", type=address, required=True, help="address of the block of table addresses")
        if name == "loadmap":
            p.add_argument("--out", help="write the rows as JSON")
            p.add_argument("--symbols", help="a file of `name = 0xADDRESS;` lines: report those outside the image")
        elif name == "functions":
            p.add_argument(
                "--out",
                help="write one line per function: first archive with that content, slot, address, size, address-blind hash",
            )
            p.add_argument("--common", type=int, default=20, help="report two slots that share this many functions (default 20)")
            p.add_argument(
                "--symbols",
                help="a file of `name = 0xADDRESS;` lines: each is an entry of the modules where a function with a frame starts there",
            )
        elif name == "unlisted":
            p.add_argument("--inventory", required=True, help="address, name and size per line, as funcscan.py compare reads it")
            p.add_argument("--library", type=address, required=True, help="a function that starts at or above this is library code")
            p.add_argument("--end", type=address, help="address after the last word to sweep (default: the end of the inventory)")
            p.add_argument("--config", help="a build configuration: its resident units say which functions are in the build")
            p.add_argument("--symbols", help="as for `functions`: entries of the modules")
            p.add_argument("--show", type=int, default=10, help="addresses to print per finding")
            p.add_argument(
                "--out",
                help="one line per function: address, size, area, in the build, reference, then calls, words and pairs,"
                " each counted in the executable and in the modules",
            )
        else:
            p.add_argument("--first", type=address, default=4, help="slot of the first-side block (default 4)")
            p.add_argument("--second", type=address, default=5, help="slot of the second-side block (default 5)")
            p.add_argument("--show", type=int, default=0, help="print up to this many unexplained words per pair")
        p.set_defaults(fn=fn)
    args = parser.parse_args()
    return args.fn(args)


if __name__ == "__main__":
    sys.exit(main())
