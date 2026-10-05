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


def main() -> int:
    failed = 0
    with tempfile.TemporaryDirectory() as tmp:
        root = Path(tmp)
        for name, proc, want_status, want_text in [*baseline_cases(root), *extract_safety_cases(root), *pac_cases(root)]:
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
