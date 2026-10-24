#!/usr/bin/env python3
"""Analysis tool: the code ranges of the game's module chunks and their data pins (the table that code_pins.c reads).

What it does. modules.c pins each overlay module image by the SHA-256 of its chunk as on the disc, and the port runs its own C for every
function of a module, so the bytes of those functions are never executed. A pack (the private copy of what the port needs from the user's
disc) can therefore hold the chunk with zeros where the functions are, if the runtime checks such a chunk against a data pin: the SHA-256
of the chunk with the same ranges zeroed (port/arcade/code_pins.h is the contract). For every [[image]] of ps1/src/build.toml this tool
reads the image's chunk from the disc (the table-0 chunk of the image's slot in the image's archive; only the archive's file name is used,
the path in build.toml is a placeholder), checks its SHA-256 against the image's pin, and derives the ranges:

  established  every function of the image that the matching build declares (the [[unit]] entries whose image is this one: exact address and
               size). For an image that is `like` another (the second side of a fighter: the base image's units linked again at another
               address) the base image's functions moved by the difference of the two addresses; each moved range must lie inside the chunk.
               A unit named in the image's `leave_out` is not moved (none is left out in build.toml today).
  inventory    every other function of the same content in ps1/inventory/modules.tsv (the rows of the image's archive and slot, which
               contents.tsv names as the content's first archive) whose last two words are `jr ra` (0x03e00008 at end - 8) and its delay
               slot. A row that fails is skipped and counted (reason: not jr ra, outside the chunk, too short, unaligned); a row wholly
               inside an established range is counted as already covered.

Ranges are merged (sorted, not overlapping, touching ranges joined) and stored as offsets in the chunk (address - image address). The
data pin is the SHA-256 of the chunk with the ranges zeroed. The tool prints, per image, the chunk size, the bytes zeroed (established,
added by the inventory, skipped rows) and the share, then totals by class of image (fighters: PLnn files; stages: STAGEnn; endings: ENDnn;
screens: every other archive) and the total weighted by how many archives carry each content (ps1/inventory/contents.tsv), which is what a
pack saves. It also checks, for a `like` image, that its own inventory rows are the base's rows moved (it prints the count that are not).

Inputs: the disc image (--disc, raw 2352-byte sectors). It writes nothing but what --c-table names and a temporary folder (--compare-c); it
prints numbers and names, never game bytes.

    python3 port/arcade/code_pins.py --disc BIN [--c-table FILE] [--compare-c] [--resident]

--c-table writes port/arcade/code_pins_table.c (arcade_code_pins[], arcade_code_pin_count: offsets, sizes and hashes only).
--compare-c compiles code_pins.c, the checked-in code_pins_table.c and tests/code_pins_test.c into a temporary folder and, for every
image, zeroes the real chunk with arcade_code_zero and checks that the SHA-256 equals the row's data pin and that the row's pin is
build.toml's and the row's ranges the derived ones, and that the checked-in table is the text this tool writes now. Exit 0 only if all hold.
--resident prints, as a finding only (it is not in the table), how many code bytes the same rules would zero in the resident program
(SLPS_004.15: units of build.toml without an image, then the other functions of ps1/inventory/game.tsv and library.tsv).
"""
import argparse
import collections
import hashlib
import shutil
import struct
import subprocess
import sys
import tempfile
import time
import tomllib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(ROOT / 'ps1/tools'))
import fighters as F        # noqa: E402  read only: the disc reader
import pac                  # noqa: E402  read only: the archive reader

BUILD = ROOT / 'ps1/src/build.toml'
INVENTORY = ROOT / 'ps1/inventory'
JR_RA = 0x03e00008


class Fail(Exception):
    pass


# ---------------------------------------------------------------- the build and the inventory

def read_build(path=BUILD):
    """(images, funcs) from build.toml: images as the file has them (a list of dicts), funcs {image name or None: [(name, unit, address, size)]}."""
    with open(path, 'rb') as f:
        config = tomllib.load(f)
    images = config.get('image', [])
    names = [i['name'] for i in images]
    if len(set(names)) != len(names):
        raise Fail('an image name is used twice')
    funcs = collections.defaultdict(list)
    for u in config.get('unit', []):
        image = u.get('image')
        if image is not None and image not in names:
            raise Fail('unit %s is in image %s, which is not declared' % (u.get('name'), image))
        for fn in u.get('functions', []):
            funcs[image].append((fn['name'], u['name'], fn['address'], fn.get('size', 0)))
    return images, dict(funcs)


def read_tsv(path, columns):
    rows = []
    for number, line in enumerate(Path(path).read_text().splitlines(), 1):
        if not line.strip():
            continue
        fields = line.split('\t')
        if len(fields) < columns:
            raise Fail('%s:%d: expected %d columns' % (Path(path).name, number, columns))
        rows.append(fields)
    return rows


def read_inventory(directory=INVENTORY):
    """(rows, archives): rows {(slot, first archive): [(address, size)]} of modules.tsv, archives {(slot, first archive): [names]} of contents.tsv."""
    rows = collections.defaultdict(list)
    for r in read_tsv(Path(directory) / 'modules.tsv', 5):
        rows[(int(r[1], 16), r[0])].append((int(r[2], 16), int(r[3])))
    archives = {}
    for r in read_tsv(Path(directory) / 'contents.tsv', 4):
        names = r[3].split(',')
        if names[0] != r[0] or len(names) != int(r[2]):
            raise Fail('contents.tsv: the row of %s slot %s does not list its archives as it counts them' % (r[0], r[1]))
        archives[(int(r[1], 16), r[0])] = names
    return dict(rows), archives


# ---------------------------------------------------------------- the ranges

def established(image, by_name, funcs):
    """[(address, size)] of the functions the matching build establishes for the image, in the image's own address space.
    A `like` image gets its base's functions moved; a unit named in leave_out is not moved. Raises Fail on a function without a size."""
    base = by_name[image['like']] if 'like' in image else image
    if 'like' in image and 'like' in base:
        raise Fail('image %s is like %s, which is itself a second link' % (image['name'], base['name']))
    shift = image['address'] - base['address']
    left = set(image.get('leave_out', [])) if base is not image else set()
    out = []
    for name, unit, address, size in funcs.get(base['name'], []):
        if size <= 0:
            raise Fail('function %s of unit %s has no size' % (name, unit))
        if unit in left:
            continue
        out.append((address + shift, size))
    return out


def merge(ranges):
    """Sorted, non-overlapping ranges; touching and overlapping ranges are joined. ranges: [(start, size)]."""
    out = []
    for start, size in sorted(ranges):
        if size <= 0:
            continue
        if out and start <= out[-1][0] + out[-1][1]:
            end = max(out[-1][0] + out[-1][1], start + size)
            out[-1] = (out[-1][0], end - out[-1][0])
        else:
            out.append((start, size))
    return out


def total(ranges):
    return sum(s for _, s in ranges)


KEEP_HEAD = 4   # bytes at a chunk's start that are never zeroed: the game's disc stream compares the first word with the archive header's
               # (check_stream_word) and loads the file again when they differ


def keep_head(ranges):
    """The ranges without the chunk's first KEEP_HEAD bytes (the disc stream checks the first word against the archive header)."""
    out = []
    for o, n in ranges:
        if o < KEEP_HEAD:
            n -= KEEP_HEAD - o
            o = KEEP_HEAD
        if n > 0:
            out.append((o, n))
    return out


def derive_ranges(address, chunk, est, rows):
    """The ranges of a chunk linked at `address`, from established (address, size) and inventory (address, size) rows.
    dict(ranges (offsets), est_bytes, added_bytes, covered (rows already inside an established range), checked (rows added), skipped
    [(address, size, reason)]). An established range outside the chunk raises Fail."""
    size = len(chunk)
    est_rel = []
    for a, s in est:
        if a < address or a - address + s > size:
            raise Fail('an established function at %#x (%d bytes) lies outside the chunk (%#x, %d bytes)' % (a, s, address, size))
        est_rel.append((a - address, s))
    est_m = merge(est_rel)

    def inside(start, length):
        return any(s <= start and start + length <= s + n for s, n in est_m)
    extra, skipped, covered = [], [], 0
    for a, s in rows:
        o = a - address
        if o < 0 or s <= 0 or o + s > size:
            skipped.append((a, s, 'outside the chunk'))
        elif inside(o, s):
            covered += 1
        elif s < 8:
            skipped.append((a, s, 'too short'))
        elif o % 4 or s % 4:
            skipped.append((a, s, 'unaligned'))
        elif struct.unpack_from('<I', chunk, o + s - 8)[0] != JR_RA:
            skipped.append((a, s, 'not jr ra'))
        else:
            extra.append((o, s))
    ranges = keep_head(merge(est_rel + extra))
    return dict(ranges=ranges, est_bytes=total(keep_head(est_m)), added_bytes=total(ranges) - total(keep_head(est_m)), covered=covered, checked=len(extra), skipped=skipped)


def zeroed(chunk, ranges):
    b = bytearray(chunk)
    for o, s in ranges:
        b[o:o + s] = bytes(s)
    return bytes(b)


def sha(b):
    return hashlib.sha256(b).hexdigest()


# ---------------------------------------------------------------- the disc

class Disc:
    def __init__(self, path):
        self.f = open(path, 'rb')
        self.index, self.sec = F.iso_files(self.f)
        self.files = {}
        for p, (lba, size) in self.index.items():
            self.files.setdefault(p.rsplit('/', 1)[-1].upper(), []).append((lba, size))
        self.cache = {}

    def read(self, name):
        name = name.upper()
        if name not in self.cache:
            hits = self.files.get(name, [])
            if len(hits) != 1:
                raise Fail('%s: %d files of that name on the disc' % (name, len(hits)))
            lba, size = hits[0]
            self.cache[name] = self.sec(lba, (size + 2047) // 2048)[:size]
        return self.cache[name]

    def chunk(self, archive, slot):
        data = self.read(archive)
        hits = [c for c in pac.chunk_table(data) if c['table'] == 0 and c['slot'] == slot]
        if len(hits) != 1:
            raise Fail('%s: %d table-0 chunks with slot %#x' % (archive, len(hits), slot))
        c = hits[0]
        return data[c['offset']:c['offset'] + c['size']]


# ---------------------------------------------------------------- the whole derivation

def image_class(archive):
    stem = archive.upper()
    if stem.startswith('PL'):
        return 'fighters'
    if stem.startswith('STAGE'):
        return 'stages'
    if stem.startswith('END'):
        return 'endings'
    return 'screens'


def derive_all(chunks, images, funcs, rows, archives):
    """One result per image, in build.toml's order. chunks(archive file name, slot) gives the chunk's bytes."""
    by_name = {i['name']: i for i in images}
    out = []
    for image in images:
        archive = Path(image['archive']).name
        key = (image['slot'], archive)
        if key not in rows or key not in archives:
            raise Fail('image %s (%s, slot %#x) is not a content of the inventory' % (image['name'], archive, image['slot']))
        chunk = chunks(archive, image['slot'])
        if sha(chunk) != image['sha256'].lower():
            raise Fail('image %s: the chunk of %s slot %#x does not have the pinned SHA-256' % (image['name'], archive, image['slot']))
        est = established(image, by_name, funcs)
        d = derive_ranges(image['address'], chunk, est, rows[key])
        d.update(name=image['name'], archive=archive, slot=image['slot'], size=len(chunk), pin=image['sha256'].lower(),
                 data_pin=sha(zeroed(chunk, d['ranges'])), copies=len(archives[key]), cls=image_class(archive), like=image.get('like'),
                 first=est[0][0] if est else 0)
        if 'like' in image:
            base = by_name[image['like']]
            shift = image['address'] - base['address']
            base_rows = {(a + shift, s) for a, s in rows.get((base['slot'], Path(base['archive']).name), [])}
            own = set(rows[key])
            d['like_differs'] = len(base_rows ^ own)
        out.append(d)
    return out


# ---------------------------------------------------------------- the table

def c_table(results):
    lines = ['/* GENERATED by `python3 port/arcade/code_pins.py --disc DISC --c-table FILE` (FILE = port/arcade/code_pins_table.c); do not edit.',
             ' * For each of the %d module images of the build, and the resident program (SLPS_004.15, the whole file: its pin is the build\'s baseline)' % (len(results) - 1),
             ' * the chunk size, its SHA-256 (the build\'s pin), the SHA-256 of the chunk with the' ,
             ' * code ranges zeroed, and the ranges (offset and size in the chunk). Only offsets, sizes and hashes: no byte of the game. */',
             '#include "code_pins.h"', '']
    for i, r in enumerate(results):
        if r['ranges']:
            lines.append('static const struct arcade_code_range ranges_%d[] = {' % i)
            row = []
            for o, s in r['ranges']:
                row.append('{%#x, %#x}' % (o, s))
                if len(row) == 6:
                    lines.append('    ' + ', '.join(row) + ',')
                    row = []
            if row:
                lines.append('    ' + ', '.join(row) + ',')
            lines.append('};')
    lines += ['', 'const struct arcade_code_pin arcade_code_pins[] = {']

    def hexbytes(h):
        b = bytes.fromhex(h)
        return '{' + ', '.join('0x%02x' % x for x in b[:16]) + ',\n      ' + ', '.join('0x%02x' % x for x in b[16:]) + '}'
    for i, r in enumerate(results):
        lines.append('    {"%s", %#x,\n     %s,\n     %s,\n     %s, %d},' % (
            r['name'], r['size'], hexbytes(r['pin']), hexbytes(r['data_pin']), 'ranges_%d' % i if r['ranges'] else 'NULL', len(r['ranges'])))
    lines += ['};', '', 'const unsigned arcade_code_pin_count = %d;' % len(results)]
    return '\n'.join(lines) + '\n'


# ---------------------------------------------------------------- the report

def report(results):
    print('%-9s %-11s %-4s %8s %8s %8s %6s %5s %5s %5s %7s' % ('image', 'archive', 'slot', 'chunk', 'zeroed', 'estab.', 'added', 'rows+', 'cover', 'skip', 'share'))
    for r in results:
        print('%-9s %-11s %-4s %8d %8d %8d %6d %5d %5d %5d %6.1f%%' % (
            r['name'], r['archive'], '%#x' % r['slot'] if r['slot'] is not None else '-', r['size'], total(r['ranges']), r['est_bytes'], r['added_bytes'], r['checked'], r['covered'],
            len(r['skipped']), 100.0 * total(r['ranges']) / r['size']))
        if r.get('like_differs'):
            print('    like %s: %d inventory rows differ from the base\'s rows moved' % (r['like'], r['like_differs']))
    print()
    classes = collections.OrderedDict((c, [0, 0, 0, 0, 0, 0, 0, 0]) for c in ('fighters', 'screens', 'stages', 'endings', 'program'))
    for r in results:
        z = total(r['ranges'])
        c = classes[r['cls']]
        c[0] += 1
        c[1] += r['size']
        c[2] += z
        c[3] += r['est_bytes']
        c[4] += r['added_bytes']
        c[5] += len(r['skipped'])
        c[6] += r['size'] * r['copies']
        c[7] += z * r['copies']
    print('%-9s %7s %10s %10s %10s %9s %6s %12s %12s %7s' % ('class', 'images', 'chunks', 'zeroed', 'estab.', 'added', 'skip', 'on disc', 'saved', 'share'))
    for name, c in classes.items():
        print('%-9s %7d %10d %10d %10d %9d %6d %12d %12d %6.1f%%' % (name, c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], 100.0 * c[7] / c[6] if c[6] else 0))
    tot = [sum(c[i] for c in classes.values()) for i in range(8)]
    print('%-9s %7d %10d %10d %10d %9d %6d %12d %12d %6.1f%%' % ('all', tot[0], tot[1], tot[2], tot[3], tot[4], tot[5], tot[6], tot[7], 100.0 * tot[7] / tot[6]))
    print('over the images once: %d of %d bytes zeroed (%.1f%%)' % (tot[2], tot[1], 100.0 * tot[2] / tot[1]))
    print('weighted by the archives that carry each content (a pack stores every archive): %d of %d bytes zeroed (%.1f%%), %d bytes saved' % (
        tot[7], tot[6], 100.0 * tot[7] / tot[6], tot[7]))
    reasons = collections.Counter(reason for r in results for _, _, reason in r['skipped'])
    print('skipped inventory rows: %d (%s)' % (sum(reasons.values()), ', '.join('%s %d' % kv for kv in sorted(reasons.items())) or 'none'))
    likes = [r for r in results if r['like']]
    print('like images: %d; inventory rows that are not the base\'s rows moved: %d' % (len(likes), sum(r['like_differs'] for r in likes)))
    print('inventory rows already inside an established range: %d; rows added by the jr ra check: %d' % (
        sum(r['covered'] for r in results), sum(r['checked'] for r in results)))


# ---------------------------------------------------------------- the resident program

def resident_finding(disc, images, funcs):
    """(zeroed bytes, text lines): the same rules on the resident program's text."""
    hits = disc.files.get('SLPS_004.15', [])
    if len(hits) != 1:
        raise Fail('SLPS_004.15 is not on the disc once')
    data = disc.read('SLPS_004.15')
    if data[:8] != b'PS-X EXE':
        raise Fail('SLPS_004.15 is not a PS-X EXE')
    addr, size = struct.unpack_from('<II', data, 0x18)
    text = data[0x800:0x800 + size]
    if len(text) != size:
        raise Fail('the executable is shorter than its header says')
    est = [(a, s) for _, _, a, s in funcs.get(None, [])]
    inv = []
    for r in read_tsv(INVENTORY / 'game.tsv', 3):
        inv.append((int(r[0], 16), int(r[1])))
    for r in read_tsv(INVENTORY / 'library.tsv', 4):
        inv.append((int(r[0], 16), int(r[1])))
    est_in = [(a, s) for a, s in est if addr <= a and a - addr + s <= size]
    d = derive_ranges(addr, text, est_in, inv)
    lines = ['resident program (SLPS_004.15, text %#x, %d bytes): %d build.toml functions without an image (%d inside the text)' % (addr, size, len(est), len(est_in)),
             '  established %d bytes, added by the jr ra check %d bytes (%d of %d game.tsv and library.tsv rows; %d already covered), skipped %d rows' % (
                 d['est_bytes'], d['added_bytes'], d['checked'], len(inv), d['covered'], len(d['skipped'])),
             '  zeroed %d of %d bytes (%.1f%%); skipped by reason: %s' % (
                 total(d['ranges']), size, 100.0 * total(d['ranges']) / size,
                 ', '.join('%s %d' % kv for kv in sorted(collections.Counter(x[2] for x in d['skipped']).items())) or 'none')]
    return total(d['ranges']), lines


def program_result(disc, images, funcs, baseline):
    """The row of the resident program (SLPS_004.15, the whole file): its established functions (build.toml's units without an image)
    inside the text, as file offsets (0x800 + address - text address), but the function that holds the entry (pc0: the runtime scans its
    code for the jal to main) and the file's header; the pin is the build's baseline."""
    data = disc.read('SLPS_004.15')
    if data[:8] != b'PS-X EXE':
        raise Fail('SLPS_004.15 is not a PS-X EXE')
    pc0 = struct.unpack_from('<I', data, 0x10)[0]
    addr, size = struct.unpack_from('<II', data, 0x18)
    if sha(data) != baseline:
        raise Fail('SLPS_004.15 does not have the build\'s baseline SHA-256')
    est = [(a, s) for _, _, a, s in funcs.get(None, []) if addr <= a and a - addr + s <= size and not (a <= pc0 < a + s)]
    ranges = merge([(0x800 + a - addr, s) for a, s in est])
    return dict(name='SLPS_004.15', archive='SLPS_004.15', slot=None, size=len(data), pin=sha(data), data_pin=sha(zeroed(data, ranges)),
                ranges=ranges, est_bytes=total(ranges), added_bytes=0, checked=0, covered=0, skipped=[], copies=1, cls='program', like=None, first=0)


# ---------------------------------------------------------------- the C check

def build_driver(tmp, table):
    exe = str(Path(tmp) / 'code_pins_test')
    subprocess.run(['gcc', '-std=c99', '-O1', '-Wall', '-Wextra', '-Werror', '-I', str(HERE), '-o', exe,
                    str(HERE / 'code_pins.c'), str(table), str(HERE / 'tests/code_pins_test.c')], check=True, capture_output=True, text=True, timeout=120)
    return exe


def compare_c(results, chunks, text, disc_read=None):
    table = HERE / 'code_pins_table.c'
    same = table.exists() and table.read_text() == text
    print('checked-in code_pins_table.c ' + ('equals' if same else 'DIFFERS FROM') + ' the table this tool derives')
    if not same:
        return 1
    tmp = tempfile.mkdtemp(prefix='code_pins-')
    try:
        exe = build_driver(tmp, table)
        listing = subprocess.run([exe, 'list'], check=True, capture_output=True, text=True, timeout=60).stdout.split('\n')
        head = listing[0].split()
        if head != ['count', str(len(results))]:
            print('the C table has %s rows, the build %d images' % (head, len(results)))
            return 1
        c_rows = {l.split()[0]: l.split() for l in listing[1:] if l.strip()}
        status, ok = 0, 0
        src, dst = Path(tmp) / 'in', Path(tmp) / 'out'
        for r in results:
            problems = []
            row = c_rows.get(r['name'])
            if not row:
                print('%s: no row in the C table' % r['name'])
                status = 1
                continue
            _, size, pin, data_pin, count = row
            chunk = chunks(r['archive'], r['slot']) if r['slot'] is not None else disc_read(r['archive'])
            if int(size) != len(chunk) or int(count) != len(r['ranges']):
                problems.append('size or range count differs')
            if pin != r['pin']:
                problems.append('the row\'s pin is not the build\'s')
            src.write_bytes(chunk)
            run = subprocess.run([exe, 'zero', r['name'], str(src), str(dst)], capture_output=True, text=True, timeout=60)
            if run.returncode:
                problems.append('arcade_code_zero refused (%d)' % run.returncode)
            else:
                out = dst.read_bytes()
                if sha(out) != data_pin:
                    problems.append('the zeroed chunk does not have the row\'s data pin')
                if out != zeroed(chunk, r['ranges']):
                    problems.append('the zeroed chunk differs from the model')
            if data_pin != r['data_pin']:
                problems.append('the row\'s data pin is not the derived one')
            wrong = subprocess.run([exe, 'zero', r['name'], str(src), str(dst), 'short'], capture_output=True, text=True, timeout=60).returncode
            if wrong != 3:
                problems.append('a wrong size was not refused')
            if problems:
                status = 1
                print('%s: %s' % (r['name'], '; '.join(problems)))
            else:
                ok += 1
        print('%d of %d images: the C zeroes the real chunk to the row\'s data pin, the row\'s pin is the build\'s' % (ok, len(results)))
        return status
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help='disc image, raw 2352-byte sectors')
    ap.add_argument('--c-table', metavar='FILE', help='write port/arcade/code_pins_table.c to FILE')
    ap.add_argument('--compare-c', action='store_true', help='build the checked-in table in C and check it against the real chunks')
    ap.add_argument('--resident', action='store_true', help='print what the same rules would zero in the resident program (a finding)')
    args = ap.parse_args(argv)
    started = time.monotonic()
    try:
        disc = Disc(args.disc)
        images, funcs = read_build()
        rows, archives = read_inventory()
        results = derive_all(disc.chunk, images, funcs, rows, archives)
        with open(BUILD, 'rb') as fb:
            baseline = tomllib.load(fb)['baseline']['sha256'].lower()
        results.append(program_result(disc, images, funcs, baseline))
    except Fail as e:
        print('FAILED: %s' % e)
        return 1
    report(results)
    text = c_table(results)
    status = 0
    if args.c_table:
        Path(args.c_table).write_text(text)
        print('wrote %s' % args.c_table)
    if args.resident:
        try:
            for line in resident_finding(disc, images, funcs)[1]:
                print(line)
        except Fail as e:
            print('FAILED: %s' % e)
            status = 1
    if args.compare_c:
        try:
            status = compare_c(results, disc.chunk, text, disc.read) or status
        except (Fail, subprocess.CalledProcessError) as e:
            print('FAILED: %s' % (e.stderr if isinstance(e, subprocess.CalledProcessError) else e))
            status = 1
    print('done in %.1f s' % (time.monotonic() - started))
    return status


if __name__ == '__main__':
    sys.exit(main())
