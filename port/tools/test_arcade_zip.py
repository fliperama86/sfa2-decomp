#!/usr/bin/env python3
"""Seconds: the arcade archive reader (zip.c) and its inflate on synthetic archives and streams; no game files.
Native build with sanitizers; one plain -m32 run when the machine can do it."""
from pathlib import Path
import random,shutil,struct,subprocess,tempfile,unittest,zipfile,zlib,io
ROOT=Path(__file__).resolve().parents[2]
SRC=[str(ROOT/'port/arcade/zip.c'),str(ROOT/'port/arcade/tests/zip_test.c')]
FLAGS=['gcc','-std=c99','-O1','-Wall','-Wextra','-Werror']

def build(entries,comment=b'',eocd=None):
    """A ZIP by hand. Per entry: name, data, method, level, comp (raw bytes), flags, dd (data descriptor), lname,
    lmethod, lextra, cextra, ccrc, ccsize, cusize, cmethod, coff, disk. eocd: n, cdsize, cdoff, disk."""
    out=bytearray();cd=bytearray()
    for e in entries:
        name=e['name'] if isinstance(e['name'],bytes) else e['name'].encode()
        data=e.get('data',b'');method=e.get('method',8)
        if 'comp' in e:comp=e['comp']
        elif method==0:comp=data
        else:
            c=zlib.compressobj(e.get('level',6),zlib.DEFLATED,-15);comp=c.compress(data)+c.flush()
        crc=zlib.crc32(data);usize=len(data);csize=len(comp)
        dd=e.get('dd',False);flags=e.get('flags',0)|(8 if dd else 0)
        off=len(out);lname=e.get('lname',name);lextra=e.get('lextra',b'');cextra=e.get('cextra',b'')
        out+=struct.pack('<IHHHHHIIIHH',0x04034b50,20,flags,e.get('lmethod',method),0,0x21,0 if dd else crc,0 if dd else csize,0 if dd else usize,len(lname),len(lextra))+lname+lextra+comp
        if dd:out+=struct.pack('<IIII',0x08074b50,crc,csize,usize)
        cd+=struct.pack('<IHHHHHHIIIHHHHHII',0x02014b50,20,20,flags,e.get('cmethod',method),0,0x21,e.get('ccrc',crc),e.get('ccsize',csize),e.get('cusize',usize),len(name),len(cextra),0,e.get('disk',0),0,0,e.get('coff',off))+name+cextra
    eo=dict(n=len(entries),cdsize=len(cd),cdoff=len(out),disk=0);eo.update(eocd or {})
    out+=cd
    out+=struct.pack('<IHHHHIIH',0x06054b50,eo['disk'],0,eo['n'],eo['n'],eo['cdsize'],eo['cdoff'],len(comment))+comment
    return bytes(out)

def zf_archive(items,comp,level=None,comment=b''):
    bio=io.BytesIO()
    with zipfile.ZipFile(bio,'w',comp,compresslevel=level) as z:
        for n,d in items:z.writestr(n,d)
        z.comment=comment
    return bio.getvalue()

class Bits:
    """LSB-first bit writer for hand-made deflate streams."""
    def __init__(s):s.v=0;s.n=0
    def put(s,val,n):s.v|=val<<s.n;s.n+=n;return s
    def code(s,val,n):
        for i in range(n-1,-1,-1):s.put((val>>i)&1,1)
        return s
    def fixed(s,sym):
        if sym<144:return s.code(0x30+sym,8)
        if sym<256:return s.code(0x190+sym-144,9)
        if sym<280:return s.code(sym-256,7)
        return s.code(0xC0+sym-280,8)
    def bytes(s):return s.v.to_bytes((s.n+7)//8,'little')

def fixed_stream(dist_code=0,final=True,sym=257):
    """'a', then a match (length 3 by default) at the distance of dist_code, then end of block."""
    b=Bits().put(1 if final else 0,1).put(1,2).fixed(ord('a')).fixed(sym).code(dist_code,5)
    return b.fixed(256).bytes()

def zeros(n):
    """Code-length program (symbol 18 only) for n zero lengths."""
    out=[]
    while n:
        k=min(n,138) if n-min(n,138)==0 or n-min(n,138)>=11 else n-11
        out.append((18,k-11,7));n-=k
    return out

def canon(cl):
    """Canonical codes (code, length) of a code-length table {symbol: length}."""
    codes={};code=0;prev=0
    for ln,sym in sorted((l,s) for s,l in cl.items()):
        code<<=ln-prev;codes[sym]=(code,ln);code+=1;prev=ln
    return codes

def dyn_a(prog=None,tail=((0,1),(1,1)),cl=None,hlit=257,hdist=1):
    """A dynamic block with 'a' and the end of block one bit each and one distance code: decodes to 'a'.
    prog is a list of (code-length symbol, extra, extra bits); cl the code-length code, symbols 1 and 18 by default."""
    cl=cl or {18:1,1:1};codes=canon(cl)
    prog=prog or zeros(97)+[(1,0,0)]+zeros(158)+[(1,0,0),(1,0,0)]
    b=Bits().put(1,1).put(2,2).put(hlit-257,5).put(hdist-1,5).put(14,4)
    for sym in [16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1]:b.put(cl.get(sym,0),3)
    for sym,extra,nb in prog:
        b.code(*codes[sym])
        if nb:b.put(extra,nb)
    for c in tail:b.code(*c)   # by default 'a' is code 0 and the end of block code 1
    return b.bytes()

def make_data():
    rng=random.Random(2024)
    words=[bytes(rng.choices(range(97,123),k=rng.randint(2,9))) for _ in range(500)]
    text=b' '.join(rng.choice(words) for _ in range(60000))
    skew=bytes(rng.choices(range(256),weights=[1.35**-i for i in range(256)],k=1<<20))
    block=rng.randbytes(32768)
    return dict(random=rng.randbytes(2<<20),repeat=block*64,run=b'a'*1_000_000,text=text,skew=skew,tiny=b'x')

class Checks:
    """Shared helpers; self.exe is the driver binary, self.tmp a scratch directory."""
    def drv(self,*args):
        return subprocess.run([str(self.exe),*map(str,args)],capture_output=True,timeout=60)
    def scratch(self,data,name='f.bin'):
        d=Path(tempfile.mkdtemp(dir=self.tmp));p=d/name;p.write_bytes(data);return p
    def accepted(self,archive,expected):
        p=self.scratch(archive,'a.zip');out=p.parent/'out';out.mkdir()
        r=self.drv('list',p,out)
        self.assertEqual(r.returncode,0,r.stderr.decode(errors='replace'))
        lines=r.stdout.decode().splitlines()
        self.assertEqual(len(lines),len(expected))
        for i,(line,(name,data)) in enumerate(zip(lines,expected)):
            n,size,crc=line.rsplit(' ',2)
            self.assertEqual((n,int(size),int(crc,16)),(name,len(data),zlib.crc32(data)))
            self.assertTrue((out/f'{i}.bin').read_bytes()==data,name)
        shutil.rmtree(p.parent)
    def refused(self,archive,text):
        p=self.scratch(archive,'a.zip');r=self.drv('list',p)
        err=r.stderr.decode(errors='replace')
        self.assertEqual(r.returncode,1,(text,r.stdout))
        self.assertTrue(err.startswith('arcade archive: '),err)
        self.assertIn(text,err)
        shutil.rmtree(p.parent)
    def inflates(self,raw,expected):
        p=self.scratch(raw,'raw');o=p.parent/'o'
        r=self.drv('inflate',p,len(expected),o)
        self.assertEqual(r.returncode,0,r.stderr.decode(errors='replace'))
        self.assertTrue(o.read_bytes()==expected)
        shutil.rmtree(p.parent)
    def inflate_refused(self,raw,size,why=''):
        p=self.scratch(raw,'raw');r=self.drv('inflate',p,size,p.parent/'o')
        self.assertEqual((r.returncode,r.stderr),(1,b'inflate refused\n'),why);self.assertFalse((p.parent/'o').exists(),why)
        shutil.rmtree(p.parent)

class ArcadeZip(Checks,unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp=tempfile.mkdtemp(prefix='arcade-zip-');cls.addClassCleanup(shutil.rmtree,cls.tmp,True)
        cls.exe=Path(cls.tmp)/'zip_test'
        subprocess.run([*FLAGS,'-fsanitize=address,undefined','-fno-sanitize-recover=undefined',*SRC,'-o',str(cls.exe)],check=True,timeout=60)
        cls.data=make_data()

    def test_crc32(self):
        for data in [b'',b'a',self.data['random'][:100001],b'123456789']:
            p=self.scratch(data);r=self.drv('crc',p)
            self.assertEqual(int(r.stdout,16),zlib.crc32(data));shutil.rmtree(p.parent)
        self.assertEqual(zlib.crc32(b'123456789'),0xCBF43926)

    def test_zipfile_archives(self):
        d=self.data;items=[('stored.bin',d['random']),('repeat.bin',d['repeat']),('run.bin',d['run']),('text.txt',d['text']),('skew.bin',d['skew']),('empty',b''),('one',d['tiny'])]
        for comp,level in [(zipfile.ZIP_STORED,None),(zipfile.ZIP_DEFLATED,0),(zipfile.ZIP_DEFLATED,1),(zipfile.ZIP_DEFLATED,6),(zipfile.ZIP_DEFLATED,9)]:
            arc=zf_archive(items,comp,level)
            with zipfile.ZipFile(io.BytesIO(arc)) as z:expected=[(n,z.read(n)) for n,_ in items]
            self.assertEqual(expected,items)
            self.accepted(arc,expected)

    def test_many_and_comment(self):
        rng=random.Random(5)
        items=[(f'dir/m{i:04}.bin',rng.randbytes(rng.randint(0,300))) for i in range(1000)]
        self.accepted(zf_archive(items,zipfile.ZIP_DEFLATED,6,comment=b'comment ' * 100),items)
        # a comment that holds a signature of an end record does not fool the scan
        fake=b'PK\x05\x06'+bytes(16)+b'\xff\xff'+b'x'*40
        self.accepted(zf_archive(items[:3],zipfile.ZIP_DEFLATED,6,comment=fake),items[:3])
        self.accepted(zf_archive(items[:3],zipfile.ZIP_DEFLATED,6,comment=b'c'*65535),items[:3])
        self.accepted(build([]),[])
        limit=[(f'f{i}',b'') for i in range(4096)]
        self.accepted(build([dict(name=n,data=d,method=0) for n,d in limit]),limit)

    def test_hand_made(self):
        d=self.data
        raw_fixed=zlib.compressobj(6,zlib.DEFLATED,-15,8,zlib.Z_FIXED);fixed=raw_fixed.compress(d['text'])+raw_fixed.flush()
        self.assertEqual(fixed[0]&6,2)  # block type 1
        entries=[dict(name='fixed',data=d['text'],comp=fixed),
                 dict(name='dd',data=d['skew'][:5000],dd=True),
                 dict(name='extras',data=b'hello extras',method=0,lextra=b'L'*17,cextra=b'C'*5),
                 dict(name='stored8',data=b'abc'*9999,level=0)]
        self.accepted(build(entries,comment=b'tail'),[(e['name'],e['data']) for e in entries])

    def test_inflate_streams(self):
        d=self.data;text=d['text'][:200000]
        for wbits in (-9,-12,-15):
            for strategy in (zlib.Z_DEFAULT_STRATEGY,zlib.Z_FILTERED,zlib.Z_HUFFMAN_ONLY,zlib.Z_RLE,zlib.Z_FIXED):
                for data in (text,d['skew'][:100000],d['run'][:70000]):
                    c=zlib.compressobj(6,zlib.DEFLATED,wbits,8,strategy);self.inflates(c.compress(data)+c.flush(),data)
        for level in range(1,10):
            c=zlib.compressobj(level,zlib.DEFLATED,-15);self.inflates(c.compress(text)+c.flush(),text)
        # sync flushes leave empty stored blocks after blocks that end between bytes
        c=zlib.compressobj(6,zlib.DEFLATED,-15);a,b=text[:7777],text[7777:30000]
        raw=c.compress(a)+c.flush(zlib.Z_SYNC_FLUSH)+c.compress(b)+c.flush(zlib.Z_FULL_FLUSH)+c.compress(a)+c.flush()
        self.inflates(raw,a+b+a)
        self.inflates(b'\x01'+struct.pack('<HH',5,0xFFFA)+b'hello',b'hello')
        self.inflates(b'\x03\x00',b'');self.inflates(b'\x01\x00\x00\xff\xff',b'')
        self.inflates(fixed_stream(),b'aaaa');self.inflates(dyn_a(),b'a')

    def test_refused_archives(self):
        a=dict(name='a',data=b'hello hello hello hello',method=0);b=dict(name='b',data=b'text '*400)
        good=build([a,b]);self.accepted(good,[(e['name'],e['data']) for e in (a,b)])
        R=self.refused
        R(build([a,dict(b,ccrc=1234)]),'CRC')
        R(build([dict(a,ccrc=1234),b]),'CRC')
        R(good[:-10],'end of central directory');R(good[:len(good)//2],'end of central directory')
        R(b'','end of central directory');R(b'PK\x05\x06'+bytes(10),'end of central directory')
        R(good+b'junk','end of central directory')
        R(build([a,dict(b,cmethod=12,lmethod=12)]),'method 12')
        R(build([a,dict(b,cmethod=8,lmethod=0)]),'local header')
        R(build([a,dict(a)]),'duplicate')
        R(build([a,dict(b,lname=b'c')]),'local header');R(build([a,dict(b,lname=b'bb')]),'local header')
        R(build([a,dict(b,ccsize=10**6)]),'outside');R(build([a,dict(b,coff=len(good))]),'local header')
        R(build([a,dict(b,flags=1)]),'encrypted');R(build([a,dict(b,flags=0x40)]),'encrypted')
        R(build([a,dict(b,ccsize=0xFFFFFFFF)]),'ZIP64');R(build([a,dict(b,cusize=0xFFFFFFFF)]),'ZIP64')
        R(build([a,dict(b,coff=0xFFFFFFFF)]),'ZIP64')
        R(build([a,b],eocd=dict(n=0xFFFF)),'ZIP64');R(build([a,b],eocd=dict(cdoff=0xFFFFFFFF)),'ZIP64')
        R(build([a,b],eocd=dict(cdsize=0xFFFFFFFF)),'ZIP64')
        zloc=struct.pack('<IIQI',0x07064b50,0,0,1)
        z=build([a,b]);e=z.rindex(b'PK\x05\x06');R(z[:e]+zloc+z[e:],'ZIP64')
        R(build([a,b],eocd=dict(disk=1)),'split')
        R(build([a,b],eocd=dict(cdoff=len(good))),'outside');R(build([a,b],eocd=dict(cdsize=10**6)),'outside')
        R(build([a,b],eocd=dict(n=3)),'damaged')
        R(build([a,dict(b,name='x'*64)]),'name');R(build([a,dict(b,name=b'')]),'name')
        R(build([a,dict(b,name=b'b\0c')]),'name')
        R(build([dict(name=f'f{i}',method=0) for i in range(4097)]),'more than')
        R(build([a,dict(b,disk=1)]),'disk')
        R(build([dict(a,cusize=len(a['data'])+1)]),'two sizes')
        R(build([a,dict(b,cusize=len(b['data'])+1)]),'deflate');R(build([a,dict(b,cusize=len(b['data'])-1)]),'deflate')
        R(build([a,dict(b,cusize=10**9)]),'impossible')
        R(build([a,dict(b,comp=b'\x07\0\0')]),'deflate');R(build([a,dict(b,comp=b'')]),'impossible')
        R(build([a,dict(name='c',comp=b'')]),'deflate')

    def test_refused_streams(self):
        I=self.inflate_refused
        self.inflates(fixed_stream(0),b'aaaa')
        I(fixed_stream(1),4,'distance before the start')
        I(fixed_stream(0),5,'ends early');I(fixed_stream(0),3,'more than outsize');I(fixed_stream(0),0,'nothing wanted')
        I(fixed_stream(0)[:-1],4,'truncated');I(fixed_stream(0,final=False),4,'no final block');I(b'',0,'no input')
        I(b'\x07',0,'block type 3');I(b'\x07\x00\x00',0,'block type 3')
        I(fixed_stream(sym=286),4,'length symbol 286');I(fixed_stream(sym=287),4,'length symbol 287')
        head=Bits().put(1,1).put(1,2)
        for _ in range(6):head.fixed(200)
        I(head.fixed(265).bytes(),17,'length extra bits cut off')
        head=Bits().put(1,1).put(1,2)
        for _ in range(9):head.fixed(200)
        I(head.fixed(257).code(4,5).bytes(),12,'distance extra bits cut off')
        I(fixed_stream(dist_code=30),4,'distance symbol 30');I(fixed_stream(dist_code=31),4,'distance symbol 31')
        I(b'\x01'+struct.pack('<HH',5,0)+b'hello',5,'bad complement');I(b'\x01'+struct.pack('<HH',5,0xFFFA)+b'hel',5,'stored past input')
        I(b'\x01'+struct.pack('<HH',5,0xFFFA)+b'hello',4,'stored past output');I(b'\x01\x05\x00',5,'stored header cut')
        # hand-made dynamic blocks: the good one decodes, each change is refused
        I(dyn_a(),2,'short output')
        I(Bits().put(1,1).put(2,2).put(0,5).put(0,5).put(0,4).put(1,3).put(1,3).put(1,3).put(1,3).bytes(),1,'over-subscribed code-length code')
        I(dyn_a(cl={18:1,1:2}),1,'incomplete code-length code')
        # incomplete literal and distance codes, a lone one-bit code, an empty distance code
        lit2=zeros(97)+[(2,0,0)]+zeros(158)+[(2,0,0),(1,0,0)]
        I(dyn_a(lit2,tail=((0,2),(1,2)),cl={18:1,1:2,2:2}),1,'incomplete literal code')
        I(dyn_a(zeros(97)+[(1,0,0)]+zeros(158)+[(1,0,0),(2,0,0),(2,0,0)],hdist=2,cl={18:1,1:2,2:2}),1,'incomplete distance code')
        self.inflates(dyn_a(zeros(256)+[(1,0,0),(1,0,0)],tail=((0,1),)),b'')
        self.inflates(dyn_a(zeros(97)+[(1,0,0)]+zeros(158)+[(1,0,0),(0,0,0)],cl={0:1,1:2,18:2}),b'a')
        self.inflates(dyn_a(cl={18:1,1:2,16:2}),b'a')  # complete with a third symbol
        I(dyn_a(zeros(97)+[(1,0,0)]+zeros(158)+[(1,0,0)]+zeros(30)+[(1,0,0)],hlit=287),1,'287 length codes')
        I(dyn_a(zeros(97)+[(1,0,0)]+zeros(158)+[(1,0,0),(1,0,0)]+zeros(30),hdist=31),1,'31 distance codes')
        I(dyn_a([(16,0,2)]+zeros(96)+[(1,0,0)]+zeros(158)+[(1,0,0),(1,0,0)],cl={18:1,1:2,16:2}),1,'repeat of a length before the first')
        base=zeros(97)+[(1,0,0)]+zeros(158)
        I(dyn_a(base+[(1,0,0),(18,0,7)]),1,'repeat past the end')
        I(dyn_a([(18,60,7)]*3,tail=()),1,'all zero, no end of block')
        I(dyn_a(zeros(97)+[(1,0,0),(1,0,0)]+zeros(157)+[(1,0,0)]*2),1,'over-subscribed literal code')
        I(dyn_a(zeros(97)+[(1,0,0)]+zeros(159)+[(1,0,0)]),1,'no end-of-block code')
        I(dyn_a(base[:-1]),1,'lengths cut short')
        I(dyn_a(base+[(1,0,0)]),1,'no distance code lengths left')

class ArcadeZip32(Checks,unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp=tempfile.mkdtemp(prefix='arcade-zip32-');cls.addClassCleanup(shutil.rmtree,cls.tmp,True)
        cls.exe=Path(cls.tmp)/'zip_test32'
        try:
            subprocess.run([*FLAGS,'-m32',*SRC,'-o',str(cls.exe)],check=True,capture_output=True,timeout=60)
            subprocess.run([str(cls.exe),'crc',str(Path(__file__))],check=True,capture_output=True,timeout=10)
        except (subprocess.CalledProcessError,OSError,subprocess.TimeoutExpired) as e:
            reason='gcc -m32 cannot build or run here: '+str(getattr(e,'stderr',None) or e)[:200]
            print('SKIP test_arcade_zip -m32:',reason);raise unittest.SkipTest(reason)
        cls.data=make_data()

    def test_m32(self):
        d=self.data;items=[('text',d['text']),('skew',d['skew']),('repeat',d['repeat']),('empty',b''),('random',d['random'][:100000])]
        for level in (1,9):self.accepted(zf_archive(items,zipfile.ZIP_DEFLATED,level),items)
        self.accepted(zf_archive(items,zipfile.ZIP_STORED),items)
        a=dict(name='a',data=b'hello',method=0)
        self.refused(build([dict(a,ccrc=7)]),'CRC');self.refused(build([a,dict(a)]),'duplicate')
        self.refused(build([a],eocd=dict(n=0xFFFF)),'ZIP64')
        self.inflates(fixed_stream(),b'aaaa');self.inflates(dyn_a(),b'a')
        self.inflate_refused(fixed_stream(1),4);self.inflate_refused(b'\x07',0)

if __name__ == '__main__':
    # The line the review reads (tools/ai_workflow/review.py): the last line, on a pass only.
    import sys, time
    started = time.monotonic()
    result = unittest.main(exit=False).result
    took = time.monotonic() - started
    bad, ran = len(result.failures) + len(result.errors), result.testsRun
    if bad or not ran:
        print(f"{bad} of {ran} tests FAILED in {took:.1f} seconds")
        sys.exit(1)
    print(f"all {ran} tests passed in {took:.1f} seconds")
