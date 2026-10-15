#!/usr/bin/env python3
"""Seconds, synthetic native map wrapping/position controls; no private inputs."""
from pathlib import Path
import subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[2]
class MapControls(unittest.TestCase):
    def test_native_map_boundaries(self):
        with tempfile.TemporaryDirectory(prefix='intro-maps-') as directory:
            binary=Path(directory)/'maps'
            subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-fsanitize=undefined','-fno-sanitize-recover=undefined',*[str(ROOT/'port/intro'/p) for p in ['maps.c','world.c','particles.c','tests/maps_test.c']],'-o',str(binary)],check=True,timeout=20)
            subprocess.run([str(binary)],check=True,timeout=5)
    def test_real_mutants(self):
        source=(ROOT/'port/intro/maps.c').read_text()
        for old,new in [('column_steps[]={128,64,32}','column_steps[]={64,64,32}'),('y=~y','y=0u-y'),('blank=layer==2 ? 0 : 32','blank=32')]:
            with self.subTest(mutation=old),tempfile.TemporaryDirectory(prefix='intro-maps-') as directory:
                self.assertEqual(source.count(old),1)
                directory=Path(directory);unit=directory/'maps.c';unit.write_text(source.replace(old,new));binary=directory/'mutant'
                subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-I'+str(ROOT/'port/intro'),str(unit),*[str(ROOT/'port/intro'/p) for p in ['world.c','particles.c','tests/maps_test.c']],'-o',str(binary)],check=True,timeout=20)
                result=subprocess.run([str(binary)],stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=5)
                self.assertNotEqual(result.returncode,0)
if __name__=='__main__':unittest.main()
