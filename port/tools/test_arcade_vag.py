#!/usr/bin/env python3
"""Seconds: the C sample converter (port/arcade/vag.c) on synthetic data; no game files.

The library is built warning-free (-std=c99 -Wall -Wextra -Werror -pedantic) natively, also with -m32 when that links, and run in a driver
with the address and undefined-behavior sanitizers. Made-up sample ROMs and a made-up Z80 instrument table go through it: the SPU ADPCM
decoder against the Python one (samples.py), the encoder byte for byte against the Python model (samples.py `vag_model`) at several rates,
the block-count rule, the PS1 flags, the quality of the encode, silence outside the sample, every refusal (nothing written past the room),
the instrument reader and the table functions with a made-up table.
"""
import math
import os
import random
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'port/arcade'))
import samples as S   # noqa: E402

DIR = ROOT / 'port/arcade'
FLAGS = ['-std=c99', '-O1', '-Wall', '-Wextra', '-Werror', '-pedantic']
TMP = tempfile.TemporaryDirectory(prefix='vag-test-')

TABLE = r'''
#include "vag.h"
const struct arcade_vag_sample arcade_vag_samples[] = {{1, 22050, 0, 0}, {0xffff, 0, 77, 0}, {2, 40000, 40, 1}};
const unsigned arcade_vag_sample_count = 3;
static const uint16_t ids[] = {2, 1, 0, 0};
const struct arcade_vag_bank arcade_vag_banks[] = {{"PL00", 0, 3, 0}, {"PL00X", 1, 1, 3}};
const unsigned arcade_vag_bank_count = 2;
const uint16_t *arcade_vag_bank_ids(const struct arcade_vag_bank *bank) { return ids + bank->first; }
'''

DRIVER = r'''
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vag.h"
static unsigned char *slurp(const char *p, size_t *n) {
    FILE *f = fopen(p, "rb"); unsigned char *b; long len;
    if (!f) exit(3);
    fseek(f, 0, SEEK_END); len = ftell(f); rewind(f);
    b = malloc(len ? len : 1); if (fread(b, 1, len, f) != (size_t)len) exit(3); fclose(f);
    *n = (size_t)len; return b;
}
static void spit(const char *p, const void *b, size_t n) { FILE *f = fopen(p, "wb"); if (!f) exit(3); fwrite(b, 1, n, f); fclose(f); }
int main(int argc, char **argv) {
    char err[128] = "";
    const char *cmd = argc > 1 ? argv[1] : "";
    size_t n, m;
    if (!strcmp(cmd, "encode")) {            /* encode ROM start length rate blocks OUT [room] */
        unsigned char *rom = slurp(argv[2], &n), *out;
        unsigned blocks = (unsigned)atol(argv[6]); size_t room = argc > 8 ? (size_t)atol(argv[8]) : (size_t)blocks * 16;
        int r;
        out = malloc(room ? room : 1);   /* exactly the room: the sanitizer sees any write past it */
        memset(out, 0xa5, room);
        r = arcade_vag_encode(rom, n, (unsigned)atol(argv[3]), (unsigned)atol(argv[4]), (unsigned)atol(argv[5]), blocks, out, room, err, sizeof err);
        if (r) { int touched = 0; size_t i; for (i = 0; i < room; i++) touched |= out[i] != 0xa5; fprintf(stderr, "%s%s\n", err, touched ? " TOUCHED" : ""); return 1; }
        spit(argv[7], out, room); return 0;
    }
    if (!strcmp(cmd, "decode")) {            /* decode IN OUT */
        unsigned char *in = slurp(argv[2], &n); int16_t *pcm = malloc(sizeof(int16_t) * (n / 16 * 28 + 1)); unsigned char *fl = malloc(n / 16 + 1);
        arcade_vag_decode(in, n, pcm, n / 16 * 28, &m, fl);
        spit(argv[3], pcm, m * 2); spit(argv[4], fl, n / 16); printf("%zu\n", m); return 0;
    }
    if (!strcmp(cmd, "instrument")) {        /* instrument SOUND index */
        unsigned char *snd = slurp(argv[2], &n); struct arcade_instrument in;
        if (arcade_instrument(snd, n, (unsigned)atol(argv[3]), &in)) { puts("refused"); return 1; }
        printf("%u %u %u %u\n", in.start, in.loop, in.end, in.root); return 0;
    }
    if (!strcmp(cmd, "blocks")) { printf("%u\n", arcade_vag_blocks((unsigned)strtoul(argv[2], 0, 10), (unsigned)strtoul(argv[3], 0, 10))); return 0; }
    if (!strcmp(cmd, "table")) {             /* table ROM SOUND */
        unsigned char *rom = slurp(argv[2], &n), *snd = slurp(argv[3], &m), *out = malloc(1 << 16); size_t size = 0; unsigned i;
        const struct arcade_vag_bank *b = arcade_vag_find_bank("PL00", 0);
        printf("bank %d count %d ids", b != NULL, b ? b->count : -1);
        if (b) for (i = 0; i < b->count; i++) printf(" %u", arcade_vag_bank_ids(b)[i]);
        printf("\nfind %d %d %d\n", arcade_vag_find_bank("PL00", 1) != NULL, arcade_vag_find_bank("PL00X", 1) != NULL, arcade_vag_find_bank(NULL, 0) != NULL);
        for (i = 0; i < 4; i++) {
            int r = arcade_vag_build(rom, n, snd, m, i, out, 1 << 16, &size, err, sizeof err);
            printf("build %u -> %d size %zu %s\n", i, r, size, err);
            if (!r) { char name[64]; snprintf(name, sizeof name, "%s.%u", argv[4], i); spit(name, out, size); }
        }
        return 0;
    }
    return 2;
}
'''


class Built:
    exe = None
    plain = None


def build():
    if Built.exe:
        return
    d = Path(TMP.name)
    (d / 'driver.c').write_text(DRIVER)
    (d / 'table.c').write_text(TABLE)
    srcs = [str(d / 'driver.c'), str(DIR / 'vag.c'), str(d / 'table.c')]
    # warnings as errors, native, plain
    subprocess.run(['gcc', *FLAGS, '-I', str(DIR), '-c', str(DIR / 'vag.c'), '-o', str(d / 'vag.o')], check=True, capture_output=True, timeout=60)
    subprocess.run(['gcc', *FLAGS, '-I', str(DIR), '-c', str(d / 'table.c'), '-o', str(d / 'table.o')], check=True, capture_output=True, timeout=60)
    # sanitized driver
    subprocess.run(['gcc', '-std=c99', '-O1', '-g', '-fsanitize=address,undefined', '-fno-sanitize-recover=undefined', '-Wall', '-Wextra', '-Werror',
                    '-I', str(DIR), '-o', str(d / 'drv'), *srcs, '-lm'], check=True, capture_output=True, text=True, timeout=120)
    Built.exe = d / 'drv'
    # -m32: compile only (the 32-bit run needs a 32-bit libm); skipped when this machine cannot
    probe = d / 'probe.c'
    probe.write_text('int main(void){return 0;}\n')
    Built.m32 = subprocess.run(['gcc', '-m32', '-o', str(d / 'probe'), str(probe)], capture_output=True).returncode == 0
    if Built.m32:
        subprocess.run(['gcc', '-m32', *FLAGS, '-I', str(DIR), '-c', str(DIR / 'vag.c'), '-o', str(d / 'vag32.o')], check=True, capture_output=True, timeout=60)


def run(*args, check=True):
    build()
    r = subprocess.run([str(Built.exe), *map(str, args)], capture_output=True, text=True, timeout=120,
                       env={**os.environ, 'ASAN_OPTIONS': 'detect_leaks=0'})   # the driver leaks its buffers on purpose
    if check and r.returncode:
        raise AssertionError(f'driver {args[0]} failed ({r.returncode}): {r.stderr[-400:]}')
    return r


def signed(b):
    return b - 256 if b > 127 else b


def synth_rom(n, seed=1):
    """Signed 8-bit test material (chip order): two tones and a little noise, as bytes."""
    rng = random.Random(seed)
    return bytes((round(40 * math.sin(0.05 * k) + 25 * math.sin(0.31 * k + 1) + rng.gauss(0, 2)) + 256) & 255 for k in range(n))


def encode(rom, start, length, rate, blocks, name='enc', room=None, check=True):
    d = Path(TMP.name)
    (d / 'rom.bin').write_bytes(rom)
    args = ['encode', d / 'rom.bin', start, length, rate, blocks, d / name]
    if room is not None:
        args.append(room)
    r = run(*args, check=check)
    return r if not check else (d / name).read_bytes()


def instrument_view(records, size=0x8000 + 8 * 512):
    """A made-up Z80 view with records [(bank, start, loop, end, root)] at 0x8000."""
    v = bytearray(size)
    for i, (bank, start, loop, end, root) in enumerate(records):
        struct.pack_into('<BHHHB', v, 0x8000 + 8 * i, bank, start, loop, end, root)
    return bytes(v)


class Decoder(unittest.TestCase):
    def test_decode_matches_python(self):
        rng = random.Random(5)
        data = bytearray()
        for _ in range(40):
            # every filter 0..4 and shifts 0..12, plus the out-of-range values the decoder treats alike (shift 13..15, filter 5..7)
            data += bytes([(rng.randrange(8) << 4) | rng.randrange(16), rng.choice([0, 0, 2, 4])]) + bytes(rng.randrange(256) for _ in range(14))
        d = Path(TMP.name)
        (d / 'in.bin').write_bytes(bytes(data))
        out = run('decode', d / 'in.bin', d / 'pcm.bin', d / 'fl.bin')
        pcm, flags = S.decode_adpcm(bytes(data))
        self.assertEqual(int(out.stdout), len(pcm))
        self.assertEqual(list(struct.unpack(f'<{len(pcm)}h', (d / 'pcm.bin').read_bytes())), pcm)
        self.assertEqual(list((d / 'fl.bin').read_bytes()), flags)

    def test_short_input_decodes_whole_blocks_only(self):
        d = Path(TMP.name)
        (d / 'in.bin').write_bytes(bytes([0x0c, 0] + [0x11] * 14 + [0x0c]))
        self.assertEqual(int(run('decode', d / 'in.bin', d / 'pcm.bin', d / 'fl.bin').stdout), 28)


class Rule(unittest.TestCase):
    def test_blocks_rule_equals_the_model(self):
        for length in (1, 2, 100, 671, 672, 673, 9999, 16381, 22525):
            for rate in (4000, 11025, 22050, 24000, 26222, 44100, 48000):
                got = int(run('blocks', length, rate).stdout)
                self.assertEqual(got, S.vag_blocks(length, rate), (length, rate))
        # a hand-computed case: 11751 data at 44100 Hz is 21594 samples, 772 blocks, plus two
        self.assertEqual(S.vag_blocks(11751, 44100), 774)

    def test_blocks_refuses_bad_arguments(self):
        self.assertEqual(int(run('blocks', 0, 22050).stdout), 0)
        self.assertEqual(int(run('blocks', 100, 3999).stdout), 0)
        self.assertEqual(int(run('blocks', 100, 48001).stdout), 0)


class Instruments(unittest.TestCase):
    def test_reads_records(self):
        view = instrument_view([(0, 0, 0x5007, 0x7ffe, 60), (0x3f, 0xc6c9, 0xcffb, 0xcffd, 72)])
        d = Path(TMP.name)
        (d / 'z80.bin').write_bytes(view)
        self.assertEqual(run('instrument', d / 'z80.bin', 0).stdout.split(), ['0', '20487', '32766', '60'])
        self.assertEqual(run('instrument', d / 'z80.bin', 1).stdout.split(), [str(0x3fc6c9), str(0x3fcffb), str(0x3fcffd), '72'])
        parsed = S.parse_instruments(view)
        self.assertEqual((parsed[1]['start'], parsed[1]['end'], parsed[1]['root']), (0x3fc6c9, 0x3fcffd, 72))

    def test_refuses(self):
        d = Path(TMP.name)
        for name, recs, idx in (('range', [(0, 0, 1, 2, 60)], 435), ('bank', [(0x40, 0, 1, 2, 60)], 0), ('order', [(1, 5, 3, 9, 60)], 0),
                                ('order2', [(1, 0, 9, 3, 60)], 0)):
            (d / f'{name}.bin').write_bytes(instrument_view(recs))
            self.assertEqual(run('instrument', d / f'{name}.bin', idx, check=False).returncode, 1, name)
        (d / 'short.bin').write_bytes(bytes(0x8000 + 8 * 100))
        self.assertEqual(run('instrument', d / 'short.bin', 434, check=False).returncode, 1)
        with self.assertRaises(S.FormatError):
            S.parse_instruments(bytes(0x8000 + 8 * 100))
        with self.assertRaises(S.FormatError):
            S.parse_instruments(instrument_view([(0x41, 0, 0, 0, 0)]))


class Encoder(unittest.TestCase):
    def test_equals_the_python_model_at_several_rates(self):
        rom = synth_rom(3000)
        chip = [signed(b) for b in rom]
        tab = S.kernel_table()
        for start, length, rate in ((100, 300, 24000), (0, 257, 44100), (500, 640, 11025), (1000, 500, 28000), (50, 333, 40000),
                                    (7, 280, 22050), (1500, 400, 26222), (2000, 200, 14000)):
            blocks = S.vag_blocks(length, rate)
            ours = encode(rom, start, length, rate, blocks)
            model = S.vag_model(chip, start, length, rate, blocks, tab=tab)
            self.assertEqual(len(ours), 16 * blocks)
            self.assertEqual(ours, model, (start, length, rate))

    def test_structure_and_flags(self):
        rom = synth_rom(2000)
        ours = encode(rom, 0, 600, 32000, S.vag_blocks(600, 32000))
        pcm, flags = S.decode_adpcm(ours)
        self.assertEqual(S.flag_structure(flags), 'oneshot')
        self.assertEqual(ours[:16], bytes(16))                        # the zero warm-up block
        self.assertEqual(ours[-16:], bytes([0, 7]) + bytes([0x77] * 14))   # the closing block
        self.assertEqual(pcm[:28], [0] * 28)
        self.assertEqual(len(pcm), 28 * (len(ours) // 16))

    def test_quality_and_gain(self):
        rom = synth_rom(4000, 3)
        chip = [signed(b) for b in rom]
        for rate in (24000, 44100, 28000):
            length = 1500
            ours = encode(rom, 200, length, rate, S.vag_blocks(length, rate))
            pcm, _ = S.decode_adpcm(ours)
            rho = 24000 / rate
            ref = S.resample_lanczos(chip[200:200 + length], 0, rho, 28 * 30, 8)
            got = pcm[28:28 + len(ref)]
            self.assertGreater(S.snr_db([256 * v for v in ref], got), 24, rate)
        # the amplitude is 256 times the 8-bit value
        flat = bytes([40] * 400)
        pcm, _ = S.decode_adpcm(encode(flat, 0, 400, 24000, S.vag_blocks(400, 24000)))
        mid = pcm[28 + 100:28 + 300]
        self.assertTrue(all(abs(v - 40 * 256) < 400 for v in mid), (min(mid), max(mid)))

    def test_silence_outside_the_sample(self):
        # more blocks than the data needs: the rest decodes to nothing; and nothing leaks from the data before the start
        rom = bytes([100] * 600)
        ours = encode(rom, 300, 100, 24000, 20)
        pcm, _ = S.decode_adpcm(ours)
        self.assertTrue(all(abs(v) < 80 for v in pcm[28 + 150:28 + 18 * 28]), 'the tail is silent')
        # what lies before the start or after the end does not reach the output, even at a rate whose kernel is wide
        other = bytes([-90 & 255] * 300) + rom[300:400] + bytes([77] * 200)
        self.assertEqual(encode(rom, 300, 100, 44100, S.vag_blocks(100, 44100)), encode(other, 300, 100, 44100, S.vag_blocks(100, 44100), 'o'))
        self.assertEqual(encode(rom, 300, 100, 11025, S.vag_blocks(100, 11025)), encode(other, 300, 100, 11025, S.vag_blocks(100, 11025), 'o'))

    def test_length_past_the_rom_is_clipped(self):
        rom = synth_rom(500)
        a = encode(rom, 400, 100, 24000, 10, 'a')
        b = encode(rom, 400, 5000, 24000, 10, 'b')
        self.assertEqual(a, b)

    def test_refusals_write_nothing(self):
        rom = synth_rom(1000)
        for name, args, room in (('rate low', (0, 100, 3999, 8), None), ('rate high', (0, 100, 48001, 8), None), ('blocks', (0, 100, 24000, 3), None),
                                 ('start', (1000, 100, 24000, 8), None), ('length', (0, 0, 24000, 8), None), ('room', (0, 100, 24000, 8), 8 * 16 - 1)):
            r = encode(rom, *args, name='refused', room=room, check=False)
            self.assertEqual(r.returncode, 1, name)
            self.assertNotIn('TOUCHED', r.stderr, name)
            self.assertTrue(r.stderr.startswith('vag:'), (name, r.stderr))
        # exactly enough room works (the sanitizer watches the heap block)
        self.assertEqual(len(encode(rom, 0, 100, 24000, 8, room=8 * 16)), 8 * 16)

    def test_m32_compiles(self):
        build()
        if not Built.m32:
            self.skipTest('no 32-bit toolchain here')
        self.assertTrue((Path(TMP.name) / 'vag32.o').exists())


class Table(unittest.TestCase):
    def test_lookup_and_build(self):
        d = Path(TMP.name)
        rom = synth_rom(9000)
        # instrument 1 = samples 0x1000..; instrument 2 = a short one; the table's sample 0 uses instrument 1 with 22050 Hz and blocks 0 (refused)
        view = instrument_view([(0, 0, 0, 0, 60), (0, 1000, 1200, 1699, 72), (0, 3000, 3000, 3029, 72)])
        (d / 'rom.bin').write_bytes(rom)
        (d / 'z80.bin').write_bytes(view)
        r = run('table', d / 'rom.bin', d / 'z80.bin', d / 'built', check=True)
        lines = r.stdout.splitlines()
        self.assertEqual(lines[0], 'bank 1 count 3 ids 2 1 0')
        self.assertEqual(lines[1], 'find 0 1 0')
        self.assertTrue(lines[2].startswith('build 0 -> -1'), lines[2])       # blocks 0 in the made-up table: refused
        self.assertTrue(lines[3].startswith('build 1 -> -1') and 'no source' in lines[3], lines[3])
        self.assertTrue(lines[4].startswith('build 2 -> 0 size 640'), lines[4])
        built = (d / 'built.2').read_bytes()
        self.assertEqual(built, S.vag_model([signed(b) for b in rom], 3000, 30, 40000, 40))
        self.assertTrue(lines[5].startswith('build 3 -> -1') and 'no such sample' in lines[5], lines[5])


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
