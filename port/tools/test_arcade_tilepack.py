#!/usr/bin/env python3
"""Seconds: the sprite tile compression tool (port/arcade/tilepack.py) on synthetic data; no game files.

The reader's four block forms are built by hand and decoded; the encoder and the reader are run against each other on made-up
tiles (all forms, back references, the cut at the end of a tile, the farthest-source rule, the stale flag bits, pins); the sizes
that choose the form are checked at their edges; the tile conversion from the arcade's planes is compared with a plain
per-pixel version; the offsets and the diagnostics are read back from a small made-up chunk.
"""
import random
import struct
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import tilepack as T   # noqa: E402


def random_tile(rng, zeros=0, palette=8):
    """64 words from a small palette of values (so that matches exist), `zeros` of them zero."""
    words = [rng.randrange(1, palette) * 0x1111 + rng.randrange(0, 3) for _ in range(64)]
    for i in rng.sample(range(64), zeros):
        words[i] = 0
    return words


def distinct_tile(rng, zeros=0):
    """64 words that all differ from each other (and from anything in a tile made by another call): no match at all."""
    words = rng.sample(range(1, 65535), 64)
    for i in rng.sample(range(64), zeros):
        words[i] = 0
    return words


class Reader(unittest.TestCase):
    def test_literal_block(self):
        tile = list(range(1, 65))
        form, out = T.decode_block(struct.pack('<65H', 0, *tile), 0)
        self.assertEqual((form, out), ('lit', tile))

    def test_zero_block(self):
        flags = (1 << 3) | (1 << 63)
        block = struct.pack('<H4H2H', 0x8000, *[flags >> (16 * j) & 0xffff for j in range(4)], 0xaaaa, 0xbbbb)
        form, out = T.decode_block(block, 0)
        self.assertEqual(form, 'zero')
        self.assertEqual([i for i, w in enumerate(out) if w], [3, 63])
        self.assertEqual((out[3], out[63]), (0xaaaa, 0xbbbb))
        self.assertEqual(T.decode_block(struct.pack('<H4H', 0x8000, 0, 0, 0, 0), 0)[1], [0] * 64)   # the all-zero tile: 10 bytes

    def test_lz_block_reads_back_from_the_blocks_start(self):
        lit = list(range(100, 164))
        head = struct.pack('<65H', 0, *lit)                   # words 0..64 of the chunk: the n word, then the tile

        def block(ref_at, back, k=62):
            flags = 1 << ref_at
            words = [1000 + i for i in range(k)]
            words[ref_at] = back << 2 | 1                     # a copy of 3 words
            return head + struct.pack('<H', k) + flags.to_bytes(8, 'little')[:2 * ((k - 1 >> 4) + 1)] + struct.pack('<%dH' % k, *words)

        form, out = T.decode_block(block(20, 40), 130)        # 40 words back from word 65: word 25 = lit[24]
        self.assertEqual(form, 'lz')
        self.assertEqual(len(out), 64)
        self.assertEqual(out[20:23], lit[24:27])
        self.assertEqual(out[:20], [1000 + i for i in range(20)])
        self.assertEqual(out[23:], [1021 + i for i in range(41)])
        out = T.decode_block(block(5, 65), 130)[1]             # 65 back = word 0: the n word, then the tile's first words
        self.assertEqual(out[5:8], [0, lit[0], lit[1]])
        with self.assertRaises(ValueError):
            T.decode_block(block(5, 66), 130)                  # before the chunk
        with self.assertRaises(ValueError):
            T.decode_block(block(5, 40, k=40), 130)            # 40 items, a copy of 3: 42 words, not a tile

    def test_rle_block(self):
        tile = [0x1234] * 20 + list(range(1, 45))
        out = T.decode_block(T.encode_rle(tile), 0)
        self.assertEqual(out, ('rle', tile))

    def test_a_bad_block_is_refused(self):
        with self.assertRaises(ValueError):
            T.decode_block(struct.pack('<3H', 2, 0b01, 5) + b'\0' * 8, 0)     # decodes to 3 words


class Encoder(unittest.TestCase):
    def roundtrip(self, tiles, **kw):
        chunk, forms = T.encode_chunk(tiles, **kw)
        decoded = T.decode_chunk(chunk)
        self.assertEqual([w for _f, w in decoded], tiles)
        self.assertEqual([f for f, _w in decoded], forms)
        return chunk, forms

    def test_round_trip_of_every_form(self):
        rng = random.Random(1)
        tiles = [[0] * 64, distinct_tile(rng), distinct_tile(rng, zeros=20), random_tile(rng, 5), random_tile(rng, 30), random_tile(rng, 0, 3)]
        tiles += [list(tiles[1]), list(tiles[3])] + [random_tile(rng, rng.randrange(0, 40), rng.choice([3, 6, 12])) for _ in range(60)]
        chunk, forms = self.roundtrip(tiles)
        self.assertEqual(set(forms), {'lit', 'zero', 'lz'})
        self.assertEqual(forms[0], 'zero')
        self.assertEqual(len(T.block_sizes(chunk)), len(tiles))

    def test_chunk_layout(self):
        rng = random.Random(2)
        tiles = [distinct_tile(rng), distinct_tile(rng, 20), [0] * 64]
        chunk, forms = T.encode_chunk(tiles)
        self.assertEqual(forms, ['lit', 'zero', 'zero'])
        sizes = T.block_sizes(chunk)
        self.assertEqual(sizes[0], (12, 130))
        self.assertEqual(sizes[1], (142, 10 + 2 * 44))
        self.assertEqual(sizes[2], (142 + 98, 10))
        self.assertEqual(len(chunk), 142 + 98 + 10)                  # no padding, the chunk ends with the last block

    def test_zero_form_needs_fourteen_zero_words(self):
        rng = random.Random(3)
        for zeros, form in ((13, 'lit'), (14, 'zero'), (15, 'zero')):
            chunk, forms = T.encode_chunk([distinct_tile(rng, zeros)])
            self.assertEqual(forms, [form], zeros)
        self.assertEqual(T.block_sizes(T.encode_chunk([distinct_tile(rng, 14)])[0])[0][1], 110)

    def test_lz_wins_when_it_is_small_and_smaller_than_zero(self):
        rng = random.Random(4)
        base = distinct_tile(rng)
        again = list(base)                      # an identical tile: 64 words, chains of 5-word references
        chunk, forms = self.roundtrip([base, again])
        self.assertEqual(forms, ['lit', 'lz'])
        self.assertLessEqual(T.block_sizes(chunk)[1][1], 108)
        # the same tile with 16 zeros: the zero form (10 + 2 * 48 = 106) against LZ of an identical tile (smaller), LZ wins
        z = list(base)
        for i in range(0, 64, 4):
            z[i] = 0
        chunk, forms = self.roundtrip([base, z])
        self.assertEqual(forms[1], 'lz')

    def test_farthest_of_equal_matches(self):
        rng = random.Random(5)
        a = distinct_tile(rng)
        gap = distinct_tile(rng)
        tiles = [a, gap, list(a)]               # tile 2 matches tile 0 only (tile 1 has other words)
        _chunk, _ = self.roundtrip(tiles)
        stream = T.Stream()
        for t in tiles[:2]:
            stream.append(T.encode_tile(stream, t)[0])
        start = len(stream.words)
        m = T.find_match(stream, a, 0, start, 5)
        self.assertEqual(m, (5, start - 1))     # the literal block 0 starts at 0; its n word is word 0, tile word 0 is word 1
        # a second copy of the same words nearer: the farthest is still chosen
        stream.append(T.lit_block(a))
        start = len(stream.words)
        self.assertEqual(T.find_match(stream, a, 0, start, 5), (5, start - 1))

    def test_longest_before_nearest_or_farthest(self):
        stream = T.Stream()
        stream.append([0, 11, 22, 33])                       # n word, then a 3-word run
        stream.append([0, 11, 22, 33, 44, 55])               # a longer run, nearer
        start = len(stream.words)
        tile = [11, 22, 33, 44, 55] + [999 + i for i in range(59)]
        self.assertEqual(T.find_match(stream, tile, 0, start, 5), (5, start - 5))
        self.assertEqual(T.find_match(stream, tile, 0, start, 3), (3, start - 1))     # at most 3 words: the farthest of the 3-word ones

    def test_window_is_16383_words(self):
        stream = T.Stream()
        stream.words = [0] * 20000
        stream.pairs[(0x1111, 0x2222)] = [100, 5000]
        stream.words[100:102] = [0x1111, 0x2222]
        stream.words[5000:5002] = [0x1111, 0x2222]
        tile = [0x1111, 0x2222] + [3000 + i for i in range(62)]
        self.assertEqual(T.find_match(stream, tile, 0, 16483, 5), (2, 16383))              # the largest distance is allowed
        self.assertEqual(T.find_match(stream, tile, 0, 16484, 5), (2, 16484 - 5000))       # one more and word 100 is out of reach

    def test_reference_is_cut_at_the_end_of_a_tile(self):
        stream = T.Stream()
        row = [0xa5a5, 0x5a5a, 0xc3c3, 0x3c3c, 0xf00f]
        stream.append([0] + row * 3)
        start = len(stream.words)
        for pos, expect in ((54, 5), (56, 4), (58, 3), (60, 2), (61, 2)):
            tile = [4000 + i for i in range(pos)] + row[:min(5, 64 - pos)] + [5000 + i for i in range(64 - pos - 5)]
            tile = tile[:64]
            items = T.lz_parse(stream, tile, start)
            ref = [it for it in items if it[1] > 1 and it[0] == pos]
            self.assertTrue(ref, pos)
            self.assertEqual(ref[0][1], expect, pos)
        tile = [4000 + i for i in range(62)] + row[:2]
        self.assertTrue(all(it[1] == 1 for it in T.lz_parse(stream, tile, start) if it[0] >= 62))    # the last word is a literal

    def test_pins_keep_words_literal(self):
        rng = random.Random(6)
        a = distinct_tile(rng)
        pins = {7, 8}
        chunk, forms = self.roundtrip([a, list(a)], pins={1: pins})
        items = [it for it in T.lz_parse(self._stream_of([a]), a, 65, pins)]
        for pos, length, _d in items:
            if length > 1:
                self.assertFalse(any(q in pins for q in range(pos, pos + length)))
        self.assertTrue(any(pos in pins and length == 1 for pos, length, _d in items))

    def _stream_of(self, tiles):
        stream = T.Stream()
        for t in tiles:
            stream.append(T.lit_block(t))
        return stream

    def test_unused_flag_bits_keep_the_previous_attempts(self):
        stream = T.Stream()
        stream.flags = [0, 0xfc00, 0, 0]
        tile = [0x1111 * (1 + i % 5) + i for i in range(64)]
        items = [(i, 1, 0) for i in range(20)]               # 20 literals: the second flag word has 4 bits used
        block = T.lz_block(tile[:20], [(i, 1, 0) for i in range(20)], stream.flags)
        self.assertEqual(block[:3], [20, 0, 0xfc00])
        stream.flags = [0, 0xfc00 | 0xf, 0, 0]
        block = T.lz_block(tile[:20], items, stream.flags)
        self.assertEqual(block[2], 0xfc00)                   # the 4 used bits were written clear, the rest stayed

    def test_tiles_that_become_zero_blocks_still_update_the_flags(self):
        rng = random.Random(7)
        stream = T.Stream()
        stream.flags = [0xffff] * 4
        block, form = T.encode_tile(stream, distinct_tile(rng, 30))        # kept as a zero block, but attempted as LZ (64 literals)
        self.assertEqual(form, 'zero')
        self.assertEqual(stream.flags, [0, 0, 0, 0])


class Arcade(unittest.TestCase):
    @staticmethod
    def reference_tile(q):
        """The plain per-pixel version: arcade planes (byte y*8 + (x // 8) * 4 + plane, bit 7 - x % 8), index + 1 modulo 16, low nibble first."""
        px = []
        for y in range(16):
            for x in range(16):
                b = 0
                for pl in range(4):
                    b |= (q[y * 8 + (x // 8) * 4 + pl] >> (7 - x % 8) & 1) << pl
                px.append((b + 1) & 15)
        return bytes(px[2 * k] | px[2 * k + 1] << 4 for k in range(128))

    def test_conversion_matches_the_plain_version(self):
        rng = random.Random(8)
        gfx = bytes(rng.randrange(256) for _ in range(128 * 3))
        for code in range(3):
            self.assertEqual(T.arcade_tile(gfx, code), self.reference_tile(gfx[code * 128:code * 128 + 128]), code)

    def test_transparent_arcade_index_becomes_zero(self):
        gfx = bytes([0xff] * 128)                           # every pixel index 15
        self.assertEqual(T.arcade_tile(gfx, 0), bytes(128))

    def test_flips(self):
        rng = random.Random(9)
        data = bytes(rng.randrange(256) for _ in range(128))
        for f in (1, 2, 3):
            self.assertEqual(T.flip_tile(T.flip_tile(data, f), f), data)
        self.assertEqual(T.flip_tile(data, 0), data)
        px = [b >> (4 * i) & 15 for b in data for i in (0, 1)]
        across = [b >> (4 * i) & 15 for b in T.flip_tile(data, 1) for i in (0, 1)]
        self.assertEqual(across[0], px[15])
        self.assertEqual(across[16], px[31])
        down = [b >> (4 * i) & 15 for b in T.flip_tile(data, 2) for i in (0, 1)]
        self.assertEqual(down[:16], px[240:])
        self.assertEqual(T.flip_tile(data, 3), T.flip_tile(T.flip_tile(data, 1), 2))

    def test_tile_words_are_little_endian_pairs_of_bytes(self):
        data = bytes(range(128))
        self.assertEqual(T.tile_words(data)[:2], [0x0100, 0x0302])


class Diagnostics(unittest.TestCase):
    def test_disc_pins_find_the_words_a_later_tile_copies(self):
        rng = random.Random(10)
        a = distinct_tile(rng)
        chunk, forms = T.encode_chunk([a, list(a)])
        tiles = [w for _f, w in T.decode_chunk(chunk)]
        pins = T.disc_pins(chunk, tiles)
        self.assertEqual(forms, ['lit', 'lz'])
        self.assertTrue(pins[0])                             # tile 1 copies words of tile 0
        self.assertNotIn(1, pins)

    def test_compare_chunks(self):
        rng = random.Random(11)
        tiles = [distinct_tile(rng, 20) for _ in range(4)]
        chunk, _ = T.encode_chunk(tiles)
        self.assertEqual(T.compare_chunks(chunk, chunk), (4, 4, None))
        other = bytearray(chunk)
        o, _size = T.block_sizes(chunk)[2]
        other[o + 12] ^= 1
        self.assertEqual(T.compare_chunks(bytes(other), chunk), (3, 4, 2))
        self.assertEqual(T.form_name(chunk, 0), 'zero')


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
