#!/usr/bin/env python3
"""Seconds: independent native initialization, resource failure and finished state.
Full native progression/resource coverage is checked privately by import_full.
"""
from pathlib import Path
import subprocess,sys,tempfile,unittest
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'port/intro'))
from import_full import SOURCES
class PlayerControls(unittest.TestCase):
    def test_startup_and_bounds(self):
        with tempfile.TemporaryDirectory(prefix='intro-player-') as directory:
            binary=Path(directory)/'test'
            subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-fsanitize=undefined','-fno-sanitize-recover=undefined',*[str(ROOT/'port/intro'/(x+'.c')) for x in SOURCES],str(ROOT/'port/intro/tests/player_test.c'),'-o',str(binary)],check=True,timeout=30)
            subprocess.run([str(binary)],check=True,timeout=5)
if __name__=='__main__':unittest.main()
