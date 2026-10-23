#!/usr/bin/env python3
"""Reference model of the arcade's music driver (the Z80 sound program sz2.01a + sz2.02a); analysis tool.

What it does. The arcade's sound CPU runs one program that plays the 59 songs and 868 sound effects of the game on the
QSound chip. This module is the executable form of port/arcade/music.md: it reads the song table and the driver's data
tables out of the Z80 program (a data file of the arcade set; no Z80 code is run, nothing is decrypted), decodes a song's
tracks, and runs the driver's own algorithm (track interpreter, note envelopes, low frequency oscillator, slides, fades,
pitch and volume formulas) tick by tick, as plain Python of our own. The result is the stream of QSound register
writes the Z80 program would make, and from it a list of note events. The runtime player is port/arcade/music.c (music.h), C that follows this model statement by statement; `--compare-c`
builds it into a temporary folder and checks that its register writes (tick, register, value) equal this model's, for the 59
songs, the 868 sound effects (with every pan motion) and random scripts of commands, and `--render` plays a song or effect through
its mixer to a WAV file, which is the way to listen to it. Both are private tools: they need the arcade set and write nothing
but the WAV file named.

Inputs: --arcade ZIP (MAME's sfa2u; only sz2.01a and sz2.02a are read) or --sound FILE (the two members one after
the other, 262,144 bytes). It writes nothing. Commands:

    python3 port/arcade/music.py table  --arcade ZIP                      # the command table: one line per kind and range
    python3 port/arcade/music.py song N --arcade ZIP [--ticks 2000]       # one song: header, tracks, then its note events as text
    python3 port/arcade/music.py writes N --arcade ZIP [--ticks 500]      # the QSound register writes (tick reg value)
    python3 port/arcade/music.py all    --arcade ZIP                      # one line per song: tracks, notes, length
    python3 port/arcade/music.py --compare-c --arcade ZIP [--sanitize]    # the C player's register writes against this model's
    python3 port/arcade/music.py --render CODE --arcade ZIP --seconds N --out FILE.wav   # listen: the C player's mixer to a WAV

A tick is one pass of the driver over all tracks: 62.5 per second (the chip's 250 Hz interrupt, every fourth one).
Verification (2026-10-23): a throwaway Z80 emulator run on the same program produced the register writes of every
song (59 songs x 6000 ticks, and 868 sound effects); the writes of this model are equal to them (see music.md,
"Checked against"). The emulator is not part of the repository and the port will not have one.
"""
import argparse
import ctypes
import os
import random
import struct
import subprocess
import sys
import tempfile
import time
import wave
import zipfile
from pathlib import Path

# ---- addresses in the Z80 program (file offset == Z80 address in the fixed 32 KB; above 0x8000 a "linear address"
# is the file offset, and the driver's bank = (linear >> 14) - 2 puts it in the window at 0x8000).
INSTR_TABLE = 0x8000     # 8-byte records: bank, start, loop start, end (16-bit LE), root key
CMD_TABLE = 0x9000       # u16 BE count N, u8 song-volume index, u8 master-volume index, then entries of 4 bytes at 0x9006
PROG_TABLE = 0x7000      # envelope programs, 8 bytes each (control command 8); 0x7800 for command 31
T_DURATION = (0x0d36, 0x0d2f, 0x0d3d)       # note length tables: normal, binary (flag bit 5), one-shot (flag bit 4)
T_OCTAVE = 0x0d44        # 16 bytes: key offset added by flag bits 0-3
T_VOLUME = 0x25e9        # 128 u16: track volume / echo send curve
T_LEVEL = 0x26e9         # 128 u16: song volume / sustain level
T_ATTACK = 0x27e9        # 64 u16: attack start level and rate
T_RATE = 0x2869          # 128 u16: envelope rates (the first 64 equal T_ATTACK + 64)
T_LFO_RATE = 0x28e9
T_VIB_DEPTH = 0x29e9
T_TREM_DEPTH = 0x2ae9
T_OCT_OF_KEY = 0x1542    # 108 bytes: octave of a key index
T_OCT_BASE = 0x15ae      # 9 bytes: first key index of an octave
T_FINE = 0x1767          # 12 * 256 bytes: pitch correction within an octave (key remainder, fraction)
T_FREQ_SFX = 0x15b7      # 108 u16: rate of key index for sound effects
T_FREQ_MUS = 0x168f      # 108 u16: rate of key index for music

# ---- offsets in a track's 64-byte state (named for the model; the Z80 keeps them in RAM at 0xf200 + 0x40 * voice for sound
# effects and 0xf600 + 0x40 * voice for music)
PTR, FLAGS, DUR, GATE_T, GATE_RATIO = 0, 3, 4, 7, 6
TRANSPOSE, FINE, INS_FINE, ROOT, ECHO = 9, 10, 12, 11, 15

def u16le(b, a): return b[a] | b[a + 1] << 8
def mul(a, b): return (a * b) >> 16        # 0x2c0a: HL * DE >> 16, exactly (checked on 200,000 random pairs)
def s8(v): return v - 256 if v >= 128 else v


class Rom:
    """The Z80 program as a flat 256 KB view; `lin` addresses are file offsets."""
    def __init__(self, data):
        if len(data) != 0x40000:
            raise ValueError('the sound program view is 262144 bytes')
        self.d = bytes(data)

    @classmethod
    def from_zip(cls, path):
        with zipfile.ZipFile(path) as z:
            return cls(z.read('sz2.01a') + z.read('sz2.02a'))

    def w(self, a): return self.d[a] | self.d[a + 1] << 8
    def tab(self, base, i): return self.d[base + 2 * i] | self.d[base + 2 * i + 1] << 8

    def command_count(self): return self.d[CMD_TABLE] << 8 | self.d[CMD_TABLE + 1]

    def command_entry(self, code):
        """(linear address, kind) of a command code, kind 'music', 'effect' or None (empty). The code is taken
        modulo the table size, as the Z80 does (0x0212)."""
        n = self.command_count()
        o = CMD_TABLE + 6 + 4 * (code % n)
        lin = self.d[o] << 16 | self.d[o + 1] << 8 | self.d[o + 2]
        if lin == 0:
            return 0, None
        h = self.d[lin]
        return lin, 'music' if h == 0 else 'effect' if h < 0x80 else 'adpcm'

    def song_tracks(self, lin):
        """Music header at `lin`: byte 0 (type 0), then 16 big-endian offsets from `lin`; 0 = unused voice."""
        return [(self.d[lin + 1 + 2 * i] << 8 | self.d[lin + 2 + 2 * i]) for i in range(16)]

    def instrument(self, i):
        o = (INSTR_TABLE + 8 * i) & 0xffff      # the Z80 adds in 16 bits (an unset instrument, 0xffff, reads 0x7ff8)
        d = self.d
        return dict(bank=d[o], start=u16le(d, o + 1), loop=u16le(d, o + 3), end=u16le(d, o + 5), root=d[o + 7])


class Track:
    """The 64 bytes of a track's state, indexed as the Z80 keeps them (see music.md, "Track state"); `program` is the
    last envelope program selected, kept only for the note events."""
    def __init__(self):
        self.t = bytearray(64)
        self.program = None
    def __getitem__(self, i): return self.t[i]
    def __setitem__(self, i, v): self.t[i] = v & 255
    def ptr(self): return self.t[0] | self.t[1] << 8 | self.t[2] << 16
    def set_ptr(self, v): self.t[0] = v & 255; self.t[1] = (v >> 8) & 255; self.t[2] = (v >> 16) & 255


class TrackEnd(Exception):
    """Raised by the end-of-track command: leaves the interpreter (the Z80 pops its return address)."""


class Driver:
    """The driver state: 16 sound-effect tracks, 16 music tracks, the global fades. `writes` collects
    (tick, register, value) of every write to the QSound chip (registers: see music.md)."""

    def __init__(self, rom, stereo=True):
        self.rom = rom
        self.stereo = stereo
        self.sfx = [Track() for _ in range(16)]
        self.mus = [Track() for _ in range(16)]
        self.writes = []
        self.events = []
        self.tick = 0
        self.irq = 0
        self.music = False              # 0xf006: the track being processed is a music track
        self.sfx_on = False             # 0xf020: a sound effect holds the voice being processed
        self.voice = 0                  # 0xf00d
        self.tempo = 0                  # 0xf010: decrement of the duration counters this pass
        self.song_tempo = 0             # 0xf012
        self.tempo_bend = 0             # 0xf014 (signed, added to the tempo)
        self.transpose = 0              # 0xf01a: whole-song transposition, semitones
        self.pitch_bend = 0             # 0xf01b: 1/256 semitone units
        rom_d = rom.d
        self.vol_scale = rom.tab(T_LEVEL, rom_d[CMD_TABLE + 2])      # 0xf016 from the byte at 0x9002
        self.master_scale = rom.tab(T_LEVEL, rom_d[CMD_TABLE + 3])   # 0xf042 from the byte at 0x9003
        self.song_vol = 0               # 0xf018
        self.fade_mode = 0              # 0xf035: 0 none, 1 fade in, 0xff fade out (the music fade)
        self.fade_level = 0             # 0xf038
        self.fade_rate = 0              # 0xf036
        self.fade_target = 0            # 0xf03a
        self.gfade_mode = 0             # 0xf030: the global fade (also sound effects)
        self.gfade_level = 0            # 0xf033
        self.gfade_rate = 0             # 0xf031
        self.report_pan = False         # 0xf041
        self.reset_chip()

    # ---- QSound writes ---------------------------------------------------------------------------------------
    def wr(self, reg, val):
        self.writes.append((self.tick, reg, val & 0xffff))

    def reset_chip(self):
        """0x09ec: the constants the driver gives the chip, then every voice off (0x0a4c)."""
        for reg, val in ((0xde, 0), (0xdf, 0x2e), (0xe0, 0), (0xe1, 0x30), (0xe4, 0x3fff), (0xe5, 0x3fff), (0xe6, 0x3fff),
                         (0xe7, 0x3fff), (0xe2, 0xffff), (0xda, 0x0db2), (0xdc, 0x0e11), (0xe3, 0x39)):
            self.wr(reg, val)
        for v in range(16):
            self.key_off(v)

    def key_off(self, v):
        """0x0a5b: echo send 0, volume 0, rate 0 then 0x1000 (8-bit register arithmetic, as the Z80 does)."""
        v &= 255
        self.wr((0xba + v) & 255, 0)
        c = ((v << 3) | (v >> 5)) & 255                 # RLC C three times
        self.wr((c + 6) & 255, 0)
        self.wr((c + 2) & 255, 0)
        self.wr((c + 2) & 255, 0x1000)

    def pan_write(self, v, e):
        """0x0ac7 / 0x0ade: pan register = 0x110 + pan (0..0x20, 0x10 centre); mono output forces the centre."""
        if not self.stereo:
            e = 0x10
        self.wr(0x80 + v, 0x110 + e)

    # ---- byte reader (0x0b6a / 0x1106): 24-bit linear pointer in the track ---------------------------------------
    def rd(self, t):
        p = t.ptr()
        v = self.rom.d[p]
        t.set_ptr(p + 1)
        return v

    # ---- command packets from the 68000 (0x01e6) -------------------------------------------------------------
    def command(self, packet):
        """`packet`: up to 16 bytes as the 68000 writes them to the shared RAM (code high byte, code low byte, parameters)."""
        iy = (bytes(packet) + bytes(16))[:16]
        self.iy = iy
        if iy[0] == 0xff:
            self.special(iy)
            return
        lin, kind = self.rom.command_entry(iy[0] << 8 | iy[1])
        if kind is None:
            return
        self.base = lin
        h = self.rom.d[lin]
        if h == 0:
            self.start_song(lin)
        elif h < 0x80:
            self.start_effect(lin, h, iy)

    def start_song(self, lin):
        """0x04b6: replace the music. Header: type 0, then 16 offsets (voice 0..15) from the header's address."""
        self.song_tempo = 0
        self.transpose = 0
        self.fade_mode = 0
        self.tempo_bend = 0
        self.pitch_bend = 0
        self.song_vol = mul(self.vol_scale, 0xfffe)
        d = self.rom.d
        for v in range(16):
            off = d[lin + 1 + 2 * v] << 8 | d[lin + 2 + 2 * v]
            t = self.mus[v]
            if off == 0:
                was = t[32]
                t[32] = 0
                if was:
                    self.music_voice_released(v)
                continue
            t.t[:] = bytes(64)
            t[32] = 0xff
            t.set_ptr(lin + off)
            t[49] = 0xff
            t[23] = 1
            t[37] = 5
            t[55] = 0x10
            self.music_voice_released(v)

    def music_voice_released(self, v):
        """0x0540: the voice is free of a sound effect: key off and centre the pan."""
        if self.sfx[v][32] == 0:
            self.key_off(v)
            self.pan_write(v, 0x10)

    def start_effect(self, lin, prio, iy):
        """0x0241: a sound effect: priority byte, then 16 offsets; a voice is taken unless an effect of higher priority holds it."""
        d = self.rom.d
        for v in range(16):
            off = d[lin + 1 + 2 * v] << 8 | d[lin + 2 + 2 * v]
            if off == 0:
                continue
            t = self.sfx[v]
            if prio < t[52]:
                continue
            t.t[:] = bytes(64)
            t[32] = 0xff
            t.set_ptr(lin + off)
            t[52] = prio
            t[49] = 0xff
            t[23] = 1
            t[37] = 5
            self.eff_prio = prio
            self.effect_pan_setup(t, iy, v)
            self.key_off(v)
            self.pan_write(v, t[55])

    def effect_pan_setup(self, t, iy, v):
        """0x02be: packet byte 6 (low 3 bits) picks the pan motion m, bytes 7..12 are its parameters. t[54] = m + 1 for the
        motions (0 = fixed pan). Pan p is the 16-bit value t[55]:t[56] (integer part 0..0x20 first); the rate is t[60]:t[59]."""
        m = iy[6] & 7
        p7, p8 = iy[7], iy[8]
        rate = iy[9] << 8 | iy[10]
        neg = lambda x: (-x) & 0xffff
        t[54] = m + 1

        def put(pan, bound, rt):
            t[55], t[56] = pan, 0
            t[58] = bound
            t[60], t[59] = rt >> 8, rt & 255
        if m == 0:
            t[54] = 0
            t[55] = min(p7, 0x20)
        elif m == 1:
            a = p7
            if a >= 0x11:
                a = min(a, 0x20)
                rate = neg(rate)
            t[55], t[56] = a, 0
            t[60], t[59] = rate >> 8, rate & 255
        elif m == 2:
            b = p7
            if b < 0x11:
                bound = min(b + p8, 0x20)
            else:
                b = min(b, 0x20)
                rate = neg(rate)
                bound = max(b - p8, 0)
            put(b, bound, rate)
            t[57] = p8
        elif m in (3, 5, 7):
            put(p7, min(p7 + p8, 0x20), rate)
            if m == 5:
                t[62], t[61] = iy[11], iy[12]
            if m == 7:
                self.effect_copy(t, iy, v)
        else:                                   # m == 4 or 6
            b = min(p7, 0x20)
            put(b, max(b - p8, 0), neg(rate))
            if m == 6:
                q = neg(iy[11] << 8 | iy[12])
                t[62], t[61] = q >> 8, q & 255

    def effect_copy(self, t, iy, v):
        """0x0458 (motion 7): the effect is copied to the next voice (voice 0 after 15), to start four passes later and sweep
        the pan the other way, unless an effect of higher priority holds that voice."""
        w = (v + 1) & 15
        u = self.sfx[w]
        if self.eff_prio < u[52]:
            return
        u.t[:] = t.t
        b = min(iy[7], 0x20)
        rate = (-(iy[9] << 8 | iy[10])) & 0xffff
        u[58] = max(b - iy[8], 0)
        u[55], u[56] = b, 0
        u[60], u[59] = rate >> 8, rate & 255
        u[63] = 4

    # ---- special commands (packet byte 0 = 0xff, byte 1 = number) -------------------------------------------------
    def special(self, iy):
        n = iy[1]
        if n == 0:                        # 0x082a: stop everything
            self.gfade_mode = 0
            for t in self.sfx + self.mus:
                t.t[:] = bytes(64)
            self.reset_chip_voices()
        elif n == 1:                      # 0x083e: stop all sound effects
            for v in range(16):
                self.voice = v
                if self.sfx[v][32]:
                    self.effect_end(v)
        elif n == 2:                      # 0x086b: stop the effect on one voice
            v = iy[4] & 15
            self.voice = v
            if self.sfx[v][32]:
                self.effect_end(v)
        elif n == 5:                      # 0x08c1: stop the music
            for v in range(16):
                t = self.mus[v]
                if t[32]:
                    t[32] = 0
                    if self.sfx[v][32] == 0:
                        self.key_off(v)
        elif n == 6 or n == 7:            # fades of the music
            rate = iy[3] << 8 | iy[4]
            if rate == 0:
                return
            if n == 6:
                if self.fade_mode == 0:
                    self.fade_level = 0
                self.fade_mode = 1
                self.fade_rate = rate
            else:
                if self.fade_mode == 0:
                    self.fade_level = 0xffff
                self.fade_mode = 0xff
                self.fade_rate = (-rate) & 0xffff
            self.fade_target = iy[2]
        elif n == 8:                      # 0x094c: tempo bend, signed amount in byte 4, sign in byte 3 (0xff = negative)
            v = (iy[4] * 0x369) >> 8
            self.tempo_bend = (-v) & 0xffff if iy[3] == 0xff else v
        elif n == 9:                      # 0x096a: pitch bend
            v = iy[4] << 4
            self.pitch_bend = (-v) & 0xffff if iy[3] == 0xff else v
        elif n == 10:
            self.report_pan = True
        elif n == 11:
            self.report_pan = False
        elif n == 12 or n == 13:          # 0x099b / 0x09c1: fade of everything
            rate = iy[3] << 8 | iy[4]
            if rate == 0:
                return
            if n == 12:
                if self.gfade_mode == 1:
                    return
                if self.gfade_mode == 0:
                    self.gfade_level = 0
                self.gfade_mode = 1
                self.gfade_rate = rate
            else:
                if self.gfade_mode == 0xff:
                    return
                if self.gfade_mode == 0:
                    self.gfade_level = 0xffff
                self.gfade_mode = 0xff
                self.gfade_rate = (-rate) & 0xffff
        elif n == 3:                      # 0x0895: the ADPCM channels off
            self.adpcm_reset()
        elif n == 4:                      # 0x08a5: one ADPCM channel; the Z80 then keys off voice (byte 4) as well
            self.key_off(iy[4])
        # 14 does nothing; no table entry starts an ADPCM sound, so the ADPCM channels are not modelled

    def reset_chip_voices(self):
        """0x0a4c: every voice off (0x0a5b), then the three ADPCM channels (0x0a7d)."""
        for v in range(16):
            self.key_off(v)
        self.adpcm_reset()

    def adpcm_reset(self):
        for c in range(3):
            self.wr(0xcd + 4 * c, 0)
            self.wr(0xd6 + c, 1)

    def effect_end(self, v):
        """0x0f44: an effect ends: voice off; if the music still has a note on this voice it takes the voice back."""
        s = self.sfx[v]
        s[32] = 0
        s[52] = 0
        self.key_off(v)
        m = self.mus[v]
        if m[32] == 0:
            return
        self.load_instrument(m, v, m[49] << 8 | m[48])
        self.pan_write(v, m[55])
        if m[FLAGS] & 0x80:
            self.wr(0x93, 0x27ff)
            self.wr(0xd9, 0x07ff)
            self.wr(0xba + v, self.rom.tab(T_VOLUME, m[ECHO]) << 2)
            self.wr(v * 8 + 1, m[34] << 8 | m[33])
            self.wr(v * 8 + 3, 0x8000)
            self.music, self.voice = True, v
            self.voice_update(m, v, True, force=True)
            self.music = False
        else:
            m[53] = 0xff

    # ---- one pass of the main loop (0x00f9..0x01e3), once per four interrupts -----------------------------------------
    def run_pass(self):
        for v in range(16):
            self.voice = v
            self.music = False
            self.sfx_on = False
            s = self.sfx[v]
            if s[32]:
                if s[63]:
                    s[63] -= 1
                    if s[63] == 0:
                        self.key_off(1)          # 0x0129: A still holds the old counter (1), so the Z80 keys off voice 1, not v
                        self.pan_write(v, s[55])
                        self.sfx_on = True
                else:
                    self.tempo = s[51] << 8 | s[50]
                    try:
                        self.track_tick(s, v, False)
                    except TrackEnd:
                        pass
                    if s[32]:
                        self.sfx_on = True
            m = self.mus[v]
            if m[32]:
                self.music = True
                self.tempo = (self.song_tempo + self.tempo_bend) & 0xffff
                try:
                    self.track_tick(m, v, True)
                except TrackEnd:
                    pass
                self.music = False
        self.run_fades()

    def run_fades(self):
        """0x0175..0x01e3: the two fades, once per pass."""
        m = self.gfade_mode
        if m:
            lvl = self.gfade_level + self.gfade_rate
            a = (0xff + (m >> 7)) & 255                     # 0xff for a fade in, 0 for a fade out
            a = (a + (1 if lvl > 0xffff else 0)) & 255
            lvl &= 0xffff
            if a == 0:                                      # finished: in = no scaling, out = silence
                self.gfade_mode = m & 0xfe
                lvl = 0
            self.gfade_level = lvl
        if self.fade_mode:
            tgt = self.fade_target
            lvl = self.fade_level + self.fade_rate
            carry = lvl > 0xffff
            lvl &= 0xffff
            if self.fade_mode == 1:
                if carry or (lvl >> 8) > tgt:
                    lvl = tgt << 8 | 0xff
                    if tgt == 0xff:
                        self.fade_mode = 0
            elif not carry or (lvl >> 8) <= tgt:
                lvl = tgt << 8
            self.fade_level = lvl

    # ---- the track interpreter (0x0b07) -----------------------------------------------------------------------------
    def track_tick(self, t, v, music):
        d = self.rom.d
        dur = t[4] | t[5] << 8
        if dur != 0:
            de = self.tempo
            gate = t[7] | t[8] << 8
            if gate:
                r = gate - de
                if r > 0:
                    gate = r
                else:
                    t[37] = 4
                    rec = t[35] | t[36] << 8
                    t[40], t[41] = self.rom.tab(T_RATE, d[rec + 4]) & 255, self.rom.tab(T_RATE, d[rec + 4]) >> 8
                    gate = 0
                t[7], t[8] = gate & 255, gate >> 8
            r = dur - de
            t[4], t[5] = r & 255, (r >> 8) & 255
            if r > 0 and ((r >> 8) & 255) < 0xc1:
                self.after_tick(t, v)
                return
        while True:
            a = self.rd(t)
            if a < 0x20:
                self.control(a, t, v)
                continue
            self.note(a, t, v)
            return

    def note(self, a, t, v):
        d = self.rom.d
        L = (a >> 5) & 7
        fl = t[FLAGS]
        if fl & 0x30 == 0:
            tab = T_DURATION[0]
        elif fl & 0x10 == 0:
            tab = T_DURATION[1]
        else:
            t[FLAGS] = fl & ~0x10
            tab = T_DURATION[2]
        add = d[tab + L - 1] << 8
        dur = (t[4] | t[5] << 8) + add
        t[4], t[5] = dur & 255, (dur >> 8) & 255
        n = a & 0x1f
        if n == 0:
            return
        k = (n + d[T_OCTAVE + (t[FLAGS] & 0x0f)]) & 255
        if self.music:
            k = (k + self.transpose) & 255
        k = (k + t[TRANSPOSE]) & 255
        e = s8((t[FINE] + t[INS_FINE]) & 255)
        pitch = (k << 8) + e
        pitch &= 0xffff
        if (pitch >> 8) >= 0x62:
            pitch = 0x6100 | (pitch & 255)
        # portamento (0x0c2d)
        sl = t[16] & 0x7f
        t[16] = sl
        cur = pitch
        if sl:
            rate = (sl * 2) & 255
            prev = t[21]
            if prev and prev != (pitch >> 8):
                if prev > (pitch >> 8):
                    rate = (-rate) & 0xffff
                t[17], t[18] = rate >> 8, rate & 255
                cur = t[21] << 8 | t[22]
                t[16] |= 0x80
        t[21], t[22] = pitch >> 8, pitch & 255
        t[19], t[20] = cur >> 8, cur & 255
        if t[FLAGS] & 0x80:
            self.note_tied(t, v)
        else:
            if t[23] & 1:
                t[23] &= 0x3f
                t[27] = 0
                t[26] = 0
            t[37] = 0
            a0 = self.rom.tab(T_ATTACK, d[t[35] | t[36] << 8])
            t[40], t[41] = a0 & 255, a0 >> 8
            t[42], t[43] = a0 & 255, a0 >> 8
            self.events.append(dict(tick=self.tick, voice=v, music=self.music, key=pitch >> 8, pitch=pitch,
                                    program=t.program, instrument=t[49] << 8 | t[48], volume=t[14] << 8 | t[13],
                                    echo=t[ECHO], pan=t[55], length=add >> 8, gate=t[GATE_RATIO]))
            if not self.sfx_on:
                self.wr(v * 8 + 2, 0)
                self.wr(0x93, 0x27ff)
                self.wr(0xd9, 0x07ff)
                self.wr(0xba + v, (self.rom.tab(T_VOLUME, t[ECHO]) << 2))
                self.wr(v * 8 + 1, t[34] << 8 | t[33])
                self.wr(v * 8 + 3, 0x8000)
            t[53] = 0
            self.voice_update(t, v, self.music)
        # bit 7 := bit 6 (the tie flag is armed for the next note); the gate
        b = t[FLAGS]
        t[FLAGS] = (b & 0x7f) | ((b & 0x40) << 1)
        if t[FLAGS] & 0x80:
            t[7], t[8] = 0, 0
        else:
            g = mul(t[4] | t[5] << 8, t[GATE_RATIO] << 8)
            t[7], t[8] = g & 255, g >> 8

    def note_tied(self, t, v):
        t[53] = 0
        self.voice_update(t, v, self.music)

    def after_tick(self, t, v):
        """0x1145 .. 0x1400: pan motion, oscillator phase, pitch slide, envelope, then the voice registers."""
        self.pan_motion(t, v)
        # oscillator phase (0x12d4)
        ph = t[26] | t[27] << 8
        rate = t[24] | t[25] << 8
        ph += rate
        t[26], t[27] = ph & 255, (ph >> 8) & 255
        if ph > 0xffff:
            t[23] = (t[23] + 0x40) & 255
        # pitch slide (0x12f2)
        if t[16] & 0x80:
            rate = t[17] << 8 | t[18]
            cur = t[19] << 8 | t[20]
            tgt = t[21] << 8 | t[22]
            a = 0xff if rate & 0x8000 else 0
            new = (cur + rate) & 0xffff
            diff = new - tgt
            borrow = diff < 0
            if diff == 0:
                cur = tgt
                t[16] &= 0x7f
            else:
                a = (a + (1 if borrow else 0)) & 255
                if a != 0:
                    cur = new
                else:
                    cur = tgt
                    t[16] &= 0x7f
            t[19], t[20] = cur >> 8, cur & 255
        self.envelope(t, v)

    def envelope(self, t, v):
        """0x132c: the five envelope stages (t[37]); level is t[42..43], rate t[40..41], sustain level t[38..39]."""
        d = self.rom.d
        st = t[37]
        level = t[42] | t[43] << 8
        rate = t[40] | t[41] << 8
        if st == 0:
            level += rate
            if level <= 0xffff:
                t[42], t[43] = level & 255, level >> 8
                self.voice_update(t, v, self.music)
                return
            nxt = 0xffff
        elif st == 1:
            sus = t[38] | t[39] << 8
            lv = level - rate
            if lv < 0:
                nxt = sus
            else:
                lv2 = lv - sus
                if lv2 <= 0:
                    nxt = sus
                else:
                    level = lv
                    t[42], t[43] = level & 255, level >> 8
                    self.voice_update(t, v, self.music)
                    return
        elif st in (3, 4):
            lv = level - rate
            if lv <= 0:
                nxt = 0
            else:
                t[42], t[43] = lv & 255, lv >> 8
                self.voice_update(t, v, self.music)
                return
        else:
            return                                   # stages 2 and 5: 0x1349 is a bare return; no register is written
        # next stage (0x13cb)
        if nxt == 0:
            a = 5
        else:
            a = t[37] + 1
            if a == 2:
                a = 3
            rec = (t[35] | t[36] << 8) + a
            r = self.rom.tab(T_RATE, d[rec])
            t[40], t[41] = r & 255, r >> 8
        t[37] = a
        t[43], t[42] = nxt >> 8, nxt & 255
        self.voice_update(t, v, self.music)

    # ---- volume and pitch registers of a voice (0x1400) -----------------------------------------------------------------
    def voice_update(self, t, v, music, force=False):
        if not force:
            if t[53]:
                return
            if self.sfx_on:
                return
        d = self.rom.d
        lfo_rate = t[24] | t[25] << 8
        phase = t[26] | t[27] << 8
        depth_t = t[30] | t[31] << 8
        level = t[42] | t[43] << 8
        if lfo_rate and depth_t:
            x = (0xffff - phase) if t[23] & 0x40 else phase
            x = mul(x, depth_t)
            level = mul(0xffff - x, level)
        vol = level
        if music:
            vol = mul(vol, self.song_vol)
            if self.fade_mode:
                vol = mul(vol, self.fade_level)
        if self.gfade_mode:
            vol = mul(vol, self.gfade_level)
        if self.master_scale:
            vol = mul(vol, self.master_scale)
        vol = mul(vol, t[14] << 8 | t[13])
        self.wr(v * 8 + 6, vol)
        # pitch
        vib = t[28] | t[29] << 8
        cur = t[19] << 8 | t[20]
        if lfo_rate and vib:
            x = (0xffff - phase) if t[23] & 0x40 else phase
            x = mul(x, vib)
            if t[23] & 0x80:
                x = (-x) & 0xffff
            cur = (x + cur) & 0xffff
        if music:
            cur = (cur + self.pitch_bend) & 0xffff
        h = ((cur >> 8) - 1) & 255
        a = (0x3c - t[ROOT] + h) & 255
        if a >= 0x6c:
            a = 0x6b if ((a - 0x6c) & 0x80) else 0
        frac = cur & 255
        self.wr(v * 8 + 2, self.rate_of(a, frac, music))

    def rate_of(self, idx, frac, music):
        """0x14fd..0x153f: rate register for key index 0..107 (middle of the octave table) and a 1/256-semitone fraction."""
        d = self.rom.d
        octv = d[T_OCT_OF_KEY + idx]
        r = idx - d[T_OCT_BASE + octv]
        base = d[T_FINE + (r << 8) + frac] << octv
        return (base + self.rom.tab(T_FREQ_MUS if music else T_FREQ_SFX, idx)) & 0xffff

    # ---- pan motion of sound effects (0x1145) ------------------------------------------------------------------------
    def pan_motion(self, t, v):
        k = t[54]
        if k <= 1:
            return
        P = t[55] << 8 | t[56]
        R = t[60] << 8 | t[59]
        B = t[58]
        hl = (P + R) & 0xffff
        h, l = hl >> 8, hl & 255
        stop = False
        neg = lambda x: (-x) & 0xffff
        if k == 2:                                   # bounce between 0 and 0x20
            if h >= 0x21:
                h = 0 if R & 0x8000 else 0x20
                R = neg(R)
        elif k == 3:                                 # bounce between two bounds that keep their distance
            if not R & 0x8000:
                if h >= B:
                    h = B
                    B = max(B - t[57], 0)
                    R = neg(R)
            else:
                if not (h > B and h < 0x21):
                    h = B
                    B = min(B + t[57], 0x20)
                    R = neg(R)
        elif k == 4:                                 # sweep up to the bound and stop
            if h >= B:
                h, stop = B, True
        elif k == 5:                                 # sweep down to the bound and stop
            if not (h > B and h < 0x21):
                h, stop = B, True
        elif k == 6:                                 # sweep up to the bound, then at the second rate to 0x20
            if h >= B:
                h = B
                if h == 0x20:
                    stop = True
                else:
                    R = t[62] << 8 | t[61]
                    B = 0x20
        elif k == 7:                                 # sweep down to the bound, then at the second rate to 0
            if not (h > B and h < 0x21):
                h = B
                if h == 0:
                    stop = True
                else:
                    R = t[62] << 8 | t[61]
                    B = 0
        elif k == 8:                                 # sweep either way to the bound and stop
            if not R & 0x8000:
                if h >= B:
                    h, stop = B, True
            elif not (h > B and h < 0x21):
                h, stop = B, True
        t[58] = B
        t[60], t[59] = R >> 8, R & 255
        if stop:
            t[54] = 0
        t[55], t[56] = h, l
        self.pan_write(v, h)

    # ---- control commands (0x0d62) ---------------------------------------------------------------------------------------
    def control(self, c, t, v):
        d = self.rom.d
        rd = lambda: self.rd(t)
        if c == 0: t[FLAGS] ^= 0x20
        elif c == 1: t[FLAGS] ^= 0x40
        elif c == 2: t[FLAGS] |= 0x10
        elif c == 3: t[FLAGS] ^= 0x08
        elif c == 4: t[FLAGS] = (t[FLAGS] & 0x97) | rd()
        elif c == 5:
            h = rd(); l = rd()
            if self.music:
                self.song_tempo = h << 8 | l
            else:
                t[51], t[50] = h, l
        elif c == 6: t[GATE_RATIO] = rd()
        elif c == 7:
            x = self.rom.tab(T_VOLUME, rd()); t[13], t[14] = x & 255, x >> 8
        elif c == 8: self.program(t, v, PROG_TABLE, rd())
        elif c == 9: t[FLAGS] = (t[FLAGS] & 0xf8) | rd()
        elif c == 10:
            b = rd()
            if self.music: self.transpose = b
        elif c == 11: t[TRANSPOSE] = rd()
        elif c == 12: t[FINE] = rd()
        elif c == 13: t[16] = rd() & 0x7f
        elif 14 <= c <= 17:
            k = 44 + c - 14
            b = rd()
            if t[k] == 0:
                t[k] = b
                self.jump(t)
            else:
                t[k] = (t[k] - 1) & 255
                if t[k]:
                    self.jump(t)
                else:
                    t.set_ptr(t.ptr() + 2)
        elif 18 <= c <= 21:
            k = 44 + c - 18
            if t[k] - 1 == 0:
                t[k] = 0
                b = rd()
                t[FLAGS] = (t[FLAGS] & 0x97) | b
                self.jump(t)
            else:
                t.set_ptr(t.ptr() + 3)
        elif c == 22: self.jump(t)
        elif c == 23: self.track_end(t, v)
        elif c == 24:
            b = rd()
            if self.music:
                t[55] = b
                if not self.sfx_on:
                    self.pan_write(v, b)
        elif c == 25: t[ECHO] = rd()
        elif c == 26:
            b = rd()
            if self.music:
                self.song_vol = mul(self.vol_scale, self.rom.tab(T_LEVEL, b))
        elif c == 27:
            x = self.rom.tab(T_VIB_DEPTH, rd()); t[28], t[29] = x & 255, x >> 8
        elif c == 28:
            x = self.rom.tab(T_TREM_DEPTH, rd()); t[30], t[31] = x & 255, x >> 8
        elif c == 29:
            x = self.rom.tab(T_LFO_RATE, rd()); t[24], t[25] = x & 255, x >> 8
        elif c == 30:
            b = rd(); t[23] = (t[23] & ~1) | b
        elif c == 31: self.program(t, v, 0x7800, rd())

    def jump(self, t):
        """0x0f09: signed 16-bit offset (high byte first) added to the pointer after the two bytes."""
        h = self.rd(t); l = self.rd(t)
        off = h << 8 | l
        if off & 0x8000: off -= 0x10000
        t.set_ptr(t.ptr() + off)

    def program(self, t, v, base, n):
        """0x0df9 / 0x1054: select the envelope program `n`: record of 8 bytes: instrument id (u16), tuning, attack, decay,
        sustain level, second decay, release."""
        d = self.rom.d
        rec = base + 8 * n
        t.program = n if base == PROG_TABLE else 256 + n
        t[INS_FINE] = d[rec + 2]
        t[35], t[36] = (rec + 3) & 255, (rec + 3) >> 8
        s = self.rom.tab(T_LEVEL, d[rec + 5])
        t[38], t[39] = s & 255, s >> 8
        ident = d[rec + 1] << 8 | d[rec]
        if (t[49] << 8 | t[48]) == ident:
            return
        t[49], t[48] = ident >> 8, ident & 255
        if not self.sfx_on:
            self.load_instrument(t, v, ident)

    def load_instrument(self, t, v, ident):
        """0x10a2: the sample of instrument record `ident` goes to the voice."""
        ins = self.rom.instrument(ident)
        self.wr(v * 8 + 2, 0)
        t[34], t[33] = ins['start'] >> 8, ins['start'] & 255
        self.wr(v * 8 + 1, ins['start'])
        self.wr(v * 8 + 5, ins['end'])
        self.wr(v * 8 + 4, (ins['end'] - ins['loop']) & 0xffff)
        self.wr(((v - 1) & 15) * 8, 0x8000 | ins['bank'])
        t[ROOT] = ins['root']

    def track_end(self, t, v):
        if self.music:
            t[32] = 0
            if not self.sfx_on:
                self.key_off(v)
        else:
            self.effect_end(v)
        raise TrackEnd

    # ---- running ------------------------------------------------------------------------------------------------------------
    def step(self):
        """One 250 Hz interrupt; the track pass happens on every fourth (tick numbers 3, 7, 11 ...)."""
        if self.irq % 4 == 3:
            self.run_pass()
        self.irq += 1
        self.tick = self.irq


def run_song(rom, code, ticks, packet=None):
    drv = Driver(rom)
    drv.command(packet if packet else [code >> 8, code & 255])
    for _ in range(ticks):
        drv.step()
    return drv


def cmd_table(rom):
    n = rom.command_count()
    kinds = {}
    for c in range(n):
        lin, k = rom.command_entry(c)
        kinds.setdefault(k, []).append(c)
    for k, cs in kinds.items():
        runs = []
        s = p = cs[0]
        for x in cs[1:]:
            if x != p + 1:
                runs.append((s, p)); s = x
            p = x
        runs.append((s, p))
        print('%-7s %4d codes: %s' % (k, len(cs), ' '.join('%x-%x' % r if r[0] != r[1] else '%x' % r[0] for r in runs)))
    print('table at 0x9006: %d entries (0x%x), song volume index %d, master volume index %d' % (n, n, rom.d[CMD_TABLE + 2], rom.d[CMD_TABLE + 3]))


def event_lines(events):
    """The note events as text: time in seconds (a tick is 1/250 s), voice, instrument record, program, key, track volume
    (the curve value of control command 7), echo send index, pan 0..32, length in driver units (256 = one pass of tempo 1)."""
    out = []
    for e in events:
        out.append('t=%7.3f %s voice %2d ins %3d prog %3s key %3d vol %04x echo %3d pan %2d len %3d' % (
            e['tick'] / 250.0, 'M' if e['music'] else 'E', e['voice'], e['instrument'],
            '-' if e['program'] is None else e['program'], e['key'], e['volume'], e['echo'], e['pan'], e['length']))
    return out


# ---- the C player (music.c) under ctypes: the runtime's code, built into a temporary folder -----------------------------------------
HERE = Path(__file__).resolve().parent
SOUND_SIZE = 0x40000
SAMPLES_SIZE = 0x400000
OUTPUT_RATE = 44100
C_FLAGS = ['-std=c99', '-O2', '-Wall', '-Wextra', '-Werror', '-pedantic']
SANITIZE_FLAGS = ['-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', '-fno-omit-frame-pointer', '-g']


class _RomViews(ctypes.Structure):
    _fields_ = [('program', ctypes.c_void_p), ('tiles', ctypes.c_void_p), ('samples', ctypes.c_void_p), ('sound', ctypes.c_void_p)]


def sanitizer_preload():
    """The path of the address sanitizer's runtime, to preload into Python when music.c is built with the sanitizers."""
    out = subprocess.run(['cc', '-print-file-name=libasan.so'], capture_output=True, text=True).stdout.strip()
    return out if out and os.path.isabs(out) else None


class CMusic:
    """music.c built as a shared library in `tmp` (a folder that exists) and its functions with their types. With `sanitize`, Python
    must have been started with LD_PRELOAD=sanitizer_preload() (sanitized_environment)."""

    def __init__(self, tmp, sanitize=False):
        so = Path(tmp) / ('music-san.so' if sanitize else 'music.so')
        subprocess.run(['cc', *C_FLAGS, *(SANITIZE_FLAGS if sanitize else []), '-shared', '-fPIC', '-o', str(so), str(HERE / 'music.c')],
                       check=True, capture_output=True, timeout=120)
        L = self.lib = ctypes.CDLL(str(so))
        vp, u32, sz = ctypes.c_void_p, ctypes.c_uint32, ctypes.c_size_t
        L.arcade_music_open.argtypes = [ctypes.POINTER(_RomViews), ctypes.c_int]
        L.arcade_music_open.restype = vp
        L.arcade_music_close.argtypes = [vp]
        L.arcade_music_reset.argtypes = [vp]
        L.arcade_music_command.argtypes = [vp, ctypes.c_uint]
        L.arcade_music_packet.argtypes = [vp, ctypes.c_char_p, sz]
        L.arcade_music_stop.argtypes = [vp]
        L.arcade_music_tick.argtypes = [vp]
        L.arcade_music_chip_write.argtypes = [vp, ctypes.c_uint, ctypes.c_uint]
        L.arcade_music_render.argtypes = [vp, vp, sz]
        L.arcade_music_tracks.argtypes = [vp]
        L.arcade_music_tracks.restype = ctypes.c_uint
        L.arcade_music_set_log.argtypes = [vp, vp, sz]
        L.arcade_music_log_count.argtypes = [vp]
        L.arcade_music_log_count.restype = sz
        L.arcade_music_voice_state.argtypes = [vp, ctypes.c_uint, vp]

    def player(self, sound, samples=None, stereo=True, log=1 << 20):
        return CPlayer(self, sound, samples, stereo, log)


def sanitized_environment():
    """The environment to start a Python with that can load the sanitized music.c, or None when the compiler has no sanitizer runtime."""
    pre = sanitizer_preload()
    if not pre or not os.path.exists(pre):
        return None
    env = dict(os.environ, LD_PRELOAD=pre, ASAN_OPTIONS='detect_leaks=0:abort_on_error=0', UBSAN_OPTIONS='halt_on_error=1:print_stacktrace=1')
    return env


class CPlayer:
    """One C player over a sound program (256 KB bytes) and optional samples (4 MB bytes). `writes()` gives what the driver wrote to the
    chip since the player was made (the reset's writes included), as the model's `(tick, register, value)` tuples."""

    def __init__(self, cm, sound, samples, stereo, log):
        if len(sound) != SOUND_SIZE:
            raise ValueError('the sound program view is 262144 bytes')
        self.cm, self.lib = cm, cm.lib
        self.sound = ctypes.create_string_buffer(bytes(sound), SOUND_SIZE)
        self.samples = ctypes.create_string_buffer(bytes(samples), SAMPLES_SIZE) if samples is not None else None
        self.views = _RomViews(None, None, ctypes.cast(self.samples, ctypes.c_void_p) if self.samples is not None else None,
                               ctypes.cast(self.sound, ctypes.c_void_p))
        self.cap = log
        self.log = ctypes.create_string_buffer(8 * log)
        self.m = self.lib.arcade_music_open(ctypes.byref(self.views), 1 if stereo else 0)
        if not self.m:
            raise MemoryError('arcade_music_open')
        self.lib.arcade_music_set_log(self.m, ctypes.cast(self.log, ctypes.c_void_p), log)
        self.lib.arcade_music_reset(self.m)

    def close(self):
        if self.m:
            self.lib.arcade_music_close(self.m)
            self.m = None

    def __enter__(self): return self
    def __exit__(self, *a): self.close()

    def packet(self, p): return self.lib.arcade_music_packet(self.m, bytes(p), len(p))
    def command(self, code): return self.lib.arcade_music_command(self.m, code)
    def tick(self, n=1):
        f, m = self.lib.arcade_music_tick, self.m
        for _ in range(n):
            f(m)
    def tracks(self): return self.lib.arcade_music_tracks(self.m)
    def chip_write(self, reg, val): self.lib.arcade_music_chip_write(self.m, reg, val)

    def voice(self, v):
        """(bank, addr, rate, loop, end, volume, send, pan) of the mixer's voice v."""
        buf = ctypes.create_string_buffer(16)
        self.lib.arcade_music_voice_state(self.m, v, buf)
        bank, pan = buf.raw[0], buf.raw[14]
        addr, rate, loop, end, vol, send = struct.unpack_from('<6H', buf.raw, 2)
        return bank, addr, rate, loop, end, vol, send, pan

    def render(self, frames):
        """`frames` frames of 44,100 Hz interleaved stereo 16-bit little-endian, as bytes."""
        out = ctypes.create_string_buffer(4 * max(frames, 1))
        n = self.lib.arcade_music_render(self.m, out, frames)
        if n != frames:
            raise ValueError('arcade_music_render returned %d' % n)
        return out.raw[:4 * frames]

    def writes(self):
        n = self.lib.arcade_music_log_count(self.m)
        if n > self.cap:
            raise OverflowError('the write log is full (%d writes)' % n)
        return [(a, b, c) for a, b, c in struct.iter_unpack('<IHH', self.log.raw[:8 * n])]


def model_writes(rom, schedule, ticks, stereo=True):
    """The model's writes for `schedule` ([(interrupt, packet)] in order of interrupt) over `ticks` interrupts."""
    drv = Driver(rom, stereo)
    i = 0
    for t in range(ticks + 1):
        while i < len(schedule) and schedule[i][0] <= t:
            drv.command(schedule[i][1])
            i += 1
        if t < ticks:
            drv.step()
    return drv.writes


def c_writes(player, schedule, ticks):
    i = 0
    for t in range(ticks + 1):
        while i < len(schedule) and schedule[i][0] <= t:
            player.packet(schedule[i][1])
            i += 1
        if t < ticks:
            player.tick()
    return player.writes()


def first_difference(a, b):
    """None when the lists are equal, else a line about the first index where they differ."""
    for i, (x, y) in enumerate(zip(a, b)):
        if x != y:
            return 'write %d: C %s, model %s' % (i, fmt_write(x), fmt_write(y))
    if len(a) != len(b):
        return 'C wrote %d, the model %d' % (len(a), len(b))
    return None


def fmt_write(w): return 'tick %d reg %02x val %04x' % w


def effect_packet(code, rng):
    """The 68000's packet for an effect: the code, then a pan motion (byte 6 low bits) with parameters in bytes 7..12."""
    p = bytearray(13)
    p[0], p[1] = code >> 8, code & 255
    p[6] = rng.randrange(8)
    p[7] = rng.choice([0, 8, 0x10, 0x18, 0x20, 0x30, rng.randrange(256)])
    p[8] = rng.randrange(0x30)
    p[9], p[10] = rng.randrange(4), rng.randrange(256)
    p[11], p[12] = rng.randrange(4), rng.randrange(256)
    return bytes(p)


def special_packet(rng):
    n = rng.choice([0, 1, 2, 5, 6, 7, 8, 9, 12, 13, 14, 3, 4, 10, 11])
    p = bytearray(13)
    p[0], p[1] = 0xff, n
    p[2] = rng.choice([0, 0x40, 0x80, 0xff, rng.randrange(256)])
    p[3] = rng.choice([0, 0xff, rng.randrange(256)]) if n in (8, 9) else rng.randrange(4)
    p[4] = rng.randrange(256) if n not in (2, 4) else rng.randrange(16)
    if n in (6, 7, 12, 13):
        p[3], p[4] = rng.randrange(8), rng.randrange(1, 256)
    return bytes(p)


def random_script(rng, rom, length):
    """A script of commands: songs, effects (many pan motions), fades, bends and stops, at random interrupts."""
    songs = [c for c in range(rom.command_count()) if rom.command_entry(c)[1] == 'music']
    effects = [c for c in range(rom.command_count()) if rom.command_entry(c)[1] == 'effect']
    sched, t = [], 0
    for _ in range(length):
        t += rng.choice([0, 1, 3, 4, 50, 200, 500])
        r = rng.random()
        if r < 0.25 and songs:
            c = rng.choice(songs)
            sched.append((t, bytes([c >> 8, c & 255])))
        elif r < 0.7 and effects:
            sched.append((t, effect_packet(rng.choice(effects), rng)))
        else:
            sched.append((t, special_packet(rng)))
    return sched, t + 1500


def compare_c(rom, tmp, sanitize=False, quiet=False, scripts=150):
    """Compare the C player's register writes with the model's. Returns (lines, ok)."""
    cm = CMusic(tmp, sanitize)
    lines, ok = [], True

    def one(player_rom_sound, schedule, ticks, stereo=True):
        with cm.player(player_rom_sound, stereo=stereo) as pl:
            return first_difference(c_writes(pl, schedule, ticks), model_writes(rom, schedule, ticks, stereo)), len(pl.writes())

    sound = rom.d
    songs = [c for c in range(rom.command_count()) if rom.command_entry(c)[1] == 'music']
    effects = [c for c in range(rom.command_count()) if rom.command_entry(c)[1] == 'effect']
    for label, codes, ticks, make in (
            ('songs', songs, 3000, lambda c, rng: bytes([c >> 8, c & 255])),
            ('effects', effects, 800, lambda c, rng: effect_packet(c, rng))):
        t0, equal, total, first = time.time(), 0, 0, None
        for c in codes:
            rng = random.Random(c)
            diff, n = one(sound, [(0, make(c, rng))], ticks, stereo=(c % 5 != 4))
            total += n
            if diff is None:
                equal += 1
            elif first is None:
                first = 'code 0x%x: %s' % (c, diff)
        lines.append('%-8s %3d of %3d equal (%d writes, %.1f s)' % (label, equal, len(codes), total, time.time() - t0))
        if first:
            ok = False
            lines.append('  first difference: ' + first)
    t0, equal, total, first = time.time(), 0, 0, None
    for i in range(scripts):
        rng = random.Random(1000 + i)
        sched, ticks = random_script(rng, rom, rng.randrange(6, 30))
        diff, n = one(sound, sched, ticks, stereo=(i % 4 != 3))
        total += n
        if diff is None:
            equal += 1
        elif first is None:
            first = 'script %d: %s' % (i, diff)
    lines.append('%-8s %3d of %3d equal (%d writes, %.1f s)' % ('scripts', equal, scripts, total, time.time() - t0))
    if first:
        ok = False
        lines.append('  first difference: ' + first)
    return lines, ok


def chip_samples(path):
    """The chip's view of the sample ROM from the arcade archive: sz2.11m + sz2.12m with every byte pair swapped."""
    with zipfile.ZipFile(path) as z:
        raw = bytearray(z.read('sz2.11m') + z.read('sz2.12m'))
    raw[0::2], raw[1::2] = raw[1::2], raw[0::2]
    return bytes(raw)


def render_c(rom, samples, code, seconds, out_path, tmp, sanitize=False):
    """Play command `code` through the C player's mixer for `seconds` and write a stereo 16-bit 44,100 Hz WAV. Returns (frames, took)."""
    lin, kind = rom.command_entry(code)
    if kind not in ('music', 'effect'):
        raise SystemExit('code %d (0x%x) is not a song or an effect' % (code, code))
    cm = CMusic(tmp, sanitize)
    frames = int(seconds * OUTPUT_RATE)
    t0 = time.time()
    with cm.player(rom.d, samples, log=16) as pl, wave.open(str(out_path), 'wb') as w:
        w.setnchannels(2)
        w.setsampwidth(2)
        w.setframerate(OUTPUT_RATE)
        pl.packet([code >> 8, code & 255, 0, 0, 0, 0, 0, 0x10])
        done = 0
        while done < frames:
            n = min(4410, frames - done)
            w.writeframes(pl.render(n))
            done += n
    return frames, time.time() - t0


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('cmd', nargs='?', choices=['table', 'song', 'writes', 'all'])
    ap.add_argument('code', nargs='?', type=lambda s: int(s, 0))
    ap.add_argument('--arcade')
    ap.add_argument('--sound')
    ap.add_argument('--ticks', type=int, default=2000)
    ap.add_argument('--compare-c', action='store_true', help='build music.c and compare its register writes with the model\'s')
    ap.add_argument('--sanitize', action='store_true', help='with --compare-c or --render: build music.c with ASAN and UBSAN (re-runs Python preloaded)')
    ap.add_argument('--render', type=lambda s: int(s, 0), metavar='CODE', help='play this song or effect through the C player to a WAV file')
    ap.add_argument('--seconds', type=float, default=30.0)
    ap.add_argument('--out', help='the WAV file for --render')
    a = ap.parse_args(argv)
    if not (a.arcade or a.sound):
        ap.error('--arcade ZIP or --sound FILE is needed')
    if a.sanitize and (a.compare_c or a.render is not None) and 'asan' not in os.environ.get('LD_PRELOAD', ''):
        env = sanitized_environment()
        if env is None:
            print('no sanitizer runtime on this machine')
            return 1
        return subprocess.run([sys.executable, *sys.argv], env=env).returncode
    rom = Rom(open(a.sound, 'rb').read()) if a.sound else Rom.from_zip(a.arcade)
    if a.compare_c:
        with tempfile.TemporaryDirectory(prefix='music-c-') as tmp:
            lines, ok = compare_c(rom, tmp, a.sanitize)
        print('\n'.join(lines))
        print('C == model: %s' % ('yes' if ok else 'NO'))
        return 0 if ok else 1
    if a.render is not None:
        if not a.arcade or not a.out:
            ap.error('--render needs --arcade ZIP (the samples) and --out FILE.wav')
        with tempfile.TemporaryDirectory(prefix='music-c-') as tmp:
            frames, took = render_c(rom, chip_samples(a.arcade), a.render, a.seconds, a.out, tmp, a.sanitize)
        print('rendered %d frames (%.1f s of audio) of code 0x%x in %.2f s (%.0fx real time) to %s' % (
            frames, frames / OUTPUT_RATE, a.render, took, frames / OUTPUT_RATE / max(took, 1e-9), a.out))
        return 0
    if a.cmd is None:
        ap.error('a command is needed')
    if a.cmd == 'table':
        cmd_table(rom)
    elif a.cmd in ('writes', 'song'):
        if a.code is None:
            ap.error('a command code is needed')
        lin, k = rom.command_entry(a.code)
        if a.cmd == 'writes':
            for t, r, v in run_song(rom, a.code, a.ticks).writes:
                print(t, '%02x' % r, '%04x' % v)
        else:
            print('code %d (0x%x): %s at 0x%x' % (a.code, a.code, k, lin))
            if k == 'music':
                print('tracks (voice:offset):', ' '.join('%d:%x' % (i, o) for i, o in enumerate(rom.song_tracks(lin)) if o))
            drv = run_song(rom, a.code, a.ticks)
            print('\n'.join(event_lines(drv.events)))
    else:
        for c in range(rom.command_count()):
            lin, k = rom.command_entry(c)
            if k != 'music':
                continue
            drv = run_song(rom, c, a.ticks)
            print('song %2d: %2d tracks, %5d notes in %d ticks, tempo 0x%04x' % (
                c, sum(1 for o in rom.song_tracks(lin) if o), len(drv.events), a.ticks, drv.song_tempo))
    return 0


if __name__ == '__main__':
    sys.exit(main())
