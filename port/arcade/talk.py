#!/usr/bin/env python3
"""Analysis tool: the talk before a fight in the arcade's English. Derives the table that talk.c reads, from the disc and the arcade set, and
checks the C by drawing what it builds.

What it does. The PS1 shows a talk with the shared boss_demo module (table 0 slot 0x2a, the same 21,196 bytes in the 63 files RDMnn.PAC,
BOSSnn.PAC and GDEMOnn.PAC, nn from 00 to 14). A file's second chunk of table 2, slot 0 (32,768 bytes) is the text page: 256 x 256 pixels of
4 bits, low nibble first, sent to video memory at (768,0). The module draws box b of a talk as two 128 x 48 sprites of the page at v = 48 * b
(build_selector_sprite_pairs_slot2a), so a box is 3 lines of 16 rows, a line 16 Japanese cells of 16 pixels. The module's script (a table of
44 u32 pointers at module offset 0x1bdc, indexed by game_state.stage_index) is a list of 6-byte records: w0 = box (low byte), speaker side
(high byte), bit 15 on the last record; w1 = the box's timer (0xf0 frames) with bit 15 the side; w2 = the box's line count, which centres the
box vertically ((3 - n) * 8 - 16). stage_index 0 to 0x14 picks BOSSnn's script nn, 0x15 to 0x29 RDMnn's script 0x15 + nn (every one of the
42 pages' row counts per box equals its script's w2s: checked here), 0x2a and 0x2b the two GDEMO scripts (boxes 0, 1, 2 and boxes 3, 1, 2).
nn is the player's kind; the opponent is in the page. Files 12, 13 and 14 carry the pages of 04, 11 and 02 (byte-equal: checked here).

The arcade (US) keeps the talks in English in the 16-bit text form of texts.h from 0xee9c8: lines of 32 words, boxes of 3 lines (192
bytes), box g at 0xee9c8 + 192 * g, g = 1 to 144 for the 36 talks in use (box 0 is blank), then two test talks (never used) and the Akuma talk
at g = 153 to 155 (3 boxes). A byte a box of line counts follows at 0xf5ec8. The talks are not all 4 boxes: the arcade's T23 (BOSS02's) has 3
and T35 (BOSS0D's) 5, the others 4 (the starts were found one by one against the Japanese pages' first boxes; the table's `text` is the start).
talk.c draws a talk with the arcade's 8 x 16 font (quotes.h) into a page, 8 pixels a column, lines at rows 48 * b + 16 * l, and writes the
English line count of each box into the w2 of the file's script, so that the box is centred as the Japanese was.

Rule A (the mapping from files to talks) is an inference, made by reading the Japanese pages and the English together; it is not in any table
of either game. The arcade's talks come in pairs a fighter, T(2p) for RDM[kind] and T(2p + 1) for BOSS[kind], the fighters in this order of
PS1 kind (ARCADE_ORDER): 00 01 04 0b 05 08 07 06 03 09 0a 02 0c 0f 10 11 0e 0d. The English has not the Japanese's speaker turns box for
box, so a talk's start is where the Japanese page's first box is (a speaker's first line); T0 to T22 start 4 boxes after each other, T23 has 3
boxes, then every talk has 4 up to T34, and T35 has 5. Evidence for each pair (gists, not the games' lines):
  p0  Ryu     T0 RDM00: Sakura catches Ryu and asks for a fight. T1 BOSS00: Ryu meets Akuma ("Gouken" in the Japanese).
  p1  Ken     T2 RDM01: Dan calls Ken "Blondie" and offers a coin to buy his own moves. T3 BOSS01: Ryu and Ken, the lesson Ken gave Sagat.
  p2  Chun-Li T4 RDM04: Gen asks a girl what she seeks (Japanese: where she learned that move). T5 BOSS04: Chun-Li to Bison about her father.
  p3  Sagat   T6 RDM0B: Adon asks how Sagat lost to Ryu. T7 BOSS0B: Sagat's revenge on Ryu with the new Tiger move.
  p4  Adon    T8 RDM05: Bison invites Adon to Shadaloo. T9 BOSS05: Adon hunts the Sagat who ran away.
  p5  Birdie  T10 RDM08: Dhalsim and the yoga insult from Birdie. T11 BOSS08: Birdie asks the Shadaloo boss for an offer.
  p6  Guy     T12 RDM07: Rose senses doubt in Guy. T13 BOSS07: Guy and Bison, Bushin style against soldiers.
  p7  Sodom   T14 RDM06: Sodom and the word "Shogun". T15 BOSS06: the gang that Guy broke up.
  p8  Nash    T16 RDM03: a trooper and his orders (Rolento's coup in the Japanese). T17 BOSS03: Nash arrests Bison, the helicopter.
  p9  Rose    T18 RDM09: the power Rose felt. T19 BOSS09: Rose warns Bison, counts to three.
  p10 Bison   T20 RDM0A: Nash, the commander of Shadaloo. T21 BOSS0A: Bison to Ryu, "lord and master".
  p11 Gouki   T22 RDM02: the avenged master. T23 BOSS02: Akuma asked what he wants (3 boxes: the Japanese has a silent box). FLAG: Gouki's
              pair is by elimination (the last fighter left); the content is compatible but there is no anchor line like the others.
  p12 Dan     T24 RDM0C: Rolento's remark on Dan's moves, then Dan's reply and Rolento's journey (the first box is the line that the
              Japanese has as Rolento's "interesting technique"). T25 BOSS0C: Dan's remark on Sagat's eye, Sagat's reply, Dan's father.
  p13 Dhalsim T26 RDM0F: Zangief asks how Dhalsim trained (yoga). T27 BOSS0F: Dhalsim and the evil soul.
  p14 Zangief T28 RDM10: Birdie's hairstyle against Zangief's Russia. T29 BOSS10: Zangief introduces himself.
  p15 Gen     T30 RDM11: Chun-Li asks Gen about Shadaloo. T31 BOSS11: Gen sends Akuma to hell.
  p16 Rolento T32 RDM0E: Sodom to Rolento, Mad Gear. T33 BOSS0E: Rolento remembers his supreme leader (Nash).
  p17 Sakura  T34 RDM0D: Sagat, who looks for Ryu, meets Sakura. T35 BOSS0D: Sakura finds Ryu, "little girl" (5 boxes: the 4 first are
              drawn, the last, Sakura's closing exclamation, is not: FLAG).
GDEMO (all 21 files, one page): T38 (3 boxes), in the order of box_of 0, 1, 2, 0.
Boxes whose line count differs between the Japanese and the English (the reason for rewriting w2): see the findings line.

Flagged calls: (0) a talk with fewer boxes than the page ends early: the first script record that shows a page box with no arcade box
(box_of >= boxes) gets bit 15 of its w0, which advance_pass_or_call_end_by_high_bit_slot2a reads (only that bit) to end the talk; BOSS02's
4th record is that one. A talk with more boxes than the page loses the last (BOSS0D). (1) GDEMO's second script shows box 3 first, and the arcade's talk has no 4th box: page box 3 draws the arcade's first box.
(2) A GDEMO file has two scripts, so two table rows (the same page, two script offsets); a row a file for the others. (3) The line count of
a box is the number of lines up to the last one that has a character (a blank line before text counts), at least 1.

Inputs: the disc image (--disc, raw 2352-byte sectors) and the arcade set's ZIP (--arcade; MAME's sfa2u). It writes nothing but what --c-table
and --previews name and a temporary folder (--compare-c); it prints numbers, never game bytes (--text prints the first line of a few talks).

    python3 port/arcade/talk.py --disc BIN --arcade ZIP [--c-table FILE] [--compare-c] [--previews DIR] [--text]

--compare-c compiles talk.c, the checked-in talk_table.c and tests/talk_test.c into a temporary folder and, for every row on the real disc and
arcade set: builds the page and checks it equals the model's drawing pixel by pixel (the model draws each character of the arcade's lines at
its place with the font rule), that every 8 x 16 cell of the page matches the glyph of the arcade's character, and that rows below the boxes are
0; builds the script in the file's module and checks that the only bytes changed are the w2 of the script's records, that each equals the
English line count of the box the record shows, and, where a record shows a box the arcade's talk lacks, that this one record's w0 high byte
has bit 15 and the records after it are untouched, and that the mask marks exactly those bytes. Ends with status 0 only when all of it holds.
--previews DIR writes a PNG a file: the page's 4 boxes with the module's palette (the 16 colours that init_palette_and_blocks_for_stage_slot2a
uploads to video memory (0x2c0,0x80), read from its source under ps1/src).
"""
import argparse
import hashlib
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import time
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(ROOT / 'port/intro'))
sys.path.insert(0, str(ROOT / 'ps1/tools'))
import fighters as F        # noqa: E402  read only: the disc reader, the arcade views
import pac                  # noqa: E402  read only: the chunk table
import quotes as Q          # noqa: E402  read only: the font rule, the text words, the PNG writer

MODULE_SLOT = 0x2a
PAGE_TABLE, PAGE_SLOT, PAGE_ORDINAL = 2, 0, 1
SCRIPT_TABLE = 0x1bdc       # data_801e1bdc_slot2a: a u32 pointer a stage_index
SCRIPT_ENTRIES = 44
MODULE_BASE = 0x801e0000
BOX0 = 0xee9c8              # the arcade's box 0 (blank); box g is at BOX0 + 192 * g, g = 1 .. 155
TALKS = 36                  # the arcade's talks T0 .. T35 in use (T36 and T37 are test text), in boxes from g = 1
BOXES_OF_TALK = {23: 3, 35: 5}   # every other talk has 4 boxes; T23 (BOSS02) has 3, T35 (BOSS0D) has 5, the last of which is not drawn
AKUMA_BOX = 153             # the Akuma talk (GDEMO): boxes 153 .. 155
FAMILIES = ('BOSS', 'RDM')  # stage_index 0 and 0x15 families; a talk pair is T(2p) RDM, T(2p + 1) BOSS
ARCADE_ORDER = [0x00, 0x01, 0x04, 0x0b, 0x05, 0x08, 0x07, 0x06, 0x03, 0x09, 0x0a, 0x02, 0x0c, 0x0f, 0x10, 0x11, 0x0e, 0x0d]
ALIAS = {0x12: 0x04, 0x13: 0x11, 0x14: 0x02}      # files whose page is byte-equal to another kind's
GDEMO_SCRIPTS = (42, 43)
GDEMO_BOX_OF = (0, 1, 2, 0)
COLUMNS, LINES, BOXES = 32, 3, 4
PAGE_BYTES = 32768
PAL_SOURCE = ROOT / 'ps1/src/overlays/boss_demo_module/init_palette_and_blocks_for_stage_slot2a.c'
Fail = Q.Fail


# ---------------------------------------------------------------- the arcade's talks and the model of talk.c

def checked_box(prog, text, a):
    out = []
    for l in range(LINES):
        s = ''
        for k in range(COLUMNS):
            at = text + a * LINES * COLUMNS * 2 + l * COLUMNS * 2 + 2 * k
            if at + 2 > len(prog):
                raise Fail('the talk at %#x lies outside the program' % text)
            c = Q.word_char(prog, at)
            if c is None:
                raise Fail('the word at %#x is outside the font' % at)
            s += chr(c)
        out.append(s)
    return out


def arcade_box(row, b):
    a = row['box_of'][b]
    return min(a, row['boxes'] - 1)


def box_line_count(lines):
    last = 0
    for l, s in enumerate(lines):
        if s.strip(' '):
            last = l + 1
    return max(last, 1)


def model_page(row, prog, glyphs):
    """The page (32,768 bytes) as the contract draws it, written independently of talk.c: pixel by pixel into a 256 x 256 array."""
    px = [[0] * 256 for _ in range(256)]
    for b in range(BOXES):
        lines = checked_box(prog, row['text'], arcade_box(row, b))
        for l, s in enumerate(lines):
            for k, ch in enumerate(s):
                if ch == ' ':
                    continue
                g = glyphs(ord(ch))
                for y in range(16):
                    for x in range(8):
                        px[48 * b + 16 * l + y][8 * k + x] = g[y][x]
    out = bytearray(PAGE_BYTES)
    for y in range(256):
        for x in range(0, 256, 2):
            out[y * 128 + x // 2] = px[y][x] | px[y][x + 1] << 4
    return bytes(out)


def page_pixels(page):
    return [[(page[y * 128 + x // 2] >> (4 if x & 1 else 0)) & 15 for x in range(256)] for y in range(256)]


def read_back(page, glyphs):
    """[[candidate sets]] a (box, line) of the page: every 8 x 16 cell matched to the font's glyphs (a set of characters, or None)."""
    by_pixels = {}
    for c in range(0x20, 0x7f):
        by_pixels.setdefault(bytes(v for r in glyphs(c) for v in r), set()).add(chr(c))
    px = page_pixels(page)
    out = []
    for b in range(BOXES):
        for l in range(LINES):
            row = []
            for k in range(COLUMNS):
                key = bytes(px[48 * b + 16 * l + y][8 * k + x] for y in range(16) for x in range(8))
                row.append(by_pixels.get(key))
            out.append(row)
    return out


def model_script(row, prog, module):
    """(new module, mask) as the contract writes them: the w2 of every record but the last, up to the first record that shows a page box
    whose arcade box does not exist (box_of >= boxes), which gets bit 15 of its w0 instead (the talk ends there; the rest is left alone)."""
    out, mask = bytearray(module), bytearray(len(module))
    counts = [box_line_count(checked_box(prog, row['text'], arcade_box(row, b))) for b in range(BOXES)]
    for i in range(row['records'] - 1):
        at = row['script'] + 6 * i
        if row['box_of'][module[at]] >= row['boxes']:
            out[at + 1] |= 0x80
            mask[at + 1] = 1
            break
        out[at + 4:at + 6] = struct.pack('<H', counts[module[at]])
        mask[at + 4:at + 6] = b'\1\1'
    return bytes(out), bytes(mask)


# ---------------------------------------------------------------- the disc

def u32(m, a):
    return struct.unpack_from('<I', m, a)[0]


def parse_script(module, off):
    """[(box, side, timer_and_side, w2)] of the records of the script at module offset off, and the record count (the end record included)."""
    recs, o = [], off
    while True:
        if o + 6 > len(module) or len(recs) > 8:
            raise Fail('the script at %#x does not end' % off)
        w0, w1, w2 = struct.unpack_from('<3H', module, o)
        o += 6
        if w0 & 0x8000:
            return recs, len(recs) + 1
        if (w0 & 0xff) >= BOXES:
            raise Fail('the script at %#x names box %d' % (off, w0 & 0xff))
        recs.append((w0 & 0xff, w0 >> 8, w1, w2))


class Disc:
    def __init__(self, path):
        self.files = {}      # name -> (module, page)
        with open(path, 'rb') as f:
            index, sec = F.iso_files(f)
            for fam in ('RDM', 'BOSS', 'GDEMO'):
                for k in range(0x15):
                    name = '%s%02X.PAC' % (fam, k)
                    hits = [p for p in index if p.upper().endswith('/' + name)]
                    if len(hits) != 1:
                        raise Fail('%s is not on the disc' % name)
                    lba, size = index[hits[0]]
                    data = sec(lba, (size + 2047) // 2048)[:size]
                    chunks = pac.chunk_table(data)
                    mods = [c for c in chunks if (c['table'], c['slot']) == (0, MODULE_SLOT)]
                    pages = [c for c in chunks if (c['table'], c['slot']) == (PAGE_TABLE, PAGE_SLOT)]
                    if len(mods) != 1 or len(pages) <= PAGE_ORDINAL or pages[PAGE_ORDINAL]['size'] != PAGE_BYTES:
                        raise Fail('%s lacks its module or its text page' % name)
                    c = pages[PAGE_ORDINAL]
                    self.files[name] = (data[mods[0]['offset']:mods[0]['offset'] + mods[0]['size']], data[c['offset']:c['offset'] + c['size']])


def japanese_rows(page, b):
    return sum(1 for r in range(3) if any(page[(3 * b + r) * 16 * 128:(3 * b + r + 1) * 16 * 128]))


def derive(disc, prog):
    """dict(rows, notes): rows in the table's order (RDM, BOSS, GDEMO; files 00 to 14; GDEMO twice)."""
    if len({hashlib.sha256(m).hexdigest() for m, _p in disc.files.values()}) != 1:
        raise Fail('the 63 modules are not the same')
    module = disc.files['RDM00.PAC'][0]
    pointers = [u32(module, SCRIPT_TABLE + 4 * i) - MODULE_BASE for i in range(SCRIPT_ENTRIES)]
    scripts = [parse_script(module, o) for o in pointers]
    notes = dict(scripts=len(set(pointers)), page_boxes_checked=0, jp_en_differ=0, jp_en_total=0, aliases=0)
    rows = []

    starts, g = [], 1
    for t in range(TALKS):
        starts.append(g)
        g += BOXES_OF_TALK.get(t, 4)
    if g != 145:
        raise Fail('the talks do not end at box 144 (%d)' % g)

    def talk_for(fam, kind):
        p = ARCADE_ORDER.index(ALIAS.get(kind, kind))
        t = 2 * p + (0 if fam == 'RDM' else 1)
        return t, BOX0 + 192 * starts[t], min(BOXES_OF_TALK.get(t, 4), 4)
    for fam in ('RDM', 'BOSS'):
        for k in range(0x15):
            name = '%s%02X.PAC' % (fam, k)
            si = k if fam == 'BOSS' else 0x15 + k
            recs, count = scripts[si]
            page = disc.files[name][1]
            if k in ALIAS and page != disc.files['%s%02X.PAC' % (fam, ALIAS[k])][1]:
                raise Fail('%s is not byte-equal to the page of its kind %02x' % (name, ALIAS[k]))
            notes['aliases'] += k in ALIAS
            for r in recs:                  # the Japanese rows of the box a record shows equal the record's line count
                if japanese_rows(page, r[0]) != r[3]:
                    raise Fail('%s: box %d has %d rows, its script says %d' % (name, r[0], japanese_rows(page, r[0]), r[3]))
            notes['page_boxes_checked'] += len(recs)
            t, text, boxes = talk_for(fam, k)
            row = dict(file=name, text=text, boxes=boxes, box_of=(0, 1, 2, 3), script=pointers[si], records=count)
            for b in range(BOXES):
                n_en = box_line_count(checked_box(prog, text, min(b, boxes - 1)))
                notes['jp_en_total'] += 1
                notes['jp_en_differ'] += n_en != japanese_rows(page, b)
            rows.append(row)
    for k in range(0x15):
        name = 'GDEMO%02X.PAC' % k
        page = disc.files[name][1]
        if page != disc.files['GDEMO00.PAC'][1]:
            raise Fail('%s is not byte-equal to GDEMO00' % name)
        for si in GDEMO_SCRIPTS:
            recs, count = scripts[si]
            for r in recs:
                if japanese_rows(page, r[0]) != r[3]:
                    raise Fail('%s: box %d has %d rows, its script says %d' % (name, r[0], japanese_rows(page, r[0]), r[3]))
            rows.append(dict(file=name, text=BOX0 + 192 * AKUMA_BOX, boxes=3, box_of=GDEMO_BOX_OF, script=pointers[si], records=count))
    if rows[-1]['text'] + 3 * 192 > len(prog):
        raise Fail('the talks lie outside the program')
    return dict(rows=rows, notes=notes)


def c_table(d):
    lines = ['/* GENERATED by `python3 port/arcade/talk.py --disc DISC --arcade SET --c-table FILE` (FILE = port/arcade/talk_table.c); do not edit.',
             ' * For each of the 63 talk files (RDMnn, BOSSnn, GDEMOnn: a GDEMO file has two rows, one a script): the arcade address of its',
             ' * talk, the boxes the arcade has, the arcade box each page box draws, and the module offset and record count of its script.',
             ' * Only offsets, addresses and counts: no byte of either game. */',
             '#include "talk.h"', '', 'static const struct arcade_talk_page pages[] = {']
    for r in d['rows']:
        lines.append('    {"%s", %#x, %d, {%d, %d, %d, %d}, %#x, %d},' % ((r['file'], r['text'], r['boxes']) + tuple(r['box_of']) + (r['script'], r['records'])))
    lines += ['};', '', 'const struct arcade_talk arcade_talk = {%d, %d, %d, %#x, pages, sizeof pages / sizeof pages[0]};' % (
        PAGE_TABLE, PAGE_SLOT, PAGE_ORDINAL, MODULE_SLOT)]
    return '\n'.join(lines) + '\n'


# ---------------------------------------------------------------- previews

def module_palette():
    """The 16 colours of init_palette_and_blocks_for_stage_slot2a, read from its source: [(r, g, b)], colour 0 shown as dark blue."""
    text = PAL_SOURCE.read_text()
    m = re.search(r'u16 pal\[16\] = \{([^}]*)\}', text)
    if not m:
        raise Fail('the palette is not in %s' % PAL_SOURCE.name)
    ws = [int(x, 16) for x in re.findall(r'0x[0-9a-fA-F]+', m.group(1))]
    if len(ws) != 16:
        raise Fail('the palette has %d colours' % len(ws))
    return [(20, 24, 60) if i == 0 else (((w & 31) * 255) // 31, (((w >> 5) & 31) * 255) // 31, (((w >> 10) & 31) * 255) // 31) for i, w in enumerate(ws)]


def preview(path, page, pal, scale=3):
    px = page_pixels(page)[:48 * BOXES]
    out = bytearray()
    for row in px:
        line = b''.join(bytes(pal[v]) * scale for v in row)
        out += line * scale
    Q.png(path, 256 * scale, 48 * BOXES * scale, bytes(out))


# ---------------------------------------------------------------- the C

def build_driver(tmp):
    exe = str(Path(tmp) / 'talk_test')
    subprocess.run(['gcc', '-std=c99', '-O1', '-Wall', '-Wextra', '-Werror', '-o', exe, str(HERE / 'talk.c'), str(HERE / 'talk_table.c'),
                    str(HERE / 'tests/talk_test.c')], check=True, timeout=180)
    return exe


def parse_table_dump(text):
    lines = text.strip().split('\n')
    pt, ps, po, ms, count = map(int, lines[0].split())
    rows = []
    for ln in lines[1:1 + count]:
        f = ln.split()
        v = [int(x, 0) for x in f[1:]]
        rows.append(dict(file=f[0], text=v[0], boxes=v[1], box_of=tuple(v[2:6]), script=v[6], records=v[7]))
    return (pt, ps, po, ms), rows


def row_args(r):
    return [r['file'], r['text'], r['boxes']] + list(r['box_of']) + [r['script'], r['records']]


def compare_c(d, disc, prog, gfx, text, tmp, previews):
    same = (HERE / 'talk_table.c').exists() and (HERE / 'talk_table.c').read_text() == text
    print('checked-in talk_table.c ' + ('equals' if same else 'DIFFERS FROM') + ' the table this tool derives')
    if not same:
        return 1
    exe = build_driver(tmp)
    head, c_rows = parse_table_dump(subprocess.run([exe, 'table'], check=True, capture_output=True, text=True, timeout=60).stdout)
    if head != (PAGE_TABLE, PAGE_SLOT, PAGE_ORDINAL, MODULE_SLOT) or c_rows != d['rows']:
        print('the C table differs from the derived one')
        return 1
    prog_f, gfx_f, mod_f, out = Path(tmp) / 'program', Path(tmp) / 'gfx', Path(tmp) / 'module', Path(tmp) / 'out'
    prog_f.write_bytes(prog)
    gfx_f.write_bytes(gfx)
    out.mkdir()
    cache = {}

    def glyphs(c):
        if c not in cache:
            cache[c] = Q.glyph(gfx, c)
        return cache[c]
    pal = module_palette() if previews else None
    status, ok_pages, ok_scripts, built, previewed, early = 0, 0, 0, {}, 0, 0
    for r in c_rows:
        name = r['file']
        module, disc_page = disc.files[name]
        problems = []
        run = subprocess.run([exe, 'page'] + [str(x) for x in row_args(r)] + [str(prog_f), str(gfx_f), str(out)], capture_output=True, text=True, timeout=120)
        if run.returncode:
            print('%s: the C refused the page: %s %s' % (name, run.stdout.strip(), run.stderr.strip()))
            status = 1
            continue
        page = (out / 'page').read_bytes()
        key = (r['text'], r['boxes'], r['box_of'])
        if key not in built:
            if page != model_page(r, prog, glyphs):
                problems.append('the page differs from the model')
            back = read_back(page, glyphs)
            for b in range(BOXES):
                lines = checked_box(prog, r['text'], arcade_box(r, b))
                for l, s in enumerate(lines):
                    got = back[b * LINES + l]
                    if any(c is None or s[k] not in c for k, c in enumerate(got)):
                        problems.append('box %d line %d: a cell is not the arcade character' % (b, l))
            if any(page[48 * BOXES * 128:]):
                problems.append('rows below the boxes are not 0')
            built[key] = page
        elif built[key] != page:
            problems.append('the same talk built two pages')
        if previews and not problems and (previews, name) not in built:
            Path(previews).mkdir(parents=True, exist_ok=True)
            preview(Path(previews) / name.replace('.PAC', '.png'), page, pal)
            built[(previews, name)] = 1
            previewed += 1
        ok_pages += not problems
        # the script
        mod_f.write_bytes(module)
        run = subprocess.run([exe, 'script'] + [str(x) for x in row_args(r)] + [str(prog_f), str(mod_f), str(out)], capture_output=True, text=True, timeout=120)
        sp = []
        if run.returncode:
            sp.append('the C refused the script: %s %s' % (run.stdout.strip(), run.stderr.strip()))
        else:
            new, changed = (out / 'module').read_bytes(), (out / 'changed').read_bytes()
            want, mask = model_script(r, prog, module)
            if new != want:
                sp.append('the module differs from the model')
            if changed != mask:
                sp.append('the mask is not the w2 bytes')
            diff = [i for i in range(len(module)) if module[i] != new[i]]
            if any(not mask[i] for i in diff):
                sp.append('a byte outside the w2s changed')
            ends = [i for i in range(r['records'] - 1) if r['box_of'][module[r['script'] + 6 * i]] >= r['boxes']]
            want_bytes = 2 * (ends[0] if ends else r['records'] - 1) + (1 if ends else 0)
            if sum(mask) != want_bytes:
                sp.append('the mask holds %d bytes, not %d' % (sum(mask), want_bytes))
            early += bool(ends)
            if ends and not new[r['script'] + 6 * ends[0] + 1] & 0x80:
                sp.append('the early end has no bit 15')
        ok_scripts += not sp
        if problems or sp:
            status = 1
            print('%s: PROBLEMS' % name)
            for p in (problems + sp)[:5]:
                print('    ' + p)
    print('%d of %d pages equal the model and the arcade text, %d of %d scripts only changed their w2s to the English line counts and, where a record shows a box the arcade lacks, ended the talk there (%d scripts end early; %d previews)' % (
        ok_pages, len(c_rows), ok_scripts, len(c_rows), early, previewed))
    return status


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help='disc image, raw 2352-byte sectors')
    ap.add_argument('--arcade', required=True, help="arcade set ZIP (MAME's sfa2u)")
    ap.add_argument('--c-table', metavar='FILE', help='write port/arcade/talk_table.c to FILE')
    ap.add_argument('--compare-c', action='store_true', help='build the checked-in table in C and check what it builds')
    ap.add_argument('--previews', metavar='DIR', help='write a PNG a file (private) into DIR; with --compare-c')
    ap.add_argument('--text', action='store_true', help='print the first line of a few talks (game text)')
    args = ap.parse_args(argv)
    started = time.monotonic()
    try:
        disc = Disc(args.disc)
        prog, gfx = F.arcade_views(args.arcade)
        d = derive(disc, prog)
    except Fail as e:
        print('FAILED: %s' % e)
        return 1
    n = d['notes']
    print('%d rows (%d files, GDEMO twice); %d distinct scripts of %d; %d script records checked against the Japanese pages\' rows (all equal); '
          '%d alias files byte-equal to their kind\'s page' % (len(d['rows']), len(disc.files), n['scripts'], SCRIPT_ENTRIES, n['page_boxes_checked'], n['aliases']))
    print('  boxes whose English line count differs from the Japanese rows: %d of %d' % (n['jp_en_differ'], n['jp_en_total']))
    for r in d['rows'][:3] + d['rows'][21:23] + d['rows'][42:44] + d['rows'][-2:]:
        print('  %s: talk %#x, %d boxes, box_of %s, script %#x, %d records' % (r['file'], r['text'], r['boxes'], r['box_of'], r['script'], r['records']))
    if args.text:
        for g in (1, 5, 93, 96, 140, 153):
            print('  box %d: %s' % (g, ' / '.join(s.strip() for s in checked_box(prog, BOX0 + 192 * g, 0) if s.strip())))
    text = c_table(d)
    status = 0
    if args.c_table:
        Path(args.c_table).write_text(text)
        print('wrote %s' % args.c_table)
    if args.compare_c:
        tmp = tempfile.mkdtemp(prefix='talk-')
        try:
            status = compare_c(d, disc, prog, gfx, text, tmp, args.previews)
        except Fail as e:
            print('FAILED: %s' % e)
            status = 1
        finally:
            shutil.rmtree(tmp, ignore_errors=True)
    print('done in %.1f s' % (time.monotonic() - started))
    return status


if __name__ == '__main__':
    sys.exit(main())
