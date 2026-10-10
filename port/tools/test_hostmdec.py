#!/usr/bin/env python3
"""Controls for the core of the movie decoder, port/src/mdec.c, built alone with a C compiler.

    python3 test_hostmdec.py [--mdec FILE] [--cc CC]

What is tested: mdec.c alone (no other file of the runtime, no disc, no game, no window) on bitstreams and run-level codes that
this file invents. A small C program (below) takes commands in text on standard input and prints results in text. Its buffers
end exactly at a page that cannot be read or written, with bytes of a fixed pattern in front, so that a read past the end of an
input, or a write past the end of an output, ends the program with an access violation, and a write in front is seen; the pattern
of the untouched rest of every output is checked, too. Two controls show that the guards bite (`over`, `slack`).

NOTHING HERE IS COMPARED WITH A CONSOLE'S OUTPUT. There is none to compare with. The expected values come from three places:
cases worked out by hand in the comments below (exact), a reference written here in another form than the C (the mathematical
two-dimensional inverse transform in floating point; the colour conversion with the document's decimal constants; an encoder of
bitstreams with the document's table typed again here), and the structure of the data (what must be refused). Where the documents say
the arithmetic is not exact the comparison has a bound, and the bound is argued in the comment of the case, not found by running.

The specification is the public psx-spx document: the chapter "Macroblock Decoder (MDEC)" for the chip (run-level decoding, the
transform with the scale table, the colour conversion, the zigzag order), and the chapter "CDROM File Formats" for the frame's
bitstream (header, versions, table of codes, escape, end-of-block, end-of-frame, version 3 offsets). The tables below (zigzag, scale
table, the codes) are typed here from the documents separately from the C's copies.

--mdec FILE names another copy of mdec.c (the mutants of the controls run on a scratch copy). Every child runs under a time limit; a
child that dies is a failure of the case, except where the case expects the death.

Every case prints `ok NAME` or `FAIL NAME: why`; the last line is `all N cases behaved as required` or the number that did not.
"""

from __future__ import annotations

import argparse
import math
import random
import shutil
import subprocess
import sys
import tempfile
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
SRC = HERE.parent / "src"
LIMIT = 60   # seconds for any child

# result codes of mdec.h
OK, E_HEADER, E_VERSION, E_CODE, E_TRUNC, E_ROOM, E_INCOMPLETE, E_DEPTH, E_ARG, E_RUN, E_COMMAND = 0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10
EOB = 0xFE00

C_MAIN = r'''
#include "mdec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif

#define PAGE 4096u

/* A buffer that ends where an unreadable page begins; the bytes in front of it, up to the start of its pages, are a pattern. */
struct guarded { unsigned char *p, *base; size_t size, slack; };

static void gnew(struct guarded *g, size_t size, int fill)
{
    size_t pages = (size + PAGE - 1) / PAGE + 1;
#ifdef _WIN32
    DWORD old;
    g->base = VirtualAlloc(NULL, (pages + 1) * PAGE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!g->base || !VirtualProtect(g->base + pages * PAGE, PAGE, PAGE_NOACCESS, &old)) { puts("no memory"); exit(3); }
#else
    g->base = mmap(NULL, (pages + 1) * PAGE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (g->base == MAP_FAILED || mprotect(g->base + pages * PAGE, PAGE, PROT_NONE)) { puts("no memory"); exit(3); }
#endif
    g->size = size;
    g->p = g->base + pages * PAGE - size;
    g->slack = (size_t)(g->p - g->base);
    memset(g->base, 0xA5, g->slack);
    memset(g->p, fill, size);
}

static int slack_ok(const struct guarded *g)
{
    size_t i;
    for (i = 0; i < g->slack; i++) if (g->base[i] != 0xA5) return 0;
    return 1;
}

static void gfree(struct guarded *g)
{
#ifdef _WIN32
    VirtualFree(g->base, 0, MEM_RELEASE);
#else
    munmap(g->base, (g->slack + g->size + PAGE) );
#endif
}

/* the bytes from `from` on are still the fill */
static int rest_is(const struct guarded *g, size_t from, int fill)
{
    size_t i;
    for (i = from; i < g->size; i++) if (g->p[i] != (unsigned char)fill) return 0;
    return 1;
}

static int unhex(const char *s, unsigned char *out)
{
    int n = 0;
    if (s[0] == '-') return 0;
    while (s[0] && s[1] && s[0] != '\n') {
        unsigned v;
        char t[3] = { s[0], s[1], 0 };
        v = (unsigned)strtoul(t, NULL, 16);
        out[n++] = (unsigned char)v;
        s += 2;
    }
    return n;
}

static struct port_mdec m;

int main(void)
{
    char *line = malloc(1 << 24);
    unsigned char *tmp = malloc(1 << 23);
    port_mdec_reset(&m);
    while (fgets(line, 1 << 24, stdin)) {
        char cmd[16];
        int at = 0;
        if (sscanf(line, "%15s%n", cmd, &at) != 1) continue;
        if (!strcmp(cmd, "reset")) {
            port_mdec_reset(&m);
            puts("reset");
        } else if (!strcmp(cmd, "tables")) {
            int i;
            printf("tables ");
            for (i = 0; i < 64; i++) printf("%02x", m.iq_y[i]);
            printf(" ");
            for (i = 0; i < 64; i++) printf("%02x", m.iq_uv[i]);
            printf(" ");
            for (i = 0; i < 64; i++) printf("%02x%02x", (unsigned)m.scale[i] & 0xFF, ((unsigned)m.scale[i] >> 8) & 0xFF);
            printf("\n");
        } else if (!strcmp(cmd, "setq") || !strcmp(cmd, "sets")) {
            unsigned command, size;
            char hex[1 << 16];
            struct guarded g;
            int n, rc;
            sscanf(line + at, "%x %u %65535s", &command, &size, hex);
            n = unhex(hex, tmp);
            gnew(&g, size, 0);
            memcpy(g.p, tmp, (unsigned)n < size ? (unsigned)n : size);
            rc = cmd[3] == 'q' ? port_mdec_set_quant(&m, command, g.p, size) : port_mdec_set_scale(&m, command, g.p, size);
            printf("%s %d %d\n", cmd, rc, slack_ok(&g));
            gfree(&g);
        } else if (!strcmp(cmd, "vlc")) {
            unsigned room, used = 99999;
            char *hex = malloc(1 << 24);
            struct guarded fr, co;
            int n, rc, i, intact;
            unsigned char *copy;
            sscanf(line + at, "%u %s", &room, hex);
            n = unhex(hex, tmp);
            gnew(&fr, (size_t)n, 0);
            memcpy(fr.p, tmp, (size_t)n);
            copy = malloc((size_t)n + 1);
            memcpy(copy, tmp, (size_t)n);
            gnew(&co, (size_t)room * 2, 0xCC);
            rc = port_mdec_vlc(fr.p, (unsigned)n, (unsigned short *)co.p, room, &used);
            intact = slack_ok(&fr) && slack_ok(&co) && !memcmp(copy, fr.p, (size_t)n) && used <= room && rest_is(&co, (size_t)used * 2, 0xCC);
            printf("vlc %d %u %d ", rc, used, intact);
            for (i = 0; i < (int)used && used <= room; i++) printf("%04x", ((unsigned short *)co.p)[i]);
            printf("\n");
            gfree(&fr); gfree(&co); free(hex); free(copy);
        } else if (!strcmp(cmd, "dec")) {
            int depth, mask, sgn, i, rc, intact;
            unsigned room, blocks = 99999, written = 99999;
            char *hex = malloc(1 << 24);
            struct guarded co, px;
            int n;
            sscanf(line + at, "%d %d %d %u %s", &depth, &mask, &sgn, &room, hex);
            n = unhex(hex, tmp) / 2;
            gnew(&co, (size_t)n * 2, 0);
            memcpy(co.p, tmp, (size_t)n * 2);
            gnew(&px, room, 0xCC);
            rc = port_mdec_decode(&m, (const unsigned short *)co.p, (unsigned)n, depth, mask, sgn, px.p, room, &blocks, &written);
            intact = slack_ok(&co) && slack_ok(&px) && written <= room && rest_is(&px, written, 0xCC);
            printf("dec %d %u %u %d ", rc, blocks, written, intact);
            for (i = 0; i < (int)written && written <= room; i++) printf("%02x", px.p[i]);
            printf("\n");
            gfree(&co); gfree(&px); free(hex);
        } else if (!strcmp(cmd, "args")) {
            int k, rc = 12345;
            unsigned used = 7, blocks = 7, written = 7;
            unsigned short codes[4] = { 0 };
            unsigned char px[4] = { 0 }, frame[16] = { 0 };
            sscanf(line + at, "%d", &k);
            switch (k) {
            case 1: rc = port_mdec_vlc(frame, 8, codes, 4, NULL); break;
            case 2: rc = port_mdec_vlc(NULL, 4, codes, 4, &used); break;
            case 3: rc = port_mdec_vlc(NULL, 0, codes, 4, &used); break;
            case 4: rc = port_mdec_vlc(frame, 8, NULL, 3, &used); break;
            case 5: rc = port_mdec_decode(&m, codes, 0, 24, 0, 0, px, 4, NULL, &written); break;
            case 6: rc = port_mdec_decode(NULL, codes, 0, 24, 0, 0, px, 4, &blocks, &written); break;
            case 7: rc = port_mdec_decode(&m, NULL, 2, 24, 0, 0, px, 4, &blocks, &written); break;
            case 8: rc = port_mdec_decode(&m, codes, 0, 24, 0, 0, NULL, 4, &blocks, &written); break;
            case 9: rc = port_mdec_decode(&m, NULL, 0, 24, 0, 0, NULL, 0, &blocks, &written); break;
            case 10: rc = port_mdec_set_quant(&m, 0x40000000u, NULL, 64); break;
            }
            printf("args %d %d\n", k, rc);
        } else if (!strcmp(cmd, "over")) {
            struct guarded g;
            volatile unsigned char c;
            gnew(&g, 16, 0);
            c = g.p[16];   /* one past the end: must end the program */
            printf("survived %d\n", c);
        } else if (!strcmp(cmd, "slack")) {
            struct guarded g;
            gnew(&g, 16, 0);
            g.p[-1] = 0;   /* one in front: the pattern must show it */
            printf("slack %d\n", slack_ok(&g));
        }
        fflush(stdout);
    }
    return 0;
}
'''

# ---- the documents' tables, typed here again --------------------------------------------------------------------------------

# zigzag[0..63] of the chapter "Macroblock Decoder (MDEC)": the scan index of the natural position i (row-major)
ZIGZAG = [
    0, 1, 5, 6, 14, 15, 27, 28,
    2, 4, 7, 13, 16, 26, 29, 42,
    3, 8, 12, 17, 25, 30, 41, 43,
    9, 11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
    20, 22, 33, 38, 46, 51, 55, 60,
    21, 34, 37, 47, 50, 56, 59, 61,
    35, 36, 48, 49, 57, 58, 62, 63,
]
ZAGZIG = [ZIGZAG.index(k) for k in range(64)]   # the natural position of scan index k

# the scale table of MDEC(3), as the chapter gives it
SCALE_ROWS = """
5A82 5A82 5A82 5A82 5A82 5A82 5A82 5A82
7D8A 6A6D 471C 18F8 E707 B8E3 9592 8275
7641 30FB CF04 89BE 89BE CF04 30FB 7641
6A6D E707 8275 B8E3 471C 7D8A 18F8 9592
5A82 A57D A57D 5A82 5A82 A57D A57D 5A82
471C 8275 18F8 6A6D 9592 E707 7D8A B8E3
30FB 89BE 7641 CF04 CF04 7641 89BE 30FB
18F8 B8E3 6A6D 8275 7D8A 9592 471C E707
"""
SCALE = [int(w, 16) - 65536 if int(w, 16) >= 32768 else int(w, 16) for w in SCALE_ROWS.split()]

# the default intra quantiser matrix of ISO/IEC 11172-2 in natural order, with the first entry 2 (the task's choice, not the
# document's); the document says only that the library's table begins 02h 10h 10h 13h and ends 53h
MPEG_NATURAL = [
    2, 16, 19, 22, 26, 27, 29, 34,
    16, 16, 22, 24, 27, 29, 34, 37,
    19, 22, 26, 27, 29, 34, 34, 38,
    22, 22, 26, 27, 29, 34, 37, 40,
    22, 26, 27, 29, 32, 35, 40, 48,
    26, 27, 29, 32, 35, 40, 48, 58,
    26, 27, 29, 34, 38, 46, 56, 69,
    27, 29, 35, 38, 46, 56, 69, 83,
]
DEFAULT_QUANT = [MPEG_NATURAL[ZAGZIG[k]] for k in range(64)]   # in scan order

# "Huffman codes for AC values BS v1/v2/v3/iki" of the chapter "CDROM File Formats". x: index bits, s: the sign bit (1: negative).
AC_ROWS = [
    ("11s", [0x0001]),
    ("011s", [0x0401]),
    ("010xs", [0x0002, 0x0801]),
    ("0011xs", [0x1001, 0x0C01]),
    ("00101s", [0x0003]),
    ("00100xxxs", [0x3401, 0x0006, 0x3001, 0x2C01, 0x0C02, 0x0403, 0x0005, 0x2801]),
    ("0001xxs", [0x1C01, 0x1801, 0x0402, 0x1401]),
    ("00001xxs", [0x0802, 0x2401, 0x0004, 0x2001]),
    ("0000001xxxs", [0x4001, 0x1402, 0x0007, 0x0803, 0x0404, 0x3C01, 0x3801, 0x1002]),
    ("00000001xxxxs", [0x000B, 0x2002, 0x1003, 0x000A, 0x0804, 0x1C02, 0x5401, 0x5001,
                       0x0009, 0x4C01, 0x4801, 0x0405, 0x0C03, 0x0008, 0x1802, 0x4401]),
    ("000000001xxxxs", [0x2802, 0x2402, 0x1403, 0x0C04, 0x0805, 0x0407, 0x0406, 0x000F,
                        0x000E, 0x000D, 0x000C, 0x6801, 0x6401, 0x6001, 0x5C01, 0x5801]),
    ("0000000001xxxxs", [0x001F, 0x001E, 0x001D, 0x001C, 0x001B, 0x001A, 0x0019, 0x0018,
                         0x0017, 0x0016, 0x0015, 0x0014, 0x0013, 0x0012, 0x0011, 0x0010]),
    ("00000000001xxxxs", [0x0028, 0x0027, 0x0026, 0x0025, 0x0024, 0x0023, 0x0022, 0x0021,
                          0x0020, 0x040E, 0x040D, 0x040C, 0x040B, 0x040A, 0x0409, 0x0408]),
    ("000000000001xxxxs", [0x0412, 0x0411, 0x0410, 0x040F, 0x1803, 0x4002, 0x3C02, 0x3802,
                           0x3402, 0x3002, 0x2C02, 0x7C01, 0x7801, 0x7401, 0x7001, 0x6C01]),
]
EOB_BITS = "10"
ESCAPE_BITS = "000001"   # followed by the raw 16 bit value

# "DC v3": offsets of the DC value, in units of 4; "s": 0 negative, 1 positive; "n": index bits. Chroma = Cr and Cb.
DC3_CHROMA = [("00", None), ("01s", 0), ("10sn", 1), ("110snn", 2), ("1110snnn", 3), ("11110snnnn", 4),
              ("111110snnnnn", 5), ("1111110snnnnnn", 6), ("11111110snnnnnnn", 7)]
DC3_LUMA = [("100", None), ("00s", 0), ("01sn", 1), ("101snn", 2), ("110snnn", 3), ("1110snnnn", 4),
            ("11110snnnnn", 5), ("111110snnnnnn", 6), ("1111110snnnnnnn", 7)]
EOF_V3 = "1111111111"
EOF_V2 = "0111111111"


# ---- encoder of bitstreams (the reference for the reader) ---------------------------------------------------------------------

def run_level_value(run: int, level: int) -> int:
    return (run << 10) | (level & 0x3FF)


# (run, magnitude) -> (pattern, index) for every entry of the table
AC_LOOKUP = {}
for _pat, _vals in AC_ROWS:
    for _i, _v in enumerate(_vals):
        AC_LOOKUP[(_v >> 10, _v & 0x3FF)] = (_pat, _i)
AC_ENTRIES = [(_v >> 10, _v & 0x3FF) for _pat, _vals in AC_ROWS for _v in _vals]


def ac_bits(run: int, level: int, force_escape: bool = False) -> str:
    """A (run, level) pair as bits: the table's code when the pair is in the table, else the escape."""
    key = (run, abs(level))
    if key in AC_LOOKUP and level != 0 and not force_escape:
        pat, idx = AC_LOOKUP[key]
        nx = pat.count("x")
        bits = pat[: len(pat) - nx - 1]
        bits += (format(idx, "0%db" % nx) if nx else "") + ("1" if level < 0 else "0")
        return bits
    return ESCAPE_BITS + format(run_level_value(run, level), "016b")


def dc3_bits(off4: int, luma: bool) -> str:
    """An offset (a multiple of 4, in -1020..1020) as bits."""
    table = DC3_LUMA if luma else DC3_CHROMA
    if off4 == 0:
        return [p for p, n in table if n is None][0]
    mag = abs(off4) // 4
    nb = mag.bit_length() - 1
    lo, hi = 1 << nb, (2 << nb) - 1
    pat = [p for p, n in table if n == nb][0]
    positive = off4 > 0
    n = mag - lo if positive else hi - mag
    head = pat[: pat.index("s")]
    return head + ("1" if positive else "0") + (format(n, "0%db" % nb) if nb else "")


def wrap10(v: int) -> int:
    return ((v + 512) & 0x3FF) - 512


def frame_bytes(version: int, quant: int, bits: str, *, size_field: int = 1, ident: int = 0x3800) -> bytes:
    bits = bits + "0" * (-len(bits) % 16)
    out = bytearray()
    for h in (size_field, ident, quant, version):
        out += h.to_bytes(2, "little")
    for i in range(0, len(bits), 16):
        out += int(bits[i:i + 16], 2).to_bytes(2, "little")
    return bytes(out)


def encode_frame(version: int, quant: int, macroblocks, *, eof: bool = True):
    """macroblocks: a list of 6-block lists; a block is (dc, [(run, level), ...]) with dc the DC value for versions 1 and 2 (-512..510 in
    Cr, up to 511 elsewhere) and the TARGET DC (a multiple of 4) for version 3. Returns (bits, expected codes)."""
    bits, codes = "", []
    old = [0, 0, 0]
    for mb in macroblocks:
        for b, (dc, pairs) in enumerate(mb):
            if version == 3:
                which = 0 if b == 0 else 1 if b == 1 else 2
                off = dc - old[which]
                bits += dc3_bits(off, b >= 2)
                old[which] = wrap10(old[which] + off)
                dcfield = old[which] & 0x3FF
            else:
                bits += format(dc & 0x3FF, "010b")
                dcfield = dc & 0x3FF
            codes.append((quant << 10) | dcfield)
            for run, level in pairs:
                bits += ac_bits(run, level)
                codes.append(run_level_value(run, level))
            bits += EOB_BITS
            codes.append(EOB)
    if eof:
        bits += EOF_V3 if version == 3 else EOF_V2
    return bits, codes


def flat_mb(dcs=(0, 0, 0, 0, 0, 0)):
    return [(d, []) for d in dcs]


# ---- running the driver ----------------------------------------------------------------------------------------------------

BATCH = 100_000   # characters of commands per child: a Windows program started from WSL stalls on a much larger pipe


def run_driver(exe: Path, lines: list[str], prefix: tuple = ()):
    """Runs the driver on the commands (in batches of about BATCH characters, each started with the `prefix` commands, whose output is
    dropped). Returns (status of the first child that did not end with 0 or 0, the output lines of the commands)."""
    out, batch, size = [], [], 0
    batches = []
    for line in lines:
        if batch and size + len(line) > BATCH:
            batches.append(batch)
            batch, size = [], 0
        batch.append(line)
        size += len(line) + 1
    batches.append(batch)
    for batch in batches:
        try:
            p = subprocess.run([str(exe)], input=("\n".join(list(prefix) + batch) + "\n").encode(), capture_output=True, timeout=LIMIT)
        except subprocess.TimeoutExpired:
            return None, out
        out += p.stdout.decode().splitlines()[len(prefix):]
        if p.returncode != 0:
            return p.returncode, out
    return 0, out


def hexs(b: bytes) -> str:
    return b.hex() if b else "-"


def vlc(exe, frames, room=4096):
    """Runs port_mdec_vlc on each frame: a list of (rc, used, intact, codes)."""
    rc, out = run_driver(exe, [f"vlc {room if not isinstance(room, list) else room[i]} {hexs(f)}" for i, f in enumerate(frames)])
    if rc != 0 or len(out) != len(frames):
        raise RuntimeError(f"the driver ended with status {rc} after {len(out)} of {len(frames)} lines")
    res = []
    for line in out:
        t = line.split()
        codes = [int(t[4][i:i + 4], 16) for i in range(0, len(t[4]), 4)] if len(t) > 4 else []
        res.append((int(t[1]), int(t[2]), t[3] == "1", codes))
    return res


def dec(exe, jobs, setup=()):
    """jobs: tuples (codes, depth, mask, signed, room). Returns a list of (rc, blocks, written, intact, pixel bytes)."""
    lines = []
    for codes, depth, mask, sgn, room in jobs:
        lines.append(f"dec {depth} {int(mask)} {int(sgn)} {room} " + hexs(b"".join(c.to_bytes(2, "little") for c in codes)))
    rc, out = run_driver(exe, lines, tuple(setup))
    if rc != 0 or len(out) != len(jobs):
        raise RuntimeError(f"the driver ended with status {rc} after {len(out)} of {len(jobs)} lines")
    res = []
    for line in out:
        t = line.split()
        res.append((int(t[1]), int(t[2]), int(t[3]), t[4] == "1", bytes.fromhex(t[5]) if len(t) > 5 else b""))
    return res


# The case whose commands the driver is running. A decoder that reads or writes beyond a size ends the driver at a guard
# page, in the middle of a case: the case is then named here, so that the failure is a named one and not only a status.
IN_WORK = [None]


def about(name: str) -> str:
    IN_WORK[0] = name
    return name


def check(name, bad):
    IN_WORK[0] = None
    return name, bad


def expect(name, got, want):
    return check(name, None if got == want else f"got {got!r}, expected {want!r}")


# ---- the reference of the chip's arithmetic -----------------------------------------------------------------------------------

def codes_for_block(qs, dc, pairs):
    return [(qs << 10) | (dc & 0x3FF)] + [run_level_value(r, l) for r, l in pairs] + [EOB]


def coefficients(qs, dc, pairs, qt):
    """The block's dequantised coefficients F (natural order) from the levels, by the chapter's rule: the first value is dc * qt[0]
    (qs > 0) or dc * 2 (qs = 0), a later value is (level * qt[k] * qs + 4) / 8 rounded down (qs > 0) or level * 2 (qs = 0, stored
    WITHOUT the zigzag), all saturated to -1024..1023."""
    F = [0] * 64
    sat = lambda v: max(-1024, min(1023, v))
    F[0] = sat(dc * (qt[0] if qs else 2))
    k = 0
    for run, level in pairs:
        k += run + 1
        if k > 63:
            break
        val = (level * qt[k] * qs + 4) // 8 if qs else level * 2
        F[ZAGZIG[k] if qs else k] = sat(val)
    return F


def idct_float(F):
    """The mathematical orthonormal 2-D inverse DCT of the natural-order block F (row = vertical frequency, column = horizontal)."""
    c = lambda u: 1 / math.sqrt(2) if u == 0 else 1.0
    cs = [[math.cos((2 * x + 1) * u * math.pi / 16) for u in range(8)] for x in range(8)]
    out = [0.0] * 64
    for y in range(8):
        for x in range(8):
            s = 0.0
            for v in range(8):
                for u in range(8):
                    f = F[v * 8 + u]
                    if f:
                        s += c(u) * c(v) * f * cs[x][u] * cs[y][v]
            out[y * 8 + x] = s / 4
    return out


def rnd_round(v):
    return math.floor(v + 0.5)


def dc_only_value(F0):
    """The document's integer transform on a block with only the first value, worked out for this case (see the comments of the
    cases): both passes reduce to  v = (v * (0x5A82 / 8) + 0xFFF) / 0x2000  with the division rounding down."""
    w = 0x5A82 >> 3
    a = (F0 * w + 0xFFF) >> 13
    return (a * w + 0xFFF) >> 13


def pixel_dec(y, cr, cb, signed=False):
    """The decimal formula of the chapter, rounded to nearest (halves up), saturated, offset."""
    r = max(-128, min(127, rnd_round(y + 1.402 * cr)))
    g = max(-128, min(127, rnd_round(y + (-0.3437 * cb) + (-0.7143 * cr))))
    b = max(-128, min(127, rnd_round(y + 1.772 * cb)))
    return tuple(v if signed else v + 128 for v in (r, g, b))


def mb_codes(cr=(0, ()), cb=(0, ()), ys=((0, ()),) * 4, qs=1):
    """One macroblock's codes from (dc, pairs) per block, all with the quantisation scale qs."""
    out = codes_for_block(qs, *cr) + codes_for_block(qs, *cb)
    for y in ys:
        out += codes_for_block(qs, *y)
    return out


def px24(data, mb, x, y):
    o = mb * 768 + (x + y * 16) * 3
    return tuple(data[o:o + 3])


def px15(data, mb, x, y):
    o = mb * 512 + (x + y * 16) * 2
    return data[o] | (data[o + 1] << 8)


# ---- the cases ----------------------------------------------------------------------------------------------------------------

def cases(exe: Path, tmp: Path):
    rnd = random.Random(20261010)

    # ---- the guards bite (negative controls for every "never reads or writes beyond" case below) ----
    rc, out = run_driver(exe, ["over"])
    yield check("guard: a read one past the end of a buffer ends the program", None if rc not in (0, None) and not out else f"status {rc}, output {out}")
    rc, out = run_driver(exe, ["slack"])
    yield check("guard: a write in front of a buffer is seen", None if rc == 0 and out == ["slack 0"] else f"status {rc}, output {out}")

    # ---- the tables at reset and the setters ----
    rc, out = run_driver(exe, ["tables"])
    t = out[0].split()
    want_scale = b"".join((v & 0xFFFF).to_bytes(2, "little") for v in SCALE).hex()
    yield expect("reset: both quantisation tables are the default matrix in scan order (first four 2, 16, 16, 19; last 83)", (t[1], t[2]), (bytes(DEFAULT_QUANT).hex(),) * 2)
    yield expect("reset: the scale table is the document's, signed halfwords", t[3], want_scale)
    yield check("the default matrix starts 02 10 10 13 and ends 53 (the only values the document gives)", None if DEFAULT_QUANT[:4] == [0x02, 0x10, 0x10, 0x13] and DEFAULT_QUANT[63] == 0x53 else str(DEFAULT_QUANT[:4]))
    qy, qc = bytes(range(1, 65)), bytes(range(101, 165))
    sc = b"".join(((i * 7 - 200) & 0xFFFF).to_bytes(2, "little") for i in range(64))
    lines = ["reset", f"setq 40000000 64 {qy.hex()}", "tables", f"setq 40000001 128 {(qy + qc).hex()}", "tables", f"sets 60000000 128 {sc.hex()}", "tables"]
    rc, out = run_driver(exe, lines)
    r1, r2, r3 = out[2].split(), out[4].split(), out[6].split()
    yield expect("set_quant, command bit 0 clear: only the luminance table changes", (r1[1], r1[2]), (qy.hex(), bytes(DEFAULT_QUANT).hex()))
    yield expect("set_quant, command bit 0 set: both tables change, 64 bytes each", (r2[1], r2[2]), (qy.hex(), qc.hex()))
    yield expect("set_scale: 64 signed little-endian halfwords", r3[3], sc.hex())
    bad_lines = ["reset", f"setq 60000000 64 {qy.hex()}", f"setq 40000000 63 {qy[:63].hex()}", f"setq 40000001 127 {(qy + qc)[:127].hex()}",
                 f"sets 40000000 128 {sc.hex()}", f"sets 60000000 127 {sc[:127].hex()}", "tables"]
    short = about("set_quant refuses a command that is not 2, 64 bytes short, 128 short; set_scale a wrong command and 127 bytes: each E_COMMAND, tables unchanged")
    rc, out = run_driver(exe, bad_lines)
    if rc != 0 or len(out) != len(bad_lines):
        # a setter that does not look at the size reads past the short table and ends the driver at the guard page
        yield check(short, f"the driver ended with status {rc} after {len(out)} of {len(bad_lines)} lines (a read beyond a size ends it at the guard)")
    else:
        yield expect(short, ([int(l.split()[1]) for l in out[1:6]], out[6].split()[1:]),
                     ([E_COMMAND] * 5, [bytes(DEFAULT_QUANT).hex(), bytes(DEFAULT_QUANT).hex(), want_scale]))
    rc, out = run_driver(exe, ["reset", f"setq 40000000 64 {qy.hex()}", f"setq 40000001 128 {(qy + qc).hex()}", f"sets 60000000 128 {sc.hex()}", f"setq 40000001 129 {(qy + qc).hex()}00"])
    yield expect("controls of the refusals: the exact sizes (64, 128, 128) and a larger size are accepted", [int(l.split()[1]) for l in out[1:]], [0, 0, 0, 0])
    rc, out = run_driver(exe, [f"args {k}" for k in range(1, 11)])
    yield expect("null pointers: every one that matters is E_ARG; a null with size 0 is not", [int(l.split()[2]) for l in out], [E_ARG] * 2 + [E_HEADER] + [E_ARG] + [E_ARG] * 4 + [OK] + [E_COMMAND])

    # ---- the bitstream: the header ----
    one_eof = frame_bytes(2, 3, EOF_V2)
    yield expect("by hand: version 2, quantisation 3, nothing but the end-of-frame code 0111111111 (bytes 01 00 00 38 03 00 02 00 C0 7F) gives no codes",
                 (one_eof.hex(), vlc(exe, [bytes.fromhex("0100003803000200C07F")])[0][:2]), (bytes.fromhex("0100003803000200C07F").hex(), (OK, 0)))
    good = bytes.fromhex("0100003803000200C07F")
    def with_(i, v, n=2):
        b = bytearray(good)
        b[i:i + n] = v.to_bytes(n, "little")
        return bytes(b)
    cases_h = [
        ("header: 7 bytes is not a frame", good[:7], E_HEADER),
        ("header: 0 bytes", b"", E_HEADER),
        ("header: file id 3801h", with_(2, 0x3801), E_HEADER),
        ("header: file id 3700h", with_(2, 0x3700), E_HEADER),
        ("header: quantisation 40h", with_(4, 0x40), E_HEADER),
        ("header: quantisation 8001h (the variant of single games)", with_(4, 0x8001), E_HEADER),
        ("version 0 is refused", with_(6, 0), E_VERSION),
        ("version 4 is refused", with_(6, 4), E_VERSION),
        ("version 8002h is refused", with_(6, 0x8002), E_VERSION),
    ]
    got = vlc(exe, [c[1] for c in cases_h])
    for (name, _, want), g in zip(cases_h, got):
        yield expect(name, (g[0], g[1], g[2]), (want, 0, True))
    controls = [with_(4, 0x3F), with_(6, 1), with_(0, 0x7FFF), good + b"\x00"]
    got = vlc(exe, controls)
    yield expect("controls of the header refusals: quantisation 3Fh, version 1, any size field, an odd trailing byte are accepted",
                 [(g[0], g[1]) for g in got], [(OK, 0)] * 4)
    v3_eof = frame_bytes(3, 0, EOF_V3)
    yield expect("version 3 ends at ten 1-bits, version 2 at 0111111111: each end code in the other version is not the end",
                 [vlc(exe, [v3_eof])[0][0], vlc(exe, [frame_bytes(2, 0, EOF_V3)])[0][0], vlc(exe, [frame_bytes(3, 0, EOF_V2)])[0][0]],
                 [OK, E_TRUNC, E_TRUNC])

    # ---- the bitstream: hand-worked frames ----
    # Version 2, quantisation 5, one macroblock; Cr has DC 4 (0000000100) and no AC (end of block 10); Cb DC -1 (1111111111) and the
    # pair (run 0, level 1) = code 11s with s=0 -> 110, then end of block; Y1..Y4 DC 0 (0000000000) with end of block, Y2 with the pair
    # (run 2, level -1): (run 2, level 1) is the value 0801h, row 010xs index 1 -> 010 1 s, s=1 -> 01011; end code 0111111111 (Cr
    # position, DC 1FFh).
    bits = ("0000000100" + "10" + "1111111111" + "110" + "10" + "0000000000" + "10" + "0000000000" + "01011" + "10"
            + "0000000000" + "10" + "0000000000" + "10" + "0111111111")
    want = [(5 << 10) | 4, EOB, (5 << 10) | 0x3FF, 0x0001, EOB, 5 << 10, EOB, 5 << 10, run_level_value(2, -1), EOB, 5 << 10, EOB, 5 << 10, EOB]
    g = vlc(exe, [frame_bytes(2, 5, bits)])[0]
    yield expect("by hand, version 2: DC 4; DC -1 and the pair (0, +1); a pair (2, -1) written 01011 (010, index 1, sign 1); the codes in the order Cr Cb Y1 Y2 Y3 Y4",
                 (g[0], g[1], g[3]), (OK, len(want), want))
    # Version 3, quantisation 2, one macroblock: Cr offset +4 = 01s with s=1: 011; Cb offset 0 = 00; Y1 offset 0 = 100; Y2 offset -4 = 00s
    # with s=0: 000; Y3 offset 0 = 100; Y4 offset +4 = 001; every block then end of block 10; the end code is ten 1-bits.
    bits3 = "011" + "10" + "00" + "10" + "100" + "10" + "000" + "10" + "100" + "10" + "001" + "10" + "1111111111"
    want3 = [(2 << 10) | 4, EOB, 2 << 10, EOB, 2 << 10, EOB, (2 << 10) | 0x3FC, EOB, (2 << 10) | 0x3FC, EOB, 2 << 10, EOB]
    g = vlc(exe, [frame_bytes(3, 2, bits3)])[0]
    yield expect("by hand, version 3: the running DC per component (Cr +4; Cb 0; Y -4 then unchanged then +4 back to 0)", (g[0], g[3]), (OK, want3))
    # The escape: 000001 and the 16 bits as they are. (run 40, level 300) is not in the table.
    esc = "000001" + format(run_level_value(40, 300), "016b")
    bits_e = "0000000000" + esc + "10" + "".join("0000000000" + "10" for _ in range(5)) + "0111111111"
    g = vlc(exe, [frame_bytes(1, 0, bits_e)])[0]
    yield expect("by hand: the escape code 000001 + 16 raw bits for (run 40, level 300) gives the value A12Ch", (g[0], g[3][:3]), (OK, [0, 0xA12C, EOB]))
    yield expect("the escape of version 1's compressor (level 0, run 5) is passed on as the value 1400h", (vlc(exe, [frame_bytes(1, 0, "0000000000" + "000001" + format(5 << 10, "016b") + "10" + "".join("0000000000" + "10" for _ in range(5)) + "0111111111")])[0][3][1],), (0x1400,))

    # ---- the bitstream: the round trip, both versions ----
    # every entry of the table, both signs: one block each, six per macroblock
    rich_frames = []
    entries = [(r, l * sg) for (r, l) in AC_ENTRIES for sg in (1, -1)]
    assert len(AC_ENTRIES) == sum(len(v) for _, v in AC_ROWS)
    for version in (1, 2, 3):
        mbs = []
        for i in range(0, len(entries), 6):
            chunk = entries[i:i + 6]
            chunk += [(0, 1)] * (6 - len(chunk))
            mb = []
            for b, (r, l) in enumerate(chunk):
                dc = (i * 8 + b * 4) % 400 - 200
                mb.append((dc - dc % 4 if version == 3 else dc, [(r, l)]))
            mbs.append(mb)
        bits, want = encode_frame(version, 7, mbs)
        rich_frames.append((frame_bytes(version, 7, bits), len(bits)))
        g = vlc(exe, [frame_bytes(version, 7, bits)], room=len(want))[0]
        yield expect(f"round trip, version {version}: every code of the table with both signs ({len(entries)} blocks)", (g[0], g[1], g[2], g[3] == want), (OK, len(want), True, True))
    # a block of many pairs, positive and negative, ending exactly at the last coefficient (no room for another)
    pairs = []
    k = 0
    while True:
        run = rnd.choice([0, 0, 1, 2, 5, 31])
        if k + run + 1 > 63:
            break
        pairs.append((run, rnd.choice([-1, 1]) * rnd.choice([1, 2, 7, 18, 40, 511])))
        k += run + 1
    pairs.append((62 - k, 3))   # reaches coefficient 63
    for version in (2, 3):
        bits, want = encode_frame(version, 1, [[(4 if version == 3 else 3, pairs)] * 6])
        g = vlc(exe, [frame_bytes(version, 1, bits)])[0]
        yield expect(f"round trip, version {version}: a block whose last pair reaches the 64th coefficient, followed by its end-of-block code", (g[0], g[3] == want), (OK, True))
    # the escape for pairs the table does not have, and for level 0 (version 1's compressor), levels -512 and 511
    odd = [(0, 100), (0, -200), (3, 511), (3, -512), (31, 2), (20, 0), (0, 0), (40, -7), (1, 19)]
    for version in (1, 2, 3):
        bits, want = encode_frame(version, 9, [[(8, [p]) for p in odd[:6]], [(8, [p]) for p in odd[3:9]]])
        g = vlc(exe, [frame_bytes(version, 9, bits)])[0]
        yield expect(f"round trip, version {version}: pairs outside the table go through the escape (level 0, -512, 511)", (g[0], g[3] == want), (OK, True))
    # version 3 offsets: all sizes, both signs, in Y and in Cr/Cb, with the wrap at 10 bits
    offs = []
    for nb in range(8):
        lo, hi = 1 << nb, (2 << nb) - 1
        for mag in sorted({lo, hi, (lo + hi) // 2}):
            offs += [4 * mag, -4 * mag]
    offs.append(0)
    # the offsets are handed in exactly (not derived from targets, so that the wrap is used): block b of the stream takes offs[b % len]
    bits, want, old = "", [], [0, 0, 0]
    for n in range(6 * (len(offs) // 3 + 1)):
        which = 0 if n % 6 == 0 else 1 if n % 6 == 1 else 2
        off = offs[n % len(offs)]
        bits += dc3_bits(off, which == 2) + EOB_BITS
        old[which] = wrap10(old[which] + off)
        want += [(5 << 10) | (old[which] & 0x3FF), EOB]
    bits += EOF_V3
    g = vlc(exe, [frame_bytes(3, 5, bits)], room=len(want))[0]
    yield expect("round trip, version 3: every size of offset, both signs, for Cr/Cb and for Y, with the wrap of the running DC", (g[0], g[3] == want), (OK, True))
    # 508 + 4 wraps to -512: the field 200h
    bits = dc3_bits(508, False) + EOB_BITS + dc3_bits(0, False) + EOB_BITS + 4 * (dc3_bits(0, True) + EOB_BITS) + dc3_bits(4, False) + EOB_BITS
    bits += dc3_bits(0, False) + EOB_BITS + 4 * (dc3_bits(0, True) + EOB_BITS) + EOF_V3
    g = vlc(exe, [frame_bytes(3, 0, bits)])[0]
    yield expect("version 3: the running DC wraps within 10 bits (508 + 4 gives the field 200h, -512)", (g[0], g[3][0], g[3][12]), (OK, 508, 0x200))
    # version 2: the DC range, and +1FFh in a block that is not Cr is a DC like another
    mbs = [[(-512, []), (511, []), (511, []), (0, []), (-1, []), (255, [])]]
    bits, want = encode_frame(2, 63, mbs)
    g = vlc(exe, [frame_bytes(2, 63, bits)])[0]
    yield expect("round trip, version 2: DC -512 and 510 in Cr; 1FFh as the DC of Cb and Y is a DC", (g[0], g[3] == want), (OK, True))
    # many macroblocks, and the used count
    mbs = [[(rnd.randrange(-300, 300), [(rnd.randrange(0, 5), rnd.choice([-1, 1]) * rnd.randrange(1, 60))]) for _ in range(6)] for _ in range(50)]
    for version in (2, 3):
        mm = [[((d - d % 4) if version == 3 else d, p) for d, p in mb] for mb in mbs]
        bits, want = encode_frame(version, 12, mm)
        g = vlc(exe, [frame_bytes(version, 12, bits)])[0]
        yield expect(f"round trip, version {version}: 50 macroblocks with random pairs; *used is the number of codes", (g[0], g[1], g[3] == want), (OK, len(want), True))

    # ---- the bitstream: every refusal, with a negative control ----
    def refuse(version, bits):
        return vlc(exe, [frame_bytes(version, 4, bits)])[0]
    blk = lambda dc="0000000000", tail="10": dc + tail
    rest5 = "".join(blk() for _ in range(5))
    g = refuse(2, "0000000000" + "000000000000" + "1" * 20 + rest5 + EOF_V2)
    c = refuse(2, "0000000000" + "10" + rest5 + EOF_V2)
    yield expect("E_CODE: the unused code of twelve 0-bits after a DC (control: end of block there is accepted)", (g[0], c[0]), (E_CODE, OK))
    g = refuse(2, "0000000000" + "000001" + format(EOB, "016b") + rest5 + EOF_V2)
    c = refuse(2, "0000000000" + "000001" + format(run_level_value(3, 5), "016b") + "10" + rest5 + EOF_V2)
    yield expect("E_CODE: an escape whose raw value is FE00h (control: the value 0C05h is accepted)", (g[0], c[0]), (E_CODE, OK))
    g = refuse(3, "00" + "10" + "111111110" + "0" * 20 + EOF_V3)
    c = refuse(3, "00" + "10" + "11111110" + "0" + "0000000" + "10" + "".join(blk("100", "10") for _ in range(4)) + EOF_V3)
    yield expect("E_CODE: version 3, the unused chroma code 111111110 (control: 11111110s + 7 bits, the longest offset, is accepted)", (g[0], c[0]), (E_CODE, OK))
    g = refuse(3, "00" + "10" + "1111111110" + "0" * 20 + EOF_V3)
    yield expect("E_CODE: version 3, the unused chroma code 1111111110", (g[0],), (E_CODE,))
    g = refuse(3, "00" + "10" + "00" + "10" + "11111110" + "0" * 20 + EOF_V3)
    c = refuse(3, "00" + "10" + "00" + "10" + "1111110" + "0" + "0000000" + "10" + "".join(blk("100", "10") for _ in range(3)) + EOF_V3)
    yield expect("E_CODE: version 3, the unused luma code 11111110 (control: 1111110s + 7 bits is accepted)", (g[0], c[0]), (E_CODE, OK))
    g = refuse(3, "00" + "10" + EOF_V3 + "0" * 20)
    c = refuse(3, "00" + "10" + "00" + "10" + "".join(blk("100", "10") for _ in range(4)) + EOF_V3)
    yield expect("E_CODE: version 3, ten 1-bits where a Cb block begins are not the end of the frame (control: the same bits at a Cr position end it)", (g[0], c[0]), (E_CODE, OK))
    esc63 = "000001" + format(run_level_value(63, 1), "016b")
    esc62 = "000001" + format(run_level_value(62, 1), "016b")
    g = refuse(2, "0000000000" + esc63 + "10" + rest5 + EOF_V2)
    c = refuse(2, "0000000000" + esc62 + "10" + rest5 + EOF_V2)
    yield expect("E_RUN: a run of 63 from the DC goes past the 64th coefficient (control: 62 reaches it and is accepted)", (g[0], c[0]), (E_RUN, OK))
    g = refuse(2, "0000000000" + "11" + "0" + "000001" + format(run_level_value(62, 1), "016b") + "10" + rest5 + EOF_V2)
    yield expect("E_RUN: a pair after one that already used a run of 0 and a second of 62", (g[0],), (E_RUN,))
    # E_TRUNC: no end-of-frame code, nothing at all after the header, a cut inside a block
    full_bits, full_codes = encode_frame(2, 4, [flat_mb((1, 2, 3, 4, 5, 6)), flat_mb((-1, -2, -3, -4, -5, -6))])
    full = frame_bytes(2, 4, full_bits)
    g = vlc(exe, [frame_bytes(2, 4, full_bits[:-10])])[0]
    yield expect("E_TRUNC: the frame has no end-of-frame code (control: with it, accepted)", (g[0], vlc(exe, [full])[0][0]), (E_TRUNC, OK))
    yield expect("E_TRUNC: eight bytes, the header alone, are a frame without its end code", (vlc(exe, [full[:8]])[0][0],), (E_TRUNC,))
    needed = 8 + 2 * ((len(full_bits) + 15) // 16)
    got = vlc(exe, [full[:n] for n in range(8, len(full) + 1)])
    bad = [n for n, g in zip(range(8, len(full) + 1), got) if g[0] != (OK if n >= needed else E_TRUNC) or not g[2]]
    yield check("E_TRUNC: the frame cut at every length from 8 bytes up: refused as truncated until the end code is whole, then accepted; guards intact (negative controls: the last lengths)", None if not bad else f"lengths {bad[:6]}")
    # E_ROOM
    want_n = len(full_codes)
    got = vlc(exe, [full] * (want_n + 2), room=list(range(0, want_n + 2)))
    bad = [n for n, g in enumerate(got) if (g[0], g[1]) != ((OK if n >= want_n else E_ROOM), (want_n if n >= want_n else n)) or not g[2]]
    yield check("E_ROOM: every room from 0 up: refused with *used = room until the room holds all the codes, then accepted with *used = their number; nothing written past the room", None if not bad else f"rooms {bad[:6]}")

    # E_TRUNC for cuts inside a code: the frames of every code of the table (versions 1 to 3), and sixteen frames whose escape code starts at
    # each of the sixteen bit positions of a 16 bit unit, each cut at every length. A cut that leaves the end code whole is accepted.
    shift_frames = []
    for j in range(16):
        pairs = [(0, 1)] * j + [(40, 300), (0, -2)]
        bits, _ = encode_frame(2, 3, [[(5, pairs)] + [(0, [])] * 5])
        shift_frames.append((frame_bytes(2, 3, bits), len(bits)))
    for label, frames_ in (("every code of the table, versions 1 to 3", rich_frames), ("an escape code at each of the 16 bit positions", shift_frames)):
        bad, n_cuts = [], 0
        for fb, nbits in frames_:
            need = 8 + 2 * ((nbits + 15) // 16)
            cuts = list(range(8, len(fb) + 1))
            got = vlc(exe, [fb[:n] for n in cuts])
            n_cuts += len(cuts)
            bad += [n for n, g in zip(cuts, got) if g[0] != (OK if n >= need else E_TRUNC) or not g[2]]
        yield check(f"E_TRUNC: {label}, cut at every length ({n_cuts} cuts): refused as truncated until the end code is whole, then accepted; guards intact", None if not bad else f"lengths {bad[:6]}")

    # ---- the bitstream: corrupt and random input never leaves its buffers ----
    frames, rooms = [], []
    for _ in range(400):
        v = rnd.choice([1, 2, 3, 2])
        body = bytes(rnd.randrange(256) for _ in range(rnd.randrange(0, 70)))
        frames.append(frame_bytes(v, rnd.randrange(64), "") [:8] + body)
        rooms.append(rnd.choice([0, 1, 5, 20, 100]))
    good_bits, good_codes = encode_frame(3, 9, [[(d - d % 4, p) for d, p in mb] for mb in mbs[:10]])
    gb = frame_bytes(3, 9, good_bits)
    for _ in range(400):
        b = bytearray(gb)
        for _ in range(rnd.randrange(1, 5)):
            b[8 + rnd.randrange(len(b) - 8)] ^= 1 << rnd.randrange(8)
        b = bytes(b[: rnd.randrange(8, len(b) + 1)])
        frames.append(b)
        rooms.append(rnd.choice([0, 3, 50, 4096]))
    got = vlc(exe, frames, room=rooms)
    bad = [i for i, g in enumerate(got) if not g[2] or g[0] > 0 or g[0] < E_COMMAND]
    yield check("corrupt input: 800 random and bit-flipped frames, with small rooms, never read or write out of bounds and always give a defined result", None if not bad else f"frames {bad[:5]}")

    # ---- the decoder: hand-worked blocks (see the comments) ----
    # The transform of the chapter for a block with only the first value F: pass 1 gives, at row 0, a = (F * 2896 + 4095) >> 13 where
    # 2896 = 0x5A82 / 8 rounded down; pass 2 gives every pixel b = (a * 2896 + 4095) >> 13.
    #   code (qs=1, dc=16): F = 16 * 2 = 32 (the first quantisation entry at reset is 2): a = (92672 + 4095) >> 13 = 96767 >> 13 = 11;
    #   b = (31856 + 4095) >> 13 = 35951 >> 13 = 4.  So Y = 4 and with Cr = Cb = 0 every pixel is (4+128) = 132.
    #   dc=-16: F = -32: a = (-92672 + 4095) >> 13 = -88577 >> 13 = -11 (rounds down: -10.8); b = (-31856 + 4095) >> 13 = -27761 >> 13 = -4.
    #   dc=32: F = 64: a = (185344 + 4095) >> 13 = 23; b = (66608 + 4095) >> 13 = 8.   dc=0: 0.
    def flat(cr=0, cb=0, y=(0, 0, 0, 0), qs=1):
        return mb_codes((cr, ()), (cb, ()), tuple((v, ()) for v in y), qs)
    r = dec(exe, [(flat(y=(16, 16, 16, 16)), 24, 0, 0, 768)])[0]
    yield expect("by hand: DC 16 in all four luminance blocks, no colour: 768 bytes, every pixel (132, 132, 132)", (r[0], r[1], r[2], r[3], set(r[4][i:i + 3] for i in range(0, 768, 3))), (OK, 1, 768, True, {bytes([132] * 3)}))
    r = dec(exe, [(flat(y=(-16, -16, -16, -16)), 24, 0, 0, 768)])[0]
    yield expect("by hand: DC -16 gives Y = -4 (the shift rounds down: -10.8 -> -11 -> -4), every pixel 124", set(r[4][i:i + 3] for i in range(0, 768, 3)), {bytes([124] * 3)})
    r = dec(exe, [(flat(y=(16, -16, 32, 0)), 24, 0, 0, 768)])[0]
    quad = lambda x, y: px24(r[4], 0, x, y)
    ok = all(quad(x, y) == ((132,) * 3, (124,) * 3, (136,) * 3, (128,) * 3)[(y // 8) * 2 + x // 8] for x in range(16) for y in range(16))
    yield check("by hand: the four luminance blocks are Y1 upper left (132), Y2 upper right (124), Y3 lower left (136), Y4 lower right (128)", None if ok else str([quad(0, 0), quad(8, 0), quad(0, 8), quad(8, 8)]))
    # Cr = 16: dc 64 -> F = 128: a = (370688 + 4095) >> 13 = 45; b = (130320 + 4095) >> 13 = 16.  Y = 0.
    #   R = (91881 * 16 + 32768) >> 16 = 1502864 >> 16 = 22 (1.402 * 16 = 22.4);  G = (-46812 * 16 + 32768) >> 16 = -716224 >> 16 = -11
    #   (-0.7143 * 16 = -11.4);  B = 0.   Unsigned: (150, 117, 128).
    r = dec(exe, [(flat(cr=64), 24, 0, 0, 768)])[0]
    yield expect("by hand: Cr = 16 alone gives (150, 117, 128) in every pixel (Cr is the first block of the six)", set(r[4][i:i + 3] for i in range(0, 768, 3)), {bytes([150, 117, 128])})
    # Cb = 16: R = 0; G = (-22525 * 16 + 32768) >> 16 = -327632 >> 16 = -5 (-0.3437 * 16 = -5.5); B = (116131 * 16 + 32768) >> 16 = 1890864 >> 16 = 28 (28.35).
    r = dec(exe, [(flat(cb=64), 24, 0, 0, 768)])[0]
    yield expect("by hand: Cb = 16 alone gives (128, 123, 156) in every pixel (Cb is the second block)", set(r[4][i:i + 3] for i in range(0, 768, 3)), {bytes([128, 123, 156])})
    # signed output: the same pixels without the 128 added, as bytes: Cr = 16 gives (22, -11, 0) = 16 F5 00
    r = dec(exe, [(flat(cr=64), 24, 0, 1, 768)])[0]
    yield expect("by hand: signed output leaves (22, -11, 0) = 16 F5 00", set(r[4][i:i + 3] for i in range(0, 768, 3)), {bytes([0x16, 0xF5, 0x00])})
    # 15 bits: red 150 >> 3 = 18, green 117 >> 3 = 14, blue 128 >> 3 = 16: 18 + (14 << 5) + (16 << 10) = 41D2h; with the mask bit C1D2h.
    r0 = dec(exe, [(flat(cr=64), 15, 0, 0, 512)])[0]
    r1 = dec(exe, [(flat(cr=64), 15, 1, 0, 512)])[0]
    yield expect("by hand: 15 bit output of that pixel is 41D2h, with the mask bit C1D2h, 2 bytes a pixel, 512 bytes a macroblock",
                 (r0[2], {px15(r0[4], 0, x, y) for x in range(16) for y in range(16)}, {px15(r1[4], 0, x, y) for x in range(16) for y in range(16)}), (512, {0x41D2}, {0xC1D2}))
    r = dec(exe, [(flat(cr=64), 15, 0, 1, 512)])[0]
    yield expect("by hand: 15 bit signed output of (22, -11, 0): (16 >> 3) + ((0xF5 >> 3) << 5) = 2 + 30 * 32 = 03C2h", {px15(r[4], 0, x, y) for x in range(16) for y in range(16)}, {0x03C2})
    # saturation of the first value: with the luminance table entry 255 and dc 511, 511 * 255 = 130305 is saturated to 3FFh = 1023 (and
    # -512 * 255 to -400h = -1024): a = (1023 * 2896 + 4095) >> 13 = 362, b = (362 * 2896 + 4095) >> 13 = 128 -> saturated to 127 -> 255;
    # for -1024: a = (-1024 * 2896 + 4095) >> 13 = -362 (-361.5 rounds down), b = (-362 * 2896 + 4095) >> 13 = -128 -> 0 (not saturated).
    q255 = bytes([255] * 64)
    setup = ["reset", f"setq 40000000 64 {q255.hex()}"]
    r = dec(exe, [(flat(y=(511, -512, 511, -512)), 24, 0, 0, 768)], setup)[0]
    yield expect("by hand: the first value saturates at both ends (511 * 255 -> 3FFh, -512 * 255 -> -400h); Y = 128 is saturated to 127 (pixel 255), Y = -128 gives 0",
                 (px24(r[4], 0, 0, 0), px24(r[4], 0, 8, 0), px24(r[4], 0, 0, 8), px24(r[4], 0, 8, 8)), ((255,) * 3, (0,) * 3, (255,) * 3, (0,) * 3))
    # saturation of the colour: Y = 4 and Cr = 16 give R = 26 (no saturation); a big Cr: dc 511 * 2 = 1022 -> F = 1022, Cr = 128 (saturated
    # sum) -> R = 0 + 179 -> 127 (pixel 255), G = (-46812 * 128 + 32768) >> 16 = -91 (pixel 37), B = 128 - 128 = 0 -> pixel 128 (chroma only)
    #   Cr: a = (1022 * 2896 + 4095) >> 13 = 361, b = (361 * 2896 + 4095) >> 13 = 128 (1045456 + 4095 = 1049551 >> 13 = 128.1).
    r = dec(exe, [(flat(cr=511), 24, 0, 0, 768)])[0]
    yield expect("by hand: Cr = 128: red saturates at 127 (pixel 255), green is -91 (pixel 37), blue stays 128", set(r[4][i:i + 3] for i in range(0, 768, 3)), {bytes([255, 37, 128])})
    r = dec(exe, [(flat(cr=-512, cb=511, y=(-100, -100, -100, -100)), 24, 0, 0, 768)])[0]
    # Cr = -128 (F = -1024: a = -362, b = -128), Cb = 128, Y = -100 (F = -200 * 2 ... dc -100 -> F -200: a = (-579200 + 4095) >> 13 = -71, b = (-205616 + 4095) >> 13 = -25)
    # Y = -25: R = -25 + ((91881 * -128 + 32768) >> 16) = -25 - 179 -> -128 (pixel 0); G = -25 + ((-22525 * 128 + 46812 * 128 + 32768) >> 16) = -25 + 47 = 22
    # (pixel 150: 128 * 24287 / 65536 = 47.4); B = -25 + ((116131 * 128 + 32768) >> 16) = -25 + 227 -> 127 (pixel 255)
    yield expect("by hand: Cr = -128, Cb = 128, Y = -25: red saturates at -128 (0), green 22 (150), blue at 127 (255)", set(r[4][i:i + 3] for i in range(0, 768, 3)), {bytes([0, 150, 255])})
    # one AC coefficient: block Y1 with dc 0 and the pair (run 0, level 8) at scan index 1 (horizontal frequency 1, natural position 1):
    # F[1] = (8 * 16 * 1 + 4) >> 3 = 16 (16 is the reset table's second entry). Pass 1 (only src[1] is not 0, so y = 1, z = 0):
    # row 1 of dst = (16 * 2896 + 4095) >> 13 = 50431 >> 13 = 6 in every column x. Pass 2: src[y + 8] = 6 for y = 0..7, so
    # dst[x + 8y] = (6 * (scale[8 + x] / 8) + 4095) >> 13, scale row 1 / 8 rounded down = 4017, 3405, 2275, 799, -800, -2276, -3406, -4018:
    #   3 (28197 >> 13), 2 (24525), 2 (17745), 1 (8889), -1 (-705 >> 13), -2 (-9561), -2 (-16341), -3 (-20013)  for every row.
    r = dec(exe, [(mb_codes(ys=((0, ((0, 8),)), (0, ()), (0, ()), (0, ()))), 24, 0, 0, 768)])[0]
    row = [px24(r[4], 0, x, 0)[0] for x in range(8)]
    same_rows = all([px24(r[4], 0, x, y)[0] for x in range(8)] == row for y in range(8))
    yield expect("by hand: one coefficient at scan index 1 gives the row 131 130 130 129 127 126 126 125 in every row of Y1 (horizontal frequency), the rest 128",
                 (row, same_rows, px24(r[4], 0, 8, 0)[0], px24(r[4], 0, 0, 8)[0]), ([131, 130, 130, 129, 127, 126, 126, 125], True, 128, 128))
    # the same at scan index 2: natural position 8 (vertical frequency 1): the same numbers down the columns
    r = dec(exe, [(mb_codes(ys=((0, ((1, 8),)), (0, ()), (0, ()), (0, ()))), 24, 0, 0, 768)])[0]
    col = [px24(r[4], 0, 0, y)[0] for y in range(8)]
    # F[8] = (8 * qt[2] * 1 + 4) >> 3 = 16 (qt[2] = 16), now at row 1, column 0. Pass 1 (src[8], so y = 0, z = 1): dst[x] = (16 * w + 4095) >> 13
    # in row 0 only, w = scale row 1 / 8 rounded down = 4017, 3405, 2275, 799, -800, -2276, -3406, -4018: 8, 7, 4, 2, -2, -4, -7, -8.
    # Pass 2 (z = 0): dst[x + 8y] = (src[y] * 2896 + 4095) >> 13: 3, 2, 1, 1, -1, -1, -2, -3 down the rows (not the mirror of the horizontal
    # case: the rounding happens in a different order), equal along each row.
    yield expect("by hand: a coefficient at scan index 2 is natural position 8, the vertical frequency: 131 130 129 129 127 127 126 125 down the rows, equal along each row",
                 (col, all([px24(r[4], 0, x, y)[0] for x in range(8)] == [col[y]] * 8 for y in range(8))), ([131, 130, 129, 129, 127, 127, 126, 125], True))
    # the quantisation scale 0 mode: no table, no zigzag, the value is level * 2 and the position is the scan index used as a natural
    # position. Block Y1 with dc 0 and the pair (run 1, level 8): k = 2, F[2] = 16 (row 0, column 2: horizontal frequency 2; with the
    # zigzag it would have been natural position 8). Pass 1 (src[2]: y = 2, z = 0): row 2 of dst = (16 * 2896 + 4095) >> 13 = 6. Pass 2
    # (z = 2): dst[x + 8y] = (6 * w + 4095) >> 13 with w = scale row 2 / 8 rounded down = 3784, 1567, -1568, -3785, -3785, -1568, 1567, 3784:
    # 3 (26799), 1 (13497), -1 (-5313), -3 (-18615), -3, -1, 1, 3, the same in every row.
    r = dec(exe, [(mb_codes(ys=((0, ((1, 8),)), (0, ()), (0, ()), (0, ())), qs=0), 24, 0, 0, 768)])[0]
    row = [px24(r[4], 0, x, 0)[0] for x in range(8)]
    yield expect("by hand, quantisation scale 0: the value is level * 2 at the scan index as a natural position (horizontal frequency 2): 131 129 127 125 125 127 129 131 in every row",
                 (row, all([px24(r[4], 0, x, y)[0] for x in range(8)] == row for y in range(8))), ([131, 129, 127, 125, 125, 127, 129, 131], True))
    # and its first value is dc * 2, not dc * the table's first entry: with the table entry 255, dc 16 still gives F = 32, Y = 4 (pixel 132);
    # the same code with the scale 1 saturates (16 * 255 = 4080 -> 1023, Y = 127 -> pixel 255)
    r = dec(exe, [(flat(y=(16, 16, 16, 16), qs=0), 24, 0, 0, 768), (flat(y=(16, 16, 16, 16), qs=1), 24, 0, 0, 768)], ["reset", f"setq 40000000 64 {q255.hex()}"])
    yield expect("by hand, quantisation scale 0: the first value ignores the table (pixel 132 with the entry 255); scale 1 uses it and saturates (255)",
                 (px24(r[0][4], 0, 3, 3), px24(r[1][4], 0, 3, 3)), ((132,) * 3, (255,) * 3))

    # ---- more hand-worked blocks; each is worked out from the document's integer form and pins one rounding, one saturation or one table ----
    # (a) the rounding constant of the transform: dc -362 gives F = -724; a = (-724 * 2896 + 4095) >> 13 = -2092609 >> 13 = -256 (-255.4 rounds
    #     down); b = (-256 * 2896 + 4095) >> 13 = -737281 >> 13 = -91 (-737281 is one below -90 * 8192; with 0x1000 in place of 0xFFF it would be -90).
    #     Y = -91, pixel 37.
    r = dec(exe, [(flat(y=(-362,) * 4), 24, 0, 0, 768)])[0]
    yield expect("by hand: dc -362 gives Y = -91, pixel 37 (the sum is one below a multiple of 0x2000: the constant is 0xFFF)", px24(r[4], 0, 4, 4), (37,) * 3)
    # (b) the division of a negative table entry in the transform rounds down: F = -81 at scan index 3 (natural position 16, vertical frequency 2:
    #     the pair (run 2, level -34) with qs 1: (-34 * 19 + 4) >> 3 = -642 >> 3 = -81, the third quantisation entry being 19). Pass 1 (src[16]:
    #     y = 0, z = 2) with scale row 2 / 8 rounded down = 3784, 1567, -1568, -3785, -3785, -1568, 1567, 3784: row 0 = -37 (-36.9), -15 (-14.99),
    #     16 (16.004), 37 (37.9), 37, 16, -15, -37. Pass 2 (z = 0): row y = (v_y * 2896 + 4095) >> 13 = -13, -5, 6, 13, 13, 6, -5, -13, equal along
    #     each row. (Divided with truncation toward 0 the weights -1568 and -3785 become -1567 and -3784, v_2 = 15 and row 2 is 5.)
    r = dec(exe, [(mb_codes(ys=((0, ((2, -34),)), (0, ()), (0, ()), (0, ()))), 24, 0, 0, 768)])[0]
    col = [px24(r[4], 0, 0, y)[0] for y in range(8)]
    yield expect("by hand: F = -81 at natural position 16: 115 123 134 141 141 134 123 115 down the rows (the table entries divided by 8 round down)",
                 (col, all([px24(r[4], 0, x, y)[0] for x in range(8)] == [col[y]] * 8 for y in range(8))), ([115, 123, 134, 141, 141, 134, 123, 115], True))
    # (c) the quantised value rounds down: the pair (run 4, level -34) at scan index 5 (natural position 2) with qs 1: (-34 * 19 + 4) >> 3 = -81
    #     (-80.25 rounds down; truncation would give -80). Pass 1 (src[2]: y = 2, z = 0): row 2 = (-81 * 2896 + 4095) >> 13 = -230481 >> 13 = -29.
    #     Pass 2 (z = 2): (-29 * w + 4095) >> 13 with w = 3784, 1567, -1568, -3785 (and mirrored): -13 (-12.9), -6 (-5.05), 6, 13: the row
    #     -13 -6 6 13 13 6 -6 -13 in every row of the block (with -80 the second value would be -5).
    r = dec(exe, [(mb_codes(ys=((0, ((4, -34),)), (0, ()), (0, ()), (0, ()))), 24, 0, 0, 768)])[0]
    row = [px24(r[4], 0, x, 0)[0] for x in range(8)]
    yield expect("by hand: (run 4, level -34) at scan index 5 gives -81 (rounded down) and the row 115 122 134 141 141 134 122 115",
                 (row, all([px24(r[4], 0, x, y)[0] for x in range(8)] == row for y in range(8))), ([115, 122, 134, 141, 141, 134, 122, 115], True))
    # (d) the +4 of the quantised value: the pair (run 4, level -11): (-11 * 19 + 4) >> 3 = -205 >> 3 = -26 (-25.6 rounds down; without the +4,
    #     -209 >> 3 = -27). Pass 1: row 2 = (-26 * 2896 + 4095) >> 13 = -71201 >> 13 = -9. Pass 2: -9 * 3784 = -34056: (-34056 + 4095) >> 13 = -4;
    #     -9 * 1567: -2 ; -9 * -1568: 2 ; -9 * -3785: 4: the row -4 -2 2 4 4 2 -2 -4 (with -27: v = -10 and the first value -5).
    r = dec(exe, [(mb_codes(ys=((0, ((4, -11),)), (0, ()), (0, ()), (0, ()))), 24, 0, 0, 768)])[0]
    row = [px24(r[4], 0, x, 0)[0] for x in range(8)]
    yield expect("by hand: (run 4, level -11) gives -26 (the +4 matters) and the row 124 126 130 132 132 130 126 124",
                 (row, all([px24(r[4], 0, x, y)[0] for x in range(8)] == row for y in range(8))), ([124, 126, 130, 132, 132, 130, 126, 124], True))
    # (e) the saturation of a value, where it shows: Y1 has F = -1024 at natural position 8 (the pair (run 1, level -512): (-512 * 16 + 4) >> 3
    #     = -1024) and Cr = +100 (dc 400: F = 800, a = 283, b = 100) adds 140 to red (91881 * 100 + 32768 >> 16 = 140); green and blue stay far
    #     below -128 (pixel 0). Pass 1 (src[8]: y = 0, z = 1) with scale row 1 / 8 = 4017, 3405, 2275, 799, -800, -2276, -3406, -4018:
    #     row 0 = -502, -426, -284, -100, 100, 284, 426, 502; pass 2: row y = (v_y * 2896 + 4095) >> 13 = -177, -151, -100, -35, 35, 100, 151, 177.
    #     Red = -37 -> 91 in row 0 and -11 -> 117 in row 1. (If -1023 were the limit, row 1 would be -150 and red 118.)
    r = dec(exe, [(mb_codes(cr=(400, ()), ys=((0, ((1, -512),)), (0, ()), (0, ()), (0, ()))), 24, 0, 0, 768)])[0]
    yield expect("by hand: F = -1024 (the lower limit) with Cr = 100: red 91 in row 0 and 117 in row 1, green and blue 0",
                 (px24(r[4], 0, 2, 0), px24(r[4], 0, 5, 1)), ((91, 0, 0), (117, 0, 0)))
    # (f) the upper limit: the pair (run 1, level 511) with qs 3 at scan index 2 is (511 * 16 * 3 + 4) >> 3 = 3066, saturated to 1023; Cr = -100
    #     (dc -400: F = -800, Cr = -100) subtracts 140 from red ((91881 * -100 + 32768) >> 16 = -140) and adds 71 to green. Pass 1 with the row 1
    #     weights: row 0 = 502, 425, 284, 100, -100, -284, -425, -502; pass 2: row y = 177, 150, 100, 35, -35, -100, -150, -177.
    #     Red = 177 - 140 = 37 -> 165 in row 0, 150 - 140 = 10 -> 138 in row 1; green and blue are saturated at 127 (pixel 255).
    #     (With a limit of 7FFh or none, the value would be 2047 or 3066 and red larger.)
    r = dec(exe, [(mb_codes(cr=(-400, ()), ys=((0, ((1, 511),)), (0, ()), (0, ()), (0, ())), qs=3), 24, 0, 0, 768)])[0]
    yield expect("by hand: a value above the upper limit is held at 1023 (Y = 177 and 150 in rows 0 and 1): red 165 and 138, green and blue 255",
                 (px24(r[4], 0, 2, 0), px24(r[4], 0, 5, 1)), ((165, 255, 255), (138, 255, 255)))
    # (g) the colour constants away from the rounding edges. Y = -30 (dc -120: F = -240, a = -85, b = -30). Cr = 100 (dc 400): red = -30 + 140
    #     = 110 -> 238; green = -30 + ((-46812 * 100 + 32768) >> 16 = -71) = -101 -> 27; blue = -30 -> 98.
    #     Cb = 60 (dc 240: F = 480, a = 170, b = 60): red 98; green = -30 + ((-22525 * 60 + 32768) >> 16 = -21) = -51 -> 77;
    #     blue = -30 + ((116131 * 60 + 32768) >> 16 = 106) = 76 -> 204.
    r = dec(exe, [(flat(cr=400, y=(-120,) * 4), 24, 0, 0, 768), (flat(cb=240, y=(-120,) * 4), 24, 0, 0, 768)])
    yield expect("by hand: Y = -30 with Cr = 100 gives (238, 27, 98); with Cb = 60 gives (98, 77, 204)", (px24(r[0][4], 0, 7, 9), px24(r[1][4], 0, 7, 9)), ((238, 27, 98), (98, 77, 204)))

    # ---- the order of macroblocks and of padding ----
    mb1 = flat(y=(16, 16, 16, 16)) ; mb2 = flat(y=(-16, -16, -16, -16))
    r = dec(exe, [(mb1 + mb2, 24, 0, 0, 1536)])[0]
    yield expect("two macroblocks in a row: the second follows the first at byte 768", (r[0], r[1], r[2], px24(r[4], 0, 5, 5), px24(r[4], 1, 5, 5), len(r[4])), (OK, 2, 1536, (132,) * 3, (124,) * 3, 1536))
    r = dec(exe, [([EOB, EOB] + mb1 + [EOB] * 3 + mb2 + [EOB] * 7, 24, 0, 0, 1536)])[0]
    yield expect("padding: FE00h before a block and after the last is skipped (the same two macroblocks)", (r[0], r[1], r[2], px24(r[4], 0, 5, 5), px24(r[4], 1, 5, 5)), (OK, 2, 1536, (132,) * 3, (124,) * 3))
    r = dec(exe, [([], 24, 0, 0, 0), ([EOB] * 5, 24, 0, 0, 0)])
    yield expect("no codes, or nothing but padding: no macroblocks, result 0", [(x[0], x[1], x[2], x[3]) for x in r], [(OK, 0, 0, True)] * 2)
    # a block that reaches the last coefficient with no end-of-block code of its own: the next block starts right after it
    blk_noeob = [(1 << 10) | 0] + [run_level_value(62, 5)]          # dc 0, then a pair that lands on coefficient 63
    mb_a = blk_noeob * 6
    mb_b = []
    for _ in range(6):
        mb_b += blk_noeob + [EOB]
    ra = dec(exe, [(mb_a, 24, 0, 0, 768), (mb_b, 24, 0, 0, 768)])
    yield expect("a block whose 64th coefficient is stored needs no end-of-block code: with and without it the same macroblock, result 0", (ra[0][0], ra[1][0], ra[0][4] == ra[1][4], len(ra[0][4])), (OK, OK, True, 768))
    mb_bad = blk_noeob * 5 + [(1 << 10)]
    rb = dec(exe, [(mb_bad, 24, 0, 0, 768)])[0]
    yield expect("control of the above: the same macroblock cut after its sixth block's DC is refused", (rb[0], rb[1], rb[2]), (E_INCOMPLETE, 0, 0))

    # ---- the transform and the colour, against the references ----
    # THE BOUND OF THE TRANSFORM, argued from the document. The document's integer form computes, in two passes,
    #   dst = (sum_z src * floor(scale / 8) + 0xFFF) >> 13,
    # and says that the hardware "appears to be working roughly like that, still the results aren't perfect". The exact transform is
    # out[y][x] = sum over (v, u) of F[v][u] * T(v, y) * T(u, x) with T(k, n) = scale_k * cos((2n+1) k pi / 16) / 4, where the
    # table's rows are 1.414 (k = 0) and 2 cos(..) (k >= 1) times 16384, i.e. the orthonormal transform. Three things differ:
    #  (1) the table is 16 bit fixed point and the low 3 bits are dropped; measured on the document's table (a number of the table,
    #      not of the C): each effective weight differs from the true one by at most 1.2e-4;
    #  (2) each pass rounds to an integer: error at most 0.5 per value;
    #  (3) pass 2 multiplies the errors of pass 1 by the weights, whose absolute sum for one output is at most W = 2.65 (the largest sum
    #      of |T(k, n)| over k, computed from the formula).
    # For blocks whose coefficients all have |F| <= 64 (the cases below are drawn so): the error of a pass 1 value is at most
    # 0.5 + 8 * 64 * 1.2e-4 = 0.562; a value of pass 1 is at most 2.65 * 64 + 0.6 = 170 in size; the error of the result is at most
    # 0.5 (its rounding) + 2.65 * 0.562 (the errors of pass 1) + 8 * 170 * 1.2e-4 (the weights again) = 2.15 against the exact value T.
    # The integer result is compared with round(T): the two differ by at most 2.15 + 0.5 = 2.65, and since both are integers, by at most 2.
    # So the bound is 2 for the transform. (Blocks with larger coefficients are covered by the hand-worked cases above.)
    # THE BOUND OF THE COLOUR CONVERSION: the integer constants are the decimals times 65536, rounded (error at most 0.5/65536 each, times
    # |Cr|, |Cb| <= 128: 0.001), and the sum is rounded to an integer (0.5): against the decimal formula rounded to the nearest integer
    # the two differ by at most 1 (a value within 0.001 of a half can round differently, never further).
    # The first check below applies this bound to the conversion alone, for flat blocks whose values are known exactly (dc_only_value is the
    # case worked out above, for any first value).
    kinds = [-512, -400, -256, -192, -130, -100, -64, -33, -10, -1, 0, 1, 10, 33, 64, 100, 130, 192, 256, 400, 511]
    jobs, want = [], []
    combos = [(cr, cb, ys) for cr in kinds for cb in kinds for ys in [tuple(rnd.choice(kinds) for _ in range(4))]]
    for cr, cb, ys in combos:
        jobs.append(flat(cr=cr, cb=cb, y=ys))
    res = dec(exe, [(sum(jobs[i:i + 20], []), 24, 0, 0, 768 * 20) for i in range(0, len(jobs), 20)])
    out = b"".join(r[4] for r in res)
    worst, bad = 0, None
    for mbi, (cr, cb, ys) in enumerate(combos):
        crv, cbv = dc_only_value(2 * cr), dc_only_value(2 * cb)
        for b4, y in enumerate(ys):
            yv = dc_only_value(2 * y)
            x0, y0 = (b4 % 2) * 8, (b4 // 2) * 8
            got = px24(out, mbi, x0 + 3, y0 + 5)
            ref = pixel_dec(yv, crv, cbv)
            d = max(abs(a - b) for a, b in zip(got, ref))
            worst = max(worst, d)
            if d > 1 and bad is None:
                bad = (cr, cb, y, got, ref)
    yield check(f"colour conversion: {len(combos) * 4} flat blocks (chroma and luminance over -512..511, saturating at both ends) within 1 of the decimal formula", None if bad is None else str(bad))
    yield check("colour conversion: the sweep is not vacuous (pixels reach both 0 and 255)", None if min(out) == 0 and max(out) == 255 else f"{min(out)}..{max(out)}")

    # transform, luminance: random blocks, Cr = Cb = 0 so that the pixel is Y + 128
    def rand_block(qs, qt):
        for _ in range(1000):
            dc = rnd.randrange(-30, 31)
            nz = rnd.randrange(0, 20)
            pairs, k = [], 0
            for _ in range(nz):
                run = rnd.choice([0, 0, 0, 1, 2, 4])
                if k + run + 1 > 63:
                    break
                k += run + 1
                pairs.append((run, rnd.choice([-1, 1]) * rnd.randrange(1, 8)))
            F = coefficients(qs, dc, pairs, qt)
            if max(abs(v) for v in F) <= 64:
                return dc, pairs, F
        raise RuntimeError("no block")
    worst_t, bad = 0, None
    jobs, refs = [], []
    for n in range(60):
        qs = rnd.choice([0, 1, 1, 2, 3])
        ys = [rand_block(qs, DEFAULT_QUANT) for _ in range(4)]
        jobs.append((mb_codes(ys=tuple((d, tuple(p)) for d, p, F in ys), qs=qs), 24, 0, 0, 768))
        refs.append([[rnd_round(v) for v in idct_float(F)] for d, p, F in ys])
    for (job, ref), r in zip(zip(jobs, refs), dec(exe, jobs)):
        for b4 in range(4):
            x0, y0 = (b4 % 2) * 8, (b4 // 2) * 8
            for y in range(8):
                for x in range(8):
                    got = px24(r[4], 0, x0 + x, y0 + y)[0] - 128
                    d = abs(got - ref[b4][y * 8 + x])
                    worst_t = max(worst_t, d)
                    if d > 2 and bad is None:
                        bad = (b4, x, y, got, ref[b4][y * 8 + x])
    yield check("transform: 60 macroblocks of random blocks (quantisation scale 0 to 3, |F| <= 64) within 2 of the exact inverse transform rounded", None if bad is None else str(bad))
    yield check("transform: the comparison is not vacuous (the random blocks are not flat: some pixel differs from its block's first by 5 or more)", None if worst_t >= 0 and any(max(r) - min(r) >= 10 for rr in refs for r in rr) else "flat")
    # transform, chroma and the upsampling: Y = 0, Cr and Cb random; each pixel must come from the chroma value of its 2 by 2 square
    bad = None
    jobs, refs = [], []
    for n in range(30):
        qs = rnd.choice([1, 2])
        cr, cb = rand_block(qs, DEFAULT_QUANT), rand_block(qs, DEFAULT_QUANT)
        jobs.append((mb_codes(cr=(cr[0], tuple(cr[1])), cb=(cb[0], tuple(cb[1])), qs=qs), 24, 0, 0, 768))
        refs.append(([rnd_round(v) for v in idct_float(cr[2])], [rnd_round(v) for v in idct_float(cb[2])]))
    for r, (crf, cbf) in zip(dec(exe, jobs), refs):
        for y in range(16):
            for x in range(16):
                ci = x // 2 + (y // 2) * 8
                got = px24(r[4], 0, x, y)
                found = any(max(abs(a - b) for a, b in zip(got, pixel_dec(0, crf[ci] + dr, cbf[ci] + db))) <= 1 for dr in range(-2, 3) for db in range(-2, 3))
                if not found and bad is None:
                    bad = (x, y, got, crf[ci], cbf[ci])
    yield check("chroma: 30 macroblocks of random Cr and Cb blocks: every pixel is the decimal conversion (within 1) of chroma values within 2 of the exact transform of the block, taken at (x/2, y/2)", None if bad is None else str(bad))

    # ---- the decoder: every refusal, with a negative control ----
    m24 = flat(y=(16, 16, 16, 16))
    got = dec(exe, [(m24, d, 0, 0, 1024) for d in (24, 15, 8, 4, 0, 16, 32, -24)])
    yield expect("E_DEPTH: 8, 4, 0, 16, 32 and -24 are refused with nothing written; 24 and 15 are accepted", [(g[0], g[2]) for g in got], [(OK, 768), (OK, 512)] + [(E_DEPTH, 0)] * 6)
    got = dec(exe, [(m24, 24, 0, 0, n) for n in (0, 1, 767, 768, 769)] + [(m24, 15, 0, 0, n) for n in (511, 512)] + [(m24 + m24, 24, 0, 0, n) for n in (767, 768, 1535, 1536)])
    yield expect("E_ROOM: 767 of 768 bytes refused, 768 accepted; 511 of 512 (15 bit) refused, 512 accepted; two macroblocks in 1535 give the first and refuse",
                 [(g[0], g[1], g[2], g[3]) for g in got],
                 [(E_ROOM, 0, 0, True)] * 3 + [(OK, 1, 768, True)] * 2 + [(E_ROOM, 0, 0, True), (OK, 1, 512, True)] + [(E_ROOM, 0, 0, True), (E_ROOM, 1, 768, True), (E_ROOM, 1, 768, True), (OK, 2, 1536, True)])
    full_codes_mb = m24 + m24
    got = dec(exe, [(full_codes_mb[:n], 24, 0, 0, 1536) for n in range(0, len(full_codes_mb) + 1)])
    mb_len = len(m24)
    bad = []
    for n, g in enumerate(got):
        # complete macroblocks in the first n codes; the codes after them must be only padding for the result to be 0
        done = n // mb_len
        rest = full_codes_mb[done * mb_len:n]
        # a complete macroblock includes its trailing end-of-block code; one cut before that code is still complete (the block is full only
        # when its end code is read), so what is expected is: result 0 if the remainder is empty, else E_INCOMPLETE unless the last block
        # is the only thing missing its end code
        if not rest:
            want = (OK, done)
        elif n == (done + 1) * mb_len - 1:
            want = (E_INCOMPLETE, done)   # the end code of the sixth block is missing: the block is not finished
        else:
            want = (E_INCOMPLETE, done)
        if (g[0], g[1]) != want or not g[3]:
            bad.append((n, g[:2]))
    yield check("E_INCOMPLETE: the codes of two macroblocks cut at every length: complete macroblocks are written, a cut inside a macroblock is refused, guards intact", None if not bad else str(bad[:5]))
    fz = []
    for _ in range(300):
        codes = [rnd.choice([EOB, rnd.randrange(65536), (rnd.randrange(64) << 10) | rnd.randrange(1024), rnd.randrange(65536)]) for _ in range(rnd.randrange(0, 90))]
        fz.append((codes, rnd.choice([24, 15, 24, 15, 8]), rnd.randrange(2), rnd.randrange(2), rnd.choice([0, 100, 511, 512, 768, 2000])))
    setup = ["reset", "setq 40000001 128 " + bytes(rnd.randrange(256) for _ in range(128)).hex(), "sets 60000000 128 " + bytes(rnd.randrange(256) for _ in range(128)).hex()]
    got = dec(exe, fz, setup)
    bad = [i for i, g in enumerate(got) if not g[3] or g[0] > 0 or g[0] < E_COMMAND]
    yield check("random codes, random tables (quantisation and scale), small rooms: never out of bounds, the result always one of the defined ones", None if not bad else str(bad[:5]))
    # extreme scale table and codes: no overflow of the arithmetic's range (the results are only required to be defined)
    big = bytes([0xFF, 0x7F] * 64)
    small = bytes([0x00, 0x80] * 64)
    ext = [(([(63 << 10) | 0x1FF] + [run_level_value(0, 0x1FF)] * 63 + [EOB]) * 6, 24, 0, 0, 768), (([(63 << 10) | 0x200] + [run_level_value(0, 0x200)] * 63 + [EOB]) * 6, 24, 0, 0, 768)]
    for s in (big, small):
        for q in (255, 255):
            got = dec(exe, ext, ["reset", f"setq 40000001 128 {bytes([q] * 128).hex()}", f"sets 60000000 128 {s.hex()}"])
            yield check("extreme tables (scale 7FFFh or 8000h everywhere, quantisation 255) and the largest levels: defined, in bounds", None if all(g[0] == OK and g[3] for g in got) else str([g[:4] for g in got]))

    # ---- the read of a bitstream decoded ----
    # the whole path on invented data: codes from the reader into the decoder give the macroblocks that the encoder was given
    bits, codes_ = encode_frame(2, 1, [flat_mb((0, 0, 16, -16, 32, 0)), flat_mb((0, 0, 16, 16, 16, 16))])
    g = vlc(exe, [frame_bytes(2, 1, bits)])[0]
    r = dec(exe, [(g[3], 24, 0, 0, 1536)])[0]
    yield expect("the reader's codes go into the decoder: two invented macroblocks, the Y1 Y2 Y3 Y4 values 4 -4 8 0 in the first (Cr = Cb = 0)",
                 (g[0], r[0], r[1], px24(r[4], 0, 0, 0)[0], px24(r[4], 0, 8, 0)[0], px24(r[4], 0, 0, 8)[0], px24(r[4], 0, 8, 8)[0]), (OK, OK, 2, 132, 124, 136, 128))


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--mdec", type=Path, default=SRC / "mdec.c")
    ap.add_argument("--cc", default=None)
    args = ap.parse_args()
    cc = args.cc or shutil.which("cc")
    if not cc:
        print("the host's C compiler `cc` is missing; these controls need it")
        return 2
    t0 = time.time()
    failed = count = 0
    with tempfile.TemporaryDirectory(prefix="hostmdec-test-") as tmp:
        root = Path(tmp)
        (root / "mdecmain.c").write_text(C_MAIN)
        exe = root / "mdectest"
        proc = subprocess.run([cc, "-O1", "-Wall", "-Wextra", "-Werror", "-I", str(SRC), "-o", str(exe), str(root / "mdecmain.c"), str(args.mdec)], capture_output=True, text=True, timeout=180)
        if proc.returncode != 0:
            print("FAIL the test program does not build:\n" + proc.stderr[-1500:])
            return 1
        if not exe.exists() and exe.with_suffix(".exe").exists():
            exe = exe.with_suffix(".exe")
        last = "(none)"
        try:
            for name, bad in cases(exe, root):
                last = name
                count += 1
                if bad is None:
                    print(f"ok   {name}")
                else:
                    print(f"FAIL {name}: {bad}")
                    failed += 1
        except Exception as e:  # the driver ended, or answered with fewer lines than it was asked for
            where = f"`{IN_WORK[0]}`" if IN_WORK[0] else f"the case that follows `{last}`"
            print(f"FAIL {where}: the test program stopped or answered short: {type(e).__name__}: {e}")
            print(f"{count} cases before it; the cases after it were not run")
            return 1
    print(f"{count} cases, {time.time() - t0:.1f} seconds")
    print(f"{failed} case(s) behaved wrongly" if failed else f"all {count} cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
