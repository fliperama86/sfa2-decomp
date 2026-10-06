#!/usr/bin/env python3
"""Controls for baseline.py and pac.py using synthetic inputs.

Builds a tiny fake disc image and fake chunk archives in a temporary
directory. No game data is involved.
"""

from __future__ import annotations

import hashlib
import json
import os
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

import pac

TOOLS = Path(__file__).resolve().parent
SECTOR_SIZE, PAYLOAD_OFFSET, USER_DATA = 2352, 24, 2048


def tool(name: str, *args) -> subprocess.CompletedProcess:
    return subprocess.run([sys.executable, str(TOOLS / name), *map(str, args)], capture_output=True, text=True)


def make_image(path: Path, files: dict[str, bytes]) -> dict:
    """Write a raw image holding the files one after another and return its inventory."""
    image, entries, lba = bytearray(), [], 0
    for name, data in files.items():
        entries.append({"path": name, "lba": lba, "size": len(data), "flags": 0})
        for start in range(0, len(data), USER_DATA):
            sector = bytearray(SECTOR_SIZE)
            piece = data[start : start + USER_DATA]
            sector[PAYLOAD_OFFSET : PAYLOAD_OFFSET + len(piece)] = piece
            image += sector
            lba += 1
    path.write_bytes(image)
    return {
        "input": str(path),
        "size": len(image),
        "sha256": hashlib.sha256(image).hexdigest(),
        "sector_size": SECTOR_SIZE,
        "payload_offset": PAYLOAD_OFFSET,
        "files": entries,
    }


def make_archive(chunks: list[tuple[int, bytes]]) -> bytearray:
    """Build a chunk archive in the layout pac.py documents."""
    data = bytearray(USER_DATA)
    struct.pack_into("<I", data, 0, len(chunks))
    for index, (kind, body) in enumerate(chunks):
        struct.pack_into("<II", data, 0x20 + 32 * index, kind, len(body))
        data[0x28 + 32 * index : 0x2C + 32 * index] = body[:4].ljust(4, b"\0")
        data += body + bytes(-len(body) % USER_DATA)
    return data


def make_code(base: int, functions: int = 10) -> bytes:
    """MIPS-shaped code linked at `base`: each function calls the next one and returns.

    A word pointing at the block's own start and an address formed by `lui`
    and `addiu` follow, so that relinking changes every kind of word that
    `pac.py sides` names.
    """
    words = []
    for n in range(functions):
        target = base + 20 * ((n + 1) % functions)
        words += [0x27BDFFE8, 0x0C000000 | (target >> 2) & 0x03FFFFFF, 0, 0x03E00008, 0x27BD0018]
    data = base + 0x8000  # the low half has its sign bit set: the upper half carries
    words += [base, 0x3C020000 | (data + 0x8000) >> 16, 0x24420000 | data & 0xFFFF]
    return struct.pack(f"<{len(words)}I", *words)


def make_executable(start: int, pointers: int, tables: list[list[int]]) -> bytes:
    """A PS-X executable holding destination tables one after another, then the block of their addresses."""
    words, starts = [], []
    for table in tables:
        starts.append(start + 4 * len(words))
        words += table
    words += [0] * ((pointers - start) // 4 - len(words))
    words += starts + [0]
    body = struct.pack(f"<{len(words)}I", *words)
    header = bytearray(0x800)
    header[:8] = b"PS-X EXE"
    struct.pack_into("<II", header, 0x18, start, len(body))
    return bytes(header) + body


def make_program(start: int, code: list[int], tables: list[list[int]]) -> tuple[bytes, int]:
    """A PS-X executable holding code and data words, then the destination tables, then the block of their addresses.

    Returns the executable and the address of the block.
    """
    words, starts = list(code), []
    for table in tables:
        starts.append(start + 4 * len(words))
        words += table
    pointers = start + 4 * len(words)
    words += starts + [0]
    body = struct.pack(f"<{len(words)}I", *words)
    header = bytearray(0x800)
    header[:8] = b"PS-X EXE"
    struct.pack_into("<II", header, 0x18, start, len(body))
    return bytes(header) + body, pointers


def baseline_cases(root: Path):
    files = {"A.BIN": bytes(range(256)) * 20, "DIR/B.BIN": b"second file" * 300}
    image = root / "disc.img"
    inventory = root / "inventory.json"
    inventory.write_text(json.dumps(make_image(image, files)))
    manifest = root / "manifest.json"

    yield "pin", tool("baseline.py", "pin", "--inventory", inventory, "--out", manifest), 0, "pinned 2 files"
    yield "verify-image", tool("baseline.py", "verify", "--manifest", manifest, "--image", image), 0, "2 matched"
    out = root / "out"
    yield "extract", tool("baseline.py", "extract", "--manifest", manifest, "--image", image, "--out", out), 0, "extracted 2 files"
    same = all((out / name).read_bytes() == data for name, data in files.items())
    yield "extract-content", subprocess.CompletedProcess([], 0 if same else 1, "content equal" if same else "", ""), 0, "content equal"
    yield "verify-files", tool("baseline.py", "verify", "--manifest", manifest, "--files", out), 0, "2 matched"

    damaged = bytearray(image.read_bytes())
    damaged[PAYLOAD_OFFSET + 5] ^= 1
    bad_image = root / "damaged.img"
    bad_image.write_bytes(damaged)
    yield "verify-damaged-image", tool("baseline.py", "verify", "--manifest", manifest, "--image", bad_image), 1, "MISMATCH"
    yield "extract-damaged-image", tool(
        "baseline.py", "extract", "--manifest", manifest, "--image", bad_image, "--out", root / "out2"
    ), 1, "MISMATCH"
    (out / "A.BIN").write_bytes(b"changed")
    yield "verify-damaged-file", tool("baseline.py", "verify", "--manifest", manifest, "--files", out), 1, "MISMATCH (files): A.BIN"
    yield "extract-no-match", tool(
        "baseline.py", "extract", "--manifest", manifest, "--image", image, "--out", root / "out3", "--match", "none*"
    ), 1, "extracted 0 files"
    yield "pin-wrong-image", tool("baseline.py", "pin", "--inventory", inventory, "--out", root / "m2.json", "--image", bad_image), 1, "image mismatch"


def outcome(ok: bool, text: str) -> subprocess.CompletedProcess:
    """Wrap a direct check so it reports like a tool run."""
    return subprocess.CompletedProcess([], 0 if ok else 1, text if ok else "", "")


def extract_safety_cases(root: Path):
    """Extraction must stay inside --out and must not write through links or onto its inputs."""
    base = root / "safety"
    base.mkdir()
    files = {"A.BIN": b"alpha" * 500, "DIR/B.BIN": b"beta" * 700}
    image = base / "disc.img"
    inventory = base / "inventory.json"
    inventory.write_text(json.dumps(make_image(image, files)))
    manifest = base / "manifest.json"
    yield "safety-setup", tool("baseline.py", "pin", "--inventory", inventory, "--out", manifest), 0, "pinned 2 files"
    good = json.loads(manifest.read_text())
    image_bytes, manifest_bytes = image.read_bytes(), manifest.read_bytes()

    outside = base / "outside"
    outside.mkdir()
    important = outside / "important.txt"
    important.write_bytes(b"keep me")

    def untouched() -> bool:
        return (
            important.read_bytes() == b"keep me"
            and sorted(x.name for x in outside.iterdir()) == ["important.txt"]
            and image.read_bytes() == image_bytes
            and manifest.read_bytes() == manifest_bytes
        )

    def renamed(name: str, old: str, new: str) -> Path:
        changed = json.loads(json.dumps(good))
        for entry in changed["files"]:
            if entry["path"] == old:
                entry["path"] = new
        path = base / f"{name}.json"
        path.write_text(json.dumps(changed))
        return path

    def extract(manifest_path: Path, image_path: Path, out: Path) -> subprocess.CompletedProcess:
        return tool("baseline.py", "extract", "--manifest", manifest_path, "--image", image_path, "--out", out)

    traversal = renamed("traversal", "A.BIN", "../outside/important.txt")
    yield "extract-parent-path", extract(traversal, image, base / "out1"), 1, "UNSAFE MANIFEST PATH"
    yield "extract-parent-path-untouched", outcome(untouched(), "outside and inputs unchanged"), 0, "unchanged"
    absolute = renamed("absolute", "A.BIN", str(important))
    yield "extract-absolute-path", extract(absolute, image, base / "out2"), 1, "UNSAFE MANIFEST PATH"
    yield "extract-absolute-path-untouched", outcome(untouched(), "outside and inputs unchanged"), 0, "unchanged"
    yield "verify-unsafe-manifest", tool("baseline.py", "verify", "--manifest", traversal, "--image", image), 1, "UNSAFE MANIFEST PATH"

    out3 = base / "out3"
    out3.mkdir()
    (out3 / "DIR").symlink_to(outside, target_is_directory=True)
    yield "extract-symlinked-directory", extract(manifest, image, out3), 1, "is a symbolic link"
    yield "extract-symlinked-directory-untouched", outcome(untouched(), "outside and inputs unchanged"), 0, "unchanged"

    out4 = base / "out4"
    out4.mkdir()
    os.link(important, out4 / "A.BIN")
    yield "extract-hard-linked-destination", extract(manifest, image, out4), 0, "extracted 2 files"
    replaced = (out4 / "A.BIN").read_bytes() == files["A.BIN"]
    yield "extract-hard-linked-destination-untouched", outcome(untouched() and replaced, "link target unchanged, file replaced"), 0, "unchanged"

    out5 = base / "out5"
    out5.mkdir()
    (out5 / "A.BIN").symlink_to(important)
    yield "extract-symlinked-destination", extract(manifest, image, out5), 0, "extracted 2 files"
    replaced = not (out5 / "A.BIN").is_symlink() and (out5 / "A.BIN").read_bytes() == files["A.BIN"]
    yield "extract-symlinked-destination-untouched", outcome(untouched() and replaced, "link target unchanged, file replaced"), 0, "unchanged"

    out6 = base / "out6"
    out6.mkdir()
    image_in_out = out6 / "A.BIN"
    image_in_out.write_bytes(image_bytes)
    yield "extract-onto-image", extract(manifest, image_in_out, out6), 1, "is an input of this command"
    yield "extract-onto-image-untouched", outcome(image_in_out.read_bytes() == image_bytes, "image unchanged"), 0, "unchanged"

    out7 = base / "out7"
    (out7 / "DIR").mkdir(parents=True)
    manifest_in_out = out7 / "DIR" / "B.BIN"
    manifest_in_out.write_bytes(manifest_bytes)
    yield "extract-onto-manifest", extract(manifest_in_out, image, out7), 1, "is an input of this command"
    yield "extract-onto-manifest-untouched", outcome(manifest_in_out.read_bytes() == manifest_bytes, "manifest unchanged"), 0, "unchanged"

    out8 = base / "out8"
    out8.mkdir()
    os.link(image, out8 / "A.BIN")
    yield "extract-onto-linked-image", extract(manifest, image, out8), 1, "is an input of this command"
    yield "extract-onto-linked-image-untouched", outcome(untouched(), "outside and inputs unchanged"), 0, "unchanged"


def pac_cases(root: Path):
    good = make_archive([(4, b"abcd" * 700), (0x20000, b"xy" * 10)])
    path = root / "GOOD.PAC"
    path.write_bytes(good)
    yield "pac-list", tool("pac.py", "list", path), 0, "2 chunks"
    chunk = root / "chunk.bin"
    yield "pac-extract", tool("pac.py", "extract", path, 1, chunk), 0, "wrote 20 bytes"

    wrong_word = bytearray(good)
    wrong_word[USER_DATA] ^= 1
    (root / "WORD.PAC").write_bytes(wrong_word)
    yield "pac-first-word", tool("pac.py", "list", root / "WORD.PAC"), 1, "first word does not match"
    (root / "SHORT.PAC").write_bytes(good[:-USER_DATA])
    yield "pac-truncated", tool("pac.py", "list", root / "SHORT.PAC"), 1, "runs past the end"
    (root / "LONG.PAC").write_bytes(good + bytes(USER_DATA))
    yield "pac-trailing-data", tool("pac.py", "list", root / "LONG.PAC"), 1, "file size is"
    empty = bytearray(good)
    struct.pack_into("<I", empty, 0, 0)
    (root / "EMPTY.PAC").write_bytes(empty)
    yield "pac-zero-count", tool("pac.py", "list", root / "EMPTY.PAC"), 1, "implausible chunk count"

    scan_ok = root / "scan-ok"
    scan_ok.mkdir()
    (scan_ok / "GOOD.PAC").write_bytes(good)
    yield "pac-scan", tool("pac.py", "scan", scan_ok), 0, "1 archives parsed, 0 rejected"
    scan_bad = root / "scan-bad"
    scan_bad.mkdir()
    (scan_bad / "GOOD.PAC").write_bytes(good)
    (scan_bad / "WORD.PAC").write_bytes(wrong_word)
    yield "pac-scan-rejects", tool("pac.py", "scan", scan_bad), 1, "1 rejected"
    yield "pac-list-table-and-slot", tool("pac.py", "list", path), 0, "(table 2, slot 0x0)"

    # The loader's tables: an executable whose table 0 sends slot 4 to FIRST and slot 5 to SECOND.
    start, pointers, first, second = 0x80100000, 0x80100100, 0x80200000, 0x80218000
    table0 = [0x80300000, 0x80300000, 0x80300000, 0x80300000, first, second]
    exe = root / "MAIN.EXE"
    exe.write_bytes(make_executable(start, pointers, [table0, [0x80400000], [0x80500000]]))
    side1, side2 = make_code(first), make_code(second)
    maps = root / "maps"
    maps.mkdir()
    (maps / "PL00.PAC").write_bytes(make_archive([(4, side1), (0x10000, b"data" * 8)]))
    (maps / "PL00X.PAC").write_bytes(make_archive([(5, side2)]))
    loadmap = lambda *a: tool("pac.py", "loadmap", *a)  # noqa: E731
    yield "loadmap-agrees", loadmap(exe, maps, "--pointers", hex(pointers)), 0, "2 agree with the table, 0 differ"
    yield "loadmap-bounds", loadmap(exe, maps, "--pointers", hex(pointers)), 0, "table 0: at most 6 entries"
    yield "loadmap-last-table", loadmap(exe, maps, "--pointers", hex(pointers)), 0, "table 2: length not bounded"

    # The same archives against a table that sends slot 5 somewhere else.
    moved = root / "MOVED.EXE"
    moved.write_bytes(make_executable(start, pointers, [table0[:5] + [second + 0x1000], [0x80400000], [0x80500000]]))
    yield "loadmap-estimate-differs", loadmap(moved, maps, "--pointers", hex(pointers)), 1, "ESTIMATE DIFFERS: 0x80218000"
    # A code chunk whose slot lies beyond the table has no destination to agree with.
    beyond = root / "beyond"
    beyond.mkdir()
    (beyond / "A.PAC").write_bytes(make_archive([(9, side1)]))
    yield "loadmap-code-without-entry", loadmap(exe, beyond, "--pointers", hex(pointers)), 1, "ESTIMATE DIFFERS"
    # A data chunk there is counted, not judged.
    nodata = root / "nodata"
    nodata.mkdir()
    (nodata / "A.PAC").write_bytes(make_archive([(9, b"data" * 8), (4, side1)]))
    yield "loadmap-data-without-entry", loadmap(exe, nodata, "--pointers", hex(pointers)), 0, "chunks without a table entry: 1"
    yield "loadmap-no-pointer-block", loadmap(exe, maps, "--pointers", hex(start)), 1, "no block of table addresses"
    # Symbols outside the image: one at a function start of the first block, one in the middle
    # of a function, one where nothing is loaded, one inside the image (not reported).
    names = root / "symbols.ld"
    names.write_text(
        f"entry = {first + 20:#x};\nmiddle = {first + 24:#x};\nnowhere = 0x80600000;\ninside = {start:#x};\n"
    )
    with_symbols = loadmap(exe, maps, "--pointers", hex(pointers), "--symbols", names)
    yield "loadmap-symbol-at-function", with_symbols, 0, f"entry {first + 20:#x}  slot 0x4: 1/1/1"
    yield "loadmap-symbol-inside-function", with_symbols, 0, f"middle {first + 24:#x}  slot 0x4: 1/0/1"
    yield "loadmap-symbol-unloaded", with_symbols, 0, "no chunk reaches 1 symbol(s) from 0x80600000 to 0x80600000"
    yield "loadmap-symbol-in-image-skipped", outcome("inside" not in with_symbols.stdout, "not reported"), 0, "not reported"
    yield "loadmap-pointers-outside", loadmap(exe, maps, "--pointers", "0x80700000"), 1, "no block of table addresses"
    yield "loadmap-not-an-executable", loadmap(path, maps, "--pointers", hex(pointers)), 1, "not a PS-X executable"
    short = root / "SHORT.EXE"
    short.write_bytes(exe.read_bytes()[:-8])
    yield "loadmap-short-executable", loadmap(short, maps, "--pointers", hex(pointers)), 1, "shorter than its header says"
    yield "loadmap-rejected-archive", loadmap(exe, scan_bad, "--pointers", hex(pointers)), 1, "1 rejected"
    empty_dir = root / "none"
    empty_dir.mkdir()
    yield "loadmap-no-archive", loadmap(exe, empty_dir, "--pointers", hex(pointers)), 1, "no archive parsed"

    # The two sides: relinked code differs only in ways the distance explains.
    sides = lambda *a: tool("pac.py", "sides", *a)  # noqa: E731
    yield "sides-relinked", sides(exe, maps, "--pointers", hex(pointers)), 0, "PL00.PAC and PL00X.PAC: 40 words identical, 13 differ, 0 other"
    # Ten calls, one pointer and one address pair: see make_code.
    for want in ("10  jump target moved", "1  word pointing into the block", "1  upper half", "1  lower half"):
        yield f"sides-class-{want.split()[1]}-{want.split()[2]}", sides(exe, maps, "--pointers", hex(pointers)), 0, want
    # One word changed for no reason the distance explains.
    odd = root / "odd"
    odd.mkdir()
    changed = bytearray(side2)
    struct.pack_into("<I", changed, 8, 0x12345678)
    (odd / "PL00.PAC").write_bytes(make_archive([(4, side1)]))
    (odd / "PL00X.PAC").write_bytes(make_archive([(5, bytes(changed))]))
    yield "sides-unexplained-word", sides(exe, odd, "--pointers", hex(pointers), "--show", 4), 0, "0x8: 00000000 12345678"
    # A twin of another size is not compared, and a run that compared nothing fails.
    sized = root / "sized"
    sized.mkdir()
    (sized / "PL00.PAC").write_bytes(make_archive([(4, side1)]))
    (sized / "PL00X.PAC").write_bytes(make_archive([(5, side2 + bytes(4))]))
    yield "sides-size-differs", sides(exe, sized, "--pointers", hex(pointers)), 1, "sizes differ (0xd4, 0xd8), not compared"
    yield "sides-no-twin", sides(exe, nodata, "--pointers", hex(pointers)), 1, "0 pairs compared"
    yield "sides-slot-outside-table", sides(exe, maps, "--pointers", hex(pointers), "--second", 9), 1, "table 0 has no such slots"


def function_cases(root: Path):
    """`pac.py functions`: the inventory of functions inside the code-bearing chunks."""
    OPEN, CLOSE, RETURN, ONE, TWO = 0x27BDFFE8, 0x27BD0018, 0x03E00008, 0x24020001, 0x24020002
    UPPER, LOAD, STOP = 0x3C028020, 0x8C420000, 0xFFFFFFFF  # lui v0 / lw v0,0(v0) / not an instruction
    words = lambda *w: struct.pack(f"<{len(w)}I", *w)  # noqa: E731
    start, pointers, first, second = 0x80100000, 0x80100100, 0x80200000, 0x80218000
    table0 = [0x80300000, 0x80300000, 0x80300000, 0x80300000, first, second]
    exe = root / "FUNCTIONS.EXE"
    exe.write_bytes(make_executable(start, pointers, [table0, [0x80400000], [0x80500000]]))
    side1, side2 = make_code(first), make_code(second)
    functions = lambda directory, *a: tool("pac.py", "functions", exe, directory, "--pointers", hex(pointers), *a)  # noqa: E731

    def folder(name: str, **archives: bytes) -> Path:
        path = root / name
        path.mkdir()
        for stem, data in archives.items():
            (path / f"{stem}.PAC").write_bytes(data)
        return path

    # Two blocks of ten functions each, the same code linked at two addresses: see make_code.
    # A function is 20 bytes; the three words after the last one never return and are not counted.
    twins = folder("twins", PL00=make_archive([(4, side1), (0x10000, b"data" * 8)]), PL00X=make_archive([(5, side2)]))
    out = root / "made" / "functions.tsv"
    run = functions(twins, "--out", out)
    yield "functions-contents", run, 0, "2 archives parsed, 0 rejected; code-bearing chunks with distinct contents: 2"
    row = "  0x4   0x80200000         1            212         10             200                 10                       1                 0"
    yield "functions-slot-row", run, 0, row
    yield "functions-totals", run, 0, "all slots: 20 functions, 400 bytes; distinct by bytes: 20; distinct address-blind: 1 functions, 20 bytes"
    yield "functions-same-set", run, 0, "slots 0x4 and 0x5: the same 1 address-blind distinct functions"
    yield "functions-no-symbols-no-line", outcome("symbols" not in run.stdout, "no line"), 0, "no line"
    lines = out.read_text().splitlines() if out.exists() else []
    yield "functions-out-lines", outcome(len(lines) == 20 and len({line.split("\t")[4] for line in lines}) == 1, "20 lines, one hash"), 0, "20 lines"
    yield "functions-out-first", outcome(bool(lines) and lines[0].startswith(f"PL00.PAC\t0x4\t{first:08x}\t20\t"), "first line"), 0, "first line"
    blind = hashlib.sha256(pac.address_blind(list(struct.unpack("<5I", side1[:20])))).hexdigest()[:16]
    yield "functions-out-hash", outcome(bool(lines) and lines[0].split("\t")[4] == blind, "sixteen digits"), 0, "sixteen digits"
    yield "functions-out-second-block", outcome(len(lines) == 20 and lines[10].startswith(f"PL00X.PAC\t0x5\t{second:08x}\t20\t"), "line 11"), 0, "line 11"

    # A second content in slot 4 with one more function, and the first content in two archives.
    other = words(OPEN, ONE, RETURN, CLOSE)
    wider = side1 + words(STOP) + other
    shared = folder(
        "shared",
        A=make_archive([(4, side1)]),
        B=make_archive([(4, side1)]),
        C=make_archive([(4, wider)]),
        D=make_archive([(5, side2)]),
    )
    out = root / "shared.tsv"
    run = functions(shared, "--out", out)
    yield "functions-content-counted-once", run, 0, "4 archives parsed, 0 rejected; code-bearing chunks with distinct contents: 3"
    row = "  0x4   0x80200000         2            444         21             416                 11                       2                 1"
    yield "functions-two-contents-row", run, 0, row
    yield "functions-two-contents-totals", run, 0, "all slots: 31 functions, 616 bytes; distinct by bytes: 21; distinct address-blind: 2 functions, 36 bytes"
    yield "functions-per-content", run, 0, "slot 0x4: of its 2 address-blind distinct functions, 1 are in one of its 2 contents only and 1 in all of them"
    yield "functions-one-content-no-line", outcome("slot 0x5: of its" not in run.stdout, "no line"), 0, "no line"
    yield "functions-common-default", outcome("in common" not in run.stdout and "the same" not in run.stdout, "not reported"), 0, "not reported"
    yield "functions-common-one", functions(shared, "--common", 1), 0, "slots 0x4 and 0x5: 1 address-blind distinct functions in common"
    # Three contents in a slot: one function in all of them, two in one content each.
    third = side1 + words(STOP, OPEN, TWO, RETURN, CLOSE)
    three = folder("three", A=make_archive([(4, side1)]), B=make_archive([(4, wider)]), C=make_archive([(4, third)]))
    yield "functions-per-content-of-three", functions(three), 0, (
        "slot 0x4: of its 3 address-blind distinct functions, 2 are in one of its 3 contents only and 1 in all of them"
    )
    # Two slots with as many distinct functions each, but not the same ones.
    unlike = folder("unlike", A=make_archive([(4, wider)]), B=make_archive([(5, side2 + words(STOP, OPEN, TWO, RETURN, CLOSE))]))
    yield "functions-unlike-sets", functions(unlike, "--common", 1), 0, "slots 0x4 and 0x5: 1 address-blind distinct functions in common"
    yield "functions-unlike-sets-not-the-same", outcome("the same" not in functions(unlike).stdout, "not reported"), 0, "not reported"
    # An address is written with eight digits.
    low = root / "LOW.EXE"
    low.write_bytes(make_executable(start, pointers, [[0x00100000], [0x80400000], [0x80500000]]))
    low_out = root / "low.tsv"
    tool("pac.py", "functions", low, folder("low", A=make_archive([(0, make_code(0x00100000))])), "--pointers", hex(pointers), "--out", low_out)
    rows = low_out.read_text().splitlines() if low_out.exists() else []
    yield "functions-out-low-address", outcome(bool(rows) and rows[0].startswith("A.PAC\t0x0\t00100000\t20\t"), "eight digits"), 0, "eight digits"
    names = [line.split("\t")[0] for line in out.read_text().splitlines()] if out.exists() else []
    yield "functions-out-first-archive", outcome(names == ["A.PAC"] * 10 + ["C.PAC"] * 11 + ["D.PAC"] * 10, "A, C, D"), 0, "A, C, D"

    # Symbols. Words 54 and 55 of this block look like instructions and sit before a function that
    # nothing calls. It starts at word 56 with a load, opens its frame at word 58 and is 24 bytes.
    glued = side1 + words(STOP, ONE, TWO, UPPER, LOAD, OPEN, ONE, RETURN, CLOSE)
    entry = folder("entry", E=make_archive([(4, glued)]))
    yield "functions-glued", functions(entry), 0, "all slots: 11 functions, 232 bytes"

    def symbols(name: str, *addresses: int) -> Path:
        path = root / f"{name}.ld"
        path.write_text("".join(f"s{n} = {a:#x};\n" for n, a in enumerate(addresses)))
        return path

    at_load = functions(entry, "--symbols", symbols("load", first + 4 * 56))
    yield "functions-symbol-at-load", at_load, 0, "all slots: 11 functions, 224 bytes"
    yield "functions-symbol-taken", at_load, 0, "symbols whose address lies in a module: 1 cases; taken as a function start there: 1"
    yield "functions-symbol-at-frame", functions(entry, "--symbols", symbols("frame", first + 4 * 58)), 0, "all slots: 11 functions, 216 bytes"
    for label, index in (("in-glue", 55), ("after-frame", 59), ("at-return", 60)):
        ignored = functions(entry, "--symbols", symbols(label, first + 4 * index))
        yield f"functions-symbol-{label}", ignored, 0, "all slots: 11 functions, 232 bytes"
        yield f"functions-symbol-{label}-count", ignored, 0, "1 cases; taken as a function start there: 0"
    # An address that is not a multiple of four, and one where no module is, are not cases.
    stray = functions(entry, "--symbols", symbols("stray", first + 4 * 56, first + 4 * 56 + 2, 0x80600000))
    yield "functions-symbol-stray", stray, 0, "1 cases; taken as a function start there: 1"
    # The same address in two modules: a function start in one, the middle of a function in the other.
    # The other module keeps its two functions whole.
    pair = side1 + words(STOP) + other + words(OPEN, TWO, RETURN, CLOSE)
    two = folder("two", E=make_archive([(4, glued)]), F=make_archive([(4, pair)]))
    both = functions(two, "--symbols", symbols("two", first + 4 * 56))
    yield "functions-symbol-per-module", both, 0, "all slots: 23 functions, 456 bytes"
    yield "functions-symbol-per-module-count", both, 0, "2 cases; taken as a function start there: 1"

    # A symbol below the module is not a case.
    below = functions(entry, "--symbols", symbols("below", first - 8))
    yield "functions-symbol-below-the-module", below, 0, "0 cases; taken as a function start there: 0"
    # A symbol at the first address of a module is a case; one at the address after its last word is not.
    edge = folder("edge", E=make_archive([(4, side1)]))
    at_start = functions(edge, "--symbols", symbols("start", first))
    yield "functions-symbol-at-module-start", at_start, 0, "1 cases; taken as a function start there: 1"
    at_end = functions(edge, "--symbols", symbols("end", first + len(side1)))
    yield "functions-symbol-at-module-end", at_end, 0, "0 cases; taken as a function start there: 0"
    # One content in two slots is inventoried in both.
    run = functions(folder("twice", A=make_archive([(4, side1), (5, side1)])))
    yield "functions-content-in-two-slots", run, 0, "code-bearing chunks with distinct contents: 2"
    yield "functions-content-in-two-slots-totals", run, 0, "all slots: 20 functions, 400 bytes; distinct by bytes: 10; distinct address-blind: 1 functions, 20 bytes"
    # A jump table inside a module is read: its function goes on past the first return to the two cases.
    switch = words(OPEN, 0x3C018020, 0x8C220108, 0x00400008, 0, ONE, RETURN, 0, TWO, RETURN, CLOSE)
    jumping = side1 + words(STOP) + switch + words(STOP, first + 236, first + 248)
    yield "functions-jump-table", functions(folder("jumping", A=make_archive([(4, jumping)]))), 0, "all slots: 11 functions, 244 bytes"
    # The four counts of funcscan.py follow the totals, summed over the contents: here two contents,
    # each with one function that holds two returns.
    yield "functions-checks", run, 0, (
        "20 bytes\nchecks: `jr ra` words outside every function: 0; functions with more than one `jr ra`: 0;"
        " functions that open more than one stack frame: 0; functions that end neither with `jr ra` nor with a stub's `jr t2`: 0\n"
    )
    summed = functions(folder("summed", A=make_archive([(4, jumping)]), B=make_archive([(4, jumping + words(STOP))])))
    yield "functions-checks-summed", summed, 0, "outside every function: 0; functions with more than one `jr ra`: 2; functions that open"

    # A destination that is not a multiple of four is refused before any address is turned into a word index.
    askew = root / "ASKEW.EXE"
    askew.write_bytes(make_executable(start, pointers, [[0x80300002], [0x80400000], [0x80500000]]))
    yield "functions-destination-not-a-multiple-of-four", tool(
        "pac.py", "functions", askew, folder("askew", A=make_archive([(0, make_code(0x80300000))])), "--pointers", hex(pointers)
    ), 1, "slot 0x0: the destination 0x80300002 is not a multiple of four"
    # Bytes of a chunk after its last whole word are not swept.
    yield "functions-chunk-not-a-multiple-of-four", functions(folder("ragged", A=make_archive([(4, side1 + b"\x01\x02")]))), 0, (
        "  0x4   0x80200000         1            214         10             200"
    )

    # What is not inventoried, and what fails.
    yield "functions-no-code", functions(folder("plain", A=make_archive([(4, b"data" * 8)]))), 1, "no code-bearing chunk found"
    yield "functions-slot-beyond-table", functions(folder("far", A=make_archive([(9, side1)]))), 1, "no code-bearing chunk found"
    yield "functions-other-table", functions(folder("table1", A=make_archive([(0x10004, side1)]))), 1, "no code-bearing chunk found"
    damaged = bytearray(make_archive([(4, side1)]))
    damaged[USER_DATA] ^= 1
    rejected = functions(folder("rejected", A=make_archive([(4, side1)]), B=bytes(damaged)))
    yield "functions-rejected-archive", rejected, 1, "1 archives parsed, 1 rejected"
    yield "functions-rejected-still-counts", rejected, 1, "all slots: 10 functions, 200 bytes"
    yield "functions-not-an-executable", tool("pac.py", "functions", twins / "PL00.PAC", twins, "--pointers", hex(pointers)), 1, "not a PS-X executable"
    single = root / "SINGLE.EXE"
    single.write_bytes(make_executable(start, pointers, [table0]))
    yield "functions-no-pointer-block", tool("pac.py", "functions", single, twins, "--pointers", hex(pointers)), 1, "no block of table addresses"

    # starts_function: the forms taken as the start of a function with a frame.
    other_base = 0x8C620000  # lw v0,0(v1): not through the register the `lui` loaded
    for label, code, want in (
        ("frame", [OPEN], True),
        ("one-load", [UPPER, LOAD, OPEN], True),
        ("two-loads", [UPPER, LOAD, UPPER, LOAD, OPEN], True),
        ("three-loads", [UPPER, LOAD, UPPER, LOAD, UPPER, LOAD, OPEN], False),
        ("load-through-another-register", [UPPER, other_base, OPEN], False),
        ("load-of-a-byte", [UPPER, 0x80420000, OPEN], True),  # lb, the first kind of load
        ("load-of-an-unsigned-half", [UPPER, 0x94420000, OPEN], True),  # lhu, the last
        ("load-of-a-word-part", [UPPER, 0x98420000, OPEN], False),  # lwr
        ("store-instead-of-load", [UPPER, 0xA0420000, OPEN], False),  # sb
        ("no-instruction-instead-of-load", [UPPER, 0x7C420000, OPEN], False),
        ("load-without-frame", [UPPER, LOAD, ONE, OPEN], False),
        ("other-instruction", [ONE, OPEN], False),
        ("load-after-another-instruction", [ONE, LOAD, OPEN], False),
        ("negative-constant", [0x2402FFFF], False),  # li v0,-1
        ("stack-pointer-from-another-register", [0x245DFFE8], False),  # addiu sp,v0,-24
        ("another-register-from-the-stack-pointer", [0x27A2FFE8], False),  # addiu v0,sp,-24
        ("frame-closed", [CLOSE], False),
        ("upper-half-at-the-end", [UPPER], False),
        ("load-at-the-end", [UPPER, LOAD], False),
        ("nothing", [], False),
    ):
        yield f"starts-function-{label}", outcome(pac.starts_function(code, 0) is want, "as required"), 0, "as required"
    yield "starts-function-index", outcome(pac.starts_function([ONE, OPEN], 1) and not pac.starts_function([OPEN, ONE], 1), "as required"), 0, "as required"

    # address_blind: what two functions may differ in and still count as the same.
    jal, jump, branch = 0x0C000000, 0x08000000, 0x10400003
    add_at, table_load = 0x00220821, 0x8C220000  # addu at,at,v0 / lw v0,0(at)
    for label, one, two, want in (
        ("call-target", [jal | 0x100], [jal | 0x200], True),
        ("jump-target", [jump | 0x100], [jump | 0x200], True),
        ("call-is-not-jump", [jal | 0x100], [jump | 0x100], False),
        ("upper-half", [0x3C020001], [0x3C020002], True),
        ("upper-half-register", [0x3C020001], [0x3C030001], False),
        ("load-through-address", [UPPER, LOAD | 0x10], [UPPER, LOAD | 0x20], True),
        ("store-through-address", [UPPER, 0xAC430010], [UPPER, 0xAC430020], True),
        ("address-completed", [UPPER, 0x24420010], [UPPER, 0x24420020], True),
        ("offset-after-address-completed", [UPPER, 0x24420010, 0x8C430004], [UPPER, 0x24420010, 0x8C430008], False),
        ("offset-after-load", [UPPER, LOAD, 0x8C430004], [UPPER, LOAD, 0x8C430008], False),
        ("offset-through-other-register", [UPPER, 0x8C830004], [UPPER, 0x8C830008], False),
        ("constant", [ONE], [TWO], False),
        ("constant-after-register-reused", [UPPER, ONE], [UPPER, TWO], False),
        ("indexed-table", [0x3C010001, add_at, table_load | 0x10], [0x3C010002, add_at, table_load | 0x20], True),
        ("register-computed-anew", [UPPER, 0x00641021, 0x8C430004], [UPPER, 0x00641021, 0x8C430008], False),
        ("branch-distance", [branch], [branch + 1], False),
        # The register is overwritten by a load or a sum that does not use it.
        ("register-loaded-anew", [UPPER, 0x8C820000, 0x8C430004], [UPPER, 0x8C820000, 0x8C430008], False),
        ("register-summed-anew", [UPPER, 0x24820000, 0x8C430004], [UPPER, 0x24820000, 0x8C430008], False),
        # A store through the register leaves the address in it, even a store of the register itself.
        ("store-keeps-the-address", [UPPER, 0xAC420010, 0xAC430014], [UPPER, 0xAC420010, 0xAC430024], True),
        ("indexed-table-other-order", [0x3C010001, 0x00410821, table_load | 0x10], [0x3C010002, 0x00410821, table_load | 0x20], True),
        # A call: its delay slot still uses the address, what follows does not, but for a saved register.
        ("delay-slot-of-a-call", [UPPER, jal, LOAD | 0x10], [UPPER, jal, LOAD | 0x20], True),
        ("offset-after-a-call", [UPPER, 0xAC430010, jal, 0, 0x8C430004], [UPPER, 0xAC430010, jal, 0, 0x8C430008], False),
        ("offset-after-a-call-through-a-register", [0x3C030001, 0x0040F809, 0, 0x8C640004], [0x3C030001, 0x0040F809, 0, 0x8C640008], False),
        ("offset-after-a-jump", [UPPER, 0xAC430010, jump, 0, 0x8C430004], [UPPER, 0xAC430010, jump, 0, 0x8C430008], True),
        ("offset-after-another-register-jump", [0x3C030001, 0x00400008, 0, 0x8C640004], [0x3C030001, 0x00400008, 0, 0x8C640008], True),
    ):
        same = pac.address_blind(one) == pac.address_blind(two)
        yield f"address-blind-{label}", outcome(same is want, "as required"), 0, "as required"
    yield "address-blind-length", outcome(len(pac.address_blind([ONE, TWO, RETURN])) == 12, "as required"), 0, "as required"
    # The kinds of instruction whose 16-bit field is zeroed after a `lui` of their base register (v0 here),
    # and those that overwrite their target register (v0 again, from a0): by opcode.
    through = lambda op, low: pac.address_blind([UPPER, op << 26 | 2 << 21 | 3 << 16 | low])  # noqa: E731
    zeroed = [op for op in range(4, 64) if op != 0x0F and through(op, 0x10) == through(op, 0x20)]
    sums, loads, stores = list(range(0x08, 0x0F)), list(range(0x20, 0x27)), list(range(0x28, 0x3B))
    yield "address-blind-zeroed-opcodes", outcome(zeroed == sums + loads + stores, f"as required {zeroed}"), 0, "as required"
    after = lambda op, low: pac.address_blind([UPPER, op << 26 | 4 << 21 | 2 << 16, 0x8C430000 | low])  # noqa: E731
    kept = [op for op in range(4, 64) if op != 0x0F and after(op, 0x10) == after(op, 0x20)]
    others = [op for op in range(4, 64) if op != 0x0F and op not in sums + loads]
    yield "address-blind-overwriting-opcodes", outcome(kept == others, f"as required {kept}"), 0, "as required"
    # The registers that keep an address across a call: s0 to s7, gp, sp, fp.
    across = lambda reg, low: pac.address_blind([0x3C000000 | reg << 16, jal, 0, 0x8C040000 | reg << 21 | low])  # noqa: E731
    saved = [reg for reg in range(32) if across(reg, 0x10) == across(reg, 0x20)]
    yield "address-blind-saved-registers", outcome(saved == [*range(16, 24), 28, 29, 30], f"as required {saved}"), 0, "as required"


def unlisted_cases(root: Path):
    """`pac.py unlisted`: the functions of the executable that an inventory does not list, and how they are referred to.

    One executable holds a function in each of its 64-byte blocks, one per rule, and the functions that refer
    to them. Its expected output is worked out here from the layout, not read from the tool.
    """
    OPEN, CLOSE, RETURN, ONE, STOP = 0x27BDFFE8, 0x27BD0018, 0x03E00008, 0x24020001, 0xFFFFFFFF
    V0, V1, A0, A1 = 2, 3, 4, 5
    ADDI, ADDIU, ANDI, ORI, XORI = 0x08, 0x09, 0x0C, 0x0D, 0x0E
    jal = lambda t: 0x0C000000 | t >> 2 & 0x03FFFFFF  # noqa: E731
    jump = lambda t: 0x08000000 | t >> 2 & 0x03FFFFFF  # noqa: E731
    lui = lambda reg, value: 0x3C000000 | reg << 16 | value & 0xFFFF  # noqa: E731
    imm = lambda op, rt, rs, value: op << 26 | rs << 21 | rt << 16 | value & 0xFFFF  # noqa: E731
    upper = lambda target, op: (target + 0x8000) >> 16 if op in (ADDI, ADDIU) else target >> 16  # noqa: E731

    def form(target: int, op: int = ADDIU, into: int = A0) -> list[int]:
        """`lui v0` and the instruction that completes `target` from it."""
        return [lui(V0, upper(target, op)), imm(op, into, V0, target)]

    def verdict(ok: bool, detail: str) -> subprocess.CompletedProcess:
        return subprocess.CompletedProcess([], 0 if ok else 1, "as required" if ok else "", "" if ok else detail)

    # The layout. The executable starts one block before the first function of the inventory.
    START, BLOCK, SLOT_BASE = 0x8010F800, 0x40, 0x80200000
    IMAGE = START - BLOCK
    names = [
        "call_exe", "jump_exe", "self_jal", "self_j", "self_first", "adjacent_call", "adjacent_before", "switch", "calls_unlisted", "callee_of_unlisted",
        "loose_jal", "mod_only", "both",
        "word_exe", "word_mod", "word_inside", "all_three", "mod_call_exe_word", "word_and_pair",
        "pair_exe", "pair_mod", "pair_ori", "pair_reach8", "pair_reach9", "pair_beyond", "pair_lui_same",
        "pair_lui_other", "pair_first_a", "pair_first_b", "pair_other_rt", "pair_other_rs",
        "not_andi", "not_xori", "none", "area_below", "area_at", "straddler", "build_yes", "build_image",
        "symbol_word", "not_addi", "pair_positive",
    ]  # fmt: skip
    addr = {"first": START, **{name: START + BLOCK * (n + 1) for n, name in enumerate(names)}}
    assert addr["pair_exe"] & 0xFFFF >= 0x8000 and addr["pair_ori"] & 0xFFFF >= 0x8000
    assert addr["pair_positive"] & 0xFFFF < 0x8000 and addr["pair_first_a"] >> 16 == addr["pair_first_b"] >> 16 == 0x8010
    memory: dict[int, int] = {}
    spans: list[tuple[int, int]] = []

    def put(address: int, words: list[int], function: bool = True) -> None:
        for n, word in enumerate(words):
            memory[address + 4 * n] = word
        if function:
            spans.append((address, 4 * len(words)))

    plain = [OPEN, ONE, RETURN, CLOSE]
    bodies = {name: plain for name in names}
    bodies["self_jal"] = [OPEN, jal(addr["self_jal"]), 0, RETURN, CLOSE]
    bodies["self_j"] = [OPEN, jump(addr["self_j"]), 0, RETURN, CLOSE]
    bodies["self_first"] = [jal(addr["self_first"]), 0, RETURN, CLOSE]
    table = addr["switch"] + 4 * 12  # the cases are the words 5 and 8 of the function, after a `lui at` and a `lw v0,lo(at)`
    bodies["switch"] = [
        OPEN, lui(1, upper(table, ADDIU)), imm(0x23, V0, 1, table), 0x00400008, 0, ONE, RETURN, 0, ONE, RETURN, CLOSE,
    ]  # fmt: skip
    bodies["calls_unlisted"] = [OPEN, jal(addr["callee_of_unlisted"]), 0, RETURN, CLOSE]
    bodies["straddler"] = [OPEN, *[ONE] * 6, RETURN, CLOSE]
    put(IMAGE, plain)  # before the first address of the inventory: never swept
    put(addr["first"], plain)
    for name in names:
        put(addr[name], bodies[name])
    put(addr["switch"] + 44, [STOP, addr["switch"] + 20, addr["switch"] + 32, STOP], False)  # the table of the cases
    cursor = START + BLOCK * (len(names) + 1)
    listed: list[tuple[int, int, str]] = [(addr["first"], 16, "first")]
    # A function that calls the one before it, right after it.
    put(addr["adjacent_call"] + 16, [jal(addr["adjacent_call"]), 0, RETURN, CLOSE])
    listed.append((addr["adjacent_call"] + 16, 16, "adjacent_host"))
    # A function that ends with the call, right before it.
    put(addr["adjacent_before"] - 12, [OPEN, RETURN, jal(addr["adjacent_before"])])
    listed.append((addr["adjacent_before"] - 12, 12, "adjacent_before_host"))

    def host(name: str, words: list[int], gap: bool = True, framed: bool = True) -> None:
        nonlocal cursor
        body = [OPEN, *words, RETURN, CLOSE] if framed else words
        put(cursor, body)
        listed.append((cursor, 4 * len(body), name))
        cursor += 4 * len(body) + (4 if gap else 0)

    def loose(words: list[int], gap: bool = True) -> None:
        nonlocal cursor
        put(cursor, words, False)
        cursor += 4 * len(words) + (4 if gap else 0)

    def pair_host(name: str, target: int, op: int = ADDIU) -> None:
        host(name, form(target, op))

    host("calls", [
        jal(addr["call_exe"]), 0, jal(addr["both"]), 0, jal(addr["all_three"]), 0, jump(addr["jump_exe"]), 0, addr["word_inside"], jal(addr["build_yes"]), 0,
    ])  # fmt: skip
    pair_host("h_pair_exe", addr["pair_exe"])
    pair_host("h_all_three", addr["all_three"])
    pair_host("h_word_and_pair", addr["word_and_pair"])
    pair_host("h_pair_ori", addr["pair_ori"], ORI)
    pair_host("h_pair_positive", addr["pair_positive"])
    pair = form(addr["pair_reach8"])
    host("h_reach8", [pair[0], *[0] * 7, pair[1]])
    pair = form(addr["pair_reach9"])
    host("h_reach9", [pair[0], *[0] * 8, pair[1]])
    target = addr["pair_lui_same"]
    host("h_lui_same", [lui(V0, upper(target, ADDIU)), lui(V0, 0), imm(ADDIU, A0, V0, target)])
    target = addr["pair_lui_other"]
    host("h_lui_other", [lui(V0, upper(target, ADDIU)), lui(V1, 0), imm(ADDIU, A0, V0, target)])
    first_a, first_b = addr["pair_first_a"], addr["pair_first_b"]
    host("h_first", [lui(V0, upper(first_a, ADDIU)), imm(ADDIU, A0, V0, first_b), imm(ADDIU, A0, V0, first_a)])
    target = addr["pair_other_rt"]
    host("h_other_rt", [lui(V0, upper(target, ADDIU)), imm(ADDIU, A1, A1, 5), imm(ADDIU, A0, V0, target)])
    target = addr["pair_other_rs"]
    host("h_other_rs", [lui(V0, upper(target, ADDIU)), imm(ADDIU, A0, V1, target)])
    for name, op in (("not_addi", ADDI), ("not_andi", ANDI), ("not_xori", XORI)):
        host(f"h_{name}", [lui(V0, upper(addr[name], op)), imm(op, A0, V0, addr[name])])
    # The addiu lies right after the function: out of reach whatever its distance.
    target = addr["pair_beyond"]
    host("h_beyond", [OPEN, RETURN, lui(V0, upper(target, ADDIU))], gap=False, framed=False)
    loose([imm(ADDIU, A0, V0, target), STOP])
    loose([jal(addr["loose_jal"]), STOP])
    loose([addr["word_exe"], addr["all_three"], addr["word_and_pair"], addr["mod_call_exe_word"], STOP])
    host("last", [])
    inventory_end = cursor - 4
    after = cursor
    put(after, plain)
    cursor += 16
    after_end = cursor
    code = [memory.get(a, 0) for a in range(IMAGE, cursor + 4, 4)]
    table0 = [0x80300000] * 4 + [SLOT_BASE]
    tables = [table0, [0x80400000], [0x80500000]]
    program, pointers = make_program(IMAGE, code, tables)
    image_end = IMAGE + len(program) - 0x800
    exe = root / "UNLISTED.EXE"
    exe.write_bytes(program)

    # The modules: nine functions in slot 4, and a data word after them. The same content in two archives.
    def module_function(*words: int) -> list[int]:
        return [OPEN, *words, RETURN, CLOSE]

    module: list[int] = []
    for words in (
        [jal(addr["mod_only"])], [jal(addr["both"])], [addr["word_inside"]], [jal(addr["mod_call_exe_word"])],
        form(addr["pair_mod"]), [], [], [], [],
    ):  # fmt: skip
        module += module_function(*words)
    module += [0, addr["word_mod"], STOP]
    module_bytes = struct.pack(f"<{len(module)}I", *module)

    def folder(name: str, **archives: bytes) -> Path:
        path = root / name
        path.mkdir()
        for stem, data in archives.items():
            (path / f"{stem}.PAC").write_bytes(data)
        return path

    # A second content in slot 4, the same one in slot 3, and chunks that must not be read: a code chunk of
    # table 1, one of a slot beyond table 0, and data in slot 4 that holds the address eight times.
    def words_of(functions: list[list[int]]) -> bytes:
        flat = [w for words in functions for w in module_function(*words)]
        return struct.pack(f"<{len(flat)}I", *flat)

    second = words_of([[jal(addr["mod_only"])], *[[]] * 8])
    trap = words_of([[jal(addr["none"])], *[[]] * 8])
    data = struct.pack("<8I", *[addr["none"]] * 8)
    mods = folder(
        "unl-mods",
        A=bytes(make_archive([(4, module_bytes)])),
        B=bytes(make_archive([(4, module_bytes)])),
        C=bytes(make_archive([(4, second), (0x10004, trap), (9, trap), (4, data)])),
        D=bytes(make_archive([(3, second)])),
    )

    def inventory_file(name: str, entries: list[tuple[int, int, str]]) -> Path:
        path = root / f"{name}.tsv"
        path.write_text("".join(f"{a:08x}\t{n}\t{size}\n" for a, size, n in entries))
        return path

    inventory = inventory_file("unl-inventory", listed)
    library = addr["area_at"]

    # What each unlisted function is referred to by: the class and the six counts (calls, words, pairs; executable, modules).
    EXE, MOD = "called by the executable", "called by modules only"
    WORD, FORMED, NONE = "address in a data word", "address formed in code", "no reference found"
    order = [EXE, MOD, WORD, FORMED, NONE]
    expected = {
        "call_exe": (EXE, (1, 0, 0, 0, 0, 0)),
        "jump_exe": (EXE, (1, 0, 0, 0, 0, 0)),
        "adjacent_call": (EXE, (1, 0, 0, 0, 0, 0)),
        "adjacent_before": (EXE, (1, 0, 0, 0, 0, 0)),
        "callee_of_unlisted": (EXE, (1, 0, 0, 0, 0, 0)),
        "build_yes": (EXE, (1, 0, 0, 0, 0, 0)),
        "mod_only": (MOD, (0, 3, 0, 0, 0, 0)),
        "both": (EXE, (1, 1, 0, 0, 0, 0)),
        "word_exe": (WORD, (0, 0, 1, 0, 0, 0)),
        "word_mod": (WORD, (0, 0, 0, 1, 0, 0)),
        "all_three": (EXE, (1, 0, 1, 0, 1, 0)),
        "mod_call_exe_word": (MOD, (0, 1, 1, 0, 0, 0)),
        "word_and_pair": (WORD, (0, 0, 1, 0, 1, 0)),
        "pair_exe": (FORMED, (0, 0, 0, 0, 1, 0)),
        "pair_mod": (FORMED, (0, 0, 0, 0, 0, 1)),
        "pair_ori": (FORMED, (0, 0, 0, 0, 1, 0)),
        "pair_reach8": (FORMED, (0, 0, 0, 0, 1, 0)),
        "pair_lui_other": (FORMED, (0, 0, 0, 0, 1, 0)),
        "pair_first_b": (FORMED, (0, 0, 0, 0, 1, 0)),
        "pair_other_rt": (FORMED, (0, 0, 0, 0, 1, 0)),
        "pair_positive": (FORMED, (0, 0, 0, 0, 1, 0)),
    }  # every other function: NONE and six zeros. self_jal, self_j, loose_jal, word_inside, pair_reach9, pair_beyond,
    # pair_lui_same, pair_first_a, pair_other_rs and the three not_* are the ones that must stay without a reference.
    name_at = {a: n for n, a in addr.items()} | {after: "after"}
    size_of = dict(spans)

    def predict(end: int, owned: set[int], library: int = addr["area_at"], entries=listed, with_config: bool = False) -> dict:
        wanted = {a for a, _, _ in entries}
        swept = sorted((a, s) for a, s in spans if START <= a and a + s <= end)
        inside = {x for a, s in swept for x in range(a, a + s, 4)}
        between = [memory.get(x, 0) for x in range(START, end, 4) if x not in inside]
        area = lambda a: "game" if a < library else "library"  # noqa: E731
        unlisted = [(a, s) for a, s in swept if a not in wanted]
        rows, classes = {}, {}
        for a, s in unlisted:
            ref, counts = expected.get(name_at[a], (NONE, (0,) * 6))
            built = "yes" if a in owned else "no"
            rows[a] = f"{a:08x}\t{s}\t{area(a)}\t{built}\t{ref}\t" + "\t".join(map(str, counts))
            n, b = classes.get((area(a), built, ref), (0, 0))
            classes[area(a), built, ref] = (n + 1, b + s)
        table = [
            f" {k[0]:8} {k[1]:13} {k[2]:25} {n:9} {b:7}"
            for k, (n, b) in sorted(classes.items(), key=lambda kv: (kv[0][0], kv[0][1] == "yes", order.index(kv[0][2])))
        ]
        totals = {}
        for part in ("game", "library"):
            listed_n = sum(1 for a in wanted if area(a) == part)
            more = sum(1 for a, _ in unlisted if area(a) == part)
            built = sum(1 for a, _ in swept if area(a) == part and a in owned)
            tail = f"; in the build: {built}" if with_config else ""
            totals[part] = f"{part}: {listed_n} in the inventory and {more} not, {listed_n + more} together{tail}"
        return {
            "rows": rows,
            "table": " area     in the build  reference                 functions   bytes\n" + "\n".join(table) + "\n",
            "totals": totals,
            "swept": f"swept {START:#x} to {end:#x}, {end - START} bytes: {len(swept)} functions, {4 * len(inside)} bytes;"
            f" outside them {between.count(0)} zero words and {len(between) - between.count(0)} other words",
            "inventory": f"inventory: {len(wanted)} functions; starts that the sweep does not find: {len([a for a in wanted if a not in dict(swept)])}",
            "count": f"not in the inventory: {len(unlisted)} functions, {sum(s for _, s in unlisted)} bytes",
        }

    def unlisted(*args, directory: Path = mods, program_file: Path = exe, inv: Path = inventory, lib: int = library, pointer: int = pointers):
        return tool("pac.py", "unlisted", program_file, directory, "--pointers", hex(pointer), "--inventory", inv, "--library", hex(lib), *args)

    # No configuration: every function is out of the build, and no line is about the build.
    out0 = root / "unl-made" / "deeper" / "rows.tsv"
    run = unlisted("--out", out0)
    want = predict(inventory_end, set())
    yield "unlisted-parsed", run, 0, "4 archives parsed, 0 rejected; code-bearing chunks with distinct contents: 3"
    yield "unlisted-swept", run, 0, want["swept"]
    yield "unlisted-inventory", run, 0, want["inventory"]
    yield "unlisted-count", run, 0, want["count"]
    yield "unlisted-classes", run, 0, want["table"]
    yield "unlisted-game-total", run, 0, want["totals"]["game"] + "\n"
    yield "unlisted-library-total", run, 0, want["totals"]["library"] + "\n"
    yield "unlisted-no-build-lines", verdict("functions of the build" not in run.stdout and "in the build:" not in run.stdout, run.stdout), 0, "as required"
    yield "unlisted-out-folder-made", verdict(out0.exists(), "no file"), 0, "as required"
    rows = out0.read_text().splitlines() if out0.exists() else []
    by_address = {line.split("\t")[0]: line for line in rows}
    yield "unlisted-out-rows-in-order", verdict(rows == [want["rows"][a] for a in sorted(want["rows"])], "\n".join(rows)), 0, "as required"
    yield "unlisted-out-row-count", verdict(len(rows) == len(names), f"{len(rows)} rows"), 0, "as required"
    for name in names:
        a = addr[name]
        yield f"unlisted-row-{name}", verdict(by_address.get(f"{a:08x}") == want["rows"][a], f"{by_address.get(f'{a:08x}')!r} not {want['rows'][a]!r}"), 0, "as required"
    yield "unlisted-before-the-inventory-not-swept", verdict(f"{IMAGE:08x}" not in by_address, "swept"), 0, "as required"
    yield "unlisted-after-the-inventory-not-swept", verdict(f"{after:08x}" not in by_address, "swept"), 0, "as required"

    # The range: to the end of the inventory by default, to --end when given.
    run = unlisted("--end", hex(after_end), "--out", root / "unl-end.tsv")
    want_end = predict(after_end, set())
    yield "unlisted-end-swept", run, 0, want_end["swept"]
    yield "unlisted-end-count", run, 0, want_end["count"]
    yield "unlisted-end-classes", run, 0, want_end["table"]
    end_rows = (root / "unl-end.tsv").read_text().splitlines() if (root / "unl-end.tsv").exists() else []
    yield "unlisted-end-adds-the-function", verdict(f"{after:08x}\t16\tlibrary\tno\t{NONE}\t0\t0\t0\t0\t0\t0" in end_rows, str(end_rows[-1:])), 0, "as required"
    run = unlisted("--end", hex(image_end), "--out", root / "unl-whole.tsv")
    yield "unlisted-end-at-the-image-end", run, 0, f"swept {START:#x} to {image_end:#x}, {image_end - START} bytes:"
    yield "unlisted-end-past-the-image", unlisted("--end", hex(image_end + 4)), 1, "is not a range of words inside the image"
    yield "unlisted-end-zero", unlisted("--end", "0"), 1, f"the range to sweep, {START:#x} to 0x0, is not a range of words inside the image"
    yield "unlisted-end-not-on-a-word", unlisted("--end", hex(inventory_end + 2)), 1, "is not a range of words inside the image"
    yield "unlisted-end-at-the-start", unlisted("--end", hex(START)), 1, "is not a range of words inside the image"
    yield "unlisted-end-before-the-start", unlisted("--end", hex(START - 4)), 1, "is not a range of words inside the image"
    below = inventory_file("unl-below", [(IMAGE - 0x1000, 16, "x")])
    yield "unlisted-start-below-the-image", unlisted(inv=below), 1, "is not a range of words inside the image"
    odd = inventory_file("unl-odd", [(START + 2, 14, "x")])
    yield "unlisted-start-not-on-a-word", unlisted(inv=odd), 1, "is not a range of words inside the image"
    at_image = inventory_file("unl-at-image", [(IMAGE, 16, "before")])
    run = unlisted(inv=at_image)
    yield "unlisted-start-at-the-image", run, 0, f"swept {IMAGE:#x} to {IMAGE + 16:#x}, 16 bytes: 1 functions, 16 bytes;"

    # Inventories that do not fit.
    missing = inventory_file("unl-missing", [*listed, (addr["none"] + 0x20, 16, "gap")])
    run = unlisted(inv=missing)
    yield "unlisted-start-not-found", run, 1, f"inventory: {len(listed) + 1} functions; starts that the sweep does not find: 1"
    yield "unlisted-start-not-found-still-sorts", run, 1, predict(inventory_end, set())["count"]
    empty = root / "unl-empty.tsv"
    empty.write_text("nothing here\nnor here\n")
    run = unlisted(inv=empty)
    yield "unlisted-empty-inventory", run, 1, "the inventory is empty"
    yield "unlisted-empty-inventory-no-traceback", verdict("Traceback" not in run.stderr, run.stderr), 0, "as required"
    run = unlisted(inv=root / "unl-absent.tsv")
    yield "unlisted-no-inventory-file", run, 1, "No such file"
    yield "unlisted-no-inventory-file-no-traceback", verdict("Traceback" not in run.stderr, run.stderr), 0, "as required"
    for flag, value in (("--library", "x"), ("--inventory", "x"), ("--pointers", "x")):
        arguments = {"--pointers": hex(pointers), "--inventory": str(inventory), "--library": hex(library)}
        del arguments[flag]
        run = tool("pac.py", "unlisted", exe, mods, *[a for pair in arguments.items() for a in pair])
        yield f"unlisted-requires-{flag[2:]}", run, 2, f"the following arguments are required: {flag}"

    # The area: below --library is game, at it or above is library, and the start decides.
    straddle = addr["straddler"]
    run = unlisted("--library", hex(straddle + 0x10), "--out", root / "unl-straddle.tsv")
    want_lib = predict(inventory_end, set(), straddle + 0x10)
    straddle_rows = (root / "unl-straddle.tsv").read_text().splitlines() if (root / "unl-straddle.tsv").exists() else []
    yield "unlisted-straddling-function-is-game", verdict(f"{straddle:08x}\t36\tgame\tno\t{NONE}" in "\n".join(straddle_rows), "\n".join(straddle_rows[-6:])), 0, "as required"
    yield "unlisted-next-function-is-library", verdict(f"{addr['build_yes']:08x}\t16\tlibrary\t" in "\n".join(straddle_rows), "\n".join(straddle_rows[-6:])), 0, "as required"
    yield "unlisted-straddle-totals", run, 0, want_lib["totals"]["game"] + "\n"
    yield "unlisted-straddle-classes", run, 0, want_lib["table"]
    yield "unlisted-area-at-the-limit", verdict(by_address.get(f"{addr['area_at']:08x}", "").split("\t")[2:3] == ["library"] and by_address.get(f"{addr['area_below']:08x}", "").split("\t")[2:3] == ["game"], str(by_address.get(f"{addr['area_at']:08x}"))), 0, "as required"

    # The build configuration.
    outside = [0x80000000, addr["none"] + 4, after]
    config = root / "unl-build.toml"
    config.write_text(
        "[[unit]]\nname = \"resident\"\n"
        f"functions = [{{ name = \"a\", address = {addr['first']:#x} }}, {{ name = \"b\", address = {addr['build_yes']:#x} }}]\n"
        "[[unit]]\nname = \"overlay\"\nimage = \"overlay.bin\"\n"
        f"functions = [{{ name = \"c\", address = {addr['build_image']:#x} }}, {{ name = \"d\", address = {addr['pair_exe']:#x} }}]\n"
        "[[unit]]\nname = \"bare\"\n"
        "[[unit]]\nname = \"outside\"\n"
        f"functions = [{', '.join(f'{{ address = {a:#x} }}' for a in outside)}]\n"
    )
    owned = {addr["first"], addr["build_yes"], *outside}
    run = unlisted("--config", config, "--show", 2, "--out", root / "unl-config.tsv")
    want_cfg = predict(inventory_end, owned, library, with_config=True)
    yield "unlisted-config-outside-count", run, 0, "functions of the build that are no start of the sweep: 3\n"
    yield "unlisted-config-show-limit", run, 0, f"  {outside[0]:#x}\n  {outside[1]:#x}\n"
    yield "unlisted-config-show-limit-last", verdict(f"  {outside[2]:#x}" not in run.stdout, run.stdout), 0, "as required"
    yield "unlisted-config-classes", run, 0, want_cfg["table"]
    yield "unlisted-config-game-total", run, 0, want_cfg["totals"]["game"] + "\n"
    yield "unlisted-config-library-total", run, 0, want_cfg["totals"]["library"] + "\n"
    yield "unlisted-config-swept", run, 0, want_cfg["swept"]
    cfg_rows = (root / "unl-config.tsv").read_text().splitlines() if (root / "unl-config.tsv").exists() else []
    cfg_by = {line.split("\t")[0]: line for line in cfg_rows}
    for name in ("build_yes", "build_image", "pair_exe", "call_exe"):
        a = addr[name]
        yield f"unlisted-config-row-{name}", verdict(cfg_by.get(f"{a:08x}") == want_cfg["rows"][a], f"{cfg_by.get(f'{a:08x}')!r} not {want_cfg['rows'][a]!r}"), 0, "as required"
    yield "unlisted-config-whole-rows", verdict([cfg_by.get(f"{a:08x}") for a in sorted(want_cfg["rows"])] == [want_cfg["rows"][a] for a in sorted(want_cfg["rows"])], "rows differ"), 0, "as required"
    # A unit of the resident image may say so: `image = "resident"` and no key are the same.
    spelled = root / "unl-build-spelled.toml"
    spelled.write_text(config.read_text().replace('name = "resident"\n', 'name = "resident"\nimage = "resident"\n'))
    plain_run, spelled_run = unlisted("--config", config), unlisted("--config", spelled)
    yield "unlisted-config-resident-spelled-out", spelled_run, 0, want_cfg["totals"]["game"] + "\n"
    yield "unlisted-config-resident-spelled-out-same", verdict('image = "resident"' in spelled.read_text() and spelled_run.stdout == plain_run.stdout, spelled_run.stdout), 0, "as required"
    yield "unlisted-config-show-default", unlisted("--config", config), 0, f"  {outside[0]:#x}\n  {outside[1]:#x}\n  {outside[2]:#x}\n"
    run = unlisted("--config", config, "--end", hex(after_end), "--out", root / "unl-config-end.tsv")
    want_cfg_end = predict(after_end, owned, library, with_config=True)
    yield "unlisted-config-end-outside-count", run, 0, "functions of the build that are no start of the sweep: 2\n"
    yield "unlisted-config-end-library-total", run, 0, want_cfg_end["totals"]["library"] + "\n"
    yield "unlisted-config-end-classes", run, 0, want_cfg_end["table"]
    # A configuration without units declares nothing.
    nothing = root / "unl-nothing.toml"
    nothing.write_text("title = 'x'\n")
    run = unlisted("--config", nothing)
    yield "unlisted-config-without-units", run, 0, "functions of the build that are no start of the sweep: 0\n"
    yield "unlisted-config-without-units-total", run, 0, predict(inventory_end, set(), library, with_config=True)["totals"]["game"] + "\n"
    broken = root / "unl-broken.toml"
    broken.write_text("[[unit\nx")
    run = unlisted("--config", broken)
    yield "unlisted-config-broken", run, 1, "Expected ']]'"
    yield "unlisted-config-broken-no-traceback", verdict("Traceback" not in run.stderr, run.stderr), 0, "as required"
    for label, entry in (("without-address", '{ name = "a" }'), ("address-as-text", '{ address = "0x80110000" }'), ("address-as-truth", "{ address = true }"), ("no-table", "4")):
        odd_config = root / f"unl-{label}.toml"
        odd_config.write_text(f"[[unit]]\nname = \"resident\"\nfunctions = [{entry}]\n")
        run = unlisted("--config", odd_config)
        yield f"unlisted-config-function-{label}", run, 1, "a function of a unit has no integer address"
        yield f"unlisted-config-function-{label}-no-traceback", verdict("Traceback" not in run.stderr, run.stderr), 0, "as required"
    run = unlisted("--config", root / "unl-absent.toml")
    yield "unlisted-config-missing", run, 1, "No such file"
    yield "unlisted-config-missing-no-traceback", verdict("Traceback" not in run.stderr, run.stderr), 0, "as required"

    # The modules: a rejected archive, a destination that is not a multiple of four, an executable that does not fit.
    damaged = bytearray(make_archive([(4, module_bytes)]))
    damaged[USER_DATA] ^= 1
    bad = folder("unl-bad", A=bytes(make_archive([(4, module_bytes)])), B=bytes(damaged))
    run = unlisted(directory=bad)
    yield "unlisted-rejected-archive", run, 1, "1 archives parsed, 1 rejected; code-bearing chunks with distinct contents: 1"
    yield "unlisted-rejected-still-sorts", run, 1, want["count"]
    askew, askew_pointers = make_program(IMAGE, code, [[0x80300000] * 4 + [SLOT_BASE + 2], [0x80400000], [0x80500000]])
    (root / "UNL-ASKEW.EXE").write_bytes(askew)
    yield "unlisted-destination-not-a-multiple-of-four", unlisted(program_file=root / "UNL-ASKEW.EXE", pointer=askew_pointers), 1, (
        f"slot 0x4: the destination {SLOT_BASE + 2:#x} is not a multiple of four"
    )
    run = unlisted(program_file=mods / "A.PAC")
    yield "unlisted-not-an-executable", run, 1, "not a PS-X executable"
    yield "unlisted-not-an-executable-no-traceback", verdict("Traceback" not in run.stderr, run.stderr), 0, "as required"
    yield "unlisted-no-pointer-block", unlisted(pointer=IMAGE), 1, "no block of table addresses"

    # An address is written with eight digits.
    low_code = [OPEN, ONE, RETURN, CLOSE] * 2
    low_exe, low_pointers = make_program(0x00100000, low_code, [[0x00300000] * 4 + [0x00200000], [0x00400000], [0x00500000]])
    (root / "UNL-LOW.EXE").write_bytes(low_exe)
    low_mods = folder("unl-low", A=bytes(make_archive([(4, make_code(0x00200000))])))
    low_out = root / "unl-low.tsv"
    unlisted("--end", "0x00100020", "--out", low_out, program_file=root / "UNL-LOW.EXE", directory=low_mods, pointer=low_pointers, lib=0x00100000, inv=inventory_file("unl-low-inventory", [(0x00100000, 16, "x")]))
    low_rows = low_out.read_text().splitlines() if low_out.exists() else []
    yield "unlisted-out-low-address", verdict(low_rows == [f"00100010\t16\tlibrary\tno\t{NONE}\t0\t0\t0\t0\t0\t0"], str(low_rows)), 0, "as required"

    # The upper four bits of a call's target are those of its delay slot's address. A `jal` in the last
    # word below 0x90000000 calls a function above; one word earlier it calls a function below.
    edge = 0x8FFFFFC0
    near, beyond = edge + 4 * 4, edge + 4 * 20
    for label, padding, target in (("across", 6, beyond), ("below", 5, near)):
        caller = [OPEN, *[ONE] * padding, jal(target), 0, RETURN, CLOSE]
        edge_code = [*plain, *plain, *caller, *[0] * (12 - len(caller)), *plain]
        assert edge + 4 * edge_code.index(jal(target)) == (0x8FFFFFFC if label == "across" else 0x8FFFFFF8)
        edge_exe, edge_pointers = make_program(edge, edge_code, [[0x00300000] * 4 + [0x00200000], [0x00400000], [0x00500000]])
        (root / f"UNL-EDGE-{label}.EXE").write_bytes(edge_exe)
        edge_out = root / f"unl-edge-{label}.tsv"
        unlisted(
            "--end", hex(edge + 4 * len(edge_code)), "--out", edge_out, program_file=root / f"UNL-EDGE-{label}.EXE", directory=low_mods,
            pointer=edge_pointers, lib=beyond, inv=inventory_file(f"unl-edge-{label}", [(edge, 16, "first"), (edge + 32, 4 * len(caller), "caller")]),
        )  # fmt: skip
        edge_rows = edge_out.read_text().splitlines() if edge_out.exists() else []
        called = lambda a: f"{EXE}\t1" if a == target else f"{NONE}\t0"  # noqa: E731
        wanted_rows = [f"{near:08x}\t16\tgame\tno\t{called(near)}\t0\t0\t0\t0\t0", f"{beyond:08x}\t16\tlibrary\tno\t{called(beyond)}\t0\t0\t0\t0\t0"]
        yield f"unlisted-call-{label}-a-region", verdict(edge_rows == wanted_rows, str(edge_rows)), 0, "as required"

    # Entries of the modules: a function whose start is a symbol is told from the words before it.
    glue = [addr["symbol_word"], ONE, 0x3C028020, 0x8C420000, OPEN, ONE, RETURN, CLOSE]
    glued = b"".join(struct.pack("<I", w) for w in [*[w for _ in range(8) for w in plain], STOP, *glue])
    symbolic = folder("unl-glue", A=bytes(make_archive([(4, glued)])))
    symbols = root / "unl-symbols.ld"
    symbols.write_text(f"entry = {SLOT_BASE + 4 * (32 + 1 + 2):#x};\n")
    for label, extra, count in (("without-symbols", [], 0), ("with-symbols", ["--symbols", symbols], 1)):
        made = root / f"unl-glue-{label}.tsv"
        run = unlisted(*extra, "--out", made, directory=symbolic)
        found = {line.split("\t")[0]: line.split("\t") for line in made.read_text().splitlines()} if made.exists() else {}
        columns = found.get(f"{addr['symbol_word']:08x}", [])
        yield f"unlisted-glue-{label}", verdict(columns[8:9] == [str(count)] and columns[4:5] == [WORD if count else NONE], str(columns)), 0, "as required"


def main() -> int:
    failed = 0
    with tempfile.TemporaryDirectory() as tmp:
        root = Path(tmp)
        for name, proc, want_status, want_text in [*baseline_cases(root), *extract_safety_cases(root), *pac_cases(root), *function_cases(root), *unlisted_cases(root)]:
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
