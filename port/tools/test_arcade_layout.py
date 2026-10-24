#!/usr/bin/env python3
"""Seconds: the disc's layout (port/arcade/layout.py and the backend in port/src/disc.c) on made-up data; no game files.

layout.py: the name rule, the stream model (the subheader of every sector from a count and 8 channel ends, and the ends back from the sectors), the
refusal of a stream the model does not reproduce, the filler runs, and the table's text. The backend (tests/layout_test.c with the real disc.c, in a
temporary folder, with the sanitizers): a made-up 40-sector disc and a made-up pack of it in format 03 (a file, a music stream with one sector kept whole, a file the pack leaves out; no raw
extent, no subheader). With the layout, find and list are the table's with no sector read (without it the same pack cannot list: it holds no volume
descriptor), a sector outside the files is made from the layout (the pack cannot serve it without), a filler run has its own submode, a silent stream
sector has the model's subheader, a kept one stays the pack's, a pack of format 02 is refused with a line, a sector of a left-out file is still
refused, and a layout that is not the pack's disc (another length, a missing file, another size, overlapping files) is refused with a line.
"""
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
ARC = ROOT / 'port/arcade'
SRC = ROOT / 'port/src'
sys.path.insert(0, str(ARC))
import layout as L   # noqa: E402

FLAGS = ('-std=gnu99', '-O1', '-Wall', '-Wextra', '-Werror')
SAN = ('-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', '-g')
SECTOR = 2352
ENDS = [8, 9, 10, 11, 12, 3, 14, 15]            # the made-up stream's channel ends (the driver's table)


def pack_bytes(magic=b'SFA2PK03'):
    """A made-up pack of the 40-sector disc (the format is in port/src/disc.c): a file, and a music stream with one sector kept whole. No raw
    extents and no subheaders: the sectors outside the files and the silent stream sectors are the layout's."""
    pattern = bytes(i % 251 for i in range(5000))
    stream = struct.pack('<II', 4, 1) + bytes([0xcd]) * SECTOR      # one run: sector 4 of the stream, kept whole
    parts = [
        (2, 3, 0, 5000, 1, 0, struct.pack('<II', 0, 5000) + pattern),
        (5, 16, 2, 0, 1, 0, stream),
    ]
    head = magic + struct.pack('<III', 40, len(parts), 0)
    at = len(head) + 64 * len(parts)
    table = b''
    for first, count, kind, size, rc, bc, data in parts:
        table += struct.pack('<6I', first, count, kind, size, rc, bc) + bytes(32) + struct.pack('<Q', at)
        at += len(data)
    return head + table + b''.join(p[-1] for p in parts)


class Pure(unittest.TestCase):
    def test_names(self):
        self.assertEqual(L.clean_name('/PAC/PL00X.PAC;1'), 'pl00x.pac')
        self.assertEqual(L.clean_name('/SYSTEM.CNF;1'), 'system.cnf')
        self.assertEqual(L.clean_name('/ZERO2_XA/ZERO200.XA;1'), 'zero200.xa')
        self.assertEqual(L.clean_name('/A.'), 'a')

    def test_stream_model_round_trip(self):
        count, end = 48, [40, 41, 34, 35, 28, 29, 38, 39]
        subs = [L.model_subheader(count, end, i) for i in range(count)]
        self.assertEqual(L.stream_ends(subs), end)
        self.assertEqual(subs[0], bytes([1, 0, 0x64, 1]) * 2)
        self.assertEqual(subs[41], bytes([1, 1, 0xe4, 1]) * 2)       # channel 1's last sound sector
        self.assertEqual(subs[37], bytes([1, 0, 0, 0]) * 2)          # channel 5 ended at sector 29: its later places are padding
        self.assertEqual(subs[47], bytes([1, 0, 0x80, 0]) * 2)       # the file's last sector
        self.assertEqual(L.model_subheader(count, end, 38), bytes([1, 6, 0xe4, 1]) * 2)

    def test_a_stream_the_model_does_not_give_is_refused(self):
        count, end = 24, [16, 17, 18, 19, 20, 21, 22, 23 - 1]
        subs = [bytearray(L.model_subheader(count, end, i)) for i in range(count)]
        subs[9][2] = 0x41                                               # a submode the model never gives
        with self.assertRaises(L.Fail):
            L.stream_ends([bytes(s) for s in subs])
        subs = [bytearray(L.model_subheader(count, end, i)) for i in range(count)]
        subs[5][2] = 0xe4                                               # a channel that ends twice
        with self.assertRaises(L.Fail):
            L.stream_ends([bytes(s) for s in subs])
        with self.assertRaises(L.Fail):
            L.stream_ends([bytes([1, 0, 0x64, 1]) * 2] * 8)              # no channel ends

    def test_fill_runs(self):
        form2 = bytes([0, 0, 0x20, 0]) * 2
        data = bytes([0, 0, 8, 0]) * 2
        headers = [data] * 3 + [form2] * 20 + [data] * 4 + [form2] * 5
        covered = [True] * 3 + [False] * 20 + [False] * 4 + [False] * 5
        self.assertEqual(L.fill_runs(headers, covered), [(3, 20, 0x20)])  # 16 or more of one kind outside the files; the short run is not a row
        covered[10] = True                                                # a file in the middle splits the run
        self.assertEqual(L.fill_runs(headers, covered), [])

    def test_table_text(self):
        text = L.c_table(40, [('a.dat', 2, 5000), ('s.xa', 5, 32768)], [(5, 16, ENDS)], [(24, 3, 0x20)])
        self.assertIn('{"a.dat", 2, 5000},', text)
        self.assertIn('{5, 16, {8, 9, 10, 11, 12, 3, 14, 15}},', text)
        self.assertIn('{24, 3, 0x20},', text)
        self.assertIn('40, layout_files, 2, layout_streams, 1, layout_fills, 1', text)
        self.assertIn('port_arcade_layout', text)
        self.assertNotIn(chr(0x2014), text)


class Backend(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.dir = tempfile.TemporaryDirectory()
        d = Path(cls.dir.name)
        cls.exe = d / 'layout_test'
        subprocess.run(['gcc', *FLAGS, *SAN, '-I', str(SRC), str(SRC / 'disc.c'), str(SRC / 'sha256.c'), str(ARC / 'tests/layout_test.c'), '-o', str(cls.exe)],
                       check=True, timeout=240)
        cls.pack = d / 'pack'
        cls.pack.write_bytes(pack_bytes())

    @classmethod
    def tearDownClass(cls):
        cls.dir.cleanup()

    def run_exe(self, *args, layout=True, bad=None):
        cmd = [str(self.exe)] + (['--layout'] if layout else []) + (['--bad', str(bad)] if bad else []) + [str(self.pack)] + [str(a) for a in args]
        return subprocess.run(cmd, capture_output=True, text=True, timeout=60)

    def test_list_and_find_come_from_the_table(self):
        r = self.run_exe('list')
        self.assertEqual((r.returncode, r.stdout.split('\n')[:3]), (0, ['a.dat 2 5000', 's.xa 5 32768', 'gone.str 21 6144']))
        self.assertEqual(self.run_exe('find', 'A.DAT;1').stdout.strip(), '2 5000')
        self.assertEqual(self.run_exe('find', 'gone.str').stdout.strip(), '21 6144')
        r = self.run_exe('find', 'nope.bin')
        self.assertEqual(r.returncode, 1)
        self.assertIn('not in the layout', r.stdout)
        # the same pack holds no volume descriptor: without the layout it cannot list
        self.assertEqual(self.run_exe('list', layout=False).returncode, 1)

    def test_sectors_outside_the_files_are_made_from_the_layout(self):
        head = '00ffffffffffffffffffff00' + '000200' + '02'            # sync, the address of sector 0 (150), mode 2
        r = self.run_exe('raw', 0)
        self.assertEqual(r.stdout.strip(), head + '00000800' + '00000800' + ' 0')   # a form 1 data sector of zeros: the pack holds no raw extent
        self.assertEqual(self.run_exe('raw', 0, layout=False).returncode, 1)         # without the layout the pack cannot serve it
        r = self.run_exe('raw', 24)                                      # a filler run: its own submode
        self.assertTrue(r.stdout.startswith('00ffffffffffffffffffff00' + '000224' + '02' + '00002000' + '00002000'), r.stdout)   # address 174 is 00:02:24
        self.assertEqual(self.run_exe('raw', 30).returncode, 0)
        self.assertEqual(self.run_exe('raw', 40).returncode, 1)         # past the end
        self.assertEqual(self.run_exe('raw', 21).returncode, 1)         # a sector of a file the pack leaves out is still refused

    def test_stream_subheaders_are_the_models(self):
        for i in (0, 5, 8, 12, 13, 15):
            want = L.model_subheader(16, ENDS, i).hex()
            r = self.run_exe('raw', 5 + i)
            self.assertEqual(r.stdout[32:48], want, (i, r.stdout))
            self.assertEqual(self.run_exe('sub', 5 + i).stdout.strip(), want)
        self.assertEqual(self.run_exe('raw', 5).stdout[32:48], '0100640101006401')
        self.assertEqual(self.run_exe('raw', 5, layout=False).returncode, 1)         # the pack holds no subheader: it is not read without the layout
        r = self.run_exe('raw', 9)                                       # the sector the pack keeps whole stays the pack's
        self.assertEqual(r.stdout[:48], 'cd' * 24)
        self.assertEqual(self.run_exe('raw', 9, layout=False).stdout[:48], 'cd' * 24)
        self.assertEqual(self.run_exe('sub', 4).returncode, 1)          # outside the stream
        self.assertEqual(self.run_exe('sub', 21).returncode, 1)

    def test_a_pack_of_the_earlier_format_is_refused(self):
        old = Path(self.dir.name) / 'old.pack'
        old.write_bytes(pack_bytes(b'SFA2PK02'))
        r = subprocess.run([str(self.exe), str(old), 'list'], capture_output=True, text=True, timeout=60)
        self.assertEqual(r.returncode, 2)
        self.assertIn('earlier format', r.stdout)
        self.assertIn('--arcade-import', r.stdout)
        junk = Path(self.dir.name) / 'junk.pack'
        junk.write_bytes(b'not a pack at all, not at all')
        r = subprocess.run([str(self.exe), str(junk), 'list'], capture_output=True, text=True, timeout=60)
        self.assertEqual(r.returncode, 2)

    def test_file_bytes_come_from_the_pack(self):
        pattern = bytes(i % 251 for i in range(5000))
        r = self.run_exe('read', 2, 5000)
        self.assertEqual(r.stdout.split(), [pattern[:8].hex(), str(sum(pattern))])

    def test_a_layout_that_is_not_the_packs_disc_is_refused(self):
        for bad, text in ((1, 'overlap'), (2, 'another length'), (3, 'not a file of the layout'), (4, 'not a file of the layout')):
            r = self.run_exe('list', bad=bad)
            self.assertEqual(r.returncode, 3, (bad, r.stdout))
            self.assertIn(text, r.stdout)


if __name__ == '__main__':
    import time
    started = time.monotonic()
    result = unittest.main(exit=False).result
    took = time.monotonic() - started
    bad, ran = len(result.failures) + len(result.errors), result.testsRun
    if bad or not ran:
        print(f"{bad} of {ran} tests FAILED in {took:.1f} seconds")
        sys.exit(1)
    print(f"all {ran} tests passed in {took:.1f} seconds")
