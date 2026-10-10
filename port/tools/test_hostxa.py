#!/usr/bin/env python3
"""Controls for the decoder of the disc's streamed audio, port/src/xa.c, built natively with the host's `cc`.

    python3 test_hostxa.py [--xa FILE] [--cc CC]

What is tested: xa.c alone (no other file of the runtime, no disc, no cross compiler) on sectors of invented bytes. A small C
program (below) feeds raw sectors to xa_decode, or source frames to xa_resample, and writes what comes out. This file holds a
reference written from the same documented rule in another form than the C: the C walks the groups and blocks and keeps each
channel's history in an array; the reference first cuts the sector into blocks of nibbles, then runs one channel's blocks through
a single loop, with Python's floor division and integer powers, and the resampler's outputs are found from their position in the
whole stream (output j at j * rate / 44,100 source frames) where the C carries a phase from call to call. No code is shared.
The rule is the console's hardware documentation (the description of CD-ROM XA ADPCM in the "psx-spx" document):

    s = (t << shift) + ((old * f0 + older * f1 + 32) >> 6), shift = 12 - range (a range above 12 is taken as 9),
    (f0, f1) = (0, 0), (60, 0), (115, -52), (98, -55) by the filter, clamped to 16 bits, the clamped value is the history.

The cases, each comparing every sample: every filter at the ranges 0, 6 and 12; the ranges 13 to 15; clamps (the test checks that
the two limits are reached); stereo and mono at 37,800 and 18,900 Hz; the history across two and three sectors; a sector decoded
fresh and after another (they must differ, so that the history matters); random sectors from a fixed seed; the subheader's two
copies differing (refused, history untouched); an 8-bit sector; a sector that is not audio; the `+ 32` and the rounding of a
negative value, with expected samples written out by hand (a value that comes out differently with and without the `+ 32` and
with a division that truncates); the resampler against the reference over a whole stream at several chunk sizes, and by hand on
four frames.

--xa FILE names another copy of xa.c (the mutants of the controls run on a scratch copy; see docs/records/). Every child is
run under a time limit and its exit status is checked first; a missing line or another status is a failure.

Every case prints `ok NAME` or `FAIL NAME: why`; the last line is `all N cases behaved as required` or the number that did not.
"""

from __future__ import annotations

import argparse
import random
import shutil
import struct
import subprocess
import sys
import tempfile
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
SRC = HERE.parent / "src"

SECTOR = 2352
LIMIT = 30   # seconds for any child

C_MAIN = r'''
#include "xa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void put32(int32_t v) { fwrite(&v, 4, 1, stdout); }

/* decode: raw sectors on standard input, one history for the whole run. Per sector: the result code; for XA_OK the frame count, the
 * source rate, the stereo flag, then the frames (int16, left and right). */
static int decode(void)
{
    static uint8_t raw[XA_SECTOR];
    static int16_t out[XA_MAX_FRAMES * 2];
    struct xa_state st;
    struct xa_info info;
    xa_reset(&st);
    while (fread(raw, 1, XA_SECTOR, stdin) == XA_SECTOR) {
        int r = xa_decode(&st, raw, out, &info);
        put32(r);
        if (r == XA_OK) {
            put32((int32_t)info.frames);
            put32(info.rate);
            put32(info.stereo);
            fwrite(out, 2, info.frames * 2u, stdout);
        }
    }
    return 0;
}

/* resample RATE CHUNK: int16 stereo frames on standard input, fed in calls of CHUNK frames (the last one shorter). */
static int resample(int rate, unsigned chunk)
{
    static int16_t in[1 << 22];
    static int16_t out[1 << 23];
    struct xa_state st;
    size_t n = fread(in, 4, sizeof in / 4, stdin), at = 0;
    xa_reset(&st);
    while (at < n) {
        unsigned take = (unsigned)(n - at < chunk ? n - at : chunk), got;
        if (xa_resampled_bound(take, (unsigned)rate) * 2u > sizeof out / 2) return 3;
        got = xa_resample(&st, in + at * 2, take, (unsigned)rate, out);
        fwrite(out, 4, got, stdout);
        at += take;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 2 && !strcmp(argv[1], "decode")) return decode();
    if (argc == 4 && !strcmp(argv[1], "resample")) return resample(atoi(argv[2]), (unsigned)atoi(argv[3]));
    return 64;
}
'''


# ---- the reference -------------------------------------------------------------

COEF = [(0, 0), (60, 0), (115, -52), (98, -55)]
OK, BAD_SUBHEADER, EIGHT_BIT, NOT_AUDIO = 0, -1, -2, -3


def ref_block(nibbles, header, hist):
    """28 samples of one block; hist is [old, older] of its channel, updated in place."""
    rng, filt = header & 15, (header >> 4) & 3
    shift = 12 - (9 if rng > 12 else rng)
    f0, f1 = COEF[filt]
    out = []
    for n in nibbles:
        t = n - 16 if n >= 8 else n
        s = t * 2 ** shift + (hist[0] * f0 + hist[1] * f1 + 32) // 64
        s = max(-32768, min(32767, s))
        hist[1], hist[0] = hist[0], s
        out.append(s)
    return out


class RefDecoder:
    def __init__(self):
        self.hist = [[0, 0], [0, 0]]

    def sector(self, raw):
        """(result code, frames) with frames a list of (left, right); the history changes only on OK."""
        if raw[16:20] != raw[20:24]:
            return BAD_SUBHEADER, None
        _, _, submode, coding = raw[16:20]
        if not submode & 4:
            return NOT_AUDIO, None
        if coding & 0x10:
            return EIGHT_BIT, None
        stereo = coding & 1
        rate = 18900 if coding & 4 else 37800
        hist = [list(h) for h in self.hist]
        blocks = []   # (channel, samples) in the order of the sector
        for g in range(18):
            grp = raw[24 + 128 * g: 24 + 128 * (g + 1)]
            for b in range(8):
                words = [grp[16 + 4 * j + b // 2] for j in range(28)]
                nibbles = [(w >> (4 * (b % 2))) & 15 for w in words]
                ch = b % 2 if stereo else 0
                blocks.append((ch, ref_block(nibbles, grp[4 + b], hist[ch])))
        self.hist = hist
        if stereo:
            left = [s for ch, smp in blocks if ch == 0 for s in smp]
            right = [s for ch, smp in blocks if ch == 1 for s in smp]
            frames = list(zip(left, right))
        else:
            frames = [(s, s) for _, smp in blocks for s in smp]
        return OK, frames


def ref_decode(sectors):
    """[(code, frames, rate, stereo)] for a run of sectors with one history."""
    d = RefDecoder()
    out = []
    for raw in sectors:
        code, frames = d.sector(raw)
        if code != OK:
            out.append((code, None, None, None))
        else:
            out.append((OK, frames, 18900 if raw[19] & 4 else 37800, raw[19] & 1))
    return out


def ref_resample(frames, rate):
    """Linear interpolation to 44,100 Hz over the whole stream. The stream is taken as its first frame once more before it; output j
    sits at j * rate / 44,100 frames of that stream; it exists once the frame after it is known; the division truncates toward zero."""
    stream = [frames[0]] + list(frames) if frames else []
    out = []
    j = 0
    while stream:
        pos = j * rate
        idx, frac = divmod(pos, 44100)
        if idx + 1 > len(stream) - 1:
            break
        a, b = stream[idx], stream[idx + 1]
        pair = []
        for c in (0, 1):
            d = (b[c] - a[c]) * frac
            q = abs(d) // 44100
            pair.append(a[c] + (q if d >= 0 else -q))
        out.append(tuple(pair))
        j += 1
    return out


# ---- the sectors ---------------------------------------------------------------

def make_sector(file=1, channel=0, submode=0x64, coding=1, headers=None, data=None, copy=None):
    """headers: 8 header bytes used for every group (or a function (group, block) -> byte); data: a function (group, word, byte) -> byte
    for the 112 data bytes of a group, or None for zeros. copy: the subheader's second copy if it is to differ."""
    raw = bytearray(SECTOR)
    raw[16:20] = bytes([file, channel, submode, coding])
    raw[20:24] = bytes(copy) if copy is not None else raw[16:20]
    for g in range(18):
        base = 24 + 128 * g
        for b in range(8):
            h = headers(g, b) if callable(headers) else (headers[b] if headers else 0)
            raw[base + 4 + b] = h
            raw[base + b] = h if b < 4 else raw[base + b]
        for k in range(4):
            raw[base + 12 + k] = raw[base + 8 + k]
        for j in range(28):
            for k in range(4):
                raw[base + 16 + 4 * j + k] = data(g, j, k) & 255 if data else 0
    return bytes(raw)


def rand_data(rnd):
    table = {}

    def f(g, j, k):
        return table.setdefault((g, j, k), rnd.randrange(256))
    return f


# ---- running ---------------------------------------------------------------------

def run_c(exe: Path, args: list[str], data: bytes):
    t0 = time.time()
    try:
        p = subprocess.run([str(exe), *args], input=data, capture_output=True, timeout=LIMIT)
    except subprocess.TimeoutExpired:
        return None, b""
    return p.returncode, p.stdout


def read_decoded(buf: bytes):
    out, at = [], 0
    while at < len(buf):
        (code,) = struct.unpack_from("<i", buf, at)
        at += 4
        if code != OK:
            out.append((code, None, None, None))
            continue
        frames, rate, stereo = struct.unpack_from("<iii", buf, at)
        at += 12
        smp = struct.unpack_from(f"<{frames * 2}h", buf, at)
        at += frames * 4
        out.append((OK, list(zip(smp[0::2], smp[1::2])), rate, stereo))
    return out


def decode_both(exe, sectors):
    rc, buf = run_c(exe, ["decode"], b"".join(sectors))
    if rc != 0:
        return rc, None, None
    return rc, read_decoded(buf), ref_decode(sectors)


def same(got, want):
    """None when equal, else the first difference."""
    if len(got) != len(want):
        return f"{len(got)} results, expected {len(want)}"
    for i, (g, w) in enumerate(zip(got, want)):
        if g[0] != w[0]:
            return f"sector {i}: code {g[0]}, expected {w[0]}"
        if g[0] != OK:
            continue
        if g[2:] != w[2:]:
            return f"sector {i}: rate/stereo {g[2:]}, expected {w[2:]}"
        if len(g[1]) != len(w[1]):
            return f"sector {i}: {len(g[1])} frames, expected {len(w[1])}"
        for k, (a, b) in enumerate(zip(g[1], w[1])):
            if a != b:
                return f"sector {i}: frame {k} is {a}, expected {b}"
    return None


def check(name, bad):
    return name, bad


def compare_case(exe, name, sectors, extra=None):
    rc, got, want = decode_both(exe, sectors)
    if rc != 0:
        return check(name, f"the child ended with status {rc}")
    bad = same(got, want)
    if bad is None and extra:
        bad = extra(want)
    return check(name, bad)


# ---- cases ------------------------------------------------------------------------

def cases(exe: Path, tmp: Path):
    rnd = random.Random(20261010)

    # every filter at ranges 0, 6 and 12 (stereo; random nibbles; the same header in every block of the sector)
    for filt in range(4):
        for rng in (0, 6, 12):
            sec = make_sector(headers=[(filt << 4) | rng] * 8, data=rand_data(rnd))
            yield compare_case(exe, f"filter {filt} at range {rng}, stereo", [sec])
    for rng in (13, 14, 15):
        sec = make_sector(headers=[(3 << 4) | rng] * 8, data=rand_data(rnd))
        yield compare_case(exe, f"range {rng} (above 12), filter 3", [sec])

    # clamps: the highest nibble 7 with the shift 12 and filter 1 climbs to 32767; nibble 8 climbs down to -32768
    def reaches(lo, hi):
        def ex(want):
            vals = [v for r in want for fr in r[1] for v in fr]
            if hi and 32767 not in vals:
                return "the made-up sector does not reach 32767"
            if lo and -32768 not in vals:
                return "the made-up sector does not reach -32768"
            return None
        return ex
    hi = make_sector(headers=[0x10] * 8, data=lambda g, j, k: 0x77)
    lo = make_sector(headers=[0x10] * 8, data=lambda g, j, k: 0x88)
    yield compare_case(exe, "clamp at 32767", [hi], reaches(False, True))
    yield compare_case(exe, "clamp at -32768", [lo], reaches(True, False))
    yield compare_case(exe, "clamp at both ends, filter 3, up then down", [make_sector(headers=[0x30] * 8, data=lambda g, j, k: 0x77 if j < 14 else 0x88)] * 2, reaches(True, True))
    # and the history is the clamped value (an unclamped one would come back from beyond the limit): a long climb, then zeros
    def up_then_down(g, j, k):
        return 0x77 if g < 3 else 0x00
    yield compare_case(exe, "after a clamp the history is the clamped value", [make_sector(headers=[0x10] * 8, data=up_then_down)], reaches(False, True))

    # stereo and mono at both rates
    for coding, label in ((0x01, "stereo 37800"), (0x05, "stereo 18900"), (0x00, "mono 37800"), (0x04, "mono 18900")):
        sec = make_sector(coding=coding, headers=lambda g, b: ((b % 4) << 4) | ((g + b) % 13), data=rand_data(rnd))
        yield compare_case(exe, f"{label} Hz, headers of every kind", [sec])

    # the history across sectors
    a = make_sector(headers=[0x2a] * 8, data=rand_data(rnd))
    b = make_sector(headers=[0x1c] * 8, data=rand_data(rnd))
    c = make_sector(headers=[0x3b] * 8, data=rand_data(rnd))
    yield compare_case(exe, "history across two sectors", [a, b])
    yield compare_case(exe, "history across three sectors", [a, b, c])
    yield compare_case(exe, "history across three mono sectors", [make_sector(coding=0, headers=[0x2a] * 8, data=rand_data(rnd)), make_sector(coding=0, headers=[0x1c] * 8, data=rand_data(rnd)), make_sector(coding=4, headers=[0x3b] * 8, data=rand_data(rnd))])

    def history_matters():
        rc1, got1, _ = decode_both(exe, [b])
        rc2, got2, _ = decode_both(exe, [a, b])
        if rc1 != 0 or rc2 != 0:
            return f"status {rc1} and {rc2}"
        return None if got1[0][1] != got2[1][1] else "the second sector decodes the same with and without the first before it"
    yield check("a sector after another differs from the same sector alone (the history is carried)", history_matters())

    # random sectors, fixed seed: a run of 40 with random headers and data
    run = []
    for _ in range(40):
        coding = rnd.choice([0, 1, 4, 5])
        run.append(make_sector(coding=coding, headers=lambda g, b: rnd.randrange(256), data=rand_data(rnd)))
    yield compare_case(exe, "40 random sectors in a row (seed 20261010)", run)

    # refusals; the history stays what it was
    bad_copy = make_sector(headers=[0x2a] * 8, data=rand_data(rnd), copy=[1, 0, 0x64, 2])
    yield compare_case(exe, "the subheader's two copies differ: refused, and the history is untouched", [a, bad_copy, b])
    for pos in (16, 17, 18, 19):
        raw = bytearray(a)
        raw[20 + pos - 16] ^= 0x01
        yield compare_case(exe, f"a copy differing at byte {20 + pos - 16} is refused", [bytes(raw)])
    yield compare_case(exe, "an 8-bit sector is refused, history untouched", [a, make_sector(coding=0x11, headers=[0x2a] * 8, data=rand_data(rnd)), b])
    yield compare_case(exe, "a sector that is not audio (submode without 0x04) is refused", [a, make_sector(submode=0x08, headers=[0x2a] * 8, data=rand_data(rnd)), b])

    # by hand: a mono sector, block 0 only, filter 1 range 12 (shift 0). With the + 32 a value of 7 stays 7 ((7 * 60 + 32) >> 6 = 7);
    # without it the next samples would be 6, 5, 4 ((7 * 60) >> 6 = 6). A value of -1 stays -1 ((-60 + 32) >> 6 = -1, rounding down);
    # a division that truncates would give 0.
    def block0(first_nibble):
        def d(g, j, k):
            return first_nibble if g == 0 and j == 0 and k == 0 else 0
        return make_sector(coding=0, headers=lambda g, b: 0x1c if (g == 0 and b == 0) else 0, data=d)

    def hand(sector, want):
        rc, buf = run_c(exe, ["decode"], sector)
        if rc != 0:
            return f"the child ended with status {rc}"
        got = [fr[0] for fr in read_decoded(buf)[0][1][:len(want)]]
        return None if got == want else f"the first {len(want)} samples are {got}, written out by hand {want}"
    yield check("by hand, the + 32: nibble 7, filter 1, shift 0 gives 7, 7, 7, 7, 7, 7", hand(block0(7), [7] * 6))
    yield check("by hand, rounding down: nibble -1 (15), filter 1, shift 0 gives -1 forever", hand(block0(15), [-1] * 6))
    # filter 2, nibble 7, shift 0, from the rule: s0 = 7; s1 = (7 * 115 + 32) >> 6 = 837 >> 6 = 13;
    # s2 = (13 * 115 - 7 * 52 + 32) >> 6 = 1163 >> 6 = 18; s3 = (18 * 115 - 13 * 52 + 32) >> 6 = 1426 >> 6 = 22
    f2 = make_sector(coding=0, headers=lambda g, b: 0x2c if (g == 0 and b == 0) else 0, data=lambda g, j, k: 7 if (g == 0 and j == 0 and k == 0) else 0)
    yield check("by hand, filter 2: nibble 7 at shift 0 gives 7, 13, 18, 22", hand(f2, [7, 13, 18, 22]))

    # the resampler
    n = 9000
    for rate in (37800, 18900):
        frames = [(rnd.randrange(-32768, 32768), rnd.randrange(-32768, 32768)) for _ in range(n)]
        want = ref_resample(frames, rate)
        data = b"".join(struct.pack("<hh", *f) for f in frames)
        for chunk in (1, 7, 28, 1000, 2016, 4032, n):
            rc, buf = run_c(exe, ["resample", str(rate), str(chunk)], data)
            if rc != 0:
                yield check(f"resample {rate} Hz in calls of {chunk}", f"the child ended with status {rc}")
                continue
            got = list(struct.iter_unpack("<hh", buf))
            bad = None
            if len(got) != len(want):
                bad = f"{len(got)} frames, expected {len(want)}"
            else:
                for k, (x, y) in enumerate(zip(got, want)):
                    if tuple(x) != y:
                        bad = f"frame {k} is {x}, expected {y}"
                        break
            yield check(f"resample {rate} Hz, {n} frames in calls of {chunk}: {len(want)} outputs, every sample", bad)
    # by hand: four frames at 22,050 Hz (half a frame a step); the left channel 0, -1, -2, -3 and the right 0, 100, 200, 300
    src = [(0, 0), (-1, 100), (-2, 200), (-3, 300)]
    hand_want = [(0, 0), (0, 0), (0, 0), (0, 50), (-1, 100), (-1, 150), (-2, 200), (-2, 250)]
    rc, buf = run_c(exe, ["resample", "22050", "3"], b"".join(struct.pack("<hh", *f) for f in src))
    got = [tuple(x) for x in struct.iter_unpack("<hh", buf)] if rc == 0 else None
    yield check("by hand, four frames at 22050 Hz: the division truncates toward zero", None if got == hand_want else f"status {rc}, {got}, written out by hand {hand_want}")
    yield check("the reference agrees with the hand-written resampler case", None if ref_resample(src, 22050) == hand_want else f"{ref_resample(src, 22050)}")


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--xa", type=Path, default=SRC / "xa.c")
    ap.add_argument("--cc", default=None)
    args = ap.parse_args()
    cc = args.cc or shutil.which("cc")
    if not cc:
        print("the host's C compiler `cc` is missing; these controls need it")
        return 2
    t0 = time.time()
    failed = count = 0
    with tempfile.TemporaryDirectory(prefix="hostxa-test-") as tmp:
        root = Path(tmp)
        (root / "xamain.c").write_text(C_MAIN)
        exe = root / "xatest"
        proc = subprocess.run([cc, "-O1", "-Wall", "-Wextra", "-Werror", "-I", str(SRC), "-o", str(exe), str(root / "xamain.c"), str(args.xa)], capture_output=True, text=True, timeout=180)
        if proc.returncode != 0:
            print("FAIL the test program does not build:\n" + proc.stderr[-1500:])
            return 1
        for name, bad in cases(exe, root):
            count += 1
            if bad is None:
                print(f"ok   {name}")
            else:
                print(f"FAIL {name}: {bad}")
                failed += 1
    print(f"{count} cases, {time.time() - t0:.1f} seconds")
    print(f"{failed} case(s) behaved wrongly" if failed else f"all {count} cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
