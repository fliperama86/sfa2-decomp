#!/usr/bin/env python3
"""Build a sparse full-intro asset pack from the examined user-owned archive.

Runs project-authored native C to find its data/tile closure, never an original
CPU interpreter. Seconds to about a minute; no network or running game. The
inline data carrier is decrypted transiently; only requested data is retained.
"""
import argparse
import ctypes as C
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import sys
import zipfile
from import_scene import ARCHIVE_SHA,PROGRAM_SHA,GFX_SHA,OPCODE_SHA,lanes,unshuffle,require,private_output
ROOT=Path(__file__).resolve().parents[2]
SOURCES=['world','director','actors','actor_portraits','actor_montage','actor_maps','actor_palette','actor_logo','actor_duel','maps','fade','text','particles','presentation','sprites','player','video','resources']
MAGIC=b'SFA2I003'
SEED=0x1234

def archive_inputs(path):
    require(hashlib.sha256(path.read_bytes()).hexdigest()==ARCHIVE_SHA,'unsupported arcade archive')
    with zipfile.ZipFile(path) as archive:
        require(len(archive.namelist())==len(set(archive.namelist())),'duplicate ZIP member')
        raw=b''.join(archive.read(f'sz2u.{i:02d}') for i in range(3,9))
        data=bytearray(len(raw));data[::2],data[1::2]=raw[1::2],raw[::2]
        gfx=unshuffle(b''.join(lanes([archive.read(f'sz2.{i:02d}m') for i in group]) for group in ((13,15,17,19),(14,16,18,20))))
    require(hashlib.sha256(data).hexdigest()==PROGRAM_SHA,'data carrier digest')
    require(hashlib.sha256(gfx).hexdigest()==GFX_SHA,'graphics layout digest')
    return bytes(data),gfx,archive_key(path)

def archive_key(path):
    with zipfile.ZipFile(path) as archive:key=archive.read('sfa2u.key')
    require(len(key)==20,'archive key size')
    return key

def decrypt(data,key,cxx):
    binary=ROOT/'port/build/intro-decrypt.so';binary.parent.mkdir(parents=True,exist_ok=True)
    folder=ROOT/'port/intro/tools/cps2crypt'
    subprocess.run([cxx,'-std=c++17','-O2','-Wall','-Wextra','-Werror','-Wno-sign-compare','-shared','-fPIC',str(folder/'cps2crypt.cpp'),str(folder/'decrypt.cpp'),'-o',str(binary)],check=True,timeout=30)
    dll=C.CDLL(str(binary));dll.intro_import_decrypt.argtypes=[C.c_void_p,C.c_size_t,C.c_void_p,C.c_size_t,C.c_void_p]
    src=C.create_string_buffer(data);k=C.create_string_buffer(key);out=C.create_string_buffer(len(data))
    require(dll.intro_import_decrypt(src,len(data),k,len(key),out)==0,'import data decryption')
    result=out.raw
    require(hashlib.sha256(result).hexdigest()==OPCODE_SHA,'decrypted inline-data carrier digest')
    return result

def encode(spans,tiles):
    """Canonical big-endian sparse ranges and sorted indexed tile records."""
    require(0<len(spans)<=32768 and 0<len(tiles)<=65536,'resource counts')
    spans=sorted(spans);tiles=sorted(tiles);offset=8+16*len(spans)+12*len(tiles)
    table=bytearray(struct.pack('>II',len(spans),len(tiles)));body=bytearray();end=-1;last=-1
    for plane,address,data in spans:
        key=plane*2**32+address
        require(plane in (0,1) and 0<=address<0x300000 and 0<len(data)<=0x300000-address and key>=end,'data range/order')
        table+=struct.pack('>IIII',plane,address,len(data),offset);body+=data;offset+=len(data);end=key+len(data)
    for key,data in tiles:
        side=key>>24;code=(key&0xffffff)>>1;half=key&1
        require(key>last and side in (8,16,32) and (side==8 or half==0) and code<0x1400000//(64 if side==8 else side*side//2) and len(data)==side*side and max(data)<=15,'tile key/pixels')
        table+=struct.pack('>III',key,len(data),offset);body+=data;offset+=len(data);last=key
    require(offset<=64*1024*1024,'pack capacity')
    payload=bytes(table+body)
    return MAGIC+hashlib.sha256(payload).digest()+payload

def probe_library(cc):
    binary=ROOT/'port/build/intro-import.so';binary.parent.mkdir(parents=True,exist_ok=True)
    units=[ROOT/'port/intro'/(name+'.c') for name in SOURCES]+[ROOT/'port/intro/tools/import_probe.c']
    subprocess.run([cc,'-std=c11','-O2','-Wall','-Wextra','-Werror','-shared','-fPIC',*map(str,units),'-o',str(binary)],check=True,timeout=30)
    dll=C.CDLL(str(binary));dll.intro_probe_start.argtypes=[C.c_void_p,C.c_size_t,C.c_void_p,C.c_size_t,C.c_void_p,C.c_size_t,C.c_uint]
    dll.intro_probe_packed_start.argtypes=[C.c_void_p,C.c_size_t,C.c_uint]
    dll.intro_probe_error.restype=C.c_char_p;dll.intro_probe_frame_hash.restype=C.c_uint64
    dll.intro_probe_reads.argtypes=[C.c_uint];dll.intro_probe_reads.restype=C.c_void_p
    dll.intro_probe_tile.argtypes=[C.c_uint,C.POINTER(C.c_uint32)];dll.intro_probe_tile.restype=C.c_void_p
    return dll

def play(dll):
    hashes=[]
    for tick in range(6000):
        result=dll.intro_probe_step();error=dll.intro_probe_error()
        require(result>=0 and not error,f'native resource closure stopped at {tick}: {error!r}')
        hashes.append(dll.intro_probe_frame_hash())
        if result:return hashes
    raise ValueError('native intro did not finish within its bounded update budget')

def build(data,inlined,gfx,dll):
    buffers=[C.create_string_buffer(x) for x in (data,inlined,gfx)]
    require(dll.intro_probe_start(buffers[0],len(data),buffers[1],len(inlined),buffers[2],len(gfx),SEED)==0,'native probe initialization')
    expected=play(dll);spans=[];tiles=[]
    for plane,source in enumerate((data,inlined)):
        mask=C.string_at(dll.intro_probe_reads(plane),len(source))
        for match in re.finditer(b'\x01+',mask):spans.append((plane,match.start(),source[match.start():match.end()]))
    key=C.c_uint32()
    for slot in range(65536):
        pointer=dll.intro_probe_tile(slot,C.byref(key))
        if pointer:tiles.append((key.value,C.string_at(pointer,(key.value>>24)**2)))
    pack=encode(spans,tiles);payload=C.create_string_buffer(pack[40:])
    require(dll.intro_probe_packed_start(payload,len(pack)-40,SEED)==0,'sparse pack validation')
    actual=play(dll)
    require(actual==expected,'sparse resources changed native frame output')
    return pack,dict(ticks=len(actual),data_ranges=len(spans),data_bytes=sum(len(x[2]) for x in spans),tiles=len(tiles),tile_bytes=sum(len(x[1]) for x in tiles),seed=SEED,frame_comparison='all native full-input versus sparse-pack RGB555 frame hashes equal; not original-frame playback')

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('archive',type=Path);parser.add_argument('output',type=Path)
    parser.add_argument('--opcodes',type=Path,help='optional pinned development carrier instead of decryption');parser.add_argument('--cc',default='cc');parser.add_argument('--cxx',default='c++');args=parser.parse_args()
    try:
        output=private_output(args.output);report=output.with_suffix(output.suffix+'.json')
        require(not output.exists() and not report.exists(),'output or report exists; preserve earlier evidence')
        data,gfx,key=archive_inputs(args.archive)
        inlined=args.opcodes.read_bytes() if args.opcodes else decrypt(data,key,args.cxx)
        require(hashlib.sha256(inlined).hexdigest()==OPCODE_SHA,'inline-data carrier digest')
        dll=probe_library(args.cc);pack,evidence=build(data,inlined,gfx,dll)
        evidence.update(pack_sha256=hashlib.sha256(pack).hexdigest(),pack_bytes=len(pack),archive_sha256=ARCHIVE_SHA,program_sha256=PROGRAM_SHA,inline_carrier_sha256=OPCODE_SHA,graphics_sha256=GFX_SHA,
                        source_sha256={str(path.relative_to(ROOT)):hashlib.sha256(path.read_bytes()).hexdigest() for path in [ROOT/'port/intro'/(name+'.c') for name in SOURCES]+[Path(__file__),ROOT/'port/intro/import_scene.py',ROOT/'port/intro/tools/import_probe.c']+sorted((ROOT/'port/intro/tools/cps2crypt').glob('*.cpp'))+sorted((ROOT/'port/intro').glob('*.h'))})
        output.parent.mkdir(parents=True,exist_ok=True)
        with output.open('xb') as f:f.write(pack)
        with report.open('x') as f:json.dump(evidence,f,indent=2);f.write('\n')
        print(json.dumps({k:v for k,v in evidence.items() if not k.endswith('sha256')}))
    except (ValueError,OSError,KeyError,zipfile.BadZipFile,subprocess.SubprocessError) as error:
        print(f'intro import refused: {error}',file=sys.stderr);return 1
    return 0
if __name__=='__main__':raise SystemExit(main())
