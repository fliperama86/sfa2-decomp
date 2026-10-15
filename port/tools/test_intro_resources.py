#!/usr/bin/env python3
"""Seconds: sparse pack parsing and lookup bounds, synthetic input only."""
from pathlib import Path
import subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[2]
class ResourceControls(unittest.TestCase):
    def test_bounds_and_lookup(self):
        with tempfile.TemporaryDirectory(prefix='intro-resources-') as directory:
            binary=Path(directory)/'test'
            subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-fsanitize=undefined','-fno-sanitize-recover=undefined',str(ROOT/'port/intro/resources.c'),str(ROOT/'port/intro/tests/resources_test.c'),'-o',str(binary)],check=True,timeout=20)
            subprocess.run([str(binary)],check=True,timeout=5)
if __name__=='__main__':unittest.main()
