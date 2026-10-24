#!/usr/bin/env python3
"""Seconds: the runtime C of the animation steps (port/arcade/sequence.c) against the Python model; no game files.

Builds sequence.c, the generated tables and a driver with the address and undefined-behavior sanitizers, makes small
arcade programs (sequence records in several groups, jumps, a jump without bit 15, scaled flags, repeated frames, tables
and slices, frame overrides, lists known and unknown) and checks that the C gives, byte for byte and flag for flag, what
sequences.py's model_build gives, then random programs. It checks that a row that does not fit its program (a run past the
records, overlapping runs, a frame beyond the frame records, a table past the program, a slice outside the tables, a bad
record) is refused and never overflows, the list offsets of a made-up fighter (a record drawn twice keeps its last list),
that the generated table is sane, that the C compiles warning-free for 32 bits, and the helpers that write the table.
"""
from pathlib import Path
import random, shutil, struct, subprocess, sys, tempfile, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
sys.path.insert(0, str(ROOT / 'port/tools'))
import fighters as F
import sequences as Q
from test_arcade_fighter import Prog
from test_arcade_sequences import put_record, frame_bytes

FLAGS = ('-std=c99', '-O1', '-Wall', '-Wextra', '-Werror')
SOURCES = [ROOT / 'port/arcade' / n for n in ('sequence.c', 'sequences_table.c', 'fighter.c', 'fighters_table.c')]
G1, G2, ART = 0x120000, 0x121000, 0x101000
LOAD, STEPS = 0x801b0000, 0x400


def row_of(**kw):
    row = dict(file='TEST.PAC', load=LOAD, steps=STEPS, frames=0x40, step_count=8, frame_count=6, table_entries=4, rules=[],
               scale=(0, 0), groups=[(G1, 5)], runs=[(0, 0, 5)], own=[(5, 3)], tables=[], slices=[], frame_overrides=[])
    row.update(kw)
    return row


def spec_text(row, offsets):
    rules = 1 if Q.JUMP_WITHOUT_BIT15 in row['rules'] else 0
    out = [f'{row["load"]} {row["steps"]} {row["frames"]} {row["step_count"]} {row["frame_count"]} {row["table_entries"]} {rules} {row["scale"][0]} {row["scale"][1]}']
    for key in ('groups', 'runs', 'own', 'tables', 'slices', 'frame_overrides'):
        out.append(str(len(row[key])))
        out += [' '.join(map(str, x)) for x in row[key]]
    out.append(str(len(offsets)))
    out += [f'{a} {o}' for a, o in sorted(offsets.items())]
    return '\n'.join(out) + '\n'


class SequenceC(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.dir = tempfile.TemporaryDirectory(prefix='arcade-sequence-')
        d = Path(cls.dir.name)
        cls.exe = d / 'sequence_test'
        subprocess.run(['gcc', *FLAGS, '-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', *map(str, SOURCES),
                        str(ROOT / 'port/arcade/tests/sequence_test.c'), '-o', str(cls.exe)], check=True, timeout=180)

    @classmethod
    def tearDownClass(cls):
        cls.dir.cleanup()

    # ------------------------------------------------------------ helpers

    def run_c(self, prog, row, offsets):
        """(status, text, outputs) of the C for a made-up program, row and list offsets."""
        d = Path(self.dir.name) / 'case'
        shutil.rmtree(d, ignore_errors=True)
        d.mkdir()
        (d / 'prog.bin').write_bytes(bytes(prog))
        (d / 'spec.txt').write_text(spec_text(row, offsets))
        r = subprocess.run([str(self.exe), 'build', str(d / 'prog.bin'), str(d / 'spec.txt'), str(d)], capture_output=True, text=True, timeout=60)
        if r.returncode:
            return r.returncode, r.stdout + r.stderr, None
        got = {k: (d / f'{k}.bin').read_bytes() for k in ('steps', 'frames', 'table')}
        for k in ('steps', 'frames', 'table'):
            got[k + '_built'] = list((d / f'{k}_built.bin').read_bytes())
        return 0, r.stdout, got

    def same(self, prog, row, offsets):
        """The C equals the model, byte for byte and flag for flag."""
        m = Q.model_build(bytes(prog), offsets, row)
        status, text, got = self.run_c(prog, row, offsets)
        self.assertEqual(status, 0, text)
        for k in ('steps', 'frames', 'table'):
            self.assertEqual(got[k], m[k], k)
            self.assertEqual(got[k + '_built'], [int(b) for b in m[k + '_built']], k + ' built')
        self.assertNotIn('leak', text)
        return m

    def refused(self, prog, row, offsets, message):
        status, text, _ = self.run_c(prog, row, offsets)
        self.assertEqual(status, 1, text)
        self.assertIn(message, text)
        self.assertNotIn('leak', text)

    def world(self):
        """Two groups of records with a table in between and one before, a jump in each, shared frames."""
        data = bytearray(0x130000)
        arts = [ART + 2 * k for k in range(6)]
        a = G1
        g1 = []
        for k, (dur, fl, art, fk) in enumerate([(5, 0, 0, 1), (5, 0, 1, 2), (6, 0x48, 2, 1), (4095, 0x8000, 1, 2), (3, 0, 0, 3)]):
            g1.append(a)
            a = put_record(data, a, dur, fl, arts[art], frame_bytes(fk), g1[0] + 24 if fl & 0x8000 else None)
        a = G2
        g2 = []
        for k, (dur, fl, art, fk) in enumerate([(2, 0, 3, 0), (7, 0, 4, 4), (4095, 0x8000, 5, 1)]):
            g2.append(a)
            a = put_record(data, a, dur, fl, arts[art], bytes(16) if fk == 0 else frame_bytes(fk), g1[1] if fl & 0x8000 else None)
        recs = g1 + g2
        t1, t2 = G1 - 12, G1 + 0x200       # a table of three entries and a zero, right before group 1; one after it, of two
        for k, r in enumerate([recs[0], recs[2], recs[5]]):
            struct.pack_into('>h', data, t1 + 2 * k, r - t1)
        for k, r in enumerate([recs[7], recs[1]]):
            struct.pack_into('>h', data, t2 + 2 * k, r - t2)
        offsets = {arts[0]: 0, arts[1]: 30, arts[2]: 60, arts[3]: 90, arts[4]: 120}     # arts[5] has no list
        row = row_of(groups=[(G1, 5), (G2, 3)], step_count=10, frame_count=6, table_entries=6,
                     runs=[(0, 0, 5), (5, 6, 3)], own=[(5, 1)], tables=[(t1, 3), (t2, 2)], slices=[(0, 0, 3), (3, 4, 2)], scale=(0, 4))
        return data, offsets, row, recs

    # ------------------------------------------------------------ the rules

    def test_the_whole_world_equals_the_model(self):
        data, offsets, row, recs = self.world()
        m = self.same(data, row, offsets)
        self.assertEqual(m['steps_built'], [1, 1, 1, 1, 1, 0, 1, 1, 0, 0])             # the own step, and the record whose art has no list
        self.assertEqual(m['frames_built'], [1, 1, 1, 1, 1, 0])
        steps = m['steps']
        self.assertEqual(struct.unpack_from('<hhIhH', steps, 12 * 2), (6, 3, 60, 0, 1))      # flags 0x48 scaled to 3; frame 1 seen before
        self.assertEqual(struct.unpack_from('<hhIhH', steps, 12 * 3), (4095, -32768, 30, -2, 2))   # a jump back to record 1, two steps before
        self.assertEqual(struct.unpack_from('<hhIhH', steps, 12 * 6)[4], 0)                  # an all-zero frame is number 0
        self.assertEqual(struct.unpack_from('<I', m['table'], 0)[0], LOAD + STEPS)          # entry 0: record 0, step 0
        self.assertEqual(m['table_built'], [1, 1, 1, 0, 1, 1])                               # slot 3 is in no slice: the PS1's own

    def test_frame_overrides_and_a_second_use_of_a_frame(self):
        data, offsets, row, recs = self.world()
        row['frame_count'] = 8
        row['frame_overrides'] = [(1, 5)]                       # the PS1 numbered step 1 apart
        m = self.same(data, row, offsets)
        self.assertEqual(struct.unpack_from('<hhIhH', m['steps'], 12)[4], 5)
        self.assertEqual(m['frames_built'][5], 1)
        row['frame_overrides'] = [(1, 8)]
        self.refused(data, row, offsets, 'a frame override outside')

    def test_a_jump_without_bit_15_needs_the_rule(self):
        data = bytearray(0x130000)
        a = put_record(data, G1, 4, 0, ART, frame_bytes(1))
        a = put_record(data, a, 4095, 0, ART + 2, frame_bytes(2), target=G1)      # bit 15 clear, 4 bytes follow
        put_record(data, a, 7, 0, ART + 4, frame_bytes(3))
        offsets = {ART: 0, ART + 2: 20, ART + 4: 40}
        row = row_of(groups=[(G1, 3)], runs=[(0, 0, 3)], own=[(3, 5)], rules=[Q.JUMP_WITHOUT_BIT15])
        m = self.same(data, row, offsets)
        self.assertEqual(struct.unpack_from('<hhIhH', m['steps'], 12)[1:4], (-32768, 20, -1))
        self.assertEqual(m['steps_built'][:3], [1, 1, 1])
        row = row_of(groups=[(G1, 3)], runs=[(0, 0, 3)], own=[(3, 5)])
        self.refused(data, row, offsets, 'no sequence record')            # without the rule the third record starts 4 bytes early

    def test_unbuilt_steps_stay_zero(self):
        data, offsets, row, recs = self.world()
        del offsets[ART]                                         # records 0 and 4 lose their list
        m = self.same(data, row, offsets)
        self.assertEqual(m['steps_built'][0], 0)
        self.assertEqual(m['steps'][:12], bytes(12))
        # a jump whose target has no step is not built
        row = row_of(groups=[(G1, 5)], runs=[(2, 0, 3)], own=[(3, 5)], scale=(0, 0))
        data2, offsets2, _row, _ = self.world()
        m = self.same(data2, row, offsets2)
        self.assertEqual(m['steps_built'][:3], [1, 0, 1])           # record 3 jumps to record 1, which is in no run

    def test_the_scale_range(self):
        data, offsets, row, recs = self.world()
        row['scale'] = (0, 0)
        m = self.same(data, row, offsets)
        self.assertEqual(struct.unpack_from('<hhIhH', m['steps'], 24)[1], 0x48)
        row['scale'] = (2, 1)
        m = self.same(data, row, offsets)
        self.assertEqual(struct.unpack_from('<hhIhH', m['steps'], 24)[1], 3)

    # ------------------------------------------------------------ rows that do not fit

    def test_rows_that_do_not_fit_are_refused(self):
        data, offsets, row, recs = self.world()
        self.refused(data, dict(row, runs=[(0, 0, 9)]), offsets, 'a run outside')
        self.refused(data, dict(row, runs=[(0, 8, 5)]), offsets, 'a run outside')
        self.refused(data, dict(row, runs=[(0, 0, 3), (2, 3, 3)]), offsets, 'runs overlap')
        self.refused(data, dict(row, frame_count=3), offsets, 'beyond the 3 frame records')
        self.refused(data, dict(row, slices=[(0, 0, 9)]), offsets, 'a table slice outside')
        self.refused(data, dict(row, slices=[(0, 5, 3)]), offsets, 'a table slice outside')
        self.refused(data, dict(row, tables=[(len(data) - 2, 3)]), offsets, 'runs past the program')
        self.refused(data, dict(row, groups=[(G1 + 2, 5), (G2, 3)]), offsets, 'no sequence record')
        self.refused(data, dict(row, groups=[(len(data) - 10, 1)]), offsets, 'no sequence record')
        bad = bytearray(data)
        struct.pack_into('>H', bad, G1 + 24 * 2, 0)              # a zero countdown is not a record
        self.refused(bad, row, offsets, 'no sequence record')

    # ------------------------------------------------------------ random programs

    def random_world(self, rnd):
        data = bytearray(0x130000)
        n1, n2 = rnd.randrange(3, 10), rnd.randrange(0, 6)
        pool = [bytes(16)] + [frame_bytes(k) for k in range(1, 5)]
        recs, groups = [], []
        for start, n in ((G1, n1), (G2, n2)):
            if not n:
                continue
            groups.append((start, n))
            a = start
            for _ in range(n):
                recs.append(a)
                fl = rnd.choice((0, 0, 0, 0x8000, 0x48, 0x8130, 0x18, 0x1))
                tgt = None
                if fl & 0x8000:
                    tgt = rnd.choice(recs + [0x7777]) if rnd.random() < 0.9 else recs[0]
                a = put_record(data, a, rnd.randrange(1, 4096), fl, ART + 2 * rnd.randrange(6), rnd.choice(pool), tgt)
        total = len(recs)
        offsets = {ART + 2 * k: 10 * k for k in range(6) if rnd.random() < 0.8}
        steps_n = total + rnd.randrange(0, 4)
        order = sorted(rnd.sample(range(steps_n), total))
        runs = []
        for i, j in enumerate(order):
            if runs and runs[-1][0] + runs[-1][2] == i and runs[-1][1] + runs[-1][2] == j and rnd.random() < 0.9:
                runs[-1][2] += 1
            else:
                runs.append([i, j, 1])
        own = Q.spans([j for j in range(steps_n) if j not in order])
        tables, t = [], G1 - 0x100
        flat = []
        for _ in range(rnd.randrange(0, 3)):
            n = rnd.randrange(1, 5)
            ents = [rnd.choice(recs) for _ in range(n)]
            for k, r in enumerate(ents):
                struct.pack_into('>h', data, t + 2 * k, r - t)
            tables.append((t, n))
            flat += ents
            t += 2 * (n + 1)
        ne = rnd.randrange(len(flat), len(flat) + 3)
        slices, slot, e = [], 0, 0
        while e < len(flat) and slot < ne:
            n = rnd.randrange(1, len(flat) - e + 1)
            n = min(n, ne - slot)
            slices.append((e, slot, n))
            e += n
            slot += n + rnd.randrange(0, 2)
            slot = min(slot, ne)
        nf = rnd.randrange(3, 8)
        over = [(rnd.choice(order), rnd.randrange(nf)) for _ in range(rnd.randrange(0, 3))] if order else []
        over = list({s: n for s, n in over}.items())
        scale = (rnd.randrange(total), rnd.randrange(0, 4)) if rnd.random() < 0.4 else (0, 0)
        row = row_of(groups=groups, step_count=steps_n, frame_count=nf, table_entries=ne, runs=[tuple(x) for x in runs], own=own,
                     tables=tables, slices=slices, frame_overrides=over, scale=scale)
        return data, offsets, row

    def test_random_programs_equal_the_model(self):
        rnd = random.Random(7)
        done = refused = 0
        for case in range(120):
            data, offsets, row = self.random_world(rnd)
            with self.subTest(case=case):
                try:
                    Q.model_build(bytes(data), offsets, row)
                except (ValueError, struct.error):
                    status, text, _ = self.run_c(data, row, offsets)
                    self.assertEqual(status, 1, text)           # the C refuses what the model refuses
                    refused += 1
                    continue
                self.same(data, row, offsets)
                done += 1
        self.assertGreaterEqual(done, 60)
        self.assertGreaterEqual(refused, 1)

    # ------------------------------------------------------------ list offsets from a made-up fighter

    def test_list_offsets_of_a_made_up_fighter(self):
        p = Prog()
        a0 = p.record([(1, 0x0701, 0, 0)])
        a1 = p.record([(20, 0x0000, 0, 0), (21, 0x0000, 32, 0)])
        a2 = p.record([(30, 0x0000, 0, 0)])
        a3 = p.record([(5, 3, 7)], shared=0)                     # a record of mode 0
        for segs, rules in (([(a0, 3, 0), (0, 2, 0), (a3, 1, 0x20000)], 4), ([(a1, 1, 0), (a1, 1, 0), (a2, 1, 0)], 0), ([(0, 1, 0)], 4)):
            with self.subTest(segs=segs):
                lists = F.model_build(p.view(), segs, False, False, bool(rules & 4))[0]
                want = Q.model_offsets(lists, segs, p.view())
                d = Path(self.dir.name) / 'lists'
                shutil.rmtree(d, ignore_errors=True)
                d.mkdir()
                (d / 'prog.bin').write_bytes(bytes(p.data))
                (d / 'spec.txt').write_text(f'{rules} 65536 65536 {len(segs)}\n' + ''.join(f'{a} {c} {b}\n' for a, c, b in segs))
                r = subprocess.run([str(self.exe), 'lists', str(d / 'prog.bin'), str(d / 'spec.txt'), str(d)], capture_output=True, text=True, timeout=60)
                self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
                got = {int(a): int(b) for a, b in (ln.split() for ln in (d / 'lists.txt').read_text().split('\n') if ln)}
                self.assertEqual(got, want)
        lists = F.model_build(p.view(), [(a1, 1, 0), (a1, 1, 0)], False, False, False)[0]
        self.assertEqual(Q.model_offsets(lists, [(a1, 1, 0), (a1, 1, 0)], p.view()), {a1: len(lists) // 2})      # the later list is kept

    def test_list_offsets_with_pads(self):
        # the PS1's lists may lie padded (fighter.c's pads): the offsets are where the builder puts the lists, not where the lists end
        p = Prog()
        a0 = p.record([(1, 0x0701, 0, 0)])
        a1 = p.record([(20, 0x0000, 0, 0), (21, 0x0000, 32, 0)])
        a2 = p.record([(30, 0x0000, 0, 0)])
        segs = [(a0, 1, 0), (a1, 1, 0), (a2, 1, 0)]
        lists = F.model_build(p.view(), segs)[0]
        base = Q.model_offsets(lists, segs, p.view())
        d = Path(self.dir.name) / 'lists'
        shutil.rmtree(d, ignore_errors=True)
        d.mkdir()
        (d / 'prog.bin').write_bytes(bytes(p.data))
        (d / 'spec.txt').write_text(f'0 65536 65536 {len(segs)}\n' + ''.join(f'{a} {c} {b}\n' for a, c, b in segs) + 'P 2\n0 6\n1 4\n')
        r = subprocess.run([str(self.exe), 'lists', str(d / 'prog.bin'), str(d / 'spec.txt'), str(d)], capture_output=True, text=True, timeout=60)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        got = {int(a): int(b) for a, b in (ln.split() for ln in (d / 'lists.txt').read_text().split('\n') if ln)}
        self.assertEqual(got, {a0: base[a0], a1: base[a1] + 6, a2: base[a2] + 10})

    # ------------------------------------------------------------ the table, the build, the helpers

    def test_the_generated_table_is_sane(self):
        r = subprocess.run([str(self.exe), 'table'], capture_output=True, text=True, timeout=30)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn('table: ', r.stdout)

    def test_compiles_warning_free_for_32_bits(self):
        d = Path(self.dir.name)
        probe = d / 'probe.c'
        probe.write_text('int main(void) { return 0; }\n')
        if subprocess.run(['gcc', '-m32', '-o', str(d / 'probe'), str(probe)], capture_output=True).returncode:
            self.skipTest('no 32-bit toolchain')
        for src in SOURCES[:2]:
            r = subprocess.run(['gcc', '-m32', *FLAGS, '-c', str(src), '-o', str(d / 'x32.o')], capture_output=True, text=True, timeout=60)
            self.assertEqual(r.returncode, 0, r.stderr)

    def test_the_table_text(self):
        row = row_of(file='PL99.PAC', groups=[(0x1000, 5)], runs=[(0, 0, 5)], own=[(5, 3)], tables=[(0xff0, 4)], slices=[(0, 0, 4)],
                     frame_overrides=[(2, 4)], rules=[Q.JUMP_WITHOUT_BIT15], scale=(1, 2))
        other = dict(row, file='PL99X.PAC', load=0x801c8000)
        bare = dict(row, file='PL98.PAC', own=[], frame_overrides=[], rules=[], scale=(0, 0))
        text = Q.c_table([row, other, bare], [('PL04.PAC', 'not in fighters_table.c: x')])
        self.assertEqual(text.count('runs_pl99[] = {'), 1)                  # identical arrays are written once
        self.assertEqual(text.count('own_pl99[] = {'), 1)
        self.assertIn('{"PL99X.PAC", 0x801c8000u, 0x400u, 0x40u, 8u, 6u, 4u, ARCADE_SEQUENCE_JUMP_WITHOUT_BIT15, 1u, 2u, groups_pl99, 1u, runs_pl99, 1u, '
                      'own_pl99, 1u, tables_pl99, 1u, slices_pl99, 1u, frames_pl99, 1u},', text)
        self.assertIn('{"PL98.PAC", 0x801b0000u, 0x400u, 0x40u, 8u, 6u, 4u, 0u, 0u, 0u, groups_pl99, 1u, runs_pl99, 1u, NULL, 0u,', text)
        self.assertIn('PL04.PAC: not in fighters_table.c: x', text)
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
