"""Seconds, generated data only: importer, native scene, pack and opening hook.

No original ROM, game, GPU/window or audio. Compiles just the affected feature.
Unchanged selector/tick oracle evidence belongs to test_intro_sequence.py.
"""
import ctypes as C
import hashlib
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/intro'))
import import_scene as imp

SEQ = 102
FORE = SEQ + 1152
BACK = FORE + 6*384*144
PORTRAIT = BACK + 4*256*256*2

class Sequence(C.Structure):
    _fields_ = [('record', C.c_uint32), ('remaining', C.c_uint16), ('flags', C.c_uint16)]
class Scene(C.Structure):
    _fields_ = [('data', C.c_void_p), ('foreground', Sequence), ('tick', C.c_uint)]


def fixture():
    p = bytearray(imp.PAYLOAD_SIZE)
    struct.pack_into('>H', p, 0, 2)
    for i in range(6):
        struct.pack_into('>HHI', p, 2+i*16, 4, 0x8000 if i == 5 else 0, i)
    struct.pack_into('>I', p, 98, 2)
    p[FORE:BACK] = b'\x0f'*(BACK-FORE)
    p[BACK:PORTRAIT] = b'\0\x0f'*((PORTRAIT-BACK)//2)
    p[PORTRAIT:] = b'\x0f'*(len(p)-PORTRAIT)
    return p


def pack(payload):
    return imp.MAGIC + hashlib.sha256(payload).digest() + payload

STUBS = r'''
#include "port.h"
#include <stdlib.h>
#include <string.h>
unsigned uploaded,begin_count,end_count,frames,skip;
void port_halt(int code,const char *fmt,...) { (void)code;(void)fmt;abort(); }
int port_gpu_intro_begin(void) { begin_count++;return 0; }
void port_gpu_intro_frame(const uint16_t *p) { (void)p;uploaded++; }
void port_gpu_intro_end(void) { end_count++; }
void port_pad_host_init(void) {}
void port_pad_host_read(unsigned char *p,int n) { memset(p,255,n);p[0]=0;p[1]=0x41;if(skip)p[2]&=~8; }
void port_input_apply(unsigned char *p,unsigned n) { (void)p;(void)n; }
void port_clock_frame_drawn(unsigned n) { (void)n; }
unsigned port_frames(void) { return frames; }
void port_tick(void) { frames++; }
'''

class IntroScene(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory()
        cls.root = Path(cls.temp.name)
        (cls.root / 'stubs.c').write_text(STUBS)
        lib = cls.root / 'scene.so'
        subprocess.run(['gcc', '-std=c11', '-O1', '-Wall', '-Wextra', '-Werror', '-shared', '-fPIC',
            '-I', str(ROOT / 'port/src'), str(ROOT / 'port/intro/scene.c'),
            str(ROOT / 'port/intro/sequence.c'), str(ROOT / 'port/src/intro.c'),
            str(ROOT / 'port/src/sha256.c'), str(cls.root / 'stubs.c'), '-o', str(lib)], check=True, timeout=15)
        cls.lib = C.CDLL(str(lib))
        cls.lib.intro_scene_init.argtypes = [C.POINTER(Scene), C.c_void_p, C.c_size_t]
        cls.lib.intro_scene_frame.argtypes = [C.POINTER(Scene), C.c_void_p]
        cls.lib.port_intro_configure.argtypes = [C.c_int, C.c_char_p, C.c_void_p, C.c_size_t]
        cls.lib.port_intro_program.argtypes = [C.c_char_p]

    @classmethod
    def tearDownClass(cls):
        cls.lib.port_intro_configure(0, b'', None, 0)
        cls.temp.cleanup()

    def test_import_safeguards(self):
        unknown=self.root/"unknown.zip"
        unknown.write_bytes(b"not a supported archive")
        with self.assertRaisesRegex(ValueError,"unsupported arcade archive"): imp.import_zip(unknown)
        with self.assertRaisesRegex(ValueError,"private assets"): imp.private_output(ROOT/"port/intro/output.dat")
        self.assertEqual(imp.private_output(ROOT/"port/local/intro-asset-probe/output.dat"), ROOT/"port/local/intro-asset-probe/output.dat")

    def test_lanes_unshuffle_and_planar_pixels(self):
        self.assertEqual(imp.lanes([bytes((i,i+1,i+8,i+9)) for i in (0,2,4,6)]), bytes(range(16)))
        self.assertEqual(imp.unshuffle(bytes(range(32)),32), bytes(list(range(8))+list(range(16,24))+list(range(8,16))+list(range(24,32))))
        for side in (16,32):
            source = bytes((x+3*y)%16 for y in range(side) for x in range(side))
            planar = bytearray(side*side//2)
            for y in range(side):
                for x in range(side):
                    for bit in range(4):
                        planar[y*side//2+x//8*4+bit] |= ((source[y*side+x]>>bit)&1)<<(7-x%8)
            self.assertEqual(imp.tile(planar,0,side),source)
            with self.assertRaises(ValueError): imp.tile(planar,1,side)
        with self.assertRaises(ValueError): imp.lanes([b'odd']*4)
        with self.assertRaises(ValueError): imp.unshuffle(bytes(17),16)

    def test_foreground_whole_block_flips(self):
        # Generated two-by-two block crosses low-nibble tile boundary.
        rom = bytearray(0x9f268)
        struct.pack_into('>4H',rom,0,2,4,0,0)
        struct.pack_into('>I',rom,0x9f260,32)
        struct.pack_into('>hh',rom,32,-192,-144)
        gfx = bytearray((0x10020)*128)
        # Codes 15,0,31,16: distinguish both tile order and within-tile flip.
        for number,code in enumerate((15,0,31,16)):
            a=(0x10000+code)*128
            for y in range(16):
                for x in range(16):
                    v=number*2+(x==0 and y==0)
                    for bit in range(4): gfx[a+y*8+x//8*4+bit] |= ((v>>bit)&1)<<(7-x%8)
        for flip in (0,32,64,96):
            struct.pack_into('>HH',rom,8,15,0x1100|flip)
            out=imp.foreground(rom,gfx,0)
            for y in range(32):
                for x in range(32):
                    sx,sy=(31-x if flip&32 else x),(31-y if flip&64 else y)
                    expected=(sy//16*2+sx//16)*2+(sx%16==0 and sy%16==0)
                    self.assertEqual(out[y*384+x],expected)
            self.assertEqual(out[383],15)

    def test_scene_clocks_and_layer_priority(self):
        p=fixture()
        # Sprite red, map green, portrait banks blue/red/green.
        struct.pack_into('>H',p,SEQ+2,0xf00)
        struct.pack_into('>H',p,SEQ+32+2,0x0f0)
        for i,c in enumerate((0x00f,0xf00,0x0f0)):
            struct.pack_into('>H',p,SEQ+32+1024+(i*16+1)*2,c)
        for f in range(6):
            p[FORE+f*384*144+(144-1)*384+f]=1
        # Map sample y=16 at tick0, x=0 and a different frame-1 marker.
        struct.pack_into('>H',p,BACK+16*256*2,1)
        struct.pack_into('>H',p,BACK+256*256*2+(16*256+1)*2,1)
        p[PORTRAIT+2]=1
        data=C.create_string_buffer(bytes(p));state=Scene()
        self.assertEqual(self.lib.intro_scene_init(C.byref(state),data,len(p)),0)
        pixels=(C.c_uint16*(384*224))()
        for tick in range(192):
            self.assertEqual(state.foreground.record,2+(tick//4%6)*16)
            self.assertEqual(self.lib.intro_scene_frame(C.byref(state),pixels),1)
            self.assertEqual(pixels[2],(0x7c00,0x001f,0x03e0,0x001f)[tick%4])
            self.assertEqual(pixels[223*384+tick//4%6],0x001f)
            if tick in (0,6): self.assertEqual(pixels[0],0x03e0)
        old=bytes(pixels)
        self.assertEqual(self.lib.intro_scene_frame(C.byref(state),pixels),0)
        self.assertEqual(bytes(pixels),old)

    def test_validation_atomic_and_pack_runtime(self):
        p=fixture();state=Scene();data=C.create_string_buffer(bytes(p))
        self.assertEqual(self.lib.intro_scene_init(C.byref(state),data,len(p)),0)
        before=bytes(state)
        for at,value in ((0,1),(4,1),(9,7),(98,1),(SEQ,0xff),(FORE,16),(BACK,2),(PORTRAIT,16)):
            bad=bytearray(p);bad[at]=value;buf=C.create_string_buffer(bytes(bad))
            self.assertEqual(self.lib.intro_scene_init(C.byref(state),buf,len(bad)),-1,(at,value))
            self.assertEqual(bytes(state),before)
        self.assertEqual(self.lib.intro_scene_init(C.byref(state),data,len(p)-1),-1)
        err=C.create_string_buffer(256);path=self.root/'scene.dat'
        self.assertEqual(self.lib.port_intro_configure(0,b'/missing',err,len(err)),0)
        self.assertEqual(self.lib.port_intro_program(b'z2o'),0)
        self.assertEqual(self.lib.port_intro_configure(1,b'/missing',err,len(err)),-1)
        valid=pack(p)
        for content in (valid[:-1],valid+b'x',b'badmagic'+valid[8:],valid[:80]+b'X'+valid[81:],pack(p[:FORE]+b'\x10'+p[FORE+1:])):
            path.write_bytes(content)
            self.assertEqual(self.lib.port_intro_configure(1,str(path).encode(),err,len(err)),-1)
        path.write_bytes(valid)
        self.assertEqual(self.lib.port_intro_configure(1,str(path).encode(),err,len(err)),0,err.value)
        self.assertEqual(self.lib.port_intro_program(b'logo'),0)
        before=C.c_uint.in_dll(self.lib,'uploaded').value
        self.assertEqual(self.lib.port_intro_program(b'z2o'),1)
        self.assertEqual(C.c_uint.in_dll(self.lib,'uploaded').value-before,192)
        self.assertEqual(C.c_uint.in_dll(self.lib,'end_count').value,C.c_uint.in_dll(self.lib,'begin_count').value)
        C.c_uint.in_dll(self.lib,'skip').value=1
        self.assertEqual(self.lib.port_intro_program(b'z2o'),1)
        self.assertEqual(C.c_uint.in_dll(self.lib,'uploaded').value-before,193)
        self.lib.port_intro_configure(0,b'/missing',err,len(err))
        self.assertEqual(self.lib.port_intro_program(b'z2o'),0)

    def test_gpu_upload_contract(self):
        # Compile the actual three new GPU functions against command stubs.
        source=(ROOT/'port/src/gpu.c').read_text().split('/* Host-owned pixels',1)[1]
        source=source[source.index('int port_gpu_intro_begin'):source.index('static DISPENV *host_PutDispEnv')]
        stubs=r'''
#include <assert.h>
#include <stdint.h>
#include "video.h"
typedef unsigned long u_long;
typedef struct {short x,y,w,h;} RECT;
typedef struct {RECT disp;} DISPENV;
static int available=1,reset,ui_canvas_active,display_on,canvas,upload,flush;
static const uint16_t *expected;
int Psyz_Sfa2ExperimentalVideoAvailable(void) { return available; }
void host_ResetGraph(int n) { assert(n==0);reset++; }
void SetDefDispEnv(DISPENV *e,int x,int y,int w,int h) { *e=(DISPENV){{x,y,w,h}}; }
void PutDispEnv(DISPENV *e) { assert(e->disp.x==0 && e->disp.y==0 && e->disp.w==368 && e->disp.h==240); }
void Psyz_Sfa2SetArcadeCanvas(int n) { canvas=n; }
void LoadImage(RECT *r,u_long *p) { assert(r->x==0 && r->y==8 && r->w==384 && r->h==224);assert((void *)p==expected);upload++; }
void Psyz_GpuExeque(void) { flush++; }
void SetDispMask(int n) { display_on=n; }
'''
        main=r'''
int main(void) {
 uint16_t pixel=0;expected=&pixel;
 available=0;assert(port_gpu_intro_begin()==-1 && !reset);
 available=1;assert(!port_gpu_intro_begin() && reset==1 && canvas && ui_canvas_active);
 port_gpu_intro_frame(&pixel);assert(upload==1 && flush==1 && display_on);
 port_gpu_intro_end();assert(!display_on && !canvas && !ui_canvas_active);
 return 0;
}
'''
        file=self.root/'gpu.c';exe=self.root/'gpu';file.write_text(stubs+source+main)
        subprocess.run(['gcc','-std=c11','-Wall','-Wextra','-Werror','-I',str(ROOT/'port/src'),str(file),'-o',str(exe)],check=True,timeout=15)
        subprocess.run([str(exe)],check=True,timeout=5)

if __name__=='__main__': unittest.main()
