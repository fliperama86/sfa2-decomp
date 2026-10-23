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

A row is keyed by (hash of the VAB header, hash of the run of entries): the same pair is the same sound in every file that
has it (25 different banks and lists on the disc). Reads the disc image and the arcade set (both only read); writes
nothing. It prints numbers and codes, never game bytes. An analysis and reference tool; the runtime table is
emitted by `--c-table`, and the runtime that uses it will be C.

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
    return [r['index'] for r in instruments if r['start'] <= address <= r['end']]


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
    # pass 4: neighbors
    for ci in range(len(ctxs)):
        mine = sorted((r for r in rows if r['ctx'] == ci), key=lambda r: r['group'])
        for i in range(1, len(mine) - 1):
            r, a, b = mine[i], mine[i - 1], mine[i + 1]
            if r['method'] == 'none' and r['reason'] == 'no-source' and r['kind'] == 'note' and a['method'] == b['method'] == 'exact' \
                    and b['code0'] - a['code0'] == 2 and (a['code0'] + 1) in effects:
                finish(r, cls[a['code0'] + 1], 'inferred')
    return rows


# ---------------------------------------------------------------- reports

KIND_ORDER = ('note', 'sequence', 'note+sequence', 'stop')
ROLE_ORDER = ('fighter', 'title bank', 'shared bank 2', 'shared bank 3', 'continue', 'ending', 'select', 'demo', 'other')
METHODS = ('exact', 'block', 'prefix', 'partial', 'instrument', 'seq-first', 'inferred')


def row_status(r):
    return r['method'] if r['method'] != 'none' else 'none:' + r['reason']


def report(rows, ctxs):
    print(f'{len(ctxs)} banks with lists; {len(rows)} distinct sounds (header + run of entries)')
    mapped = [r for r in rows if r['code0'] is not None]
    print(f'mapped {len(mapped)} of {len(rows)}: ' + ', '.join(f'{m} {sum(1 for r in rows if r["method"] == m)}' for m in METHODS))
    print('not mapped: ' + ', '.join(f'{k} {v}' for k, v in sorted(collections.Counter(row_status(r) for r in rows if r['code0'] is None).items())))
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
        code = '-' if r['code0'] is None else (f'{r["code0"]:03x}' + (f'/{r["code1"]:03x}' if r['code1'] != r['code0'] else ''))
        alt = ' alt ' + ','.join(f'{c:03x}' for c in r['alt']) if r['alt'] else ''
        print(f'{first[0]:9} slot {first[1]} entry {first[2]:3} x{len(r["contexts"]):<3} {r["kind"]:13} {row_status(r):18} {code}{alt}')


def c_table(rows, effects):
    """port/arcade/sounds_table.c: every sound as numbers, sorted by (header hash, run hash), so that a binary search finds it."""
    ordered = sorted(rows, key=lambda r: r['key'])
    method_no = {m: i for i, m in enumerate(METHODS)}
    reason_no = {'stop': 8, 'no-source': 9, 'no-record': 10, 'no-effect': 11}
    out = ['/* Generated by port/arcade/sounds.py --c-table; numbers only. The PS1 sound requests as the arcade\'s effect codes (music.md 12b).',
           ' * key_vab: FNV-1a 32 of the VAB header (3104 bytes, 512 more for each program above one; the program count is at offset 0x12).',
           ' * key_run: FNV-1a 32 of the 8-byte entries of the sound, from `start` to the first entry without bit 23 of word 1.',
           ' * code0 / code1: the arcade effect for side 0 and side 1 (0xffff: none). method: 0 exact, 1 block, 2 prefix, 3 partial, 4 instrument,',
           ' * 5 seq-first, 6 inferred (mapped); 8 stop only, 9 no arcade source for the sample, 10 no instrument record, 11 no arcade effect (not mapped).',
           ' * flags: bit 0 the sample match is weak (under 50 percent). */',
           '#include "sounds.h"', '',
           '/* The arcade\'s pan (68000 program, table at 0x13ccc) for an object dx pixels (0 .. 0x17f) right of the camera: pan_first + the number of',
           ' * edges at or below dx, minus one. 16 is the centre of 0 .. 32. */',
           f'const unsigned arcade_sound_pan_first = {PAN_FIRST};',
           f'const unsigned short arcade_sound_pan_edges[{len(PAN_EDGES)}] = {{' + ', '.join(str(e) for e in PAN_EDGES) + '};',
           '',
           'const struct arcade_sound arcade_sounds[] = {']
    for r in ordered:
        first = r['contexts'][0]
        if r['code0'] is not None:
            c0, c1, m = r['code0'], r['code1'], method_no[r['method']]
        else:
            c0 = c1 = 0xffff
            m = reason_no[r['reason']]
        more = ' x%d' % len(r['contexts']) if len(r['contexts']) > 1 else ''
        out.append(f'    {{0x{r["key"][0]:08x}u, 0x{r["key"][1]:08x}u, 0x{c0:03x}, 0x{c1:03x}, {m}, {1 if r["weak"] else 0}}},'
                   f'  /* {first[0]} slot {first[1]} entry {first[2]}{more} */')
    out += ['};', f'const unsigned arcade_sound_count = {len(ordered)};', '']
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
    return ctxs, resolve_rows(ctxs, matches, instruments, effects), effects


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
    ctxs, rows, effects = load(a)
    if a.c_table:
        sys.stdout.write(c_table(rows, effects))
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
