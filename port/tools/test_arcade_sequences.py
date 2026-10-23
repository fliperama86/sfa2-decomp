#!/usr/bin/env python3
"""Seconds: the animation step tool's pure functions on made-up data; no disc, no arcade set.

Checks port/arcade/sequences.py: the frame record's byte swap, the 12-byte step, the step run finder, the arcade
record walk (jumps, a jump with bit 15 clear, the walk back to the first record), the pairing of records with steps
(exact keys, list offsets alone, frame bytes, order), the frame numbering, the frame table finder, the table finders
of both sides, the flag scale and the range text. One small made-up world (an arcade program with art records,
sequence records and tables, and a PS1 module built from them by the rules) then goes through the whole analysis and
must come out exact.
"""
from pathlib import Path
import struct, sys, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import fighters as F
import sequences as Q

BASE = 0x120000      # the arcade address of the first sequence record in the made-up world
ART = 0x101000       # the arcade address of the first art record


def put_record(data, a, dur, flags, art, frame, target=None):
    """An arcade record (24 bytes, or 28 with a jump target) at a; returns the next address."""
    struct.pack_into('>HHI', data, a, dur, flags, art)
    data[a + 8:a + 24] = frame
    if target is not None:
        struct.pack_into('>I', data, a + 24, target)
        return a + 28
    return a + 24


def frame_bytes(k):
    """Sixteen arcade frame bytes, distinct for k, with a nonzero first group."""
    return bytes([1, k, 0, 0, 5, 0, 0, k, 0, 0, 0, 0, 0, 0, 0, 0xff])


class Pure(unittest.TestCase):
    def test_convert_frame_reverses_each_group_of_four(self):
        fr = bytes(range(16))
        self.assertEqual(Q.convert_frame(fr), bytes([3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12]))
        self.assertEqual(Q.convert_frame(Q.convert_frame(fr)), fr)

    def test_step_bytes(self):
        b = Q.pack_step(5, 0x8000, 14, -5, 211)
        self.assertEqual(len(b), 12)
        self.assertEqual(b, struct.pack('<hhIhH', 5, -32768, 14, -5, 211))
        mod = b'\0' * 8 + b
        p = Q.step_at(mod, 8)
        self.assertEqual((p['dur'], p['fl'], p['dp'], p['lo'], p['fi'], p['odd']), (5, 0x8000, 14, -5, 211, 0))
        self.assertEqual(Q.step_at(Q.pack_step(1, 0, 15, 0, 0), 0)['dp'], 14)       # the low bit of the pointer is masked
        self.assertEqual(Q.step_at(Q.pack_step(1, 0, 15, 0, 0), 0)['odd'], 1)

    def test_find_steps(self):
        steps = b''.join(Q.pack_step(3 + k % 4, 0x8000 if k % 10 == 9 else 0, 2 * (k % 7), -3 if k % 10 == 9 else 0, k % 5) for k in range(60))
        pointers = struct.pack('<5I', *(0x801b0000 + 0x40 + 12 * k for k in range(5)))   # a pointer table must not read as steps
        mod = b'\x11' * 0x40 + steps + pointers + b'\0' * 64
        s, found = Q.find_steps(mod, 0x3e, 100)
        self.assertEqual((s, len(found)), (0x40, 60))
        self.assertEqual(found[9]['fl'], 0x8000)
        self.assertEqual(Q.find_steps(mod[:0x40 + 12 * 20], 0x40, 100), (None, []))   # too short a run
        self.assertEqual(Q.find_steps(mod, 0x3e, 4)[1], [])                          # every pointer lies outside the lists

    def make_prog(self):
        data = bytearray(0x180000)
        a = BASE
        recs = []
        for k, (dur, fl) in enumerate([(5, 0), (5, 0), (6, 0), (4095, 0x8000), (3, 0)]):
            recs.append(a)
            tgt = BASE + 24 if fl & 0x8000 else None
            a = put_record(data, a, dur, fl, ART + 16 * (k % 3), frame_bytes(k % 4), tgt)
        return data, recs, a

    def test_walk_records(self):
        data, recs, end = self.make_prog()
        out = Q.walk_records(bytes(data), BASE)
        self.assertEqual([r['a'] for r in out], recs)
        self.assertEqual(out[3]['tgt'], BASE + 24)
        self.assertEqual(out[0]['tgt'], None)
        self.assertEqual(out[4]['fr'], frame_bytes(0))
        self.assertTrue(Q.valid_record(bytes(data), BASE))
        self.assertFalse(Q.valid_record(bytes(data), BASE + 1))
        self.assertFalse(Q.valid_record(bytes(data), end))               # zeros: no countdown
        self.assertEqual(len(Q.walk_records(bytes(data), BASE, limit=2)), 2)

    def test_a_jump_without_bit_15_is_taken_as_a_jump(self):
        data = bytearray(0x180000)
        a = put_record(data, BASE, 4, 0, ART, frame_bytes(1))
        a = put_record(data, a, 4095, 0, ART + 16, frame_bytes(2), target=a)   # bit 15 clear, 4 bytes follow
        a = put_record(data, a, 7, 0, ART + 32, frame_bytes(3))
        out = Q.walk_records(bytes(data), BASE)
        self.assertEqual(len(out), 3)
        self.assertTrue(out[1]['quirk'])
        self.assertEqual(out[1]['fl'] & 0x8000, 0x8000)
        self.assertFalse(out[0]['quirk'])

    def test_extend_back_finds_the_first_record(self):
        data, recs, _end = self.make_prog()
        self.assertEqual(Q.extend_back(bytes(data), recs[3]), BASE)
        self.assertEqual(Q.extend_back(bytes(data), recs[4]), BASE)
        self.assertEqual(Q.extend_back(bytes(data), BASE), BASE)

    def test_find_run_by_the_first_known_step(self):
        data, recs, _end = self.make_prog()
        steps = [dict(dur=6, fl=0, dp=40), dict(dur=4095, fl=0x8000, dp=50)]
        inverse = {40: ART + 32, 50: ART + 16}
        self.assertEqual(Q.find_run(bytes(data), steps, inverse), recs[2])
        self.assertIsNone(Q.find_run(bytes(data), steps, {99: ART}))

    def arcade(self, specs):
        """Records [(dur, flags, art)] as the pairing sees them (frame bytes from the art's low bits)."""
        return [dict(a=BASE + 24 * k, dur=d, fl=f, art=art, tgt=None, fr=frame_bytes(art & 15), quirk=False) for k, (d, f, art) in enumerate(specs)]

    def test_pair_exact_keys_then_list_offsets_then_order(self):
        amap = {ART + 16 * k: 100 + 20 * k for k in range(8)}
        arc = self.arcade([(5, 0, ART), (5, 0, ART + 16), (6, 0, ART + 32), (7, 0, ART + 48), (8, 0, ART + 64), (9, 0, ART + 80)])
        steps = [dict(dur=5, fl=0, dp=100, fi=1), dict(dur=5, fl=0, dp=120, fi=2),
                 dict(dur=9, fl=0, dp=140, fi=3),       # same list as record 2, the PS1 changed the countdown
                 dict(dur=7, fl=0, dp=999, fi=4),       # a list the arcade record does not have: not paired with record 3
                 dict(dur=8, fl=0, dp=180, fi=5), dict(dur=9, fl=0, dp=200, fi=6)]
        self.assertEqual(Q.pair_records(arc, steps, amap), [(0, 0), (1, 1), (2, 2), (4, 4), (5, 5)])

    def test_pair_leaves_the_steps_the_arcade_lacks(self):
        amap = {ART + 16 * k: 100 + 20 * k for k in range(4)}
        arc = self.arcade([(5, 0, ART), (5, 0, ART + 16), (6, 0, ART + 32), (7, 0, ART + 48)])
        steps = [dict(dur=5, fl=0, dp=100, fi=1), dict(dur=2, fl=0, dp=700, fi=2), dict(dur=2, fl=0, dp=714, fi=2), dict(dur=7, fl=0, dp=160, fi=4)]
        self.assertEqual(Q.pair_records(arc, steps, amap), [(0, 0), (3, 3)])

    def test_pair_by_frame_bytes_when_the_art_is_unknown(self):
        amap = {}
        arc = self.arcade([(5, 0, ART + 1), (6, 0, ART + 2), (7, 0, ART + 3)])
        frames = {k: Q.convert_frame(frame_bytes(k + 1)) for k in range(3)}
        steps = [dict(dur=6, fl=0, dp=10 * k, fi=k) for k in range(3)]        # countdowns and flags changed, lists unknown
        steps[0]['dur'], steps[2]['dur'] = 5, 7
        steps[1]['fl'] = 1
        pairs = Q.pair_records(arc, steps, amap, lambda j: frames[j])
        self.assertEqual(pairs, [(0, 0), (1, 1), (2, 2)])

    def test_frame_numbers(self):
        z, a, b = bytes(16), b'a' * 16, b'b' * 16
        self.assertEqual(Q.frame_numbers([a, a, z, b, a, b], [1, 1, 0, 2, 1, 2]), [1, 1, 0, 2, 1, 2])
        # a difference in the PS1's number is followed, it does not carry on
        self.assertEqual(Q.frame_numbers([a, b, a, b], [1, 5, 1, 5]), [1, 2, 1, 5])
        # steps of the PS1's own do not move the count
        self.assertEqual(Q.frame_numbers([a, b, a], [1, 40, 1], own={1}), [1, None, 1])
        self.assertEqual(Q.frame_numbers([a, None], [1, 2]), [1, None])

    def test_find_frame_base(self):
        frames = [bytes([i + 1]) * 16 for i in range(6)]
        mod = b'\xee' * 32 + bytes(16) + b''.join(frames)       # frame 0 (zeros) at 32, frame k at 32 + 16 k
        base, votes = Q.find_frame_base(mod, {k + 1: frames[k] for k in range(6)})
        self.assertEqual((base, votes), (32, 6))
        self.assertIsNone(Q.find_frame_base(mod, {1: b'\x77' * 16}))

    def test_scale_flags(self):
        self.assertEqual(Q.scale_flags(0x48), 3)
        self.assertEqual(Q.scale_flags(0x8130), 0x8102)
        self.assertEqual(Q.scale_flags(0x8118), 0x8101)
        self.assertIsNone(Q.scale_flags(0x8000))
        self.assertIsNone(Q.scale_flags(0x49))

    def test_ranges(self):
        self.assertEqual(Q.ranges([1, 2, 3, 7, 9, 10]), ['1-3', '7', '9-10'])
        self.assertEqual(Q.ranges([]), [])

    def test_arcade_tables_and_ps1_tables(self):
        data, recs, end = self.make_prog()
        t1, t2 = BASE - 16, BASE - 6          # a table of 4 entries and a zero, then a table of 2 and a zero
        for k, r in enumerate([recs[0], recs[2], recs[3], recs[1]]):
            struct.pack_into('>h', data, t1 + 2 * k, r - t1)
        for k, r in enumerate([recs[4], recs[0]]):
            struct.pack_into('>h', data, t2 + 2 * k, r - t2)
        tabs = Q.find_tables(bytes(data), set(recs), BASE, end)
        self.assertEqual([(q, e) for q, e in tabs], [(t1, [recs[0], recs[2], recs[3], recs[1]]), (t2, [recs[4], recs[0]])])
        # tables after the last record
        tafter = end
        struct.pack_into('>h', data, tafter, recs[1] - tafter)
        tabs = Q.find_tables(bytes(data), set(recs), BASE, end)
        self.assertEqual(tabs[-1], (tafter, [recs[1]]))
        # the PS1 side: pointers to steps right after the n steps
        mod = b'\0' * 4 + b''.join(Q.pack_step(1, 0, 0, 0, 0) for _ in range(5)) + struct.pack('<4I', *(0x801b0004 + 12 * k for k in (2, 0, 4, 1))) + struct.pack('<I', 0x12345678)
        self.assertEqual(Q.ps1_table_entries(mod, 4, 5, 0x801b0000), [2, 0, 4, 1])


class World(unittest.TestCase):
    """A made-up arcade program and a PS1 module built from it by the rules go through the whole analysis."""

    def build(self):
        data = bytearray(0x180000)
        # the coordinates of the art records: one block each at the pointer found through the table
        arts, lists = [], []
        for k in range(3):
            a = ART + 12 * k
            struct.pack_into('>HHHH', data, a, 2, 1, 0, 0)               # mode 2, one cell, one block, coordinate index 0
            struct.pack_into('>HH', data, a + 8, 0x100 + k, 0)           # tile code and attribute
            arts.append(a)
        struct.pack_into('>I', data, F.COORD_TABLE, 0x2000)
        struct.pack_into('>hh', data, 0x2000, -8, 0)
        a = BASE
        specs = [(5, 0, 0, 1), (5, 0, 1, 2), (6, 0x48, 2, 1), (4095, 0x8000, 1, 2), (3, 0, 0, 3)]
        recs = []
        for dur, fl, art, fk in specs:
            recs.append(a)
            a = put_record(data, a, dur, fl, arts[art], frame_bytes(fk), BASE + 24 if fl & 0x8000 else None)
        end = a
        # a table of the first, third and fourth records, right before the first record
        t = BASE - 8
        for k, r in enumerate([recs[0], recs[2], recs[3]]):
            struct.pack_into('>h', data, t + 2 * k, r - t)
        # the PS1 module: lists (32 bytes each) at 0x100, steps after them, pointers, then the frame table
        lbase, lsize = 0x100, 0x60
        offs = {arts[k]: 32 * k for k in range(3)}
        mod = bytearray(lbase + lsize)
        s0 = lbase + lsize
        frame_index = {}
        for dur, fl, art, fk in specs:
            frame_index.setdefault(fk, len(frame_index) + 1)
        steps = bytearray()
        for k, (dur, fl, art, fk) in enumerate(specs):
            lo = (1 - k) if fl & 0x8000 else 0
            low = fl & 0xff
            steps += Q.pack_step(dur, (fl & 0x8100) | low // 0x18 if low else fl, offs[arts[art]], lo, frame_index[fk])
        mod += steps
        mod += struct.pack('<3I', *(0x801b0000 + s0 + 12 * k for k in (0, 2, 3)))
        mod += b'\0' * ((-len(mod)) % 16)
        fbase = len(mod)
        mod += bytes(16) + b''.join(Q.convert_frame(frame_bytes(fk)) for fk, _ in sorted(frame_index.items(), key=lambda x: x[1]))
        mod += b'\0' * 32
        lists = [dict(n=1, mode=0, cells=[(1, 0)], at=lbase + 32 * k) for k in range(3)]
        r = dict(lists=lists, lbase=lbase, listsend=lbase + lsize, segments=[(arts[0], 3, 0, 0)])
        return bytes(data), bytes(mod), r, s0, fbase, recs

    def test_whole_analysis(self):
        prog, mod, r, s0, fbase, recs = self.build()
        view = F.View(prog)
        out = Q.analyse(mod, r, prog, view, min_run=3)
        self.assertNotIn('error', out)
        self.assertEqual((out['start'], out['a0'], len(out['steps']), len(out['arc'])), (s0, BASE, 5, 5))
        self.assertEqual(out['pairs'], [(k, k) for k in range(5)])
        self.assertEqual((out['exact'], out['scaled']), (4, 1))        # the record with flags 0x48 is the scaled one
        self.assertEqual(out['base'], fbase)
        self.assertEqual((out['nframes'], out['fr_exact'], out['fr_diff'], out['fr_own']), (4, 3, 0, 0))
        self.assertEqual(out['tables'], [3])
        self.assertEqual((out['table_arcade'], out['table_ps1'], out['table_match']), (3, 3, 3))
        self.assertEqual((out['only_arc'], out['only_ps1']), ([], []))
        text = Q.line('PL99.PAC', out)
        self.assertIn('exact 4 (48 B) + 1 with scaled flags', text)
        self.assertNotIn('differ:', text)

    def test_a_changed_step_is_told_apart(self):
        prog, mod, r, s0, fbase, recs = self.build()
        bad = bytearray(mod)
        struct.pack_into('<h', bad, s0 + 12 * 1, 9)             # the PS1 changed a countdown
        struct.pack_into('<h', bad, s0 + 12 * 3 + 8, 0)         # and a loop
        out = Q.analyse(bytes(bad), r, prog, F.View(prog), min_run=3)
        self.assertEqual((out['exact'], out['scaled']), (2, 1))
        self.assertEqual((out['counts']['dur'], out['counts']['loop']), (1, 1))
        self.assertEqual(out['first']['dur'], (1, 1))
        self.assertIn('differ: ', Q.line('PL99.PAC', out))
        self.assertTrue(any('first loop difference' in d for d in Q.details('PL99.PAC', out)))

    def test_errors(self):
        prog, mod, r, s0, fbase, recs = self.build()
        self.assertEqual(Q.analyse(mod[:0x100], r, prog, F.View(prog), min_run=3)['error'], 'no step run after the lists')
        flat = dict(r, lists=[dict(n=1, mode=0, cells=[(1, 0)], at=0x500)])     # no art record has a list the module's steps use
        self.assertEqual(Q.analyse(mod, flat, prog, F.View(prog), min_run=3)['error'], 'no arcade record for the first steps')
        self.assertIn('no step run', Q.line('PL99.PAC', dict(error='no step run after the lists')))


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
