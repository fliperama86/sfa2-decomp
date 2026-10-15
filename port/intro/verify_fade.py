#!/usr/bin/env python3
"""Seconds: isolated palette helpers, synthetic palettes, audited original writes.

Needs the pinned private opcode image and Unicorn. Never part of runtime.
No artwork, disc, complete game or other intro function is executed.
"""
import argparse,ctypes as C,hashlib,json,random,struct,subprocess,tempfile
from pathlib import Path
from verify_sequence import OPCODE_SHA256,output_path
ROOT=Path(__file__).resolve().parents[2]

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--opcodes',type=Path,required=True);p.add_argument('--seed',type=int,default=0);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    destination=output_path(a.output);opcodes=a.opcodes.read_bytes()
    if hashlib.sha256(opcodes).hexdigest()!=OPCODE_SHA256:raise ValueError('not the pinned opcode input')
    from unicorn import Uc,UC_ARCH_M68K,UC_MODE_BIG_ENDIAN,UC_HOOK_CODE,UC_HOOK_MEM_WRITE
    from unicorn import m68k_const as m
    u=Uc(UC_ARCH_M68K,UC_MODE_BIG_ENDIAN);u.ctl_set_cpu_model(m.UC_CPU_M68K_M68000)
    for address,size in [(0xa000,0x2000),(0xffff0000,0x10000),(0x900000,0x10000)]:u.mem_map(address,size)
    u.mem_write(0xa000,opcodes[0xa000:0xc000]);writes=0
    def instruction(u,address,n,context):
        if not 0xa608<=address<0xa962:raise AssertionError(f'helper escaped {address:x}')
    def write(u,access,address,n,value,context):
        nonlocal writes
        if not any(lo<=address and address+n<=hi for lo,hi in [(0x90c000,0x90d000),(0xffffcf40,0xffffcf43),(0xffff036e,0xffff0370),(0xfffff700,0xfffff804)]):raise AssertionError(f'write escaped {address:x}+{n}')
        writes+=1
    u.hook_add(UC_HOOK_CODE,instruction);u.hook_add(UC_HOOK_MEM_WRITE,write);rng=random.Random(a.seed);cases=0
    with tempfile.TemporaryDirectory(prefix='intro-fade-oracle-') as directory:
        binary=Path(directory)/'fade.so';subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-shared','-fPIC',str(ROOT/'port/intro/fade.c'),str(ROOT/'port/intro/tests/fade_bridge.c'),'-o',str(binary)],check=True,timeout=20)
        dll=C.CDLL(str(binary));dll.intro_fade_test.argtypes=[C.c_void_p,C.c_void_p,C.c_uint,C.POINTER(C.c_uint)]
        for mode,entry in [(1,0xa608),(0xc01,0xa71a),(0x1e01,0xa90c)]:
            for case in range(64):
                target=rng.randrange(65536) if mode!=0x1e01 else [0,1,15,0x8000][case%4]
                palette=bytearray(struct.pack('>2048H',*(rng.randrange(65536) for i in range(2048))))
                # Equal-first-color and mismatched remaining colors test the bank gate.
                if mode!=0x1e01:
                    for layer in range(4):
                        if (case>>layer)&1:struct.pack_into('>H',palette,layer*1024,((target>>(layer*4))&15)<<12)
                flags=bytes([rng.randrange(256),255,255]);u.mem_write(0x90c000,bytes(palette));u.mem_write(0xffffcf40,flags);u.mem_write(0xffff036e,struct.pack('>H',target));u.mem_write(0xfffff800,struct.pack('>I',0xb000))
                u.reg_write(m.UC_M68K_REG_SR,0x2000);u.reg_write(m.UC_M68K_REG_A5,0xffff8000);u.reg_write(m.UC_M68K_REG_A6,0xffff035c);u.reg_write(m.UC_M68K_REG_A7,0xfffff800)
                u.emu_start(entry,0xb000,count=100000,timeout=500000)
                if u.reg_read(m.UC_M68K_REG_PC)!=0xb000:raise AssertionError('helper did not return')
                cp=C.create_string_buffer(bytes(palette));cf=C.create_string_buffer(flags);ct=C.c_uint(target)
                assert dll.intro_fade_test(cp,cf,mode,C.byref(ct))==0
                if cp.raw[:4096]!=bytes(u.mem_read(0x90c000,4096)) or cf.raw[:3]!=bytes(u.mem_read(0xffffcf40,3)) or ct.value!=int.from_bytes(u.mem_read(0xffff036e,2),'big'):raise AssertionError((mode,case))
                cases+=1
    paths=['port/intro/fade.c','port/intro/world.h','port/intro/tests/fade_bridge.c','port/intro/verify_fade.py']
    report=dict(status='pass',seed=a.seed,cases=cases,palette_words_compared=cases*2048,writes=writes,opcode_sha256=OPCODE_SHA256,source_sha256={path:hashlib.sha256((ROOT/path).read_bytes()).hexdigest() for path in paths})
    with destination.open('x') as f:json.dump(report,f,indent=2);f.write('\n')
    print(json.dumps({k:v for k,v in report.items() if k not in ('opcode_sha256','source_sha256')}))
if __name__=='__main__':main()
