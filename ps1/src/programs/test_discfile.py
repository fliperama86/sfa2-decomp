#!/usr/bin/env python3
"""Controls for discfile.py on a disc image that this test builds (no private input).

    python test_discfile.py

The made-up image has raw 2352-byte sectors in mode 2 form 1 (data at byte 24), an ISO 9660 volume
descriptor at sector 16, a root directory at sector 17 that holds a directory `PAC` and a file
`TOP.TXT;1`, `PAC` at sector 18 with `ONE.EXE;1` and `TWO.EXE;1`, and the files at sectors 19 to
22. ONE.EXE holds 3,000 bytes (two sectors) and TWO.EXE 5 bytes. Prints one line per case, then
`all cases behaved as required` (status 0) or a count of failures (status 1). A run takes well
under a second.
"""

from __future__ import annotations

import hashlib
import io
import contextlib
import struct
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import discfile as D  # noqa: E402

ONE = bytes((i * 7 + 3) & 0xFF for i in range(3000))
TWO = b"hello"
TOP = b"top"


def sector(data: bytes, mode: int = 2, form2: bool = False, sync: bytes = D.SYNC) -> bytes:
    if mode == 2:
        body = sync + b"\x00\x02\x00" + bytes([2]) + bytes([0, 0, 0x20 if form2 else 0, 0]) * 2 + data.ljust(2048, b"\0")
    else:
        body = sync + b"\x00\x02\x00" + bytes([1]) + data.ljust(2048, b"\0")
    return body.ljust(D.SECTOR, b"\0")


def record(name: str, block: int, size: int, directory: bool) -> bytes:
    raw = name.encode()
    body = bytearray(33 + len(raw))
    body[2:6] = struct.pack("<I", block)
    body[10:14] = struct.pack("<I", size)
    body[25] = 2 if directory else 0
    body[32] = len(raw)
    body[33:] = raw
    if len(body) % 2:
        body.append(0)
    body[0] = len(body)
    return bytes(body)


def image(mode: int = 2, form2_at: int | None = None, sync_bad_at: int | None = None, volume: bytes = b"CD001") -> bytes:
    root = record("\0", 17, 2048, True) + record("PAC", 18, 2048, True) + record("TOP.TXT;1", 22, len(TOP), False)
    pac = record("\0", 18, 2048, True) + record("ONE.EXE;1", 19, len(ONE), False) + record("TWO.EXE;1", 21, len(TWO), False)
    descriptor = bytearray(2048)
    descriptor[1:6] = volume
    descriptor[156:156 + len(record("\0", 17, 2048, True))] = record("\0", 17, 2048, True)
    descriptor[156 + 2:156 + 6] = struct.pack("<I", 17)
    descriptor[156 + 10:156 + 14] = struct.pack("<I", 2048)
    content = {16: bytes(descriptor), 17: root, 18: pac, 19: ONE[:2048], 20: ONE[2048:], 21: TWO, 22: TOP}
    sectors = []
    for number in range(23):
        sectors.append(sector(content.get(number, b""), mode, form2=(number == form2_at),
                              sync=b"\x01" * 12 if number == sync_bad_at else D.SYNC))
    return b"".join(sectors)


def run(disc: Path, path: str, out: Path):
    stdout, stderr = io.StringIO(), io.StringIO()
    with contextlib.redirect_stdout(stdout), contextlib.redirect_stderr(stderr):
        status = D.main([str(disc), path, str(out)])
    return status, stdout.getvalue(), stderr.getvalue()


def with_image(data: bytes, test):
    with tempfile.TemporaryDirectory() as tmp:
        disc = Path(tmp) / "disc.bin"
        disc.write_bytes(data)
        return test(Path(tmp), disc)


def refused_with(data: bytes, path: str, needle: str):
    def test(tmp, disc):
        out = tmp / "out.bin"
        status, stdout, stderr = run(disc, path, out)
        ok = status == 2 and needle in stderr and stdout == "" and not out.exists()
        return None if ok else f"{status} {stdout!r} {stderr!r} out exists {out.exists()}"
    return with_image(data, test)


def case_reads_a_file_in_a_directory():
    def test(tmp, disc):
        out = tmp / "out.bin"
        status, stdout, stderr = run(disc, "PAC/ONE.EXE", out)
        want = f"PAC/ONE.EXE: 3000 bytes, sha256 {hashlib.sha256(ONE).hexdigest()}\n"
        return None if (status, stdout, stderr) == (0, want, "") and out.read_bytes() == ONE else f"{status} {stdout!r} {stderr!r}"
    return with_image(image(), test)


def case_reads_the_other_files():
    def test(tmp, disc):
        got = []
        for number, path in enumerate(("PAC/TWO.EXE", "TOP.TXT", "/PAC/TWO.EXE")):
            D.extract(disc, path, tmp / f"o{number}")  # each into a file of its own: the tool writes over nothing
            got.append((tmp / f"o{number}").read_bytes())
        return None if got == [TWO, TOP, TWO] else f"got {got}"
    return with_image(image(), test)


def case_mode_one_sectors():
    def test(tmp, disc):
        D.extract(disc, "PAC/ONE.EXE", tmp / "o")
        return None if (tmp / "o").read_bytes() == ONE else "wrong bytes"
    return with_image(image(mode=1), test)


def case_cue_names_the_image():
    with tempfile.TemporaryDirectory() as tmp:
        (Path(tmp) / "my disc.bin").write_bytes(image())
        (Path(tmp) / "d.cue").write_text('FILE "my disc.bin" BINARY\n  TRACK 01 MODE2/2352\n    INDEX 01 00:00:00\n')
        D.extract(Path(tmp) / "d.cue", "PAC/TWO.EXE", Path(tmp) / "o")
        return None if (Path(tmp) / "o").read_bytes() == TWO else "wrong bytes"


def case_cue_without_a_file_or_with_another_type_or_track():
    with tempfile.TemporaryDirectory() as tmp:
        (Path(tmp) / "disc.bin").write_bytes(image())
        wrong = {"TRACK 01 MODE2/2352\n": "names no FILE",
                 'FILE "disc.bin" WAVE\n  TRACK 01 AUDIO\n': "not BINARY",
                 'FILE "disc.bin" BINARY\n  TRACK 01 AUDIO\n': "not MODE1/2352 or MODE2/2352",
                 'FILE "disc.bin" BINARY\n': "not MODE1/2352 or MODE2/2352",
                 'FILE "nowhere.bin" BINARY\n  TRACK 01 MODE2/2352\n': "nowhere.bin cannot be read"}
        for text, needle in wrong.items():
            (Path(tmp) / "d.cue").write_text(text)
            status, _, err = run(Path(tmp) / "d.cue", "PAC/TWO.EXE", Path(tmp) / "o")
            if status != 2 or needle not in err or (Path(tmp) / "o").exists():
                return f"{text!r}: {status} {err!r}"
    return None


def case_path_not_on_the_disc():
    one = refused_with(image(), "PAC/THREE.EXE", "PAC/THREE.EXE is not on the disc (THREE.EXE is not in PAC)")
    two = refused_with(image(), "NOPE/ONE.EXE", "NOPE/ONE.EXE is not on the disc (NOPE is not in the root)")
    three = refused_with(image(), "pac/ONE.EXE", "is not on the disc")  # names are compared as written
    return one or two or three


def case_directory_as_a_file_and_file_as_a_directory():
    return (refused_with(image(), "PAC", "PAC: PAC is a directory")
            or refused_with(image(), "TOP.TXT/ONE.EXE", "TOP.TXT is a file")
            or refused_with(image(), "", "names no file"))


def case_image_of_the_wrong_form():
    good = image()
    return (refused_with(good[:-1], "TOP.TXT", "not a whole number of raw 2352-byte sectors")
            or refused_with(b"", "TOP.TXT", "not a whole number")
            or refused_with(good[: 16 * D.SECTOR], "TOP.TXT", "is past the image's 16")
            or refused_with(image(volume=b"XXXXX"), "TOP.TXT", "not an ISO 9660 volume descriptor")
            or refused_with(image(sync_bad_at=16), "TOP.TXT", "sector 16 has no sync pattern")
            or refused_with(image(sync_bad_at=22), "TOP.TXT", "sector 22 has no sync pattern")
            or refused_with(image(form2_at=22), "TOP.TXT", "sector 22 is Form 2"))


def case_file_past_the_image_is_refused():
    data = image()
    return refused_with(data[: 20 * D.SECTOR], "PAC/ONE.EXE", "is past the image's 20")


def case_arguments_and_unwritable_output():
    stdout, stderr = io.StringIO(), io.StringIO()
    with contextlib.redirect_stdout(stdout), contextlib.redirect_stderr(stderr):
        status = D.main(["a", "b"])
    if (status, stdout.getvalue()) != (2, "") or "usage" not in stderr.getvalue():
        return f"{status} {stderr.getvalue()!r}"
    def test(tmp, disc):
        status, _, err = run(disc, "TOP.TXT", tmp / "no-such-folder" / "o")
        return None if status == 2 and "cannot be written" in err else f"{status} {err!r}"
    return with_image(image(), test)


CUE = 'FILE "disc.bin" BINARY\n  TRACK 01 MODE2/2352\n    INDEX 01 00:00:00\n'


def snapshot(tmp: Path) -> dict:
    """What a folder holds: for each name, a file's bytes (read through a link), a folder's names, or where a link
    that leads nowhere points."""
    import os
    found = {}
    for entry in sorted(tmp.iterdir()):
        if entry.is_symlink() and not entry.exists():
            found[entry.name] = ("link to nothing", os.readlink(entry))
        elif entry.is_dir():
            found[entry.name] = ("folder", sorted(e.name for e in entry.iterdir()))
        else:
            found[entry.name] = ("file", entry.is_symlink(), entry.read_bytes())
    return found


def output_refused(make, disc_name: str, out_name: str, needle: str):
    """A run whose OUT is `out_name` after `make(tmp)` has prepared the folder: refused with `needle`, status 2,
    nothing printed, and everything in the folder as before, byte for byte (the inputs above all)."""
    with tempfile.TemporaryDirectory() as folder:
        tmp = Path(folder)
        (tmp / "disc.bin").write_bytes(image())
        (tmp / "d.cue").write_text(CUE)
        make(tmp)
        before = snapshot(tmp)
        status, stdout, stderr = run(tmp / disc_name, "PAC/TWO.EXE", tmp / out_name)
        if (status, stdout) != (2, "") or needle not in stderr:
            return f"{out_name}: {status} {stdout!r} {stderr!r}"
        after = snapshot(tmp)
        if after != before:
            return f"{out_name}: the folder changed: {sorted(n for n in set(after) | set(before) if after.get(n) != before.get(n))}"
        if before["disc.bin"] != ("file", False, image()) or before["d.cue"] != ("file", False, CUE.encode()):
            return f"{out_name}: the inputs were not what the case made"
    return None


def case_output_that_is_the_image_is_refused_and_the_image_kept():
    # OUT is the image itself: by its own name, with the image given directly and through a cue.
    return (output_refused(lambda tmp: None, "disc.bin", "disc.bin", "is the disc file given itself")
            or output_refused(lambda tmp: None, "d.cue", "disc.bin", "is the disc's image itself"))


def case_output_that_is_the_cue_is_refused_and_the_cue_kept():
    return output_refused(lambda tmp: None, "d.cue", "d.cue", "is the disc file given itself")


def case_output_that_is_a_link_to_an_input_is_refused():
    # A symbolic link and a second name (hard link) of the image, and a symbolic link to the cue.
    import os
    return (output_refused(lambda tmp: os.symlink(tmp / "disc.bin", tmp / "s"), "disc.bin", "s", "is the disc file given itself")
            or output_refused(lambda tmp: os.link(tmp / "disc.bin", tmp / "h"), "disc.bin", "h", "is the disc file given itself")
            or output_refused(lambda tmp: os.symlink(tmp / "disc.bin", tmp / "s"), "d.cue", "s", "is the disc's image itself")
            or output_refused(lambda tmp: os.link(tmp / "disc.bin", tmp / "h"), "d.cue", "h", "is the disc's image itself")
            or output_refused(lambda tmp: os.symlink(tmp / "d.cue", tmp / "s"), "d.cue", "s", "is the disc file given itself")
            or output_refused(lambda tmp: os.link(tmp / "d.cue", tmp / "h"), "d.cue", "h", "is the disc file given itself"))


def case_output_that_exists_is_refused_and_kept():
    # Any other thing at OUT: a file, a folder, a symbolic link to another file, a link that leads nowhere.
    import os
    return (output_refused(lambda tmp: (tmp / "old").write_bytes(b"keep me"), "disc.bin", "old", "exists")
            or output_refused(lambda tmp: (tmp / "dir").mkdir(), "disc.bin", "dir", "exists")
            or output_refused(lambda tmp: ((tmp / "old").write_bytes(b"keep me"), os.symlink(tmp / "old", tmp / "s")),
                              "disc.bin", "s", "exists")
            or output_refused(lambda tmp: os.symlink(tmp / "nowhere", tmp / "s"), "disc.bin", "s", "exists"))


def case_existing_output_is_refused_before_anything_is_read():
    # The image is not an image at all (5 bytes); the refusal is about OUT all the same, so the check of OUT comes
    # before any reading. For a file at OUT and for a link at OUT that leads nowhere.
    import os
    for make in (lambda tmp: (tmp / "o").write_bytes(b"keep me"), lambda tmp: os.symlink(tmp / "nowhere", tmp / "o")):
        with tempfile.TemporaryDirectory() as folder:
            tmp = Path(folder)
            (tmp / "disc.bin").write_bytes(b"not a")
            make(tmp)
            before = snapshot(tmp)
            status, stdout, stderr = run(tmp / "disc.bin", "PAC/TWO.EXE", tmp / "o")
            if (status, stdout) != (2, "") or "exists" not in stderr or "sectors" in stderr or snapshot(tmp) != before:
                return f"{status} {stdout!r} {stderr!r}"
    return None


def case_second_run_does_not_replace_the_first_output():
    def test(tmp, disc):
        out = tmp / "out.bin"
        first = run(disc, "PAC/TWO.EXE", out)
        second = run(disc, "PAC/ONE.EXE", out)  # another file of the disc to the same OUT
        ok = first[0] == 0 and second[0] == 2 and "exists" in second[2] and second[1] == "" and out.read_bytes() == TWO
        return None if ok and disc.read_bytes() == image() else f"{first} {second}"
    return with_image(image(), test)


def case_exclusive_creation_is_the_guard():
    # The check before the read is not what protects: with it taken out, the creation itself refuses, for the
    # image under its own name, under a second name and behind a symbolic link, and the image stays as it was.
    import os
    def test(tmp, disc):
        os.link(disc, tmp / "h")
        os.symlink(disc, tmp / "s")
        real = D.refuse_existing
        D.refuse_existing = lambda out, sources: None
        try:
            for name in ("disc.bin", "h", "s"):
                status, stdout, stderr = run(disc, "PAC/TWO.EXE", tmp / name)
                # the tool's own line for it, not the system's words for a failed open
                if (status, stdout) != (2, "") or "exists: the tool does not write over a file" not in stderr \
                        or disc.read_bytes() != image():
                    return f"{name}: {status} {stdout!r} {stderr!r}"
        finally:
            D.refuse_existing = real
        return None
    return with_image(image(), test)


def case_write_that_fails_part_way_removes_nothing():
    class Failing:
        def __init__(self, handle):
            self.handle = handle
        def __enter__(self):
            return self
        def __exit__(self, *exc):
            self.handle.close()
            return False
        def write(self, data):
            self.handle.write(data[:2])
            raise OSError(28, "No space left on device")

    def test(tmp, disc):
        out = tmp / "out.bin"
        real = open
        D.open = lambda path, mode="r", *a, **k: Failing(real(path, mode, *a, **k)) if mode == "xb" else real(path, mode, *a, **k)
        try:
            status, stdout, stderr = run(disc, "PAC/TWO.EXE", out)
        finally:
            del D.open
        ok = status == 2 and stdout == "" and "not written completely" in stderr and out.read_bytes() == TWO[:2]
        return None if ok and disc.read_bytes() == image() else f"{status} {stdout!r} {stderr!r}"
    return with_image(image(), test)


def case_missing_image_is_refused():
    with tempfile.TemporaryDirectory() as tmp:
        status, _, err = run(Path(tmp) / "none.bin", "TOP.TXT", Path(tmp) / "o")
        return None if status == 2 and "none.bin cannot be read" in err else f"{status} {err!r}"


CASES = [(name[5:].replace("_", "-"), fn) for name, fn in sorted(globals().items()) if name.startswith("case_")]


def main() -> int:
    failed = 0
    for name, case in CASES:
        try:
            why = case()
        except Exception as exc:  # a case that raises is a case that fails
            why = f"raised {type(exc).__name__}: {exc}"
        if why:
            failed += 1
            print(f"FAIL {name}: {why}")
        else:
            print(f"ok {name}")
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
