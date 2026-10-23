#!/usr/bin/env python3
"""Seconds: the arcade set's views from a synthetic archive and the program piece table; no game files.

Builds port/arcade/rom.c, program.c and zip.c with a driver. A made-up set with
the examined set's member names and sizes (random bytes, stored) is assembled
and each view compared with a reference written here from the layout of
port/intro/import_scene.py; the digests then refuse it, as they must. Members
missing or of another size are refused. The piece builder's layouts and bounds
and the table's own order run in the driver.
"""
from pathlib import Path
import os, subprocess, sys, tempfile, unittest, zipfile
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/intro'))
from import_scene import lanes, unshuffle   # the layout the intro's importer already pins with the real set

PROGRAM = [f'sz2u.{i:02d}' for i in range(3, 9)]
TILES = [[f'sz2.{i:02d}m' for i in g] for g in ((13, 15, 17, 19), (14, 16, 18, 20))]
SIZES = {**{n: 0x80000 for n in PROGRAM}, **{n: 0x400000 for n in TILES[0]}, **{n: 0x100000 for n in TILES[1]},
         'sz2.11m': 0x200000, 'sz2.12m': 0x200000, 'sz2.01a': 0x20000, 'sz2.02a': 0x20000}

def turned(b):
    out = bytearray(len(b)); out[0::2] = b[1::2]; out[1::2] = b[0::2]; return bytes(out)

class ArcadeRom(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.dir = tempfile.TemporaryDirectory(prefix='arcade-rom-')
        d = Path(cls.dir.name); cls.exe = d / 'rom_test'
        subprocess.run(['gcc', '-std=c99', '-O1', '-Wall', '-Wextra', '-Werror', '-fsanitize=address,undefined', '-fno-sanitize-recover=undefined',
                        *[str(ROOT / p) for p in ('port/arcade/rom.c', 'port/arcade/program.c', 'port/arcade/zip.c', 'port/src/sha256.c', 'port/arcade/tests/rom_test.c')],
                        '-o', str(cls.exe)], check=True, timeout=60)
        cls.members = {n: os.urandom(s) for n, s in SIZES.items()}

    @classmethod
    def tearDownClass(cls):
        cls.dir.cleanup()

    def archive(self, name, members):
        path = Path(self.dir.name) / name
        with zipfile.ZipFile(path, 'w', zipfile.ZIP_STORED) as z:
            for n, b in members.items(): z.writestr(n, b)
        return path

    def test_views_match_the_reference_and_digests_refuse(self):
        out = Path(self.dir.name) / 'views'; out.mkdir()
        r = subprocess.run([str(self.exe), 'assemble', str(self.archive('set.zip', self.members)), str(out)], capture_output=True, text=True, timeout=60)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        m = self.members
        self.assertEqual((out / 'program.bin').read_bytes(), turned(b''.join(m[n] for n in PROGRAM)))
        self.assertEqual((out / 'tiles.bin').read_bytes(), unshuffle(b''.join(lanes([m[n] for n in g]) for g in TILES)))
        self.assertEqual((out / 'samples.bin').read_bytes(), turned(m['sz2.11m'] + m['sz2.12m']))
        self.assertEqual((out / 'sound.bin').read_bytes(), m['sz2.01a'] + m['sz2.02a'])
        self.assertIn('verify: arcade set: the program ROMs', r.stdout)

    def test_missing_or_resized_members_are_refused(self):
        less = dict(self.members); del less['sz2.17m']
        r = subprocess.run([str(self.exe), 'assemble', str(self.archive('less.zip', less)), self.dir.name], capture_output=True, text=True, timeout=60)
        self.assertEqual(r.returncode, 1); self.assertIn('sz2.17m: not in the archive', r.stdout)
        resized = dict(self.members); resized['sz2u.05'] = resized['sz2u.05'][:-2]
        r = subprocess.run([str(self.exe), 'assemble', str(self.archive('resized.zip', resized)), self.dir.name], capture_output=True, text=True, timeout=60)
        self.assertEqual(r.returncode, 1); self.assertIn('sz2u.05: not the size', r.stdout)

    def test_pieces(self):
        r = subprocess.run([str(self.exe), 'pieces'], capture_output=True, text=True, timeout=10)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)

if __name__ == '__main__':
    # The line the review reads (tools/ai_workflow/review.py): the last line, on a pass only.
    import sys, time
    started = time.monotonic()
    result = unittest.main(exit=False).result
    took = time.monotonic() - started
    bad, ran = len(result.failures) + len(result.errors), result.testsRun
    if bad or not ran:
        print(f"{bad} of {ran} tests FAILED in {took:.1f} seconds")
        sys.exit(1)
    print(f"all {ran} tests passed in {took:.1f} seconds")
