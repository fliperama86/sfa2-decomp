#!/usr/bin/env python3
"""Seconds: native audio override and PS1-sector loader/lifetime with synthetic disc."""
from pathlib import Path
import subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[2]
class HostAudioControls(unittest.TestCase):
    def test_stream_selector(self):
        with tempfile.TemporaryDirectory(prefix='intro-audio-selector-') as directory:
            binary=Path(directory)/'test'
            subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-ffunction-sections','-fdata-sections',str(ROOT/'port/intro/tests/audio_override_test.c'),'-Wl,--gc-sections','-o',str(binary)],check=True,timeout=20)
            subprocess.run([str(binary)],check=True,timeout=5)
    def test_sector_loader_and_lifetime(self):
        with tempfile.TemporaryDirectory(prefix='intro-audio-host-') as directory:
            binary=Path(directory)/'test'
            subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-fsanitize=undefined','-fno-sanitize-recover=undefined',*[str(ROOT/x) for x in ['port/intro/tests/audio_host_test.c','port/src/intro_audio.c','port/src/disc.c','port/intro/audio.c','port/src/xa.c']],'-o',str(binary)],check=True,timeout=20)
            subprocess.run([str(binary),str(Path(directory)/'disc.bin')],check=True,timeout=5)
if __name__=='__main__':unittest.main()
