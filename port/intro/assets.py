#!/usr/bin/env python3
"""Decode twelve background pages from the supplied Saturn JP opening files.

Reads only OPENING.BIN and OPENING.Z, extracted from the owner's disc. About
one second, Python standard library only; no CPU execution, build or game.
All binary-derived output is restricted to this checkout's port/local/.
See README.md for evidence, profile assumptions and exclusions.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import zlib

ROOT = Path(__file__).resolve().parents[2]
BASE = 0x06070000
PATTERN_BASE = 0x20000
PINS = {
    'OPENING.BIN': (0x119800, '4639290195050338ee299b699fdc962f7d42fdfc34ce288cbab500da370c83d8'),
    'OPENING.Z': (0x32140, 'e42a5ce647ecfb87665341c1e8b62a8b91128779dabc4bb7cc06d047e6ebb6a6'),
}
# Project-authored interpretation of the first setup's calls, not game tables.
# The three labels describe decoded art, not proven screen ordering.
LAYERS = (
    ('logo', 0x0609583c, (4, 5), 8),
    ('fire', 0x0609b85c, tuple(range(2, 10)), 16),
    ('portrait', 0x0609f4bc, (2, 3), 16),
)
PALETTE_LOADS = ((0x06079f00, 0), (0x06077a76, 0x200),
                 (0x0607803a, 0x400), (0x060781e2, 0x600))


class InputError(ValueError):
    """Unsupported revision, malformed data or unsafe output request."""


def checked(data: bytes, offset: int, size: int) -> bytes:
    if offset < 0 or size < 0 or offset + size > len(data):
        raise InputError(f'read outside input: offset {offset}, size {size}')
    return data[offset:offset + size]


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def pinned(data: bytes, name: str) -> None:
    size, sha = PINS[name]
    if len(data) != size or digest(data) != sha:
        raise InputError(f'{name}: unsupported revision or altered input')


def palettes(code: bytes) -> list[int]:
    result = [0] * 2048
    for address, first in PALETTE_LOADS:
        for count in range(len(result) - first + 1):
            color = struct.unpack('>H', checked(code, address - BASE + count * 2, 2))[0]
            if color == 0xffff:
                break
            if first + count >= len(result):
                raise InputError('unterminated palette or color RAM overflow')
            result[first + count] = color
    return result


def pattern(pixels: bytes, character: int, size: int) -> bytes:
    """Saturn 4bpp character, 8x8 or four 8x8 cells in TL/TR/BL/BR order."""
    if size not in (8, 16) or not 0 <= character < 0x8000:
        raise InputError('unsupported character size or number')
    if size == 16 and character % 4:
        raise InputError('16x16 character is not four-cell aligned')
    start = character * 32 - PATTERN_BASE
    data = checked(pixels, start, size * size // 2)
    out = bytearray(size * size)
    for y in range(size):
        for x in range(size):
            cell = (y // 8) * (size // 8) + x // 8
            packed = data[cell * 32 + (y % 8) * 4 + (x % 8) // 2]
            out[y * size + x] = (packed >> (0 if x % 2 else 4)) & 15
    return bytes(out)


def page(code: bytes, pixels: bytes, palette: list[int], address: int,
         size: int) -> tuple[bytes, list[dict]]:
    """Decode a 256x256 uploaded rectangle, not an entire VDP2 map/page."""
    if size not in (8, 16) or len(palette) != 2048:
        raise InputError('invalid page format or palette size')
    side = 256 // size
    entries = checked(code, address - BASE, side * side * 4)
    rgba = bytearray(256 * 256 * 4)
    cells = []
    cache = {}
    for n in range(side * side):
        flags, character = struct.unpack_from('>HH', entries, n * 4)
        if flags & ~0xc07f:
            raise InputError('unsupported pattern-name flags')
        if character not in cache:
            cache[character] = pattern(pixels, character, size)
        indices = cache[character]
        colors = palette[(flags & 127) * 16:(flags & 127) * 16 + 16]
        cells.append(dict(x=(n % side) * size, y=(n // side) * size,
                          character=character, file_offset=character * 32 - PATTERN_BASE,
                          palette=flags & 127, flip_x=bool(flags & 0x4000),
                          flip_y=bool(flags & 0x8000), pixels_sha256=digest(indices)))
        for y in range(size):
            for x in range(size):
                sx = size - 1 - x if flags & 0x4000 else x
                sy = size - 1 - y if flags & 0x8000 else y
                index = indices[sy * size + sx]
                if not index:
                    continue  # pen zero transparent, regardless of its color
                color = colors[index]
                at = (((n // side) * size + y) * 256 + (n % side) * size + x) * 4
                rgba[at:at + 4] = bytes(((color & 31) * 255 // 31,
                                        ((color >> 5) & 31) * 255 // 31,
                                        ((color >> 10) & 31) * 255 // 31, 255))
    return bytes(rgba), cells


def png(rgba: bytes) -> bytes:
    if len(rgba) != 256 * 256 * 4:
        raise InputError('expected a 256x256 RGBA rectangle')

    def chunk(kind: bytes, data: bytes) -> bytes:
        return struct.pack('>I', len(data)) + kind + data + struct.pack('>I', zlib.crc32(kind + data))

    rows = b''.join(b'\0' + rgba[y * 1024:(y + 1) * 1024] for y in range(256))
    return (b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', 256, 256, 8, 6, 0, 0, 0))
            + chunk(b'IDAT', zlib.compress(rows)) + chunk(b'IEND', b''))


def export(code: bytes, pixels: bytes) -> tuple[dict, dict[str, bytes]]:
    pinned(code, 'OPENING.Z')
    pinned(pixels, 'OPENING.BIN')
    palette = palettes(code)
    report = dict(schema=1, profile='Saturn JP T-1212G V1.001, first opening setup',
                  input_sha256={'OPENING.Z': digest(code), 'OPENING.BIN': digest(pixels)},
                  evidence='static interpretation, no runtime observation',
                  limits=['rectangles are not screen coordinates or complete scene layers',
                          'initial palettes only; no palette animation, priority or color offsets',
                          'no sprites, sound, timing or arcade-equivalence claim'], pages=[])
    images = {}
    for label, table, indices, size in LAYERS:
        for index in indices:
            address = struct.unpack('>I', checked(code, table - BASE + index * 4, 4))[0]
            rgba, cells = page(code, pixels, palette, address, size)
            name = f'{label}-{index}.png'
            images[name] = png(rgba)
            report['pages'].append(dict(image=name, pointer_table=table, index=index,
                                        map_address=address, character_size=size,
                                        rgba_sha256=digest(rgba), png_sha256=digest(images[name]), cells=cells))
    return report, images


def write_export(folder: Path, prefix: str, report: dict, images: dict[str, bytes]) -> Path:
    folder = folder.resolve()
    if not folder.is_relative_to((ROOT / 'port/local').resolve()):
        raise InputError('output must be inside this checkout\'s port/local/')
    if not re.fullmatch(r'[a-z0-9][a-z0-9-]{0,63}', prefix):
        raise InputError('output prefix must be a short lowercase identifier')
    names = {f'{prefix}-{name}': data for name, data in images.items()}
    # No input path, local username or host identifier is needed in the report.
    report = dict(report, output_prefix=prefix)
    report_name = f'{prefix}.json'
    names[report_name] = (json.dumps(report, indent=2) + '\n').encode()
    if any((folder / name).exists() for name in names):
        raise InputError('output already exists; preserved without overwrite')
    folder.mkdir(parents=True, exist_ok=True)
    for name, data in names.items():
        with (folder / name).open('xb') as stream:
            stream.write(data)
    return folder / report_name


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--opening-bin', type=Path, required=True)
    parser.add_argument('--opening-z', type=Path, required=True)
    parser.add_argument('--output', type=Path, default=ROOT / 'port/local/intro-asset-probe')
    parser.add_argument('--prefix', default='backgrounds')
    args = parser.parse_args()
    try:
        report, images = export(args.opening_z.read_bytes(), args.opening_bin.read_bytes())
        path = write_export(args.output, args.prefix, report, images)
    except (OSError, InputError) as exc:
        parser.exit(2, f'intro assets: {exc}\n')
    print(f'{len(images)} background rectangles decoded; private report: {path}')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
