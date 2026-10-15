#!/usr/bin/env python3
"""Seconds, no private input: original-computed synthetic native-state fixtures.

Covers only the new world/director and the listed recovered actor handlers,
not the unfinished full intro integration, sound or other actor families.
"""
import ctypes as C
import json
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT=Path(__file__).resolve().parents[2]
SOURCES=['world.c','director.c','actors.c','actor_portraits.c','actor_montage.c','particles.c','tests/director_bridge.c']
MUTANTS={
    'row_wrap':('director.c','(gw(w,IG_ROW)-1u)&511u','(gw(w,IG_ROW)-1u)&255u'),
    'double_update':('actors.c','tick_collect(w,a,1);break;','/* deliberately omitted inner update */ break;'),
    'signed_boundary':('actors.c','is16(iw16(a+IA_X))<=-64','is16(iw16(a+IA_X))<-64'),
    'headshot_word_alias':('director.c','sl(w,IG_FLAGS,2)','sl(w,IG_FLAGS,0)'),
}
def patch(buffer,ranges):
    for address,hex_bytes in ranges:
        address=int(address,16);data=bytes.fromhex(hex_bytes)
        if address+len(data)>len(buffer):raise AssertionError('fixture range')
        buffer[address:address+len(data)]=data

def check_fixtures(directory,mutation=None):
    units=[]
    for relative in SOURCES:
        path=ROOT/'port/intro'/relative
        if mutation and relative==MUTANTS[mutation][0]:
            _,old,new=MUTANTS[mutation];source=path.read_text()
            if source.count(old)!=1:raise AssertionError('non-unique mutation '+mutation)
            path=directory/relative;path.write_text(source.replace(old,new))
        units.append(str(path))
    binary=directory/'native.so'
    subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-shared','-fPIC',
                    '-fsanitize=undefined','-fno-sanitize-recover=undefined',
                    '-I'+str(ROOT/'port/intro'),*units,'-o',str(binary)],check=True,timeout=30)
    dll=C.CDLL(str(binary));dll.intro_director_test_inputs.argtypes=[C.c_void_p,C.c_size_t,C.c_void_p,C.c_size_t]
    dll.intro_director_test_run.argtypes=[C.c_void_p,C.c_void_p,C.c_uint];dll.intro_director_test_run.restype=C.c_char_p
    dll.intro_actor_test_run.argtypes=[C.c_void_p,C.c_void_p,C.c_uint,C.c_uint16];dll.intro_actor_test_run.restype=C.c_char_p
    zeros=C.create_string_buffer(bytes(128));dll.intro_director_test_inputs(zeros,128,None,0)
    fixture=json.loads((ROOT/'port/intro/native-fixtures.json').read_text());base=bytearray(65536);patch(base,fixture['base']);failed=[]
    for case in fixture['cases']:
        before=bytearray(base);patch(before,case['before']);expected=bytearray(before);patch(expected,case['after'])
        expected_video=bytearray(0x40000);patch(expected_video,case['video'])
        ram=C.create_string_buffer(bytes(before));video=C.create_string_buffer(bytes(0x40000))
        if case['kind']==2:err=dll.intro_actor_test_run(ram,video,case['pool'],int(case['actor'],16))
        else:err=dll.intro_director_test_run(ram,video,case['kind'])
        if err or ram.raw[:0xf000]!=expected[:0xf000] or video.raw[:0x40000]!=expected_video:
            failed.append((case['name'],err.decode() if err else 'state differs'))
    return failed

class NativeControls(unittest.TestCase):
    def test_synthetic_fixtures(self):
        with tempfile.TemporaryDirectory(prefix='intro-native-') as path:
            self.assertEqual(check_fixtures(Path(path)),[])
    def test_nontrivial_mutants(self):
        for mutation in MUTANTS:
            with self.subTest(mutation=mutation),tempfile.TemporaryDirectory(prefix='intro-native-') as path:
                self.assertTrue(check_fixtures(Path(path),mutation),'undetected '+mutation)
if __name__=='__main__':unittest.main()
