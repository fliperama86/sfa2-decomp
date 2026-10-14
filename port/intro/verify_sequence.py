#!/usr/bin/env python3
"""Offline differential check of two CPS2 sequence routines, seconds per seed.

Reads the user's pinned decrypted opcode image and project-authored C. Only
these two routines execute, with synthetic tables/objects and audited accesses.
Unicorn is a development oracle, never a runtime dependency of the port.
Reports are private and exclusive-created. No game is booted, no assets read.
"""
from __future__ import annotations
import argparse
import hashlib
import json
from pathlib import Path
import random
import struct
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/tools'))
from intro_sequence_support import fixture_rom, fixtures, invoke, native

OPCODE_SHA256 = '5b2bc2e9e1201460b996850270e6b88ff55670ccfa64c74ef62d81ad1ba83384'
START, SELECT, END = 0x18cb6, 0x18cce, 0x18ce6
OBJECT, STACK, STOP = 0xff0000, 0xfff000, 0x18000


def checked_opcodes(data):
    if len(data) != 0x300000 or hashlib.sha256(data).hexdigest() != OPCODE_SHA256:
        raise ValueError('not the pinned supplied-revision opcode image')
    return data[START:END]


def output_path(value):
    path = Path(value).resolve()
    if not path.is_relative_to((ROOT / 'port/local').resolve()):
        raise ValueError('evidence must stay under this checkout port/local')
    if path.exists():
        raise ValueError('refusing to overwrite existing evidence')
    return path


class Oracle:
    def __init__(self, code):
        from unicorn import Uc, UC_ARCH_M68K, UC_MODE_BIG_ENDIAN, UC_HOOK_MEM_READ, UC_HOOK_MEM_WRITE, UC_HOOK_CODE
        from unicorn import m68k_const as m
        self.m = m
        self.u = Uc(UC_ARCH_M68K, UC_MODE_BIG_ENDIAN)
        self.u.ctl_set_cpu_model(m.UC_CPU_M68K_M68000)
        self.u.mem_map(0, 0x10000)
        self.u.mem_map(0x18000, 0x1000)
        self.u.mem_map(OBJECT, 0x10000)
        self.u.mem_write(START, code)
        self.u.hook_add(UC_HOOK_MEM_READ, self.read)
        self.u.hook_add(UC_HOOK_MEM_WRITE, self.write)
        self.u.hook_add(UC_HOOK_CODE, self.instruction)
        self.writes = 0

    def read(self, uc, access, address, size, value, data):
        if not (0 <= address and address + size <= self.size or
                OBJECT <= address and address + size <= OBJECT + 0x80 or
                STACK <= address and address + size <= STACK + 4):
            raise AssertionError(f'original read outside contract: {address:x}+{size}')

    def write(self, uc, access, address, size, value, data):
        if not (OBJECT + 0x1c <= address and address + size <= OBJECT + 0x20 or
                OBJECT + 0x32 <= address and address + size <= OBJECT + 0x36):
            raise AssertionError(f'original write outside contract: {address:x}+{size}')
        self.writes += 1

    def instruction(self, uc, address, size, data):
        if not START <= address < END:
            raise AssertionError(f'original left the two routines: {address:x}')

    def run(self, op, rom, before, table=0, index=0):
        self.size = len(rom)
        if self.size > 0x10000:
            raise ValueError('synthetic fixture exceeds oracle mapping')
        u, m = self.u, self.m
        u.mem_write(0, bytes(rom))
        initial = bytearray(b'\xa5' * 0x80)
        struct.pack_into('>I', initial, 0x1c, before[0])
        struct.pack_into('>HH', initial, 0x32, *before[1:])
        u.mem_write(OBJECT, bytes(initial))
        # Reset every register, including supervisor/stack-bank state.
        u.reg_write(m.UC_M68K_REG_SR, 0x2000)
        for name in [f'D{i}' for i in range(8)] + [f'A{i}' for i in range(8)]:
            u.reg_write(getattr(m, 'UC_M68K_REG_' + name), 0)
        u.reg_write(m.UC_M68K_REG_A6, OBJECT)
        u.reg_write(m.UC_M68K_REG_A7, STACK)
        u.reg_write(m.UC_M68K_REG_A0, table)
        u.reg_write(m.UC_M68K_REG_D0, index)
        u.mem_write(STACK, struct.pack('>I', STOP))
        u.emu_start(SELECT if op == 'select' else START, STOP, timeout=100000, count=64)
        if u.reg_read(m.UC_M68K_REG_PC) != STOP:
            raise AssertionError('original did not return within its execution bound')
        result = bytes(u.mem_read(OBJECT, len(initial)))
        after = [struct.unpack_from('>I',result,0x1c)[0], *struct.unpack_from('>HH',result,0x32)]
        struct.pack_into('>I', initial, 0x1c, after[0])
        struct.pack_into('>HH', initial, 0x32, *after[1:])
        if result != bytes(initial) or bytes(u.mem_read(0,len(rom))) != bytes(rom):
            raise AssertionError('original altered bytes outside its state fields')
        return after


def random_case(rng, number):
    rom = bytearray(rng.randbytes(4096))
    op = 'select' if number % 2 == 0 else 'tick'
    count = rng.choice([0,1,2,0xffff,rng.randrange(65536)])
    before = [0x100, count, rng.randrange(65536)]
    table, index = 0x800, rng.getrandbits(32)
    target = rng.choice([0,0x100,0x300,0xc00,0xff0])
    if op == 'select':
        struct.pack_into('>h',rom,table+(index&255)*2,target-table)
    else:
        struct.pack_into('>I',rom,0x110,target)
    # All records are synthetic. Random contents exercise complete flag words.
    return op,rom,before,table,index


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--opcodes', type=Path, required=True)
    parser.add_argument('--seed', type=int, default=0)
    parser.add_argument('--cases', type=int, default=2000)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if not 1 <= args.cases <= 20000:
        parser.error('cases must be in 1..20000')
    destination = output_path(args.output)
    code = checked_opcodes(args.opcodes.read_bytes())
    oracle = Oracle(code)
    rng = random.Random(args.seed)
    fixed = fixtures()
    with native() as dll:
        for case in fixed:
            rom = fixture_rom(case)
            params = (case['op'],rom,case['before'],case.get('table',0),case.get('index',0))
            expected = oracle.run(*params)
            if expected != case['after'] or invoke(dll,*params) != (0,expected):
                raise AssertionError(f"fixture differs: {case['name']}")
        for number in range(args.cases):
            params = random_case(rng,number)
            expected = oracle.run(*params)
            if invoke(dll,*params) != (0,expected):
                raise AssertionError(f'random case {number} differs')
    case = next(c for c in fixed if c['name']=='advance-loads-full-flags')
    params = ('tick',fixture_rom(case),case['before'],0,0)
    expected = oracle.run(*params)
    with native(control=True) as mutant:
        if invoke(mutant,*params) == (0,expected):
            raise AssertionError('wrong-stride mutation was not detected')
    sources = ['port/intro/sequence.c','port/intro/sequence.h',
               'port/intro/sequence-fixtures.json','port/intro/verify_sequence.py',
               'port/tools/intro_sequence_support.py']
    import unicorn
    report = dict(commit=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip(),
                  source_sha256={p:hashlib.sha256((ROOT/p).read_bytes()).hexdigest() for p in sources},
                  opcode_sha256=OPCODE_SHA256, oracle='Unicorn '+unicorn.__version__,
                  scope='selector/countdown reader only, synthetic data, no game boot',
                  seed=args.seed,cases=args.cases,fixtures=len(fixed),
                  audited_writes=oracle.writes,wrong_stride_rejected=True,status='pass')
    destination.parent.mkdir(parents=True,exist_ok=True)
    with destination.open('x') as f:
        json.dump(report,f,indent=2);f.write('\n')
    print(json.dumps({k:report[k] for k in ('status','seed','cases','fixtures','audited_writes','wrong_stride_rejected')}))


if __name__ == '__main__':
    main()
