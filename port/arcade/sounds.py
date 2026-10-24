#!/usr/bin/env python3
"""Analysis tool: map the PS1 game's sound requests to the arcade's sound effect codes; writes nothing.

What it does. The PS1 game asks for a sound by calling the sound player func_80120604(song, alt, start, pos)
(ps1/src/resident/sound), through play_sound_at(object, side, code), play_side_sound_at(object, side, index) and
call_func_with_side_x_origin(side, code) of resident/sound/set_sound_mono_or_stereo.c. `song` (0 to 3) picks one of four
loaded sound banks (a Sony VAB) and its list of sound commands; `start` is the entry of that list where the sound begins
(code = song * 256 + start); `alt` is the side (0 or 1) and moves the voices by four so that the two players do not cut
each other; `pos` is the object's x minus the camera's, clamped to -192 .. 575, from which the pan is made. The list is
8-byte entries (table 3 slots 4 to 7 of a chunk archive): word 0 = command in bits 12-15 (0 note on, 1 play a sequence of
the bank's SEP file, 9 volume, 11 stop a channel), low 12 bits = sequence or channel, byte 2 = pan (0xff: from `pos`),
byte 3 = program; word 1 = tone (bits 12-15), channel (bits 7-11), priority (16-22), volume (0-6), note (24-30), and bit 23
= the sound goes on with the next entry. A sound is the run of entries from `start` to the first entry without bit 23.

The arcade's own request is a 16-bit effect code that the 68000 hands to the sound CPU (music.md, sections 2 and 10), and
the arcade has the effect pairs of the two players (P1 and P2 differ by 0x100, 0x80, 0x20 or 1, and play on other voices).
The PS1 game logic comes from the arcade's but its sound tables do not follow the arcade's numbering, so this tool finds
the correspondence by measurement:

  1. every PS1 note-on and every note of the SEP sequences it plays is followed through the VAB header (program, tone, or
     program and key range) to a sample, and through samples.py's matches to the arcade sample it was made from, and
     through the Z80 program's instrument table (music.py) to the arcade instrument records that contain it;
  2. its pitch is turned into semitones relative to the arcade sample's natural rate: 12 log2(44100 / R) + note - center
     + shift / 128, R the rate samples.py found; the arcade effect's note has 12 log2(rate / 4096) = key - 1 - root (the
     PS1's tool plays the root-60 instruments one semitone above that: measured, 296 of 296 and 20 of 20 within 1);
  3. the PS1 sound becomes a timeline of (time, instrument, pitch) events (notes at 0, sequence notes at their SEP times)
     and is compared with the timeline of each of the arcade's 868 effects (music.py runs them, 10 s each): times within
     40 ms plus 3 percent, the same instrument, pitch within 0.75 semitone;
  4. an effect is a P1 and P2 pair when two effects have the same events (instrument, key, times, volume) and other
     voices: the sound gets the code for side 0 and the one for side 1 (the lower code and its twin on voices two higher);
  5. when several effects remain the sound takes the one nearest in code to the sounds of its list that are certain (the
     arcade numbers a fighter's sounds as a block), else the lowest.

Every sound gets a method: exact (one effect class), block (several; chosen by nearness), partial (the PS1 plays only
some of the arcade effect's notes), seq-first (a sequence of echoes with no arcade effect of the same timeline; the first
note's effect is used), inferred (a neighbor sound in the list is the arcade's code minus or plus one and the sample has
no arcade source), or none with the reason: no-source (the sample has no arcade source: the PS1's own), no-effect (an
arcade sample that no effect plays at this pitch), stop (a list entry that only stops or sets volume).

Three further steps give every sound its final outcome (apply_decisions). A sound whose sample matches an arcade record that starts up to 64
bytes after its own start counts as inside it (the PS1 sample begins with a warm-up block). Sounds without an arcade source that sit between
two certain sounds of a fighter's list whose codes differ by exactly the number of sounds take the codes in between ('inferred': the arcade
numbers a fighter's sounds in list order). The HAND table places the sounds that the measurement cannot (the PS1's own recordings of menu
and HUD sounds) by the arcade's 68000 program: the arcade function that does the same at the same moment calls the effect; its rows carry
their evidence; HAND_OVERRIDE rows replace a measured code where the arcade's own code shows another effect. Every other sound without a code
(a stop, no arcade source, no effect, no record) is silent (code 0xfffe in the table, ARCADE_SOUND_SILENT): the arcade
plays nothing there. No row keeps 0xffff, so no request with a row falls back to the PS1's sound banks.

In the analysis a row is keyed by (hash of the VAB header, hash of the run of entries): the same pair is the same sound in every file that
has it (25 different banks and lists on the disc). The runtime table is keyed otherwise (the runtime reads no part of the disc's sound
data): by a bank id and the sound's start. A bank is a VAB header chunk of the disc (a file and a slot, 120 of them, numbered by their place in
the sorted list, from 1); the runtime builds each header itself (sounds.c) with the id in its id field, and a request finds its cell by the id
of the header in its slot and its start. One cell for each place a row has in the lists (1654 cells). Reads the disc image and the arcade set
(both only read); writes nothing. It prints numbers and codes, never game bytes. An analysis and reference tool; the runtime table is
emitted by `--c-table`, and the runtime that uses it is C.

    python3 port/arcade/sounds.py report  --disc BIN --arcade ZIP [--matches FILE]
    python3 port/arcade/sounds.py list    --disc BIN --arcade ZIP [--matches FILE] [--only NAME]   # every sound, one line
    python3 port/arcade/sounds.py callsites --disc BIN --arcade ZIP [--matches FILE] [--src ps1/src]   # the constants the PS1 C requests
    python3 port/arcade/sounds.py --c-table --disc BIN --arcade ZIP [--matches FILE]   # prints port/arcade/sounds_table.c

--matches is samples.py's kept matches (`samples.py match --save FILE`, 223 s on 32 cores); without it the match is run
first.
"""
import argparse
import bisect
import collections
import hashlib
import math
import os
import re
import struct
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import music            # noqa: E402
import samples          # noqa: E402

ROOT = Path(__file__).resolve().parents[2]
LIST_SLOTS = (4, 5, 6, 7)          # table 3 slots that hold sound command lists; list slot s belongs to bank slot s - 4
SEP_BASE = 0x100                   # table 5 slot 0x100 + k is the SEP file of bank slot k
NOTE_ON, PLAY_SEQ, VOLUME, STOP = 0, 1, 9, 11
TIME_TOL, TIME_REL = 0.040, 0.03   # seconds, and a share of the time
PITCH_TOL = 0.75                   # semitones
ROOT60_SHIFT = 1.0                 # the PS1's tool plays the arcade's root-60 instruments this much above the formula
FIRST_PASS_TICK = 3                # the driver's first pass is at interrupt 3 (12 ms): an effect's time zero
EFFECT_TICKS = 2500                # 10 s of interrupts are run for each effect
# The arcade's pan from the object's distance dx to the camera (clamped to 0 .. 0x17f): the first dx of each run of
# pan 9 .. 23 in the table at 0x13ccc of the 68000 program (music.md section 10; read from the decrypted analysis copy).
PAN_EDGES = (0, 6, 20, 40, 66, 97, 132, 171, 213, 252, 287, 318, 344, 364, 378)
PAN_FIRST = 9
# A PS1 sample made from an arcade record starts a few bytes before the record (the PS1's warm-up block): 2, 24 and 37 bytes were
# measured, so an address up to this many bytes below a record's start counts as inside it.
RECORD_SLACK = 64
SILENT = 0xfffe                    # the table's code for a request that plays nothing on purpose (ARCADE_SOUND_SILENT in sounds.h)


# ---------------------------------------------------------------- pure functions (tested without game data)

def decode_entry(w0, w1):
    """The fields of a command entry (two little-endian words of the list)."""
    return dict(cmd=(w0 >> 12) & 15, low=w0 & 0xfff, pan=(w0 >> 16) & 0xff, prog=(w0 >> 24) & 0xff,
                tone=(w1 >> 12) & 15, chan=(w1 >> 7) & 31, prio=(w1 >> 16) & 0x7f, vol=w1 & 0x7f,
                note=(w1 >> 24) & 0x7f, more=(w1 >> 23) & 1)


def parse_list(chunk):
    """[(word0, word1)] of a list chunk (8 bytes an entry; a cut-off last entry is ignored)."""
    return [struct.unpack_from('<II', chunk, i) for i in range(0, len(chunk) - 7, 8)]


def sound_groups(entries):
    """[(start, end)] (end exclusive): a sound is the run from `start` to the first entry whose bit 23 of word 1 is clear,
    as the player's loop `while (word1 & 0x800000)` runs it. The groups tile the list."""
    out, start = [], 0
    for i, (_, w1) in enumerate(entries):
        if not (w1 >> 23) & 1:
            out.append((start, i + 1))
            start = i + 1
    if start < len(entries):
        out.append((start, len(entries)))
    return out


def fnv1a(data, h=0x811c9dc5):
    for b in data:
        h = ((h ^ b) * 0x01000193) & 0xffffffff
    return h


def vab_header_length(vh):
    """The bytes of a VAB header chunk: 32-byte head, 128 program records of 16 bytes, 16 tones of 32 bytes for each program,
    and the 256 sample sizes of 2 bytes (3104 bytes for one program, 3616 for two, 4128 for three)."""
    if vh[:4] != b'pBAV':
        raise ValueError('not a VAB header')
    programs = struct.unpack_from('<H', vh, 0x12)[0]
    return 0x20 + 128 * 16 + programs * 16 * 32 + 512


def semitones(rate, note, center, shift):
    """The PS1 note in semitones relative to the arcade sample's natural rate (24,000 Hz): the sample was resampled to `rate` Hz
    and the tone plays 44,100 samples a second at its center key: 12 log2(44100 / rate) + (note - center) + shift / 128."""
    return 12 * math.log2(44100.0 / rate) + (note - center) + shift / 128.0


def arcade_semitones(pitch, root):
    """The arcade note in the same unit: the driver's rate is about 4096 * 2^((key - 1 - root) / 12) (music.md section 5); `pitch` is the
    key in the high byte and the fraction (1/256 semitone, with the program's tuning) in the low byte."""
    return pitch / 256.0 - 1 - root


def tone_by_index(tones, prog, tone):
    for t in tones:
        if t['program'] == prog and t['tone'] == tone:
            return t
    return None


def tone_by_key(tones, prog, key):
    """The tone a SEP note selects: of the program, the one whose key range holds the key (the first, if several)."""
    for t in tones:
        if t['program'] == prog and t['low'] <= key <= t['high']:
            return t
    return None


def read_vlq(c, p):
    v = 0
    while True:
        b = c[p]
        p += 1
        v = v << 7 | (b & 0x7f)
        if not b & 0x80:
            return v, p


def parse_sep(c):
    """[dict(id, res, tempo (microseconds a quarter), events)] of a SEP file (magic 'pQES', u16 version, then per sequence: u16 id,
    u16 resolution, 3 bytes tempo, 2 bytes time signature, u32 size, then MIDI events). Events are (tick, kind, channel, a, b) with
    kind 9 note on, 8 note off, 12 program change, 'tempo' for the meta event 0x51 (a = microseconds)."""
    if c[:4] != b'pQES':
        raise ValueError('not a SEP file')
    p, seqs = 6, []
    while p + 13 <= len(c):
        sid, res = struct.unpack_from('>HH', c, p)
        tempo = int.from_bytes(c[p + 4:p + 7], 'big')
        size = struct.unpack_from('>I', c, p + 9)[0]
        if size == 0 or p + 13 + size > len(c):
            break
        q, end, t, run, ev = p + 13, p + 13 + size, 0, None, []
        while q < end:
            d, q = read_vlq(c, q)
            t += d
            b = c[q]
            if b & 0x80:
                run = b
                q += 1
            else:
                b = run
            hi, ch = b & 0xf0, b & 15
            if b == 0xff:
                m = c[q]
                n, q = read_vlq(c, q + 1)
                if m == 0x51 and n == 3:
                    ev.append((t, 'tempo', 0, int.from_bytes(c[q:q + 3], 'big'), 0))
                q += n
                if m == 0x2f:
                    break
            elif hi in (0x80, 0x90, 0xa0, 0xb0, 0xe0):
                ev.append((t, hi >> 4, ch, c[q], c[q + 1]))
                q += 2
            elif hi in (0xc0, 0xd0):
                ev.append((t, hi >> 4, ch, c[q], 0))
                q += 1
            else:
                raise ValueError('bad status byte in the sequence')
        seqs.append(dict(id=sid, res=res, tempo=tempo, events=ev))
        p += 13 + size
    return seqs


def sep_notes(seq):
    """[(seconds, program, key, velocity)] of the note-ons (velocity > 0) of a sequence, with the program of the channel at that
    time (0 before any program change) and tempo changes followed."""
    out, prog, tempo, last_tick, secs = [], collections.defaultdict(int), seq['tempo'], 0, 0.0
    for t, kind, ch, a, b in seq['events']:
        secs += (t - last_tick) * tempo / (seq['res'] * 1e6)
        last_tick = t
        if kind == 'tempo':
            tempo = a
        elif kind == 12:
            prog[ch] = a
        elif kind == 9 and b > 0:
            out.append((secs, prog[ch], a, b))
    return out


def pan_from_dx(dx):
    """The arcade's pan (9 .. 23 of 0 .. 32, 16 the centre) for an object dx pixels right of the camera's left edge."""
    dx = max(0, min(dx, 0x17f))
    return PAN_FIRST + bisect.bisect_right(PAN_EDGES, dx) - 1


def times_agree(a, b):
    return abs(a - b) <= TIME_TOL + TIME_REL * max(a, b)


def pitch_delta_ok(x_ps1, x_arc, root):
    return abs(x_ps1 - x_arc - (ROOT60_SHIFT if root == 60 else 0.0)) < PITCH_TOL


def match_timeline(ps1, arc, mode='exact'):
    """Does the PS1 sound's timeline fit the arcade effect's? ps1: [(seconds, set of instrument indexes, semitones)]; arc: [(seconds,
    instrument, semitones, root)]. Every PS1 event takes a different arcade event with the same instrument and a time within the
    tolerance; the pitch must be within PITCH_TOL unless mode is 'instrument'. mode 'exact': the arcade has no event left over; 'prefix':
    the PS1 events fit the arcade's first events (the SEP file ends before the arcade effect does); 'subset' and 'instrument': any events.
    Greedy by time; the lists are short (at most about 110 events)."""
    if not ps1 or len(ps1) > len(arc) or (mode == 'exact' and len(ps1) != len(arc)):
        return False
    if mode == 'prefix':
        arc = sorted(arc, key=lambda e: e[0])[:len(ps1)]
    free = list(range(len(arc)))
    for t, insts, x in sorted(ps1, key=lambda e: e[0]):
        hit = None
        for k in free:
            at, ai, ax, root = arc[k]
            if ai in insts and times_agree(t, at) and (mode == 'instrument' or pitch_delta_ok(x, ax, root)):
                hit = k
                break
        if hit is None:
            return False
        free.remove(hit)
    return True


def effect_classes(effects):
    """{code: class id}: effects whose priority and events (time, instrument, key, length, volume, echo) are equal share a class, the
    id being the lowest code. {class id: [codes]} second."""
    by_sig = collections.defaultdict(list)
    for code, e in effects.items():
        by_sig[(e['prio'], tuple((ev['tick'], ev['instrument'], ev['key'], ev['length'], ev['volume'], ev['echo']) for ev in e['events']))].append(code)
    cls, members = {}, {}
    for codes in by_sig.values():
        codes.sort()
        members[codes[0]] = codes
        for c in codes:
            cls[c] = codes[0]
    return cls, members


def side_codes(codes, voices):
    """(code for side 0, code for side 1) of an effect class: the lowest code, and the lowest member whose voices are the first one's
    plus two (the arcade's second player plays on voices 14 and 15 where the first plays on 12 and 13), else a member with other
    voices and a higher code, else the same code. `voices` is {code: sorted voice list}."""
    codes = sorted(codes)
    c0 = codes[0]
    v0 = voices[c0]
    shifted = [c for c in codes[1:] if voices[c] == [v + 2 for v in v0] and v0]
    if shifted:
        return c0, shifted[0]
    other = [c for c in codes[1:] if voices[c] != v0 and c > c0]
    return (c0, other[0]) if other else (c0, c0)


def nearest_choice(options, refs):
    """Of the class ids `options` take the one nearest to a reference code (the codes of the list's certain sounds), ties to the lower;
    no references: the lowest."""
    if not refs:
        return min(options)
    return min(options, key=lambda c: (min(abs(c - r) for r in refs), c))


# ---------------------------------------------------------------- the disc and the arcade set

def read_pacs(disc_path):
    """{archive name: [(table, slot, data)]} of every chunk archive of the disc."""
    out = {}
    with open(disc_path, 'rb') as f:
        files, sec = samples.iso_files(f)
        for path in sorted(files):
            if not path.upper().endswith('.PAC'):
                continue
            lba, size = files[path]
            data = sec(lba, (size + 2047) // 2048)[:size]
            try:
                out[path.rsplit('/', 1)[1][:-4]] = samples.pac_chunks(data)
            except samples.FormatError:
                continue
    return out


def role_of(name, slot):
    """What a list is for: by archive name and list slot."""
    if name.startswith('PL'):
        return 'fighter'
    if name.startswith('FIRSTSND'):
        return 'title bank' if slot < 6 else ('shared bank 2' if slot == 6 else 'shared bank 3')
    if name.startswith('GAME_SND'):
        return 'shared bank 2' if slot == 6 else 'shared bank 3'
    if name.startswith('CONT'):
        return 'continue'
    if name.startswith('END'):
        return 'ending'
    if name.startswith('SELECT'):
        return 'select'
    if name.startswith('DEMO'):
        return 'demo'
    return 'other'


def build_contexts(pacs):
    """[dict(key, names, slot, role, vh, info, vags, entries, sep)] one per distinct (header, body, list, sequence file): the archives that
    have a list in table 3 slot 4 to 7 together with the bank of the same slot minus four (table 3 and 4) and the SEP file (table 5)."""
    ctxs, seen = [], {}
    for name in sorted(pacs):
        chunks = pacs[name]
        for slot in LIST_SLOTS:
            lists = [d for t, s, d in chunks if t == 3 and s == slot]
            if not lists:
                continue
            k = slot - 4
            vh = [d for t, s, d in chunks if t == 3 and s == k and d[:4] == b'pBAV']
            vb = [d for t, s, d in chunks if t == 4 and s == k]
            sep = [d for t, s, d in chunks if t == 5 and s == SEP_BASE + k]
            if not vh or not vb:
                continue
            vh = vh[0][:vab_header_length(vh[0])]
            sepdata = sep[0] if sep else b''
            ident = (hashlib.md5(vh).digest(), hashlib.md5(vb[0]).digest(), hashlib.md5(lists[0]).digest(), hashlib.md5(sepdata).digest())
            if ident in seen:
                seen[ident]['names'].append((name, slot))
                continue
            info = samples.parse_vh(vh)
            ctx = dict(names=[(name, slot)], slot=slot, role=role_of(name, slot), vh=vh, info=info,
                       vags=[hashlib.md5(s).hexdigest() for s in samples.vag_slices(vb[0], info['sizes'])],
                       entries=parse_list(lists[0]), sep=parse_sep(sepdata) if sepdata else [])
            seen[ident] = ctx
            ctxs.append(ctx)
    return ctxs


def arcade_effects(rom):
    """{code: dict(prio, events, voices)} of the 868 effects: music.py runs each for EFFECT_TICKS interrupts. events = [(tick, instrument,
    key, length, volume, echo, voice)] as dictionaries of the driver."""
    out = {}
    for code in range(rom.command_count()):
        lin, kind = rom.command_entry(code)
        if kind != 'effect':
            continue
        drv = music.Driver(rom)
        drv.command([code >> 8, code & 255])
        for _ in range(EFFECT_TICKS):
            drv.step()
        out[code] = dict(prio=rom.d[lin], events=drv.events, voices=sorted({e['voice'] for e in drv.events}))
    return out


def arcade_timeline(effect, instruments):
    return [((ev['tick'] - FIRST_PASS_TICK) / 250.0, ev['instrument'], arcade_semitones(ev['pitch'], instruments[ev['instrument']]['root']),
             instruments[ev['instrument']]['root']) for ev in effect['events']]


# ---------------------------------------------------------------- resolving the PS1 sounds

def contain(instruments, address):
    """Indexes of the arcade instrument records that hold `address`. An address in no record is looked up again from RECORD_SLACK
    bytes before each record's start (an address inside a record never takes the next one)."""
    inside = [r['index'] for r in instruments if r['start'] <= address <= r['end']]
    if inside:
        return inside
    return [r['index'] for r in instruments if r['start'] - RECORD_SLACK <= address < r['start']]


def ps1_event(ctx, matches, instruments, tone, note, when):
    """Event dict of a PS1 note: (when, instruments or None, semitones, state). state: ok, weak (the sample's match covers under 50 percent),
    notone, nosource, norecord."""
    if tone is None:
        return dict(t=when, insts=None, x=None, state='notone')
    m = matches.get(ctx['vags'][tone['vag'] - 1])
    if not m or not m.get('rate'):
        return dict(t=when, insts=None, x=None, state='nosource')
    insts = contain(instruments, m['address'])
    x = semitones(m['rate'], note, tone['center'], tone['shift'])
    state = 'ok' if m['cover'] >= 50 else 'weak'
    return dict(t=when, insts=set(insts) if insts else None, x=x, state=state if insts else 'norecord')


def sound_events(ctx, matches, instruments, start, end):
    """(events, has_note, has_sequence, has_only_stop) of the run of entries [start, end) of a context."""
    events, notes, seqs, other = [], 0, 0, 0
    tones = ctx['info']['tones']
    for w0, w1 in ctx['entries'][start:end]:
        e = decode_entry(w0, w1)
        if e['cmd'] == NOTE_ON:
            if e['vol'] == 0:
                continue                      # a silent note: the PS1's own device to hold a channel
            notes += 1
            events.append(ps1_event(ctx, matches, instruments, tone_by_index(tones, e['prog'], e['tone']), e['note'], 0.0))
        elif e['cmd'] == PLAY_SEQ:
            seqs += 1
            seq = next((s for s in ctx['sep'] if s['id'] == e['low']), None)
            if seq is None:
                events.append(dict(t=0.0, insts=None, x=None, state='noseq'))
                continue
            for secs, prog, key, vel in sep_notes(seq):
                events.append(ps1_event(ctx, matches, instruments, tone_by_key(tones, prog, key), key, secs))
        else:
            other += 1
    return events, notes, seqs, other


def event_signature(events):
    return tuple((round(e['t'], 3), tuple(sorted(e['insts'])) if e['insts'] else None, None if e['x'] is None else round(e['x'], 2), e['state'])
                 for e in events)


def classify(events, notes, seqs):
    if not events and not notes and not seqs:
        return 'stop'
    if notes and seqs:
        return 'note+sequence'
    return 'sequence' if seqs else 'note'


def candidate_effects(events, timelines, by_inst, mode):
    """Codes of the arcade effects whose timeline fits the PS1 events that resolved (see match_timeline for the modes)."""
    ps1 = [(e['t'], e['insts'], e['x']) for e in events]
    cands = set()
    for e in events:
        for i in e['insts']:
            cands |= by_inst.get(i, set())
    return sorted(c for c in cands if match_timeline(ps1, timelines[c], mode))


def resolve_rows(ctxs, matches, instruments, effects):
    """One row per (header hash, entry-run hash): dict(key, contexts [(name, slot, start)], role, kind, method, reason, code0, code1,
    classes, alt (other classes that fit), events, notes, sequences, weak). Four passes: each sound against the arcade's timelines (exact, then
    prefix, subset, instrument-only); the choice among several effect classes by nearness to the certain sounds of the list; a sequence
    that no effect matches takes the effect of its first note; a sound whose sample has no arcade source between two sounds of the
    list with codes two apart takes the code between."""
    cls, members = effect_classes(effects)
    voices = {c: effects[c]['voices'] for c in effects}
    timelines = {c: arcade_timeline(effects[c], instruments) for c in effects}
    by_inst = collections.defaultdict(set)
    for c, tl in timelines.items():
        for _, i, _, _ in tl:
            by_inst[i].add(c)
    rows, index = [], {}
    for ci, ctx in enumerate(ctxs):
        vhh = fnv1a(ctx['vh'])
        for gi, (start, end) in enumerate(sound_groups(ctx['entries'])):
            raw = b''.join(struct.pack('<II', *e) for e in ctx['entries'][start:end])
            key = (vhh, fnv1a(raw))
            events, notes, seqs, _ = sound_events(ctx, matches, instruments, start, end)
            if key in index:
                index[key]['contexts'].extend((n, s, start) for n, s in ctx['names'])
                if event_signature(events) != event_signature(index[key]['events']):
                    index[key]['conflict'] = True      # the same bytes in two banks that differ in a way that matters: never seen
                continue
            row = dict(key=key, ctx=ci, group=gi, start=start, role=ctx['role'], kind=classify(events, notes, seqs),
                       contexts=[(n, s, start) for n, s in ctx['names']], events=events, notes=notes, sequences=seqs,
                       method='none', reason=None, code0=None, code1=None, classes=[], alt=[], weak=False, conflict=False)
            index[key] = row
            rows.append(row)

    def finish(r, klass, method):
        r['classes'], r['method'], r['reason'] = [klass], method, None
        r['code0'], r['code1'] = side_codes(members[klass], voices)

    # pass 1
    for r in rows:
        events = r['events']
        if r['kind'] == 'stop':
            r['reason'] = 'stop'
            continue
        ok = [e for e in events if e['state'] in ('ok', 'weak')]
        r['weak'] = any(e['state'] == 'weak' for e in ok)
        if not ok:
            states = {e['state'] for e in events}
            r['reason'] = 'no-record' if 'norecord' in states else 'no-source'
            continue
        full = len(ok) == len(events)
        for mode, method in (('exact', 'exact'), ('prefix', 'prefix'), ('subset', 'partial'), ('instrument', 'instrument')):
            if mode == 'exact' and not full:
                continue
            found = candidate_effects(ok, timelines, by_inst, mode)
            if found:
                r['classes'], r['method'] = sorted({cls[c] for c in found}), method
                break
        else:
            r['reason'] = 'no-effect' if full else 'no-source'
    # pass 2: choose among several classes
    for ci in range(len(ctxs)):
        certain = [r['classes'][0] for r in rows if r['ctx'] == ci and len(r['classes']) == 1 and r['method'] == 'exact']
        for r in rows:
            if r['ctx'] != ci or not r['classes']:
                continue
            if len(r['classes']) > 1:
                pick = nearest_choice(r['classes'], certain)
                r['alt'] = [c for c in r['classes'] if c != pick]
                r['classes'] = [pick]
                if r['method'] == 'exact':
                    r['method'] = 'block'
            finish(r, r['classes'][0], r['method'])
    # pass 3: a sequence no effect matches takes the effect of its first note
    for r in rows:
        if r['method'] == 'none' and r['reason'] in ('no-effect', 'no-source') and r['kind'] in ('sequence', 'note+sequence'):
            ok = sorted((e for e in r['events'] if e['state'] in ('ok', 'weak')), key=lambda e: e['t'])[:1]
            if ok:
                for mode in ('subset', 'instrument'):
                    one = sorted({cls[c] for c in candidate_effects(ok, timelines, by_inst, mode)})
                    if one:
                        certain = [x['classes'][0] for x in rows if x['ctx'] == r['ctx'] and len(x['classes']) == 1 and x['method'] == 'exact']
                        finish(r, nearest_choice(one, certain), 'seq-first')
                        break
    # pass 4: runs. The arcade numbers a fighter's sounds one after the other in the order of the list, so when two certain sounds
    # a and b of a list have codes that differ by exactly the number of sounds between them, the sounds between them that have no
    # arcade source take the codes in between (the sample is the PS1's own, the arcade's effect is in the same place).
    for ci in range(len(ctxs)):
        mine = sorted((r for r in rows if r['ctx'] == ci), key=lambda r: r['group'])
        i = 1
        while i < len(mine) - 1:
            a = mine[i - 1]
            j = i
            while j < len(mine) and mine[j]['method'] == 'none' and mine[j]['reason'] == 'no-source' and mine[j]['kind'] == 'note':
                j += 1
            if j > i and j < len(mine):
                b = mine[j]
                if a['method'] == b['method'] == 'exact' and b['code0'] - a['code0'] == j - i + 1 \
                        and all(a['code0'] + k in effects for k in range(1, j - i + 1)):
                    for k in range(i, j):
                        finish(mine[k], cls[a['code0'] + k - i + 1], 'inferred')
            i = max(j, i + 1)
    return rows


# ---------------------------------------------------------------- decisions: the hand table and the silent sounds

# Sounds that the measurement cannot place (the PS1 has its own recording, or no arcade effect plays the sample) but whose moment the
# arcade's 68000 program shows: the PS1 game logic comes from the arcade's, so the arcade function that does the same thing at the same
# moment calls the effect. Each row is (archive, list slot, entries, code for side 0, code for side 1, evidence); the evidence names
# addresses of the decrypted 68000 program (music.md section 10) and what the code there does. The sound wrappers are 0x13c5e and
# 0x13c64; the small routines that call them from 0x13e4c on are named below by their address. A row applies to a sound that the
# measurement left without a code; SILENT rows say that the arcade has no such moment. Numbers and addresses only.
HAND = (
    ('SELECT', 4, (0,), 0x82, 0x83,
     'select screen, cursor moved to another fighter: 0x627c8 (and twins 0x62bd0, 0x62f04) compares the player cursor (+0x82 of the player) with'
     ' the selector object and calls 0x13e8c, which sends 0x82 plus the player bit (+0x81); the PS1 does the same compare and plays entry 0'),
    ('SELECT', 4, (1,), 0x84, 0x85,
     'select screen, fighter picked: 0x628a0 adds 2 to the state (+6) then 0x628a4 calls 0x13eb0, which sends 0x84 plus the player bit;'
     ' the PS1 advances its state (+1) and plays entry 1; the same three twins as entry 0'),
    ('CONT00', 4, (6,), 0x91, 0x91,
     'results screen, count 0x1f then next step: 0x7052 subtracts 1 from the timer (a5+0xa), adds 2 to the step, sets the flag 0x161 and'
     ' jumps to 0x13f7c, which sends 0x91 (an echo of four notes of one instrument); the PS1 sound is an echo of five notes'
     ' (counter 0x1f, flag set, entry 6)'),
    ('CONT00', 4, (7,), 0x8a, 0x8a,
     'results screen, voice of the count: 0x85a38 tests the timer of the player object for 0x5f, sends the voice 0xb0 + value (0x15732) and'
     ' jumps to 0x13f2e, which sends 0x8a; the PS1 plays the voice at timer 0x5f and entry 7 one frame later'),
    ('CONT00', 4, (8,), 0x84, 0x84,
     'results screen, step with the 0x3c timer: 0x6db6 adds 2 to the step, stores 0x3c in a5+0xe, sets the flag 0x161 and jumps to 0x13ed4,'
     ' which sends 0x84; the PS1 sets its timer to 0x3c and the flag 0xff and plays entry 8 (the same recording as select entry 1)'),
    ('FIRSTSND', 7, (34,), 0x12f, 0x22f,
     'dizzy: 0x5da44 spawns three star objects (random side, 1 in 256 the third kind, flag a6+0x296 set) and jumps to 0x1484a, which sends'
     ' 0x12f plus the player bit; the PS1 spawn_three_effect_objects_with_sound is the same routine (health test, random 0x25, three objects,'
     ' flag set) and plays 0x322'),
    ('FIRSTSND', 7, (76,), 0x86, 0x87,
     'win marker: 0x82fa4 puts the marker of win number (+0xa6 of the player) minus one into the strip table of the team (+0x59) and'
     ' 0x82ffa jumps to 0x13eee, which sends 0x86 plus the team; the PS1 update_win_marker_strips plays 0x34c for the same marker'),
    ('FIRSTSND', 7, (77,), 0x88, 0x89,
     'score count-up tick: 0x5c12 transfers the bonus into the score with sbcd/abcd and, when the frame counter and 3 is zero (0x5c66), jumps'
     ' to 0x5c70 -> 0x13f0e, which sends 0x88 plus the player bit (+0x81); at the end 0x5c98 sends it once more; the PS1'
     ' transfer_bcd_amount_to_score plays 0x34d at both places'),
    ('FIRSTSND', 7, (0, 48), 0x100, 0x200,
     'hit sounds: the PS1 picks the hit sound from a table of eight strengths (0x300, 0x302, 0x304, 0x306, 0x309, 0x30b, 0x30d, 0x30f)'
     ' and eight for a knock-out (0x330, 0x333, ...); the measured rows of the ladder are the arcade codes 0x101, 0x102, 0x104, 0x105, 0x106'
     ' at ladder places 1, 2, 4, 5, 6, so place 0 is 0x100 (5 of 5 places agree, none disagrees)'),
    ('PL03', 4, (6,), 0x1a6, 0x2a6,
     'BY POSITION, not by sample (the arcade plays no effect with this sample): the list neighbors entries 5 and 7 have the codes 0x1a5 and 0x1a7,'
     ' and the arcade numbers a fighter\'s sounds in list order'),
    ('PL06', 4, (15,), 0x116, 0x216,
     'BY POSITION, not by sample (the arcade plays no effect with this sample): the list neighbors entries 13 and 16 have the codes 0x114 and 0x117,'
     ' three apart for three places, and the arcade numbers a fighter\'s sounds in list order'),
    ('FIRSTSND', 7, (39,), SILENT, SILENT,
     'dizzy star hops (the PS1 plays 0x327 on each step flag of a star): the arcade 0x5da44 plays only the one sound at the spawn'),
    ('SELECT', 4, tuple(range(2, 25)), SILENT, SILENT,
     'the 23 recordings of fighter names: the PS1 plays them from a table of its select screen (21 fighter places, entry place + 2); the arcade'
     ' select code (0x627c8 to 0x63082) calls no other sound than the cursor and the pick, and the fighter voice routine 0x157a4'
     ' is called only from a fighter state (0x228c8, flag a6+0x29b), not from the select screen'),
    ('DEMO', 6, (2, 3, 4, 5), SILENT, SILENT,
     'the PS1 menus (mode, options, button configuration, memory card) that play these entries have no arcade screen; the arcade'
     ' boot code plays only the coin sounds 0x80 and 0x81 (0x1726, 0x1738)'),
)

# Rows that replace what the measurement found, where the arcade's own code shows another effect at that moment. Same shape as HAND.
HAND_OVERRIDE = (
    ('CONT00', 4, (5,), 0xae, 0xae,
     'results screen, count 0x2f then spawn three blocks: 0x7006 subtracts 1 from the timer (a5+0xa), adds 2 to the step, stores 0x2f in the'
     ' timer, calls 0x15718 (the constant 0xae) and, from the fourth rank on (a5+0x6116 >= 4), pops three pool blocks with the handler words'
     ' 0x2601, 0x7e00 and 0x7e01. The PS1 update_countdown_and_spawn_blocks is that routine (counter 0x2f, entry 5, blocks 0x26 mode 1,'
     ' 0x7e mode 0 and 1). The measurement matched the PS1 sample to 0x34b, an effect of a fighter block that this arcade routine does not call'),
    ('FIRSTSND', 7, (6,), 0x103, 0x203,
     'hit ladder by position: the PS1 picks the hit sound from eight strengths (0x300, 0x302, 0x304, 0x306, 0x309, 0x30b, 0x30d, 0x30f) and'
     ' five of them measured as the arcade codes 0x101, 0x102, 0x104, 0x105 and 0x106 at places 1, 2, 4, 5, 6; place 3 is 0x103 (the'
     ' arcade routine 0x14078 sends 0x103 or 0x203 by player). 0x306 plays a louder second note than 0x304, as 0x103 does against 0x102'),
    ('FIRSTSND', 7, (15,), 0x107, 0x207,
     'hit ladder by position: place 7 is 0x107 (routine 0x14136 sends 0x107 or 0x207 by player); the measurement took 0x106 because both'
     ' effects use the same two instruments'),
    ('FIRSTSND', 7, (57,), 0x103, 0x203,
     'knock-out hit ladder by position: 0x333, 0x336, 0x33c, 0x340 and 0x343 measured as 0x101, 0x102, 0x104, 0x105 and 0x106, so 0x339 is'
     ' place 3, 0x103'),
    ('FIRSTSND', 7, (70,), 0x107, 0x207,
     'knock-out hit ladder by position: 0x346 is place 7, 0x107'),
)


def apply_decisions(rows, hand=HAND, override=HAND_OVERRIDE):
    """Give every row its final outcome. The override table replaces what the measurement found; the hand table gives codes to rows without
    one (a row of the hand table that the measurement mapped is left alone). Every row still without a code is silent (row['silent'] = 'stop'
    (the arcade has no PS1 channel to stop), 'no-source', 'no-effect', 'no-record', or 'hand' for a silent hand row): no row falls back to
    the PS1's own sound banks."""
    by_place = {}
    for r in rows:
        for name, slot, start in r['contexts']:
            by_place.setdefault((name, slot, start), r)
    for table, force in ((override, True), (hand, False)):
        for archive, slot, entries, code0, code1, _ in table:
            for start in entries:
                r = by_place.get((archive, slot, start))
                if r is None or (r['code0'] is not None and not force):
                    continue
                if code0 == SILENT:
                    r['silent'] = 'hand'
                else:
                    r['code0'], r['code1'], r['method'], r['reason'], r['alt'] = code0, code1, 'hand', None, []
    for r in rows:
        if r['code0'] is None and not r.get('silent') and r['reason'] in ('stop', 'no-source', 'no-effect', 'no-record'):
            r['silent'] = r['reason']
    return rows



# ---------------------------------------------------------------- reports

KIND_ORDER = ('note', 'sequence', 'note+sequence', 'stop')
ROLE_ORDER = ('fighter', 'title bank', 'shared bank 2', 'shared bank 3', 'continue', 'ending', 'select', 'demo', 'other')
METHODS = ('exact', 'block', 'prefix', 'partial', 'instrument', 'seq-first', 'inferred', 'hand')


def row_status(r):
    if r.get('silent'):
        return 'silent:' + r['silent']
    return r['method'] if r['method'] != 'none' else 'none:' + r['reason']


def report(rows, ctxs):
    print(f'{len(ctxs)} banks with lists; {len(rows)} distinct sounds (header + run of entries)')
    mapped = [r for r in rows if r['code0'] is not None]
    print(f'mapped {len(mapped)} of {len(rows)}: ' + ', '.join(f'{m} {sum(1 for r in rows if r["method"] == m)}' for m in METHODS))
    print('not mapped: ' + ', '.join(f'{k} {v}' for k, v in sorted(collections.Counter(row_status(r) for r in rows if r['code0'] is None).items())))
    silent = collections.Counter(r['silent'] for r in rows if r.get('silent'))
    left = [r for r in rows if r['code0'] is None and not r.get('silent')]
    print(f'silent on purpose: {sum(silent.values())} (' + ', '.join(f'{k} {v}' for k, v in sorted(silent.items())) + f'); still the PS1\'s own sound: {len(left)} (' +
          ', '.join(f'{k} {v}' for k, v in sorted(collections.Counter(row_status(r) for r in left).items())) + ')')
    print('by kind of sound:')
    for k in KIND_ORDER:
        sub = [r for r in rows if r['kind'] == k]
        if sub:
            print(f'  {k:14} {len(sub):4}  mapped {sum(1 for r in sub if r["code0"] is not None):4}  ' +
                  ', '.join(f'{s} {n}' for s, n in sorted(collections.Counter(row_status(r) for r in sub).items())))
    print('by list (role):')
    for role in ROLE_ORDER:
        sub = [r for r in rows if r['role'] == role]
        if sub:
            print(f'  {role:14} {len(sub):4}  mapped {sum(1 for r in sub if r["code0"] is not None):4}  ' +
                  ', '.join(f'{s} {n}' for s, n in sorted(collections.Counter(row_status(r) for r in sub).items())))
    weak = sum(1 for r in mapped if r['weak'])
    pairs = sum(1 for r in mapped if r['code1'] != r['code0'])
    print(f'{weak} mapped sounds rest on a sample match that covers under 50 percent; {pairs} have a code for side 1 other than the side 0 code;')
    print(f'{sum(1 for r in mapped if r["alt"])} had other fitting classes (block rule picked one); {sum(1 for r in rows if r["conflict"])} keys with differing banks')
    print('fighter lists (archive: sounds mapped, lowest and highest side-0 code of the exact ones):')
    blocks = collections.defaultdict(list)
    for r in rows:
        if r['role'] == 'fighter' and r['code0'] is not None and r['method'] == 'exact':
            for n, _, _ in r['contexts']:
                if not n.endswith('X') and n[:2] == 'PL':
                    blocks[n].append(r['code0'])
    print('  ' + '  '.join(f'{n} {len(c)} {min(c):03x}-{max(c):03x}' for n, c in sorted(blocks.items())))


def list_rows(rows, ctxs, only=None):
    for r in rows:
        names = sorted({n for n, _, _ in r['contexts']})
        if only and not any(only.upper() in n.upper() for n in names):
            continue
        first = r['contexts'][0]
        code = ('silent' if r.get('silent') else '-') if r['code0'] is None else (f'{r["code0"]:03x}' + (f'/{r["code1"]:03x}' if r['code1'] != r['code0'] else ''))
        alt = ' alt ' + ','.join(f'{c:03x}' for c in r['alt']) if r['alt'] else ''
        print(f'{first[0]:9} slot {first[1]} entry {first[2]:3} x{len(r["contexts"]):<3} {r["kind"]:13} {row_status(r):18} {code}{alt}')


def bank_list(pacs):
    """[(archive name, bank slot)] of every VAB header chunk of the disc (table 3, slots 0 to 3), sorted. A bank's id, written into its
    synthetic header (the VAB id field), is its place in this list plus one."""
    return sorted({(name, s) for name, chunks in pacs.items() for t, s, d in chunks if t == 3 and s < 4 and d[:4] == b'pBAV'})


def row_outcome(r, method_no):
    """(code for side 0, code for side 1, method number) of a row for the table."""
    reason_no = {'stop': 8, 'no-source': 9, 'no-record': 10, 'no-effect': 11, 'hand': 12}
    if r['code0'] is not None:
        return r['code0'], r['code1'], method_no[r['method']]
    if r.get('silent'):
        return SILENT, SILENT, reason_no[r['silent']]
    return 0xffff, 0xffff, reason_no[r['reason']]


def sound_cells(rows, banks):
    """[(bank id, start, row)] of every sound of every bank, sorted by (bank id, start): one cell for each place a row has in the lists."""
    ids = {b: i + 1 for i, b in enumerate(banks)}
    cells = {}
    for r in rows:
        for name, slot, start in r['contexts']:
            bank = ids[(name, slot - 4)]
            if (bank, start) in cells and cells[(bank, start)] is not r:
                raise ValueError(f'two sounds at {name} slot {slot} entry {start}')
            cells[(bank, start)] = r
    return [(b, s, cells[(b, s)]) for b, s in sorted(cells)]


def c_table(rows, effects, banks):
    """port/arcade/sounds_table.c: the banks (a file and a slot each) with their ids, and every sound as numbers, sorted by (bank id, start),
    so that a binary search finds it. The runtime finds a request's bank by the id in the header it built (sounds.c)."""
    method_no = {m: i for i, m in enumerate(METHODS)}
    out = ['/* Generated by port/arcade/sounds.py --c-table; numbers only. The PS1 sound requests as the arcade\'s effect codes (music.md 12b).',
           ' * arcade_sound_banks: the VAB header chunks of the disc by file (lower case) and bank slot; a bank\'s id (written into its synthetic header,',
           ' * sounds.c) is its place in the table plus one. arcade_sounds: bank id and start (the entry where the sound begins in the bank\'s command',
           ' * list) with the arcade effect for side 0 and side 1 (0xffff: none, the PS1 sound plays; 0xfffe: silent, nothing plays).',
           ' * method: 0 exact, 1 block, 2 prefix, 3 partial, 4 instrument, 5 seq-first, 6 inferred, 7 hand table (mapped); 8 stop only,',
           ' * 9 no arcade source for the sample, 10 no instrument record, 11 no arcade effect, 12 silent by the hand table (all silent, 0xfffe).',
           ' * flags: bit 0 the sample match is weak (under 50 percent). */',
           '#include "sounds.h"', '',
           '/* The arcade\'s pan (68000 program, table at 0x13ccc) for an object dx pixels (0 .. 0x17f) right of the camera: pan_first + the number of',
           ' * edges at or below dx, minus one. 16 is the centre of 0 .. 32. */',
           f'const unsigned arcade_sound_pan_first = {PAN_FIRST};',
           f'const unsigned short arcade_sound_pan_edges[{len(PAN_EDGES)}] = {{' + ', '.join(str(e) for e in PAN_EDGES) + '};',
           '',
           'const struct arcade_sound_bank arcade_sound_banks[] = {']
    for name, slot in banks:
        out.append(f'    {{"{name.lower()}.pac", {slot}}},')
    out += ['};', f'const unsigned arcade_sound_bank_count = {len(banks)};', '', 'const struct arcade_sound arcade_sounds[] = {']
    cells = sound_cells(rows, banks)
    for bank, start, r in cells:
        c0, c1, m = row_outcome(r, method_no)
        out.append(f'    {{{bank}, {start}, 0x{c0:03x}, 0x{c1:03x}, {m}, {1 if r["weak"] else 0}}},')
    out += ['};', f'const unsigned arcade_sound_count = {len(cells)};', '']
    return '\n'.join(out)


# ---------------------------------------------------------------- the constants of the PS1 C

CALL = re.compile(r'\b(play_sound_at|play_side_sound_at|call_func_with_side_x_origin)\s*\(([^;]*)\);')


def callsite_requests(src_root):
    """{(function, constant code): count} of the calls whose last argument is a plain number, in the C under ps1/src."""
    out = collections.Counter()
    variable = collections.Counter()
    for path in sorted(Path(src_root).rglob('*.c')):
        text = path.read_text(errors='replace')
        for m in CALL.finditer(text):
            last = m.group(2).rsplit(',', 1)[-1].strip()
            if re.fullmatch(r'0x[0-9a-fA-F]+|\d+', last):
                out[(m.group(1), int(last, 0))] += 1
            else:
                variable[m.group(1)] += 1
    return out, variable


def callsite_report(src_root, rows):
    """Count the PS1 C's calls with a plain-number request and say whether the sound it names is mapped in the lists that define it."""
    consts, variable = callsite_requests(src_root)
    where = collections.defaultdict(list)           # (bank slot, start) -> rows of the contexts whose list has a sound there
    for r in rows:
        for name, slot, start in r['contexts']:
            where[(slot - 4, start)].append(r)
    print('calls with a constant request, by function: ' + ', '.join(
        f'{f} {sum(n for (g, _), n in consts.items() if g == f)} calls, {len({c for (g, c) in consts if g == f})} codes' for f in sorted({g for g, _ in consts})))
    print('calls with a computed request (tables, arithmetic): ' + ', '.join(f'{f} {n}' for f, n in sorted(variable.items())))
    tot, codes = collections.Counter(), collections.Counter()
    own_calls, own_weight = 0, 0.0
    for (f, c), n in consts.items():
        if f == 'play_side_sound_at':
            # the side's own bank: song = side, so any fighter list; the share of the fighters whose sound at this entry maps
            fighters = {}
            for r in where.get((0, c & 255), []) + where.get((1, c & 255), []):
                for name, slot, start in r['contexts']:
                    if name.startswith('PL') and start == (c & 255):
                        fighters[name.rstrip('X')] = fighters.get(name.rstrip('X'), False) or r['code0'] is not None
            if fighters:
                own_calls += n
                own_weight += n * sum(fighters.values()) / len(fighters)
            continue
        rs = where.get((c >> 8, c & 255), [])
        mapped = sum(1 for r in rs if r['code0'] is not None)
        state = 'defined nowhere' if not rs else ('all contexts map' if mapped == len(rs) else ('some contexts map' if mapped else 'none maps'))
        tot[state] += n
        codes[state] += 1
    for k in ('all contexts map', 'some contexts map', 'none maps', 'defined nowhere'):
        print(f'  shared requests, {k}: {tot[k]} calls, {codes[k]} distinct requests')
    if own_calls:
        print(f'  own-bank requests (play_side_sound_at): {own_calls} calls; weighted by the fighters, {100 * own_weight / own_calls:.0f} percent map')


# ---------------------------------------------------------------- driver

def load(a):
    pacs = read_pacs(a.disc)
    ctxs = build_contexts(pacs)
    banks = bank_list(pacs)
    if a.matches and Path(a.matches).exists():
        matches = samples.parse_matches(Path(a.matches).read_text())
    else:
        keep = a.matches or str(Path(tempfile.gettempdir()) / f'sounds-matches-{os.getpid()}.tsv')
        samples.match(samples.read_banks(a.disc), a.arcade, save=keep, quiet=True)
        matches = samples.parse_matches(Path(keep).read_text())
        if not a.matches:
            os.unlink(keep)
    sound = samples.read_sound(a.arcade)
    instruments = samples.parse_instruments(sound)
    rom = music.Rom.from_zip(a.arcade)
    effects = arcade_effects(rom)
    return ctxs, apply_decisions(resolve_rows(ctxs, matches, instruments, effects)), effects, banks


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('command', nargs='?', default='report', choices=('report', 'list', 'callsites'))
    ap.add_argument('--disc')
    ap.add_argument('--arcade')
    ap.add_argument('--matches')
    ap.add_argument('--only')
    ap.add_argument('--c-table', action='store_true')
    ap.add_argument('--src', default=str(ROOT / 'ps1/src'))
    a = ap.parse_args(argv)
    if not (a.disc and a.arcade):
        ap.error('--disc and --arcade are required')
    ctxs, rows, effects, banks = load(a)
    if a.c_table:
        sys.stdout.write(c_table(rows, effects, banks))
    elif a.command == 'list':
        list_rows(rows, ctxs, a.only)
    elif a.command == 'callsites':
        callsite_report(a.src, rows)
    else:
        report(rows, ctxs)
        callsite_report(a.src, rows)
    return 0


if __name__ == '__main__':
    sys.exit(main())
