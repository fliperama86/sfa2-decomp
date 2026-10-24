#!/usr/bin/env python3
"""Seconds: normal opening hook, full-pack validation, skip/return and audio lifetime.
Native player/presentation/audio are explicit stand-ins, not asserted by this test.
"""
import ctypes as C
import hashlib
from pathlib import Path
import subprocess,sys,tempfile,unittest
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'port/intro'))
import import_full
STUBS=r'''
#include "port.h"
#include "../intro/player.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
unsigned uploaded,begins,ends,frames,skip,audio_starts,audio_stops,prepared,fade;
void port_halt(int code,const char *fmt,...) {(void)code;(void)fmt;abort();}
int port_gpu_intro_begin(void) {begins++;return 0;}
void port_gpu_intro_frame(const uint16_t *p) {assert(p[0]==123);uploaded++;}
void port_gpu_intro_end(void) {ends++;}
void port_pad_host_init(void) {}
void port_pad_host_read(unsigned char *p,int n) {memset(p,255,n);p[0]=0;p[1]=0x41;if(skip)p[2]&=~8;}
void port_input_apply(unsigned char *p,unsigned n) {(void)p;(void)n;}
void port_clock_frame_drawn(unsigned n) {(void)n;}
unsigned port_frames(void) {return frames;}
void port_tick(void) {frames++;}
int port_intro_audio_prepare(char *err,size_t size) {(void)err;(void)size;prepared++;return 0;}
int port_arcade_enabled(void) {return 0;}   /* the arcade loader is off here: the PS1 soundtrack's path */
int port_arcade_sound(unsigned code) {(void)code;return 0;}
void port_arcade_sound_stop(void) {}
unsigned port_alive(void) {return 1;}   /* main.c: the game is running */
void port_intro_audio_start(void) {audio_starts++;}
void port_intro_audio_stop(void) {audio_stops++;}
void port_intro_audio_gain(unsigned gain) {assert(gain<=65536);fade++;}
void intro_player_init(struct intro_player *p,intro_table_reader read,void *context,uint16_t seed)
{assert(seed==0x1234 && read(context,0,0x1234,2));memset(p,0,sizeof *p);}
int intro_player_step(struct intro_player *p)
{
 p->ticks++;p->world.sound_command=p->ticks==1 ? 6 : 0;
 if(p->ticks>=2584) {sw(&p->world,IG_SCENE,14);sw(&p->world,IG_STEP,10);ip16(p->world.video+0xc000,(2600-p->ticks)*0x1000u);}
 return p->ticks==2600;
}
int intro_player_render(struct intro_player *p,intro_video_tile tile,void *context,uint16_t *pixels,size_t count)
{(void)p;assert(count==384*224 && tile(context,8,1,1));pixels[0]=123;return 0;}
'''
class RuntimeControls(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=tempfile.TemporaryDirectory(prefix='intro-runtime-');cls.root=Path(cls.temp.name)
        (cls.root/'stubs.c').write_text(STUBS);binary=cls.root/'runtime.so'
        subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-fPIC','-shared','-fsanitize=undefined','-fno-sanitize-recover=undefined','-I'+str(ROOT/'port/src'),str(cls.root/'stubs.c'),*[str(ROOT/x) for x in ['port/src/intro.c','port/intro/resources.c','port/src/sha256.c']],'-o',str(binary)],check=True,timeout=20)
        cls.dll=C.CDLL(str(binary));cls.dll.port_intro_configure.argtypes=[C.c_int,C.c_char_p,C.c_void_p,C.c_size_t];cls.dll.port_intro_program.argtypes=[C.c_char_p]
    @classmethod
    def tearDownClass(cls):cls.temp.cleanup()
    def tearDown(self):self.dll.port_intro_configure(0,None,None,0)
    def count(self,name):return C.c_uint.in_dll(self.dll,name)
    def test_full_lifecycle(self):
        path=self.root/'full.dat';path.write_bytes(import_full.encode([(0,0x1234,b'abc')],[(0x08000003,bytes([15])*64)]));err=C.create_string_buffer(256)
        self.assertEqual(self.dll.port_intro_configure(1,str(path).encode(),err,len(err)),0,err.value)
        self.assertEqual(self.dll.port_intro_program(b'logo'),0);before=self.count('uploaded').value
        self.count('skip').value=0;self.assertEqual(self.dll.port_intro_program(b'z2o'),1)
        self.assertEqual(self.count('uploaded').value-before,2600)
        self.assertEqual(self.count('audio_starts').value,self.count('audio_stops').value)
        self.assertEqual(self.count('begins').value,self.count('ends').value);self.assertGreater(self.count('fade').value,0)
        self.count('skip').value=1;self.assertEqual(self.dll.port_intro_program(b'z2o'),1)
        self.assertEqual(self.count('uploaded').value-before,2601)
        self.assertEqual(self.count('audio_starts').value,self.count('audio_stops').value)
    def test_disabled_and_rejections(self):
        err=C.create_string_buffer(256);path=self.root/'bad.dat'
        self.assertEqual(self.dll.port_intro_configure(0,b'/missing',err,len(err)),0)
        self.assertEqual(self.dll.port_intro_program(b'z2o'),0)
        self.assertEqual(self.dll.port_intro_configure(1,b'/missing',err,len(err)),-1)
        valid=import_full.encode([(0,0x1234,b'abc')],[(0x08000003,bytes([15])*64)])
        for bad in [valid[:-1],valid+b'x',b'SFA2I002'+valid[8:],valid[:80]+b'X'+valid[81:]]:
            path.write_bytes(bad);self.assertEqual(self.dll.port_intro_configure(1,str(path).encode(),err,len(err)),-1)
        path.write_bytes(b'SFA2I002'+valid[8:]);self.dll.port_intro_configure(1,str(path).encode(),err,len(err));self.assertIn(b're-import',err.value)
if __name__=='__main__':unittest.main()
