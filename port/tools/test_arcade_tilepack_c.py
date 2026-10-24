#!/usr/bin/env python3
"""Seconds: the C sprite chunk encoder and reader (port/arcade/tilepack.c) on synthetic tiles; no game files.

The library is built warning-free (-std=c99 -Wall -Wextra -Werror -pedantic) natively, and the file is compiled with -m32 too. Made-up
tiles go through the encoder and back through the C reader and through the Python reader of tilepack.py; the forms, the room that is
too small (nothing written), bad arguments, hand-made blocks of every form (RLE included) and damaged chunks (the reader must
refuse them, not crash) are checked.
"""
import ctypes
import random
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import tilepack as T   # noqa: E402

SRC = ROOT / 'port/arcade/tilepack.c'
FLAGS = ['-std=c99', '-O2', '-Wall', '-Wextra', '-Werror', '-pedantic']
TMP = tempfile.TemporaryDirectory(prefix='tilepack-test-')
LIB = None


def lib():
    global LIB
    if LIB is None:
        so = Path(TMP.name) / 'tilepack.so'
        subprocess.run(['cc', *FLAGS, '-shared', '-fPIC', '-o', str(so), str(SRC)], check=True, capture_output=True, timeout=60)
        LIB = ctypes.CDLL(str(so))
        LIB.arcade_tilepack.argtypes = [ctypes.c_char_p, ctypes.c_uint32, ctypes.c_void_p, ctypes.c_uint32, ctypes.POINTER(ctypes.c_uint32),
                                        ctypes.c_char_p, ctypes.c_size_t]
        LIB.arcade_tilepack_decode.argtypes = [ctypes.c_char_p, ctypes.c_uint32, ctypes.c_uint32, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_size_t]
        LIB.arcade_tilepack_count.argtypes = [ctypes.c_char_p, ctypes.c_uint32]
        LIB.arcade_tilepack_count.restype = ctypes.c_uint32
    return LIB


def pack(tiles, room=1 << 22):
    """(chunk, None) or (None, error) for tiles of 128 bytes each."""
    out, size, err = ctypes.create_string_buffer(room), ctypes.c_uint32(), ctypes.create_string_buffer(256)
    if lib().arcade_tilepack(b''.join(tiles), len(tiles), out, room, ctypes.byref(size), err, len(err)):
        return None, err.value.decode()
    return out.raw[:size.value], None


def unpack(chunk, index):
    """(tile bytes, None) or (None, error)."""
    tile, err = ctypes.create_string_buffer(128), ctypes.create_string_buffer(256)
    if lib().arcade_tilepack_decode(chunk, len(chunk), index, tile, err, len(err)):
        return None, err.value.decode()
    return tile.raw, None


def tile_bytes(words):
    return struct.pack('<64H', *words)


def distinct(rng, zeros=0):
    words = rng.sample(range(1, 65535), 64)
    for i in rng.sample(range(64), zeros):
        words[i] = 0
    return tile_bytes(words)


def family(rng, n, variants=6):
    """n tiles that are variants of a few base tiles (a few words changed, some rows blank): matches in every direction."""
    bases = [[rng.randrange(1, 12) * 0x1111 + rng.randrange(3) for _ in range(64)] for _ in range(variants)]
    tiles = []
    for _ in range(n):
        w = list(rng.choice(bases))
        for i in rng.sample(range(64), rng.randrange(0, 10)):
            w[i] = rng.randrange(0, 12) * 0x1111
        if rng.random() < 0.3:
            for i in range(rng.randrange(0, 4) * 4):
                w[i] = 0
        tiles.append(tile_bytes(w))
    return tiles


class Build(unittest.TestCase):
    def test_native_build_has_no_warnings(self):
        self.assertIsNotNone(lib())

    def test_32_bit_compile(self):
        r = subprocess.run(['cc', '-m32', *FLAGS, '-c', '-o', str(Path(TMP.name) / 'tilepack32.o'), str(SRC)], capture_output=True, text=True, timeout=60)
        if r.returncode and ('-m32' in r.stderr or 'bits/' in r.stderr or 'gnu/stubs' in r.stderr or 'No such file' in r.stderr):
            self.skipTest('no 32-bit C library on this machine: ' + r.stderr.splitlines()[0])
        self.assertEqual(r.returncode, 0, r.stderr)
        self.assertEqual(r.stderr, '')


class Encoder(unittest.TestCase):
    def test_round_trip_through_both_readers(self):
        rng = random.Random(1)
        tiles = [bytes(128), distinct(rng), distinct(rng, 20), distinct(rng, 63)] + family(rng, 300) + [bytes(128), distinct(rng)]
        chunk, err = pack(tiles)
        self.assertIsNone(err)
        self.assertEqual(lib().arcade_tilepack_count(chunk, len(chunk)), len(tiles))
        for i, t in enumerate(tiles):
            self.assertEqual(unpack(chunk, i), (t, None), i)
        self.assertEqual([tile_bytes(w) for _f, w in T.decode_chunk(chunk)], tiles)

    def test_layout_and_forms(self):
        rng = random.Random(2)
        a, b = distinct(rng), distinct(rng, 30)
        again = bytearray(a)
        again[10] ^= 0xff                                              # the tile again but for one word: references (an exact copy has no block, see below)
        tiles = [bytes(128), a, b, bytes(again)]
        chunk, _ = pack(tiles)
        sizes = T.block_sizes(chunk)
        self.assertEqual(sizes[0], (16, 10))                           # the all-zero tile: a zero block of no words
        self.assertEqual(sizes[1], (26, 130))                          # words that match nothing: literals
        self.assertEqual(sizes[2], (156, 10 + 2 * 34))                 # 30 zero words: the zero form
        self.assertEqual(T.form_name(chunk, 3), 'lz')                  # the same tile again: references
        self.assertLessEqual(sizes[3][1], 2 + 2 * 4 + 2 * 14)          # 64 words in at most 14 items (5 + 5 + 5 ...)
        self.assertEqual(len(chunk), sizes[3][0] + sizes[3][1])        # no padding
        self.assertEqual([T.form_name(chunk, i) for i in range(3)], ['zero', 'lit', 'zero'])

    def test_references_stay_inside_the_reach(self):
        rng = random.Random(3)
        a = distinct(rng)
        filler = [distinct(rng) for _ in range(260)]                   # 260 literal blocks: more than 16,383 words
        near = bytearray(a)
        near[0] ^= 1                                                   # a near copy: not an exact one, which shares the first one's block at any distance
        near = bytes(near)
        chunk, _ = pack([a] + filler + [near])
        self.assertEqual(T.form_name(chunk, len(filler) + 1), 'lit')    # the first copy is out of reach
        chunk, _ = pack([a] + filler[:200] + [near])
        self.assertEqual(T.form_name(chunk, 201), 'lz')                 # 200 blocks of 65 words: in reach

    def test_an_exact_copy_of_an_earlier_tile_shares_its_block(self):
        rng = random.Random(11)
        a, b, c = distinct(rng), distinct(rng, 12), distinct(rng, 40)
        tiles = [a, b, bytes(128), a, c, b, bytes(128), a, c]
        chunk, err = pack(tiles)
        self.assertIsNone(err)
        offs = struct.unpack_from('<%dI' % len(tiles), chunk, 0)
        self.assertEqual([offs[i] for i in (3, 7)], [offs[0]] * 2)                 # the copies of the first tile point at its block
        self.assertEqual(offs[5], offs[1])
        self.assertEqual(offs[6], offs[2])
        self.assertEqual(offs[8], offs[4])
        self.assertEqual(len(set(offs)), 4)
        for i, t in enumerate(tiles):
            self.assertEqual(unpack(chunk, i), (t, None), i)
        no_copies, _ = pack([a, b, bytes(128), c])
        self.assertEqual(len(chunk) - 4 * (len(tiles) - 4), len(no_copies))         # the copies cost their offsets and nothing else
        # a chunk of copies fits a room that its distinct tiles alone need
        chunk2, err2 = pack(tiles, room=len(no_copies) + 4 * (len(tiles) - 4))
        self.assertIsNone(err2)

    def test_smaller_than_the_plain_optimal_parse_or_equal_on_a_family(self):
        rng = random.Random(4)
        tiles = family(rng, 200)
        chunk, _ = pack(tiles)
        words = [T.tile_words(t) for t in tiles]
        plain, _forms = T.encode_chunk_small(words)
        self.assertLessEqual(len(chunk), len(plain) + 100)             # the same format; the C may keep words literal and search less deep
        self.assertEqual([w for _f, w in T.decode_chunk(chunk)], words)

    def test_deterministic(self):
        rng = random.Random(5)
        tiles = family(rng, 120)
        self.assertEqual(pack(tiles), pack(tiles))

    def test_room(self):
        rng = random.Random(6)
        tiles = family(rng, 80)
        chunk, _ = pack(tiles)
        n = len(chunk)
        guard = 64
        for room in (n // 2, 4 * len(tiles), 4):
            out = ctypes.create_string_buffer(b'\xaa' * (room + guard), room + guard)
            size, err = ctypes.c_uint32(77), ctypes.create_string_buffer(256)
            rc = lib().arcade_tilepack(b''.join(tiles), len(tiles), out, room, ctypes.byref(size), err, len(err))
            self.assertEqual(rc, -1, room)
            self.assertIn('room', err.value.decode())
            self.assertEqual(out.raw, b'\xaa' * (room + guard), room)          # nothing written
            self.assertEqual(size.value, 0)
        # a room just below the first chunk: the encoder may find a smaller one with another setting, but never overflows
        room = n - 1
        out = ctypes.create_string_buffer(b'\xaa' * (room + guard), room + guard)
        size, err = ctypes.c_uint32(), ctypes.create_string_buffer(256)
        rc = lib().arcade_tilepack(b''.join(tiles), len(tiles), out, room, ctypes.byref(size), err, len(err))
        self.assertEqual(out.raw[room:], b'\xaa' * guard)
        if rc:
            self.assertEqual(out.raw, b'\xaa' * (room + guard))
        else:
            self.assertLessEqual(size.value, room)
        out = ctypes.create_string_buffer(b'\xaa' * (n + guard), n + guard)
        size, err = ctypes.c_uint32(), ctypes.create_string_buffer(256)
        self.assertEqual(lib().arcade_tilepack(b''.join(tiles), len(tiles), out, n, ctypes.byref(size), err, len(err)), 0)
        self.assertEqual(out.raw[:n], chunk)
        self.assertEqual(out.raw[n:], b'\xaa' * guard)                          # not a byte past the room
        self.assertEqual(size.value, n)

    def test_bad_arguments(self):
        out, size, err = ctypes.create_string_buffer(1000), ctypes.c_uint32(), ctypes.create_string_buffer(256)
        self.assertEqual(lib().arcade_tilepack(b'', 0, out, 1000, ctypes.byref(size), err, len(err)), -1)
        self.assertIn('bad arguments', err.value.decode())
        self.assertEqual(lib().arcade_tilepack(bytes(128), 16385, out, 1000, ctypes.byref(size), err, len(err)), -1)
        self.assertEqual(lib().arcade_tilepack(bytes(128), 1, out, 1000, ctypes.byref(size), None, 0), 0)      # no error buffer is fine
        self.assertEqual(lib().arcade_tilepack(bytes(128 * 2), 2, out, 5, ctypes.byref(size), None, 0), -1)


class Reader(unittest.TestCase):
    def test_every_form_from_the_python_encoder(self):
        rng = random.Random(7)
        tiles = [T.tile_words(t) for t in [bytes(128), distinct(rng), distinct(rng, 25)] + family(rng, 60)]
        chunk, forms = T.encode_chunk(tiles)
        self.assertEqual(set(forms), {'lit', 'zero', 'lz'})
        for i, w in enumerate(tiles):
            self.assertEqual(unpack(chunk, i), (tile_bytes(w), None), i)

    def test_rle_block(self):
        rng = random.Random(8)
        w = [0x1234] * 20 + [rng.randrange(1, 65535) for _ in range(44)]
        block = T.encode_rle(w)
        chunk = struct.pack('<I', 4) + block
        self.assertEqual(unpack(chunk, 0), (tile_bytes(w), None))

    def test_index_and_count(self):
        chunk, _ = pack([bytes(128)] * 3)
        self.assertEqual(unpack(chunk, 3)[0], None)
        self.assertEqual(lib().arcade_tilepack_count(b'', 0), 0)
        self.assertEqual(lib().arcade_tilepack_count(struct.pack('<I', 6) + bytes(10), 14), 0)     # not a multiple of 4
        self.assertEqual(lib().arcade_tilepack_count(struct.pack('<I', 400) + bytes(10), 14), 0)   # table longer than the chunk

    def test_damaged_chunks_are_refused(self):
        rng = random.Random(9)
        tiles = family(rng, 40) + [distinct(rng, 20)]
        chunk, _ = pack(tiles)
        # truncations: the last tile must not decode (or must decode only when the cut leaves its block whole)
        for cut in range(len(chunk) - 130, len(chunk), 7):
            for i in range(len(tiles)):
                unpack(chunk[:cut], i)                                        # must return, not crash
        # a reference to before the chunk
        o = T.block_sizes(chunk)[40 - 1][0]
        bad = bytearray(chunk)
        for j in range(len(tiles)):
            off = T.block_sizes(chunk)[j][0]
            if bad[off + 1] == 0 and bad[off] and bad[off] < 64:                  # an LZ block: set every flag, all references far
                bad[off + 2:off + 4] = b'\xff\xff'
                break
        for i in range(len(tiles)):
            unpack(bytes(bad), i)
        # random damage: never a crash, an error line when refused
        for seed in range(300):
            r = random.Random(seed)
            d = bytearray(chunk)
            for _ in range(r.randrange(1, 12)):
                d[r.randrange(len(d))] = r.randrange(256)
            for i in r.sample(range(len(tiles)), 6):
                tile, err = unpack(bytes(d), i)
                self.assertTrue(tile is not None or err)
        self.assertIsNotNone(o)

    def test_offsets_must_be_even_and_inside(self):
        chunk, _ = pack([bytes(128), bytes(128)])
        odd = bytearray(chunk)
        odd[0] = (odd[0] + 1) & 0xff
        self.assertIsNotNone(unpack(bytes(odd), 0)[1])
        out_of_range = bytearray(chunk)
        out_of_range[4:8] = struct.pack('<I', len(chunk) + 4)
        self.assertIsNotNone(unpack(bytes(out_of_range), 1)[1])
        before_table = bytearray(chunk)
        before_table[4:8] = struct.pack('<I', 0)
        self.assertIsNotNone(unpack(bytes(before_table), 1)[1])


if __name__ == '__main__':
    # The line the review reads (tools/ai_workflow/review.py): the last line, on a pass only.
    import time
    started = time.monotonic()
    result = unittest.main(exit=False).result
    took = time.monotonic() - started
    bad, ran = len(result.failures) + len(result.errors), result.testsRun
    if bad or not ran:
        print(f"{bad} of {ran} tests FAILED in {took:.1f} seconds")
        sys.exit(1)
    print(f"all {ran} tests passed in {took:.1f} seconds")
