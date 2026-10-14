#!/usr/bin/env python3
"""Seconds: compare only original metasprite consumers with native C.

Private pinned opcode image, synthetic artwork/state, audited memory and PC.
This offline development oracle is never a runtime component or game boot.
"""
import argparse
import hashlib
import json
from pathlib import Path
import random
import struct
import subprocess
import sys

ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'port/tools'))
from intro_sprites_support import MUTATIONS,fixtures,invoke,native,random_case
from verify_sequence import OPCODE_SHA256,output_path

START,END=0x129ce,0x13122
INPUT,COORD,OUTPUT,ACTOR,GLOBAL,STACK,STOP=0x200000,0x200800,0xa00000,0xfff000,0xff8000,0xfff800,0x12000
ENTRIES=((0x129ce,0x12cdc),(0x12a50,0x12d20),(0x12ae8,0x12d64),
         (0x12b78,0x12da0),(0x12c2c,None),(0x12cda,0x12e5c))

def inside(a,n,ranges):return any(lo<=a and a+n<=hi for lo,hi in ranges)
class Oracle:
    def __init__(self,opcodes):
        from unicorn import Uc,UC_ARCH_M68K,UC_MODE_BIG_ENDIAN,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
        from unicorn import m68k_const as m
        if len(opcodes)!=0x300000 or hashlib.sha256(opcodes).hexdigest()!=OPCODE_SHA256:raise ValueError('not the pinned opcode input')
        self.m=m;self.u=Uc(UC_ARCH_M68K,UC_MODE_BIG_ENDIAN);u=self.u
        u.ctl_set_cpu_model(m.UC_CPU_M68K_M68000)
        for at,n in [(0x12000,0x2000),(0x9f000,0x1000),(INPUT,0x1000),(OUTPUT,0x2000),(0xff0000,0x10000)]:u.mem_map(at,n)
        u.mem_write(START,opcodes[START:END]);self.writes=0
        u.hook_add(UC_HOOK_CODE,self.instruction);u.hook_add(UC_HOOK_MEM_READ,self.read);u.hook_add(UC_HOOK_MEM_WRITE,self.write)
    def instruction(self,u,a,n,data):
        if not (START<=a and a+n<=END):raise AssertionError(f'PC escaped {a:x}')
    def read(self,u,access,a,n,v,data):
        if not inside(a,n,self.readable):raise AssertionError(f'read escaped {a:x}+{n}')
    def write(self,u,access,a,n,v,data):
        if not inside(a,n,self.writable):raise AssertionError(f'write escaped {a:x}+{n}')
        self.writes+=1
    def run(self,case):
        u,m=self.u,self.m;mode=case['mode'];blocks=case['blocks'];s=case['style'];cells=case['cells'];bypass=s['bypass']
        if mode==8 and bypass:raise ValueError('unobserved mode-8 bypass')
        source=bytearray(struct.pack('>HH',mode,cells));coords=b''.join(struct.pack('>hh',b[0],b[1]) for b in blocks)
        if mode==0:source+=struct.pack('>HH',blocks[0][3],0)+b''.join(struct.pack('>H',b[2]) for b in blocks)
        elif mode==2:source+=struct.pack('>HH',len(blocks)-1,0)+b''.join(struct.pack('>HH',*b[2:]) for b in blocks)
        elif mode==4:source+=struct.pack('>H',len(blocks)-1)+b''.join(struct.pack('>HHhh',b[2],b[3],b[0],b[1]) for b in blocks)
        elif mode==6:source+=struct.pack('>HHI',len(blocks)-1,blocks[0][3],COORD)+b''.join(struct.pack('>H',b[2]) for b in blocks)
        elif mode==8:source+=struct.pack('>HI',len(blocks)-1,COORD)+b''.join(struct.pack('>HH',*b[2:]) for b in blocks)
        actor=bytearray(b'\x5a'*128)
        for at,key in [(0xa,'bypass'),(0xb,'flip'),(0xc,'palette_mode'),(0xd,'palette')]:actor[at]=s[key]
        struct.pack_into('>HH',actor,0x18,s['x_flags'],s['y_flags'])
        struct.pack_into('>hhH',actor,0x20,s['expand_x'],s['expand_y'],s['rotate'])
        u.mem_write(INPUT,bytes(source));u.mem_write(COORD,coords);u.mem_write(0x9f260,struct.pack('>I',COORD));u.mem_write(0x1312a,struct.pack('>4h',*s['matrix']))
        initial=b'\xa5'*(896*8);u.mem_write(OUTPUT,initial);u.mem_write(ACTOR,bytes(actor));u.mem_write(STACK,struct.pack('>I',STOP))
        self.readable=[(INPUT,INPUT+len(source)),(COORD,COORD+len(coords)),(0x9f260,0x9f264),(0x1312a,0x13132),(ACTOR,ACTOR+128),(GLOBAL-0x57ca,GLOBAL-0x57ca+12),(STACK-32,STACK+4)]
        self.writable=[(OUTPUT+16,OUTPUT+16+len(blocks)*8),(GLOBAL-0x57ca,GLOBAL-0x57ca+12),(STACK-32,STACK)]
        u.reg_write(m.UC_M68K_REG_SR,0x2000)
        for name in [f'D{i}' for i in range(8)]+[f'A{i}' for i in range(8)]:u.reg_write(getattr(m,'UC_M68K_REG_'+name),0)
        for name,value in dict(A0=INPUT+2,A2=OUTPUT+16,A5=GLOBAL,A6=ACTOR,A7=STACK,D4=(s['y']<<16)|s['x'],D7=case['budget']).items():u.reg_write(getattr(m,'UC_M68K_REG_'+name),value)
        u.emu_start(ENTRIES[mode//2][bool(bypass)],STOP,count=2000,timeout=100000)
        if u.reg_read(m.UC_M68K_REG_PC)!=STOP:raise AssertionError('original did not return in bound')
        end=u.reg_read(m.UC_M68K_REG_A2)-OUTPUT;after=bytes(u.mem_read(OUTPUT,len(initial)))
        assert end>=16 and end%8==0 and after[:16]==initial[:16] and after[end:]==initial[end:]
        assert bytes(u.mem_read(ACTOR,128))==bytes(actor)
        assert bytes(u.mem_read(INPUT,len(source)))==bytes(source) and bytes(u.mem_read(COORD,len(coords)))==coords
        return dict(result=int(end>16),count=end//8,cells_left=u.reg_read(m.UC_M68K_REG_D7)&65535,objects=after[16:end].hex())

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--opcodes',type=Path,required=True)
    parser.add_argument('--seed',type=int,default=0);parser.add_argument('--cases',type=int,default=2000);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    if not 1<=args.cases<=24000:parser.error('cases must be in 1..24000')
    destination=output_path(args.output);oracle=Oracle(args.opcodes.read_bytes());rng=random.Random(args.seed);fixed=fixtures()
    with native() as dll:
        for case in fixed:
            result=oracle.run(case)
            if result!=case['after'] or invoke(dll,case)!=result:raise AssertionError('fixture mismatch '+case['name'])
        for n in range(args.cases):
            case=random_case(rng,n);expected=oracle.run(case);actual=invoke(dll,case)
            if actual!=expected:raise AssertionError(json.dumps(dict(n=n,case=case,expected=expected,actual=actual)))
    rejected=[]
    for mutation in MUTATIONS:
        with native(mutation) as dll:
            if all(invoke(dll,c)==c['after'] for c in fixed):raise AssertionError('undetected mutation '+mutation)
        rejected.append(mutation)
    paths=['port/intro/sprites.c','port/intro/sprites.h','port/intro/sprites-fixtures.json','port/intro/verify_sprites.py','port/tools/intro_sprites_support.py']
    report=dict(status='pass',commit=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip(),seed=args.seed,cases=args.cases,fixtures=len(fixed),writes=oracle.writes,mutations_rejected=rejected,opcode_sha256=OPCODE_SHA256,source_sha256={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in paths})
    with destination.open('x') as f:json.dump(report,f,indent=2);f.write('\n')
    print(json.dumps({k:v for k,v in report.items() if k not in ('source_sha256','opcode_sha256')}))
if __name__=='__main__':main()
