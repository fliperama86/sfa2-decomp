#!/usr/bin/env python3
"""Read one file out of the user's disc image.

    python discfile.py DISC PATH OUT

DISC is the `.bin` of raw 2352-byte sectors, or a `.cue` that names it (the first FILE line is
taken; the track after it must be MODE1/2352 or MODE2/2352). PATH is the file's path on the disc
(ISO 9660, `PAC/LOGO.EXE`; names are compared without the `;1` version suffix, case as written).
The file is written to OUT, and the size and the SHA-256 of what was written are printed:

    PAC/LOGO.EXE: 129024 bytes, sha256 HEX

It reads the file's own sectors and the directories on the way: milliseconds to a second. A path
that is not on the disc, a directory given as a file, an image whose size is not a whole number
of sectors, a sector without the sync pattern, a disc without the ISO 9660 volume descriptor, a
cue that names no readable image, and a file stored in Form 2 sectors (2,324 bytes of data, no
checksum; a program is never stored so) are refused, status 2, nothing written. OUT is not
written when anything is refused.

The tool never writes over a file. OUT must be a path at which nothing is: the file is made with
exclusive creation, which the system refuses for any path that exists (a file, a folder, a
symbolic link whether it leads somewhere or not). So no input can be overwritten, under whatever
name OUT reaches it: DISC itself, the image a cue names, a symbolic link or a second name (hard
link) of either. Before anything is read, an OUT that is the same file as DISC or as the image
is refused with a line that says so, and any other OUT that exists with a line that says that;
the exclusive creation is the guard that holds even if the path changes in between. To take a
file out again, remove the old output first or name another. If the system fails in the middle
of the write, the part that was written stays under OUT and the line says so (the tool removes
nothing, ever).
"""

from __future__ import annotations

import hashlib
import os
import re
import struct
import sys
from pathlib import Path

SECTOR = 2352
SYNC = b"\x00" + b"\xff" * 10 + b"\x00"
VOLUME_DESCRIPTOR = 16


class DiscError(Exception):
    pass


def image_path(disc: Path) -> Path:
    """The `.bin` that DISC is, or that the `.cue` DISC names."""
    if disc.suffix.lower() != ".cue":
        return disc
    try:
        lines = disc.read_text(errors="replace").splitlines()
    except OSError as exc:
        raise DiscError(f"{disc.name} cannot be read: {exc.strerror or exc}") from exc
    for number, line in enumerate(lines):
        found = re.match(r'\s*FILE\s+(?:"([^"]+)"|(\S+))\s+(\S+)', line, re.IGNORECASE)
        if found is None:
            continue
        if found.group(3).upper() != "BINARY":
            raise DiscError(f"{disc.name}: the first FILE is of type {found.group(3)}, not BINARY")
        track = next((re.match(r"\s*TRACK\s+\d+\s+(\S+)", later, re.IGNORECASE) for later in lines[number + 1:]
                      if re.match(r"\s*TRACK\s", later, re.IGNORECASE)), None)
        if track is None or track.group(1).upper() not in ("MODE1/2352", "MODE2/2352"):
            raise DiscError(f"{disc.name}: the first track is not MODE1/2352 or MODE2/2352")
        return disc.parent / (found.group(1) or found.group(2))
    raise DiscError(f"{disc.name} names no FILE")


class Image:
    """A raw image: sectors of 2352 bytes, the first one is logical block 0."""

    def __init__(self, path: Path):
        try:
            self.handle = open(path, "rb")
        except OSError as exc:
            raise DiscError(f"{path.name} cannot be read: {exc.strerror or exc}") from exc
        self.name = path.name
        self.handle.seek(0, 2)
        size = self.handle.tell()
        if size == 0 or size % SECTOR:
            self.handle.close()
            raise DiscError(f"{path.name}: {size} bytes is not a whole number of raw {SECTOR}-byte sectors")
        self.sectors = size // SECTOR

    def close(self) -> None:
        self.handle.close()

    def user_data(self, block: int) -> bytes:
        """The 2,048 bytes of data of one sector."""
        if not 0 <= block < self.sectors:
            raise DiscError(f"{self.name}: sector {block} is past the image's {self.sectors}")
        self.handle.seek(block * SECTOR)
        raw = self.handle.read(SECTOR)
        if raw[:12] != SYNC:
            raise DiscError(f"{self.name}: sector {block} has no sync pattern")
        mode = raw[15]
        if mode == 1:
            return raw[16:16 + 2048]
        if mode == 2:
            if raw[18] & 0x20:
                raise DiscError(f"{self.name}: sector {block} is Form 2")
            return raw[24:24 + 2048]
        raise DiscError(f"{self.name}: sector {block} is of mode {mode}")

    def extent(self, block: int, size: int) -> bytes:
        return b"".join(self.user_data(block + i) for i in range((size + 2047) // 2048))[:size]


def records(image: Image, block: int, size: int) -> list[tuple[str, int, int, bool]]:
    """The entries (name, block, size, is a directory) of the directory at `block`."""
    data = image.extent(block, size)
    found = []
    at = 0
    while at < len(data):
        length = data[at]
        if length == 0:
            at = (at // 2048 + 1) * 2048
            continue
        record = data[at:at + length]
        if length < 34 or len(record) < length or 33 + record[32] > length:
            raise DiscError(f"{image.name}: a directory record at {at} of the directory at sector {block} is malformed")
        start, = struct.unpack("<I", record[2:6])
        file_size, = struct.unpack("<I", record[10:14])
        name = record[33:33 + record[32]].decode("latin1")
        found.append((name.split(";")[0], start, file_size, bool(record[25] & 2)))
        at += length
    return found


def read_file(image: Image, path: str) -> bytes:
    parts = [p for p in path.split("/") if p]
    if not parts:
        raise DiscError("the path names no file")
    descriptor = image.user_data(VOLUME_DESCRIPTOR)
    if descriptor[1:6] != b"CD001":
        raise DiscError(f"{image.name}: sector {VOLUME_DESCRIPTOR} is not an ISO 9660 volume descriptor")
    block, size = struct.unpack("<I", descriptor[158:162])[0], struct.unpack("<I", descriptor[166:170])[0]
    for number, part in enumerate(parts):
        entry = next((e for e in records(image, block, size) if e[0] == part), None)
        if entry is None:
            raise DiscError(f"{path} is not on the disc ({part} is not in {'/'.join(parts[:number]) or 'the root'})")
        _, block, size, directory = entry
        if directory != (number < len(parts) - 1):
            raise DiscError(f"{path}: {part} is {'a directory' if directory else 'a file'}")
    return image.extent(block, size)


def same_file(first: Path, second: Path) -> bool:
    """True when the two paths lead to one file (the same path, a symbolic link, a second name of the file).
    A path at which nothing is, or that cannot be looked at, is the same file as nothing."""
    try:
        return os.path.samefile(first, second)
    except OSError:
        return False


def refuse_existing(out: Path, sources: tuple[tuple[Path, str], ...]) -> None:
    """Refuse an OUT at which something is, saying so when it is one of the inputs."""
    for source, what in sources:
        if same_file(out, source):
            raise DiscError(f"{out.name} is {what} itself: the tool does not write over its input")
    if os.path.lexists(out):
        raise DiscError(f"{out.name} exists: the tool does not write over a file; remove it first or name another")


def write_new(out: Path, data: bytes) -> None:
    """Make the file `out` and write `data`. A path at which anything is, is never opened for writing: the
    system refuses the exclusive creation. Nothing is removed if the write fails part of the way."""
    try:
        handle = open(out, "xb")
    except FileExistsError:
        raise DiscError(f"{out.name} exists: the tool does not write over a file; remove it first or name another") from None
    except OSError as exc:
        raise DiscError(f"{out.name} cannot be written: {exc.strerror or exc}") from exc
    try:
        with handle:
            handle.write(data)
    except OSError as exc:
        raise DiscError(f"{out.name} was not written completely ({exc.strerror or exc}): what is there is a part, "
                        f"remove it") from exc


def extract(disc: Path, path: str, out: Path) -> str:
    """Write the file `path` of the disc to the new file `out`; the line that says what was written."""
    source = image_path(disc)
    refuse_existing(out, ((disc, "the disc file given"), (source, "the disc's image")))
    image = Image(source)
    try:
        data = read_file(image, path)
    finally:
        image.close()
    write_new(out, data)
    return f"{path}: {len(data)} bytes, sha256 {hashlib.sha256(data).hexdigest()}"


def main(argv: list[str] | None = None) -> int:
    argv = sys.argv[1:] if argv is None else argv
    if len(argv) != 3:
        print("usage: discfile.py DISC PATH OUT", file=sys.stderr)
        return 2
    try:
        print(extract(Path(argv[0]), argv[1], Path(argv[2])))
    except DiscError as exc:
        print(f"INPUT ERROR: {exc}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main())
