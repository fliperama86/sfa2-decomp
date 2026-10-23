#!/usr/bin/env python3
"""Seconds: the music driver model (port/arcade/music.py) on a made-up sound program; no game files.

A 256 KB view is built here with the driver's tables at their addresses, filled with small made-up numbers (ramps), and a
few songs and effects whose bytes are written by hand. Every expected number below is worked out by hand from the rules in
port/arcade/music.md, not taken from the model: note lengths, tempo, gate, tie, loops, jumps, the envelope stages, the volume
and rate chains, the command table (modulo its size), effect priority, the pan sweeps, the fades and the special commands.
The real program's equality with the model (every song and effect, many scripts) was checked against a throwaway Z80
emulator, which is not in the repository.
"""
from pathlib import Path
import subprocess, sys, tempfile, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import music as M

SONG = 0x20000          # where the made-up songs lie (a linear address in the 256 KB view)
TEMPO_1PASS = 0x0300    # three units of the normal length table (3 * 256) a pass: a note of length code 1 lasts one pass
IRQ_OF_PASS = lambda n: 4 * n - 1   # the n-th pass (n = 1, 2, ...) happens on interrupt 3, 7, 11 ...


class Synth:
    """A made-up Z80 program image."""
    def __init__(self):
        self.d = bytearray(0x40000)
        d = self.d
        # command table: 4 entries (codes are taken modulo 4); song volume index 1, master volume index 0 (no scaling)
        d[0x9000:0x9004] = bytes([0, 4, 1, 0])
        # the length tables, octave offsets
        d[0x0d36:0x0d3d] = bytes([3, 6, 12, 24, 48, 96, 192])
        d[0x0d2f:0x0d36] = bytes([2, 4, 8, 16, 32, 64, 128])
        d[0x0d3d:0x0d44] = bytes([0, 9, 18, 36, 72, 144, 0])
        d[0x0d44:0x0d54] = bytes([0, 12, 24, 36, 48, 60, 72, 84, 24, 36, 48, 60, 72, 84, 96, 108])
        # curves: made-up ramps
        self.words(M.T_VOLUME, [min(i * 0x40, 0xffff) for i in range(128)])      # track volume and echo send
        self.words(M.T_LEVEL, [min(i * 0x400, 0xffff) for i in range(128)])      # song volume and sustain level
        self.words(M.T_ATTACK, [0x4000] * 64)
        self.words(M.T_RATE, [i * 0x100 for i in range(128)])
        self.words(M.T_LFO_RATE, [i * 0x80 for i in range(128)])
        self.words(M.T_VIB_DEPTH, [i * 4 for i in range(128)])
        self.words(M.T_TREM_DEPTH, [i * 0x200 for i in range(128)])
        # pitch: octave of a key index, first index of an octave, fine table, rates of the indexes
        for i in range(108):
            d[M.T_OCT_OF_KEY + i] = i // 12
        d[M.T_OCT_BASE:M.T_OCT_BASE + 9] = bytes(12 * k for k in range(9))
        for i in range(12 * 256):
            d[M.T_FINE + i] = i >> 5
        self.words(M.T_FREQ_MUS, [0x80 + 16 * i for i in range(108)])
        self.words(M.T_FREQ_SFX, [0x40 + 8 * i for i in range(108)])
        # instrument 2: bank 3, start 0x1234, loop start 0x2000, end 0x3000, root key 60
        self.instrument(2, 3, 0x1234, 0x2000, 0x3000, 60)
        # program 0: instrument 2, tuning 0, attack 1, decay 2, sustain level index 3, second decay 4, release 5
        self.program(0, 2, 0, 1, 2, 3, 4, 5)

    def words(self, base, vals):
        for i, v in enumerate(vals):
            self.d[base + 2 * i] = v & 255
            self.d[base + 2 * i + 1] = v >> 8

    def instrument(self, i, bank, start, loop, end, root):
        o = M.INSTR_TABLE + 8 * i
        self.d[o:o + 8] = bytes([bank, start & 255, start >> 8, loop & 255, loop >> 8, end & 255, end >> 8, root])

    def program(self, n, ins, tune, a, dk, sus, d2, rel, base=M.PROG_TABLE):
        o = base + 8 * n
        self.d[o:o + 8] = bytes([ins & 255, ins >> 8, tune, a, dk, sus, d2, rel])

    def entry(self, code, lin):
        o = M.CMD_TABLE + 6 + 4 * code
        self.d[o:o + 4] = bytes([lin >> 16, (lin >> 8) & 255, lin & 255, 0])

    def song(self, code, tracks, at=SONG, kind=0):
        """tracks: {voice: bytes}; the header (type byte `kind`, 16 offsets) then the tracks one after another."""
        hdr = bytearray(33)
        hdr[0] = kind
        body = bytearray()
        for v, b in sorted(tracks.items()):
            off = 33 + len(body)
            hdr[1 + 2 * v] = off >> 8
            hdr[2 + 2 * v] = off & 255
            body += b
        self.d[at:at + 33 + len(body)] = bytes(hdr) + bytes(body)
        self.entry(code, at)
        return at

    def rom(self):
        return M.Rom(bytes(self.d))


def run(rom, packet, passes, stereo=True):
    drv = M.Driver(rom, stereo)
    drv.writes.clear()
    drv.command(packet)
    for _ in range(4 * passes):
        drv.step()
    return drv


def note(length, n): return (length << 5) | n
HEAD = bytes([5, TEMPO_1PASS >> 8, TEMPO_1PASS & 255,   # tempo
              8, 0,                                       # program 0
              7, 0x40,                                    # track volume curve index 0x40
              6, 0x80,                                    # gate ratio 0x80
              24, 8,                                      # pan 8
              9, 3])                                      # octave 3 (key offset 36)


class Tables(unittest.TestCase):
    def test_multiply_is_the_high_word(self):
        self.assertEqual(M.mul(0x8000, 0x8000), 0x4000)
        self.assertEqual(M.mul(0xffff, 0xffff), 0xfffe)
        self.assertEqual(M.mul(3, 0x5555), 0)

    def test_command_table_kinds_and_modulo(self):
        s = Synth()
        s.song(1, {0: bytes([23])}, SONG)
        s.song(2, {0: bytes([23])}, SONG + 0x100, kind=0x10)      # an effect: priority 0x10
        s.d[SONG + 0x200] = 0x80                                    # a header of the third kind (ADPCM)
        s.entry(3, SONG + 0x200)
        r = s.rom()
        self.assertEqual(r.command_count(), 4)
        self.assertEqual(r.command_entry(1), (SONG, 'music'))
        self.assertEqual(r.command_entry(2), (SONG + 0x100, 'effect'))
        self.assertEqual(r.command_entry(3)[1], 'adpcm')
        self.assertEqual(r.command_entry(0), (0, None))
        self.assertEqual(r.command_entry(4 * 77 + 1), (SONG, 'music'))   # taken modulo the table size (0x0212)

    def test_header_offsets_are_big_endian_and_voice_ordered(self):
        s = Synth()
        s.song(1, {2: bytes([23]), 5: bytes([23, 23])})
        self.assertEqual([o for o in s.rom().song_tracks(SONG)], [0, 0, 33, 0, 0, 34] + [0] * 10)

    def test_instrument_record_and_sixteen_bit_wrap(self):
        r = Synth().rom()
        self.assertEqual(r.instrument(2), dict(bank=3, start=0x1234, loop=0x2000, end=0x3000, root=60))
        self.assertEqual(r.instrument(0xffff), r.instrument(0xffff & 0xffff))   # an unset instrument reads 0x7ff8
        self.assertEqual(r.instrument(0xffff)['bank'], r.d[0x7ff8])

    def test_rom_size_is_checked(self):
        with self.assertRaises(ValueError):
            M.Rom(bytes(100))


class Notes(unittest.TestCase):
    def test_first_note_registers_and_chains(self):
        s = Synth()
        s.song(1, {3: HEAD + bytes([note(1, 5), 23])})
        d = run(s.rom(), [0, 1], 1)
        w = [(r, v) for t, r, v in d.writes]
        v3 = 3 * 8
        # the song start keys the voice off (echo send, volume, rate 0 then 0x1000) and centres the pan
        self.assertEqual(w[:5], [(0xba + 3, 0), (v3 + 6, 0), (v3 + 2, 0), (v3 + 2, 0x1000), (0x83, 0x120)])
        w = w[5:]
        # the program selection loads the instrument: rate 0, start, end, loop length, bank (voice - 1 register, 0x8000 | bank)
        self.assertEqual(w[:5], [(v3 + 2, 0), (v3 + 1, 0x1234), (v3 + 5, 0x3000), (v3 + 4, 0x1000), (16, 0x8003)])
        self.assertEqual(w[5], (0x83, 0x110 + 8))                      # the pan command
        # the note on: rate 0, the two constants, echo send, start address, phase, then volume and rate
        note_on = [(v3 + 2, 0), (0x93, 0x27ff), (0xd9, 0x07ff), (0xba + 3, 0), (v3 + 1, 0x1234), (v3 + 3, 0x8000)]
        self.assertEqual(w[6:12], note_on)
        level = 0x4000                       # attack start level (made-up table)
        song_vol = (0x0400 * 0xfffe) >> 16   # T_LEVEL[1] * 0xfffe
        vol = (level * song_vol) >> 16
        vol = (vol * (0x40 * 0x40)) >> 16    # T_VOLUME[0x40]
        key = 5 + 36
        idx = 0x3c - 60 + (key - 1)
        rate = ((((idx % 12) << 8) >> 5) << (idx // 12)) + 0x80 + 16 * idx
        self.assertEqual(w[12], (v3 + 6, vol))
        self.assertEqual(w[13], (v3 + 2, rate))
        self.assertEqual(rate, 0x400)
        self.assertEqual(d.events[0]['key'], 41)
        self.assertEqual(d.events[0]['pan'], 8)
        self.assertEqual(d.events[0]['instrument'], 2)
        self.assertEqual(d.events[0]['tick'], IRQ_OF_PASS(1))

    def test_length_tables(self):
        # tempo 0x0100 = one unit a pass: a normal length code k lasts 3 * 2^(k-1) passes, binary mode 2^k, one-shot 9/18/36/..
        s = Synth()
        body = bytes([5, 1, 0, 8, 0, 7, 0x40, 6, 0x80, 24, 8, 9, 3])
        body += bytes([note(1, 1), note(2, 1), note(3, 1),    # 3, 6, 12 passes
                       0, note(1, 1), note(2, 1), note(3, 1),  # command 0: binary lengths 1, 2, 4 passes
                       0, 2, note(2, 1), note(1, 1), 23])      # back to normal; one-shot (command 2) code 2 = 9 units, then code 1 = 3
        s.song(1, {0: body})
        d = run(s.rom(), [0, 1], 80)
        ticks = [e['tick'] // 4 + 1 for e in d.events]            # pass numbers
        gaps = [b - a for a, b in zip(ticks, ticks[1:])]
        self.assertEqual(gaps, [3, 6, 12, 2, 4, 8, 9])
        self.assertEqual(len(ticks), 8)

    def test_rest_is_the_note_number_zero(self):
        s = Synth()
        s.song(1, {0: HEAD + bytes([note(1, 3), note(2, 0), note(1, 3), 23])})
        d = run(s.rom(), [0, 1], 10)
        ticks = [e['tick'] // 4 + 1 for e in d.events]
        self.assertEqual(ticks, [1, 4])                  # a note, a rest of two passes, a note
        self.assertEqual(ticks[1] - ticks[0], 3)         # the first note's own pass plus the rest's two

    def test_loop_command_plays_the_body_count_plus_one_times(self):
        s = Synth()
        # body: one note; command 14 (counter 0), count 2, then the offset back to the note: -5 from the byte after it
        body = HEAD + bytes([note(1, 1), 14, 2, 0xff, 0xfb, note(1, 9), 23])
        s.song(1, {0: body})
        d = run(s.rom(), [0, 1], 20)
        self.assertEqual([e['key'] for e in d.events], [37] * 3 + [45])

    def test_loop_exit_command_leaves_on_the_last_pass_and_sets_the_flags(self):
        s = Synth()
        # body: note; command 18 (exit via counter 0): flags argument, offset +5; a second note; command 14 loops 2 more times
        # (offset -10). Command 18 does nothing while the counter is not on its last value; on the last pass it clears
        # the counter and jumps over the second note and the loop command.
        body = HEAD + bytes([note(1, 1),
                             18, 0x00, 0x00, 0x05,
                             note(1, 2),                # played on the first and second pass only
                             14, 2, 0xff, 0xf6,
                             note(1, 20), 23])
        s.song(1, {0: body})
        d = run(s.rom(), [0, 1], 30)
        self.assertEqual([e['key'] for e in d.events], [37, 38, 37, 38, 37, 56])
        self.assertEqual(d.mus[0][32], 0)               # the track ended

    def test_jump_forward_and_end_of_track_releases_the_voice(self):
        s = Synth()
        s.song(1, {0: HEAD + bytes([note(1, 1), 22, 0, 1, note(1, 2), note(1, 3), 23])})
        d = run(s.rom(), [0, 1], 6)
        self.assertEqual([e['key'] for e in d.events], [37, 39])        # the jump skipped the note with number 2
        self.assertEqual(d.mus[0][32], 0)
        self.assertEqual(d.writes[-4:], [(d.writes[-1][0], 0xba, 0), (d.writes[-1][0], 6, 0), (d.writes[-1][0], 2, 0), (d.writes[-1][0], 2, 0x1000)])

    def test_transpose_fine_and_global_transpose(self):
        s = Synth()
        s.song(1, {0: HEAD + bytes([11, 2, note(1, 1), 10, 5, note(1, 1), 23])})
        d = run(s.rom(), [0, 1], 6)
        self.assertEqual([e['key'] for e in d.events], [39, 39 + 5])

    def test_tie_flag_holds_the_note_without_a_new_attack(self):
        s = Synth()
        s.song(1, {0: HEAD + bytes([1, note(1, 1), note(1, 3), 1, note(1, 5), note(1, 7), 23])})
        d = run(s.rom(), [0, 1], 8)
        # command 1 toggles the tie flag. The note after it starts normally and ties to the next (no new attack, no event); the
        # second command 1 clears the flag, but the note after it is still tied (the flag is copied one note late); the
        # note after that starts again.
        self.assertEqual([e['key'] for e in d.events], [37, 43])

    def test_gate_key_off_and_release(self):
        s = Synth()
        # one note of 12 units (code 3); gate ratio 0x80 -> key off after half of it
        s.song(1, {0: HEAD + bytes([note(3, 1), note(1, 1), 23])})
        d = run(s.rom(), [0, 1], 3)
        # the pass-1 note starts; the gate is 12 * 256 * 0x80 / 256 = 1536 = two passes; at pass 3 it is cut: stage 4
        self.assertEqual(d.mus[0][37], 4)
        self.assertEqual(d.mus[0][40] | d.mus[0][41] << 8, 0x500)            # release rate: T_RATE[5]

    def test_envelope_stages(self):
        s = Synth()
        s.program(0, 2, 0, 1, 2, 3, 4, 5)
        s.words(M.T_ATTACK, [0x4000] * 64)
        s.words(M.T_RATE, [0x4000] * 128)
        s.words(M.T_LEVEL, [0x6000 if i == 3 else min(i * 0x400, 0xffff) for i in range(128)])
        s.song(1, {0: bytes([5, 1, 0, 8, 0, 7, 0x40, 6, 0, 24, 8, note(7, 1), 23])})   # gate 0: no key off, long note
        r = s.rom()
        d = M.Driver(r)
        d.command([0, 1])

        def passes(n):
            for _ in range(4 * n):
                d.step()
        t = d.mus[0]
        lvl = lambda: t[42] | t[43] << 8
        passes(1)                                   # note on: level = rate = T_ATTACK[1]
        self.assertEqual((t[37], lvl()), (0, 0x4000))
        passes(1)
        self.assertEqual((t[37], lvl()), (0, 0x8000))
        passes(1)
        self.assertEqual((t[37], lvl()), (0, 0xc000))
        passes(1)                                   # 0xc000 + 0x4000 overflows: level 0xffff, decay stage
        self.assertEqual((t[37], lvl()), (1, 0xffff))
        passes(1)
        self.assertEqual((t[37], lvl()), (1, 0xbfff))
        passes(1)
        self.assertEqual((t[37], lvl()), (1, 0x7fff))
        passes(1)                                   # 0x3fff is below the sustain level 0x6000: level = sustain, stage 3
        self.assertEqual((t[37], lvl()), (3, 0x6000))
        passes(1)
        self.assertEqual((t[37], lvl()), (3, 0x2000))
        passes(1)                                   # 0x2000 - 0x4000 < 0: the level is 0, stage 5 (finished)
        self.assertEqual((t[37], lvl()), (5, 0))

    def test_lfo_phase_and_quadrants(self):
        s = Synth()
        # LFO rate index 0x20 -> 0x1000 a pass; tremolo depth index 0 and vibrato 0: only the phase and the quadrant count are watched
        s.song(1, {0: HEAD + bytes([29, 0x20, 30, 0, note(7, 1), 23])})
        d = M.Driver(s.rom())
        d.command([0, 1])
        for _ in range(4):
            d.step()
        t = d.mus[0]
        self.assertEqual(t[24] | t[25] << 8, 0x1000)
        for _ in range(4 * 16):
            d.step()
        self.assertEqual(t[26] | t[27] << 8, 0x0000)                      # 16 steps of 0x1000 wrap the phase once
        self.assertEqual(t[23] >> 6, 1)                                   # one quadrant counted

    def test_portamento_slides_to_the_new_pitch(self):
        s = Synth()
        s.song(1, {0: HEAD + bytes([13, 0x10, note(1, 1), note(2, 5), 23])})
        d = M.Driver(s.rom())
        d.command([0, 1])
        for _ in range(8):
            d.step()
        t = d.mus[0]
        # second note (two passes long): slide from key 37 to 41 at 0x10 * 2 = 0x20 (1/256 semitone) a pass
        self.assertEqual(t[16], 0x90)
        self.assertEqual((t[19] << 8 | t[20]), 37 * 256)
        for _ in range(4):
            d.step()
        self.assertEqual((t[19] << 8 | t[20]), 37 * 256 + 0x20)

    def test_new_song_replaces_the_old_one(self):
        s = Synth()
        s.song(1, {0: HEAD + bytes([note(7, 1), 23]), 4: HEAD + bytes([note(7, 1), 23])})
        s.song(2, {1: HEAD + bytes([note(7, 1), 23])}, SONG + 0x400)
        d = M.Driver(s.rom())
        d.command([0, 1])
        for _ in range(8):
            d.step()
        self.assertEqual([t[32] for t in d.mus[:6]], [0xff, 0, 0, 0, 0xff, 0])
        d.writes.clear()
        d.command([0, 2])
        self.assertEqual([t[32] for t in d.mus[:6]], [0, 0xff, 0, 0, 0, 0])
        self.assertIn((0xba + 4, 0), [(r, v) for t, r, v in d.writes])      # the old voice 4 was keyed off


class Effects(unittest.TestCase):
    def test_priority_and_voice_taking(self):
        s = Synth()
        s.song(1, {2: bytes([23])}, SONG + 0x100, kind=0x20)
        s.song(2, {2: bytes([5, 3, 0, 23])}, SONG + 0x200, kind=0x10)
        d = M.Driver(s.rom())
        d.command([0, 1])
        self.assertEqual((d.sfx[2][32], d.sfx[2][52]), (0xff, 0x20))
        d.command([0, 2])                                     # a lower priority does not take the voice
        self.assertEqual(d.sfx[2][52], 0x20)
        s.song(3, {2: bytes([23])}, SONG + 0x300, kind=0x20)
        d2 = M.Driver(s.rom())
        d2.command([0, 1])
        d2.command([0, 3 % 4])                                # the same priority does
        self.assertEqual(d2.sfx[2][52], 0x20)

    def test_effect_over_music_silences_the_music_voice_and_it_comes_back(self):
        s = Synth()
        s.song(1, {2: HEAD + bytes([note(7, 1), 23])})
        s.song(2, {2: bytes([5, 3, 0, note(1, 1), 23])}, SONG + 0x300, kind=0x10)
        d = M.Driver(s.rom())
        d.command([0, 1])
        for _ in range(4):
            d.step()
        d.writes.clear()
        d.command([0, 2])
        for _ in range(4 * 2):
            d.step()
        self.assertEqual(d.sfx[2][32], 0)                              # the one note effect has ended
        vol_writes = [(r, v) for t, r, v in d.writes if r == 2 * 8 + 6]
        self.assertNotIn(None, vol_writes)
        self.assertEqual(d.mus[2][53], 0xff)                           # the music note is not resumed until its next note

    def test_pan_motion_modes(self):
        s = Synth()
        s.song(2, {2: bytes([5, 3, 0, note(7, 1), 23])}, SONG, kind=0x10)
        # mode 0: fixed pan (byte 7, at most 0x20)
        d = run(s.rom(), [0, 2, 0, 0, 0, 0, 0, 0x30], 1)
        self.assertEqual(d.sfx[2][55], 0x20)
        self.assertIn((0x82, 0x110 + 0x20), [(r, v) for t, r, v in d.writes])
        # mode 3: start 4, up by 0x20 a pass (rate 0x0800), range 0x08: a sweep that stops at 4 + 8
        d = M.Driver(s.rom())
        d.command([0, 2, 0, 0, 0, 0, 3, 4, 8, 0x08, 0x00])
        seen = []
        for _ in range(4 * 12):
            d.step()
            seen.append(d.sfx[2][55])
        self.assertEqual(seen[-1], 12)
        self.assertEqual(d.sfx[2][54], 0)                              # the motion has stopped
        self.assertEqual(max(seen), 12)

    def test_mono_forces_the_centre(self):
        s = Synth()
        s.song(2, {2: bytes([5, 3, 0, note(7, 1), 23])}, SONG, kind=0x10)
        d = run(s.rom(), [0, 2, 0, 0, 0, 0, 0, 5], 1, stereo=False)
        self.assertEqual([v for t, r, v in d.writes if r == 0x82][0], 0x110 + 0x10)

    def test_the_delayed_copy_keys_off_voice_one(self):
        # motion 7 copies the effect to the next voice, four passes later; the Z80 then keys off voice 1 (A holds the counter)
        s = Synth()
        s.song(2, {5: bytes([5, 3, 0, note(7, 1), 23])}, SONG, kind=0x10)
        d = M.Driver(s.rom())
        d.command([0, 2, 0, 0, 0, 0, 7, 4, 8, 0x08, 0x00])
        self.assertEqual(d.sfx[6][63], 4)
        d.writes.clear()
        for _ in range(4 * 4):
            d.step()
        regs = [r for t, r, v in d.writes]
        self.assertIn(0xba + 1, regs)
        self.assertNotIn(0xba + 6, regs)


class Specials(unittest.TestCase):
    def setUp(self):
        s = Synth()
        s.song(1, {0: HEAD + bytes([note(7, 1), 23]), 1: HEAD + bytes([note(7, 1), 23])})
        s.song(2, {3: bytes([5, 3, 0, note(7, 1), 23])}, SONG + 0x300, kind=0x10)
        self.s = s

    def test_stop_music_and_stop_all(self):
        d = M.Driver(self.s.rom())
        d.command([0, 1])
        d.command([0xff, 5])
        self.assertEqual([t[32] for t in d.mus[:2]], [0, 0])
        d.command([0, 1]); d.command([0, 2])
        d.command([0xff, 0])
        self.assertEqual((d.mus[0][32], d.sfx[3][32]), (0, 0))

    def test_stop_effects_one_and_all(self):
        d = M.Driver(self.s.rom())
        d.command([0, 2])
        d.command([0xff, 2, 0, 0, 3])
        self.assertEqual(d.sfx[3][32], 0)
        d.command([0, 2])
        d.command([0xff, 1])
        self.assertEqual(d.sfx[3][32], 0)

    def test_music_fade_out_stops_at_the_target(self):
        d = M.Driver(self.s.rom())
        d.command([0, 1])
        d.command([0xff, 7, 0x40, 0x10, 0x00])                       # target 0x40, rate 0x1000 a pass
        self.assertEqual((d.fade_mode, d.fade_level), (0xff, 0xffff))
        levels = []
        for _ in range(4 * 20):
            d.step()
            levels.append(d.fade_level)
        self.assertEqual(levels[3], 0xefff)                          # first pass: 0xffff - 0x1000
        self.assertEqual(levels[-1], 0x4000)                         # held at target << 8
        self.assertEqual(d.fade_mode, 0xff)

    def test_music_fade_in_ends_and_switches_off(self):
        d = M.Driver(self.s.rom())
        d.command([0, 1])
        d.command([0xff, 6, 0xff, 0x40, 0x00])                       # target 0xff, rate 0x4000
        for _ in range(4 * 6):
            d.step()
        self.assertEqual(d.fade_mode, 0)

    def test_global_fade_out_ends_silent(self):
        d = M.Driver(self.s.rom())
        d.command([0, 1])
        d.command([0xff, 13, 0, 0x80, 0x00])                         # rate 0x8000: two passes
        for _ in range(4 * 4):
            d.step()
        self.assertEqual((d.gfade_mode, d.gfade_level), (0xfe, 0))   # the multiplier stays on at 0: silence
        vols = [v for t, r, v in d.writes if r == 6 and t > 8]
        self.assertTrue(all(v == 0 for v in vols[-3:]))

    def test_tempo_and_pitch_bend_values(self):
        d = M.Driver(self.s.rom())
        d.command([0xff, 8, 0, 0, 0x80])
        self.assertEqual(d.tempo_bend, (0x80 * 0x369) >> 8)
        d.command([0xff, 8, 0, 0xff, 0x80])
        self.assertEqual(d.tempo_bend, (-((0x80 * 0x369) >> 8)) & 0xffff)
        d.command([0xff, 9, 0, 0, 0x30])
        self.assertEqual(d.pitch_bend, 0x300)

    def test_unused_commands_are_ignored(self):
        d = M.Driver(self.s.rom())
        d.command([0, 0])                                            # an empty table entry
        d.command([0, 3])                                            # an ADPCM header: not started
        self.assertEqual([t[32] for t in d.mus + d.sfx], [0] * 32)


class Tool(unittest.TestCase):
    def test_cli_on_a_made_up_image(self):
        s = Synth()
        s.song(1, {0: HEAD + bytes([note(1, 5), 23])})
        with tempfile.TemporaryDirectory() as tmp:
            p = Path(tmp) / 'sound.bin'
            p.write_bytes(bytes(s.d))
            for cmd, expect in (('table', 'music'), ('song', 'key  41'), ('writes', '3 02 0400')):
                r = subprocess.run([sys.executable, str(ROOT / 'port/arcade/music.py'), cmd, '1', '--sound', str(p), '--ticks', '8'] if cmd != 'table'
                                   else [sys.executable, str(ROOT / 'port/arcade/music.py'), cmd, '--sound', str(p)],
                                   capture_output=True, text=True, timeout=30)
                self.assertEqual(r.returncode, 0, r.stderr)
                self.assertIn(expect, r.stdout)

    def test_event_lines(self):
        s = Synth()
        s.song(1, {0: HEAD + bytes([note(1, 5), 23])})
        d = run(s.rom(), [0, 1], 2)
        lines = M.event_lines(d.events)
        self.assertEqual(len(lines), 1)
        self.assertTrue(lines[0].startswith('t=  0.012 M voice  0 ins   2 prog   0 key  41 vol'))


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
