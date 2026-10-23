#!/usr/bin/env python3
"""Seconds: the sound-sample analysis tool (port/arcade/samples.py) on synthetic data; no game files.

The pure functions (chunk archive, VAB header, SPU ADPCM decode and encode, rates, resampler, block states) run on
made-up inputs, and the C helper is built and run end to end: a made-up sample ROM, a PS1 sample made from it by the
rule the tool states (24,000 Hz to 22,050 Hz, times 256, ADPCM), and the search must find the rate and the address.
"""
import math
import random
import struct
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import samples as S   # noqa: E402


def make_vh(programs, tones, sizes):
    """A VAB header: tones = [(program, tone, center, shift, low, high, vag)], sizes in bytes (multiples of 8)."""
    vh = bytearray(0x20 + 128 * 16 + programs * 16 * 32 + 512)
    vh[0:4] = b'pBAV'
    struct.pack_into('<IIIHHHH', vh, 4, 7, 0, len(vh), 0xeeee, programs, len(tones), len(sizes))
    for p, t, center, shift, lo, hi, vag in tones:
        rec = 0x20 + 128 * 16 + (p * 16 + t) * 32
        vh[rec:rec + 8] = bytes([0, 0, 127, 64, center, shift, lo, hi])
        struct.pack_into('<HHhh', vh, rec + 16, 0x80ff, 0x5fc0, p, vag)
    tab = 0x20 + 128 * 16 + programs * 16 * 32
    for i, s in enumerate(sizes):
        struct.pack_into('<H', vh, tab + 2 * (i + 1), s // 8)
    return bytes(vh)


def make_pac(chunks):
    """A chunk archive from [(table, slot, data)]."""
    head = bytearray(S.SECTOR)
    struct.pack_into('<I', head, 0, len(chunks))
    body = b''
    for i, (t, s, d) in enumerate(chunks):
        struct.pack_into('<II', head, 0x20 + 32 * i, (t << 16) | s, len(d))
        body += d + bytes(-len(d) % S.SECTOR)
    return bytes(head) + body


def swapped(b):
    out = bytearray(len(b))
    out[0::2], out[1::2] = b[1::2], b[0::2]
    return bytes(out)


def smooth_noise(n, seed):
    """Deterministic low-frequency noise (a few passes of a one-pole filter), peak about 100."""
    rng = random.Random(seed)
    x = [rng.gauss(0, 1) for _ in range(n)]
    for _ in range(3):
        y, acc = [], 0.0
        for v in x:
            acc += 0.3 * (v - acc)
            y.append(acc)
        x = y
    peak = max(abs(v) for v in x)
    return [v * 100 / peak for v in x]


class Pure(unittest.TestCase):
    def test_pac_chunks(self):
        pac = make_pac([(3, 0, b'a' * 5), (4, 1, b'b' * 3000)])
        self.assertEqual(S.pac_chunks(pac), [(3, 0, b'a' * 5), (4, 1, b'b' * 3000)])
        with self.assertRaises(S.FormatError):
            S.pac_chunks(pac[:S.SECTOR + 2])
        with self.assertRaises(S.FormatError):
            S.pac_chunks(b'x' * 100)

    def test_parse_vh(self):
        vh = make_vh(1, [(0, 0, 72, 0, 0, 127, 2), (0, 1, 84, 0, 60, 60, 1), (0, 2, 80, 18, 0, 0, 0)], [64, 32])
        info = S.parse_vh(vh)
        self.assertEqual(info['programs'], 1)
        self.assertEqual(info['sizes'], [64, 32])
        self.assertEqual([(t['tone'], t['center'], t['shift'], t['vag']) for t in info['tones']], [(0, 72, 0, 2), (1, 84, 0, 1)])
        self.assertEqual(info['tones'][1]['low'], 60)
        with self.assertRaises(S.FormatError):
            S.parse_vh(b'XXXX' + vh[4:])
        with self.assertRaises(S.FormatError):
            S.parse_vh(vh[:0x20 + 128 * 16 + 40])

    def test_vag_slices(self):
        body = bytes(range(96))
        parts = S.vag_slices(body, [64, 32])
        self.assertEqual((len(parts[0]), len(parts[1]), parts[1][0]), (64, 32, 64))
        with self.assertRaises(S.FormatError):
            S.vag_slices(body, [64, 16])

    def test_decode_known_block(self):
        # filter 0, shift 12: the output is the signed nibble itself; low nibble first; flags kept
        blk = bytes([0x0c, 0x04]) + bytes([0x21, 0xf3, 0x87] + [0] * 11)
        pcm, flags = S.decode_adpcm(blk)
        self.assertEqual(flags, [4])
        self.assertEqual(pcm[:6], [1, 2, 3, -1, 7, -8])
        # filter 1 predicts 60/64 of the last output: a constant step of 4 after a jump
        blk = bytes([0x1c, 0x00, 0x70] + [0] * 13)   # nibbles 0 then 7 then zeros, shift 12
        pcm, _ = S.decode_adpcm(blk)
        self.assertEqual(pcm[0], 0)
        self.assertEqual(pcm[1], 7)
        self.assertEqual(pcm[2], (7 * 60 + 32) >> 6)
        self.assertEqual(pcm[3], (pcm[2] * 60 + 32) >> 6)

    def test_shift_scales(self):
        for shift, step in ((12, 1), (11, 2), (8, 16), (0, 4096)):
            pcm, _ = S.decode_adpcm(bytes([shift, 0, 0x01] + [0] * 13))
            self.assertEqual(pcm[0], step)

    def test_encode_reproduces_decoded_blocks(self):
        rng = random.Random(7)
        data = bytearray()
        for _ in range(6):
            data += bytes([(rng.randrange(5) << 4) | rng.randrange(10, 13), 0]) + bytes(rng.randrange(256) for _ in range(14))
        pcm, _ = S.decode_adpcm(bytes(data))
        again, _ = S.decode_adpcm(S.encode_adpcm(pcm))
        self.assertEqual(again, pcm)

    def test_encode_quality(self):
        sig = [round(3000 * math.sin(0.07 * k) + 800 * math.sin(0.31 * k)) for k in range(560)]
        dec, _ = S.decode_adpcm(S.encode_adpcm(sig))
        self.assertGreater(S.snr_db(sig, dec[:len(sig)]), 25)
        self.assertEqual(len(S.encode_adpcm(sig[:29])), 32)

    def test_tone_rate(self):
        self.assertAlmostEqual(S.tone_rate(72, 0), 44100)
        self.assertAlmostEqual(S.tone_rate(84, 0), 22050)
        self.assertEqual(S.snap_rate(S.tone_rate(80, 18)), 28000)
        self.assertEqual(S.snap_rate(S.tone_rate(78, 57)), 32000)
        self.assertEqual(S.snap_rate(S.tone_rate(74, 40)), 40000)
        self.assertIsNone(S.snap_rate(S.tone_rate(76, 22)))
        self.assertEqual(S.candidate_rates([dict(center=72, shift=0), dict(center=84, shift=0)]), [22050, 44100])
        self.assertEqual(S.candidate_rates([dict(center=76, shift=22)]), list(S.NOMINAL_RATES))

    def test_flag_structure(self):
        one = [0, 4] + [0] * 5 + [1, 7]
        self.assertEqual(S.flag_structure(one), 'oneshot')
        self.assertEqual(S.flag_structure([0, 2, 2, 2, 2, 3]), 'loop')
        mid = [0, 4, 0, 1, 7, 0, 0, 1, 7]
        self.assertEqual(S.flag_structure(mid), 'oneshot+loopbit')
        self.assertEqual(S.flag_structure([0, 0, 0]), 'other')

    def test_resampler(self):
        src = [100 * math.sin(0.05 * k) for k in range(400)]
        for rho in (0.5, 1.0, 1.7):
            out = S.resample_lanczos(src, 100.3, rho, 40)
            want = [100 * math.sin(0.05 * (100.3 + rho * k)) for k in range(40)]
            self.assertLess(max(abs(a - b) for a, b in zip(out, want)), 0.5, rho)
        # a tone above the new Nyquist is removed when the rate falls
        fast = [100 * math.sin(2.4 * k) for k in range(400)]
        out = S.resample_lanczos(fast, 100, 2.0, 40)
        self.assertLess(max(abs(v) for v in out), 5)

    def test_snr(self):
        self.assertEqual(S.snr_db([1, 2, 3], [1, 2, 3]), math.inf)
        self.assertAlmostEqual(S.snr_db([10, 10], [9, 11]), 10 * math.log10(200 / 2))

    def test_clusters_and_states(self):
        hits = [(0, 1.0, 3, 0.99, 5000 + 3 * 1024 + 2), (0, 1.0, 4, 0.98, 5000 + 4 * 1024), (0, 1.0, 7, 0.97, 9000 + 7 * 1024),
                (0, 1.0, 8, 0.5, 777), (1, 0.5, 2, 0.95, 123 + 2 * 1024)]
        got = S.cluster_hits(hits)
        self.assertEqual(sorted((s, rho, round(o / 10), b) for s, rho, o, b in got), [(0, 1.0, 500, 3), (0, 1.0, 900, 7), (1, 0.5, 12, 2)])
        self.assertEqual(S.state_summary('q' + 'M' * 9 + 'q'), (1.0, 'full'))
        share, cls = S.state_summary('q' + 'M' * 6 + '.' * 4 + 'q')
        self.assertEqual((round(share, 1), cls), (0.6, '50-80'))
        self.assertEqual(S.state_summary('q..........q')[1], 'under 50')
        self.assertEqual(S.state_summary('q' + 'M' * 8 + '.' * 1 + 'q')[1], '80-95')
        self.assertTrue(S.acceptable_line('q' + 'M' * 4 + 'nn' + '.' * 5 + 'q'))
        self.assertFalse(S.acceptable_line('q' + 'M' * 3 + 'nn' + '.' * 5 + 'q'))
        self.assertAlmostEqual(S.start_address(1000.0, 1.0884), 1000 + 28 * 1.0884)


class Helper(unittest.TestCase):
    """The C helper through the tool's own staging and search functions."""

    def test_finds_rate_and_address(self):
        n_rom = 1 << 18
        chip = [0.0] * n_rom
        wave = smooth_noise(6000, 3)
        start = 70001                     # the arcade sample: 6000 values at this chip address
        for i, v in enumerate(wave):
            chip[start + i] = v
        # a decoy elsewhere so that the search must choose
        for i, v in enumerate(smooth_noise(4000, 9)):
            chip[150000 + i] = v
        chip_i = [round(v) for v in chip]
        raw = swapped(bytes(v & 255 for v in chip_i))
        rate, rho = 22050, S.ARCADE_RATE / 22050
        count = 2800
        body = [round(256 * v) for v in S.resample_lanczos(chip_i, start, rho, count)]
        adpcm = bytearray(16) + bytearray(S.encode_adpcm(body))
        adpcm[16 + 1] = 4
        adpcm += bytes([0x00, 0x01]) + bytes(14)
        adpcm += bytes([0x00, 0x07]) + bytes([0x77] * 14)
        smp = dict(key=b'k', adpcm=bytes(adpcm), users=[('TEST', 0, 1)], tones=[])
        with tempfile.TemporaryDirectory(prefix='samples-test-') as work:
            exe = S.compile_core(work)
            rom_path, pcm_path, idx_path, dec = S.stage_inputs(work, raw, [smp])
            self.assertEqual(len(dec[0]) % 28, 0)
            found = S.find_lines(exe, [smp], rom_path, pcm_path, idx_path, lambda i: [22050], work)
            self.assertIn(0, found)
            frho, origin, gain, snr, state = found[0]
            self.assertAlmostEqual(frho, rho, places=5)
            self.assertLess(abs(S.start_address(origin, frho) - (start - 28 * 0)), 0.3)
            self.assertGreater(gain, 240)
            self.assertLess(gain, 270)
            self.assertGreater(snr, 15)
            self.assertGreaterEqual(S.state_summary(state)[0], 0.9)
            # a wrong rate list finds nothing
            none = S.find_lines(exe, [smp], rom_path, pcm_path, idx_path, lambda i: [40000], work)
            self.assertNotIn(0, none)


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
