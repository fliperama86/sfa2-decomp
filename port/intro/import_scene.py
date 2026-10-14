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
OPCODE_SHA = '5b2bc2e9e1201460b996850270e6b88ff55670ccfa64c74ef62d81ad1ba83384'
EMBER_SIZE = 608 + 612 + 36*256
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


def embers(rom, gfx, opcodes):
    require(len(opcodes) == 0x300000 and hashlib.sha256(opcodes).hexdigest() == OPCODE_SHA,
            'unsupported opcode derivative: use the pinned analysis image of this archive')
    seq = bytearray(612)
    images = []
    for kind in range(6):
        original = 0x229120 + struct.unpack_from('>h', rom, 0x229122+kind*2)[0]
        start, count = 12+kind*100, 5 if kind == 0 else 6
        struct.pack_into('>H', seq, kind*2, start)
        for i in range(count):
            ticks, flags, art, a, b = struct.unpack_from('>HHIII', rom, original+i*16)
            require(ticks == (4095 if i+1 == count else 8) and
                    flags == (0x8000 if i+1 == count else 0) and not a and not b,
                    'unexpected particle sequence')
            mode, cells, attr, offset, code = struct.unpack_from('>5H', rom, art)
            require(mode == 0 and cells == 1 and attr == 0, 'unexpected particle layout')
            coordinates, = struct.unpack_from('>I', rom, 0x9f260+offset)
            require(struct.unpack_from('>hh', rom, coordinates) == (-8, -8), 'particle anchor')
            images.append(tile(gfx, code+0x10000, 16))
            struct.pack_into('>HHI', seq, start+i*16, ticks, flags, kind*6+i)
        target, = struct.unpack_from('>I', rom, original+count*16)
        require(target == original+(count-1)*16, 'unexpected particle hold target')
        struct.pack_into('>I', seq, start+count*16, start+(count-1)*16)
        if count == 5: images.append(bytes([15])*256)
    # These are encrypted PC-relative data in the supplied archive, not opcode
    # bytes for execution. Nothing from this derivative enters the runtime but
    # these fixed-size numeric tables; original instructions are never retained.
    return opcodes[0x73d9a:0x73dfa] + opcodes[0x73e10:0x74010] + seq + b''.join(images)


def scene(rom, gfx, opcodes=None):
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
    magic = MAGIC
    if opcodes is not None:
        payload += embers(rom, gfx, opcodes)
        require(len(payload) == PAYLOAD_SIZE+EMBER_SIZE, 'internal particle payload length')
        magic = b'SFA2I002'
    return magic + hashlib.sha256(payload).digest() + payload


def import_zip(path, opcodes=None):
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
    return scene(rom, gfx, opcodes)


def private_output(path):
    root = Path(__file__).resolve().parents[1] / "local"
    require(path.resolve().is_relative_to(root.resolve()), "output must be under port/local/ (private assets)")
    return path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('archive', type=Path)
    parser.add_argument('output', type=Path)
    parser.add_argument('--opcodes', type=Path, help='private pinned analysis derivative; enables version-2 embers')
    args = parser.parse_args()
    try:
        private_output(args.output)
        pack = import_zip(args.archive, args.opcodes.read_bytes() if args.opcodes else None)
        # Never overwrite an earlier artifact or original input.
        with args.output.open('xb') as file:
            file.write(pack)
    except (ValueError, OSError, zipfile.BadZipFile, KeyError, struct.error) as error:
        parser.exit(2, f'intro import: {error}\n')
    print(f'intro: wrote private first-scene pack ({len(pack)} bytes)')


if __name__ == '__main__':
    main()
