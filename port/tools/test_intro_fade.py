#!/usr/bin/env python3
"""Seconds, synthetic palette fade controls; no private data or running game."""
from pathlib import Path
import subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[2]
class FadeControls(unittest.TestCase):
    def test_modes_clock_and_all_colors(self):
        with tempfile.TemporaryDirectory(prefix='intro-fade-') as directory:
            binary=Path(directory)/'fade-test'
            subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-fsanitize=undefined','-fno-sanitize-recover=undefined',str(ROOT/'port/intro/fade.c'),str(ROOT/'port/intro/tests/fade_test.c'),'-o',str(binary)],check=True,timeout=20)
            subprocess.run([str(binary)],check=True,timeout=5)
    def test_real_mutants(self):
        source=(ROOT/'port/intro/fade.c').read_text()
        for old,new in [('layer*4u','(3u-layer)*4u'),('w->fade_delay=spacing-1u','w->fade_delay=spacing'),('color+=1u<<shift','color+=2u<<shift')]:
            with self.subTest(mutation=old),tempfile.TemporaryDirectory(prefix='intro-fade-') as directory:
                self.assertEqual(source.count(old),1)
                directory=Path(directory);unit=directory/'fade.c';unit.write_text(source.replace(old,new));binary=directory/'mutant'
                subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-I'+str(ROOT/'port/intro'),str(unit),str(ROOT/'port/intro/tests/fade_test.c'),'-o',str(binary)],check=True,timeout=20)
                result=subprocess.run([str(binary)],stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=5)
                self.assertNotEqual(result.returncode,0)
if __name__=='__main__':unittest.main()
