"""Seconds: original-computed synthetic fixtures, bounds and real mutations.

No ROM, Unicorn, game, graphics window or audio. Only sprites.c is compiled.
"""
import ctypes as C
import unittest
import sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/"intro"))
from verify_sprites import Oracle,START,END
from intro_sprites_support import Art,Block,List,Style,MUTATIONS,fixtures,invoke,native


class SpriteTest(unittest.TestCase):
    def test_original_computed_fixtures(self):
        with native() as dll:
            for case in fixtures():
                with self.subTest(case=case['name']):
                    self.assertEqual(invoke(dll,case),case['after'])

    def test_rejection_is_atomic(self):
        with native() as dll:
            block=(Block*1)(Block(0,0,0,0))
            art=Art(0,1,1,block);style=Style();output=C.create_string_buffer(b'\xa5'*32)
            state=List(C.addressof(output),4,2,2)
            before=bytes(output)
            for subject,field,value in [(art,'mode',1),(art,'blocks',897),(art,'cells',0),
                                        (state,'capacity',897),(state,'count',5),(state,'cells_left',897),
                                        (state,'capacity',2)]:
                old=getattr(subject,field);setattr(subject,field,value)
                saved=bytes(state)
                self.assertEqual(dll.intro_sprites_append(C.byref(state),C.byref(art),C.byref(style)),-1)
                self.assertEqual(bytes(state),saved);self.assertEqual(bytes(output),before)
                setattr(subject,field,old)
            art.mode=8;style.bypass=1
            self.assertEqual(dll.intro_sprites_append(C.byref(state),C.byref(art),C.byref(style)),-1)
            self.assertEqual(bytes(output),before)

    def test_oracle_audit_guards(self):
        oracle=object.__new__(Oracle)
        oracle.readable=[(100,104)];oracle.writable=[(200,204)];oracle.writes=0
        oracle.instruction(None,START,2,None)
        with self.assertRaises(AssertionError):oracle.instruction(None,START-2,2,None)
        with self.assertRaises(AssertionError):oracle.instruction(None,END,2,None)
        with self.assertRaises(AssertionError):oracle.read(None,0,103,2,0,None)
        with self.assertRaises(AssertionError):oracle.write(None,0,204,1,0,None)
        oracle.write(None,0,200,4,0,None);self.assertEqual(oracle.writes,1)

    def test_negative_controls(self):
        fixed=fixtures()
        for mutation in MUTATIONS:
            with self.subTest(mutation=mutation),native(mutation) as dll:
                self.assertTrue(any(invoke(dll,c)!=c['after'] for c in fixed))


if __name__=='__main__':
    unittest.main()
