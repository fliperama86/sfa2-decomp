#!/usr/bin/env python3
"""Seconds: isolated original-vs-C particle routines on synthetic state/tables.

Reads a pinned private opcode image, never boots a game. Unicorn is offline
validation only. Audits original data accesses and control flow. Reports stay
private and exclusive-created. Unit fixtures require neither ROM nor Unicorn.
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
from intro_particles_support import Motion, MUTANTS, fixtures, invoke, native, tables
from verify_sequence import OPCODE_SHA256, checked_opcodes, output_path

OBJECT,GLOBAL,STACK,STOP=0xff1000,0xff8000,0xfff000,0x1000
RANGES=[(0x451c,0x460c),(0x1975a,0x19774),(0x73cc4,0x73d7a),
        (0x73d80,0x73d9a),(0x73dfa,0x73e10),(0x74010,0x74034)]
FIELDS=(0x10,0x14,0x40,0x44,0x48,0x4c,0x60)

def inside(address,size,ranges): return any(a<=address and address+size<=b for a,b in ranges)

class Oracle:
    def __init__(self,opcodes):
        from unicorn import Uc,UC_ARCH_M68K,UC_MODE_BIG_ENDIAN,UC_HOOK_CODE,UC_HOOK_MEM_READ,UC_HOOK_MEM_WRITE
        from unicorn import m68k_const as m
        checked_opcodes(opcodes)
        self.m=m;self.u=Uc(UC_ARCH_M68K,UC_MODE_BIG_ENDIAN)
        self.u.ctl_set_cpu_model(m.UC_CPU_M68K_M68000)
        self.u.mem_map(0,0x80000);self.u.mem_map(0xff0000,0x10000)
        for a,b in RANGES:self.u.mem_write(a,opcodes[a:b])
        # PC-relative reads are deliberately synthetic, not copied game tables.
        self.u.mem_write(0x73d9a,tables()[:96]);self.u.mem_write(0x73e10,tables()[96:])
        self.u.hook_add(UC_HOOK_CODE,self.instruction)
        self.u.hook_add(UC_HOOK_MEM_READ,self.read)
        self.u.hook_add(UC_HOOK_MEM_WRITE,self.write)
        self.writes=self.reads=0

    def read(self,u,access,a,n,value,data):
        if not inside(a,n,self.readable):raise AssertionError(f'original read outside contract {a:x}+{n}')
        self.reads+=1
    def write(self,u,access,a,n,value,data):
        if not inside(a,n,self.writable):raise AssertionError(f'original write outside contract {a:x}+{n}')
        self.writes+=1
    def ret(self):
        u,m=self.u,self.m;sp=u.reg_read(m.UC_M68K_REG_A7)
        pc=struct.unpack('>I',u.mem_read(sp,4))[0]
        u.reg_write(m.UC_M68K_REG_A7,sp+4);u.reg_write(m.UC_M68K_REG_PC,pc)
    def instruction(self,u,a,n,data):
        m=self.m
        if a in (0x18cce,0x18cb6):
            if a==0x18cce:
                assert (u.reg_read(m.UC_M68K_REG_D0)&255)==self.case['subtype']
                assert u.reg_read(m.UC_M68K_REG_A0)==0x229120
            self.calls.append(a);self.ret();return
        if a==0x18b7c:
            self.calls.append(a);u.emu_stop();return
        if a==0x1963a:
            # The caller has already checked >5 free. Explicit successful
            # allocator boundary, preserving D0 and returning a zeroed actor.
            self.calls.append(a);u.reg_write(m.UC_M68K_REG_A4,OBJECT)
            u.reg_write(m.UC_M68K_REG_SR,u.reg_read(m.UC_M68K_REG_SR)&~4)
            self.ret();return
        if not inside(a,n,RANGES):raise AssertionError(f'original PC escaped {a:x}')

    def run(self,case):
        u,m=self.u,self.m;self.case=case;self.calls=[]
        op=case['op'];seed=case.get('seed',0)
        u.mem_write(GLOBAL,bytes(0x6200));u.mem_write(GLOBAL+0xae,struct.pack('>H',seed))
        before=bytearray(b'\xa5'*0x80)
        before[3]=case.get('subtype',1);before[4]=0 if op=='init' else 2
        for at,v in zip(FIELDS,case.get('motion',[0]*7)):struct.pack_into('>I',before,at,v)
        if op=='spawn':before=bytearray(0x80)
        u.mem_write(OBJECT,bytes(before));u.mem_write(STACK,struct.pack('>I',STOP))
        u.reg_write(m.UC_M68K_REG_SR,0x2000)
        for name in [f'D{i}' for i in range(8)]+[f'A{i}' for i in range(8)]:u.reg_write(getattr(m,'UC_M68K_REG_'+name),0)
        u.reg_write(m.UC_M68K_REG_A5,GLOBAL);u.reg_write(m.UC_M68K_REG_A6,OBJECT);u.reg_write(m.UC_M68K_REG_A7,STACK)
        seedrange=(GLOBAL+0xae,GLOBAL+0xb0)
        self.readable=[(OBJECT,OBJECT+0x80),seedrange,(STACK-32,STACK+4),
                       (0x73d9a,0x73dfa),(0x73e10,0x74010)]
        self.writable=[seedrange,(STACK-32,STACK)]
        if op=='random':start=0x1975a
        elif op=='spawn':
            start=(0x451c,0x4560,0x45bc)[case['phase']]
            u.mem_write(GLOBAL+0xe,struct.pack('>H',case['interval']))
            u.mem_write(GLOBAL+0x6116,bytes([case['available']]))
            self.readable += [(GLOBAL+0xe,GLOBAL+0x10),(GLOBAL+0x6116,GLOBAL+0x6117)]
            self.writable += [(GLOBAL+0xe,GLOBAL+0x10)]+[(OBJECT+a,OBJECT+b) for a,b in [(0,1),(2,4),(9,10),(14,15),(24,26)]]
        else:
            start=0x73cc4 if op=='init' else 0x73d32
            fields=[(4,5),(0x10,0x18),(0x48,0x50)]
            if op=='init':fields += [(11,16),(0x1a,0x1c),(0x40,0x48),(0x60,0x64)]
            self.writable += [(OBJECT+a,OBJECT+b) for a,b in fields]
        u.emu_start(start,STOP,timeout=100000,count=160)
        pc=u.reg_read(m.UC_M68K_REG_PC)
        assert pc==(0x18b7c if op in ('init','move') else STOP),hex(pc)
        assert u.reg_read(m.UC_M68K_REG_A7)==STACK+(4 if op in ('spawn','random') else 0)
        after=bytes(u.mem_read(OBJECT,0x80));expected=bytearray(before)
        nextseed=struct.unpack('>H',u.mem_read(GLOBAL+0xae,2))[0]
        if op=='random':result=dict(result=u.reg_read(m.UC_M68K_REG_D0)&255,seed=nextseed)
        elif op=='spawn':
            active=after[0]
            result=dict(result=active,seed=nextseed,interval=struct.unpack('>H',u.mem_read(GLOBAL+0xe,2))[0],
                        spawn=[after[3],after[9],struct.unpack_from('>H',after,24)[0]] if active else [0xa5a5]*3)
            if active:
                expected[0]=1;expected[2]=0x80;expected[3]=result['spawn'][0];expected[9]=result['spawn'][1]
                expected[14]=0;struct.pack_into('>H',expected,24,result['spawn'][2])
            assert self.calls==([0x1963a] if active else [])
        else:
            values=[struct.unpack_from('>I',after,a)[0] for a in FIELDS]
            result=dict(result=0 if op=='init' else int(after[4]==6),seed=nextseed,motion=values)
            for at,v in zip(FIELDS,values):struct.pack_into('>I',expected,at,v)
            expected[4]=after[4]
            if op=='init':
                expected[11]=expected[12]=expected[13]=expected[15]=0
                struct.pack_into('>H',expected,0x1a,0x2000)
                assert self.calls==[0x18cce,0x18b7c]
            else:assert self.calls==[0x18cb6,0x18b7c]
        assert after==bytes(expected),'unexpected object write'
        assert bytes(u.mem_read(0x73d9a,96))+bytes(u.mem_read(0x73e10,512))==tables()
        return result

def random_case(rng,n):
    op=('random','spawn','init','move')[n%4]
    c=dict(op=op,seed=rng.randrange(65536))
    if op=='spawn':c.update(interval=rng.choice([0,1,1,1,2,65535]),phase=rng.randrange(3),available=rng.choice([0,5,6,39,40]))
    if op=='init':c['subtype']=rng.randrange(1,7)
    if op=='move':c['motion']=[rng.getrandbits(32) for _ in range(7)]
    return c

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--opcodes',type=Path,required=True);parser.add_argument('--seed',type=int,default=0)
    parser.add_argument('--cases',type=int,default=2000);parser.add_argument('--output',type=Path,required=True)
    a=parser.parse_args()
    if not 1<=a.cases<=20000:parser.error('cases must be 1..20000')
    dst=output_path(a.output);oracle=Oracle(a.opcodes.read_bytes());fixed=fixtures();rng=random.Random(a.seed)
    with native() as dll:
        for c in fixed:
            expected=oracle.run(c)
            if expected!=c['after'] or invoke(dll,c)!=expected:raise AssertionError(c['name'])
        for n in range(a.cases):
            c=random_case(rng,n);expected=oracle.run(c)
            if invoke(dll,c)!=expected:raise AssertionError((n,c,expected,invoke(dll,c)))
    for op,old,new in MUTANTS:
        cases=[c for c in fixed if c['op']==op]
        with native((old,new)) as dll:
            if not any(invoke(dll,c)!=c['after'] for c in cases):raise AssertionError('undetected '+op+' mutation')
    paths=['port/intro/particles.c','port/intro/particles.h','port/intro/particles-contract.md',
           'port/intro/particles-fixtures.json','port/intro/verify_particles.py',
           'port/tools/intro_particles_support.py','port/intro/verify_sequence.py']
    import unicorn
    report=dict(status='pass',commit=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip(),
                source_sha256={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in paths},
                opcode_sha256=OPCODE_SHA256,oracle='Unicorn '+unicorn.__version__,seed=a.seed,cases=a.cases,
                fixtures=len(fixed),audited_reads=oracle.reads,audited_writes=oracle.writes,
                mutants_rejected=[m[0] for m in MUTANTS],scope='particle RNG, spawn phases, initialization and motion only; synthetic tables; no game boot')
    with dst.open('x') as f:json.dump(report,f,indent=2);f.write('\n')
    print(json.dumps({k:report[k] for k in ('status','seed','cases','fixtures','audited_reads','audited_writes','mutants_rejected')}))
if __name__=='__main__':main()
