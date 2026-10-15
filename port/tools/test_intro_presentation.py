#!/usr/bin/env python3
"""Seconds, no private input: native resource parsing, queue/order and display glue."""
from pathlib import Path
import subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[2]
MUTANTS=[('linked(w,list,a,0x2e) || linked(w,list,a,0x2c)','linked(w,list,a,0x2c) || linked(w,list,a,0x2a)'),('s.y-gw(w,0x14e)','s.y+gw(w,0x14e)'),('iw16(coordinates+i*4+2)','iw16(coordinates+i*4)'),('v->objects=p->objects+32','v->objects=p->objects')]
def run(mutation=None):
    with tempfile.TemporaryDirectory(prefix='intro-presentation-') as directory:
        directory=Path(directory);source=ROOT/'port/intro/presentation.c'
        if mutation:
            old,new=mutation;text=source.read_text();assert text.count(old)==1
            source=directory/'presentation.c';source.write_text(text.replace(old,new))
        binary=directory/'test'
        subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-fsanitize=undefined','-fno-sanitize-recover=undefined','-I'+str(ROOT/'port/intro'),str(source),*[str(ROOT/'port/intro'/p) for p in ['world.c','particles.c','sprites.c','tests/presentation_test.c']],'-o',str(binary)],check=True,timeout=20)
        return subprocess.run([str(binary)],capture_output=True,timeout=5)
class PresentationControls(unittest.TestCase):
    def test_control(self):
        result=run();self.assertEqual(result.returncode,0,result.stderr.decode())
    def test_mutations(self):
        for mutation in MUTANTS:
            with self.subTest(mutation=mutation):self.assertNotEqual(run(mutation).returncode,0)
if __name__=='__main__':unittest.main()
