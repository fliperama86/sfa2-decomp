#!/usr/bin/env python3
"""Read the chunk archives (`*.PAC`) used on the disc.

Layout, established by parsing and checked against every file's size:
a little-endian u32 chunk count at offset 0, then one 32-byte entry per chunk
from offset 0x20 holding a u32 type, a u32 size in bytes and a copy of the
chunk's first four bytes. Chunk data follows from offset 0x800 in entry order,
each chunk padded to a 2,048-byte boundary.

`list` prints the table. `extract` writes one chunk. `scan` inventories a
directory and estimates, for chunks that look like MIPS code, the address the
chunk is linked for. That estimate is static: it finds the base under which
the most absolute call targets land on function prologues. Nothing here was
observed in a running game.
"""

from __future__ import annotations

import argparse
import collections
import json
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
        chunks.append({"index": index, "type": kind, "size": size, "offset": offset})
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
            print(f"  {c['index']:2} type {c['type']:#8x} size {c['size']:#8x} at {c['offset']:#8x}")
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
    args = parser.parse_args()
    return args.fn(args)


if __name__ == "__main__":
    sys.exit(main())
