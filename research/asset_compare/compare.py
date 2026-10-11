#!/usr/bin/env python3
"""Compare PS1 Japan character catalogs against user-supplied CPS2 sfa2ur1.

Reads one arcade ZIP, selected PLxx.PAC files and reconstructed initialization C.
Standard library only. Typical cost seconds per character; never boots a game.
Writes a new private JSON/CSV report, optional neutral previews. See README.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
import struct
import time
import zipfile
from collections import defaultdict
from pathlib import Path

from formats import (InputError, Reader, CPS2Reader, bank_permutation, checked,
                     cpu_order, lane_region, pac_chunks, picture, png, tile_decoder)

PROGRAM_SHA1 = (
    '334c33f9eb324d71443cc9c44e94f5a72451fa3f',
    'a01b57daba4c255d5f07465c553bcbfe51d9ab0d',
    'ae2a4e2903beec1f10fff6edac1a2385d6ac1c38',
    '4ea033834c7b260877335296f88c0db484dea289',
    'b19bfe970b217c96e782860fc3ae3fcb976ed30d',
    'a1d475066d36de7cc5d931671ccdcd89737bc7ee',
)
KNOWN_NAMES = {'00': 'Ryu', '10': 'Zangief'}
PS1_BASE = 0x801b0000


def digest(data):
    return hashlib.sha256(data).hexdigest()


def load_arcade(path):
    hashes = {}
    with zipfile.ZipFile(path) as archive:
        if len(archive.namelist()) != len(set(archive.namelist())):
            raise InputError('duplicate ZIP member names')

        def member(name, size):
            if archive.getinfo(name).file_size != size:
                raise InputError(f'unexpected ROM length: {name}')
            data = archive.read(name)
            hashes[name] = digest(data)
            return data

        parts = [member(f'sz2u.{n:02d}', 0x80000) for n in range(3, 9)]
        if tuple(hashlib.sha1(v).hexdigest() for v in parts) != PROGRAM_SHA1:
            raise InputError('unsupported program layout, tool currently supports sfa2ur1 only')
        lanes = []
        for numbers, size in [((13, 15, 17, 19), 0x400000), ((14, 16, 18, 20), 0x100000)]:
            lanes.append(lane_region([member(f'sz2.{n:02d}m', size) for n in numbers]))
    return cpu_order(b''.join(parts)), bank_permutation(b''.join(lanes)), hashes


def profile(repo, cid):
    found = set()
    sources = {}
    folder = repo / 'ps1/src' / f'slot04_{cid}'
    for path in sorted(folder.glob('*.c')):
        text = path.read_text()
        coords = re.findall(r'obj->field_98\s*=\s*data_([0-9a-f]{8})_slot04_', text)
        arts = re.findall(r'obj->field_9c\s*=\s*data_([0-9a-f]{8})_slot04_', text)
        tables = re.findall(r'dst\[i\]\s*=\s*data_([0-9a-f]{8})_slot04_', text)
        if len(coords) == len(arts) == len(tables) == 1:
            found.add(tuple(int(v[0], 16) for v in (coords, arts, tables)))
            sources[str(path.relative_to(repo))] = digest(path.read_bytes())
    if len(found) != 1:
        raise InputError('missing/ambiguous character initialization mapping')
    return next(iter(found)), sources


def sequences(data, start, end, platform, art_addresses, base=0):
    """Read every record in a bounded catalog, including terminal/loop metadata."""
    groups, rows, pos, all_rows = [], [], start, set()
    size = 12 if platform == 'ps1' else 24
    while pos < end:
        raw = checked(data, pos - base, size)
        if pos + size > end:
            raise InputError('sequence crosses catalog boundary')
        if platform == 'ps1':
            duration, flags, art, loop, _ = struct.unpack('<HHIhH', raw)
        else:
            duration, flags, art = struct.unpack_from('>HHI', raw)
        if not 0 < duration <= 0xfff or art not in art_addresses:
            raise InputError('invalid timer or artwork reference in sequence catalog')
        rows.append(dict(address=pos, duration=duration, art=art))
        all_rows.add(pos)
        pos += size
        if flags & 0x8000:
            if platform == 'ps1':
                target = rows[-1]['address'] + loop * size
            else:
                target = struct.unpack('>I', checked(data, pos-base, 4))[0]
                pos += 4
            indices = {r['address']: i for i, r in enumerate(rows)}
            terminal = duration == 0xfff and target == rows[-1]['address']
            groups.append(dict(address=rows[0]['address'], rows=rows, terminal_hold=terminal,
                               loop_index=indices.get(target), loop_address=target))
            rows = []
    if pos != end or rows:
        raise InputError('unfinished sequence catalog')
    if any(g['loop_address'] not in all_rows for g in groups):
        raise InputError('loop/transition outside sequence catalog')
    return groups


def ps1_catalog(data, mapping):
    chunks = pac_chunks(data)
    code = [c['data'] for c in chunks if (c['slot'], c['table']) == (4, 0)]
    gfx = [c['data'] for c in chunks if (c['slot'], c['table']) == (1, 0)]
    if len(code) != 1 or len(gfx) != 1:
        raise InputError('expected one character module and graphics chunk')
    coord, art, dispatch = mapping
    reader = Reader(code[0], PS1_BASE)
    table = reader.pointer(dispatch + 16)
    end = reader.pointer(table)
    if not PS1_BASE <= coord < art < end <= table <= PS1_BASE + len(code[0]):
        raise InputError('character mapping ranges disagree with PAC')
    poses, pos = {}, art
    while pos < end:
        if end - pos < 10 and not any(reader.read(pos, end-pos)):
            pos = end
            break
        pose = reader.pose(art, coord, pos-art)
        if pose['end'] > end:
            raise InputError('composition crosses animation boundary')
        poses[pos-art] = pose
        pos = pose['end']
    seqs = sequences(code[0], end, table, 'ps1', poses, PS1_BASE)
    return poses, seqs, tile_decoder(gfx[0], 'ps1')


def picture_key(pic):
    return digest(struct.pack('>HH', pic['width'], pic['height']) + pic['pixels'])


def render_catalog(poses, decode):
    pictures, failures = {}, {}
    for address, pose in poses.items():
        try:
            pic = picture(pose, decode)
            pic['key'] = picture_key(pic)
            pictures[address] = pic
        except InputError as exc:
            failures[address] = str(exc)
    return pictures, failures


def find_seed(reader, decode, seed_pose, seed_pic):
    needle = struct.pack('>HH', 2, len(seed_pose['cells']))
    pos, matches = reader.data.find(needle), []
    while pos != -1:
        if not pos % 2:
            try:
                pic = picture(reader.pose(pos), decode)
                if picture_key(pic) == seed_pic['key'] and pic['origin'] == seed_pic['origin']:
                    matches.append(pos)
            except InputError:
                pass
        pos = reader.data.find(needle, pos+1)
    if len(matches) != 1:
        raise InputError(f'CPS2 first-drawing discovery has {len(matches)} matches, not one')
    return matches[0]


def cps2_catalog(reader, decode, seed_pose, seed_pic):
    start = find_seed(reader, decode, seed_pose, seed_pic)
    poses, pos = {}, start
    while pos < len(reader.data)-8 and reader.data[pos:pos+2] == b'\x00\x02':
        pose = reader.pose(pos)
        poses[pos] = pose
        pos = pose['end']
    next_word = struct.unpack('>H', checked(reader.data, pos, 2))[0]
    if next_word in (0, 1):
        raise InputError('catalog meets an unsupported possible composition mode')
    # Earliest valid reference to the first drawing starts a complete sequence
    # catalog ending at its composition catalog. Failed alternatives stay local.
    needle = struct.pack('>I', start)
    ref, seqs, sequence_start = reader.data.find(needle, 4, start), None, None
    while ref != -1:
        candidate = ref - 4
        if not candidate % 2:
            try:
                seqs = sequences(reader.data, candidate, start, 'cps2', poses)
                sequence_start = candidate
                break
            except InputError:
                pass
        ref = reader.data.find(needle, ref+1, start)
    if seqs is None:
        raise InputError('could not bound preceding CPS2 animation catalog')
    return poses, seqs, dict(art_start=start, art_end=pos, sequence_start=sequence_start,
                             boundary_word=next_word, discovery='exact first-drawing seed, contiguous mode-2 catalog')


def art_comparison(ps, cp, partial=False):
    left, right = defaultdict(list), defaultdict(list)
    for address, pic in ps.items():
        left[pic['key']].append(address)
    for address, pic in cp.items():
        right[pic['key']].append(address)
    common = left.keys() & right.keys()
    anchor_differences = []
    for key in sorted(common):
        pa = {tuple(ps[a]['origin']) for a in left[key]}
        ca = {tuple(cp[a]['origin']) for a in right[key]}
        if pa != ca:
            anchor_differences.append(dict(ps1=left[key], cps2=right[key], ps1_origins=sorted(pa), cps2_origins=sorted(ca)))
    # Distinguish genuinely unmatched pictures from baked storage flips.
    def mirror_key(pic):
        w, h, pixels = pic['width'], pic['height'], pic['pixels']
        rows = [pixels[y*w:(y+1)*w] for y in range(h)]
        variants = [pixels, b''.join(r[::-1] for r in rows), b''.join(reversed(rows)), pixels[::-1]]
        return min(digest(struct.pack('>HH', w, h)+v) for v in variants)
    pm = {a: mirror_key(p) for a, p in ps.items()}
    cm = {a: mirror_key(p) for a, p in cp.items()}
    ps_unflipped = set(pm.values())
    cp_unflipped = set(cm.values())
    ps_only = [a for k in sorted(left.keys()-right.keys()) for a in left[k]]
    cp_only = [a for k in sorted(right.keys()-left.keys()) for a in right[k]]
    near = []
    for a in ps_only:
        p = ps[a]
        for b in cp_only:
            c = cp[b]
            if (p['width'], p['height']) != (c['width'], c['height']) or not p['pixels']:
                continue
            different = sum(x != y for x, y in zip(p['pixels'], c['pixels']))
            if different <= len(p['pixels']) // 20:
                near.append(dict(ps1=a, cps2=b, different_pixels=different,
                                 classification='candidate only, move correspondence unknown'))
    near.sort(key=lambda c: (c['different_pixels'], c['ps1'], c['cps2']))
    return dict(ps1_orientation_unique=len(ps_unflipped), cps2_orientation_unique=len(cp_unflipped),
                shared_orientation_unique=len(ps_unflipped & cp_unflipped),
                ps1_only_after_flips=[a for a in ps_only if pm[a] not in cp_unflipped],
                cps2_only_after_flips=[a for a in cp_only if cm[a] not in ps_unflipped],
                ps1_records=len(ps), cps2_records=len(cp), ps1_unique=len(left), cps2_unique=len(right),
                exact_unique=len(common), exact_matches=[dict(ps1=left[k], cps2=right[k]) for k in sorted(common)],
                ps1_only=ps_only, cps2_only=cp_only, anchor_differences=anchor_differences,
                changed_pixel_candidates=near[:100], unmatched_status='unresolved coverage' if partial else 'not present in other decoded catalog, not proven missing move')


def timeline_signature(seq, pics, timers=True):
    def art(row):
        p = pics[row['art']]
        return p['key'], tuple(p['origin'])
    # A held terminal drawing is still compared, but its sentinel is not timed.
    rows = seq['rows'][:-1] if seq['terminal_hold'] else seq['rows']
    content = tuple((art(r), r['duration']) if timers else art(r) for r in rows)
    held = art(seq['rows'][-1]) if seq['terminal_hold'] else None
    if seq['loop_index'] is None:
        raise InputError('external animation transition, not an isolated timeline')
    return content, seq['loop_index'], held


def timeline_comparison(ps_seqs, cp_seqs, ps, cp):
    outputs, unresolved, counts = [], [], defaultdict(int)
    exact, order = defaultdict(list), defaultdict(list)
    ps_exact, ps_order = set(), set()
    for seq in cp_seqs:
        try:
            exact[timeline_signature(seq, cp)].append(seq['address'])
            order[timeline_signature(seq, cp, False)].append(seq['address'])
        except (KeyError, InputError):
            unresolved.append(dict(platform='cps2', address=seq['address']))
    for seq in ps_seqs:
        try:
            signature, ordered_signature = timeline_signature(seq, ps), timeline_signature(seq, ps, False)
            ps_exact.add(signature)
            ps_order.add(ordered_signature)
            matches = exact.get(signature, [])
            ordered = order.get(timeline_signature(seq, ps, False), [])
            status = 'exact_visual_timers' if matches else 'same_drawings_different_timers' if ordered else 'unpaired'
            outputs.append(dict(ps1=seq['address'], status=status, cps2=matches or ordered,
                                pairing='by full visual content, not move ID',
                                timed_records=len(seq['rows'])-int(seq['terminal_hold'])))
            counts[status] += 1
        except (KeyError, InputError):
            unresolved.append(dict(platform='ps1', address=seq['address']))
    return dict(cps2_without_ps1_visual_timers=[a for key, addresses in exact.items() if key not in ps_exact for a in addresses],
                cps2_without_ps1_drawing_order=[a for key, addresses in order.items() if key not in ps_order for a in addresses],
                ps1_sequences=len(ps_seqs), cps2_sequences=len(cp_seqs), counts=dict(counts),
                comparisons=outputs, unresolved=unresolved)


def metadata(pictures):
    return {str(a): {k: v for k, v in pic.items() if k != 'pixels'} for a, pic in pictures.items()}


def compare_character(repo, pac_dir, cid, reader, decode, preview_dir=None, previews=0):
    mapping, sources = profile(repo, cid)
    data = (pac_dir / f'PL{cid.upper()}.PAC').read_bytes()
    poses, seqs, ps_decode = ps1_catalog(data, mapping)
    ps, ps_errors = render_catalog(poses, ps_decode)
    if 0 not in ps:
        raise InputError('first PS1 drawing undecoded, cannot discover CPS2 catalog')
    cp_poses, cp_seqs, discovery = cps2_catalog(reader, decode, poses[0], ps[0])
    cp, cp_errors = render_catalog(cp_poses, decode)
    partial = bool(ps_errors or cp_errors)
    art = art_comparison(ps, cp, partial)
    timing = timeline_comparison(seqs, cp_seqs, ps, cp)
    if preview_dir and previews:
        preview_dir.mkdir()
        for platform, addresses, pics in [('ps1', art['ps1_only'], ps), ('cps2', art['cps2_only'], cp)]:
            for address in addresses[:previews]:
                pic = pics[address]
                if pic['width']:
                    (preview_dir / f'{platform}-{address:x}.png').write_bytes(png(pic['width'], pic['height'], pic['pixels']))
    return dict(character=cid, name=KNOWN_NAMES.get(cid, f'PL{cid.upper()}'),
                ps1_pac_sha256=digest(data), initializer_sources=sources,
                ps1_mapping=list(mapping), cps2_discovery=discovery,
                artwork=art, timelines=timing, ps1_pictures=metadata(ps), cps2_pictures=metadata(cp),
                unresolved=dict(ps1_decode=ps_errors, cps2_decode=cp_errors),
                complete_decoding=not partial, scope='contiguous character catalogs, no runtime or palette-color comparison')



def cross_catalog_matches(results):
    """Do not confuse one PAC's omissions with assets absent from the whole disc."""
    index = defaultdict(list)
    for result in results:
        for address, pic in result.get('ps1_pictures', {}).items():
            index[pic['key']].append(dict(character=result['character'], artwork=int(address)))
    for result in results:
        art = result.get('artwork')
        if art is None:
            continue
        elsewhere, unseen = [], []
        for address in art['cps2_only_after_flips']:
            key = result['cps2_pictures'][str(address)]['key']
            candidates = [r for r in index.get(key, []) if r['character'] != result['character']]
            if candidates:
                elsewhere.append(dict(cps2=address, other_ps1_catalogs=candidates))
            else:
                unseen.append(address)
        art['cps2_only_seen_elsewhere_ps1'] = elsewhere
        art['cps2_not_seen_in_decoded_ps1_catalogs'] = unseen
    return dict(decoded_ps1_catalogs=sum('ps1_pictures' in r for r in results),
                unresolved_characters=[r['character'] for r in results if not r['complete_decoding']],
                status='cross-catalog exact pixels only; unresolved catalogs prevent a whole-disc absence claim')


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--ps1-pac-dir', type=Path, required=True)
    ap.add_argument('--arcade-zip', type=Path, required=True)
    ap.add_argument('--characters', default='00,10', help='comma-separated hex IDs, or all')
    ap.add_argument('--repo', type=Path, default=Path(__file__).resolve().parents[2])
    ap.add_argument('--output', type=Path, required=True, help='new private output directory')
    ap.add_argument('--previews', type=int, default=0, help='maximum unmatched previews per platform/character')
    a = ap.parse_args(argv)
    if not 0 <= a.previews <= 100:
        ap.error('--previews must be 0..100')
    ids = [f'{i:02x}' for i in range(24)] if a.characters == 'all' else a.characters.lower().split(',')
    if not ids or len(ids) != len(set(ids)) or any(not re.fullmatch(r'[0-9a-f]{2}', c) or int(c, 16) >= 24 for c in ids):
        ap.error('--characters requires distinct hex IDs from 00 through 17')
    if a.output.exists():
        ap.error('--output must not exist (inputs and earlier evidence are never overwritten)')
    started = time.monotonic()
    cpu, gfx, hashes = load_arcade(a.arcade_zip)
    reader, decode = CPS2Reader(cpu), tile_decoder(gfx, 'cps2')
    a.output.mkdir(parents=True, exist_ok=False)
    results = []
    for cid in ids:
        try:
            result = compare_character(a.repo, a.ps1_pac_dir, cid, reader, decode, a.output/cid, a.previews)
            art = result['artwork']
            print(f'{result["name"]}: {art["exact_unique"]} exact unique pictures; PS1 {art["ps1_unique"]}, CPS2 {art["cps2_unique"]}; timers {result["timelines"]["counts"]}', flush=True)
        except (InputError, OSError) as exc:
            result = dict(character=cid, complete_decoding=False, unresolved=str(exc))
            print(f'PL{cid.upper()}: unresolved: {exc}', flush=True)
        results.append(result)
    cross_catalog = cross_catalog_matches(results)
    report = dict(cross_catalog=cross_catalog, schema=1, elapsed_seconds=round(time.monotonic()-started, 3),
                  input_hashes=dict(arcade_zip=digest(a.arcade_zip.read_bytes()), arcade_members=hashes),
                  tool_hashes={p.name: digest(p.read_bytes()) for p in Path(__file__).parent.glob('*.py') if not p.name.startswith('test_')},
                  limitations=['indexed pixels, not original palette colors', 'contiguous catalogs, not runtime reachability',
                               'content-paired timelines, not verified move IDs', 'supplied graphics hashes accepted as-is'],
                  characters=results)
    (a.output/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    with (a.output/'summary.csv').open('w', newline='') as stream:
        writer=csv.writer(stream)
        writer.writerow(['character','name','complete_decoding','PS1_unique','CPS2_unique','exact_unique','PS1_sequences','CPS2_sequences','exact_visual_timers','different_timers','unpaired'])
        for r in results:
            art, timing=r.get('artwork', {}),r.get('timelines', {})
            counts=timing.get('counts', {})
            writer.writerow([r['character'],r.get('name',''),r['complete_decoding'],art.get('ps1_unique'),art.get('cps2_unique'),art.get('exact_unique'),timing.get('ps1_sequences'),timing.get('cps2_sequences'),counts.get('exact_visual_timers'),counts.get('same_drawings_different_timers'),counts.get('unpaired')])
    print(f'Report: {a.output / "report.json"} ({report["elapsed_seconds"]} s)', flush=True)
    return 0 if all(r['complete_decoding'] for r in results) else 2


if __name__ == '__main__':
    try:
        raise SystemExit(main())
    except (InputError, OSError, KeyError, zipfile.BadZipFile) as exc:
        raise SystemExit(f'Input/layout error: {exc}')
