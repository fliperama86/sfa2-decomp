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


def main() -> int:
    failed = 0
    with tempfile.TemporaryDirectory() as tmp:
        root = Path(tmp)
        for name, proc, want_status, want_text in [*baseline_cases(root), *extract_safety_cases(root), *pac_cases(root), *function_cases(root)]:
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
