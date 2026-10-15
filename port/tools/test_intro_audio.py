#!/usr/bin/env python3
"""Seconds: synthetic XA soundtrack decode, bounded mixer and gain; no sound device."""
from pathlib import Path
import subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[2]
class AudioControls(unittest.TestCase):
    def test_decode_and_mix(self):
        with tempfile.TemporaryDirectory(prefix='intro-audio-') as directory:
            binary=Path(directory)/'test'
            subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-fsanitize=undefined','-fno-sanitize-recover=undefined',*[str(ROOT/path) for path in ['port/intro/audio.c','port/src/xa.c','port/intro/tests/audio_test.c']],'-o',str(binary)],check=True,timeout=20)
            subprocess.run([str(binary)],check=True,timeout=5)
if __name__=='__main__':unittest.main()
