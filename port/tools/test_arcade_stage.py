#!/usr/bin/env python3
"""Seconds: the runtime C of the stage pieces (port/arcade/stage.c) against the Python model; no game files.

Builds stage.c, the generated descriptor table and a driver with the address and undefined-behavior sanitizers, makes a small
arcade (graphics with a transparent tile and flips, scroll 1 sets, 16 and 32 pixel blocks, palette rows, id lists) and made-up
stage descriptors, and checks that the C gives, byte for byte and known byte for known byte, the module's layer maps, the pages
(layer tiles in first-use order, extras, holes), the layer palettes, the object tiles and palettes that stages.py's model_stage
gives, for each flag combination and for random descriptors (with the exceptions of the packing, the known cells and tiles, the
layer starts given by the packing rule, and the runs of palettes that are arcade rows or filler). It also checks that malformed
input is refused (a block that does not exist, an address outside the program, a map that does not fit the module, tiles that do
not fit the pages, runs that do not add up, a run of palettes that leaves the chunk), that the generated table is sane, that the C
compiles warning-free for 32 bits, and the helpers that write the table.
"""
from pathlib import Path
import collections, random, shutil, struct, subprocess, sys, tempfile, unittest
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import stages as S

FLAGS = ('-std=c99', '-O1', '-Wall', '-Wextra', '-Werror', '-pedantic')
SOURCES = [ROOT / 'port/arcade' / n for n in ('stage.c', 'stages_table.c')]
PIECES = (('module', 'module'), ('pages', 'pages'), ('palettes', 'palettes'), ('object tiles', 'objects'), ('object palettes', 'objpal'))
GFX_BYTES = (0x10000 + 128) * 128 + 4096          # 16 pixel scroll tiles from 0x10000; 32 pixel tiles (512 bytes) from 0x4000 are the same bytes


class World:
    """A small made-up arcade: graphics (tile 0x10000 transparent), three scroll 1 sets, three 16 and three 32 pixel blocks,
    palette rows of stage 3 and the id lists."""

    def __init__(self, seed):
        self.rnd = rnd = random.Random(seed)
        self.gfx = bytearray(rnd.randbytes(GFX_BYTES))
        self.gfx[0x10000 * 128:0x10000 * 128 + 128] = b'\xff' * 128                 # transparent: arcade colour 15 everywhere
        self.gfx[0x4000 * 512:0x4000 * 512 + 512] = b'\xff' * 512                   # a transparent 32 pixel tile (the same bytes as 16 pixel tiles 0x10000 to 0x10003)
        for k in (0x10004, 0x10005):                                                # flat but not transparent
            self.gfx[k * 128:k * 128 + 128] = b'\x00' * 128
        self.prog = prog = bytearray(0x300000)
        for g in range(4):                                                          # cell groups: pieces of tiles 0x10000 + 0..5
            for c in range(16):
                code = rnd.choice([0, 1, 0, 1, 2 * rnd.randrange(6) + rnd.randrange(2)])
                struct.pack_into('>HH', prog, S.GROUPS1 + 64 * g + 4 * c, code, rnd.choice([0, 0x20, 0x40, 0x60]) | rnd.randrange(4))
        for s in range(3):
            struct.pack_into('>64H', prog, S.SETS1 + 128 * s, *[0 if s == 0 else rnd.randrange(4) for _ in range(64)])
        for b in range(3):
            for k in range(256):
                idx = 0x10000 + (0 if (b == 0 and k % 3 == 0) else rnd.randrange(12))
                struct.pack_into('>HH', prog, S.TABLE16 + b * 1024 + 4 * k, (idx - 0x10000 - 0xa400) & 0xffff, rnd.choice([0, 0x20, 0x40, 0x60]) | rnd.randrange(32))
            for k in range(64):
                idx = 0x4000 + (0 if (b == 0 and k % 4 == 0) else rnd.randrange(6))
                struct.pack_into('>HH', prog, S.TABLE32 + b * 256 + 4 * k, (idx - 0x4000 - 0x900) & 0x3fff, rnd.choice([0, 0x20, 0x40, 0x60]) | rnd.randrange(32))
        prog[0x1d020:0x1d024] = bytes([0, 1, 2, 1])
        prog[0x1d120:0x1d124] = bytes([1, 0, 2, 0])
        prog[0x1d304:0x1d308] = bytes([2, 0, 1, 2])
        for layer in range(3):
            for row in range(32):
                struct.pack_into('>16H', prog, S.PALETTE_TABLES[layer] + 0x400 * 3 + 0x20 * row, *[rnd.randrange(0x1000) if rnd.random() < 0.9 else 0 for _ in range(16)])
        for k in range(7):
            struct.pack_into('>16H', prog, S.PALETTE_OBJECTS + 0xe0 * 3 + 0x20 * k, *[rnd.randrange(0x1000) if rnd.random() < 0.9 else 0 for _ in range(16)])
        self.arc = S.Arc(bytes(prog), bytes(self.gfx))

    def descriptor(self, flags=(3, 3, 3), pal_exact=0, extras=(), holes=(), objects=None, count=96, module=0x3000, pages=3 * 32768,
                   exc=((), (), ()), cell_known=(0, 0, 0), tile_known=(0, 0, 0), more=(), start_rule=(False, False), xmaps=((), (), ()), own_cells=((), (), ()), own_tiles=((), (), ()), l0own=None, region=None):
        d = {'file': 'TEST.PAC', 'stage': 3, 'extras': list(extras), 'holes': list(holes), 'pal_exact': pal_exact, 'more': list(more),
             'object_runs': [(S.RUN_ZERO, 2, 0), (0x10003, 4, 0), (0x10009, 2, 3), (S.RUN_DISC, 1, 0)] if objects is None else objects,
             'layers': [{'map': 0x100, 'across': 64, 'down': 16, 'ids': [0x1d020, 0x1d021, 0x1d022, 0x1d023], 'start': 0, 'flags': flags[0]},
                        {'map': 0x1200, 'across': 64, 'down': 16, 'ids': [0x1d120, 0x1d121, 0x1d122, 0x1d123], 'start': 0, 'flags': flags[1]},
                        {'map': 0x2400, 'across': 32, 'down': 8, 'ids': [0x1d304 + k for k in range(4)], 'start': 0, 'flags': flags[2]}]}
        for n, layer in enumerate(d['layers']):
            layer.update({'exc': list(exc[n]), 'cell_known': cell_known[n], 'tile_known': tile_known[n]})
            if xmaps[n]:
                layer['xmaps'] = [dict(m) for m in xmaps[n]]
            if own_cells[n]:
                layer['own_cells'] = list(own_cells[n])
            if own_tiles[n]:
                layer['own_tiles'] = list(own_tiles[n])
        d['sizes'] = {'module': module, 'pages': pages, 'palettes': count * 32,
                      'object tiles': 128 * sum(c for _f, c, _fl in d['object_runs']), 'object palettes': 224}
        if l0own is not None:                  # layer 0 by the own rule: layer 1 starts where the region ends
            d['layers'][0]['flags'] = S.L0_OWN
            d['layers'][0]['l0own'] = {'skip': list(l0own.get('skip', ())), 'reserved': list(l0own.get('reserved', ()))}
            d['layers'][1]['start'] = region
        got = S.model_stage_raw(self.arc, d)
        if isinstance(got, str):               # a layer 0 the rule refuses: the caller wants the descriptor to see both sides refuse it
            d['layers'][2]['start'] = 0
            return d
        maps, tiles = got
        l1 = region if l0own is not None else S.l1_start_rule(len(tiles[0]), d['extras'], d['holes'])
        d['layers'][1]['start'] = l1
        maps, tiles = S.model_stage_raw(self.arc, d)
        d['layers'][2]['start'] = S.l2_start_rule(l1 + len(tiles[1]), d['extras'], d['holes'])
        for n in (1, 2):
            if start_rule[n - 1]:
                d['layers'][n]['flags'] |= S.START_RULE
        return d


def plane_bytes(values):
    """128 bytes of the arcade layout (planes, 8 pixels to a byte) of 256 arcade colour values (15: transparent)."""
    out = bytearray(128)
    for y in range(16):
        for x in range(16):
            for pl in range(4):
                out[y * 8 + x // 8 * 4 + pl] |= ((values[y * 16 + x] >> pl) & 1) << (7 - x % 8)
    return bytes(out)


FAST = [(200, 824)]                       # the own layer 0 tests build 200 of the layer's 1,024 cells (the rest is skipped), to stay quick


class L0World(World):
    """World whose scroll 1 pieces use few colours of rows that are random subsets of a pool, so that the own layer 0 rule meets every case:
    cells in one row, cells that fit another row, cells that need a merged row, cells with too many colours, and rows that run out."""

    def __init__(self, seed, rows_used=32, spread=8):
        World.__init__(self, seed)
        rnd = self.rnd
        gfx = bytearray(self.gfx)
        for k in range(12):
            if k in (0, 4, 5):
                continue
            top = rnd.choice([3, 3, spread])
            vals = [rnd.choice([15, 15, 15] + list(range(top))) for _ in range(256)]
            gfx[(0x10000 + k) * 128:(0x10000 + k + 1) * 128] = plane_bytes(vals)
        prog = bytearray(self.prog)
        for g in range(4):
            for c in range(16):
                code, attr = struct.unpack_from('>HH', prog, S.GROUPS1 + 64 * g + 4 * c)
                struct.pack_into('>HH', prog, S.GROUPS1 + 64 * g + 4 * c, code, (attr & 0x60) | rnd.randrange(rows_used))
        pool = [rnd.randrange(1, 0x1000) for _ in range(30)]
        for row in range(32):
            struct.pack_into('>16H', prog, S.PALETTE_TABLES[0] + 0x400 * 3 + 0x20 * row, *rnd.sample(pool, 15), 0)
        self.prog, self.gfx = prog, gfx
        self.arc = S.Arc(bytes(prog), bytes(gfx))


def spec_text(d):
    lines = [f'stage {d["stage"]}', f'size {d["sizes"]["module"]} {d["sizes"]["pages"]} {d["sizes"]["palettes"]} {d["sizes"]["object tiles"]}',
             f'pal {d["pal_exact"] & (2 ** 64 - 1)} {d["pal_exact"] >> 64}']
    for n, l in enumerate(d['layers']):
        start = 0 if l['flags'] & S.START_RULE else l['start']            # the C must not read it when the rule gives the start
        lines.append(f'layer {n} {l["map"]} {l["across"]} {l["down"]} {start} {l["flags"]} {len(l["ids"])} ' + ' '.join(map(str, l['ids'])))
        cells = S.layer_cell_total(l, n)
        lines += [f'xmap {n} {m["map"]} {len(m["ids"])} ' + ' '.join(map(str, m['ids'])) for m in l.get('xmaps', ())]
        lines += [f'exc {n} {c} {"NEW" if sl == S.FORCE_NEW else sl}' for c, sl in l.get('exc', ())]
        if l.get('cell_known'):
            lines.append(f'cknown {n} ' + ' '.join(map(str, S.mask_words(l['cell_known'], cells))))
        if l.get('tile_known'):
            lines.append(f'tknown {n} ' + ' '.join(map(str, S.mask_words(l['tile_known'], cells + 1))))
    for n, l in enumerate(d['layers']):
        lines += [f'ocell {n} {a} {c}' for a, c in S.index_spans(l.get('own_cells', []))]
        lines += [f'otile {n} {a} {c}' for a, c in S.index_spans(l.get('own_tiles', []))]
    lo = d['layers'][0].get('l0own')
    if lo:
        lines += [f'l0skip {a} {c}' for a, c in lo['skip']] + [f'l0res {a} {c}' for a, c in lo['reserved']]
    lines += [f'extra {a} {c} {f} {fl}' for a, c, f, fl in d['extras']]
    lines += [f'hole {a} {c}' for a, c in d['holes']]
    lines += [f'more {k} {c} {st} {kind} {arg}' for k, st, c, kind, arg in d.get('more', ())]
    lines += [f'object {"ZERO" if f == S.RUN_ZERO else "DISC" if f == S.RUN_DISC else "BLANK" if f == S.RUN_BLANK else f} {c} {fl}' for f, c, fl in d['object_runs']]
    return '\n'.join(lines) + '\n'


class StageC(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.dir = tempfile.TemporaryDirectory(prefix='arcade-stage-')
        d = Path(cls.dir.name)
        cls.exe = d / 'stage_test'
        subprocess.run(['gcc', *FLAGS, '-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', *map(str, SOURCES),
                        str(ROOT / 'port/arcade/tests/stage_test.c'), '-o', str(cls.exe)], check=True, timeout=120)
        cls.world = World(11)
        (d / 'prog.bin').write_bytes(cls.world.arc.prog)
        (d / 'tiles.bin').write_bytes(cls.world.arc.gfx)
        cls.n = 0

    @classmethod
    def tearDownClass(cls):
        cls.dir.cleanup()

    def run_c(self, world, d, expect_error=False):
        """Build with the C; the pieces {name: (data, known)}, or the refusal message."""
        base = Path(self.dir.name)
        out = base / f'out{self.n}'
        StageC.n += 1
        out.mkdir()
        (base / 'spec.txt').write_text(spec_text(d))
        if world is not self.world:
            (base / 'prog2.bin').write_bytes(world.arc.prog)
            (base / 'tiles2.bin').write_bytes(world.arc.gfx)
            prog, tiles = base / 'prog2.bin', base / 'tiles2.bin'
        else:
            prog, tiles = base / 'prog.bin', base / 'tiles.bin'
        r = subprocess.run([str(self.exe), 'build', str(prog), str(tiles), str(base / 'spec.txt'), str(out)], capture_output=True, text=True, timeout=120)
        if r.returncode == 1 and r.stdout.startswith('error: '):
            return r.stdout[7:].strip()
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        got = {label: ((out / f'{name}.bin').read_bytes(), (out / f'{name}.known').read_bytes()) for label, name in PIECES}
        names = dict(PIECES)
        got['owned'] = {label for label, name in PIECES if name in (out / 'owned.txt').read_text().split()}
        got['own'] = {label: (out / f'{name}.own').read_bytes() for label, name in PIECES if label in ('palettes', 'module', 'pages')}
        return got

    def same(self, world, d):
        built = self.run_c(world, d)
        model = S.model_stage(world.arc, d)
        self.assertIsInstance(built, dict, built)
        for label, _name in PIECES:
            self.assertEqual(built[label][0], bytes(model[label][0]), f'{label} data')
            self.assertEqual(built[label][1], bytes(model[label][1]), f'{label} known')
        self.assertEqual(built['owned'], model['owned'])
        self.assertEqual(built['own'], {k: bytes(v) for k, v in model['own'].items()})
        return built, model

    def test_all_pieces_equal_the_model(self):
        d = self.world.descriptor(extras=[(400, 3, 0x10005, 1), (410, 1, 0x10007, 2)], holes=[(420, 2)], pal_exact=(1 << 5) | (1 << 64 + 3) | 1)
        built, model = self.same(self.world, d)
        self.assertGreater(sum(built['module'][1]), 0)
        self.assertEqual(sum(built['palettes'][1]), 3 * 32)             # palettes 0, 5 and 67 (layer 2 row 3)

    def test_known_masks_follow_the_flags(self):
        for flags in ((0, 0, 0), (1, 0, 0), (0, 2, 0), (3, 3, 3), (1, 2, 3)):
            d = self.world.descriptor(flags=flags, holes=[(430, 1)])
            built, _model = self.same(self.world, d)
            known_module = sum(built['module'][1])
            want = sum(4 * l['across'] * l['down'] for l, f in zip(d['layers'], flags) if f & 1)
            self.assertEqual(known_module, want, flags)
            pages_known = built['pages'][1]
            self.assertLess(sum(pages_known), len(pages_known))          # the hole at least is unknown
            if flags == (3, 3, 3):
                self.assertEqual(len(pages_known) - sum(pages_known), 128)        # exactly the hole's slot is unknown

    def test_exceptions_of_the_packing_and_the_known_bits(self):
        w = self.world
        base = w.descriptor()
        cells0 = S.layer_cells(w.arc, 0, S.ids_of(w.arc, base['layers'][0]), 64, 16, 3)
        # a cell that shares an earlier slot, one that takes a new slot though its tile exists, one that skips slots, one that is blank
        first_new = [i for i, c in enumerate(cells0) if any(c[2])][:6]
        exc = [(first_new[2], 1), (first_new[3], S.FORCE_NEW), (first_new[4] + 1, 0), (first_new[5] + 3, 30)]
        d = w.descriptor(flags=(0, 0, 0), exc=(exc, [(10, 2), (11, S.FORCE_NEW), (400, 0)], [(5, 3)]),
                         cell_known=(0b1011 | 1 << 1000, 1 << 1023 | 0xff, 3), tile_known=(0b111 | 1 << 20, 1 << 5, 0b11))
        built, model = self.same(w, d)
        data, known = built['module']
        self.assertEqual(sum(known) // 4, 4 + 9 + 2)                        # the cells named by the known bits, and no other
        pages_known = built['pages'][1]
        self.assertLess(sum(pages_known), len(pages_known))
        # with the flags set the bits are not needed
        d2 = w.descriptor(flags=(3, 3, 3), exc=(exc, [], []))
        self.same(w, d2)

    def test_the_packing_rule_gives_the_layer_starts(self):
        w = self.world
        d = w.descriptor(extras=[(300, 4, 0x10005, 0)], holes=[(304, 12)], start_rule=(True, True))
        built, model = self.same(w, d)
        d_explicit = w.descriptor(extras=[(300, 4, 0x10005, 0)], holes=[(304, 12)])
        d_explicit['layers'][1]['start'] = S.layer_start(d, 1, [len(S.model_stage_raw(w.arc, d_explicit)[1][0]), 0, 0, 0])
        # the rule's starts are the ones the helper put in the descriptor
        maps, tiles = S.model_stage_raw(w.arc, d)
        for key in ('module', 'pages'):
            self.assertEqual(bytes(model[key][0]), bytes(S.model_stage(w.arc, d_explicit)[key][0]))
        # the C does not read the start field of a layer whose flags say the rule gives it
        d['layers'][1]['start'] = 7777
        d['layers'][2]['start'] = 9999
        self.assertEqual(bytes(S.model_stage(w.arc, d)['pages'][0]), bytes(model['pages'][0]))
        built2 = self.run_c(w, d)
        self.assertEqual(built2['pages'][0], built['pages'][0])

    def test_the_other_palettes_are_arcade_rows_or_filler(self):
        w = self.world
        more = [(65, 2, 3, S.MORE_ROW, 0x275c40), (71, 2, 2, S.MORE_FILL, 0), (128, 1, 4, S.MORE_ROW, S.PALETTE_TABLES[1] + 0x400 * 3)]
        d = w.descriptor(count=160, pal_exact=(1 << 3) | (1 << 70), more=more)
        built, model = self.same(w, d)
        data, known = built['palettes']
        self.assertEqual(sum(1 for k in range(160) if known[32 * k]), 2 + 3 + 2 + 4)
        want = [0] + [S.arcade_color_to_ps1(x) or 0x8000 for x in struct.unpack_from('>15H', w.arc.prog, 0x275c40 + 0x20)]
        self.assertEqual(list(struct.unpack_from('<16H', data, 67 * 32)), want)
        fill = [0] + [S.arcade_color_to_ps1(S.FILL_COLOUR)] * 15
        self.assertEqual(list(struct.unpack_from('<16H', data, 73 * 32)), fill)
        for bad in ([(65, 2, 80, S.MORE_FILL, 0)], [(150, 1, 20, S.MORE_FILL, 0)], [(65, 2, 1, S.MORE_ROW, 0x2fffe1)], [(65, 2, 1, S.MORE_ROW, 0x400000)]):
            d = w.descriptor(count=160, more=bad)
            with self.assertRaises(ValueError):
                S.model_stage(w.arc, d)
            self.assertIsInstance(self.run_c(w, d), str, bad)

    def test_layers_pack_tiles_in_first_use_order_with_the_blank_first(self):
        d = self.world.descriptor()
        built, _model = self.same(self.world, d)
        module, pages = built['module'][0], built['pages'][0]
        blank0 = S.slot_code(d['layers'][0]['start'], 16)
        cell = struct.unpack_from('<HH', module, 0x100)
        # layer 0's first cell is set 0 (all ids 0 groups): cell (0, 0) takes the first different tile, slot 1, unless it is blank
        self.assertIn(cell[0], (blank0, S.slot_code(1, 16)))
        # the blank tile of each layer is all zero in the pages
        for n in range(3):
            side = (16, 16, 32)[n]
            self.assertEqual(S.get_slot(pages, d['layers'][n]['start'], side), bytes(side * side // 2))
        # layer 1's and 2's cell words use palette word 0x7800 + layer + 64 * row
        w1 = struct.unpack_from('<HH', module, 0x1200)
        w2 = struct.unpack_from('<HH', module, 0x2400)
        self.assertEqual(w1[1] & 63, 1)
        self.assertEqual(w2[1] & 63, 2)

    def test_object_tiles_runs_zero_disc_and_flips(self):
        d = self.world.descriptor(objects=[(S.RUN_ZERO, 1, 0), (0x10002, 2, 1), (S.RUN_DISC, 2, 0), (0x10004, 1, 2), (0x10004, 1, 3)])
        built, model = self.same(self.world, d)
        data, known = built['object tiles']
        self.assertEqual(known, b'\1' * 128 * 3 + b'\0' * 256 + b'\1' * 256)
        self.assertEqual(data[:128], bytes(128))
        self.assertEqual(data[128:256], self.world.arc.tile(0x10002, 16, True, False))
        self.assertEqual(data[640:768], self.world.arc.tile(0x10004, 16, False, True))
        self.assertEqual(data[768:896], self.world.arc.tile(0x10004, 16, True, True))

    def test_palettes_by_design_are_own_not_known(self):
        w = self.world
        more = [(65, 2, 1, S.MORE_ROW, 0x275c40), (67, 2, 2, S.MORE_APPROX, 0x275c60), (71, 1, 1, S.MORE_BLANK, 0), (73, 2, 2, S.MORE_FILL, 0)]
        d = w.descriptor(count=160, pal_exact=(1 << 3), more=more)
        built, model = self.same(w, d)
        data, known = built['palettes']
        own = built['own']['palettes']
        for k, kn, ow in ((65, 1, 0), (67, 0, 1), (69, 0, 1), (71, 0, 1), (73, 1, 0), (75, 1, 0), (3, 1, 0), (66, 0, 0)):
            self.assertEqual((known[32 * k], own[32 * k]), (kn, ow), k)
            self.assertEqual(known[32 * k:32 * k + 32], bytes([kn]) * 32)
            self.assertEqual(own[32 * k:32 * k + 32], bytes([ow]) * 32)
        want = [0] + [S.arcade_color_to_ps1(x) or 0x8000 for x in struct.unpack_from('>15H', w.arc.prog, 0x275c60 + 0x20)]
        self.assertEqual(list(struct.unpack_from('<16H', data, 69 * 32)), want)        # the approximating row is the arcade's, entry 0 transparent
        self.assertEqual(data[71 * 32:72 * 32], bytes(32))
        self.assertFalse(any(k and o for k, o in zip(known, own)))
        d = w.descriptor(count=160, more=[(65, 1, 1, 4, 0)])
        with self.assertRaises(ValueError):
            S.model_stage(w.arc, d)
        self.assertIsInstance(self.run_c(w, d), str)

    def test_cells_and_tiles_by_design_are_own_not_known(self):
        w = self.world
        d = w.descriptor(flags=(3, 3, 3), own_cells=((), [3, 4, 5, 100, 700], [7]), own_tiles=((), [2, 3], [1]))
        built, model = self.same(w, d)
        data, known = built['module']
        own = built['own']['module']
        # layer 1's cells 3, 4, 5, 100 and 700 and layer 2's cell 7: four bytes each in the module, own and not known, the others known
        for n, i in ((1, 3), (1, 4), (1, 5), (1, 100), (1, 700), (2, 7)):
            cx, cy = [(c[0], c[1]) for c in S.layer_cells(w.arc, n, S.ids_of(w.arc, d['layers'][n]), d['layers'][n]['across'], d['layers'][n]['down'], 3)][i]
            at = d['layers'][n]['map'] + 2 * S.map_word_index(cx, cy, d['layers'][n]['across'], (16, 16, 32)[n])
            self.assertEqual((known[at:at + 4], own[at:at + 4]), (bytes(4), b'\1' * 4), (n, i))
        self.assertEqual(sum(own) // 4, 6)
        self.assertEqual(sum(known) // 4, 1024 + 1024 + 256 - 6)
        pages_known, pages_own = built['pages'][1], built['own']['pages']
        self.assertEqual(sum(pages_own), 128 * 2 + 512 * 1)                         # two 16 pixel tiles and one 32 pixel tile
        self.assertFalse(any(k and o for k, o in zip(pages_known, pages_own)))
        # a tile that is not one of the layer's, and one that does not fit the pages
        for n, tiles in ((1, [9999]), (2, [5000])):
            bad = w.descriptor(own_tiles=tuple(tiles if m == n else () for m in range(3)))
            with self.assertRaises(ValueError):
                S.model_stage(w.arc, bad)
            self.assertIsInstance(self.run_c(w, bad), str)
        # the table sanity of the real table and a build with neither is unchanged
        plain, _m = self.same(w, w.descriptor(flags=(3, 3, 3)))
        self.assertEqual(sum(plain['own']['module']), 0)

    def test_object_tiles_blank_by_design_are_owned_only_without_disc_runs(self):
        w = self.world
        runs = [(S.RUN_ZERO, 1, 0), (0x10002, 2, 1), (S.RUN_BLANK, 3, 0), (0x10004, 1, 2)]
        built, model = self.same(w, w.descriptor(objects=runs))
        data, known = built['object tiles']
        self.assertEqual(built['owned'], {'object tiles'})
        self.assertEqual(known, b'\1' * 128 * 3 + b'\0' * 128 * 3 + b'\1' * 128)       # the blank ones are not known: they differ from the disc's by design
        self.assertEqual(data[384:768], bytes(384))
        # a disc run beside them: the chunk is not built whole, so nothing is owned
        built2, _m = self.same(w, w.descriptor(objects=runs + [(S.RUN_DISC, 1, 0)]))
        self.assertEqual(built2['owned'], set())
        # blank runs only, and no blank run: the same rule
        self.assertEqual(self.same(w, w.descriptor(objects=[(S.RUN_BLANK, 2, 0)]))[0]['owned'], {'object tiles'})
        self.assertEqual(self.same(w, w.descriptor(objects=[(S.RUN_ZERO, 2, 0)]))[0]['owned'], set())

    def test_palettes_and_object_palettes(self):
        d = self.world.descriptor(count=128, pal_exact=(1 << 0) | (1 << 1) | (1 << 64) | (1 << 66) | (1 << 101))
        built, _model = self.same(self.world, d)
        data, known = built['palettes']
        # palette 0 is layer 0 row 0, palette 1 layer 1 row 0, palette 64 layer 2 row 0, palette 65 is not a layer row
        want0 = S.predicted_palette(self.world.arc.prog, 3, 0, 0)
        self.assertEqual(list(struct.unpack_from('<16H', data, 0)), want0)
        self.assertEqual(list(struct.unpack_from('<16H', data, 64 * 32)), S.predicted_palette(self.world.arc.prog, 3, 2, 0))
        self.assertEqual(known[0:32], b'\1' * 32)
        self.assertEqual(known[2 * 32:3 * 32], bytes(32))
        self.assertEqual(data[65 * 32:66 * 32], bytes(32))
        self.assertEqual(known[65 * 32:66 * 32], bytes(32))
        self.assertEqual(known[101 * 32:102 * 32], bytes(32))            # palette 101 is no layer row (odd, from 64 on, in a chunk of 128): never known
        self.assertEqual(sum(1 for k in range(0, 128 * 32, 32) if known[k]), 4)
        obj, objknown = built['object palettes']
        self.assertEqual(objknown, b'\1' * 224)
        self.assertEqual(struct.unpack_from('<16H', obj, 0)[0], 0)
        self.assertTrue(all(c == 0 or c != 0x8000 for c in struct.unpack('<112H', obj)))        # black stays 0 in object palettes
        layer_colours = [c for k in range(0, 64 * 32, 32) for c in struct.unpack_from('<16H', data, k)[1:]]
        self.assertFalse(any(c == 0 for c in layer_colours))               # in layer palettes black is the opaque 0x8000
        self.assertTrue(any(c == 0x8000 for c in layer_colours))

    def test_random_descriptors(self):
        rnd = random.Random(5)
        for seed in range(16):
            world = World(100 + seed)
            flags = tuple(rnd.randrange(4) for _ in range(3))
            extras = [(300 + 10 * k, rnd.randrange(1, 4), 0x10000 + rnd.randrange(100), rnd.randrange(4)) for k in range(rnd.randrange(3))]
            holes = [(500 + 10 * k, rnd.randrange(1, 3)) for k in range(rnd.randrange(3))]
            objects = [(rnd.choice([S.RUN_ZERO, S.RUN_DISC, 0x10000 + rnd.randrange(100)]), rnd.randrange(1, 4), rnd.randrange(4)) for _ in range(rnd.randrange(1, 6))]
            cells = (1024, 1024, 256)
            exc = tuple(sorted((c, rnd.choice([S.FORCE_NEW, 0, 1, 2, 5, 20, 33])) for c in rnd.sample(range(cells[n]), rnd.randrange(0, 12))) for n in range(3))
            count = rnd.choice([96, 127, 128, 160])
            more = [(rnd.randrange(64, min(count, 100)), rnd.choice([1, 2]), rnd.randrange(1, 4), S.MORE_FILL, 0) for _ in range(rnd.randrange(3))]
            more += [(rnd.randrange(64, min(count, 100)), rnd.choice([1, 2]), rnd.randrange(1, 4), S.MORE_ROW, 0x275c40 + 0x20 * rnd.randrange(60)) for _ in range(rnd.randrange(3))]
            xm = ((), (), ())
            if rnd.random() < 0.5:
                xm = ((), ({'map': 0x3000, 'ids': [rnd.randrange(3) for _ in range(rnd.choice([2, 4]))]},), ({'map': 0x4100, 'ids': [rnd.randrange(3) for _ in range(rnd.choice([2, 4]))]},))
            total = (cells[0], cells[1] + 256 * len(xm[1][0]['ids']) if xm[1] else cells[1], cells[2] + 64 * len(xm[2][0]['ids']) if xm[2] else cells[2])
            d = world.descriptor(flags=flags, extras=extras, holes=holes, objects=objects, pal_exact=rnd.getrandbits(96), count=count, exc=exc,
                                 cell_known=tuple(rnd.getrandbits(total[n]) for n in range(3)), tile_known=tuple(rnd.getrandbits(total[n] + 1) for n in range(3)),
                                 more=[m for m in more if m[0] + m[1] * (m[2] - 1) < count], start_rule=(rnd.random() < 0.5, rnd.random() < 0.5),
                                 xmaps=xm, module=0x6000)
            try:
                S.model_stage(world.arc, d)
            except ValueError:
                self.assertIsInstance(self.run_c(world, d), str)          # a descriptor the model refuses is refused by the C too
                continue
            self.same(world, d)

    def test_layer_zero_own_rule_is_built_by_c_as_the_model_says(self):
        seen = collections.Counter()
        for seed, rows_used, spread in ((1, 32, 12), (2, 6, 4), (3, 32, 3), (4, 12, 12), (5, 3, 12), (6, 32, 12)):
            w = L0World(seed, rows_used, spread)
            wide = w.descriptor(l0own={'skip': FAST, 'reserved': []}, region=900, pages=6 * 32768)
            res = S.l0_own_build(w.arc, wide)
            seen.update(res['stats'])
            need = len(res['tiles'])
            for region, reserved, skip in ((300, [], FAST), (need + 3, [(5, 2)], FAST), (max(need - 3, 4), [(3, 1)], FAST), (300, [], [(100, 70), (300, 724)])):
                d = w.descriptor(l0own={'skip': skip, 'reserved': reserved}, region=region, module=0x6000, pages=6 * 32768)
                try:
                    S.model_stage(w.arc, d)
                except ValueError:
                    self.assertIsInstance(self.run_c(w, d), str)
                    continue
                built, model = self.same(w, d)
                # everything of layer 0 is owned (nothing known by the flag alone): cells, the region's slots, the 32 palette rows
                own_slots = sum(1 for slot in range(region) if built['own']['pages'][S.slot_place(slot, 16) + 4])
                self.assertEqual(own_slots, region - sum(1 for a, c in reserved for x in range(a, a + c) if x < region))
                self.assertEqual(sum(1 for r in range(32) if built['own']['palettes'][64 * r + 4]), 32)
                r2 = S.l0_own_build(w.arc, d)
                seen.update(r2['stats'])
        for tier in ('blank', 'one row', 're-indexed', 'merged', 'nearest', 'folded', 'merged rows'):
            self.assertGreater(seen[tier], 0, tier)

    def test_the_first_word_of_a_chunk_is_never_owned(self):
        # the game's disc stream compares each chunk's first word with the archive header's (the disc's) and reads again on a mismatch:
        # the first four bytes of the module, the pages and the palettes are never owned, so they stay the disc's
        w = L0World(2, 6, 4)
        d = w.descriptor(l0own={'skip': FAST, 'reserved': []}, region=300, module=0x6000, pages=6 * 32768,
                         own_cells=((), [0], ()), own_tiles=((), [0], ()))
        d['layers'][1]['own_cells'] = [0]
        built, model = self.same(w, d)
        own = built['own']
        self.assertEqual(own['palettes'][:4], bytes(4))                     # row 0 of layer 0 is owned, but for its first two entries
        self.assertEqual(own['palettes'][4:32], b'\1' * 28)
        self.assertEqual(own['pages'][:4], bytes(4))                        # slot 0's blank tile is owned, but for its first word
        self.assertEqual(own['pages'][4:8], b'\1' * 4)
        self.assertEqual(own['module'][:4], bytes(4))
        self.assertEqual(model['own']['palettes'][:4], bytes(4))
        # a palette row 0 by design in the ordinary descriptor too
        more = [(0, 2, 1, S.MORE_APPROX, 0x275c40)]
        d2 = self.world.descriptor(count=96, more=[(2, 1, 1, S.MORE_APPROX, 0x275c40)])
        b2, _m = self.same(self.world, d2)
        self.assertEqual(b2['own']['palettes'][:4], bytes(4))
        # and the second palette chunk: the same
        prog = self.world.arc.prog
        got = self.run_pal(prog, 4096, [(0, 1, 2, S.MORE_APPROX, 0x275c40), (1, 1, 1, S.MORE_BLANK, 0)])
        self.assertEqual(got[2][:4], bytes(4))
        self.assertEqual(got[2][4:32], b'\1' * 28)
        own = bytearray(4096)
        S.model_shared_palettes(prog, 4096, [(0, 1, 2, S.MORE_APPROX, 0x275c40), (1, 1, 1, S.MORE_BLANK, 0)], own)
        self.assertEqual(bytes(own), got[2])

    def test_layer_zero_own_known_bits_serve_what_equals_the_disc(self):
        w = L0World(2, 6, 4)
        d = w.descriptor(l0own={'skip': FAST, 'reserved': []}, region=300, module=0x6000, pages=6 * 32768)
        res = S.l0_own_build(w.arc, d)
        keys = sorted(res['words'], key=lambda k: res['index'][k])
        ck = sum(1 << res['index'][k] for k in keys[:50])                          # cells 0.. named equal
        tk = sum(1 << slot for slot in sorted(res['tiles'])[:5]) | 1 << res['zero_slots'][0]
        d['layers'][0]['cell_known'], d['layers'][0]['tile_known'] = ck, tk
        d['pal_exact'] = (1 << 4) | (1 << 10)                                      # layer 0 rows 2 and 5
        built, _model = self.same(w, d)
        module_known, module_own = built['module'][1], built['own']['module']
        known_cells = sum(1 for k in keys if module_known[S.cell_offset(d['layers'][0], 0, *k)])
        self.assertEqual(known_cells, 50)
        self.assertEqual(sum(1 for k in keys if module_own[S.cell_offset(d['layers'][0], 0, *k)]), len(keys) - 50)
        pk, po = built['pages'][1], built['own']['pages']
        self.assertEqual(sum(1 for slot in range(300) if pk[S.slot_place(slot, 16)]), min(5, len(res['tiles'])) + 1)
        self.assertFalse(any(a and b for a, b in zip(pk, po)))
        self.assertEqual([r for r in range(32) if built['palettes'][1][64 * r]], [2, 5])
        self.assertEqual([r for r in range(32) if built['own']['palettes'][64 * r + 4]], [r for r in range(32) if r not in (2, 5)])

    def test_layer_zero_own_extra_map_is_a_list_of_sets_and_refusals(self):
        w = L0World(3, 20, 4)
        d = w.descriptor(l0own={'skip': [(100, 924)], 'reserved': []}, region=300, module=0x8000, pages=6 * 32768, xmaps=([{'map': 0x4000, 'ids': [0, 2, 1, 0]}], (), ()))
        built, _model = self.same(w, d)
        self.assertEqual(sum(built['own']['module']) // 4, 100 + 1024)                  # the layer's own cells (100 not skipped) and the extra map's
        # the extra map's cells are those of sets 0, 2, 1, 0 in pages of two blocks
        res = S.l0_own_build(w.arc, d)
        self.assertEqual(len(res['words']), 100 + 1024)
        # refusals both sides: no room (slot 0 reserved, no slot at all), a set that does not exist, a map outside the module
        for what, bad in (('slot 0 reserved', w.descriptor(l0own={'skip': [], 'reserved': [(0, 1)]}, region=300, module=0x6000, pages=6 * 32768)),
                          ('module too small', w.descriptor(l0own={'skip': [], 'reserved': []}, region=300, module=0x1800, pages=6 * 32768))):
            if what == 'slot 0 reserved':
                bad['layers'][0]['l0own']['reserved'] = [(0, 1)]
            with self.subTest(what):
                with self.assertRaises(ValueError):
                    S.model_stage(w.arc, bad)
                self.assertIsInstance(self.run_c(w, bad), str)

    def test_extra_maps_continue_the_layer_scan(self):
        w = self.world
        xm = ((), ({'map': 0x3000, 'ids': [2, 0, 1, 2]},), ({'map': 0x4100, 'ids': [1, 2, 0, 1]},))
        d = w.descriptor(xmaps=xm, module=0x6000)
        built, model = self.same(w, d)
        module = built['module'][0]
        # with the flags set every cell known: the layers' own and the extra maps' (4 + 4 blocks of 256 and 64 cells... )
        self.assertEqual(sum(built['module'][1]), 4 * (1024 + 1024 + 256 + 4 * 256 + 4 * 64))
        # layer 2's own blocks are 2, 0, 1, 2 across; extra block 1 (page 0, left) is the same arcade block as the own block 1 (third), so the
        # cells carry the same words: the extra maps share the layer's tiles
        for x in range(8):
            for y in range(8):
                own = 0x2400 + 2 * S.map_word_index(16 + x, y, 32, 32)
                extra = 0x4100 + 2 * S.map_word_index(x, y, 16, 32)
                self.assertEqual(module[own:own + 4], module[extra:extra + 4], (x, y))
        # layer 1's extra block 2 (page 0, left) is the arcade block 2: layer 1's own ids are 1, 0, 2, 0: block 2 is the third
        for x in range(16):
            for y in range(16):
                own = 0x1200 + 2 * S.map_word_index(32 + x, y, 64, 16)
                extra = 0x3000 + 2 * S.map_word_index(x, y, 32, 16)
                self.assertEqual(module[own:own + 4], module[extra:extra + 4], (x, y))
        # the same descriptor without the extra maps builds the same pages (all their tiles are among the layer's own)
        plain, _m = self.same(w, w.descriptor(module=0x6000))
        self.assertEqual(built['pages'][0], plain['pages'][0])

    def test_extras_of_quarters_of_32_pixel_tiles(self):
        w = self.world
        f = lambda q, fl=0: S.SCROLL3_FLIP | q << S.QUARTER_SHIFT | fl
        extras = [(400, 3, 5, f(2)), (410, 1, 6, f(1, 1)), (411, 1, 6, f(3, 2)), (412, 2, 8, f(0, 3)), (420, 1, 0x4000 + 3, f(1)), (430, 1, 0x10000 + 3, 1)]
        built, model = self.same(w, w.descriptor(extras=extras))
        pages = built['pages'][0]
        self.assertEqual(S.get_slot(pages, 400, 16), w.arc.quarter(5, 2, False, False))
        self.assertEqual(S.get_slot(pages, 402, 16), w.arc.quarter(7, 2, False, False))
        self.assertEqual(S.get_slot(pages, 410, 16), w.arc.quarter(6, 1, True, False))
        self.assertEqual(S.get_slot(pages, 413, 16), w.arc.quarter(9, 0, True, True))
        self.assertEqual(S.get_slot(pages, 430, 16), w.arc.tile(0x10000 + 3, 16, True, False))
        for name, bad in (('quarter bits without the scroll 3 flag', 16), ('an undefined bit', 64), ('a 32 pixel tile outside the graphics', None)):
            run = (400, 1, len(w.gfx) // 512 + 3, f(1)) if bad is None else (400, 1, 5, bad)
            d = w.descriptor(extras=[run])
            with self.assertRaises(ValueError, msg=name):
                S.model_stage(w.arc, d)
            self.assertIsInstance(self.run_c(w, d), str, name)

    def test_the_known_bits_run_through_the_extra_maps(self):
        w = self.world
        xm = ((), ({'map': 0x3000, 'ids': [2, 0]},), ({'map': 0x4100, 'ids': [1, 2]},))
        d = w.descriptor(flags=(0, 0, 0), xmaps=xm, module=0x6000, cell_known=(0, 1 << 1024 | 1 << 1030, 1 << 256 | 1 << 259 | 1 << 300),
                         tile_known=(0, 1 << 3, 1 << 2))
        built, _model = self.same(w, d)
        self.assertEqual(sum(built['module'][1]) // 4, 2 + 3)
        d = w.descriptor(flags=(3, 3, 3), xmaps=xm, module=0x6000, start_rule=(True, True))
        self.same(w, d)

    def test_extra_maps_refused_by_both(self):
        w = self.world
        good = ((), ({'map': 0x3000, 'ids': [2, 0]},), ())
        cases = {'a block that does not exist': (((), ({'map': 0x3000, 'ids': [2, 200]},), ()), 0x6000),
                 'a map that does not fit the module': (((), ({'map': 0x3000, 'ids': [2, 0]},), ()), 0x3400),
                 'a map on layer 0': ((({'map': 0x3000, 'ids': [0, 1]},), (), ()), 0x6000)}
        for name, (xm, module) in cases.items():
            with self.subTest(name):
                d = w.descriptor(xmaps=good, module=0x6000)
                for n in range(3):
                    if xm[n]:
                        d['layers'][n]['xmaps'] = [dict(m) for m in xm[n]]
                d['sizes']['module'] = module
                with self.assertRaises(ValueError):
                    S.model_stage(w.arc, d)
                self.assertTrue(self.run_c(w, d), name)
                self.assertIsInstance(self.run_c(w, d), str)
        self.same(w, w.descriptor(xmaps=good, module=0x6000))

    def run_pal(self, prog, total, more):
        base = Path(self.dir.name)
        out = base / f'pal{self.n}'
        StageC.n += 1
        out.mkdir()
        (base / 'palspec.txt').write_text(f'total {total}\n' + ''.join(f'more {k} {c} {st} {kind} {arg}\n' for k, st, c, kind, arg in more))
        (base / 'palprog.bin').write_bytes(prog)
        r = subprocess.run([str(self.exe), 'pal', str(base / 'palprog.bin'), str(base / 'palspec.txt'), str(out)], capture_output=True, text=True, timeout=60)
        if r.returncode == 1 and r.stdout.startswith('error: '):
            return r.stdout[7:].strip()
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        return (out / 'palettes.bin').read_bytes(), (out / 'palettes.known').read_bytes(), (out / 'palettes.own').read_bytes()

    def test_the_second_palette_chunk_is_runs_of_arcade_rows_and_filler(self):
        prog = self.world.arc.prog
        more = [(0, 2, 5, S.MORE_ROW, 0x275c40), (1, 2, 3, S.MORE_FILL, 0), (64, 1, 4, S.MORE_ROW, S.PALETTE_TABLES[1] + 0x400 * 3), (127, 1, 1, S.MORE_ROW, 0x275c40)]
        got = self.run_pal(prog, 4096, more)
        own = bytearray(4096)
        data, known = S.model_shared_palettes(prog, 4096, more, own)
        self.assertEqual(got, (bytes(data), bytes(known), bytes(own)))
        self.assertEqual(sum(1 for k in range(128) if known[32 * k]), 5 + 3 + 4 + 1)
        want = [0] + [S.arcade_color_to_ps1(x) or 0x8000 for x in struct.unpack_from('>15H', prog, 0x275c40 + 0x40)]
        self.assertEqual(list(struct.unpack_from('<16H', got[0], 2 * 32)), want)
        for bad in ([(126, 2, 2, S.MORE_FILL, 0)], [(0, 1, 1, S.MORE_ROW, 0x2fffe1)], [(0, 1, 1, S.MORE_ROW, 0x400000)]):
            with self.assertRaises(ValueError):
                S.model_shared_palettes(prog, 4096, bad)
            self.assertIsInstance(self.run_pal(prog, 4096, bad), str, bad)
        self.assertEqual(self.run_pal(prog, 0, []), (b'', b'', b''))

    def test_the_layer_sizes_the_ps1_has(self):
        d = self.world.descriptor()
        # a tall 16 pixel layer: 512 x 1024 pixels, 2 by 4 blocks, and a 512 wide 32 pixel layer (2 blocks)
        d['layers'][1].update({'across': 32, 'down': 64, 'ids': [0x1d120 + k % 4 for k in range(8)], 'map': 0x1200})
        d['layers'][2].update({'across': 16, 'down': 8, 'ids': [0x1d304, 0x1d305], 'map': 0x4400})
        d['sizes']['module'] = 0x6000
        maps, tiles = S.model_stage_raw(self.world.arc, d)
        d['layers'][2]['start'] = S.l2_start_after(d['layers'][1]['start'] + len(tiles[1]))
        self.same(self.world, d)
        d['layers'][2].update({'across': 32, 'down': 16, 'ids': [0x1d304 + k % 4 for k in range(8)]})     # two block rows of 32 pixel cells
        maps, tiles = S.model_stage_raw(self.world.arc, d)
        d['layers'][2]['start'] = S.l2_start_after(d['layers'][1]['start'] + len(tiles[1]))
        self.same(self.world, d)
        for count in (127, 128):                                            # the palette chunk's layout changes between 127 and 128 palettes
            built, _m = self.same(self.world, self.world.descriptor(count=count, pal_exact=2 ** 128 - 1))
            self.assertEqual(sum(built['palettes'][1]) // 32, {127: 64 + 32, 128: 64 + 32}[count])

    def test_malformed_input_is_refused_by_both(self):
        w = self.world
        cases = {}
        d = w.descriptor(); d['layers'][1]['ids'][0] = 0x1d130                 # an id that names no block
        prog = bytearray(w.arc.prog); prog[0x1d130] = 200
        bad = World(11); bad.arc = S.Arc(bytes(prog), w.arc.gfx); bad.prog = prog
        cases['no such block'] = (bad, d)
        d = w.descriptor(); d['layers'][0]['ids'][1] = 0x400000
        cases['id address outside the program'] = (w, d)
        d = w.descriptor(module=0x2000)
        cases['map does not fit the module'] = (w, d)
        d = w.descriptor(pages=4096)
        cases['pages too small'] = (w, d)
        d = w.descriptor(); d['layers'][1]['start'] = 63 * 256
        cases['slot beyond the codes'] = (w, d)
        d = w.descriptor(objects=[(0x10000 + 1, 3, 0)]); d['sizes']['object tiles'] = 128 * 4
        cases['runs shorter than the chunk'] = (w, d)
        d = w.descriptor(objects=[(0x10000 + 1, 3, 0)]); d['sizes']['object tiles'] = 128 * 2
        cases['runs longer than the chunk'] = (w, d)
        d = w.descriptor(objects=[(len(w.gfx) // 128 + 5, 1, 0)])
        cases['run tile outside the graphics'] = (w, d)
        d = w.descriptor(extras=[(300, 1, len(w.gfx) // 128, 0)])
        cases['extra tile outside the graphics'] = (w, d)
        d = w.descriptor(); d['layers'][2]['ids'] = [0x1d304]
        cases['wrong number of ids'] = (w, d)
        d = w.descriptor(); d['stage'] = 0x3000
        cases['palette rows outside the program'] = (w, d)
        for name, (world, d) in cases.items():
            with self.subTest(name):
                with self.assertRaises(ValueError):
                    S.model_stage(world.arc, d)
                got = self.run_c(world, d)
                self.assertIsInstance(got, str, f'{name}: the C built it')
                self.assertTrue(got, name)
        # nothing leaks or overflows after a refusal: a good build still works (the sanitizers would say)
        self.same(self.world, self.world.descriptor())

    def test_runs_entry_point(self):
        base = Path(self.dir.name)
        (base / 'runs.txt').write_text('total 640\nobject 0x10003 3 0\nobject ZERO 1 0\nobject DISC 1 0\n')
        out = base / 'runs_out'
        out.mkdir()
        r = subprocess.run([str(self.exe), 'runs', str(base / 'tiles.bin'), str(base / 'runs.txt'), str(out)], capture_output=True, text=True)
        self.assertEqual(r.returncode, 0, r.stdout)
        data, known = (out / 'runs.bin').read_bytes(), (out / 'runs.known').read_bytes()
        self.assertEqual(len(data), 640)
        self.assertEqual(known, b'\1' * 512 + b'\0' * 128)
        self.assertEqual(data[:128], self.world.arc.tile(0x10003, 16, False, False))

    def test_the_generated_table_is_sane(self):
        r = subprocess.run([str(self.exe), 'table'], capture_output=True, text=True)
        self.assertEqual(r.returncode, 0, r.stdout)
        self.assertIn('20 stages', r.stdout)

    def test_compiles_warning_free_for_32_bits(self):
        d = Path(self.dir.name)
        probe = d / 'probe.c'
        probe.write_text('int main(void){return 0;}\n')
        if subprocess.run(['gcc', '-m32', '-o', str(d / 'probe'), str(probe)], capture_output=True).returncode:
            self.skipTest('no 32-bit toolchain')
        for src in SOURCES:
            r = subprocess.run(['gcc', '-m32', *FLAGS, '-c', str(src), '-o', str(d / 'x32.o')], capture_output=True, text=True, timeout=60)
            self.assertEqual(r.returncode, 0, r.stderr)

    def test_native_build_has_no_warnings(self):
        d = Path(self.dir.name)
        for src in SOURCES:
            r = subprocess.run(['gcc', *FLAGS, '-O2', '-c', str(src), '-o', str(d / 'x.o')], capture_output=True, text=True, timeout=60)
            self.assertEqual((r.returncode, r.stderr), (0, ''))


class TableText(unittest.TestCase):
    def test_table_text(self):
        w = World(3)
        d = w.descriptor(extras=[(400, 3, 0x10005, 1)], holes=[(420, 2)], pal_exact=(1 << 70) | 5)
        d['file'] = 'STAGE03.PAC'
        d2 = w.descriptor(objects=[])
        d2['file'] = 'STAGE04.PAC'
        d2['stage'] = 4
        text = S.c_table([d, d2], [(0x10002, 1, 0), (S.RUN_ZERO, 2, 0)], [('STAGE05.PAC', 'layer maps not found in the module')])
        self.assertNotIn(chr(0x2014), text)
        self.assertIn('static const uint32_t ids_03_0[]', text)
        self.assertIn('static const struct arcade_stage_slots extras_03[]', text)
        self.assertIn('static const struct arcade_stage_hole holes_03[]', text)
        self.assertIn('{ARCADE_STAGE_ZERO, 2u, 0u}', text)
        self.assertIn('Left out: STAGE05.PAC: layer maps not found in the module', text)
        self.assertIn('0x40ull', text)                                   # pal_exact >> 64 of bit 70
        self.assertIn('const unsigned arcade_stage_count = 2u;', text)
        self.assertIn('{"STAGE03.PAC", 3u,', text)
        self.assertIn('{"STAGE04.PAC", 4u,', text)
        # the text compiles against the header and yields the rows
        d_ = tempfile.mkdtemp(prefix='arcade-stage-table-')
        try:
            (Path(d_) / 't.c').write_text(text)
            r = subprocess.run(['gcc', *FLAGS, '-c', '-I', str(ROOT / 'port/arcade'), str(Path(d_) / 't.c'), '-o', str(Path(d_) / 't.o')], capture_output=True, text=True)
            self.assertEqual(r.returncode, 0, r.stderr)
        finally:
            shutil.rmtree(d_)

    def test_table_text_with_extra_maps_and_shared_palettes_compiles(self):
        w = World(3)
        d = w.descriptor(xmaps=((), ({'map': 0x3000, 'ids': [2, 0]},), ({'map': 0x4100, 'ids': [1, 2, 0, 1]},)), module=0x6000, flags=(3, 0, 0),
                         cell_known=(0, 1 << 1030, 1 << 259), tile_known=(0, 1 << 3, 0))
        d['file'] = 'STAGE03.PAC'
        text = S.c_table([d], [(0x10002, 1, 0)], [], (4096, [(0, 2, 5, S.MORE_ROW, 0x275c40), (1, 2, 3, S.MORE_FILL, 0)]))
        self.assertIn('{12288u, 2u, xids_03_1_0}', text)
        self.assertIn('xmaps_03_2, 1u, 0, 0u, 0, 0u, 0, 0u, 0, 0u}', text)
        self.assertIn('const unsigned arcade_stage_shared_pal_count = 2u;', text)
        d_ = tempfile.mkdtemp(prefix='arcade-stage-table-')
        try:
            (Path(d_) / 't.c').write_text(text)
            r = subprocess.run(['gcc', *FLAGS, '-c', '-I', str(ROOT / 'port/arcade'), str(Path(d_) / 't.c'), '-o', str(Path(d_) / 't.o')], capture_output=True, text=True)
            self.assertEqual(r.returncode, 0, r.stderr)
        finally:
            shutil.rmtree(d_)
        none = S.c_table([d], [(0x10002, 1, 0)], [])             # no second palette chunk described
        self.assertIn('arcade_stage_shared_pal_bytes = 0u;', none)
        self.assertIn('arcade_stage_shared_pal = 0;', none)


if __name__ == '__main__':
    # The line the review reads (tools/ai_workflow/review.py): the last line, on a pass only.
    import time
    started = time.monotonic()
    result = unittest.main(exit=False).result
    took = time.monotonic() - started
    bad, ran = len(result.failures) + len(result.errors), result.testsRun
    if bad or not ran:
        print(f"{bad} of {ran} tests FAILED in {took:.1f} seconds")
        sys.exit(1)
    print(f"all {ran} tests passed in {took:.1f} seconds")
