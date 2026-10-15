"""Seconds, generated data only: legacy importer/scene cores and GPU upload contract.

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
from unittest.mock import patch
from intro_particles_support import Motion, tables

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/intro'))
import import_scene as imp

SEQ = 102
FORE = SEQ + 1152
BACK = FORE + 6*384*144
PORTRAIT = BACK + 4*256*256*2

class Sequence(C.Structure):
    _fields_ = [('record', C.c_uint32), ('remaining', C.c_uint16), ('flags', C.c_uint16)]
class Ember(C.Structure):
    _fields_ = [('motion', Motion), ('sequence', Sequence), ('state', C.c_uint8), ('subtype', C.c_uint8)]
class Embers(C.Structure):
    _fields_ = [('data', C.c_void_p), ('slots', Ember*39), ('seed', C.c_uint16),
                ('interval', C.c_uint16), ('available', C.c_uint16), ('free_slots', C.c_uint8*39)]
class Scene(C.Structure):
    _fields_ = [('data', C.c_void_p), ('foreground', Sequence), ('tick', C.c_uint), ('embers', Embers)]


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


def ember_fixture():
    p=bytearray(imp.EMBER_SIZE)
    for i in range(12): struct.pack_into('>II',p,i*8,0x10000,0)
    for i in range(128): struct.pack_into('>hh',p,96+i*4,100,100)
    for kind in range(6):
        start,count=12+kind*100,5 if kind==0 else 6
        struct.pack_into('>H',p,608+kind*2,start)
        for i in range(count):
            struct.pack_into('>HHI',p,608+start+i*16,4095 if i+1==count else 8,
                             0x8000 if i+1==count else 0,kind*6+i)
        struct.pack_into('>I',p,608+start+count*16,start+(count-1)*16)
    p[1220:]=b'\1'*(36*256)
    p[1220+5*256:1220+6*256]=b'\x0f'*256
    return p


def pack(payload):
    magic=b'SFA2I002' if len(payload)==imp.PAYLOAD_SIZE+imp.EMBER_SIZE else imp.MAGIC
    return magic + hashlib.sha256(payload).digest() + payload

class IntroScene(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory()
        cls.root = Path(cls.temp.name)
        lib = cls.root / 'scene.so'
        subprocess.run(['gcc', '-std=c11', '-O1', '-Wall', '-Wextra', '-Werror', '-shared', '-fPIC',
            '-I', str(ROOT / 'port/src'), str(ROOT / 'port/intro/scene.c'),
            str(ROOT / 'port/intro/sequence.c'), str(ROOT / 'port/intro/particles.c'),
            str(ROOT / 'port/intro/embers.c'), '-o', str(lib)], check=True, timeout=15)
        cls.lib = C.CDLL(str(lib))
        cls.lib.intro_scene_init.argtypes = [C.POINTER(Scene), C.c_void_p, C.c_size_t]
        cls.lib.intro_scene_frame.argtypes = [C.POINTER(Scene), C.c_void_p]
        cls.lib.intro_embers_frame.argtypes = [C.POINTER(Embers),C.c_void_p,C.c_void_p]

    @classmethod
    def tearDownClass(cls):
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

    def test_validation_atomic(self):
        p=fixture();state=Scene();data=C.create_string_buffer(bytes(p))
        self.assertEqual(self.lib.intro_scene_init(C.byref(state),data,len(p)),0)
        before=bytes(state)
        for at,value in ((0,1),(4,1),(9,7),(98,1),(SEQ,0xff),(FORE,16),(BACK,2),(PORTRAIT,16)):
            bad=bytearray(p);bad[at]=value;buf=C.create_string_buffer(bytes(bad))
            self.assertEqual(self.lib.intro_scene_init(C.byref(state),buf,len(bad)),-1,(at,value))
            self.assertEqual(bytes(state),before)
        self.assertEqual(self.lib.intro_scene_init(C.byref(state),data,len(p)-1),-1)
    def test_particle_import_layout(self):
        rom=bytearray(0x240000);gfx=bytearray(0x10001*128);opcodes=bytearray(0x300000)
        opcodes[0x73d9a:0x73dfa]=tables()[:96];opcodes[0x73e10:0x74010]=tables()[96:]
        struct.pack_into('>I',rom,0x9f260,0xc0000);struct.pack_into('>hh',rom,0xc0000,-8,-8)
        for kind in range(6):
            start=0x229200+kind*100;count=5 if kind==0 else 6
            struct.pack_into('>h',rom,0x229122+kind*2,start-0x229120)
            for i in range(count):
                struct.pack_into('>HHIII',rom,start+i*16,4095 if i+1==count else 8,
                                 0x8000 if i+1==count else 0,0x230000,0,0)
            struct.pack_into('>I',rom,start+count*16,start+(count-1)*16)
        struct.pack_into('>5H',rom,0x230000,0,1,0,0,0)
        with self.assertRaisesRegex(ValueError,'unsupported opcode'): imp.embers(rom,gfx,opcodes)
        with patch.object(imp,'OPCODE_SHA',hashlib.sha256(opcodes).hexdigest()):
            data=imp.embers(rom,gfx,opcodes)
            self.assertEqual(len(data),imp.EMBER_SIZE)
            self.assertEqual(data[:608],tables())
            self.assertEqual(struct.unpack_from('>I',data,608+12+80)[0],76)
            self.assertEqual(data[1220:1220+256],bytes(256))
            self.assertEqual(data[1220+5*256:1220+6*256],bytes([15])*256)
            for at,value,why in ((0x230000,1,'layout'),(0x230004,1,'layout'),
                                 (0xc0000,0,'anchor'),(0x229200+80,1,'hold target')):
                bad=bytearray(rom);bad[at]=value
                with self.assertRaisesRegex(ValueError,why):imp.embers(bad,gfx,opcodes)

    def test_embers_lifecycle_and_priority(self):
        p=fixture()+ember_fixture();state=Scene()
        struct.pack_into('>H',p,SEQ+2,0xf00)
        struct.pack_into('>H',p,SEQ+4,0x00f)
        for pose in range(6):p[FORE+pose*384*144+(140-80)*384+100]=2
        data=C.create_string_buffer(bytes(p));pixels=(C.c_uint16*(384*224))()
        self.assertEqual(self.lib.intro_scene_init(C.byref(state),data,len(p)),0)
        self.assertEqual((state.embers.seed,state.embers.interval,state.embers.available),(0x1234,4,39))
        for _ in range(3):
            self.assertEqual(self.lib.intro_scene_frame(C.byref(state),pixels),1)
            self.assertEqual(pixels[140*384+100],0x7c00)
        self.assertEqual(self.lib.intro_scene_frame(C.byref(state),pixels),1)
        self.assertEqual(pixels[140*384+100],0x001f)
        e=state.embers.slots[38]
        self.assertEqual((e.state,e.motion.x,e.motion.y),(2,100<<16,100<<16))
        self.assertEqual(state.embers.available,38)
        state.embers.interval=1000
        self.lib.intro_scene_frame(C.byref(state),pixels)
        self.assertEqual(e.motion.x,101<<16)
        e.motion.x=417<<16
        self.lib.intro_scene_frame(C.byref(state),pixels)
        self.assertEqual((e.state,state.embers.available),(6,38))
        self.lib.intro_scene_frame(C.byref(state),pixels)
        self.assertEqual((e.state,state.embers.available),(0,39))
        self.assertEqual(state.embers.free_slots[38],38)
        state.embers.interval=1
        colors=(C.c_uint16*16)(*range(16))
        for _ in range(512):
            self.assertEqual(self.lib.intro_embers_frame(C.byref(state.embers),pixels,colors),0)
            free=state.embers.available
            self.assertGreaterEqual(free,5)
            self.assertEqual(free+sum(e.state!=0 for e in state.embers.slots),39)
            self.assertEqual(len(set(state.embers.free_slots[:free])),free)
            self.assertTrue(all(state.embers.slots[i].state==0 for i in state.embers.free_slots[:free]))

    def test_particle_pack_validation(self):
        p=fixture()+ember_fixture();data=C.create_string_buffer(bytes(p));state=Scene()
        self.assertEqual(self.lib.intro_scene_init(C.byref(state),data,len(p)),0)
        before=bytes(state)
        for at,value in ((608,1),(608+12,1),(608+12+7,255),(608+12+84,1),
                         (1220,16),(1220+5*256,1)):
            bad=bytearray(p);bad[imp.PAYLOAD_SIZE+at]=value;buf=C.create_string_buffer(bytes(bad))
            self.assertEqual(self.lib.intro_scene_init(C.byref(state),buf,len(bad)),-1)
            self.assertEqual(bytes(state),before)
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
