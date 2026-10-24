#!/usr/bin/env python3
"""Seconds: the pure parts of port/arcade/music_map.py on made-up bytes; no game files.

A small 68000 program is built here with the request function's layout (the selector jump table, three song tables, the ending dispatcher, the staff
roll and the bonus byte table) at the addresses the tool reads, and small MIPS code and a made-up archive and disc image. Every expected number is worked
out by hand from the rules in the tool's docstring: what d0 holds at a request (a `moveq` in the straight run, none past a call or jump, several over a
branch, a byte table), the tables behind the selectors, the ending blocks and their ranges, the staff and bonus songs, the constants a MIPS call passes
(in the delay slot or loaded early), the table a scene handler indexes, the pairing of an ending's requests, the merge of rows and the C table.
"""
from pathlib import Path
import io, struct, subprocess, sys, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import music_map as M

SOURCE = ROOT / 'port/arcade/music_map.py'


def put(b, a, *words):
    for i, w in enumerate(words):
        b[a + 2 * i:a + 2 * i + 2] = struct.pack('>H', w)


def put32(b, a, v):
    b[a:a + 4] = struct.pack('>I', v)


def request_program():
    """A made-up program: the request function, its jump table (selector 0, 1, 2 and the rest at four bodies), the tables behind them."""
    b = bytearray(0x100000)
    put(b, M.REQUEST, 0x4a2d, 0x00a6)
    bodies = {0: 0x15600, 1: 0x15640, 2: 0x15680, 3: 0x156a0}
    for sel in range(24):
        put(b, M.SELECTOR_JUMPS + 2 * sel, (bodies[min(sel, 3)] - M.SELECTOR_JUMPS) & 0xffff)
    for sel in (0, 1, 3):
        a = bodies[sel]
        put(b, a, 0x7200, 0x323b, 0x0008)                     # moveq #0,d1 / move.w X(pc,d0.w),d1: the extension word is at a + 4, the table at a + 4 + 8
        put(b, a + 6, 0x4ef9, 0x0001, 0x3c5e)                 # jmp 0x13c5e (6 bytes): the table starts at a + 12
    put(b, bodies[2], 0x723e)                                 # moveq #$3e,d1
    # selector 0: table at 0x1560c up to 0x15640 = 26 words; selector 1: 0x1564c up to 0x15680 = 26 words; the rest: 0x156ac to the next start (none: use the end)
    for i in range(26):
        put(b, 0x1560c + 2 * i, 100 + i)
        put(b, 0x1564c + 2 * i, 200 + i)
    return b, bodies


class Arcade(unittest.TestCase):
    def test_pc_table(self):
        b = bytearray(64)
        put(b, 10, 0x323b, 0x0008)
        self.assertEqual(M.pc_table(b, 10), 10 + 2 + 8)
        put(b, 20, 0x103b, 0x00f0)                             # a negative 8-bit displacement
        self.assertEqual(M.pc_table(b, 20), 20 + 2 - 16)

    def test_request_tables_follow_the_code(self):
        b, bodies = request_program()
        # the third body (selectors 3 up) has no later start: give it a successor so the table ends there
        put(b, M.SELECTOR_JUMPS + 2 * 2, (0x156f0 - M.SELECTOR_JUMPS) & 0xffff)
        put(b, 0x156f0, 0x723e)
        put(b, 0x156ac - 0, 0)                                  # the selector-3 table starts at 0x156ac, its words are zero
        tables, s2 = M.request_tables(bytes(b))
        self.assertEqual(s2, 0x3e)
        self.assertEqual(len(tables[0]), 26)
        self.assertEqual(tables[0][0], 100)
        self.assertEqual(tables[0][25], 125)
        self.assertEqual(tables[1][3], 203)
        self.assertEqual(len(tables[3]), (0x156f0 - 0x156ac) // 2)

    def test_request_tables_refuse_other_code(self):
        b, bodies = request_program()
        put(b, M.REQUEST, 0x4e75, 0)
        with self.assertRaises(ValueError):
            M.request_tables(bytes(b))
        b, bodies = request_program()
        for sel in range(24):                                   # all selectors to one body: not "three, then one"
            put(b, M.SELECTOR_JUMPS + 2 * sel, 0x100)
        with self.assertRaises(ValueError):
            M.request_tables(bytes(b))

    def test_song_of(self):
        t = ({0: [1, 2, 3], 1: [10, 11], 3: [20, 21, 22]}, 62)
        self.assertEqual(M.song_of(t, 2), 62)
        self.assertEqual(M.song_of(t, 0, 2), 3)
        self.assertEqual(M.song_of(t, 1, 1), 11)
        self.assertEqual(M.song_of(t, 5), 22)

    def test_request_sites(self):
        b = bytearray(0x1000)
        put(b, 0x20, 0x4eb9); put32(b, 0x22, M.REQUEST)
        put(b, 0x40, 0x4ef9); put32(b, 0x42, M.REQUEST)
        put(b, 0x60, 0x4ef9); put32(b, 0x62, M.REQUEST + 2)    # another function
        self.assertEqual(M.request_sites(bytes(b)), [0x20, 0x40])

    def d0(self, build, site=0x200):
        b = bytearray(0x1000)
        build(b)
        return M.d0_values(bytes(b), site)

    def test_d0_is_the_moveq_before_the_call(self):
        self.assertEqual(self.d0(lambda b: put(b, 0x1fe, 0x700e)), ('imm', [0x0e]))

    def test_d0_stops_at_a_call_a_jump_and_a_branch(self):
        def calls(b):
            put(b, 0x1f0, 0x7004, 0x6100, 0x0010, 0x701e)      # moveq #4,d0 / bsr.w / moveq #$1e,d0
        self.assertEqual(self.d0(calls)[1], [0x1e])
        def jump(b):
            put(b, 0x1f0, 0x7007, 0x4ef9, 0x0001, 0x900e, 0x700e)   # moveq #7 / jmp abs / moveq #$e
        self.assertEqual(self.d0(jump)[1], [0x0e])
        def bra(b):
            put(b, 0x1f0, 0x7007, 0x6004, 0x700e)
        self.assertEqual(self.d0(bra)[1], [0x0e])

    def test_d0_has_both_values_over_a_conditional_branch(self):
        def two(b):                                            # moveq #$16 / tst.b / beq.w / ... / moveq #$14 / (site)
            put(b, 0x1e0, 0x7016, 0x4a2d, 0xa7b9, 0x6700, 0x0006, 0x3b7c, 0x012c, 0xa7be, 0x7014)
        self.assertEqual(self.d0(two, 0x1f6), ('imm', [0x14, 0x16]))

    def test_d0_from_a_byte_table(self):
        def table(b):
            put(b, 0x1fc, 0x103b, 0x0008)                      # move.b X(pc,d0.w),d0: ext at 0x1fe, table at 0x206
            b[0x206:0x20a] = bytes([0x2c, 0x2c, 0x2e, 0x2c])
        kind, base = self.d0(table)
        self.assertEqual((kind, base), ('table', 0x206))

    def test_site_songs(self):
        t = ({0: [1, 2], 1: [10, 11, 12], 3: [20, 21, 22, 23, 24, 25, 26, 27]}, 62)
        b = bytearray(0x1000)
        put(b, 0x1fe, 0x700a)                                  # d0 = 10: selector 5
        self.assertEqual(M.site_songs(bytes(b), t, 0x200)[2], [22])
        put(b, 0x1fe, 0x7002)                                  # selector 1: the ending theme
        self.assertEqual(M.site_songs(bytes(b), t, 0x200)[2], [10, 11, 12])
        self.assertEqual(M.site_songs(bytes(b), t, 0x200, 1)[2], [11])
        put(b, 0x1fe, 0x7004)                                  # selector 2
        self.assertEqual(M.site_songs(bytes(b), t, 0x200)[2], [62])
        put(b, 0x1fc, 0x103b, 0x0008)
        b[0x206:0x20a] = bytes([0x06, 0x06, 0x08, 0x06])
        self.assertEqual(M.site_songs(bytes(b), t, 0x200)[2], [20, 21])

    def test_describe_site(self):
        t = ({0: [1, 2], 1: [10, 11], 3: [20, 21, 22, 23, 24, 25, 26, 27]}, 62)
        b = bytearray(0x1000)
        put(b, 0x1fe, 0x7000)
        self.assertEqual(M.describe_site(bytes(b), t, 0x200), 'fighter')
        put(b, 0x1fe, 0x7002)
        self.assertEqual(M.describe_site(bytes(b), t, 0x200), 'ending')
        put(b, 0x1fe, 0x700c)
        self.assertEqual(M.describe_site(bytes(b), t, 0x200), '23')

    def test_ending_blocks_and_ranges(self):
        b = bytearray(0x100000)
        a = M.ENDING_DISPATCH
        put(b, a, 0x7000, 0x102d, 0x0130, 0xd040, 0xd040, 0x207b, 0x0004, 0x4ed0)
        starts = [0x86c58 + 0x100 * (k % 5) for k in range(M.ENDING_INDEXES)]
        for k, s in enumerate(starts):
            put32(b, a + 16 + 4 * k, s)
        blocks = M.ending_blocks(bytes(b))
        self.assertEqual(blocks, starts)
        self.assertEqual(M.block_range(blocks, 0), (0x86c58, 0x86d58))
        self.assertEqual(M.block_range(blocks, 4), (0x86c58 + 0x400, M.ENDING_CODE[1]))
        put(b, a, 0x7001)
        with self.assertRaises(ValueError):
            M.ending_blocks(bytes(b))

    def test_staff_and_bonus_songs(self):
        b = bytearray(0x100000)
        t = ({0: [1, 2], 1: list(range(40, 64)), 3: list(range(20, 41))}, 62)
        a = M.STAFF_FUNCTION
        put(b, a, 0x0c2d, 0x0002, 0x0130, 0x6718, 0x0c2d, 0x0014, 0x0130, 0x6710, 0x7000, 0x102d, 0x0130, 0x103b, 0x0010, 0x4ef9)
        # the site is the jmp: a + 0x1a; the table read is the two words before it, ext word at a + 0x18 gives table a + 0x18 + 0x10
        put(b, a + 0x16, 0x103b, 0x0010)
        base = a + 0x18 + 0x10
        for k in range(24):
            b[base + k] = 0x18 + 2 * (k % 3)                   # selectors 12, 13, 14: table indexes 9, 10, 11
        put(b, a + 0x1a, 0x4ef9)
        songs = M.staff_songs(bytes(b), t)
        self.assertEqual(songs[0], 29)
        self.assertEqual(songs[1], 30)
        self.assertEqual(songs[2], 42)                          # ending 2 plays its ending theme (table 1, entry 2)
        self.assertEqual(songs[20], 60)                         # and ending 20
        # bonus: a byte table of 22 entries, stage 11 differs
        c = bytearray(0x100000)
        put(c, M.BONUS_SITE - 4, 0x103b, 0x0008)
        for i in range(22):
            c[M.BONUS_SITE - 2 + 8 + i] = 0x2c if i != 11 else 0x2e
        bonus = M.bonus_songs(bytes(c), ({0: [], 1: [], 3: list(range(19, 40))}, 62))
        self.assertEqual(bonus[0], 19 + 19)
        self.assertEqual(bonus[11], 19 + 20)


class Ps1(unittest.TestCase):
    JAL = lambda self, target: 0x0c000000 | (target >> 2) & 0x3ffffff
    LI = lambda self, reg, v: 0x34000000 | reg << 16 | v          # ori reg, zero, v
    ADDIU = lambda self, reg, v: 0x24000000 | reg << 16 | v & 0xffff

    def test_argument_in_the_delay_slot(self):
        w = [0x27bdffe8, self.JAL(0x80151020), self.LI(4, 0x206)]
        self.assertEqual(M.argument(w, 1, 4), 0x206)

    def test_argument_loaded_early(self):
        w = [self.LI(4, 0x303), 0x94430050, 0x340500b4, 0xa4450060, 0x24630001, self.JAL(0x80151020), 0xa4430050]
        self.assertEqual(M.argument(w, 5, 4), 0x303)

    def test_argument_not_a_constant_or_behind_a_branch(self):
        w = [self.LI(4, 0x303), 0x10400003, 0, self.JAL(0x80151020), 0]       # beq between the load and the call
        self.assertIsNone(M.argument(w, 3, 4))
        w = [0x8c440000, self.JAL(0x80151020), 0]                              # lw a0: from memory
        self.assertIsNone(M.argument(w, 1, 4))

    def test_music_calls(self):
        by_code, pending = 0x80150f00, 0x80151020
        w = [self.LI(4, 1), self.LI(5, 0x502), self.JAL(by_code), 0,           # (1, 0x502)
             self.LI(4, 0x207), self.JAL(pending), 0,                          # pending(0x207)
             self.LI(4, 6), self.LI(5, 2), self.JAL(by_code), 0,               # (6, 2): a fade, not music
             self.JAL(0x80140000), 0]
        self.assertEqual(M.music_calls(w, 0x80010000, 0x80010000, by_code, pending), [(0x80010008, 0x502), (0x80010014, 0x207)])

    def test_handler_table_and_loads(self):
        w = [0x3c028019, 0x8c42f5a0, 0x27bdffe8, 0xafbf0010, 0x94420050, 0, 0x21080, 0x3c018005, 0x24210c58, 0x220821]
        self.assertEqual(M.handler_table(w), 0x80050c58)
        w2 = [0x3c018003, 0x242189d4]                                           # a negative low half
        self.assertEqual(M.handler_table(w2), 0x800289d4)
        self.assertIsNone(M.handler_table([0x3c028019, 0x8c42f5a0, 0]))
        self.assertTrue(M.loads_immediate([self.LI(2, 0x407), self.ADDIU(3, 0xb)], 0xb))
        self.assertFalse(M.loads_immediate([self.LI(2, 0x407)], 0x503))

    def test_pointer_run(self):
        mem = {0x100: 0x80011000, 0x104: 0x80011040, 0x108: 0x80011004, 0x10c: 0x5}
        self.assertEqual(M.pointer_run(lambda a: mem[a], 0x100, lambda w: 0x80011000 <= w < 0x80012000 and w % 4 == 0), [0x80011000, 0x80011040, 0x80011004])

    def test_overlay_words(self):
        ov = M.Overlay(struct.pack('<4I', 1, 2, 3, 4), 0x80010000)
        self.assertEqual(ov.word(0x80010008), 3)
        self.assertEqual(ov.words(0x80010004, 0x8001000c), [2, 3])

    def test_pac_chunk(self):
        # an archive of two chunks: (table 0, slot 0x28) of 8 bytes and (table 3, slot 2) of 4 bytes
        def entry(kind, data):
            return struct.pack('<III', kind, len(data), struct.unpack_from('<I', data)[0]) + bytes(20)
        c1, c2 = struct.pack('<2I', 0x11111111, 0x22222222), struct.pack('<I', 0x33333333)
        head = bytearray(0x800)
        head[0:4] = struct.pack('<I', 2)
        head[0x20:0x40] = entry(0x28, c1)
        head[0x40:0x60] = entry(3 << 16 | 2, c2)
        data = bytes(head) + c1.ljust(0x800, b'\0') + c2.ljust(0x800, b'\0')
        self.assertEqual(M.pac_chunk(data, 0, 0x28), c1)
        self.assertEqual(M.pac_chunk(data, 3, 2), c2)
        with self.assertRaises(ValueError):
            M.pac_chunk(data, 0, 0x29)


class Pairing(unittest.TestCase):
    def test_first_request_and_one_other(self):
        self.assertEqual(M.pair_ending([0x600, 0x601], 28, [29]), [(0x600, 28, 'table'), (0x601, 29, 'position')])

    def test_repeats_and_the_theme_requested_again(self):
        # the PS1 scene 0x504, 0x505, 0x504 against an arcade block that requests the ending theme again and one other song
        self.assertEqual(M.pair_ending([0x504, 0x505, 0x504], 44, [56, 44]), [(0x504, 44, 'table'), (0x505, 56, 'position')])

    def test_a_single_request(self):
        self.assertEqual(M.pair_ending([0x300], 30, []), [(0x300, 30, 'table')])

    def test_counts_that_do_not_agree_are_refused(self):
        with self.assertRaises(ValueError):
            M.pair_ending([0x300, 0x301], 30, [])
        with self.assertRaises(ValueError):
            M.pair_ending([0x300], 30, [31])
        with self.assertRaises(ValueError):
            M.pair_ending([0x300, 0x301, 0x302], 30, [31, 32])           # two others on each side: no order to pair them by

    def test_merge_rows(self):
        rows = [(0x102, 12, 'table', 'fighter'), (0x102, 12, 'table', 'fighter'), (0x206, 40, 'table', 'ending'), (0x206, 40, 'table', 'staff')]
        self.assertEqual(M.merge_rows(rows), {0x102: (12, 'table', 'fighter'), 0x206: (40, 'table', 'ending')})
        with self.assertRaises(ValueError):
            M.merge_rows([(1, 2, 'table', 'a'), (1, 3, 'table', 'b')])
        self.assertEqual(M.merge_rows([(1, 2, 'inferred', 'a'), (1, 2, 'position', 'b')])[1][1], 'position')

    def test_c_table(self):
        merged = {0x604: (21, 'inferred', 'declared'), 0x000: (1, 'table', 'fighter'), 0x606: (26, 'position', 'declared')}
        self.assertEqual(M.c_table(merged, False), '    {0x000, 1}, {0x606, 26},')
        self.assertEqual(M.c_table(merged, True), '    {0x000, 1}, {0x604, 21}, {0x606, 26},')
        many = {i: (i, 'table', 'x') for i in range(9)}
        self.assertEqual(M.c_table(many, False).count('\n'), 1)


class Declared(unittest.TestCase):
    def test_the_declared_pairs_are_consistent(self):
        codes = [d[0] for d in M.DECLARED]
        self.assertEqual(len(codes), len(set(codes)))
        for code, song, conf, fn, sites, why in M.DECLARED:
            self.assertIn(conf, M.CONFIDENCE)
            self.assertTrue(fn and sites and why)
            self.assertTrue(0x300 <= code < 0x800 and 1 <= song <= 62)
        self.assertEqual(len(M.SITES), 25)

    def test_audio_picks_are_in_the_table_range(self):
        for song, (code, how) in M.AUDIO.items():
            self.assertIn(how, ('high', 'probable'))
            self.assertTrue(1 <= song <= 62 and code < 0x800)


class Disc(unittest.TestCase):
    def test_iso_files(self):
        sectors = {}
        def sector(n, data):
            sectors[n] = bytes(24) + data.ljust(2048, b'\0')
        def record(name, lba, size, flags):
            nm = name.encode()
            r = bytearray(33 + len(nm))
            r[0] = len(r) + (len(r) & 1)
            r[2:6] = struct.pack('<I', lba)
            r[10:14] = struct.pack('<I', size)
            r[25] = flags
            r[32] = len(nm)
            r[33:] = nm
            return bytes(r).ljust(r[0], b'\0')
        pvd = bytearray(2048)
        pvd[156:156 + 34] = record('\0', 20, 2048, 2).ljust(34, b'\0')[:34]
        sector(16, bytes(pvd))
        sector(20, record('\0', 20, 2048, 2) + record('\1', 20, 2048, 2) + record('PAC', 21, 2048, 2) + record('SLPS_004.15;1', 30, 5000, 0))
        sector(21, record('\0', 21, 2048, 2) + record('\1', 20, 2048, 2) + record('END00.PAC;1', 40, 100, 0))
        raw = bytearray(2352 * 41)
        for n, d in sectors.items():
            raw[2352 * n:2352 * n + len(d)] = d
        files, sec = M.iso_files(io.BytesIO(bytes(raw)))
        self.assertEqual(files, {'/SLPS_004.15': (30, 5000), '/PAC/END00.PAC': (40, 100)})


class Command(unittest.TestCase):
    def test_arguments_are_required(self):
        r = subprocess.run([sys.executable, str(SOURCE), '--disc', 'nowhere.bin'], capture_output=True, text=True, timeout=30)
        self.assertEqual(r.returncode, 2)
        self.assertIn('--arcade or --decrypted', r.stderr)

    def test_a_missing_file_is_a_message_not_a_crash(self):
        r = subprocess.run([sys.executable, str(SOURCE), '--disc', 'nowhere.bin', '--decrypted', 'nowhere.bin'], capture_output=True, text=True, timeout=30)
        self.assertEqual(r.returncode, 1)
        self.assertTrue(r.stderr.startswith('music_map:'))

    def test_no_em_dash(self):
        dash = chr(0x2014)
        self.assertNotIn(dash, SOURCE.read_text())
        self.assertNotIn(dash, Path(__file__).read_text())


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
