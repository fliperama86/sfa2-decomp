#!/usr/bin/env python3
"""Seconds: isolated map consumers with synthetic tile pairs and write audit.

Private pinned original opcodes only. No game data, game boot or runtime use.
"""
import argparse,ctypes as C,hashlib,json,random,struct,subprocess,tempfile
from pathlib import Path
from verify_sequence import OPCODE_SHA256,output_path
ROOT=Path(__file__).resolve().parents[2]
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--opcodes',type=Path,required=True);p.add_argument('--seed',type=int,default=0);p.add_argument('--cases',type=int,default=2000);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    if not 1<=a.cases<=24000:p.error('cases must be 1..24000')
    destination=output_path(a.output);opcodes=a.opcodes.read_bytes()
    if hashlib.sha256(opcodes).hexdigest()!=OPCODE_SHA256:raise ValueError('not the pinned opcode image')
    from unicorn import Uc,UC_ARCH_M68K,UC_MODE_BIG_ENDIAN,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
    from unicorn import m68k_const as m
    u=Uc(UC_ARCH_M68K,UC_MODE_BIG_ENDIAN);u.ctl_set_cpu_model(m.UC_CPU_M68K_M68000)
    for address,size in [(0x18000,0x1000),(0x200000,0x5000),(0x900000,0x40000),(0xff0000,0x10000)]:u.mem_map(address,size)
    u.mem_write(0x18000,opcodes[0x18000:0x19000]);writes=0;source_size=0;base=0
    def instruction(u,address,n,context):
        if not 0x18ede<=address<0x18ff8:raise AssertionError(f'PC escaped {address:x}')
    def read(u,access,address,n,value,context):
        if not any(lo<=address and address+n<=hi for lo,hi in [(0x200000,0x200000+source_size),(0xfff800,0xfff804)]):raise AssertionError(f'read escaped {address:x}+{n}')
    def write(u,access,address,n,value,context):
        nonlocal writes
        if not (base<=address and address+n<=base+0x4000):raise AssertionError(f'write escaped {address:x}+{n}')
        writes+=1
    u.hook_add(UC_HOOK_CODE,instruction);u.hook_add(UC_HOOK_MEM_READ,read);u.hook_add(UC_HOOK_MEM_WRITE,write);rng=random.Random(a.seed)
    with tempfile.TemporaryDirectory(prefix='intro-map-oracle-') as directory:
        binary=Path(directory)/'maps.so'
        subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-shared','-fPIC',*[str(ROOT/'port/intro'/x) for x in ['maps.c','world.c','particles.c','tests/maps_bridge.c']],'-o',str(binary)],check=True,timeout=20)
        dll=C.CDLL(str(binary));dll.intro_maps_test.argtypes=[C.c_void_p,C.c_size_t,C.c_void_p,C.c_uint,C.c_uint32,C.c_uint];dll.intro_maps_test.restype=C.c_char_p
        initial=bytes([0xa5])*0x40000
        for case in range(a.cases):
            layer=case%3;base=0x900000+layer*0x4000;at=base+rng.randrange(4096)*4;inner=rng.randrange(1,9);outer=rng.randrange(1,9);visible=case&1
            source=struct.pack('>3H',rng.randrange(65536),inner-1,outer-1)+rng.randbytes(inner*outer*4);source_size=len(source)
            u.mem_write(0x200000,source);u.mem_write(0x900000,initial);u.mem_write(0xfff800,struct.pack('>I',0x18100))
            u.reg_write(m.UC_M68K_REG_SR,0x2000)
            for name,value in dict(A0=0x200000,A2=at,A7=0xfff800,D2=visible).items():u.reg_write(getattr(m,'UC_M68K_REG_'+name),value)
            u.emu_start((0x18ede,0x18f3c,0x18f9a)[layer],0x18100,count=10000,timeout=100000)
            if u.reg_read(m.UC_M68K_REG_PC)!=0x18100:raise AssertionError('original failed to return')
            cs=C.create_string_buffer(source);cv=C.create_string_buffer(initial);err=dll.intro_maps_test(cs,len(source),cv,layer,at,visible)
            if err or cv.raw[:0x40000]!=bytes(u.mem_read(0x900000,0x40000)):raise AssertionError((case,layer,hex(at),inner,outer,visible,err))
    paths=['port/intro/maps.c','port/intro/world.c','port/intro/world.h','port/intro/tests/maps_bridge.c','port/intro/verify_maps.py']
    report=dict(status='pass',seed=a.seed,cases=a.cases,writes=writes,opcode_sha256=OPCODE_SHA256,source_sha256={path:hashlib.sha256((ROOT/path).read_bytes()).hexdigest() for path in paths})
    with destination.open('x') as f:json.dump(report,f,indent=2);f.write('\n')
    print(json.dumps({k:v for k,v in report.items() if k not in ('source_sha256','opcode_sha256')}))
if __name__=='__main__':main()
