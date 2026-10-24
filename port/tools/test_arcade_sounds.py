#!/usr/bin/env python3
"""Seconds: the sound-request mapping tool (port/arcade/sounds.py) on synthetic data; no game files.

The pure functions (list entries and sounds, hashes, header length, pitch, key-range tones, SEP files, pan, timeline fit, effect
classes and side codes, the choice among classes) run on made-up inputs, and a small made-up bank, list and set of effects go through
the whole resolution: an exact sound with a side pair, a sound with two effects to choose from, one with no arcade source, one with no
effect, a stop, a sequence of echoes that the arcade plays one note longer, and the C table. Also: the slack before an arcade record,
the run rule (sounds between two certain sounds whose codes differ by the number of sounds take the codes in between), the hand table
and the silent outcome (code 0xfffe).
"""
import hashlib
import math
import struct
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import sounds as K   # noqa: E402


def entry(cmd=0, low=0, pan=0xff, prog=0, tone=0, chan=16, prio=10, vol=100, note=60, more=0):
    w0 = (prog << 24) | (pan << 16) | (cmd << 12) | low
    w1 = (note << 24) | (more << 23) | (prio << 16) | (tone << 12) | (chan << 7) | vol
    return w0, w1


def vlq(v):
    out = [v & 0x7f]
    v >>= 7
    while v:
        out.append(0x80 | (v & 0x7f))
        v >>= 7
    return bytes(reversed(out))


def make_sep(seqs):
    """seqs = [(id, res, tempo, [(delta, status+data bytes)])]"""
    out = b'pQES' + struct.pack('>H', 0)
    for sid, res, tempo, events in seqs:
        body = b''.join(vlq(d) + e for d, e in events) + vlq(0) + bytes([0xff, 0x2f, 0])
        out += struct.pack('>HH', sid, res) + tempo.to_bytes(3, 'big') + bytes([4, 4]) + struct.pack('>I', len(body)) + body
    return out


class Pure(unittest.TestCase):
    def test_entry_fields(self):
        e = K.decode_entry(*entry(cmd=11, low=0x123, pan=0x40, prog=3, tone=9, chan=17, prio=5, vol=77, note=48, more=1))
        self.assertEqual((e['cmd'], e['low'], e['pan'], e['prog'], e['tone'], e['chan'], e['prio'], e['vol'], e['note'], e['more']),
                         (11, 0x123, 0x40, 3, 9, 17, 5, 77, 48, 1))

    def test_parse_list_and_groups(self):
        entries = [entry(more=1), entry(cmd=11, low=0x11), entry(), entry(cmd=1, low=2), entry(more=1), entry(more=1), entry()]
        chunk = b''.join(struct.pack('<II', *e) for e in entries) + b'\1\2\3'
        self.assertEqual(K.parse_list(chunk), entries)
        self.assertEqual(K.sound_groups(entries), [(0, 2), (2, 3), (3, 4), (4, 7)])
        self.assertEqual(K.sound_groups([entry(more=1)]), [(0, 1)])       # a list that ends inside a run
        self.assertEqual(K.sound_groups([]), [])

    def test_hash_and_header_length(self):
        self.assertEqual(K.fnv1a(b''), 0x811c9dc5)
        self.assertEqual(K.fnv1a(b'a'), 0xe40c292c)
        self.assertEqual(K.fnv1a(b'foobar'), 0xbf9cf968)
        for programs, size in ((1, 3104), (2, 3616), (3, 4128)):
            vh = bytearray(8000)
            vh[0:4] = b'pBAV'
            struct.pack_into('<H', vh, 0x12, programs)
            self.assertEqual(K.vab_header_length(bytes(vh)), size)
        with self.assertRaises(ValueError):
            K.vab_header_length(b'nope' + bytes(100))

    def test_pitch(self):
        self.assertAlmostEqual(K.semitones(44100, 72, 72, 0), 0)
        self.assertAlmostEqual(K.semitones(44100, 60, 72, 0), -12)
        self.assertAlmostEqual(K.semitones(22050, 84, 84, 0), 12)       # a sample at half rate played at its center: one octave up
        self.assertAlmostEqual(K.semitones(44100, 60, 72, 64), -11.5)
        self.assertAlmostEqual(K.arcade_semitones(61 << 8, 72), -12)
        self.assertAlmostEqual(K.arcade_semitones((61 << 8) + 128, 60), 0.5)
        self.assertTrue(K.pitch_delta_ok(-12, -12, 72))
        self.assertTrue(K.pitch_delta_ok(0.4, -12 + 12.0, 72))
        self.assertFalse(K.pitch_delta_ok(-13, -12, 72))
        self.assertTrue(K.pitch_delta_ok(1, 0, 60))                      # the PS1's tool plays root-60 instruments one semitone up
        self.assertFalse(K.pitch_delta_ok(0, 0, 60))

    def test_tones(self):
        tones = [dict(program=0, tone=0, low=0, high=59, vag=1), dict(program=0, tone=1, low=60, high=127, vag=2),
                 dict(program=1, tone=0, low=0, high=127, vag=3)]
        self.assertEqual(K.tone_by_index(tones, 0, 1)['vag'], 2)
        self.assertIsNone(K.tone_by_index(tones, 2, 0))
        self.assertEqual(K.tone_by_key(tones, 0, 59)['vag'], 1)
        self.assertEqual(K.tone_by_key(tones, 0, 60)['vag'], 2)
        self.assertEqual(K.tone_by_key(tones, 1, 5)['vag'], 3)
        self.assertIsNone(K.tone_by_key(tones, 2, 5))

    def test_sep(self):
        # res 48, 500000 us a quarter: 24 ticks = 0.25 s. Program change to 3 on channel 0, a note, a running-status note,
        # a velocity-0 note (a note off), a tempo change to 1,000,000 and a note 24 ticks later.
        ev = [(0, bytes([0xc0, 3])), (0, bytes([0x90, 60, 100])), (24, bytes([61, 90])), (0, bytes([61, 0])),
              (0, bytes([0xff, 0x51, 3]) + (1000000).to_bytes(3, 'big')), (24, bytes([0x90, 62, 80]))]
        seqs = K.parse_sep(make_sep([(7, 48, 500000, ev), (8, 48, 500000, [(0, bytes([0x90, 70, 1]))])]))
        self.assertEqual([s['id'] for s in seqs], [7, 8])
        notes = K.sep_notes(seqs[0])
        self.assertEqual([(round(t, 3), p, k, v) for t, p, k, v in notes], [(0.0, 3, 60, 100), (0.25, 3, 61, 90), (0.75, 3, 62, 80)])
        self.assertEqual([k for _, _, k, _ in K.sep_notes(seqs[1])], [70])
        with self.assertRaises(ValueError):
            K.parse_sep(b'nope')

    def test_pan(self):
        self.assertEqual(K.pan_from_dx(-100), 9)
        self.assertEqual(K.pan_from_dx(0), 9)
        self.assertEqual(K.pan_from_dx(5), 9)
        self.assertEqual(K.pan_from_dx(6), 10)
        self.assertEqual(K.pan_from_dx(192), 16)                        # the screen's centre is pan 16, the arcade's centre
        self.assertEqual(K.pan_from_dx(0x17f), 23)
        self.assertEqual(K.pan_from_dx(5000), 23)
        self.assertEqual([K.pan_from_dx(e) for e in K.PAN_EDGES], list(range(9, 24)))
        self.assertEqual([K.pan_from_dx(e - 1) for e in K.PAN_EDGES[1:]], list(range(9, 23)))

    def test_timeline_fit(self):
        arc = [(0.0, 5, -12.0, 72), (0.5, 5, -12.0, 72), (1.0, 5, -12.0, 72)]
        ps = [(0.0, {5}, -12.0), (0.5, {5, 6}, -12.0), (1.0, {5}, -12.0)]
        self.assertTrue(K.match_timeline(ps, arc))
        self.assertFalse(K.match_timeline(ps[:2], arc))                  # exact: the arcade has an event left over
        self.assertTrue(K.match_timeline(ps[:2], arc, 'prefix'))
        self.assertFalse(K.match_timeline(ps[1:], arc, 'prefix'))        # the later two are not the first two
        self.assertTrue(K.match_timeline(ps[1:], arc, 'subset'))
        late = [(0.03, {5}, -12.0)]
        self.assertTrue(K.match_timeline(late, arc[:1]))                 # 30 ms apart is within the tolerance
        self.assertFalse(K.match_timeline([(0.2, {5}, -12.0)], arc[:1]))
        off = [(0.0, {5}, -14.0)]
        self.assertFalse(K.match_timeline(off, arc[:1]))
        self.assertTrue(K.match_timeline(off, arc[:1], 'instrument'))    # the same instrument at another pitch
        self.assertFalse(K.match_timeline([(0.0, {9}, -12.0)], arc[:1], 'instrument'))
        self.assertFalse(K.match_timeline([], arc))
        self.assertFalse(K.match_timeline(ps + ps, arc, 'subset'))       # more PS1 events than arcade events
        twice = [(0.0, {5}, -12.0), (0.0, {5}, -12.0)]
        self.assertTrue(K.match_timeline(twice, [(0.0, 5, -12.0, 72), (0.01, 5, -12.0, 72)]))   # each takes a different arcade event
        self.assertFalse(K.match_timeline(twice, [(0.0, 5, -12.0, 72)], 'subset'))

    def test_classes_and_sides(self):
        def ev(tick, inst, voice, volume=100):
            return dict(tick=tick, instrument=inst, key=61, pitch=61 << 8, length=48, volume=volume, echo=0, voice=voice)
        effects = {0x100: dict(prio=16, events=[ev(3, 5, 12)], voices=[12]), 0x200: dict(prio=16, events=[ev(3, 5, 14)], voices=[14]),
                   0x101: dict(prio=16, events=[ev(3, 5, 13, 200)], voices=[13]), 0x201: dict(prio=16, events=[ev(3, 5, 15, 200)], voices=[15]),
                   0x300: dict(prio=16, events=[ev(3, 5, 9, 300)], voices=[9]), 0x301: dict(prio=16, events=[ev(3, 5, 9, 300)], voices=[9])}
        cls, members = K.effect_classes(effects)
        self.assertEqual(cls[0x200], 0x100)                              # same events, other voices: one class
        self.assertEqual(members[0x100], [0x100, 0x200])
        self.assertNotEqual(cls[0x101], cls[0x100])                      # another volume: another class
        voices = {c: e['voices'] for c, e in effects.items()}
        self.assertEqual(K.side_codes(members[0x100], voices), (0x100, 0x200))
        self.assertEqual(K.side_codes(members[0x101], voices), (0x101, 0x201))
        self.assertEqual(K.side_codes([0x300, 0x301], voices), (0x300, 0x300))   # no twin on other voices: the same code for both sides
        self.assertEqual(K.side_codes([0x300], voices), (0x300, 0x300))

    def test_contain_slack(self):
        recs = [dict(index=0, start=0x1000, end=0x1fff, root=72), dict(index=1, start=0x2000, end=0x2fff, root=72)]
        self.assertEqual(K.contain(recs, 0x1500), [0])
        self.assertEqual(K.contain(recs, 0x1fff), [0])
        self.assertEqual(K.contain(recs, 0x2000 - 2), [0])                  # inside record 0: never takes record 1
        self.assertEqual(K.contain(recs, 0x2000 + 0x10), [1])
        recs2 = [dict(index=0, start=0x1000, end=0x17ff, root=72), dict(index=1, start=0x2000, end=0x2fff, root=72)]
        self.assertEqual(K.contain(recs2, 0x2000 - 2), [1])               # in no record, 2 bytes before record 1
        self.assertEqual(K.contain(recs2, 0x2000 - K.RECORD_SLACK), [1])
        self.assertEqual(K.contain(recs2, 0x2000 - K.RECORD_SLACK - 1), [])
        self.assertEqual(K.contain(recs2, 0x2000 + 0x3000), [])

    def test_choice(self):
        self.assertEqual(K.nearest_choice([0x155, 0x126], [0x11a, 0x11d]), 0x126)
        self.assertEqual(K.nearest_choice([0x155, 0x126], []), 0x126)
        self.assertEqual(K.nearest_choice([0x10, 0x30], [0x20]), 0x10)           # a tie goes to the lower

    def test_callsites(self):
        with tempfile.TemporaryDirectory() as d:
            (Path(d) / 'a.c').write_text(
                'void f(void) {\n play_sound_at(o, o->side, 0x31a);\n play_sound_at(0, 0, 0x205);\n play_side_sound_at(o, o->side, 12);\n'
                ' play_sound_at(o, a, table[i]);\n call_func_with_side_x_origin(0, 0x2);\n}\n')
            consts, variable = K.callsite_requests(d)
        self.assertEqual(consts[('play_sound_at', 0x31a)], 1)
        self.assertEqual(consts[('play_sound_at', 0x205)], 1)
        self.assertEqual(consts[('play_side_sound_at', 12)], 1)
        self.assertEqual(consts[('call_func_with_side_x_origin', 2)], 1)
        self.assertEqual(variable['play_sound_at'], 1)


class Resolution(unittest.TestCase):
    """A made-up bank: sample 1 (root 72, natural rate 44100) plays instrument 0, sample 2 plays instrument 1, sample 3 has no source."""

    def setUp(self):
        self.vags = [hashlib.md5(bytes([i])).hexdigest() for i in (1, 2, 3, 4)]
        tones = [dict(program=0, tone=i, center=72, shift=0, low=0, high=127, vag=i + 1) for i in range(4)]
        sep = K.parse_sep(make_sep([
            (0, 48, 500000, [(0, bytes([0x90, 60, 100])), (24, bytes([60, 90])), (24, bytes([60, 80]))]),   # 3 notes, 0.25 s apart
        ]))
        entries = [
            entry(tone=0, more=1), entry(cmd=11, low=0x11),                  # 0: a note, then a stop in the same run -> exact
            entry(tone=1),                                                   # 2: sample 2: two effects fit -> block
            entry(tone=2),                                                   # 3: no source
            entry(tone=3),                                                   # 4: source, but no effect plays it
            entry(cmd=11, low=0x10),                                         # 5: a stop only
            entry(cmd=1, low=0),                                             # 6: a sequence of three notes of sample 1
        ]
        self.ctx = dict(names=[('PL99', 4)], slot=4, role='fighter', vh=b'pBAV' + bytes(100), info=dict(tones=tones, sizes=[]), vags=self.vags,
                        entries=entries, sep=sep)
        self.matches = {self.vags[0]: dict(rate=44100, address=0x1100, cover=100, exact=True),
                        self.vags[1]: dict(rate=44100, address=0x2100, cover=40, exact=True),
                        self.vags[2]: dict(rate=None),
                        self.vags[3]: dict(rate=44100, address=0x4100, cover=100, exact=True)}
        self.instruments = [dict(index=0, start=0x1000, end=0x1fff, root=72), dict(index=1, start=0x2000, end=0x2fff, root=72),
                            dict(index=2, start=0x4000, end=0x4fff, root=72)]

        def ev(tick, inst, voice, volume=0x1000):
            return dict(tick=tick, instrument=inst, key=61, pitch=61 << 8, length=48, volume=volume, echo=0, voice=voice)
        self.effects = {
            0x180: dict(prio=16, events=[ev(3, 0, 12)], voices=[12]), 0x280: dict(prio=16, events=[ev(3, 0, 14)], voices=[14]),
            0x190: dict(prio=16, events=[ev(3, 1, 12)], voices=[12]), 0x290: dict(prio=16, events=[ev(3, 1, 14)], voices=[14]),
            0x310: dict(prio=16, events=[ev(3, 1, 12, 0x800)], voices=[12]), 0x410: dict(prio=16, events=[ev(3, 1, 14, 0x800)], voices=[14]),
            # three notes of instrument 0 0.5 s apart ... and a fourth: the PS1 sequence has three notes at 0 s, 0.25 s, 0.5 s
            0x1a0: dict(prio=16, events=[ev(3, 0, 12), ev(3 + 62, 0, 12), ev(3 + 125, 0, 12), ev(3 + 187, 0, 12)], voices=[12]),
        }

    def rows(self):
        return K.resolve_rows([self.ctx], self.matches, self.instruments, self.effects)

    def test_rows(self):
        rows = {r['start']: r for r in self.rows()}
        self.assertEqual(sorted(rows), [0, 2, 3, 4, 5, 6])
        r = rows[0]
        self.assertEqual((r['kind'], r['method'], r['code0'], r['code1']), ('note', 'exact', 0x180, 0x280))
        r = rows[2]                                                         # two classes fit (0x190 pair and 0x310 pair); no certain sound
        self.assertEqual(r['method'], 'block')
        self.assertTrue(r['weak'])                                          # the sample match covers 40 percent
        self.assertEqual(r['code0'], 0x190)
        self.assertEqual(r['alt'], [0x310])
        r = rows[3]
        self.assertEqual((r['method'], r['reason'], r['code0']), ('none', 'no-source', None))
        r = rows[4]
        self.assertEqual((r['method'], r['reason']), ('none', 'no-effect'))
        r = rows[5]
        self.assertEqual((r['kind'], r['reason']), ('stop', 'stop'))
        r = rows[6]                                                         # three SEP notes against the arcade's four
        self.assertEqual((r['kind'], r['method'], r['code0']), ('sequence', 'prefix', 0x1a0))

    def test_run_rule(self):
        """Two certain sounds a and b with codes 0x180 and 0x183 and two sounds without a source between them: they take 0x181 and 0x182."""
        tones = [dict(program=0, tone=i, center=72, shift=0, low=0, high=127, vag=i + 1) for i in range(4)]
        vags = [hashlib.md5(bytes([i + 20])).hexdigest() for i in range(4)]
        entries = [entry(tone=0), entry(tone=1), entry(tone=1, note=62), entry(tone=2)]
        ctx = dict(names=[('PL98', 4)], slot=4, role='fighter', vh=b'pBAV' + bytes(50), info=dict(tones=tones, sizes=[]), vags=vags,
                   entries=entries, sep=[])
        matches = {vags[0]: dict(rate=44100, address=0x1100, cover=100, exact=True), vags[1]: dict(rate=None),
                   vags[2]: dict(rate=44100, address=0x2100, cover=100, exact=True), vags[3]: dict(rate=None)}
        instruments = [dict(index=i, start=0x1000 * (i + 1), end=0x1000 * (i + 1) + 0xfff, root=72) for i in range(6)]

        def ev(tick, inst, voice, volume):
            return dict(tick=tick, instrument=inst, key=61, pitch=61 << 8, length=48, volume=volume, echo=0, voice=voice)
        effects = {0x180: dict(prio=16, events=[ev(3, 0, 12, 100)], voices=[12]), 0x280: dict(prio=16, events=[ev(3, 0, 14, 100)], voices=[14]),
                   0x181: dict(prio=16, events=[ev(3, 5, 12, 100)], voices=[12]), 0x281: dict(prio=16, events=[ev(3, 5, 14, 100)], voices=[14]),
                   0x182: dict(prio=16, events=[ev(3, 5, 12, 200)], voices=[12]), 0x282: dict(prio=16, events=[ev(3, 5, 14, 200)], voices=[14]),
                   0x183: dict(prio=16, events=[ev(3, 1, 12, 100)], voices=[12]), 0x283: dict(prio=16, events=[ev(3, 1, 14, 100)], voices=[14])}
        rows = {r['start']: r for r in K.resolve_rows([ctx], matches, instruments, effects)}
        self.assertEqual([(rows[i]['method'], rows[i]['code0'], rows[i]['code1']) for i in range(4)],
                         [('exact', 0x180, 0x280), ('inferred', 0x181, 0x281), ('inferred', 0x182, 0x282), ('exact', 0x183, 0x283)])
        effects2 = {c: e for c, e in effects.items() if c not in (0x182, 0x282)}          # no effect 0x182: nothing is guessed
        rows = {r['start']: r for r in K.resolve_rows([ctx], matches, instruments, effects2)}
        self.assertEqual((rows[1]['method'], rows[2]['method']), ('none', 'none'))

    def test_decisions(self):
        def row(name, start, code0=None, reason=None, method='none'):
            return dict(contexts=[(name, 4, start)], code0=code0, code1=code0, reason=reason, method=method, key=(1, start), weak=False)
        rows = [row('SELECT', 0, reason='no-source'), row('SELECT', 1, reason='no-source'), row('SELECT', 2, reason='no-source'),
                row('PL99', 5, reason='stop'), row('PL99', 6, reason='no-source'), row('PL99', 7, reason='no-effect'),
                row('PL99', 8, reason='no-record'), row('SELECT', 3, 0x190, method='exact')]
        hand = (('SELECT', 4, (0,), 0x82, 0x83, 'cursor'), ('SELECT', 4, (2,), K.SILENT, K.SILENT, 'voice'), ('SELECT', 4, (3,), 0x82, 0x83, 'ignored'),
                ('GONE', 4, (0,), 0x82, 0x83, 'no such row'))
        K.apply_decisions(rows, hand)
        r = rows
        self.assertEqual((r[0]['code0'], r[0]['code1'], r[0]['method'], r[0]['reason']), (0x82, 0x83, 'hand', None))
        self.assertEqual((r[1]['code0'], r[1]['silent']), (None, 'no-source'))                 # no hand row: silent by the default
        self.assertEqual((r[2]['code0'], r[2]['silent']), (None, 'hand'))
        self.assertEqual(r[3]['silent'], 'stop')
        self.assertEqual(r[4]['silent'], 'no-source')
        self.assertEqual((r[5]['silent'], r[6]['silent']), ('no-effect', 'no-record'))          # nothing falls back to the PS1's sound
        self.assertEqual((r[7]['code0'], r[7]['method']), (0x190, 'exact'))                    # a measured row is left alone
        effects = {}
        text = K.c_table(rows, effects)
        self.assertEqual(text.count('0xfffe, 0xfffe, 8,'), 1)
        self.assertEqual(text.count('0xfffe, 0xfffe, 9,'), 2)
        self.assertEqual(text.count('0xfffe, 0xfffe, 12,'), 1)
        self.assertEqual(text.count('0x082, 0x083, 7,'), 1)
        self.assertEqual(text.count('0xfffe, 0xfffe, 11,'), 1)
        self.assertEqual(text.count('0xfffe, 0xfffe, 10,'), 1)
        self.assertNotIn('0xffff, 0xffff', text)
        over = (('SELECT', 4, (3,), 0x82, 0x83, 'replaces the measured row'),)
        K.apply_decisions(rows, hand, over)
        self.assertEqual((rows[7]['code0'], rows[7]['code1'], rows[7]['method']), (0x82, 0x83, 'hand'))

    def test_hand_table(self):
        seen = set()
        for archive, slot, entries, code0, code1, evidence in K.HAND:
            self.assertTrue(entries and slot in K.LIST_SLOTS)
            self.assertTrue(evidence and chr(0x2014) not in evidence)
            for e in entries:
                self.assertNotIn((archive, slot, e), seen)               # one decision for a sound
                seen.add((archive, slot, e))
            self.assertEqual(code0 == K.SILENT, code1 == K.SILENT)
            if code0 != K.SILENT:
                self.assertTrue(0 < code0 < 0x400 and 0 < code1 < 0x400)

    def test_override_table(self):
        places = set()
        for archive, slot, entries, code0, code1, evidence in K.HAND_OVERRIDE:
            self.assertTrue(entries and slot in K.LIST_SLOTS and evidence and chr(0x2014) not in evidence)
            self.assertTrue(0 < code0 < 0x400 and 0 < code1 < 0x400)
            for e in entries:
                places.add((archive, slot, e))
        self.assertEqual(len(places), sum(len(o[2]) for o in K.HAND_OVERRIDE))
        self.assertFalse(places & {(a, sl, e) for a, sl, es, *_ in K.HAND for e in es})   # a place is decided once

    def test_keys_are_unique_and_stable(self):
        rows = self.rows()
        keys = [r['key'] for r in rows]
        self.assertEqual(len(set(keys)), len(keys))
        self.assertEqual([r['key'] for r in self.rows()], keys)
        # a second archive with the same bytes joins the same row
        self.ctx2 = dict(self.ctx, names=[('PL99X', 5)])
        again = K.resolve_rows([self.ctx, self.ctx2], self.matches, self.instruments, self.effects)
        self.assertEqual(len(again), len(rows))
        self.assertEqual(sorted(n for n, _, _ in again[0]['contexts']), ['PL99', 'PL99X'])
        self.assertFalse(any(r['conflict'] for r in again))

    def test_table(self):
        text = K.c_table(self.rows(), self.effects)
        self.assertIn('const struct arcade_sound arcade_sounds[] = {', text)
        self.assertIn('arcade_sound_count = 6;', text)
        self.assertEqual(text.count('0x180, 0x280, 0,'), 1)
        self.assertEqual(text.count('0xffff, 0xffff, 9,'), 1)                 # no source
        self.assertEqual(text.count('0xffff, 0xffff, 11,'), 1)                # no effect
        self.assertEqual(text.count('0xffff, 0xffff, 8,'), 1)                 # stop
        lines = [ln for ln in text.splitlines() if ln.startswith('    {0x')]
        hashes = [ln.split(',')[0:2] for ln in lines]
        self.assertEqual(hashes, sorted(hashes))                            # sorted by key for a binary search
        self.assertNotIn(chr(0x2014), text)


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
