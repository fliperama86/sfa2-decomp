"""Sub-second native compiler/fixtures for particle-only checks, no game input."""
from contextlib import contextmanager
import ctypes as C
import json
from pathlib import Path
import struct
import subprocess
import tempfile

ROOT=Path(__file__).resolve().parents[2]
INTRO=ROOT/'port/intro'
class Motion(C.Structure):
    _fields_=[(n,C.c_uint32) for n in ('x','y','vx','vy','sway','acceleration','limit')]
class Spawn(C.Structure):
    _fields_=[(n,C.c_uint16) for n in ('subtype','layer','priority')]

def tables():
    return (b''.join(struct.pack('>II',0x10000+i,0xfffe0000-i) for i in range(12))+
            b''.join(struct.pack('>hh',i-16,208-i) for i in range(128)))

@contextmanager
def native(mutation=None):
    with tempfile.TemporaryDirectory(prefix='intro-particles-') as tmp:
        src=INTRO/'particles.c'
        if mutation:
            old,new=mutation
            text=src.read_text()
            if text.count(old)!=1: raise AssertionError('mutation is not unique')
            src=Path(tmp)/'mutant.c';src.write_text(text.replace(old,new))
        lib=Path(tmp)/'particles.so'
        subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-shared','-fPIC',
                        '-I',str(INTRO),str(src),'-o',str(lib)],check=True,timeout=15)
        dll=C.CDLL(str(lib))
        dll.intro_particle_random.argtypes=[C.POINTER(C.c_uint16)]
        dll.intro_particle_random.restype=C.c_uint8
        dll.intro_particle_spawn.argtypes=[C.POINTER(C.c_uint16),C.POINTER(C.c_uint16),
                                          C.c_uint,C.c_uint,C.POINTER(Spawn)]
        dll.intro_particle_init.argtypes=[C.POINTER(Motion),C.c_uint,C.POINTER(C.c_uint16),C.c_void_p]
        dll.intro_particle_move.argtypes=[C.POINTER(Motion)]
        yield dll

def invoke(dll,case):
    seed=C.c_uint16(case.get('seed',0))
    op=case['op']
    if op=='random': return dict(result=dll.intro_particle_random(C.byref(seed)),seed=seed.value)
    if op=='spawn':
        interval=C.c_uint16(case['interval']);out=Spawn(0xa5a5,0xa5a5,0xa5a5)
        result=dll.intro_particle_spawn(C.byref(interval),C.byref(seed),case['phase'],case['available'],C.byref(out))
        return dict(result=result,seed=seed.value,interval=interval.value,spawn=[out.subtype,out.layer,out.priority])
    # Guard the C object and immutable table, not only its logical result.
    storage=C.create_string_buffer(b'\xa5'*(C.sizeof(Motion)+16),C.sizeof(Motion)+16)
    p=Motion.from_buffer(storage,8)
    for (name,_),value in zip(Motion._fields_,case.get('motion',[0]*7)): setattr(p,name,value)
    data=C.create_string_buffer(tables(),len(tables()))
    if op=='init': result=dll.intro_particle_init(C.byref(p),case['subtype'],C.byref(seed),data)
    elif op=='move': result=dll.intro_particle_move(C.byref(p))
    else: raise ValueError('unknown operation')
    if storage.raw[:8]!=b'\xa5'*8 or storage.raw[-8:]!=b'\xa5'*8 or data.raw!=tables():
        raise AssertionError('C wrote outside its contract')
    return dict(result=result,seed=seed.value,motion=[getattr(p,n) for n,_ in Motion._fields_])

def fixtures(): return json.loads((INTRO/'particles-fixtures.json').read_text())

MUTANTS=[
    ('random','*seed*3u','*seed*2u'),
    ('spawn','available<=5','available<5'),
    ('init','&8u','&0u'),
    ('move','magnitude>=p->limit','magnitude>p->limit'),
]
