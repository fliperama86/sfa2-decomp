#!/usr/bin/env python3
"""Controls for baseline.py and pac.py using synthetic inputs.

Builds a tiny fake disc image and fake chunk archives in a temporary
directory. No game data is involved.
"""

from __future__ import annotations

import hashlib
import json
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


def main() -> int:
    failed = 0
    with tempfile.TemporaryDirectory() as tmp:
        root = Path(tmp)
        for name, proc, want_status, want_text in [*baseline_cases(root), *pac_cases(root)]:
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
