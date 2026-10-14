"""Synthetic input/ctypes adapter for the bounded intro object consumers."""
from contextlib import contextmanager
import ctypes as C
import json
from pathlib import Path
import random
import struct
import subprocess
import tempfile

ROOT=Path(__file__).resolve().parents[2]
class Block(C.Structure):
    _fields_=[('x',C.c_int16),('y',C.c_int16),('code',C.c_uint16),('attributes',C.c_uint16)]
class Art(C.Structure):
    _fields_=[('mode',C.c_uint),('cells',C.c_uint),('blocks',C.c_uint),('block',C.POINTER(Block))]
class Style(C.Structure):
    _fields_=[('x',C.c_uint16),('y',C.c_uint16),('x_flags',C.c_uint16),('y_flags',C.c_uint16),
              ('bypass',C.c_uint8),('flip',C.c_uint8),('palette_mode',C.c_uint8),('palette',C.c_uint8),
              ('expand_x',C.c_int16),('expand_y',C.c_int16),('rotate',C.c_uint8),('matrix',C.c_int16*4)]
class List(C.Structure):
    _fields_=[('objects',C.c_void_p),('capacity',C.c_size_t),('count',C.c_size_t),('cells_left',C.c_uint)]

MUTATIONS={
 'word_scale':('signed_word((uint16_t)(centered*(int32_t)amount))','centered*(int32_t)amount'),
 'flip_composition':('palette(attr^flip,s,art->mode==8 ? 96u : 0xff60u)','palette(attr|flip,s,art->mode==8 ? 96u : 0xff60u)'),
 'clamp':('x=448;','x=447;'),
 'cell_budget':('list->cells_left-=art->cells','list->cells_left-=art->blocks'),
}
@contextmanager
def native(mutation=None):
    with tempfile.TemporaryDirectory(prefix='intro-sprites-') as directory:
        directory=Path(directory);source=(ROOT/'port/intro/sprites.c').read_text()
        if mutation:
            old,new=MUTATIONS[mutation]
            if source.count(old)!=1:raise AssertionError('non-unique sprite mutation')
            source=source.replace(old,new)
        unit=directory/'sprites.c';unit.write_text(source);binary=directory/'sprites.so'
        subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-shared','-fPIC',
                        '-I'+str(ROOT/'port/intro'),str(unit),'-o',str(binary)],check=True,timeout=20)
        dll=C.CDLL(str(binary));dll.intro_sprites_append.argtypes=[C.POINTER(List),C.POINTER(Art),C.POINTER(Style)]
        yield dll

def invoke(dll,case):
    blocks=(Block*len(case['blocks']))(*(Block(*b) for b in case['blocks']))
    art=Art(case['mode'],case['cells'],len(blocks),blocks)
    style=Style()
    for key,value in case['style'].items():
        if key=='matrix':style.matrix[:]=value
        else:setattr(style,key,value)
    output=C.create_string_buffer(b'\xa5'*(896*8));before=bytes(output)
    state=List(C.addressof(output),896,2,case['budget'])
    result=dll.intro_sprites_append(C.byref(state),C.byref(art),C.byref(style))
    assert output.raw[:16]==before[:16] and output.raw[state.count*8:]==before[state.count*8:]
    return dict(result=result,count=state.count,cells_left=state.cells_left,objects=output.raw[16:state.count*8].hex())

def random_case(rng,n):
    mode=(0,2,4,6,8,10)[n%6];bypass=(n//6)&1 if mode!=8 else 0
    count=rng.randrange(1,6);attr=rng.randrange(4)*0x1000+rng.randrange(4)*256+rng.randrange(128)
    blocks=[]
    for i in range(count):
        blocks.append([rng.randrange(-32768,32768),rng.randrange(-32768,32768),rng.randrange(65536),
                       attr if mode in (0,6) else rng.randrange(4)*0x1000+rng.randrange(4)*256+rng.randrange(128)])
    cells=count if mode==0 else rng.randrange(count,100)
    return dict(mode=mode,cells=cells,budget=rng.choice([0,cells-1,cells,896]),blocks=blocks,
                style=dict(x=rng.randrange(65536),y=rng.randrange(65536),x_flags=rng.randrange(8)<<13,
                           y_flags=rng.randrange(4)<<13,bypass=bypass,flip=rng.randrange(4),
                           palette_mode=rng.choice([0,1,128,255]),palette=rng.randrange(256),
                           expand_x=rng.choice([0,-1,1,32767,-32768]),expand_y=rng.choice([0,-1,1,32767,-32768]),
                           rotate=rng.randrange(2),matrix=[rng.randrange(-32768,32768) for _ in range(4)]))

def fixtures():
    return json.loads((ROOT/'port/intro/sprites-fixtures.json').read_text())['cases']
