#!/usr/bin/env python3
"""Seconds: the C music player (port/arcade/music.c) against the model (music.py) and as a mixer; no game files.

The driver: a made-up Z80 program image (the one test_arcade_music.py builds, with random envelope and oscillator tables and a
command table of six entries) holds random songs and effects written by a generator that uses every one of the 32 control commands;
random scripts of packets (songs, effects with every pan motion, every special command, in stereo and mono) run through the model
and through the C driver, and the register writes (tick, register, value) must be equal. The C is built warning-free (-std=c99
-Wall -Wextra -Werror -pedantic) natively and with -m32; a small C program built with the address and undefined-behaviour
sanitizers runs the same images, and random junk as a sound program, and must come out clean.
The mixer: voices set by register writes on made-up samples, checked for rate, volume, pan, the bank, the loop and the end, the
phase, the echo, the interpolation, and the driver's clock against the output.
"""
import array
import math
import os
import random
import shutil
import signal
import struct
import subprocess
import sys
import tempfile
import time
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
sys.path.insert(0, str(ROOT / 'port/tools'))
import music as M                      # noqa: E402
import test_arcade_music as TM         # noqa: E402

SRC = ROOT / 'port/arcade/music.c'
INC = ROOT / 'port/arcade'
TMP = tempfile.TemporaryDirectory(prefix='music-c-test-')
_CM = None

# The mixer's constants that music.h calls guesses (music.c names them): the output gain, the chip's rate against the output rate.
OUTPUT_GAIN = 0.6
CHIP_RATE = 60e6 / 2 / 1248
RATE = 44100


def cm():
    global _CM
    if _CM is None:
        _CM = M.CMusic(TMP.name)
    return _CM


# ---- a richer made-up image: random curves, six command table entries -------------------------------------------------------------
def rich_image(rng):
    s = TM.Synth()
    d = s.d
    d[M.CMD_TABLE + 1] = 6                                            # six entries
    d[M.CMD_TABLE + 2] = rng.randrange(1, 100)                        # song volume index
    d[M.CMD_TABLE + 3] = rng.choice([0, 0, 100])                      # master volume index (0 = no scaling)
    s.words(M.T_ATTACK, [rng.randrange(0x800, 0xffff) for _ in range(64)])
    s.words(M.T_RATE, [rng.randrange(0x40, 0x3000) for _ in range(128)])
    s.words(M.T_LFO_RATE, [rng.randrange(0, 0x5000) for _ in range(128)])
    s.words(M.T_VIB_DEPTH, [rng.randrange(0, 0x1000) for _ in range(128)])
    s.words(M.T_TREM_DEPTH, [rng.randrange(0, 0xe000) for _ in range(128)])
    s.words(M.T_LEVEL, [min(i * rng.randrange(0x300, 0x500), 0xffff) for i in range(128)])
    for i in range(3, 6):
        s.instrument(i, rng.randrange(64), rng.randrange(0x8000), rng.randrange(0x8000, 0xc000), rng.randrange(0xc000, 0xffff), rng.choice([60, 72]))
    for n in range(4):
        s.program(n, rng.choice([2, 3, 4, 5]), rng.randrange(256) if rng.random() < 0.3 else 0, rng.randrange(64), rng.randrange(128),
                  rng.randrange(128), rng.randrange(128), rng.randrange(128))
        s.program(n, rng.choice([2, 3, 4, 5]), rng.randrange(8), rng.randrange(64), rng.randrange(128), rng.randrange(128),
                  rng.randrange(128), rng.randrange(128), base=0x7800)
    return s


class Gen:
    """Random tracks of the driver's byte code. Every command is used; nothing can loop without time passing (every loop body has a note
    of a length that is never zero), so the model and the C both finish. `used` collects the control commands emitted."""

    def __init__(self, rng):
        self.rng, self.used = rng, set()

    def note(self, lo=1):
        return bytes([(self.rng.randrange(lo, 7) << 5) | self.rng.choice([0, 0] + list(range(1, 32)))])

    def item(self, depth):
        r, rng = self.rng, self.rng
        c = r.random()
        if c < 0.45:
            return self.note()
        if c < 0.5:
            self.used.update({2}); return bytes([2]) + self.note(2)
        if c < 0.56:
            k = r.choice([0, 1, 3]); self.used.add(k); return bytes([k])
        simple = [(4, lambda: r.choice([0, 1, 2, 3, 5, 0x80, 0x40, 0x20, 0x41, 0x86])), (5, lambda: None), (6, lambda: r.choice([0, 0x40, 0x80, 0xf0, 0xff])),
                  (7, lambda: r.randrange(128)), (8, lambda: r.randrange(5)), (9, lambda: r.randrange(8)), (10, lambda: r.randrange(256)),
                  (11, lambda: r.choice([0, 1, 12, 244, 255])), (12, lambda: r.randrange(256)), (13, lambda: r.choice([0, 0, 1, 20, 0x7f, 0x85])),
                  (24, lambda: r.randrange(0x21)), (25, lambda: r.randrange(128)), (26, lambda: r.randrange(128)), (27, lambda: r.randrange(128)),
                  (28, lambda: r.randrange(128)), (29, lambda: r.randrange(128)), (30, lambda: r.choice([0, 1, 0x40, 0xc0])), (31, lambda: r.randrange(5))]
        if c < 0.82:
            n, arg = r.choice(simple)
            self.used.add(n)
            if n == 5:
                return bytes([5, r.randrange(1, 9), r.randrange(256)])
            return bytes([n, arg()])
        if c < 0.9 and depth < 4:                                # a loop: the body plays count + 1 times
            k = depth
            body = self.note() + self.body(depth + 1, r.randrange(1, 6))
            cnt = r.randrange(3)
            self.used.update({14 + k})
            return body + bytes([14 + k, cnt]) + struct.pack('>h', -(len(body) + 4))
        if c < 0.95 and depth < 4:                               # a loop with an exit on the last pass
            k = depth
            b1 = self.note() + self.body(depth + 1, r.randrange(1, 3))
            b2 = self.note() + self.body(depth + 1, r.randrange(1, 3))
            tail = bytes([14 + k, r.randrange(1, 3)])
            first = bytes([18 + k, r.choice([0, 0x80, 1])]) + struct.pack('>h', len(b2) + 4)
            self.used.update({14 + k, 18 + k})
            return b1 + first + b2 + tail + struct.pack('>h', -(len(b1) + len(first) + len(b2) + 4))
        if c < 0.98:                                             # a forward jump over junk
            junk = bytes(r.randrange(256) for _ in range(r.randrange(1, 5)))
            self.used.add(22)
            return bytes([22]) + struct.pack('>h', len(junk)) + junk
        self.used.add(23)
        return bytes([23])

    def body(self, depth, n):
        return b''.join(self.item(depth) for _ in range(n))

    def track(self, music):
        r = self.rng
        head = bytes([5, r.randrange(1, 9), r.randrange(256), 8, r.randrange(4), 7, r.randrange(128), 6, r.choice([0x40, 0x80, 0xf0]),
                      24 if music else 25, r.randrange(0x21) if music else r.randrange(128), 9, r.randrange(1, 7)])
        self.used.update({5, 8, 7, 6, 24 if music else 25, 9})
        body = self.note() + self.body(0, r.randrange(4, 40))
        if r.random() < 0.6:
            self.used.add(22)
            return head + body + bytes([22]) + struct.pack('>h', -(len(body) + 3))   # the song loops for ever
        return head + body + bytes([23])

    def voices(self, music):
        vs = self.rng.sample(range(16), self.rng.randrange(1, 7))
        return {v: self.track(music) for v in vs}


def pack_all(s, rng, gen):
    """Songs at codes 1 and 2, effects at 3 (priority 5), 4 (0x20), 5 (0x7f), and the empty code 0."""
    at = 0x20000
    for code, kind in ((1, 0), (2, 0), (3, 5), (4, 0x20), (5, 0x7f)):
        tracks = gen.voices(kind == 0)
        s.song(code, tracks, at, kind=kind)
        at += 0x2000
    return s


def packet_for(rng, code):
    if code in (0, 1, 2):
        return bytes([0, code])
    return M.effect_packet(code, rng)


def script(rng, n):
    sched, t = [], 0
    for _ in range(n):
        t += rng.choice([0, 1, 2, 3, 5, 40, 100, 300])
        c = rng.random()
        if c < 0.4:
            sched.append((t, packet_for(rng, rng.choice([1, 2, 7, 8]))))
        elif c < 0.75:
            sched.append((t, packet_for(rng, rng.choice([3, 4, 5, 9, 10]))))
        else:
            sched.append((t, M.special_packet(rng)))
    return sched, t + 1200


def alarm(seconds):
    def handler(*_):
        raise TimeoutError('a run took too long: a loop that never ends?')
    signal.signal(signal.SIGALRM, handler)
    signal.alarm(seconds)


def compare_random(cmusic, seeds, specials=None):
    """For each seed: random image, random script; C writes == model writes. Returns the control commands used and special numbers."""
    used, special = set(), set()
    for seed in seeds:
        rng = random.Random(seed)
        s = rich_image(rng)
        gen = Gen(rng)
        pack_all(s, rng, gen)
        used |= gen.used
        sched, ticks = script(rng, rng.randrange(3, 14))
        special |= {p[1] for _t, p in sched if p[0] == 0xff}
        stereo = seed % 3 != 2
        alarm(60)
        want = M.model_writes(s.rom(), sched, ticks, stereo)
        with cmusic.player(s.d, stereo=stereo) as pl:
            got = M.c_writes(pl, sched, ticks)
        alarm(0)
        diff = M.first_difference(got, want)
        if diff:
            raise AssertionError('seed %d: %s' % (seed, diff))
    return used, special


class Build(unittest.TestCase):
    def test_native_build_has_no_warnings(self):
        self.assertIsNotNone(cm())

    def test_other_optimization_levels(self):
        for opt in ('-O0', '-O1', '-O3'):
            r = subprocess.run(['cc', '-std=c99', opt, '-Wall', '-Wextra', '-Werror', '-pedantic', '-c', '-o', str(Path(TMP.name) / 'o.o'), str(SRC)],
                               capture_output=True, text=True, timeout=60)
            self.assertEqual((r.returncode, r.stderr), (0, ''), opt)

    def test_32_bit_compile(self):
        r = subprocess.run(['cc', '-m32', *M.C_FLAGS, '-c', '-o', str(Path(TMP.name) / 'music32.o'), str(SRC)], capture_output=True, text=True, timeout=60)
        if r.returncode and ('-m32' in r.stderr or 'bits/' in r.stderr or 'gnu/stubs' in r.stderr or 'No such file' in r.stderr):
            self.skipTest('no 32-bit C library on this machine: ' + r.stderr.splitlines()[0])
        self.assertEqual((r.returncode, r.stderr), (0, ''))


class DriverEqualsModel(unittest.TestCase):
    def test_random_images_and_scripts(self):
        used, special = compare_random(cm(), range(40))
        self.assertTrue(set(range(32)) - {30} <= used, sorted(set(range(32)) - used))
        self.assertEqual(special & {0, 1, 2, 5, 6, 7, 8, 9, 12, 13, 14, 3, 4, 10, 11}, {0, 1, 2, 5, 6, 7, 8, 9, 12, 13, 14, 3, 4, 10, 11})

    def test_command_30_is_used_by_no_song_but_equal(self):
        s = TM.Synth()
        s.song(1, {0: TM.HEAD + bytes([30, 0xc0, TM.note(1, 5), 30, 1, TM.note(1, 6), 23])})
        sched = [(0, bytes([0, 1]))]
        with cm().player(s.d) as pl:
            self.assertIsNone(M.first_difference(M.c_writes(pl, sched, 60), M.model_writes(s.rom(), sched, 60)))

    def test_portamento_lands_exactly_and_a_fade_in_to_full_ends(self):
        s = TM.Synth()
        body = TM.HEAD + bytes([13, 0x40, TM.note(2, 5), TM.note(2, 6), TM.note(2, 5), TM.note(2, 11), TM.note(2, 5), 22, 0xff, 0xf6])
        s.song(1, {0: body, 1: body})
        fade = bytes([0xff, 6, 0xff, 0x01, 0x00])
        sched = [(0, bytes([0, 1])), (100, fade), (400, bytes([0xff, 7, 0x30, 0x02, 0x00])), (700, fade), (900, bytes([0xff, 12, 0, 0x40, 0])),
                 (1100, bytes([0xff, 13, 0, 0x80, 0])), (1300, bytes([0xff, 13, 0, 0x80, 0])), (1500, bytes([0xff, 12, 0, 0x40, 0]))]
        with cm().player(s.d) as pl:
            self.assertIsNone(M.first_difference(M.c_writes(pl, sched, 1800), M.model_writes(s.rom(), sched, 1800)))

    def test_every_pan_motion_and_priority(self):
        rng = random.Random(77)
        s = rich_image(rng)
        pack_all(s, rng, Gen(rng))
        for mode in range(8):
            for p7 in (0, 5, 0x10, 0x11, 0x1f, 0x20, 0x30):
                p = bytearray(13)
                p[0], p[1], p[6], p[7], p[8], p[9], p[10], p[11], p[12] = 0, 3 + mode % 3, mode, p7, 6, 0, 0x90, 0, 0xa0
                sched = [(0, bytes(p)), (30, bytes([0, 4]) + bytes(11)), (90, M.effect_packet(5, rng))]
                with cm().player(s.d) as pl:
                    self.assertIsNone(M.first_difference(M.c_writes(pl, sched, 700), M.model_writes(s.rom(), sched, 700)), (mode, p7))

    def test_short_and_empty_packets_and_unknown_codes(self):
        s = rich_image(random.Random(5))
        pack_all(s, random.Random(5), Gen(random.Random(5)))
        for p in (b'', b'\x00', b'\xff', bytes([0, 1]), bytes([0xff, 1]), bytes([0xff, 99]), bytes([0xff, 6, 0, 0, 0]), bytes([0x12, 0x34]), bytes([0, 0]),
                  bytes([0, 6]), bytes([0, 1]) + bytes(40)):
            sched = [(0, p), (10, bytes([0, 1])), (20, p)]
            with cm().player(s.d) as pl:
                self.assertIsNone(M.first_difference(M.c_writes(pl, sched, 300), M.model_writes(s.rom(), sched, 300)), p)

    def test_mono_forces_every_pan_to_the_centre(self):
        s = TM.Synth()
        s.song(1, {0: TM.HEAD + bytes([TM.note(1, 5), 23])})
        with cm().player(s.d, stereo=False) as pl:
            pl.packet([0, 1])
            pl.tick(8)
            pans = [v for _t, r, v in pl.writes() if 0x80 <= r < 0x90]
        self.assertTrue(pans and all(v == 0x120 for v in pans), pans)

    def test_reset_is_power_on(self):
        s = TM.Synth()
        s.song(1, {0: TM.HEAD + bytes([TM.note(1, 5), 22, 0xff, 0xfc])})
        with cm().player(s.d) as pl:
            first = M.c_writes(pl, [(0, bytes([0, 1]))], 80)
            self.assertEqual(pl.tracks(), 1)
            cm().lib.arcade_music_set_log(pl.m, ctypes_cast(pl.log), pl.cap)
            cm().lib.arcade_music_reset(pl.m)
            self.assertEqual(pl.tracks(), 0)
            again = M.c_writes(pl, [(0, bytes([0, 1]))], 80)
            self.assertEqual(first, again)


def ctypes_cast(buf):
    import ctypes
    return ctypes.cast(buf, ctypes.c_void_p)


class Api(unittest.TestCase):
    def player(self, samples=False):
        s = TM.Synth()
        s.song(1, {0: TM.HEAD + bytes([TM.note(1, 5), 23])})
        s.song(2, {3: TM.HEAD + bytes([TM.note(2, 7), 23])}, at=TM.SONG + 0x100, kind=0x10)
        s.d[TM.SONG + 0x200] = 0x80
        s.entry(3, TM.SONG + 0x200)
        return cm().player(s.d, bytes(M.SAMPLES_SIZE) if samples else None)

    def test_command_results(self):
        with self.player() as pl:
            self.assertEqual(pl.command(1), 0)
            self.assertEqual(pl.tracks(), 1)
            self.assertEqual(pl.command(2), 0)
            self.assertEqual(pl.tracks(), 2)
            self.assertEqual(pl.command(0), -1)       # an empty entry
            self.assertEqual(pl.command(3), -1)       # an ADPCM sound is not played
            self.assertEqual(pl.packet(b''), -1)      # an empty packet is the code 0: empty
            self.assertEqual(pl.packet(bytes([0xff, 0])), 0)
            self.assertEqual(pl.tracks(), 0)

    def test_effect_command_has_a_centred_fixed_pan(self):
        with self.player() as pl:
            pl.command(2)
            pans = [(r, v) for _t, r, v in pl.writes() if 0x80 <= r < 0x90]
        self.assertEqual(pans[-1], (0x83, 0x120))

    def test_stop_keys_every_voice_off(self):
        with self.player() as pl:
            pl.command(1)
            pl.tick(8)
            n = len(pl.writes())
            M_stop = cm().lib.arcade_music_stop
            M_stop(pl.m)
            w = pl.writes()[n:]
            self.assertEqual(pl.tracks(), 0)
            self.assertEqual(sum(1 for _t, r, v in w if r == 0x00 + 6 and v == 0), 1)       # voice 0's volume
            self.assertEqual(len([1 for _t, r, v in w if 0xba <= r < 0xca and v == 0]), 16)

    def test_bad_arguments(self):
        lib = cm().lib
        import ctypes
        self.assertIsNone(lib.arcade_music_open(None, 1))
        views = M._RomViews(None, None, None, None)
        self.assertIsNone(lib.arcade_music_open(ctypes.byref(views), 1))      # no sound program
        lib.arcade_music_close(None)
        lib.arcade_music_tick(None)
        lib.arcade_music_reset(None)
        self.assertEqual(lib.arcade_music_render(None, None, 10), -1)
        with self.player() as pl:
            self.assertEqual(lib.arcade_music_render(pl.m, None, 10), -1)
            self.assertEqual(lib.arcade_music_render(pl.m, None, 0), 0)
            self.assertEqual(lib.arcade_music_packet(pl.m, None, 3), -1)
            self.assertEqual(lib.arcade_music_command(None, 1), -1)

    def test_no_samples_is_silence_and_the_driver_still_runs(self):
        with self.player() as pl:
            pl.command(1)
            out = pl.render(RATE // 4)
            self.assertEqual(out, bytes(len(out)))
            self.assertTrue(pl.writes())
            self.assertEqual(pl.tracks(), 0)           # the one note ended


# ---- the mixer ---------------------------------------------------------------------------------------------------------------------------
class Mixer(unittest.TestCase):
    """A voice is set by register writes (music.md section 7), the samples are made up, the output is read back."""

    def make(self, samples, stereo=True):
        s = TM.Synth()
        rom = bytearray(M.SAMPLES_SIZE)
        for bank, data in samples.items():
            rom[bank * 0x10000:bank * 0x10000 + len(data)] = bytes(x & 255 for x in data)
        return cm().player(s.d, bytes(rom), stereo)

    @staticmethod
    def voice(pl, v, bank, start, end, loop_len, rate, vol=0x2000, pan=16, send=0):
        pl.chip_write(((v - 1) & 15) * 8, 0x8000 | bank)
        pl.chip_write(v * 8 + 2, 0)
        pl.chip_write(v * 8 + 1, start)
        pl.chip_write(v * 8 + 5, end)
        pl.chip_write(v * 8 + 4, loop_len)
        pl.chip_write(v * 8 + 6, vol)
        pl.chip_write(0x80 + v, 0x110 + pan)
        pl.chip_write(0xba + v, send)
        pl.chip_write(v * 8 + 3, 0)
        pl.chip_write(v * 8 + 2, rate)

    @staticmethod
    def channels(raw):
        a = array.array('h')
        a.frombytes(raw)
        return a[0::2], a[1::2]

    def test_rate_is_the_chips_pitch(self):
        sine = [round(100 * math.sin(2 * math.pi * i / 32)) for i in range(4096)]
        for rate in (0x0800, 0x1000, 0x2000, 0x3000):
            with self.make({1: sine}) as pl:
                self.voice(pl, 0, 1, 0, 4096, 4096, rate)
                left, _ = self.channels(pl.render(RATE))
            ups = sum(1 for a, b in zip(left, left[1:]) if a < 0 <= b)
            want = CHIP_RATE * rate / 4096 / 32
            self.assertAlmostEqual(ups, want, delta=2 + want * 0.002, msg=rate)

    def test_volume_and_pan_law(self):
        for vol, pan, wl, wr in ((0x2000, 0, 1.0, 0.0), (0x2000, 32, 0.0, 1.0), (0x2000, 16, math.sqrt(.5), math.sqrt(.5)), (0x1000, 0, 0.5, 0.0),
                                 (0x2000, 8, math.cos(8 * math.pi / 64), math.sin(8 * math.pi / 64)), (0, 16, 0, 0)):
            with self.make({0: [100] * 64}) as pl:
                self.voice(pl, 5, 0, 0, 64, 64, 0x1000, vol, pan)
                left, right = self.channels(pl.render(500))
            unit = 100 * 256 * OUTPUT_GAIN
            self.assertAlmostEqual(left[300], unit * wl, delta=unit * 0.01 + 1, msg=(vol, pan))
            self.assertAlmostEqual(right[300], unit * wr, delta=unit * 0.01 + 1, msg=(vol, pan))

    def test_pan_register_is_clamped(self):
        with self.make({}) as pl:
            for reg_value, want in ((0x110, 0), (0x130, 32), (0x1ff, 32), (0x100, 0), (0, 0), (0x118, 8)):
                pl.chip_write(0x80 + 3, reg_value)
                self.assertEqual(pl.voice(3)[7], want, hex(reg_value))

    def test_loop_wraps_at_the_end_by_the_loop_length(self):
        ramp = list(range(0, 127))
        with self.make({2: ramp}) as pl:
            self.voice(pl, 0, 2, 0, 100, 20, 0x1000, pan=0)
            left, _ = self.channels(pl.render(4000))
        vals = [x / (256 * OUTPUT_GAIN) for x in left]
        self.assertAlmostEqual(vals[100], 100 * CHIP_RATE / RATE, delta=1)    # 100 frames: 54.5 samples along the ramp
        late = vals[600:]
        self.assertLess(max(late), 100.01)
        self.assertGreater(min(late), 78.9)                                  # the loop is 80 .. 100
        wraps = sum(1 for a, b in zip(vals, vals[1:]) if a >= 90 > b)         # the fall through 90 happens once a wrap
        self.assertAlmostEqual(wraps, (4000 * CHIP_RATE / RATE - 100) / 20 + 0.5, delta=1.5)   # one wrap per loop length of samples

    def test_no_loop_holds_silent_at_the_end(self):
        with self.make({0: [50] * 100}) as pl:
            self.voice(pl, 0, 0, 0, 100, 0, 0x1000, pan=0)
            left, right = self.channels(pl.render(1000))
        self.assertGreater(left[10], 0)
        self.assertEqual(set(left[400:]), {0})
        self.assertEqual(set(right), {0})

    def test_rate_zero_holds_and_phase_interpolates(self):
        with self.make({0: [0, 100, 0, 0]}) as pl:
            self.voice(pl, 0, 0, 0, 4, 4, 0, pan=0)
            pl.chip_write(0x3, 0x8000)                       # half way between sample 0 and 1
            left, _ = self.channels(pl.render(100))
        self.assertEqual(set(left), {round(50 * 256 * OUTPUT_GAIN)})

    def test_banks_are_filed_one_slot_early_and_select_the_samples(self):
        with self.make({3: [10] * 64, 4: [-20 & 255] * 64}) as pl:
            pl.chip_write(0x78, 0x8003)                      # register of voice 15: bank of voice 0
            pl.chip_write(0x00, 0x8004)                      # register of voice 0: bank of voice 1
            self.assertEqual((pl.voice(0)[0], pl.voice(1)[0]), (3, 4))
            for v in (0, 1):
                pl.chip_write(v * 8 + 1, 0)
                pl.chip_write(v * 8 + 5, 64)
                pl.chip_write(v * 8 + 4, 64)
                pl.chip_write(v * 8 + 6, 0x2000)
                pl.chip_write(0x80 + v, 0x110)
                pl.chip_write(v * 8 + 2, 0x1000)
            left, _ = self.channels(pl.render(50))
        self.assertAlmostEqual(left[20], (10 - 20) * 256 * OUTPUT_GAIN, delta=2)

    def test_sixteen_voices_add(self):
        with self.make({0: [10] * 64}) as pl:
            for v in range(16):
                self.voice(pl, v, 0, 0, 64, 64, 0x1000, pan=0)
            left, _ = self.channels(pl.render(100))
        self.assertAlmostEqual(left[50], 16 * 10 * 256 * OUTPUT_GAIN, delta=3)

    def test_output_is_clipped_not_wrapped(self):
        with self.make({0: [127] * 64}) as pl:
            for v in range(16):
                self.voice(pl, v, 0, 0, 64, 64, 0x1000, pan=0)
            left, _ = self.channels(pl.render(100))
        self.assertEqual(left[50], 32767)

    def test_echo_repeats_with_the_feedback(self):
        with self.make({0: [100, 100, 0, 0]}) as pl:
            self.voice(pl, 0, 0, 0, 2, 0, 0x1000, pan=16, send=0x4000)    # a 2-sample impulse of the dry sound, then silence
            pl.chip_write(0x93, 0x4000)                                    # feedback 0.5
            pl.chip_write(0xd9, 0x554 + 100)                               # 100 chip samples of delay: 183 output frames
            left, right = self.channels(pl.render(800))
        self.assertEqual(left, right)
        dry = left[0]
        self.assertGreater(dry, 0)
        self.assertEqual(left[1], round(left[1]))
        d = 183
        self.assertAlmostEqual(left[d], dry * 0.5, delta=2)               # the send is 0x4000 / 0x8000
        self.assertAlmostEqual(left[2 * d], dry * 0.25, delta=2)          # then the feedback halves it each trip
        self.assertAlmostEqual(left[3 * d], dry * 0.125, delta=2)
        self.assertEqual(left[d - 1], 0)
        self.assertEqual(left[d + 40], 0)

    def test_no_echo_without_a_send_or_length(self):
        for send, length in ((0, 0x554 + 100), (0x4000, 0), (0x4000, 0x500)):
            with self.make({0: [100, 100, 0, 0]}) as pl:
                self.voice(pl, 0, 0, 0, 2, 0, 0x1000, pan=16, send=send)
                pl.chip_write(0x93, 0x4000)
                if length:
                    pl.chip_write(0xd9, length)
                left, _ = self.channels(pl.render(800))
            self.assertEqual(set(left[10:]), {0}, (send, length))

    def test_clock_is_250_hz_and_a_pass_every_fourth(self):
        s = TM.Synth()
        s.song(1, {0: TM.HEAD + bytes([TM.note(1, 5), 22, 0xff, 0xfc])})        # tempo 0x300: a note a pass; repeats for ever
        with cm().player(s.d, bytes(M.SAMPLES_SIZE)) as pl:
            pl.command(1)
            pl.render(RATE)
            ticks = sorted({t for t, r, v in pl.writes() if t})
        self.assertEqual(ticks[0], 3)
        self.assertEqual(ticks[-1], 247)                       # 250 interrupts in a second: the last pass is the 62nd, at 247
        self.assertEqual(ticks, list(range(3, 248, 4)))

    def test_block_size_does_not_matter(self):
        rng = random.Random(3)
        s = rich_image(rng)
        pack_all(s, rng, Gen(rng))
        smp = bytes(rng.randrange(256) for _ in range(M.SAMPLES_SIZE // 64)) * 64
        outs = []
        for block in (RATE, 777, 1, 4410):
            with cm().player(s.d, smp) as pl:
                pl.command(1)
                pl.command(3)
                raw, left = b'', 3 * RATE // 2
                while left:
                    n = min(block, left)
                    raw += pl.render(n)
                    left -= n
                outs.append(raw)
        self.assertTrue(any(outs[0]))
        self.assertTrue(all(o == outs[0] for o in outs))

    def test_a_song_is_audible_and_stereo_is_wider_than_mono(self):
        rng = random.Random(9)
        s = rich_image(rng)
        pack_all(s, rng, Gen(rng))
        smp = bytes(rng.randrange(256) for _ in range(M.SAMPLES_SIZE // 64)) * 64
        got = []
        for stereo in (True, False):
            with cm().player(s.d, smp, stereo) as pl:
                pl.command(1)
                left, right = self.channels(pl.render(RATE))
                got.append((left, right))
        self.assertTrue(any(got[0][0]))
        self.assertEqual(got[1][0], got[1][1])             # mono: every pan is the centre, so both channels are the same
        self.assertNotEqual(got[0][0], got[0][1])


# ---- a C program under the sanitizers, and the 32-bit build ----------------------------------------------------------------------------------
SMOKE = r'''
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "music.h"
static unsigned char *load(const char *p, size_t n) {
    FILE *f = fopen(p, "rb"); unsigned char *b = calloc(1, n);
    if (!f || !b || fread(b, 1, n, f) == 0) { fprintf(stderr, "cannot read %s\n", p); exit(2); }
    fclose(f); return b;
}
static unsigned long long seed;
static unsigned rnd(void) { seed = seed * 6364136223846793005ull + 1442695040888963407ull; return (unsigned)(seed >> 33); }
int main(int argc, char **argv) {
    struct arcade_rom r; struct arcade_music *m; int16_t *out; size_t frames, i, left; unsigned long long sum = 0;
    if (argc < 6) return 2;
    memset(&r, 0, sizeof r);
    r.sound = load(argv[1], ARCADE_SOUND_SIZE); r.samples = load(argv[2], ARCADE_SAMPLES_SIZE);
    frames = (size_t)atol(argv[5]);
    m = arcade_music_open(&r, 1); if (!m) return 3;
    out = malloc(4 * 4096);
    if (strcmp(argv[3], "render") == 0) {                      /* render CODE: raw samples to stdout */
        arcade_music_command(m, (unsigned)atoi(argv[4]));
        for (left = frames; left; ) { size_t n = left < 4096 ? left : 4096; arcade_music_render(m, out, n); fwrite(out, 4, n, stdout); left -= n; }
    } else {                                                   /* fuzz SEED: random packets while rendering; a checksum to stdout */
        seed = (unsigned)atoi(argv[4]);
        for (left = frames; left; ) {
            size_t n = left < 700 ? left : 700; unsigned char p[16];
            if (rnd() % 3 == 0) {
                for (i = 0; i < 16; i++) p[i] = (unsigned char)rnd();
                if (rnd() % 2) { p[0] = 0; p[1] = (unsigned char)(rnd() % 8); }
                if (rnd() % 3 == 0) { p[0] = 0xff; p[1] = (unsigned char)(rnd() % 16); }
                arcade_music_packet(m, p, 1 + rnd() % 16);
            }
            if (rnd() % 50 == 0) arcade_music_chip_write(m, rnd() % 0x100, rnd());
            arcade_music_render(m, out, n);
            for (i = 0; i < 2 * n; i++) sum = sum * 31 + (unsigned short)out[i];
            left -= n;
        }
        printf("%llu %u\n", sum, arcade_music_tracks(m));
    }
    arcade_music_close(m); free(out); free((void *)r.sound); free((void *)r.samples);
    return 0;
}
'''


def build_smoke(name, extra):
    src = Path(TMP.name) / 'smoke.c'
    src.write_text(SMOKE)
    exe = Path(TMP.name) / name
    r = subprocess.run(['cc', '-std=c99', '-O1', '-Wall', '-Wextra', '-Werror', *extra, '-I', str(INC), '-o', str(exe), str(src), str(SRC)],
                       capture_output=True, text=True, timeout=180)
    return exe, r


def files_for(seed):
    rng = random.Random(seed)
    s = rich_image(rng)
    pack_all(s, rng, Gen(rng))
    snd, smp = Path(TMP.name) / ('snd%d.bin' % seed), Path(TMP.name) / 'smp.bin'
    snd.write_bytes(bytes(s.d))
    if not smp.exists():
        smp.write_bytes(bytes(random.Random(1).randrange(256) for _ in range(M.SAMPLES_SIZE // 64)) * 64)
    return snd, smp


def junk_files(seed):
    rng = random.Random(seed)
    d = bytearray(rng.randrange(256) for _ in range(M.SOUND_SIZE))
    d[M.CMD_TABLE:M.CMD_TABLE + 2] = rng.choice([1, 2, 4, 0x3ff, 0xffff]).to_bytes(2, 'big')
    for i in range(0, 64, 4):                                   # entries that point into the junk, and a few past the end
        lin = rng.choice([0x20000 + rng.randrange(0x1000), rng.randrange(M.SOUND_SIZE), 0x3ffff, 0xffffff, 0])
        d[M.CMD_TABLE + 6 + i:M.CMD_TABLE + 10 + i] = bytes([lin >> 16, (lin >> 8) & 255, lin & 255, 0])
    for lin in range(0x20000, 0x20400, 0x40):
        d[lin] = rng.choice([0, 0, 1, 0x7f, 0x80])
        for v in range(16):
            d[lin + 1 + 2 * v] = rng.choice([0, 0, 0, 0, 0, 1, 2, 3])
    p = Path(TMP.name) / ('junk%d.bin' % seed)
    p.write_bytes(bytes(d))
    return p


class Sanitizers(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.exe, r = build_smoke('smoke-san', ['-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', '-fno-omit-frame-pointer', '-g'])
        if r.returncode:
            if 'sanitize' in r.stderr or 'libasan' in r.stderr or 'cannot find' in r.stderr:
                raise unittest.SkipTest('no sanitizer runtime: ' + r.stderr.splitlines()[0])
            raise AssertionError(r.stderr)
        cls.env = dict(os.environ, ASAN_OPTIONS='detect_leaks=1:abort_on_error=0', UBSAN_OPTIONS='halt_on_error=1:print_stacktrace=1')

    def run_smoke(self, snd, smp, *args):
        r = subprocess.run([str(self.exe), str(snd), str(smp), *args], capture_output=True, timeout=300, env=self.env)
        self.assertEqual((r.returncode, r.stderr.decode()[:2000]), (0, ''))
        return r.stdout

    def test_made_up_songs_render_clean(self):
        for seed in range(6):
            snd, smp = files_for(seed)
            self.run_smoke(snd, smp, 'render', str(1 + seed % 2), str(RATE * 3))
            self.run_smoke(snd, smp, 'fuzz', str(seed), str(RATE * 20))

    def test_junk_as_a_sound_program_never_crashes_or_hangs(self):
        for seed in range(12):
            snd, smp = junk_files(seed), files_for(0)[1]
            self.run_smoke(snd, smp, 'fuzz', str(seed), str(RATE * 20))


class ThirtyTwoBit(unittest.TestCase):
    def test_same_sound_as_the_64_bit_build_within_rounding(self):
        exe, r = build_smoke('smoke32', ['-m32'])
        if r.returncode:
            if any(w in r.stderr for w in ('-m32', 'bits/', 'gnu/stubs', 'No such file', 'cannot find', 'skipping incompatible')):
                self.skipTest('no 32-bit C library on this machine')
            raise AssertionError(r.stderr)
        snd, smp = files_for(2)
        frames = RATE * 3
        got = subprocess.run([str(exe), str(snd), str(smp), 'render', '1', str(frames)], capture_output=True, timeout=120)
        if got.returncode:
            self.skipTest('cannot run 32-bit programs here')
        a = array.array('h')
        a.frombytes(got.stdout)
        s = M.CMusic(TMP.name)
        with s.player(snd.read_bytes(), smp.read_bytes()) as pl:
            pl.command(1)
            b = array.array('h')
            b.frombytes(pl.render(frames))
        self.assertEqual(len(a), len(b))
        self.assertTrue(any(b))
        self.assertLessEqual(max(abs(x - y) for x, y in zip(a, b)), 2)


if __name__ == '__main__':
    # The line the review reads (tools/ai_workflow/review.py): the last line, on a pass only.
    started = time.monotonic()
    result = unittest.main(exit=False).result
    took = time.monotonic() - started
    bad, ran = len(result.failures) + len(result.errors), result.testsRun
    if bad or not ran:
        print(f"{bad} of {ran} tests FAILED in {took:.1f} seconds")
        sys.exit(1)
    print(f"all {ran} tests passed in {took:.1f} seconds")
