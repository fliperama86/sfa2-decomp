#!/usr/bin/env python3
"""Import the bounded first intro scene from a supplied arcade ZIP, seconds.

Outputs private assets only. No decryption, original-code execution or network.
The known supplied revision is deliberately pinned; unknown dumps fail closed.
Synthetic unit controls are in test_scene.py, requiring no game input.
"""
import argparse
import hashlib
from pathlib import Path
import struct
import zipfile

ARCHIVE_SHA = '7d3c1e06fff291e92abb041d65197f7d5e4b9c3967d75c26292e5308d740efbb'
PROGRAM_SHA = 'd6f4a2c7de9259e6241dbaaed6af8f673d49b03e5df23f5f50614909ea82937c'
GFX_SHA = '493c16ea3da9561e10e02fae3f686aa7ec78b3189ffe4501b20ea9aa2e1a832f'
MAGIC = b'SFA2I001'
PAYLOAD_SIZE = 102 + (16 + 512 + 48)*2 + 6*384*144 + 4*256*256*2 + 512*256


def require(ok, why):
    if not ok:
        raise ValueError(why)


def lanes(parts):
    require(len(parts) == 4 and len({len(p) for p in parts}) == 1 and len(parts[0]) % 2 == 0,
            'four equal word lanes required')
    out = bytearray(sum(map(len, parts)))
    for lane, part in enumerate(parts):
        out[lane*2::8] = part[::2]
        out[lane*2+1::8] = part[1::2]
    return bytes(out)


def unshuffle(data, bank_size=0x200000):
    require(bank_size >= 16 and bank_size % 16 == 0 and len(data) % bank_size == 0,
            'whole graphics banks required')
    out = bytearray(len(data))
    for start in range(0, len(data), bank_size):
        bank = data[start:start+bank_size]
        for byte in range(8):
            out[start+byte:start+bank_size//2:8] = bank[byte::16]
            out[start+bank_size//2+byte:start+bank_size:8] = bank[byte+8::16]
    return bytes(out)


def tile(gfx, code, side):
    require(side in (16, 32), 'tile size')
    size = side*side//2
    require(0 <= code and (code+1)*size <= len(gfx), 'tile outside graphics')
    p = gfx[code*size:(code+1)*size]
    return bytes(sum(((p[y*side//2+x//8*4+plane] >> (7-x%8)) & 1) << plane
                     for plane in range(4)) for y in range(side) for x in range(side))


def blit(out, width, height, pixels, side, x, y, attr=0, palette=False):
    for j in range(side):
        for i in range(side):
            v = pixels[(side-1-j if attr & 64 else j)*side + (side-1-i if attr & 32 else i)]
            if v != 15 and 0 <= x+i < width and 0 <= y+j < height:
                out[(y+j)*width+x+i] = v + ((attr & 31)*16 if palette else 0)


def map_pixels(rom, gfx, index, side):
    n = 256//side
    base, bias, bank = (0x2d1f00, 0xa400, 0x10000) if side == 16 else (0x2f8700, 0x900, 0x4000)
    out = [15]*(256*256)
    for i in range(n*n):
        code, attr = struct.unpack_from('>HH', rom, base+index*n*n*4+i*4)
        require(not attr & ~0x7f, 'unexpected map attributes')
        code = ((code+bias) & (0xffff if side == 16 else 0x3fff)) + bank
        blit(out, 256, 256, tile(gfx, code, side), side, i//n*side, i%n*side, attr, side == 16)
    return out


def foreground(rom, gfx, address):
    mode, count, blocks, coordinates = struct.unpack_from('>4H', rom, address)
    require(mode == 2 and blocks < 256, 'unexpected foreground layout')
    coordinates, = struct.unpack_from('>I', rom, 0x9f260+coordinates)
    out = bytearray([15])*(384*144)
    cells = 0
    for i in range(blocks+1):
        code, attr = struct.unpack_from('>HH', rom, address+8+i*4)
        x, y = struct.unpack_from('>hh', rom, coordinates+i*4)
        require(attr & 0x9f == 0, 'unexpected foreground palette/bank')
        w, h = ((attr >> 8) & 15)+1, (attr >> 12)+1
        require(0 <= 192+x and 192+x+w*16 <= 384 and 0 <= 144+y and 144+y+h*16 <= 144,
                'foreground outside traced bounds')
        cells += w*h
        for row in range(h):
            for col in range(w):
                cx, cy = (w-1-col if attr & 32 else col), (h-1-row if attr & 64 else row)
                cell = (code & ~15) + ((code+cx) & 15) + cy*16 + 0x10000
                blit(out, 384, 144, tile(gfx, cell, 16), 16, 192+x+col*16, 144+y+row*16, attr)
    require(count == cells, 'foreground cell count')
    return bytes(out)


def scene(rom, gfx):
    start = 0x229120 + struct.unpack_from('>h', rom, 0x229120)[0]
    records = bytearray(rom[start:start+100])
    require(len(records) == 100, 'truncated sequence')
    pictures = []
    for i in range(6):
        ticks, flags, art = struct.unpack_from('>HHI', records, i*16)
        require(ticks == 4 and flags == (0x8000 if i == 5 else 0), 'unexpected sequence shape')
        pictures.append(foreground(rom, gfx, art))
        struct.pack_into('>I', records, i*16+4, i)
    require(struct.unpack_from('>I', records, 96)[0] == start, 'unexpected loop target')
    struct.pack_into('>I', records, 96, 2)
    bg = [map_pixels(rom, gfx, i, 16) for i in range(0x8a, 0x8e)]
    left, right = (map_pixels(rom, gfx, i, 32) for i in (0x6a, 0x6b))
    portrait = bytes(v for y in range(256) for row in (left, right) for v in row[y*256:(y+1)*256])
    payload = (b'\0\2' + records + rom[0x276dc0:0x276de0] + rom[0x28b5c0:0x28b9c0]
               + rom[0x2959c0:0x295a20] + b''.join(pictures)
               + b''.join(struct.pack('>65536H', *frame) for frame in bg) + portrait)
    require(len(payload) == PAYLOAD_SIZE, 'internal payload length')
    return MAGIC + hashlib.sha256(payload).digest() + payload


def import_zip(path):
    require(hashlib.sha256(path.read_bytes()).hexdigest() == ARCHIVE_SHA,
            'unsupported arcade archive: expected the investigated US 960306 input')
    with zipfile.ZipFile(path) as archive:
        require(len(archive.namelist()) == len(set(archive.namelist())), 'duplicate ZIP member')
        # CPS2 ROM_LOAD16_WORD_SWAP: construct the CPU big-endian data view.
        raw = b''.join(archive.read(f'sz2u.{i:02d}') for i in range(3, 9))
        rom = bytearray(len(raw)); rom[::2], rom[1::2] = raw[1::2], raw[::2]
        gfx = unshuffle(b''.join(lanes([archive.read(f'sz2.{i:02d}m') for i in group])
                                for group in ((13, 15, 17, 19), (14, 16, 18, 20))))
    require(hashlib.sha256(rom).hexdigest() == PROGRAM_SHA, 'program data digest')
    require(hashlib.sha256(gfx).hexdigest() == GFX_SHA, 'graphics layout digest')
    return scene(rom, gfx)


def private_output(path):
    root = Path(__file__).resolve().parents[1] / "local"
    require(path.resolve().is_relative_to(root.resolve()), "output must be under port/local/ (private assets)")
    return path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('archive', type=Path)
    parser.add_argument('output', type=Path)
    args = parser.parse_args()
    try:
        private_output(args.output)
        pack = import_zip(args.archive)
        # Never overwrite an earlier artifact or original input.
        with args.output.open('xb') as file:
            file.write(pack)
    except (ValueError, OSError, zipfile.BadZipFile, KeyError, struct.error) as error:
        parser.exit(2, f'intro import: {error}\n')
    print(f'intro: wrote private first-scene pack ({len(pack)} bytes)')


if __name__ == '__main__':
    main()
