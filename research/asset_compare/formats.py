"""Offline PS1/CPS2 sprite formats, authored here from static format evidence.

Reads bounded PAC chunks, tiles and composition lists. Standard library only;
seconds per character, no compiler/game/emulator. See README for supported layout
and inference limits. The tests use only synthetic bytes.
"""
from __future__ import annotations

import struct
import zlib
from functools import lru_cache

class InputError(ValueError):
    pass


def checked(data: bytes, offset: int, size: int) -> bytes:
    if offset < 0 or size < 0 or offset + size > len(data):
        raise InputError(f"read outside input: offset {offset}, size {size}")
    return data[offset:offset + size]


def pac_chunks(data: bytes) -> list[dict]:
    count = struct.unpack('<I', checked(data, 0, 4))[0]
    if count > 63:
        raise InputError('PAC directory exceeds its header sector')
    out, pos = [], 0x800
    for i in range(count):
        slot, table, size, first = struct.unpack_from('<HHII', checked(data, 0x20 + i * 32, 32))
        body = checked(data, pos, size)
        if size < 4 or struct.unpack_from('<I', body)[0] != first:
            raise InputError('PAC first-word check failed')
        padded = (size + 2047) // 2048 * 2048
        checked(data, pos, padded)
        out.append(dict(slot=slot, table=table, offset=pos, size=size, data=body))
        pos += padded
    if pos != len(data):
        raise InputError('PAC trailing data or truncated padding')
    return out


def signed16(n: int) -> int:
    return ((n + 32768) % 65536) - 32768


class Reader:
    def __init__(self, data: bytes, base: int):
        self.data, self.base = data, base

    def read(self, address: int, size: int) -> bytes:
        return checked(self.data, address - self.base, size)

    def word(self, address: int) -> int:
        return struct.unpack('<H', self.read(address, 2))[0]

    def pointer(self, address: int) -> int:
        return struct.unpack('<I', self.read(address, 4))[0]

    def sequence(self, table: int, index: int, limit: int = 64) -> dict:
        if index < 0 or index >= 256:
            raise InputError('sequence index out of range')
        start = self.pointer(table + index * 4)
        rows = []
        for i in range(limit):
            address = start + i * 12
            duration, flags, art, loop, collision = struct.unpack('<hhIhH', self.read(address, 12))
            if duration <= 0:
                raise InputError('nonpositive duration outside this probe contract')
            row = dict(address=address, duration=duration, flags=flags & 65535,
                       artwork_offset=art, loop_offset=loop, collision_frame=collision)
            rows.append(row)
            if flags & 0x8000:
                target = i + loop
                if target < 0 or target > i:
                    raise InputError('loop target outside parsed sequence')
                terminal = duration == 0xfff and loop == 0
                timed = rows[:-1] if terminal else rows
                return dict(start=start, rows=rows, terminal_hold=terminal,
                            loop_target=target, timed_steps=len(timed),
                            duration_ticks=sum(r['duration'] for r in timed),
                            unique_artwork_offsets=list(dict.fromkeys(r['artwork_offset'] for r in timed)),
                            collision_frames=list(dict.fromkeys(r['collision_frame'] for r in timed)))
        raise InputError('no terminal or loop marker within sequence bound')

    def pose(self, art_base: int, coord_base: int, art: int) -> dict:
        if art < 0 or art & 1:
            raise InputError('invalid artwork offset')
        address = art_base + art
        count, mode, default_flags, unknown, coord_offset = struct.unpack('<5H', self.read(address, 10))
        if not 0 < count <= 512 or mode not in (0, 2, 4, 6):
            raise InputError('unsupported or excessive cell list')
        cells, pos = [], address + 10
        while len(cells) < count:
            tile = self.word(pos)
            pos += 2
            flags = default_flags
            if mode in (2, 6):
                flags = self.word(pos)
                pos += 2
            reps = 1
            if mode in (4, 6):
                reps = self.word(pos) + 1
                pos += 2
            if len(cells) + reps > count:
                raise InputError('cell run exceeds declared count')
            mask = ((flags & 0x20) << 10) | ((flags & 0x40) << 8) if mode in (2, 6) else 0
            for j in range(reps):
                word = (tile + j) ^ mask
                cells.append(dict(tile=word & 0x3fff, flip_x=bool(word & 0x8000),
                                  flip_y=bool(word & 0x4000), palette=flags & 0x1f))
        end = pos
        # Default orientation and origin. C consumes unsigned steps, then stores
        # signed halfwords, so reduce coordinates to signed16 after each update.
        pos = coord_base + (coord_offset & 0xfffe)
        x = y = n = 0
        while n < count:
            dx, dy, minus_one = struct.unpack('<3H', self.read(pos, 6))
            pos += 6
            group = minus_one + 1
            if n + group > count:
                raise InputError('coordinate run exceeds declared count')
            x, y = signed16(x + dx), signed16(y + dy)
            for j in range(group):
                cells[n].update(x=x, y=y)
                n += 1
                x = signed16(x + 16)
            x = signed16(x - 16)
        bounds = None
        if cells:
            xs, ys = [c['x'] for c in cells], [c['y'] for c in cells]
            bounds = dict(left=min(xs), top=min(ys), right=max(xs) + 16,
                          bottom=max(ys) + 16, width=max(xs) - min(xs) + 16,
                          height=max(ys) - min(ys) + 16)
        return dict(address=address, count=count, mode=mode, coordinate_offset=coord_offset,
                    unknown_header_word=unknown, end=end, tile_bounds=bounds, cells=cells)


def texture(data: bytes, tile: int) -> bytes:
    if tile < 0 or tile >= 0x4000:
        raise InputError('texture index outside format')
    offset = struct.unpack('<I', checked(data, tile * 4, 4))[0]
    if offset & 1:
        raise InputError('texture block is not halfword aligned')

    def word(pos):
        return struct.unpack('<H', checked(data, pos, 2))[0]

    n = word(offset)
    pos = offset + 2
    out = []
    if n == 0:
        out = list(struct.unpack('<64H', checked(data, pos, 128)))
    elif n < 256:
        if n > 64:
            raise InputError('item count exceeds original flag array')
        flags_count = (n + 15) // 16
        flags = [word(pos + i * 2) for i in range(flags_count)]
        pos += flags_count * 2
        for i in range(n):
            v = word(pos)
            pos += 2
            if flags[i // 16] & (1 << (i % 16)):
                count, source = (v & 3) + 2, offset - (v >> 2) * 2
                out.extend(word(source + j * 2) for j in range(count))
            else:
                out.append(v)
            if len(out) > 64:
                raise InputError('texture expands past one tile')
    elif n & 0xff00 == 0x8000:
        flags = [word(pos + i * 2) for i in range(4)]
        pos += 8
        for i in range(64):
            if flags[i // 16] & (1 << (i % 16)):
                out.append(word(pos))
                pos += 2
            else:
                out.append(0)
    else:
        raise InputError('texture requires a helper codec outside this probe')
    if len(out) != 64:
        raise InputError('texture does not expand to one tile')
    return bytes((v >> shift) & 15 for v in out for shift in (0, 4, 8, 12))


def png(width: int, height: int, indices: bytes) -> bytes:
    if len(indices) != width * height:
        raise InputError('preview dimensions do not match pixels')
    scanlines = bytearray()
    for y in range(height):
        scanlines.append(0)
        for v in indices[y * width:(y + 1) * width]:
            if v == 0:
                scanlines.extend((24, 24, 24, 255))
            else:
                gray = 40 + v * 14
                scanlines.extend((gray, gray, gray, 255))

    def chunk(name, data):
        return struct.pack('>I', len(data)) + name + data + struct.pack('>I', zlib.crc32(name + data))

    return (b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', width, height, 8, 6, 0, 0, 0))
            + chunk(b'IDAT', zlib.compress(scanlines)) + chunk(b'IEND', b''))


def lane_region(parts):
    lengths = {len(p) for p in parts}
    if len(parts) != 4 or len(lengths) != 1 or not next(iter(lengths)) or next(iter(lengths)) % 2:
        raise InputError('four equal nonempty word lanes required')
    size = len(parts[0]) * 4
    out = bytearray(size)
    for lane, part in enumerate(parts):
        for byte in range(2):
            out[lane * 2 + byte::8] = part[byte::2]
    return bytes(out)


def bank_permutation(data, bank_size=0x200000):
    if bank_size < 16 or bank_size % 8 or (bank_size // 8) & ((bank_size // 8) - 1) or len(data) % bank_size:
        raise InputError('CPS2 bank must contain power-of-two 64-bit words')
    out = bytearray(len(data))
    for base in range(0, len(data), bank_size):
        source = data[base:base + bank_size]
        for byte in range(8):
            out[base + byte:base + bank_size // 2:8] = source[byte::16]
            out[base + bank_size // 2 + byte:base + bank_size:8] = source[byte + 8::16]
    return bytes(out)


def encode_tile(pixels):
    if len(pixels) != 256 or any(v > 15 for v in pixels):
        raise InputError('16x16 four-bit tile required')
    out = bytearray(128)
    for y in range(16):
        for x in range(16):
            value = pixels[y * 16 + x]
            for plane in range(4):
                if value & (1 << plane):
                    out[y * 8 + (x // 8) * 4 + plane] |= 128 >> (x % 8)
    return bytes(out)


def decode_tile(tile):
    if len(tile) != 128:
        raise InputError('128-byte planar tile required')
    return bytes(sum(((tile[y * 8 + (x // 8) * 4 + p] >> (7 - x % 8)) & 1) << p
                     for p in range(4)) for y in range(16) for x in range(16))



class CPS2Reader:
    def __init__(self, data, coordinate_table=0x9f260):
        self.data, self.coordinate_table = data, coordinate_table

    def pointer(self, pos):
        return struct.unpack('>I', checked(self.data, pos, 4))[0]

    def pose(self, pos):
        mode, count, minus_one, offset = struct.unpack('>4H', checked(self.data, pos, 8))
        if mode != 2 or not 0 < count <= 512 or minus_one >= 128 or offset % 4:
            raise InputError('unsupported CPS2 composition header')
        coord = self.pointer(self.coordinate_table + offset)
        if coord % 2:
            raise InputError('unaligned CPS2 coordinates')
        cells = []
        for i in range(minus_one + 1):
            tile, attr = struct.unpack('>HH', checked(self.data, pos + 8 + i * 4, 4))
            if attr & 0x80:
                raise InputError('CPS2 relative hardware offset is unsupported')
            x, y = struct.unpack('>hh', checked(self.data, coord + i * 4, 4))
            if abs(x) > 512 or abs(y) > 512:
                raise InputError('CPS2 coordinate exceeds character bound')
            w, h = ((attr >> 8) & 15) + 1, (attr >> 12) + 1
            fx, fy = bool(attr & 32), bool(attr & 64)
            for row in range(h):
                for col in range(w):
                    sx, sy = w - 1 - col if fx else col, h - 1 - row if fy else row
                    code = tile + sx + sy * 16
                    if not fx and not fy:
                        code = (tile & ~15) + ((tile + col) & 15) + row * 16
                    cells.append(dict(tile=code, x=x + col * 16, y=y + row * 16,
                                      flip_x=fx, flip_y=fy, palette=attr & 31))
            if len(cells) > count:
                raise InputError('CPS2 block count exceeds declared cells')
        if len(cells) != count:
            raise InputError('CPS2 block count disagrees with declared cells')
        return dict(address=pos, end=pos + 8 + 4 * (minus_one + 1), cells=cells)


def cpu_order(raw):
    if len(raw) % 2:
        raise InputError('program word stream truncated')
    result = bytearray(len(raw))
    result[::2], result[1::2] = raw[1::2], raw[::2]
    return bytes(result)


def tile_decoder(gfx, platform):
    @lru_cache(maxsize=32768)
    def decode(tile):
        if platform == 'ps1':
            return texture(gfx, tile)
        return bytes((v + 1) % 16 for v in decode_tile(checked(gfx, tile * 128, 128)))
    return decode


def picture(pose, decode):
    """Crop transparent margins, but retain painted origin and palette banks."""
    cells = pose['cells']
    left, top = min(c['x'] for c in cells), min(c['y'] for c in cells)
    width = max(c['x'] for c in cells) + 16 - left
    height = max(c['y'] for c in cells) + 16 - top
    if not 0 < width <= 512 or not 0 < height <= 512:
        raise InputError('picture canvas exceeds character bound')
    pixels = bytearray(width * height)
    for c in cells:
        tile = decode(c['tile'])
        for y in range(16):
            src = (15 - y if c['flip_y'] else y) * 16
            dest = (c['y'] - top + y) * width + c['x'] - left
            for x in range(16):
                v = tile[src + (15 - x if c['flip_x'] else x)]
                if v:
                    pixels[dest + x] = v
    painted = [i for i, v in enumerate(pixels) if v]
    if not painted:
        return dict(width=0, height=0, origin=[0, 0], pixels=b'', palettes=sorted({c['palette'] for c in cells}))
    xs, ys = [i % width for i in painted], [i // width for i in painted]
    x0, y0, x1, y1 = min(xs), min(ys), max(xs) + 1, max(ys) + 1
    cropped = b''.join(pixels[y * width + x0:y * width + x1] for y in range(y0, y1))
    return dict(width=x1-x0, height=y1-y0, origin=[left+x0, top+y0], pixels=cropped,
                palettes=sorted({c['palette'] for c in cells}))
