import json
import struct
import tempfile
import unittest
import zipfile
from pathlib import Path

from compare import (art_comparison, compare_character, cps2_catalog, find_seed,
                     load_arcade, metadata, picture_key, profile, ps1_catalog,
                     render_catalog, sequences, timeline_comparison, timeline_signature)
from formats import (CPS2Reader, InputError, bank_permutation, cpu_order,
                     decode_tile, encode_tile, lane_region, picture)


def pac(code, gfx):
    header=bytearray(2048)
    struct.pack_into('<I',header,0,2)
    body=bytearray()
    for i,(slot,data) in enumerate([(4,code),(1,gfx)]):
        struct.pack_into('<HHII',header,32+i*32,slot,0,len(data),struct.unpack_from('<I',data)[0])
        body.extend(data);body.extend(bytes((-len(data))%2048))
    return bytes(header+body)


def ps_fixture():
    code=bytearray(512)
    base=0x801b0000
    struct.pack_into('<3H',code,100,0,0,0)
    struct.pack_into('<6H',code,200,1,0,0,0,0,0)
    struct.pack_into('<HHIhH',code,212,5,0x8000,0,0,1)
    struct.pack_into('<I',code,224,base+212)
    struct.pack_into('<I',code,316,base+224)
    gfx=struct.pack('<IH64H',4,0,*([0x1111]*64))
    return pac(code,gfx),(base+100,base+200,base+300)


def cp_fixture(attr=0,count=1,tile=0):
    data=bytearray(400)
    struct.pack_into('>I',data,100,160)
    struct.pack_into('>hh',data,160,0,0)
    struct.pack_into('>HHI8H',data,228,5,0x8000,256,*([0]*8))
    struct.pack_into('>I',data,252,228)
    struct.pack_into('>4H2H',data,256,2,count,0,0,tile,attr)
    struct.pack_into('>H',data,268,0x1234)
    return data


def pic(values=None,origin=None):
    p=dict(width=16,height=16,pixels=bytes([1]*256) if values is None else values,
           origin=[0,0] if origin is None else origin,palettes=[0])
    p['key']=picture_key(p)
    return p


class Formats(unittest.TestCase):
    def test_lanes_and_banks(self):
        self.assertEqual(lane_region([b'abcd',b'efgh',b'ijkl',b'mnop']),b'abefijmncdghklop')
        data=b''.join(bytes([i])*8 for i in range(16))
        expected=[0,2,4,6,1,3,5,7,8,10,12,14,9,11,13,15]
        self.assertEqual(bank_permutation(data,64),b''.join(bytes([i])*8 for i in expected))
        for x,b in [(b'x',64),(bytes(32),24),(bytes(8),8)]:
            with self.assertRaises(InputError):bank_permutation(x,b)
        with self.assertRaises(InputError):lane_region([b'ab']*3)
        with self.assertRaises(InputError):lane_region([b'a']*4)
        self.assertEqual(cpu_order(b'abcd'),b'badc')
        with self.assertRaises(InputError):cpu_order(b'a')

    def test_planes_known_vector(self):
        p=bytearray(256);p[0],p[7],p[8],p[255]=1,8,2,15
        b=bytearray(128);b[0],b[3],b[5]=128,1,128;b[124:128]=b'\1'*4
        self.assertEqual(encode_tile(p),b)
        self.assertEqual(decode_tile(b),p)
        with self.assertRaises(InputError):decode_tile(b'x')

    def test_rectangles_flips_and_wrap(self):
        for attr,expected in [(0x1100,[1,2,17,18]),(0x1120,[2,1,18,17]),(0x1140,[17,18,1,2]),(0x1160,[18,17,2,1])]:
            pose=CPS2Reader(cp_fixture(attr,4,1),100).pose(256)
            self.assertEqual([c['tile'] for c in pose['cells']],expected)
            self.assertTrue(all(c['flip_x']==bool(attr&32) and c['flip_y']==bool(attr&64) for c in pose['cells']))
        p=CPS2Reader(cp_fixture(0x0100,2,15),100).pose(256)
        self.assertEqual([c['tile'] for c in p['cells']],[15,0])
        with self.assertRaises(InputError):CPS2Reader(cp_fixture(0x80),100).pose(256)
        with self.assertRaises(InputError):CPS2Reader(cp_fixture(0,2),100).pose(256)

    def test_crop_anchor_and_pixel_flip(self):
        tile=bytearray(256);tile[0],tile[255]=1,2
        pose=dict(cells=[dict(tile=0,x=-16,y=-32,flip_x=True,flip_y=True,palette=3)])
        p=picture(pose,lambda n: tile)
        self.assertEqual((p['pixels'][0],p['pixels'][-1]),(2,1))
        self.assertEqual(p['origin'],[-16,-32]);self.assertEqual(p['palettes'],[3])
        tile=bytearray(256);tile[17]=1
        pose['cells'][0].update(flip_x=False,flip_y=False)
        p=picture(pose,lambda n:tile)
        self.assertEqual((p['width'],p['height'],p['origin'],p['pixels']),(1,1,[-15,-31],b'\1'))
        self.assertEqual(picture(pose,lambda n:bytes(256))['width'],0)


class Catalogs(unittest.TestCase):
    def test_ps_catalog_and_zero_padding(self):
        data,mapping=ps_fixture();poses,seqs,decode=ps1_catalog(data,mapping)
        self.assertEqual(len(poses),1);self.assertEqual(len(seqs),1)
        self.assertEqual(decode(0),bytes([1]*256))
        # Move animation/table by two bytes, requiring explicit zero padding.
        code=bytearray(512);base=0x801b0000
        struct.pack_into('<3H',code,100,0,0,0)
        struct.pack_into('<6H',code,200,1,0,0,0,0,0)
        struct.pack_into('<HHIhH',code,214,5,0x8000,0,0,1)
        struct.pack_into('<I',code,226,base+214);struct.pack_into('<I',code,316,base+226)
        gfx=struct.pack('<IH64H',4,0,*([0x1111]*64))
        self.assertEqual(len(ps1_catalog(pac(code,gfx),mapping)[0]),1)
        code[212]=1
        with self.assertRaises(InputError):ps1_catalog(pac(code,gfx),mapping)

    def test_seed_and_catalog(self):
        data,mapping=ps_fixture();poses,seqs,decode=ps1_catalog(data,mapping)
        pics,_=render_catalog(poses,decode)
        cp=CPS2Reader(cp_fixture(),100)
        cps,cs,discovery=cps2_catalog(cp,lambda n:bytes([1]*256),poses[0],pics[0])
        self.assertEqual((list(cps),len(cs),discovery['art_end']),([256],1,268))
        data=cp_fixture();data[280:292]=data[256:268]
        with self.assertRaisesRegex(InputError,'2 matches'):find_seed(CPS2Reader(data,100),lambda n:bytes([1]*256),poses[0],pics[0])
        with self.assertRaisesRegex(InputError,'0 matches'):find_seed(cp,lambda n:bytes([2]*256),poses[0],pics[0])

    def test_timers_loops_terminal_and_bounds(self):
        data=bytearray(100)
        struct.pack_into('<HHIhH',data,0,3,0,0,0,1)
        struct.pack_into('<HHIhH',data,12,0xfff,0x8000,0,0,1)
        s=sequences(data,0,24,'ps1',{0})[0]
        self.assertTrue(s['terminal_hold']);self.assertEqual(s['loop_index'],1)
        signature=timeline_signature(s,{0:pic()})
        self.assertEqual(len(signature[0]),1);self.assertIsNotNone(signature[2])
        for end in [23,25]:
            with self.assertRaises(InputError):sequences(data,0,end,'ps1',{0})
        struct.pack_into('<h',data,20,5)
        with self.assertRaises(InputError):sequences(data,0,24,'ps1',{0})

    def test_external_transitions_are_not_local_timeline_passes(self):
        data=bytearray(56)
        for off,d,target in [(0,3,28),(28,5,28)]:
            struct.pack_into('>HHI8H',data,off,d,0x8000,200,*([0]*8))
            struct.pack_into('>I',data,off+24,target)
        s=sequences(data,0,56,'cps2',{200})
        self.assertIsNone(s[0]['loop_index'])
        with self.assertRaises(InputError):timeline_signature(s[0],{200:pic()})
        struct.pack_into('>I',data,24,999)
        with self.assertRaises(InputError):sequences(data,0,56,'cps2',{200})

    def test_unresolved_decode_and_profile_ambiguity(self):
        def bad(n):raise InputError('unsupported codec')
        poses={0:dict(cells=[dict(tile=0,x=0,y=0,flip_x=False,flip_y=False,palette=0)])}
        pics,errors=render_catalog(poses,bad)
        self.assertFalse(pics);self.assertIn('unsupported',errors[0])
        with tempfile.TemporaryDirectory() as td:
            root=Path(td);folder=root/'ps1/src/slot04_00';folder.mkdir(parents=True)
            src='obj->field_98 = data_801b0100_slot04_00; obj->field_9c = data_801b0200_slot04_00; dst[i] = data_801b0300_slot04_00[i];'
            (folder/'a.c').write_text(src)
            self.assertEqual(profile(root,'00')[0],(0x801b0100,0x801b0200,0x801b0300))
            (folder/'b.c').write_text(src.replace('801b0200','801b0400'))
            with self.assertRaises(InputError):profile(root,'00')

    def test_small_end_to_end_character(self):
        with tempfile.TemporaryDirectory() as td:
            root=Path(td);folder=root/'ps1/src/slot04_00';folder.mkdir(parents=True)
            (folder/'a.c').write_text('obj->field_98 = data_801b0064_slot04_00; obj->field_9c = data_801b00c8_slot04_00; dst[i] = data_801b012c_slot04_00[i];')
            (root/'PL00.PAC').write_bytes(ps_fixture()[0])
            r=compare_character(root,root,'00',CPS2Reader(cp_fixture(),100),lambda n:bytes([1]*256))
            self.assertTrue(r['complete_decoding']);self.assertEqual(r['artwork']['exact_unique'],1)
            self.assertEqual(r['timelines']['counts'],{'exact_visual_timers':1})
            self.assertNotIn('pixels',r['ps1_pictures']['0'])
            json.dumps(r)

    def test_bad_zip_member_length(self):
        with tempfile.TemporaryDirectory() as td:
            p=Path(td)/'input.zip'
            with zipfile.ZipFile(p,'w') as z:z.writestr('sz2u.03',b'wrong size')
            with self.assertRaisesRegex(InputError,'ROM length'):load_arcade(p)


class Comparisons(unittest.TestCase):
    def test_duplicate_art_and_anchor_separation(self):
        p,c=pic(),pic(origin=[2,0])
        a=art_comparison({0:p,10:p},{20:c})
        self.assertEqual((a['ps1_records'],a['ps1_unique'],a['exact_unique']),(2,1,1))
        self.assertEqual(len(a['anchor_differences']),1)
        self.assertEqual(a['ps1_only'],[])

    def test_changed_pixels_are_candidates_not_move_pairs(self):
        p=pic();c=pic(bytes([2]+[1]*255))
        a=art_comparison({0:p},{1:c},partial=True)
        self.assertEqual(a['exact_unique'],0)
        self.assertEqual(a['changed_pixel_candidates'][0]['different_pixels'],1)
        self.assertIn('candidate',a['changed_pixel_candidates'][0]['classification'])
        self.assertEqual(a['unmatched_status'],'unresolved coverage')

    def test_timelines_not_paired_by_position(self):
        def seq(addr,d):return dict(address=addr,rows=[dict(art=0,duration=d)],terminal_hold=False,loop_index=0)
        ps=[seq(1,5),seq(2,6)];cp=[seq(3,6),seq(4,5),seq(5,5)]
        r=timeline_comparison(ps,cp,{0:pic()},{0:pic()})
        self.assertEqual(r['comparisons'][0]['cps2'],[4,5])
        self.assertEqual(r['comparisons'][1]['cps2'],[3])
        r=timeline_comparison(ps,[seq(9,7)],{0:pic()},{0:pic()})
        self.assertEqual(r['counts'],{'same_drawings_different_timers':2})
        r=timeline_comparison(ps,cp,{},{0:pic()})
        self.assertEqual(len(r['unresolved']),2)
        self.assertFalse(r['counts'])




class NewCoverageControls(unittest.TestCase):
    def test_mirror_equivalence_is_separate_from_exact(self):
        p=pic(bytes([1,2]+[1]*254));q=pic(b''.join(p['pixels'][y*16:(y+1)*16][::-1] for y in range(16)))
        r=art_comparison({0:p},{1:q})
        self.assertEqual(r['exact_unique'],0)
        self.assertEqual(r['shared_orientation_unique'],1)
        self.assertEqual(r['cps2_only'],[1]);self.assertEqual(r['cps2_only_after_flips'],[])
        q=pic(bytes([3]*256));r=art_comparison({0:p},{1:q})
        self.assertEqual(r['cps2_only_after_flips'],[1])

    def test_cps2_extra_visual_timeline_reported(self):
        def seq(a,d):return dict(address=a,rows=[dict(art=0,duration=d)],terminal_hold=False,loop_index=0)
        r=timeline_comparison([seq(0,5)],[seq(10,5),seq(20,6)],{0:pic()},{0:pic()})
        self.assertEqual(r['cps2_without_ps1_visual_timers'],[20])
        self.assertEqual(r['cps2_without_ps1_drawing_order'],[])

    def test_cli_report_partial_status_and_no_overwrite(self):
        import io
        from contextlib import redirect_stdout,redirect_stderr
        from unittest.mock import patch
        import compare
        with tempfile.TemporaryDirectory() as td:
            root=Path(td);folder=root/'ps1/src/slot04_00';folder.mkdir(parents=True)
            (folder/'a.c').write_text('obj->field_98 = data_801b0064_slot04_00; obj->field_9c = data_801b00c8_slot04_00; dst[i] = data_801b012c_slot04_00[i];')
            input_bytes=ps_fixture()[0];(root/'PL00.PAC').write_bytes(input_bytes)
            zip_path=root/'input.zip';zip_path.write_bytes(b'synthetic carrier, mocked loader')
            output=root/'out'
            args=['--repo',str(root),'--ps1-pac-dir',str(root),'--arcade-zip',str(zip_path),'--characters','00,10','--output',str(output)]
            with patch.object(compare,'load_arcade',return_value=(cp_fixture(),b'',{})),patch.object(compare,'CPS2Reader',side_effect=lambda d:CPS2Reader(d,100)),patch.object(compare,'tile_decoder',return_value=lambda n:bytes([1]*256)),redirect_stdout(io.StringIO()):
                self.assertEqual(compare.main(args),2)
            report=json.loads((output/'report.json').read_text())
            self.assertTrue(report['characters'][0]['complete_decoding'])
            self.assertFalse(report['characters'][1]['complete_decoding'])
            self.assertEqual((root/'PL00.PAC').read_bytes(),input_bytes)
            with patch.object(compare,'load_arcade') as loader,redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit) as caught:compare.main(args)
                self.assertEqual(caught.exception.code,2);loader.assert_not_called()


class CrossCatalogControls(unittest.TestCase):
    def test_other_archive_is_not_a_whole_disc_omission(self):
        from compare import cross_catalog_matches
        key=pic()['key']
        results=[dict(character='10',complete_decoding=True,ps1_pictures={},cps2_pictures={'100':dict(key=key)},artwork=dict(cps2_only_after_flips=[100])),
                 dict(character='16',complete_decoding=True,ps1_pictures={'200':dict(key=key)},cps2_pictures={},artwork=dict(cps2_only_after_flips=[])),
                 dict(character='17',complete_decoding=False,unresolved='unsupported')]
        summary=cross_catalog_matches(results)
        art=results[0]['artwork']
        self.assertEqual(art['cps2_not_seen_in_decoded_ps1_catalogs'],[])
        self.assertEqual(art['cps2_only_seen_elsewhere_ps1'][0]['other_ps1_catalogs'],[dict(character='16',artwork=200)])
        self.assertEqual(summary['unresolved_characters'],['17'])
        results[1]['ps1_pictures']={}
        cross_catalog_matches(results)
        self.assertEqual(results[0]['artwork']['cps2_not_seen_in_decoded_ps1_catalogs'],[100])


if __name__ == '__main__':
    unittest.main()
