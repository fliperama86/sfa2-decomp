"""Seconds, particle C plus original-computed synthetic fixtures. No game/ROM."""
from pathlib import Path
import sys
import unittest
from unittest.mock import patch
from intro_particles_support import INTRO, Motion, MUTANTS, fixtures, invoke, native
sys.path.insert(0,str(INTRO))
import verify_particles as verify

class Particles(unittest.TestCase):
    def test_original_computed_fixtures(self):
        with native() as dll:
            for case in fixtures():
                with self.subTest(case=case['name']): self.assertEqual(invoke(dll,case),case['after'])
            for subtype in (0,7,0xffffffff):
                c=dict(op='init',seed=0x1234,subtype=subtype,motion=list(range(7)))
                self.assertEqual(invoke(dll,c),dict(result=-1,seed=0x1234,motion=list(range(7))))

    def test_negative_controls(self):
        for op,old,new in MUTANTS:
            with native((old,new)) as dll:
                self.assertTrue(any(invoke(dll,c)!=c['after'] for c in fixtures() if c['op']==op),op)

    def test_oracle_input_guards(self):
        with self.assertRaises(ValueError):verify.checked_opcodes(b'unknown revision')
        with self.assertRaises(ValueError):verify.output_path(INTRO/'public-report.json')
        with patch.object(Path,'exists',return_value=True):
            with self.assertRaises(ValueError):verify.output_path(verify.ROOT/'port/local/existing-report.json')
        self.assertTrue(verify.inside(10,4,[(10,14)]))
        self.assertFalse(verify.inside(10,5,[(10,14)]))
        self.assertFalse(verify.inside(9,1,[(10,14)]))

if __name__=='__main__':unittest.main()
