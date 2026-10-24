# The arcade's music and sound driver (specification)

Status 2026-10-23. What the arcade's sound CPU does with the 59 songs and 868 sound effects of `sfa2u`, as rules for a
player of the port's own. Source: the Z80 program (`sz2.01a` + `sz2.02a`, one 256 KB view; addresses below are file
offsets of that view, which are also the Z80 addresses in the fixed 32 KB and in the window at 0x8000 with bank 0).
`music.py` is the executable form of every rule here; it reads only the data tables out of the Z80 program and
reproduces the driver's QSound register writes. Nothing here needs the Z80's code or an interpreter of it.

Evidence: a throwaway Z80 emulator (not in the repository, not in the port) ran the real program. The model's register
writes equal the program's for all 59 songs (30,000 interrupts each, 120 s), all 868 effects (800 interrupts each, and 40
random pan-motion parameter sets for each of the 8 motions), and about 600 random scripts of 12 to 40 commands mixing songs,
effects, fades, bends and stops, in stereo and mono. Section 12 lists what is not covered.

## 1. Addressing

- The song data, the tables and the code of the Z80 program all lie in the 256 KB view. A *linear address* (24 bits) is a
  file offset. The driver reads a byte at linear `a` with the window `bank = (a >> 14) - 2` at `0x8000 + (a & 0x3fff)`
  when `a >= 0x8000`; below that it reads the fixed ROM. So a pointer into the view is a plain file offset.
- The fixed 32 KB holds the code (0x0000 to 0x2cbe) with the small tables below among it and after it, and the programs at
  0x7000. The window area holds the instrument table (0x8000), the command table (0x9000) and all songs and effects.
- `mul(a, b) = (a * b) >> 16` (0x2c0a; exact: the Z80 builds it from nibble tables at 0x2cbf and 0x4cbf, checked on 200,000
  pairs). All "scaled by" below mean this.

Tables (word = little-endian 16 bits) read by the driver:

| address | size | meaning |
|---|---|---|
| 0x0d36 / 0x0d2f / 0x0d3d | 7 bytes each | note lengths, units: normal 3 6 12 24 48 96 192; binary 2 4 8 16 32 64 128; one-shot (dotted) 0 9 18 36 72 144 0 |
| 0x0d44 | 16 bytes | key offset by flag bits 0 to 3: 0 12 24 36 48 60 72 84, then the same plus 24 |
| 0x25e9 | 128 words | track volume curve (also the echo send curve): 0 .. 0x1ffd |
| 0x26e9 | 128 words | song volume scale, master scale, sustain level: 0 .. 0xfffe |
| 0x27e9 | 64 words | attack start level and attack rate |
| 0x2869 | 128 words | envelope rates |
| 0x28e9 / 0x29e9 / 0x2ae9 | 128 words | oscillator rate / vibrato depth / tremolo depth |
| 0x1542 / 0x15ae | 108 / 9 bytes | octave of a key index; first key index of an octave |
| 0x1767 | 12 * 256 bytes | pitch correction by (key index within octave, fraction) |
| 0x168f / 0x15b7 | 108 words | rate base of a key index, for music / for sound effects |
| 0x7000, 0x7800 | 256 records of 8 bytes | envelope programs (section 6) |
| 0x8000 | 435 records of 8 bytes | instruments (section 7) |
| 0x9000 | 6 + 4 * 1024 bytes | command table (section 2) |

## 2. Commands from the 68000

The 68000 hands the Z80 a packet of up to 16 bytes in shared RAM (68000 addresses `0x618001 + 2k` are Z80 `0xc000 + k`):

- byte 0, 1: the command code, high byte first. Byte 0 = 0xff: a *special command*, byte 1 is its number (section 9).
- bytes 2 to 12: parameters. For special commands see section 9. For a sound effect: byte 6 low 3 bits pan motion,
  bytes 7 to 12 its parameters (section 8). Songs take none.
- The Z80 takes a packet when its byte `0xc00f` (68000 `0x61801f`) is not 0xff, copies it to a 16-entry ring, and writes
  0xff there. The Z80's own side of the handshake: it writes 0x77 to `0xcfff`, waits for 0xff there, and requires 0x88 in
  `0xcffd` (the 68000 writes both at start). Byte `0xcffe` is the stereo flag (0 = mono: every pan is forced to the centre).
- The code is taken modulo the table size (0x400): `entry = 0x9006 + 4 * (code mod 0x400)`, four bytes: the 24-bit linear
  address of the song or effect (big-endian), then a zero. Address 0 = nothing. The byte at that address is the kind:
  0 = music, 1 to 0x7f = sound effect (its priority), 0x80 and up = an ADPCM sound (no entry uses it; not modelled).
- The table has 927 entries: music codes 1 to 0x3a and 0x3e (59 songs); effects 0x3f, 0x50-0x55, 0x60-0x95, 0xa0-0xbe,
  0xc0-0xd3, 0xe0-0xf3, 0x100-0x171, 0x180-0x271, 0x280-0x33e, 0x340-0x37e, 0x380-0x3be, 0x3c0-0x3fe (868). Song volume
  index = byte 0x9002 (104), master volume index = byte 0x9003 (127) into the table at 0x26e9.

## 3. Songs and effects

Both: byte 0 kind, then 16 big-endian offsets (voice 0 to 15; 0 = voice unused), then the tracks. Offsets are from the
byte 0 of the song. A song's tracks are 16 *music tracks*, an effect's are 16 *effect tracks*; each voice has one of each
(they run side by side; the effect's wins the voice, section 8).

Starting a **song** (0x04b6) replaces the previous one: music tempo, transposition, bends and music fade are reset
(tempo 0, transposition 0, bends 0, fade off), the song volume is `mul(T26e9[byte 0x9002], 0xfffe)`; every voice with an
offset starts a track at its offset with the state of section 4 (instrument id 0xffff = none, LFO flags 1, envelope stage 5,
pan 0x10); every voice without one stops (a track that was running is keyed off). A voice not held by an effect is keyed
off (section 7) and its pan centred.

Starting an **effect** (0x0241): for each voice with an offset: if the effect's priority (its byte 0) is lower than the
priority of the effect running on that voice (track byte 52) the voice is skipped; otherwise the track state is cleared and
started, track byte 52 = priority, the pan motion is set up (section 8), the voice is keyed off and its pan written. An
effect is never cut by one of lower priority; equal priority replaces.

## 4. Time and the pass

The Z80 gets a 250 Hz interrupt (MAME's rate for this board; the tempos below come out musical at it: tempo 0x018c gives 120.8
quarter notes a minute, taking 48 units as a quarter). Every fourth interrupt is a *pass*: 62.5 a second, the music's tick. A note of
`u` units lasts `u * 256 / tempo` passes. Commands are taken on every
interrupt, before the pass. In a pass, for voice 0 to 15 in order: the effect track of the voice (if running), then the music
track of the voice (if running); then the global fade and the music fade (section 9). Each track call is `track_tick`.

A track's state is 64 bytes (`+n` = byte offset). Pairs are little-endian unless marked (BE).

| | |
|---|---|
| +0..2 | pointer (linear address) |
| +3 | flags: bits 0-2 octave index, bit 3 +2 octaves, bit 4 next note dotted, bit 5 binary lengths, bit 6 tie armed, bit 7 note is tied |
| +4,5 | duration counter (units * 256) |
| +6 | gate ratio |
| +7,8 | gate counter |
| +9, +10 | transpose; fine tune (1/256 semitone) |
| +11, +12 | root key of the loaded instrument; tuning of the program |
| +13,14 | track volume (curve value) |
| +15 | echo send index |
| +16 | portamento: low 7 bits rate, bit 7 = sliding |
| +17,18 (BE) | slide rate (signed); +19,20 (BE) current pitch (key, fraction); +21,22 (BE) target pitch |
| +23 | oscillator flags: bit 0 reset phase on a new note; bits 6,7 = quadrant counter (bit 6 odd quadrant, bit 7 negative half) |
| +24,25 / +26,27 | oscillator rate / phase |
| +28,29 / +30,31 | vibrato depth / tremolo depth |
| +32 | running (0xff) |
| +33,34 | instrument start address |
| +35,36 | pointer to the program's bytes 3..7 (section 6) |
| +37 | envelope stage 0 attack, 1 decay, 3 second decay, 4 release, 5 finished (2 unused) |
| +38,39 | sustain level; +40,41 envelope rate; +42,43 envelope level |
| +44..47 | four loop counters |
| +48,49 | instrument id (0xffff = none) |
| +50,51 | effect tempo (a music track uses the song tempo) |
| +52 | effect priority; +53 non-zero: voice registers not written until the next note |
| +54 | pan motion mode; +55,56 pan (+55 integer part 0..0x20, +56 fraction); +57 range; +58 bound; +59,60 pan rate; +61,62 second rate; +63 start delay |

`track_tick(T)` (0x0b07), with `tempo` = the effect track's +50,51, or for a music track `song_tempo + tempo_bend` (16-bit):

```
if T.dur != 0:                                   # +4,5
    if T.gate != 0:
        T.gate -= tempo
        if T.gate <= 0 (or borrowed):  T.stage = 4; T.rate = RATE[program byte 7]; T.gate = 0     # key off: release
    T.dur = (T.dur - tempo) & 0xffff             # kept even when it goes below zero: the remainder carries over
    if T.dur > 0 (no borrow) and (T.dur >> 8) < 0xc1:  after_tick(T); return
repeat:  b = next byte
    if b < 0x20: run control command b; continue
    note(b); return
```

`after_tick` (0x1145): pan motion (section 8); oscillator phase += rate, a carry adds 0x40 to +23; the pitch slide step;
then the envelope stage step, which ends by writing the voice's volume and rate (section 5). It is *not* run in the pass where
a track fetches new bytes.

## 5. Notes

A byte `b >= 0x20` is a note: length code `L = b >> 5` (1 to 7) and note number `n = b & 0x1f` (0 = rest).

```
dur += TABLE[L-1] << 8    # table by flags: bit 4 set -> dotted table, bit 4 cleared; else bit 5 -> binary; else normal
if n == 0: return                                       # a rest only adds time; the previous note's gate still runs
key  = (n + OCT[flags & 15] + (music ? song_transpose : 0) + T.transpose) & 0xff
pitch = (key << 8) + signed(T.fine + T.program_tuning)  # 16 bits: key, fraction; if key >= 0x62 then key = 0x61
portamento: if T.slide_rate7 != 0 and T.previous_key != 0 and T.previous_key != key: start from the previous pitch, step
            +/- (rate7 * 2) per pass to the new pitch, then hold (sliding flag in +16 bit 7); else start at the new pitch
            T.previous = pitch
if flags bit 7 (tied): update the voice (volume and rate) only
else:
    if osc flags bit 0: clear bits 6,7 and the phase
    stage = 0; envelope rate = level = T27e9[program byte 3]
    if no effect holds the voice: write rate 0, 0x93 = 0x27ff, 0xd9 = 0x07ff, echo send, start address, phase 0x8000
    update the voice
flags bit 7 = flags bit 6                               # a tie is armed one note ahead
gate = mul(dur, gate_ratio << 8)  unless the new bit 7 is set (then 0 = never cut)
```

Dotted notes: control command 2 sets bit 4, the next note takes the one-shot table (1.5 times the normal length of codes 2 to 6)
and clears it. The tie flag: command 1 toggles bit 6. The first note after it starts normally; every note whose bit 7 was set by
its predecessor is tied (no attack, no gate cut) until bit 6 is cleared and one more note has passed.

**Update the voice** (0x1400; skipped when an effect holds the voice or +53 is non-zero), all 16-bit, `mul` as above:

```
level = envelope level (+42,43)
if osc rate != 0 and tremolo depth != 0:
    x = phase;  if bit 6 of +23: x = 0xffff - x         # triangle 0..1
    level = mul(0xffff - mul(x, tremolo depth), level)
vol = level
if music: vol = mul(vol, song_volume); if music fade on: vol = mul(vol, fade_level)
if global fade on: vol = mul(vol, global_fade_level)
if master scale != 0: vol = mul(vol, master_scale)
vol = mul(vol, track volume)                            # +13,14
write reg 8v+6 = vol

cur = pitch (+19,20)
if osc rate != 0 and vibrato depth != 0:
    x = phase; if bit 6: x = 0xffff - x; x = mul(x, vibrato depth); if bit 7: x = -x; cur += x
if music: cur += pitch_bend
idx = 0x3c - root_key + (cur.key - 1)   (8 bits); if idx >= 0x6c: idx = 0x6b if (idx - 0x6c) & 0x80 else 0
rate = (FINE[(idx - 12 * octave) * 256 + cur.fraction] << octave) + BASE[idx]       # octave = T1542[idx], BASE = music or sfx table
write reg 8v+2 = rate
```

The rate is about `4096 * 2^((idx - 60 + fraction/256) / 12)` (within 1.4 percent at the bottom, 0.1 percent above idx 36), so a
note at the root key plays the sample at its natural rate. Rates up to 0xcb16 occur (song 13, effects 0x86 and 0x87), above 0x7fff.

## 6. Envelope programs

Control command 8 selects program `n` from 0x7000 (command 31: 0x7800), 8 bytes: `[0,1]` instrument id, `[2]` tuning (added to
the note's fine tune), `[3]` attack, `[4]` decay rate, `[5]` sustain level, `[6]` second decay rate, `[7]` release rate; all but `[0,1]`
and `[2]` are indexes into the tables. The sustain level is `T26e9[prog[5]]`. If the id differs from the track's current instrument
the instrument is loaded (section 7) unless an effect holds the voice.

Each pass (`after_tick`), level `L` (16-bit) and rate `R` (+40,41):

| stage | step | next |
|---|---|---|
| 0 attack | `L += R` | on carry: `L = 0xffff`, stage 1, `R = T2869[prog[4]]` |
| 1 decay | `L -= R` | when `L <= sustain` (or it borrows): `L = sustain`; if sustain = 0: stage 5, else stage 3, `R = T2869[prog[6]]` |
| 3 second decay | `L -= R` | when `L <= 0`: `L = 0`, stage 5 |
| 4 release | `L -= R` | when `L <= 0`: `L = 0`, stage 5 |
| 2, 5 | nothing, no register is written | |

The voice is updated after every step of 0, 1, 3 and 4. Key off (the gate counter running out, section 5) sets stage 4 with
`R = T2869[prog[7]]`. A new note sets stage 0 with `R = L = T27e9[prog[3]]`. There is no QSound key-off: the voice's volume
runs to zero. At the end of a music track (command 23) the voice is keyed off at once.

## 7. Instruments and the QSound voice

Record `0x8000 + 8 * id`: `[0]` bank (0 to 63), `[1,2]` start, `[3,4]` loop start, `[5,6]` end (16-bit offsets in the bank; 435
records, all looping), `[7]` root key (72 for 358 of them, 60 for 25). The sample is signed 8-bit PCM at
`sample_rom[bank * 0x10000 + offset]` (the chip's view: `sz2.11m` + `sz2.12m` one after the other with every byte pair swapped).
Loading an instrument writes, for voice v: rate 0, start address, end address, loop length (end - loop start), bank.

The QSound registers the driver writes (16 bits; the Z80 writes the data then the register number):

| register | meaning |
|---|---|
| `8v+1` | start / current address (16-bit offset in the bank) |
| `8v+2` | rate: 0x1000 = the sample plays at one sample per output sample (about 24 kHz) |
| `8v+3` | phase, 0x8000 at a note on |
| `8v+4`, `8v+5` | loop length, end address |
| `8v+6` | volume, 0 to 0x1ffc (2^13 = unity is the likely scale) |
| `8((v-1) mod 16)` | bank of voice v, `0x8000 + bank` (the chip files the bank one slot early) |
| `0x80+v` | pan = `0x110 + p`, p = 0 (left) to 0x20 (right), 0x10 centre |
| `0xba+v` | echo send = `T25e9[echo index] << 2` (command 25) |
| `0x93`, `0xd9` | 0x27ff, 0x07ff at every note on (echo feedback and length, by MAME's chip model) |
| at start | `0xde`=0, `0xdf`=0x2e, `0xe0`=0, `0xe1`=0x30, `0xe4..0xe7`=0x3fff, `0xe2`=0xffff, `0xda`=0x0db2, `0xdc`=0x0e11, `0xe3`=0x39 (the chip's filter and mix set-up) |
| key off of v | `0xba+v`=0, `8v+6`=0, `8v+2`=0, `8v+2`=0x1000 |

What a PC mixer must model: 16 voices stepping through the sample ROM by `rate / 4096` samples per output sample with the
fraction kept, wrapping `pos -= loop length` at the end address; sample * volume / 0x2000; a pan gain pair by `p` (the chip
uses a 33-entry table of its own; equal power `cos`/`sin` of `p * pi / 64` is the stand-in); the echo: each voice adds its
send level into one delay line with feedback (the chip's, set by `0x93` and `0xd9`); the output rate (60 MHz / 2 / 1248 =
24,038 Hz on the real board). Not available here and so not stated: the exact pan table, the echo's length and filter.

## 8. Effects: voices, pan, priority

The 16 effect tracks run the same byte code with their own tempo (command 5 sets `+50,51`; music tracks set the song tempo).
While an effect track is running on a voice, the music track on it still runs (time passes, its envelope ticks) but writes
nothing; when the effect ends (command 23 or stop) the voice is keyed off and the music track's instrument is loaded again and,
if its note is tied, its note resumes; otherwise it stays silent until its next note.

Pan motion (packet byte 6 low 3 bits, parameters bytes 7 to 12; `p7`..`p12`):

| mode | start | motion |
|---|---|---|
| 0 | `min(p7, 0x20)` | fixed |
| 1 | `min(p7, 0x20)` | linear at rate (p9,p10 BE; negated if p7 > 0x10), reflects at 0 and 0x20 |
| 2 | p7 | between two bounds `p7` and `p7 + p8` (reversed if p7 > 0x10), bouncing, same distance |
| 3, 5, 7 | p7 | up to `min(p7 + p8, 0x20)` then stop; 5 then continues at rate (p11,p12) to 0x20; 7 also starts a copy on the next voice |
| 4, 6 | `min(p7, 0x20)` | down to `max(p7 - p8, 0)` and stop; 6 then continues at rate (p11,p12) to 0 |

Mode 7 copies the effect's track to the next voice (voice 0 after 15) with the pan sweeping the other way, starting four passes
later (when it starts the Z80 keys off voice *1*, not the copy's voice: its A register still holds the counter; harmless, kept in the model).
The 68000 passes mode 0 with a pan from the object's distance to the camera (section 10).

## 9. Control commands (the byte `< 0x20` of a track)

`arg` = the next byte. "Music only" = ignored by effect tracks.

| n | arg | action |
|---|---|---|
| 0 | | toggle binary lengths (flag bit 5) |
| 1 | | toggle the tie arm (bit 6) |
| 2 | | next note dotted (bit 4) |
| 3 | | toggle +2 octaves (bit 3) |
| 4 | f | `flags = (flags & 0x97) | f` |
| 5 | hi, lo | tempo = hi lo: music: song tempo; effect: the track's. A pass takes `tempo` off the counters; one unit of a length table is 256 |
| 6 | g | gate ratio (0 = notes never cut) |
| 7 | i | track volume = `T25e9[i]` |
| 8 | n | program n (section 6) |
| 9 | o | octave index (flag bits 0-2) |
| 10 | s | song transposition in semitones (music only) |
| 11 | s | track transposition |
| 12 | s | fine tune, signed 1/256 semitone |
| 13 | r | portamento rate (low 7 bits; 0 = off) |
| 14..17 | c, offset(2) | loop k=0..3: if counter k = 0 then counter = c and jump else (counter -= 1; if non-zero jump else skip the offset) |
| 18..21 | f, offset(2) | on the last pass of loop k (counter k = 1): clear the counter, `flags = (flags & 0x97) | f`, jump; else skip 3 bytes |
| 22 | offset(2) | jump |
| 23 | | end of track (music: the voice is keyed off; effect: it ends) |
| 24 | p | pan 0..0x20 (music only); written at once |
| 25 | e | echo send index |
| 26 | v | song volume = `mul(T26e9[byte 0x9002], T26e9[v])` (music only) |
| 27 | v | vibrato depth `T29e9[v]` |
| 28 | v | tremolo depth `T2ae9[v]` |
| 29 | v | oscillator rate `T28e9[v]` |
| 30 | b | oscillator flags (clears bit 0, ors b): unused by every song |
| 31 | n | program n of the second bank 0x7800 (effects) |

Jump offsets are signed 16-bit, high byte first, relative to the byte after them. A loop `[body][14 c off]` plays the body
`c + 1` times. Uses in the 59 songs: commands 0 to 9, 24 in all; 10 in 6; 11 in 23; 12 in 44; 13 in 37; 14 in 45; 15 in 24; 16 in 3;
17 in 2; 18 in 25; 19 in 11; 20 in 3; 21 in 2; 22 in 48; 23 in 20; 25 in 39; 26 in 47; 27 in 40; 28 in 5; 29 in 40; 31 in 1; 30 in none.

**Special commands** (packet `0xff, n`), all at once:

| n | packet bytes | action |
|---|---|---|
| 0 | | stop everything: all tracks cleared, the global fade off, every voice keyed off |
| 1 | | stop every effect (voices go back to the music) |
| 2 | b4 | stop the effect on voice b4 |
| 3, 4 | b4 | the ADPCM channels (not used) |
| 5 | | stop the music (tracks off; voices not held by an effect keyed off) |
| 6, 7 | b2 target, b3 b4 rate | music fade in / out: level `fade_level` (start 0 / 0xffff) moves `rate` a pass to `target << 8` (in: `target << 8 | 0xff`; at 0xff the fade ends); the music volume is multiplied by it while on |
| 8 | b3 sign, b4 | tempo bend `(b4 * 0x369) >> 8`, negative if b3 = 0xff, added to the tempo |
| 9 | b3 sign, b4 | pitch bend `b4 * 16` (1/256 semitone), negative if b3 = 0xff, added to every music pitch |
| 10, 11 | | pan reporting to the 68000 on / off (`0xc005`; not modelled) |
| 12, 13 | b3 b4 rate | fade of everything (also effects) in / out; at the end the multiplier is dropped (in) or stays at 0 (out) |
| 14 | | nothing |

## 10. The 68000's side (analysed on a decrypted copy; the port cannot read these tables from the raw set)

The raw program ROM is encrypted over the code and these tables, so they were read from a decrypted copy that only analysis used.

- The game queues sounds in a 256-entry ring at RAM `a5+0x4f60` (16 bytes an entry; indexes `a5+0x5f60` write, `a5+0x5f62` read),
  12 bytes `d1.l d2.l d3.l` an entry, through `0x13b22` (checks the sound enable flags `a5+0x99`, `a5+0x83`) or `0x13b2e`; the wrappers
  `0x13c5e` (saves registers) and `0x13c64` / `0x13c98` (also compute the pan: `0x13ccc[clamp(dx, 0, 0x17f)]`, 9 to 23, into byte 7,
  mode 0, `dx` = object x - camera x) are called from about 800 sites. Once a frame (0x13a7e) one entry is sent: entry bytes 0..11 go to
  packet bytes 3, 4, 0, 1, 2, 6, 7, 8, 11, 12, 9, 10. So `d1` = byte 3 | byte 4 | code high | code low; `d2` = byte 2 | byte 6 | byte 7 | byte 8.
- The special commands have their own wrappers: stop all `0x13b6c`, stop effects `0x13b7a`, stop effect `0x13b88`, ADPCM `0x13b9c` and
  `0x13baa`, stop music `0x13bbe`, fade in `0x13bcc`, fade out `0x13be0`, tempo `0x13bf4`, pitch `0x13c04`, pan report `0x13c14` and
  `0x13c20`, global fades `0x13c2c` and `0x13c3a`.
- Music is requested through one function, `0x15494` (nothing happens if byte `a5+0xa6` is set), with `d0` = 2 * selector. It jumps by the
  word table at `0x154aa` (24 entries): selector 0 plays the **fighter theme** from the 20-word table at `0x154ee` indexed by the word
  `a5+0x100` (songs 1 2 11 9 3 6 5 8 7 10 12 4 13 18 17 14 15 16 53 12); selector 1 plays from the 24-word table at `0x1552c` indexed by the
  byte `a5+0x130` (songs 28 30 40 38 31 34 33 37 36 39 41 32 42 48 47 43 44 45 31 45 40 44 43 28: 18 different ones, so one per fighter;
  by their match with the PS1's ending codes they are the **ending themes**); selector 2 sends song 62 and effect 63 (`0x155a0`);
  selectors 3 to 23 play, from the 21-word table at `0x15576`, songs 19 20 21 22 23 24 25 26 27 50 51 57 29 35 54 55 56 46 49 52 58 in this order.
- Call sites found by scanning for `jsr/jmp 0x15494` (25), with selector (`d0 / 2`) and song: `0x5818` (if `a5+0x102` = 0) selector 0;
  `0x1a3d0` and `0x8be10` selector 1; `0x1bde` selector 2; `0x27fc` song 19; `0x19b8e` song 20; `0x19f1c` song 21; `0x6c58` song 23;
  `0x5516` and `0x9b76` song 24; `0x6fb2` song 25; `0xd6d2` song 26; `0x86fcc` song 29; `0x88bda` song 35; `0x8c1dc` song 46; `0x8b064` song 49;
  `0x880c2` song 54; `0x87866` song 55; `0x8bc54` song 56; `0x913c` song 52 or 58 by the byte table at `0x9142` indexed by `a5+0x147`;
  `0x95bfe` song 50, 51 or 57 by the byte table at `0x95c10` indexed by `a5+0x130` (selector 1 when `a5+0x130` is 2 or 20). Songs 22, 27 and the
  fighter-theme and ending contexts beyond these have no direct call; what each caller is (title, select, win, continue, credits) is not read yet.
  Song 52 is a two-voice repeating tick, song 58 a short five-voice stinger.

## 11. Songs and the PS1's music codes

The PS1 plays its music from 64 recorded streams (`set_menu_command_by_code(1, code)`, code = file * 256 + channel; the sound-test
list at `data_800e9074_slot0f` has 59 codes). The arcade plays song numbers. The correspondence, by rule where the two games' tables
align and by measurement (a chroma-sequence match of each arcade song, rendered from the model's register writes through a crude
mixer, against each recorded channel) elsewhere:

- **Fighter themes (structural, supported by the audio where the audio is decisive):** the PS1 table `table_8016e6f8`
  (`0x8016e6f8`, indexed by fighter number) and the arcade table at `0x154ee` have the same index. So fighter k's PS1 code
  (`0x000`..`0x007`, `0x100`..`0x107`, `0x200`, `0x201`, `0x202`, `0x102`) is the arcade song
  `[1,2,11,9,3,6,5,8,7,10,12,4,13,18,17,14,15,16,53,12][k]`. Audio agrees at rank 1 for fighters 1, 2, 3, 5, 6, 9, 10, 11, 13, 17 and 18;
  for 0 (Ryu) the pair is second by 0.01, for 12 and 15 (Dan, Dhalsim) the song 14 matches both 0x104 and 0x107, and for fighters 4, 7, 8, 14, 16
  the audio is not decisive. No fighter's audio contradicts the rule.
- **Others (audio only):** high confidence (score above 0.5 with a margin above 0.2): song 28 = 0x600, 29 = 0x601, 30 = 0x300, 31 = 0x304,
  32 = 0x404, 37 = 0x405, 39 = 0x403, 40 = 0x206, 41 = 0x301, 42 = 0x302, 43 = 0x401, 45 = 0x506, 46 = 0x507, 48 = 0x303, 54 = 0x502,
  55 = 0x402. Probable (score 0.4 to 0.5): 38 = 0x501, 47 = 0x306, 50 = 0x203, 57 = 0x205. The short jingles (songs 21 to 27, 44, 49 to 52,
  56, 58, 62) and songs 19, 20, 33 to 36 did not match anything decisively (score below 0.4 or no margin).
  The PS1 stream does not match note for note: it is an arrangement, so a wrong pick is possible below the "high" line.
  The matches of songs 28 to 48 (the arcade's ending-theme table) all land in the PS1's ending-family codes (0x206, 0x30x, 0x40x, 0x50x, 0x60x).
- The roles (which PS1 call plays which of these) are in the PS1 C: `advance_state_index_and_set_menu_command_by_kind` plays the fighter's
  theme; the ending scenes, the continue screen, the rank screens and the boss demo play fixed codes (`0x30x`..`0x60x`).

## 12. What is not covered

- The ADPCM channels (no table entry uses them); the pan report to `0xc005`.
- Echo, pan law and mixing scale are the chip's, not derived from the Z80 program; section 7 states what the driver relies on.
- The 250 Hz interrupt rate is MAME's, supported by the tempos, not measured on a board.
- Roles of most arcade music selectors (section 10) and the PS1-to-arcade song correspondence outside the fighter themes (section 11).

## 12b. The PS1's sound requests

How the PS1 game asks for a sound effect or a voice, how the arcade asks for the same one, and the rule that turns the first into the
second (`sounds.py` is the executable form; `sounds.py --c-table` prints the rows as numbers). The port plays these through section 13's
player, not through the PS1's banks. Status 2026-10-23: analysis only, nothing wired.

**The PS1's request.** There is no queue. The game calls the sound player `func_80120604(song, alt, start, pos)` (ps1/src/resident/sound) at
once, from three wrappers in `set_sound_mono_or_stereo.c`: `play_sound_at(Object *o, int side, unsigned code)` (446 call sites),
`play_side_sound_at(Object *o, int side, int index)` (344; the side's own bank, `song = side`) and `call_func_with_side_x_origin(side,
code)` (52; the origin is the left or right of the screen). The code is `song * 256 + start`. The fields:

- `song` 0 to 3 picks a bank slot: its VAB (`data_80190a44[song]`, loaded by table 3/4 slot `song` of a chunk archive), its SEP file (table 5 slot
  0x100 + song) and its command list (`table_8016e6e0[song]`, table 3 slot `song + 4`, at most 640 bytes). Slots 0 and 1 are the two players'
  fighter banks (PLnn.PAC and PLnnX.PAC); 2 and 3 are the shared banks of GAME_SND.PAC (2: menu cursor, confirm and cancel, codes 0x200 to 0x211; 3: hits and the rest, 0x300 to 0x34f),
  FIRSTSND.PAC (the same, loaded at start), DEMO.PAC (2), END*.PAC (3), CONT*.PAC and SELECT*.PAC (0). `alt` is the side; channels are moved up by four for side 1.
- `start` is the entry of the list where the sound begins. The list is 8-byte entries; a sound is the run from `start` to the first entry whose
  word 1 has bit 23 clear. Command (word 0 bits 12-15): 0 key on (program byte 3, tone word 1 bits 12-15, note bits 24-30, channel 7-11, priority
  16-22, volume 0-6, pan byte 2: 0xff = from `pos`, else 0..127 with 0x40 the centre); 1 play sequence `low12` of the bank's SEP file; 9 volume;
  11 stop a channel (0xfff all); the others do nothing. A run often holds a key on and a stop.
- `pos` = the object's x minus the camera's, clamped to -192 .. 575 (192 for no object, 64 or 320 for the side wrappers); the PS1's pan is
  `(pos + 192) / 6` (0 left, 64 centre, 127 right).
- Other calls that touch sound: `stop_sep_by_index/word` (5 sites), `stop_all_keys_clear_table` (7), `close_vab_and_sep` (12, unloading), the
  volume setup in `start_sound_and_set_volumes`. `request_sound_command_and_switch_thread(n)` is a thread switch, not a request. Music is the CD
  stream (`set_menu_command_by_code`, section 11).

**Where a port-side replacement goes.** `func_80120604`: every effect passes it with all four values, and it is the only caller of the key-on
and sequence-play library calls. It has a C override (`port/overrides/func_80120604.c`, the shift count mask); a host override of the same
function is refused at start ("has two overrides"), so one of: (A) host overrides on the three wrappers (they have exact C and no override; each
computes `pos` itself as the wrapper does), (B) a hook call at the top of the override's C (`if (hook(song, alt, start, pos)) return;`, null in the
differential test), (C) retiring the C override when the setting is on. The replacement finds the sound (below), sends the packet of section 2
(`code >> 8`, `code & 255`, byte 6 = 0, byte 7 = pan) with `arcade_music_packet`, and does not run the PS1 library calls.

**The arcade's request** (section 10): 423 small functions between 0x13e4c and 0x186ca of the 68000 program each load a code into `d1` and send it:
33 a fixed code, 353 `(player & 1) + base`, 28 a code from a two-word table `[player 1, player 2]`, about 380 of them through the pan wrappers
(`0x13c64`, `0x13c98`: byte 7 = `0x13ccc[clamp(dx, 0, 0x17f)]`, 9 to 23, mode 0; the rest a fixed pan 0x10). Fighters' sounds are codes in their own
data. The effects come in pairs for the two players with the same events on other voices (voices 12, 13 for player 1 and 14, 15 for player 2; the
codes differ by 0x100, 0x80, 0x20, 0x10 or 1: 451 different effects among the 868 codes). Ranges by use, as the mapping finds them: 0xa1 to 0xb5 the
menu, option and jingle sounds (the PS1's 0x200 to 0x211); 0xc0 to 0xf3 and 0x180 to 0x37e the fighters' voices (a block of about ten for each, in
the order of the fighter file) and 0x100 to 0x171 shared hits and effects (0x101 to 0x10f menu bleeps); 0x60 to 0x7f one voice of each fighter, 160 ms
late (the select screen's; no PS1 sound maps to them); 0x80 to 0x95 short note patterns of one instrument.

**The mapping.** The PS1's tables do not follow the arcade's numbering (no table equals the arcade's under a shift), so it is found by measurement,
sound by sound (`sounds.py`): each key on and each sequence note goes through the VAB header to a sample, through samples.py's matches to the arcade
sample and the instrument records that hold it, and becomes an event (time, instrument, pitch). Pitch is `12 log2(44100 / R) + note - center + shift /
128` for the PS1 and `pitch / 256 - 1 - root` for the arcade (music.md section 5); over all the key ons of the 239 archives the two round to the same
number for 2385 of 2583 events of root-72 instruments (2473 within one), and for root-60 instruments the PS1 is one semitone above for 296 of 340
(20 equal): a convention of the PS1's tool, kept as a constant. The timeline of
the PS1 sound (key ons at 0, sequence notes at their SEP times) is compared with each of the 868 effects (40 ms plus 3 percent in time, the same
instrument, the pitch rule). Several fitting effects: the nearest in code to the list's certain sounds (the arcade's block of the fighter). Pair: the
effect and its twin on the voices two higher are the codes for side 0 and side 1 (`alt`). Methods, best first: *exact* (the same events), *block* (exact,
several, chosen by nearness), *prefix* (the SEP file ends one echo early), *partial* (the arcade plays more), *instrument* (the same instrument and
times at another pitch: lower confidence), *seq-first* (the first note's effect for a sequence that has none of its own), *inferred* (between two certain
neighbors two codes apart, the sample having no arcade source). Fixed by the table: a PS1 pan byte of 0x40 is the arcade's 0x10.

**Coverage** (466 different sounds, keyed by the VAB header and the run of entries; 25 banks and lists on the disc): 396 map: exact 305, block 18,
prefix 30, partial 1, instrument 19, seq-first 19, inferred 4. 70 do not: 53 have a sample with no arcade source (25 are the select screen's own
recordings of about half a second at 44.1 kHz, 28 others in the fighter files and the menus, some the PS1's own additions), 9 are samples that no
effect plays at all, 4 are samples at no instrument record, 4 only stop. By kind: single key ons 349 of 409, sequences of echoes 40 of 45 (the arcade
has the echoes as events of its own effect: 6 notes where the PS1's SEP has 5), key on and sequence 7 of 8. By list: fighters 305 of 326, title bank
22 of 24, shared bank 2 (announcer) 17 of 18, shared bank 3 (hits and menu) 40 of 52, continue 6 of 9, ending 4 of 6, demo 2 of 6, select 0 of 25. Of
the PS1 C's 815 calls with a constant request, 345 + 95 name a shared sound that maps in all or some of the lists that define it, 44 name one that
does not (0x34c and 0x34d, samples with no arcade source, are 34 of them), and 330 are own-bank sounds that map for 93 percent of the fighters (weighted by the calls).
642 of the 868 arcade codes (332 of 451 effects) are the target of some PS1 sound; 28 mapped sounds rest on a sample match that covers under 50 percent.

**Decisions** (2026-10-24, `apply_decisions` in `sounds.py`; the numbers are of the tool, the rows are in `sounds_table.c`). Three changes to the
measurement and one table. (1) A sample that starts up to 64 bytes before an arcade record counts as inside it (the PS1 sample begins with a
warm-up block; 2, 24 and 37 bytes were measured): the four rows without a record map, `FIRSTSND` entry 2 becomes exact `0xa3` (its second
note is the instrument of `0xa3`, not of `0xa1`) and one fighter sound that had matched a menu code (`0xa5`) is exact in its own block. (2) The
run rule: sounds without an arcade source between two certain sounds whose codes differ by exactly the number of sounds between them take the
codes in between (the arcade numbers a fighter's sounds in list order; 7 sounds, 3 fighters). (3) The hand table (`HAND`): the sounds that the
measurement cannot place, placed by the 68000 program: select cursor `0x82`, pick `0x84` (code at `0x627c8`, `0x628a4` and two twins), the
results screen's `0x91`, `0x8a`, `0x84` (`0x7052`, `0x85a38`, `0x6db6`), the dizzy sound `0x12f` (`0x5da44`, the same routine as the PS1's
`spawn_three_effect_objects_with_sound`), the win marker `0x86` (`0x82ffa`), the score tick `0x88` (`0x5c70`, `0x5c98`) and the lightest hit
`0x100` (place 0 of the hit ladder). Each carries its evidence in `sounds.py`. (4) Silence: a stop, and a sound with no arcade source that no
hand row places, are code `0xfffe` (`ARCADE_SOUND_SILENT`): the arcade plays nothing there (it has no per-effect stop; its effects end by
themselves, and its select screen plays no fighter voice), and so is a sound whose sample no effect plays (6 sounds; two more are placed by list position, `0x1a6` and `0x116`). `HAND_OVERRIDE` replaces five measured rows by the arcade's own code: the results screen's count (`0xae`, routine `0x7006`) and four places of the hit ladder (`0x103`, `0x107` for both strengths and knock-outs). No row of the table is `0xffff` any more.

**Runtime key.** The row is found by the FNV-1a 32 of the VAB header (the port sees it in `SsVabOpenHeadSticky(addr, vab_id)`: 3104 bytes for one program,
512 more for each further one; remember it per `vab_id`) and of the entries of the run from `start` (the list memory at
`table_8016e6e0[song] + 8 * start`). The generated `arcade_sounds[]` is sorted by that pair for a binary search; the same pair in two archives is the
same sound (no key had differing banks). Stops: command 11 and `stop_all_keys_clear_table` become special command 1 (stop every effect, music kept),
`stop_sep_by_*` the same or command 2 on the voices of the side.

**Uncertain.** (1) Sample matches: a weak match can name the wrong instrument (28 mapped sounds); the pitch and time check makes a chance agreement
unlikely but not impossible. (2) *seq-first*, *instrument* and *partial* (39 sounds, mostly the menu bleeps and the fighters' first echo) play the
arcade effect that is nearest, not necessarily the one the arcade plays. (3) Side 0 is the lower code; in 51 of the 369 pairs the arcade's
voices are not two higher, only different (27 sounds have no twin and use one code for both sides). (4) Silent sounds (code `0xfffe`) are a decision, not a measurement. (5) The pan
table numbers come from the decrypted analysis copy, as section 10's. (6) Time zero of an effect is the first pass (12 ms); not measured against the PS1.
(7) Which of two identical fighter blocks a shared code belongs to is by nearness only.

## 13. The C player

`music.c` / `music.h` is the runtime player: the driver of sections 1 to 9 as C of our own, statement by statement from `music.py`, and a
mixer for the chip. No Z80 code is run. `arcade_music_open(rom, stereo)` borrows the sound and sample views of `struct arcade_rom`;
`arcade_music_command(code)` plays a song or effect (an effect with a fixed centre pan), `arcade_music_packet` takes the whole packet of section 2
(pan motion, special commands), `arcade_music_stop` is special command 0, `arcade_music_render(m, out, frames)` makes 44,100 Hz stereo 16-bit and
calls the driver's 250 Hz interrupt by itself. `arcade_music_set_log` records the chip writes (tick, register, value): the register writes equal
the model's for the 59 songs (12 s each), the 868 effects with a pan motion each (3.2 s) and 150 random scripts (`music.py --compare-c`).
The driver ends a track that reads outside the 256 KB view or runs 200,000 commands in one call (broken data; the Z80 would run on).

The mixer reads the chip's register writes, so nothing else joins driver and sound. Choices that sections 7 and 12 leave open (guesses; the
named constants are at the top of `music.c`):

- A voice steps `rate / 4096` chip samples per chip output sample (24,038 Hz), so `rate * 0.545 / 4096` samples per output frame, position kept with
  32 fraction bits; linear interpolation; the register 8v+3 (phase) is the fraction, 16 bits (0x8000 = half a sample). At `position >= end` the
  position goes back by the loop length; a loop length of 0 holds silent at the end. The bank register of voice v is filed one slot early.
- Volume 0x2000 is unity: sample (8-bit) * 256 * volume / 0x2000; a fixed output gain of 0.6 (the loudest of the 59 songs peaks at 24,000 of
  32,767 alone); the sum is clipped, not wrapped.
- Pan: equal power, left `cos(p * pi / 64)`, right `sin(p * pi / 64)`, p = register - 0x110 clamped to 0..32 (not the chip's table).
- Echo: a stereo delay line fed by each voice's panned output times its send (register 0xba+v over 0x8000); feedback register 0x93 over 0x8000
  (0x27ff = 0.31), length `register 0xd9 - 0x554` chip samples (683, 28 ms, from 0x07ff), scaled to the output rate; the return is added to both
  channels at gain 1. No filter (registers 0xda, 0xdc, 0xe0 to 0xe7 are ignored).

## Files

`port/arcade/music.py` (the model: `Rom`, `Driver.command(packet)`, `Driver.step()`, `.writes`, `.events`; commands `table`, `song`, `writes`,
`all`; `--compare-c` and `--render` build and use the C), `port/arcade/music.c`, `music.h` (the player), `port/tools/test_arcade_music.py` (a
made-up program image, 32 tests, 0.1 s), `port/tools/test_arcade_music_c.py` (the C against the model on made-up images, the mixer, the
sanitizers and the 32-bit build, 32 tests, 4 s). `port/arcade/sounds.py` (12b: the PS1 sound requests as arcade codes; `--c-table`) and
`port/tools/test_arcade_sounds.py` (14 tests on made-up data, under a second).
