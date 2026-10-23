#!/usr/bin/env python3
"""Analysis tool: find which arcade sample each PS1 sound sample comes from, and how the PS1 made it.

What it does. The PS1 keeps its sounds as Sony VAB banks: a header (table 3, "pBAV": 128 program entries, the tones
of the used programs, 32 bytes each, then a table of 256 u16 sample sizes in 8-byte units) and a body (table 4: the
samples one after the other, SPU ADPCM, 16-byte blocks of a shift/filter byte, a flags byte and 14 bytes of 4-bit
values). Fighters' files (PLnn.PAC, PLnnX.PAC) carry slot 0/1 of each; FIRSTSND.PAC, GAME_SND.PAC, SELECT.PAC, the
CONT, END and DEMO files carry shared banks. The arcade keeps its samples in the QSound ROM (sz2.11m + sz2.12m,
signed 8-bit PCM; the chip's view is the two members one after the other with every byte pair swapped).

Rules found on 2026-10-23 with the user's disc and set (every address and rate below was measured, not assumed):
  - A PS1 sample is the arcade sample resampled from 24,000 Hz to a "rate" R, times 256 (the 8-bit value in the high
    byte), then SPU-ADPCM encoded. So PS1 sample k (k >= 28) lies at the arcade address A + rho * (k - 28), with
    rho = 24000 / R and A an integer (the arcade sample's start address in the chip's view; A is often a multiple of
    0x8000, the QSound bank size). The measured gain is 256 (LS fit 0.975..1.003 of it, 250 on average).
  - R comes from a short list: 11025, 14000, 22050, 24000, 26000, 26222, 28000, 32000, 36000, 40000, 44100. For the
    tones that play the sample at its natural key it is the tone's pitch: R = 44100 * 2^((72 - center + shift/128)/12)
    (center, shift are bytes 4 and 5 of the 32-byte tone), rounded to the list (254 of 277 matched samples; the
    others have key-range tones or a tone that disagrees with the data).
  - Block 0 of every PS1 sample is zeros (the encoder's warm-up), the sample data starts in block 1 (flags 4), the last
    data block has flags 1, and a final block (flags 7, shift 0, filter 0, nibbles 7) closes it. 12 samples instead
    loop the whole body (flags 2 in every block, 3 on the last).
  - Not all of a PS1 sample is arcade data: some lack a part (the PS1's own), some stop matching at a point; see the
    report. SELECT.PAC's 27 samples and a few others have no arcade source that this search finds.

Result of `match` on 2026-10-23 (409 distinct samples, 1,595 in the banks, 223 s on 32 cores): 182 match over 95% of
their loud blocks, 92 over 80%, 51 over 50%, 30 under 50%, 54 have no source found (23 of them SELECT.PAC's); 346 distinct
start addresses. `encode-test` on the first 40: the closed-loop encode reaches 29.3 dB against the resampled arcade data,
the PS1's own samples 17.3 dB (the PS1 encoder is the noisier one; its bytes are not reproduced).

What the tool does with that. `match` decodes every sample of the disc's banks, finds for each (once per distinct
sample) the rate and address with a coarse search of the whole ROM (blocks of 1,024 arcade samples through a box filter
of 16, in C compiled at run time) and checks each candidate at the full rate with a Lanczos resampler: per block of 196
PS1 samples it asks whether the PS1 sample equals gain * the arcade sample resampled (correlation above 0.85 is a match
'M', above 0.5 with a plausible gain is a match limited by ADPCM noise 'n', rms under 150 is quiet 'q', else '.'). It
prints one line per distinct sample: the rate, the arcade address, the share of the loud blocks that match and the SNR
of the PS1 decode against the resampled arcade data. `inventory` lists the banks only. `encode-test` resamples the
arcade data at the found address, encodes it with the closed-loop encoder here (best of 5 filters x 13 shifts per block
by squared error) and reports the SNR of that encode and of the PS1's against the resampled arcade data.

Inputs: --disc BIN (raw 2352-byte sectors) and --arcade ZIP (MAME's sfa2u), both only read. It writes nothing except a
temporary folder (the C helper, the decoded samples) that it removes. It prints sizes, addresses, rates and numbers,
never game bytes. It is an analysis and reference tool; the converter that the port runs will be written in C.

The model of the runtime converter (2026-10-23, later). The Z80 sound program (sz2.01a, sz2.02a) holds the arcade's instrument table:
435 records of 8 bytes at 0x8000 of the two members one after the other (the Z80's first banked window): bank, start, loop start and
inclusive end (16-bit addresses in the bank, bank * 64 KB in the sample ROM) and the root key (60, 72, once 75). 306 of the 354 matched PS1
samples start exactly at a record's start (the others start inside a record or end past it: the PS1 cut them); the record has no rate,
volume or envelope. A PS1 sample made from a record of `length` data (end - start + 1) at rate R has ceil(length * R / (24000 * 28)) + 2
blocks in 248 of those 306 (the rest are cut or lengthened by tens of samples, or the tone's rate is not the sample's). The runtime
converter (vag.c) takes `blocks` from a generated table (the disc's count), so its sizes are the disc's. `vag_model` is the model the C
reproduces byte for byte: a Lanczos (radius 16) resampler from a 512-step table, times 256, rounded, the closed-loop coder of `encode_block`.
`vh` classes the fields of every VAB header against the arcade and the rules.

Matching takes minutes (223 s on 32 cores), so `match --save FILE` keeps its lines (one per distinct sample) and `vh`, `compare-c` and
`c-table` take `--matches FILE`; without it they run the match first.

    python3 port/arcade/samples.py inventory --disc BIN
    python3 port/arcade/samples.py match --disc BIN --arcade ZIP [--only PL00] [--detail] [--save FILE]
    python3 port/arcade/samples.py encode-test --disc BIN --arcade ZIP [--only PL00] [--limit 8]
    python3 port/arcade/samples.py vh --disc BIN --arcade ZIP [--matches FILE]
    python3 port/arcade/samples.py compare-c --disc BIN --arcade ZIP [--matches FILE] [--model-blocks 8] [--only PL00]
    python3 port/arcade/samples.py c-table --disc BIN --arcade ZIP [--matches FILE]      # prints port/arcade/vag_table.c
"""
import argparse
import collections
import hashlib
import math
import os
import shutil
import struct
import subprocess
import sys
import tempfile
import time
import zipfile
from pathlib import Path

ARCADE_RATE = 24000
PS1_BASE_RATE = 44100
NOMINAL_RATES = (11025, 14000, 22050, 24000, 26000, 26222, 28000, 32000, 36000, 40000, 44100)
FILTERS = ((0, 0), (60, 0), (115, -52), (98, -55), (122, -60))
BLOCK_SAMPLES = 28
SECTOR = 2048


class FormatError(Exception):
    pass


# ---------------------------------------------------------------- pure functions (tested without game data)

def pac_chunks(data):
    """[(table, slot, data)] of a chunk archive: u32 count, 32-byte entries from 0x20 (u32 type = table << 16 | slot,
    u32 size), data from 0x800, each chunk padded to 2,048 bytes."""
    if len(data) < SECTOR:
        raise FormatError('shorter than one sector')
    count = struct.unpack_from('<I', data, 0)[0]
    out, pos = [], SECTOR
    for i in range(count):
        typ, size = struct.unpack_from('<II', data, 0x20 + 32 * i)
        if pos + size > len(data):
            raise FormatError(f'chunk {i} runs past the end')
        out.append((typ >> 16, typ & 0xffff, data[pos:pos + size]))
        pos += (size + SECTOR - 1) // SECTOR * SECTOR
    return out


def parse_vh(vh):
    """Parse a VAB header: {'programs': used program count, 'tones': [dict], 'sizes': [bytes of VAG 1..n]}.
    Layout: 32-byte header (magic 'pBAV', version, id, size, u16 reserved, u16 programs, u16 tones, u16 VAGs, ...),
    128 program records of 16 bytes, programs * 16 tone records of 32 bytes, then 256 u16 VAG sizes (in 8 bytes;
    entry 0 is unused)."""
    if vh[:4] != b'pBAV':
        raise FormatError('not a VAB header')
    ps, ts, vs = struct.unpack_from('<HHH', vh, 0x12)
    base = 0x20 + 128 * 16
    tones = []
    for p in range(ps):
        for t in range(16):
            rec = vh[base + (p * 16 + t) * 32: base + (p * 16 + t) * 32 + 32]
            if len(rec) < 32:
                raise FormatError('tone table cut short')
            prior, mode, vol, pan, center, shift, lo, hi = rec[:8]
            adsr1, adsr2, prog, vag = struct.unpack_from('<HHhh', rec, 16)
            if vag > 0:
                tones.append(dict(program=p, tone=t, center=center, shift=shift, low=lo, high=hi, vag=vag,
                                  vol=vol, pan=pan, adsr1=adsr1, adsr2=adsr2))
    tab = base + ps * 16 * 32
    if len(vh) < tab + 2 * (vs + 1):
        raise FormatError('size table cut short')
    sizes = [8 * struct.unpack_from('<H', vh, tab + 2 * (i + 1))[0] for i in range(vs)]
    return {'programs': ps, 'declared_tones': ts, 'tones': tones, 'sizes': sizes}


def vag_slices(vb, sizes):
    """Cut a VAB body into its samples; raises if the sizes do not add up to the body."""
    if sum(sizes) != len(vb):
        raise FormatError(f'sizes add up to {sum(sizes)}, body is {len(vb)}')
    out, pos = [], 0
    for s in sizes:
        out.append(vb[pos:pos + s])
        pos += s
    return out


def decode_adpcm(data):
    """SPU ADPCM -> (samples, flags per block). A block: low nibble of byte 0 = shift (12 = none), high = filter,
    byte 1 = flags, bytes 2..15 = 28 values, low nibble first. The filters are 0, 60/64, 115/64 - 52/64, 98/64 - 55/64
    and 122/64 - 60/64 of the last two outputs."""
    pcm, flags, h1, h2 = [], [], 0, 0
    for i in range(0, len(data) - 15, 16):
        shift, filt, fl = data[i] & 15, min(data[i] >> 4, 4), data[i + 1]
        if shift > 12:
            shift = 9
        a, c = FILTERS[filt]
        flags.append(fl)
        for k in range(28):
            n = (data[i + 2 + k // 2] >> (4 * (k & 1))) & 15
            if n >= 8:
                n -= 16
            s = ((n << 12) >> shift) + ((h1 * a + h2 * c + 32) >> 6)
            s = -32768 if s < -32768 else 32767 if s > 32767 else s
            pcm.append(s)
            h2, h1 = h1, s
    return pcm, flags


def encode_block(x, h1, h2):
    """Closed-loop encode of 28 values: the (filter, shift) of least squared error. Returns (16 bytes, h1, h2, error);
    flags are left 0."""
    best = None
    for f, (a, c) in enumerate(FILTERS):
        for shift in range(13):
            step = 4096 >> shift
            p1, p2, err, nib = h1, h2, 0, []
            for v in x:
                pred = (p1 * a + p2 * c + 32) >> 6
                n = int(round((v - pred) / step))
                n = -8 if n < -8 else 7 if n > 7 else n
                y = n * step + pred
                y = -32768 if y < -32768 else 32767 if y > 32767 else y
                err += (v - y) * (v - y)
                if best is not None and err >= best[0]:
                    break
                nib.append(n & 15)
                p2, p1 = p1, y
            else:
                if best is None or err < best[0]:
                    best = (err, f, shift, nib, p1, p2)
    err, f, shift, nib, p1, p2 = best
    out = bytearray([(f << 4) | shift, 0])
    for i in range(0, 28, 2):
        out.append(nib[i] | (nib[i + 1] << 4))
    return bytes(out), p1, p2, err


def encode_adpcm(pcm):
    """Encode samples into blocks (padded with zeros to a multiple of 28); flags are 0."""
    pcm = list(pcm) + [0] * (-len(pcm) % 28)
    out, h1, h2 = bytearray(), 0, 0
    for i in range(0, len(pcm), 28):
        b, h1, h2, _ = encode_block(pcm[i:i + 28], h1, h2)
        out += b
    return bytes(out)


def tone_rate(center, shift):
    """The rate in Hz at which a tone plays its sample at key 72: 44100 * 2^((72 - center + shift/128)/12)."""
    return PS1_BASE_RATE * 2 ** ((72 - center + shift / 128) / 12)


def snap_rate(rate, tol=0.006):
    """The nearest of NOMINAL_RATES within tol (relative), else None."""
    best = min(NOMINAL_RATES, key=lambda r: abs(r / rate - 1))
    return best if abs(best / rate - 1) <= tol else None


def candidate_rates(tones):
    """Rates to try first for a sample: its tones' rates snapped to the list (all of the list when none snap)."""
    got = sorted({snap_rate(tone_rate(t['center'], t['shift'])) for t in tones} - {None})
    return got or list(NOMINAL_RATES)


def lanczos(x, radius=16):
    if x == 0:
        return 1.0
    if abs(x) >= radius:
        return 0.0
    px = math.pi * x
    return math.sin(px) / px * math.sin(px / radius) / (px / radius)


def resample_lanczos(src, pos0, rho, count, radius=16):
    """src (a sequence of numbers) read at pos0 + rho * k for k < count, band-limited when rho > 1."""
    cut = 1.0 / rho if rho > 1 else 1.0
    half = int(math.ceil(radius / cut))
    out = []
    for k in range(count):
        p = pos0 + rho * k
        i0 = int(math.floor(p))
        s = 0.0
        for j in range(i0 - half + 1, i0 + half + 1):
            if 0 <= j < len(src):
                s += src[j] * lanczos((p - j) * cut, radius) * cut
        out.append(s)
    return out


def snr_db(ref, x):
    """10 log10 (energy of ref / energy of ref - x); inf for a perfect copy."""
    err = sum((a - b) ** 2 for a, b in zip(ref, x))
    sig = sum(a * a for a in ref)
    return math.inf if err == 0 else -math.inf if sig == 0 else 10 * math.log10(sig / err)


def flag_structure(flags):
    """A name for a sample's loop flags: 'oneshot' (flags 4 on block 1, 1 on the last data block, 7 on the last block),
    'loop' (flags 2 everywhere), 'oneshot+loopbit' (the one-shot plus a second 1/7 pair inside), else 'other'."""
    n = len(flags)
    nz = {i: v for i, v in enumerate(flags) if v}
    if n >= 3 and nz == {1: 4, n - 2: 1, n - 1: 7}:
        return 'oneshot'
    if n >= 3 and all(flags[i] == 2 for i in range(1, n - 1)) and flags[n - 1] == 3:
        return 'loop'
    if n >= 5 and nz.get(1) == 4 and nz.get(n - 2) == 1 and nz.get(n - 1) == 7 and len(nz) > 3:
        return 'oneshot+loopbit'
    return 'other'


def cluster_hits(hits, block=1024, tol=8, min_ncc=0.9):
    """hits: [(sample, rho, block index, ncc, position)] from the coarse search. Returns candidate lines
    [(sample, rho, origin, block)], one per cluster of equal origin (position - block * 1024) within tol, best first."""
    by = collections.defaultdict(list)
    for s, rho, b, ncc, pos in hits:
        if ncc >= min_ncc:
            by[(s, rho)].append((ncc, b, pos - b * block))
    out = []
    for (s, rho), v in by.items():
        used = []
        for ncc, b, org in sorted(v, reverse=True):
            if any(abs(org - u) <= tol for u in used):
                continue
            used.append(org)
            out.append((s, rho, org, b))
    return out


def state_summary(state):
    """(share of loud blocks that match, class) of a block-state string; the first and last block are ignored.
    Classes: full >= 95%, 80-95, 50-80, under 50."""
    inner = state[1:-1]
    got = sum(1 for c in inner if c in 'Mn')
    bad = sum(1 for c in inner if c == '.')
    share = got / (got + bad) if got + bad else 1.0
    return share, ('full' if share >= .95 else '80-95' if share >= .8 else '50-80' if share >= .5 else 'under 50')


def acceptable_line(state):
    """A candidate line is real when it matches at least 6 blocks, 4 of them well."""
    inner = state[1:-1]
    return inner.count('M') >= 4 and sum(1 for c in inner if c in 'Mn') >= 6


def start_address(origin, rho):
    """The arcade address of the sample's first datum: PS1 sample 28 (block 1) lies at origin + 28 * rho."""
    return origin + BLOCK_SAMPLES * rho


# ---------------------------------------------------------------- the converter's model (the C in vag.c reproduces it)

KERNEL_RADIUS = 16
KERNEL_OVERSAMPLE = 512
INSTRUMENT_TABLE = 0x8000          # in the Z80 view (sz2.01a then sz2.02a): the first banked window
INSTRUMENT_COUNT = 435
INSTRUMENT_SIZE = 8
BLOCK_BYTES = 16
# (center, shift) the PS1 tool wrote for a tone that plays a sample resampled to this rate at its natural pitch (voices and effects, root
# key 72); measured over the banks, 90% of those tones. The pair is not a formula of the rate (the nearest fine-tune differs by a few
# steps); music instruments with key ranges use other centers.
RATE_TONE = {44100: (72, 0), 22050: (84, 0), 32000: (78, 57), 40000: (74, 40), 28000: (80, 18), 36000: (76, 22), 24000: (83, 60),
             26000: (82, 110)}


def parse_instruments(sound):
    """[dict(index, start, loop, end, root)] of the Z80 program's instrument table: 435 records of 8 bytes (bank, u16 start, u16 loop
    start, u16 inclusive end, root key), addresses in the sample ROM (bank * 0x10000 + the 16-bit address). Raises FormatError when the
    view is short, a bank is past the 4 MB or the addresses are not in order."""
    out = []
    for i in range(INSTRUMENT_COUNT):
        r = sound[INSTRUMENT_TABLE + INSTRUMENT_SIZE * i:INSTRUMENT_TABLE + INSTRUMENT_SIZE * (i + 1)]
        if len(r) < INSTRUMENT_SIZE:
            raise FormatError('instrument table cut short')
        bank = r[0]
        start, loop, end = (struct.unpack_from('<H', r, k)[0] for k in (1, 3, 5))
        if bank >= 0x40 or not start <= loop <= end:
            raise FormatError(f'instrument {i} is not plausible')
        out.append(dict(index=i, start=bank << 16 | start, loop=bank << 16 | loop, end=bank << 16 | end, root=r[7]))
    return out


def vag_blocks(length, rate):
    """Blocks of a PS1 sample made from `length` arcade data at `rate` Hz: ceil(length * rate / (24000 * 28)) + 2 (a zero block, the
    data, a closing block)."""
    return -(-length * rate // (ARCADE_RATE * BLOCK_SAMPLES)) + 2


def kernel_table():
    """The resampler's kernel: Lanczos, radius 16, sampled every 1/512 (two spare entries for the interpolation)."""
    tab = []
    for i in range(KERNEL_RADIUS * KERNEL_OVERSAMPLE + 2):
        x = i / KERNEL_OVERSAMPLE
        if i == 0:
            tab.append(1.0)
        elif x >= KERNEL_RADIUS:
            tab.append(0.0)
        else:
            px = math.pi * x
            tab.append(math.sin(px) / px * math.sin(px / KERNEL_RADIUS) / (px / KERNEL_RADIUS))
    return tab


def kernel_at(tab, x):
    x = abs(x)
    if x >= KERNEL_RADIUS:
        return 0.0
    t = x * KERNEL_OVERSAMPLE
    i = int(t)
    f = t - i
    return tab[i] * (1.0 - f) + tab[i + 1] * f


def resample_table(src, rho, k0, count, tab):
    """Output samples k0 .. k0 + count - 1 of src (numbers; zero outside) read at positions rho * k, band-limited when rho > 1: the
    arithmetic of vag.c, step for step."""
    n = len(src)
    cut = 1.0 / rho if rho > 1.0 else 1.0
    half = int(math.ceil(KERNEL_RADIUS / cut))
    out = []
    for k in range(k0, k0 + count):
        p = rho * k
        i0 = int(math.floor(p))
        s = 0.0
        for j in range(i0 - half + 1, i0 + half + 1):
            if 0 <= j < n:
                s += src[j] * kernel_at(tab, (p - j) * cut) * cut
        out.append(s)
    return out


def vag_model(chip, start, length, rate, blocks, max_blocks=None, tab=None):
    """The sample the runtime builds: chip is the sample ROM in the chip's order as signed numbers, the sample its [start, start +
    length) (silence outside); block 0 zeros, blocks 1 .. blocks - 2 the closed-loop encode of 256 * the resampled data (rounded, clamped
    to 16 bits) with flags 4 on the first and 1 on the last, then the closing block (flags 7). With max_blocks, only the first blocks
    (flags as for a whole sample, but no closing block)."""
    if blocks < 4:
        raise ValueError('at least 4 blocks')
    tab = tab or kernel_table()
    src = chip[start:start + length]
    rho = ARCADE_RATE / rate
    out = bytearray(BLOCK_BYTES)
    h1 = h2 = 0
    for b in range(1, blocks - 1):
        if max_blocks is not None and b >= max_blocks:
            return bytes(out)
        vals = resample_table(src, rho, (b - 1) * BLOCK_SAMPLES, BLOCK_SAMPLES, tab)
        x = [max(-32768, min(32767, round(256.0 * v))) for v in vals]
        blk, h1, h2, _ = encode_block(x, h1, h2)
        out += blk
        if b == 1:
            out[BLOCK_BYTES + 1] |= 4
    out[(blocks - 2) * BLOCK_BYTES + 1] |= 1
    out += bytes([0, 7]) + bytes([0x77] * 14)
    return bytes(out)


def chip_signed(rom):
    """The sample ROM (the two members one after the other) as signed numbers in the order the chip reads them (byte pairs swapped)."""
    return [b - 256 if b > 127 else b for b in bytes(rom[i ^ 1] for i in range(len(rom)))]


def read_sound(arcade_path):
    """The Z80 program as the two members one after the other."""
    with zipfile.ZipFile(arcade_path) as z:
        return z.read('sz2.01a') + z.read('sz2.02a')


# ---------------------------------------------------------------- matches kept between runs

def matches_text(samples, found, decoded):
    """One line per distinct sample: index, md5, decoded length, then rate, address, quality tag, cover and snr or '-' (no source)."""
    lines = []
    for i, s in enumerate(samples):
        n = len(decoded[i])
        if i in found:
            rho, org, gain, snr, state = found[i]
            share, cls = state_summary(state)
            addr = start_address(org, rho)
            lines.append(f'{i}\t{s["key"].hex()}\t{n}\t{round(ARCADE_RATE / rho)}\t{round(addr)}\t{int(abs(addr - round(addr)) < 0.3)}\t'
                         f'{round(share * 100)}\t{cls}\t{snr:.1f}')
        else:
            lines.append(f'{i}\t{s["key"].hex()}\t{n}\t-\t-\t-\t-\tnone\t-')
    return '\n'.join(lines) + '\n'


def parse_matches(text):
    """{md5 hex: dict(rate, address, exact, cover, cls, snr) or None, with 'length' the decoded sample count}."""
    out = {}
    for ln in text.splitlines():
        p = ln.split('\t')
        if len(p) != 9:
            raise FormatError('bad matches line: ' + ln[:60])
        if p[3] == '-':
            out[p[1]] = dict(length=int(p[2]), rate=None)
        else:
            out[p[1]] = dict(length=int(p[2]), rate=int(p[3]), address=int(p[4]), exact=p[5] == '1', cover=int(p[6]), cls=p[7],
                             snr=float(p[8]))
    return out


def sample_plan(banks, matches, instruments):
    """For each distinct sample of the banks (the order of distinct_samples): dict(key, blocks (the disc's), rate, instrument (the
    record whose start is the sample's start, or None), builds (it is built from the arcade: a record start and a cover of at least
    50%), rule (the rule's block count equals the disc's), users)."""
    by_start = {r['start']: r for r in instruments}
    plan = []
    for s in distinct_samples(banks):
        m = matches.get(s['key'].hex())
        blocks = len(s['adpcm']) // BLOCK_BYTES
        row = dict(key=s['key'], blocks=blocks, rate=None, instrument=None, builds=False, rule=False, users=s['users'], cls='none')
        if m and m['rate']:
            row['rate'], row['cls'] = m['rate'], m['cls']
            rec = by_start.get(m['address'])
            if rec is not None and m['exact']:
                row['instrument'] = rec['index']
                row['builds'] = m['cls'] in ('full', '80-95', '50-80')
                row['rule'] = vag_blocks(rec['end'] - rec['start'] + 1, m['rate']) == blocks
        plan.append(row)
    return plan


# ---------------------------------------------------------------- the C helper

C_SOURCE = r'''
/* Compiled at run time by port/arcade/samples.py. Two modes.
 *   core search ROM PCM IDX JOBS   one line per (job, block): "B sample rho block ncc position rms"
 *   core verify ROM PCM IDX CANDS [STEPS]  one line per candidate: "L sample rho origin ncc gain snr STATE"
 * ROM: the raw sample ROM (the two members one after the other); the chip's view swaps each byte pair.
 * PCM: every PS1 sample as int16, one after the other; IDX: "offset count" per sample (in int16 units).
 * JOBS: "sample rho". CANDS: "sample rho origin block". rho = arcade samples per PS1 sample. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#define D 16          /* width of the box filter that makes the coarse view (arcade samples) */
#define ST 2          /* step of the coarse view */
#define LB 1024       /* the block of the coarse search, in arcade samples */
#define T (LB / ST)
#define KA 16         /* Lanczos radius */
#define OVS 512
#define BV 196        /* verification block, in PS1 samples (7 ADPCM blocks) */
typedef struct { long off; int n; } Sam;
static signed char *chip;      /* the chip's view of the ROM */
static long nchip;
static float *ktab;

static void *must(void *p) { if (!p) { fprintf(stderr, "out of memory\n"); exit(2); } return p; }
static void mk(void) {
    ktab = must(malloc((KA * OVS + 2) * sizeof(float)));
    for (int i = 0; i <= KA * OVS + 1; i++) {
        double x = (double)i / OVS;
        ktab[i] = x == 0 ? 1 : (x >= KA ? 0 : sin(M_PI * x) / (M_PI * x) * sin(M_PI * x / KA) / (M_PI * x / KA));
    }
}
static inline float kern(double x) {
    x = fabs(x); if (x >= KA) return 0;
    double t = x * OVS; int i = (int)t; float f = (float)(t - i);
    return ktab[i] * (1 - f) + ktab[i + 1] * f;
}
static inline double samp(double p, double c) {
    long i0 = (long)floor(p); int hw = (int)ceil(KA / c); double s = 0;
    for (long j = i0 - hw + 1; j <= i0 + hw; j++) {
        if (j < 0 || j >= nchip) continue;
        s += chip[j] * kern((p - j) * c) * c;
    }
    return s;
}
static void resamp(double p0, double rho, int n, float *out) {
    double c = rho > 1 ? 1 / rho : 1;
    for (int k = 0; k < n; k++) out[k] = (float)samp(p0 + k * rho, c);
}
static double ncc(const float *a, const float *b, int n, double *g) {
    double sa = 0, sb = 0, saa = 0, sbb = 0, sab = 0;
    for (int i = 0; i < n; i++) { sa += a[i]; sb += b[i]; saa += a[i] * a[i]; sbb += b[i] * b[i]; sab += a[i] * b[i]; }
    double va = saa - sa * sa / n, vb = sbb - sb * sb / n, cv = sab - sa * sb / n;
    if (g) *g = vb > 0 ? cv / vb : 0;
    if (va <= 0 || vb <= 0) return 0;
    return cv / sqrt(va * vb);
}
static uint8_t *steps;      /* shift of each ADPCM block of each sample (optional) */
static long fsize(FILE *f) { fseek(f, 0, SEEK_END); long n = ftell(f); rewind(f); return n; }

int main(int argc, char **argv) {
    if (argc < 6) { fprintf(stderr, "usage: core search|verify ROM PCM IDX LIST [STEPS]\n"); return 2; }
    int verify = !strcmp(argv[1], "verify");
    FILE *f = fopen(argv[2], "rb"); if (!f) return 2;
    long n = fsize(f); signed char *raw = must(malloc(n)); if (fread(raw, 1, n, f) != (size_t)n) return 2; fclose(f);
    nchip = n; chip = must(malloc(n));
    for (long i = 0; i < n; i++) chip[i] = raw[(i ^ 1) < n ? (i ^ 1) : i];
    mk();
    f = fopen(argv[3], "rb"); if (!f) return 2;
    long pn = fsize(f) / 2; int16_t *pcm = must(malloc(pn * 2 + 2)); if (fread(pcm, 2, pn, f) != (size_t)pn) return 2; fclose(f);
    f = fopen(argv[4], "r"); if (!f) return 2;
    Sam *ss = must(malloc(sizeof(Sam) * 4096)); int ns = 0; long o; int l;
    while (ns < 4096 && fscanf(f, "%ld %d", &o, &l) == 2) { ss[ns].off = o; ss[ns].n = l; ns++; }
    fclose(f);
    if (argc > 6) { f = fopen(argv[6], "rb"); if (f) { long sn = fsize(f); steps = must(malloc(sn + 1)); if (fread(steps, 1, sn, f) != (size_t)sn) return 2; fclose(f); } }
    f = fopen(argv[5], "r"); if (!f) return 2;
    int cap = 200000, nj = 0; int *js = must(malloc(cap * sizeof(int))), *jb = must(malloc(cap * sizeof(int)));
    double *jr = must(malloc(cap * sizeof(double))), *jo = must(malloc(cap * sizeof(double)));
    if (verify) { while (nj < cap && fscanf(f, "%d %lf %lf %d", &js[nj], &jr[nj], &jo[nj], &jb[nj]) == 4) nj++; }
    else { while (nj < cap && fscanf(f, "%d %lf", &js[nj], &jr[nj]) == 2) nj++; }
    fclose(f);

    if (!verify) {
        long nd = (n - D) / ST; if (nd < T + 1) return 0;
        float *rd = must(malloc(nd * 4));
        for (long j = 0; j < nd; j++) { int a = 0; for (int k = 0; k < D; k++) a += chip[j * ST + k]; rd[j] = a / (float)D; }
        double *p1 = must(malloc((nd + 1) * 8)), *p2 = must(malloc((nd + 1) * 8)); p1[0] = p2[0] = 0;
        for (long j = 0; j < nd; j++) { p1[j + 1] = p1[j] + rd[j]; p2[j + 1] = p2[j] + (double)rd[j] * rd[j]; }
        #pragma omp parallel for schedule(dynamic)
        for (int q = 0; q < nj; q++) {
            int si = js[q]; double rho = jr[q]; int16_t *x = pcm + ss[si].off; int len = ss[si].n;
            double *C = must(malloc((len + 1) * 8)); C[0] = 0;
            for (int i = 0; i < len; i++) C[i + 1] = C[i] + x[i];
            int nblk = (int)(len * rho / LB);
            char *out = must(malloc(nblk * 120 + 16)); out[0] = 0; int ol = 0;
            for (int b = 0; b < nblk; b++) {
                double a0 = (double)b * LB; float t[T]; double m = 0, te = 0;
                for (int j = 0; j < T; j++) {
                    double a = (a0 + (double)j * ST) / rho, bb = (a0 + (double)j * ST + D) / rho;
                    double ca, cb;
                    { double u = a < 0 ? 0 : a > len ? len : a; long i = (long)u; double fr = u - i; ca = i >= len ? C[len] : C[i] * (1 - fr) + C[i + 1] * fr; }
                    { double u = bb < 0 ? 0 : bb > len ? len : bb; long i = (long)u; double fr = u - i; cb = i >= len ? C[len] : C[i] * (1 - fr) + C[i + 1] * fr; }
                    t[j] = (float)((cb - ca) * rho / D); m += t[j];
                }
                m /= T; for (int j = 0; j < T; j++) { t[j] -= (float)m; te += (double)t[j] * t[j]; }
                double rms = sqrt(te / T), bn = -2; long bo = 0;
                if (rms > 200) for (long o2 = 0; o2 + T < nd; o2++) {
                    float d = 0; const float *qq = rd + o2;
                    for (int j = 0; j < T; j++) d += t[j] * qq[j];
                    double sx = p1[o2 + T] - p1[o2], sxx = p2[o2 + T] - p2[o2], var = sxx - sx * sx / T;
                    if (var > T) { double c = d / sqrt(var * te); if (c > bn) { bn = c; bo = o2; } }
                }
                ol += sprintf(out + ol, "B %d %.6f %d %.3f %ld %.0f\n", si, rho, b, bn, bo * ST, rms);
            }
            #pragma omp critical
            fputs(out, stdout);
            free(out); free(C);
        }
        return 0;
    }
    #pragma omp parallel for schedule(dynamic)
    for (int q = 0; q < nj; q++) {
        int si = js[q]; int16_t *x = pcm + ss[si].off; int len = ss[si].n; double rho = jr[q];
        double ka = jb[q] * (double)LB / rho; int ks = (int)ka; if (ks > len - 400) ks = len - 400; if (ks < 0) ks = 0;
        int kl = len - ks < 1400 ? len - ks : 1400;
        float *t = must(malloc(kl * 4)), *cd = must(malloc(kl * 4));
        for (int k = 0; k < kl; k++) t[k] = x[ks + k];
        double bo = jo[q], bn = -2;
        for (double d = -4; d <= 4.001; d += 0.1) { resamp(jo[q] + d + rho * ks, rho, kl, cd); double c = ncc(t, cd, kl, NULL); if (c > bn) { bn = c; bo = jo[q] + d; } }
        for (double d = -0.1; d <= 0.1001; d += 0.02) { resamp(bo + d + rho * ks, rho, kl, cd); double c = ncc(t, cd, kl, NULL); if (c > bn + 1e-9) { bn = c; bo = bo + d; } }
        free(t); free(cd);
        int nb = (len + BV - 1) / BV; char *st = must(malloc(nb + 1)); st[nb] = 0;
        float *full = must(malloc(len * 4)); resamp(bo, rho, len, full);
        float *xf = must(malloc(len * 4)); for (int k = 0; k < len; k++) xf[k] = x[k];
        double *bc = must(malloc(nb * 8)), *bg = must(malloc(nb * 8)), *bs = must(malloc(nb * 8)), *bq = must(malloc(nb * 8));
        double sxy = 0, syy = 0;
        for (int bi = 0; bi < nb; bi++) {
            int bl = BV; if ((bi + 1) * BV > len) bl = len - bi * BV;
            bc[bi] = ncc(xf + bi * BV, full + bi * BV, bl, &bg[bi]);
            double e = 0, nz = 0;
            for (int k = 0; k < bl; k++) e += (double)x[bi * BV + k] * x[bi * BV + k];
            bs[bi] = e;
            for (int k = 0; k < bl; k += 28) {
                int sh = steps ? steps[ss[si].off / 28 + (bi * BV + k) / 28] : 0; if (sh > 12) sh = 9;
                double step = 4096 >> sh; nz += 28 * step * step / 12.0;
            }
            bq[bi] = nz;
            if (bc[bi] > 0.85 && bg[bi] > 100 && bg[bi] < 600) {
                for (int k = 0; k < bl; k++) { sxy += (double)x[bi * BV + k] * full[bi * BV + k]; syy += (double)full[bi * BV + k] * full[bi * BV + k]; }
            }
        }
        double gain = syy > 0 ? sxy / syy : 256;
        double sig = 0, err = 0;
        for (int bi = 0; bi < nb; bi++) {
            int bl = BV; if ((bi + 1) * BV > len) bl = len - bi * BV;
            double e = 0;
            for (int k = 0; k < bl; k++) { double d = x[bi * BV + k] - gain * full[bi * BV + k]; e += d * d; }
            if (bs[bi] < 6 * bq[bi]) st[bi] = 'q';                 /* signal too near the ADPCM noise floor to tell */
            else if (bc[bi] > 0.85 && bg[bi] > 100 && bg[bi] < 600) st[bi] = 'M';
            else if (e < 0.8 * bs[bi] && e < 12 * bq[bi]) st[bi] = 'n';   /* explains the block, within the ADPCM noise */
            else st[bi] = '.';
            if (st[bi] == 'M' || st[bi] == 'n') { sig += bs[bi]; err += e; }
        }
        double snr = sig > 0 && err > 0 ? 10 * log10(sig / err) : 0;
        #pragma omp critical
        printf("L %d %.6f %.3f %.4f %.2f %.1f %s\n", si, rho, bo, bn, gain, snr, st);
        free(st); free(full); free(xf); free(bc); free(bg); free(bs); free(bq);
    }
    return 0;
}
'''


def compile_core(workdir):
    src = Path(workdir) / 'samples_core.c'
    src.write_text(C_SOURCE)
    exe = Path(workdir) / 'samples_core'
    for flags in (['-fopenmp'], []):
        r = subprocess.run(['gcc', '-O3', '-march=native', *flags, '-o', str(exe), str(src), '-lm'],
                           capture_output=True, text=True)
        if r.returncode == 0:
            return exe
    raise RuntimeError('cannot compile the helper: ' + r.stderr[-400:])


# ---------------------------------------------------------------- inputs

def iso_files(f):
    """{path: (lba, size)} of the disc's ISO 9660 files (raw 2352-byte sectors, data at +24), and the reader."""
    def sec(lba, n=1):
        out = b''
        for i in range(n):
            f.seek((lba + i) * 2352 + 24)
            out += f.read(2048)
        return out
    files = {}

    def walk(lba, size, path):
        d, i = sec(lba, (size + 2047) // 2048), 0
        while i < len(d):
            ln = d[i]
            if ln == 0:
                i = (i // 2048 + 1) * 2048
                continue
            e = d[i:i + ln]
            i += ln
            el, sz = struct.unpack('<I', e[2:6])[0], struct.unpack('<I', e[10:14])[0]
            nm = e[33:33 + e[32]]
            if nm in (b'\0', b'\1'):
                continue
            p = path + '/' + nm.decode('latin1').split(';')[0]
            if e[25] & 2:
                walk(el, sz, p)
            else:
                files[p] = (el, sz)
    root = sec(16)[156:156 + 34]
    walk(struct.unpack('<I', root[2:6])[0], struct.unpack('<I', root[10:14])[0], '')
    return files, sec


def read_banks(disc_path, only=None):
    """[dict(file, slot, vh info, samples=[bytes])] of every VAB (table 3 slot s with table 4 slot s) on the disc."""
    banks = []
    with open(disc_path, 'rb') as f:
        files, sec = iso_files(f)
        for path in sorted(files):
            if not path.upper().endswith('.PAC'):
                continue
            name = path.rsplit('/', 1)[1][:-4]
            if only and only.upper() not in name.upper():
                continue
            lba, size = files[path]
            data = sec(lba, (size + 2047) // 2048)[:size]
            try:
                chunks = pac_chunks(data)
            except FormatError:
                continue
            vhs = {s: c for t, s, c in chunks if t == 3 and c[:4] == b'pBAV'}
            vbs = {s: c for t, s, c in chunks if t == 4}
            for slot in sorted(vhs):
                if slot in vbs:
                    info = parse_vh(vhs[slot])
                    banks.append(dict(file=name, slot=slot, info=info, samples=vag_slices(vbs[slot], info['sizes']), vh=vhs[slot], vb=vbs[slot]))
    return banks


def read_rom(arcade_path):
    """The sample ROM as the two members one after the other."""
    with zipfile.ZipFile(arcade_path) as z:
        return z.read('sz2.11m') + z.read('sz2.12m')


def distinct_samples(banks):
    """[dict(key, adpcm, users=[(file, slot, vag)], tones)] one per distinct sample body."""
    seen, out = {}, []
    for b in banks:
        for v, adpcm in enumerate(b['samples'], 1):
            key = hashlib.md5(adpcm).digest()
            tones = [t for t in b['info']['tones'] if t['vag'] == v]
            if key not in seen:
                seen[key] = dict(key=key, adpcm=adpcm, users=[], tones=tones)
                out.append(seen[key])
            seen[key]['users'].append((b['file'], b['slot'], v))
    return out


# ---------------------------------------------------------------- the search

def run_core(exe, mode, rom_path, pcm_path, idx_path, jobs, workdir):
    lst = Path(workdir) / f'{mode}.txt'
    lst.write_text(''.join(jobs))
    steps = Path(workdir) / 'steps.bin'
    r = subprocess.run([str(exe), mode, rom_path, pcm_path, idx_path, str(lst), *([str(steps)] if steps.exists() else [])],
                       capture_output=True, text=True)
    if r.returncode:
        raise RuntimeError(f'helper {mode} failed: {r.stderr[-300:]}')
    return r.stdout.splitlines()


def find_lines(exe, samples, rom_path, pcm_path, idx_path, rates_for, workdir):
    """For each sample index, the best line (rho, origin, gain, snr, state) or None, searching the rates rates_for(i)."""
    jobs = [f'{i} {ARCADE_RATE / r:.6f}\n' for i in range(len(samples)) for r in rates_for(i)]
    hits = []
    for ln in run_core(exe, 'search', rom_path, pcm_path, idx_path, jobs, workdir):
        p = ln.split()
        hits.append((int(p[1]), float(p[2]), int(p[3]), float(p[4]), int(p[5])))
    cands = cluster_hits(hits)
    best = {}
    out = run_core(exe, 'verify', rom_path, pcm_path, idx_path,
                   [f'{s} {rho:.6f} {org} {b}\n' for s, rho, org, b in cands], workdir)
    for ln in out:
        p = ln.split()
        s, rho, org, ncc, gain, snr, state = int(p[1]), float(p[2]), float(p[3]), float(p[4]), float(p[5]), float(p[6]), p[7]
        if not acceptable_line(state):
            continue
        score = sum(1 for c in state[1:-1] if c in 'Mn')
        if s not in best or score > best[s][0]:
            best[s] = (score, rho, org, gain, snr, state)
    return {s: v[1:] for s, v in best.items()}


def stage_inputs(work, rom, samples):
    """Write the helper's inputs into work: the ROM, every sample decoded (int16), the index and the shift of each
    ADPCM block. Returns (rom_path, pcm_path, idx_path, [decoded pcm per sample])."""
    rom_path, pcm_path, idx_path = (str(Path(work) / n) for n in ('rom.bin', 'pcm.bin', 'idx.txt'))
    Path(rom_path).write_bytes(rom)
    pcm_all, idx, steps, off, dec = bytearray(), [], bytearray(), 0, []
    for smp in samples:
        pcm, _ = decode_adpcm(smp['adpcm'])
        dec.append(pcm)
        pcm_all += struct.pack(f'<{len(pcm)}h', *pcm)
        idx.append(f'{off} {len(pcm)}\n')
        steps += bytes(smp['adpcm'][i] & 15 for i in range(0, len(smp['adpcm']) - 15, 16))
        off += len(pcm)
    Path(pcm_path).write_bytes(bytes(pcm_all))
    Path(idx_path).write_text(''.join(idx))
    (Path(work) / 'steps.bin').write_bytes(bytes(steps))
    return rom_path, pcm_path, idx_path, dec


def match(banks, arcade_path, detail=False, save=None, quiet=False):
    t0 = time.time()
    rom = read_rom(arcade_path)
    samples = distinct_samples(banks)
    work = tempfile.mkdtemp(prefix='samples-')
    try:
        exe = compile_core(work)
        rom_path, pcm_path, idx_path, decoded = stage_inputs(work, rom, samples)
        found = find_lines(exe, samples, rom_path, pcm_path, idx_path, lambda i: candidate_rates(samples[i]['tones']), work)
        retry = [i for i in range(len(samples)) if i not in found]
        rest = lambda i: [r for r in NOMINAL_RATES if r not in candidate_rates(samples[i]['tones'])]
        if retry:
            more = find_lines(exe, samples, rom_path, pcm_path, idx_path,
                              lambda i: rest(i) if i in retry else [], work)
            found.update(more)
        if save:
            Path(save).write_text(matches_text(samples, found, decoded))
        if quiet:
            return
        by_class = collections.Counter()
        rates = collections.Counter()
        addresses = collections.defaultdict(set)
        first_of = {}
        for i, s in enumerate(samples):
            n = len(decoded[i])
            f, sl, v = s['users'][0]
            also = len(s['users']) - 1
            head = f'{f:<10} s{sl} v{v:<2} n={n:<6}' + (f' +{also}' if also else '')
            if i in found:
                rho, org, gain, snr, state = found[i]
                share, cls = state_summary(state)
                addr = start_address(org, rho)
                rate = ARCADE_RATE / rho
                rates[round(rate)] += 1
                addresses[round(addr)].add(i)
                frac = abs(addr - round(addr))
                print(f'{head} R={rate:7.0f} A=0x{round(addr):06x}{"" if frac < 0.3 else "?"} cover={share * 100:3.0f}% '
                      f'gain={gain:5.1f} snr={snr:4.1f} {cls}')
                by_class[cls] += 1
                if detail and cls not in first_of:
                    first_of[cls] = (head, state)
            else:
                print(f'{head} no arcade source found')
                by_class['none'] += 1
        for cls, (head, state) in first_of.items():
            print(f'  first of "{cls}": {head.strip()} blocks {state}')
        total = sum(by_class.values())
        print(f'{total} distinct samples ({sum(len(s["users"]) for s in samples)} in the banks): ' +
              ', '.join(f'{k} {by_class[k]}' for k in ('full', '80-95', '50-80', 'under 50', 'none')))
        print('rates: ' + ' '.join(f'{r}:{c}' for r, c in sorted(rates.items())))
        print(f'{len(addresses)} distinct arcade start addresses; {time.time() - t0:.0f} s')
    finally:
        shutil.rmtree(work, ignore_errors=True)


def encode_test(banks, arcade_path, limit):
    rom = read_rom(arcade_path)
    chip = [b - 256 if b > 127 else b for b in bytes(rom[i ^ 1] for i in range(len(rom)))]
    samples = distinct_samples(banks)
    work = tempfile.mkdtemp(prefix='samples-')
    try:
        exe = compile_core(work)
        rom_path, pcm_path, idx_path, dec = stage_inputs(work, rom, samples)
        found = find_lines(exe, samples, rom_path, pcm_path, idx_path, lambda i: candidate_rates(samples[i]['tones']), work)
    finally:
        shutil.rmtree(work, ignore_errors=True)
    shown, mine, theirs = 0, [], []
    for i in sorted(found):
        rho, org, gain, snr, state = found[i]
        runs = [j for j, c in enumerate(state) if c == 'M' and 4 <= j < len(state) - 2][:20]
        if len(runs) < 5:
            continue
        k_idx = [k for j in runs for k in range(j * 196, (j + 1) * 196)]
        a0 = start_address(org, rho)
        ref = [256 * resample_lanczos(chip, a0 + rho * (k - 28), rho, 1)[0] for k in k_idx]
        ps = [dec[i][k] for k in k_idx]
        # encode contiguous runs on their own
        enc_all, p = [], 0
        while p < len(k_idx):
            q = p
            while q + 1 < len(k_idx) and k_idx[q + 1] == k_idx[q] + 1:
                q += 1
            enc_all += decode_adpcm(encode_adpcm([round(v) for v in ref[p:q + 1]]))[0][:q + 1 - p]
            p = q + 1
        f, sl, v = samples[i]['users'][0]
        a, b = snr_db(ref, enc_all), snr_db(ref, ps)
        mine.append(a)
        theirs.append(b)
        print(f'{f:<10} s{sl} v{v:<2} R={ARCADE_RATE / rho:6.0f} encode-vs-arcade {a:5.1f} dB  ps1-vs-arcade {b:5.1f} dB  '
              f'encode-vs-ps1 {snr_db(ps, enc_all):5.1f} dB')
        shown += 1
        if shown >= limit:
            break
    if shown:
        print(f'{shown} samples: mean encode-vs-arcade {sum(mine) / shown:.1f} dB, mean ps1-vs-arcade {sum(theirs) / shown:.1f} dB')


def inventory(banks):
    files = collections.Counter()
    shapes = collections.Counter()
    distinct = {}
    for b in banks:
        files[b['file']] += 1
        for adpcm in b['samples']:
            distinct.setdefault(hashlib.md5(adpcm).digest(), adpcm)
            shapes[flag_structure(decode_adpcm(adpcm)[1])] += 1
        print(f'{b["file"]:<10} slot {b["slot"]}: {len(b["samples"]):2d} samples, {sum(b["info"]["sizes"]):7d} bytes, '
              f'{len(b["info"]["tones"])} tones')
    print(f'{len(banks)} banks, {sum(len(b["samples"]) for b in banks)} samples, {len(distinct)} distinct; '
          + ', '.join(f'{k} {v}' for k, v in shapes.items()))


# ---------------------------------------------------------------- the VAB headers

def vh_checks(vh, vb, plan_by_key, banks_sample_keys):
    """Per-field facts of one VAB header against the rules. Returns a Counter of 'name': hits and 'name/n': tries (so the report can show
    hits of tries). vh, vb: the header and the body; banks_sample_keys: the md5 of each of the bank's samples (VAG 1 ..)."""
    c = collections.Counter()

    def check(name, ok):
        c[name + '/n'] += 1
        c[name] += 1 if ok else 0
    ps, ts, vs = struct.unpack_from('<HHH', vh, 0x12)
    check('layout: header length = 0x20 + 2048 + 512 * programs + 512', len(vh) == 0x20 + 2048 + 512 * ps + 512)
    ver, vid, fsize, rsv = struct.unpack_from('<IIIH', vh, 4)
    check('header: version 7, id 0, reserved 0xeeee', (ver, vid, rsv) == (7, 0, 0xeeee))
    check('header: size field = header + body', fsize == len(vh) + len(vb))
    check('header: master volume 127, pan 64, attributes 0, reserved ffffffff', vh[24:32] == bytes([127, 64, 0, 0, 255, 255, 255, 255]))
    tone_total = 0
    for p in range(ps):
        r = vh[0x20 + 16 * p:0x30 + 16 * p]
        tone_total += r[0]
        check('program: volume 127, pan 64, attribute 0, reserved ffffffffffffffff', (r[1], r[4], r[5], r[6], r[7]) == (127, 64, 0, 0, 0)
              and r[8:] == bytes([255] * 8))
    check('header: tone count = sum of the programs\' tone counts', ts == tone_total)
    base = 0x20 + 128 * 16
    for i in range(ps * 16):
        r = vh[base + 32 * i:base + 32 * i + 32]
        adsr1, adsr2, prog, vag = struct.unpack_from('<HHhh', r, 16)
        if vag <= 0:
            check('unused tone slot: first 14 bytes zero (reserved bytes and adsr as in a used tone)', r[:14] == bytes(14))
            continue
        key = banks_sample_keys[vag - 1]
        row = plan_by_key[key]
        check('tone: prior 0, mode 0, vibrato/portamento/pitch-bend 0', r[0] == 0 and r[1] == 0 and r[8:14] == bytes(6))
        check('tone: volume 127, pan 64', r[2] == 127 and r[3] == 64)
        check('tone: reserved bytes 0xb1, 0xb2 and the four reserved words 0xc0 .. 0xc3', r[14:16] == b'\xb1\xb2'
              and r[24:32] == bytes([0xc0, 0, 0xc1, 0, 0xc2, 0, 0xc3, 0]))
        check('tone: parent program = its program', prog == i // 16)
        check('tone: attack/decay/sustain level adsr1 = 0x80ff', adsr1 == 0x80ff)
        check('tone: release/sustain adsr2 = 0x1fc0 (the one-shot default)', adsr2 == 0x1fc0)
        check('tone: key range 0-0 (played by tone number, not by key)', (r[6], r[7]) == (0, 0))
        check('tone: VAG number = tone number + 1', vag == i % 16 + 1)
        if row['rate']:
            rt = RATE_TONE.get(row['rate'])
            if rt is not None:
                check('tone: (center, shift) = the rate\'s pair, rate from the matched sample', (r[4], r[5]) == rt)
            else:
                c['tone: sample rate with no pair in RATE_TONE (music instruments)'] += 1
        else:
            c['tone: sample with no arcade source (rate unknown)'] += 1
    tab = base + ps * 16 * 32
    for v in range(vs):
        row = plan_by_key[banks_sample_keys[v]]
        check('VAG size = the disc\'s block count * 2 (it is the sample\'s size)', struct.unpack_from('<H', vh, tab + 2 * (v + 1))[0] == 2 * row['blocks'])
        if row['instrument'] is not None:
            check('VAG size = the arcade rule\'s block count * 2 (record length, rate)', row['rule'])
    return c


def rebuild_vh(vh, vb, plan_by_key, keys):
    """The VAB header rebuilt from the rules and the arcade, taking from `vh` only what is the PS1's own (the 'recipe': each program's tone
    count, priority and mode; per tone its VAG number when it is not the tone number + 1, key range when not 0-0, envelope when not
    0x80ff.1fc0, and (center, shift) when the sample's rate is unknown or not that rate's pair). Returns (bytes, Counter of the recipe
    items). Sizes come from the plan's block counts (the disc's; the arcade rule's where it holds)."""
    ps, ts, vs = struct.unpack_from('<HHH', vh, 0x12)
    out = bytearray(len(vh))
    recipe = collections.Counter()
    out[0:4] = b'pBAV'
    struct.pack_into('<IIIH', out, 4, 7, 0, len(vh) + len(vb), 0xeeee)
    struct.pack_into('<HHH', out, 0x12, ps, ts, vs)
    out[24:32] = bytes([127, 64, 0, 0, 255, 255, 255, 255])
    stale = vh[0x20 + 16 * 127 + 2:0x20 + 16 * 127 + 4]      # the priority and mode that every unused program record of a header repeats
    recipe['unused program records: stale priority/mode'] += 1
    for p in range(128):
        out[0x20 + 16 * p:0x30 + 16 * p] = bytes([0, 0]) + stale + bytes(4) + bytes([255] * 8)     # an unused program record
    for p in range(128):
        r = vh[0x20 + 16 * p:0x30 + 16 * p]
        if r[0]:
            out[0x20 + 16 * p:0x30 + 16 * p] = bytes([r[0], 127, r[2], r[3], 64, 0, 0, 0]) + bytes([255] * 8)
            recipe['program: tone count, priority, mode'] += 1
    base = 0x20 + 128 * 16
    for slot in range(ps * 16):
        rec = base + 32 * slot
        a = vh[rec:rec + 32]
        out[rec + 14:rec + 16] = b'\xb1\xb2'
        out[rec + 24:rec + 32] = bytes([0xc0, 0, 0xc1, 0, 0xc2, 0, 0xc3, 0])
        adsr1, adsr2, _, vag = struct.unpack_from('<HHhh', a, 16)
        if vag <= 0:
            struct.pack_into('<HHh', out, rec + 16, adsr1, adsr2, slot // 16)
            recipe['unused slot: envelope words'] += int((adsr1, adsr2) != (0x80ff, 0x5fc0))
            continue
        row = plan_by_key[keys[vag - 1]]
        rt = RATE_TONE.get(row['rate']) if row['rate'] else None
        if rt == (a[4], a[5]):
            center, shift = rt
        else:
            center, shift = a[4], a[5]
            recipe['tone: (center, shift)'] += 1
        out[rec:rec + 14] = bytes([0, 0, a[2], a[3], center, shift, a[6], a[7], 0, 0, 0, 0, 0, 0])
        recipe['tone: volume or pan not 127/64'] += int((a[2], a[3]) != (127, 64))
        recipe['tone: key range'] += int((a[6], a[7]) != (0, 0))
        if (adsr1, adsr2) != (0x80ff, 0x1fc0):
            recipe['tone: envelope'] += 1
        recipe['tone: VAG number'] += int(vag != slot % 16 + 1)
        struct.pack_into('<HHhh', out, rec + 16, adsr1, adsr2, slot // 16, vag)
        recipe['tones'] += 1
    tab = base + ps * 16 * 32
    for v in range(vs):
        struct.pack_into('<H', out, tab + 2 * (v + 1), 2 * plan_by_key[keys[v]]['blocks'])
    return bytes(out), recipe


def vh_report(banks, plan):
    plan_by_key = {p['key']: p for p in plan}
    total = collections.Counter()
    distinct_vh = set()
    for b in banks:
        keys = [hashlib.md5(a).digest() for a in b['samples']]
        total.update(vh_checks(b['vh'], b['vb'], plan_by_key, keys))
        distinct_vh.add(hashlib.md5(b['vh']).digest())
    exact, recipe = 0, collections.Counter()
    first_bad = None
    for b in banks:
        keys = [hashlib.md5(a).digest() for a in b['samples']]
        built, rec = rebuild_vh(b['vh'], b['vb'], plan_by_key, keys)
        recipe.update(rec)
        if built == b['vh']:
            exact += 1
        elif first_bad is None:
            first_bad = (b['file'], [i for i in range(len(built)) if built[i] != b['vh'][i]][:6])
    print(f'rebuilt from the rules, the arcade and a recipe: {exact} of {len(banks)} headers equal byte for byte'
          + (f' (first difference: {first_bad[0]} at {first_bad[1]})' if first_bad else ''))
    print('  the recipe (the PS1\'s own, counted over all banks): ' + ', '.join(f'{k} {v}' for k, v in sorted(recipe.items())))
    names = sorted({k[:-2] for k in total if k.endswith('/n')})
    print(f'{len(banks)} banks, {len(distinct_vh)} distinct headers')
    for n in names:
        print(f'  {total[n]:5d} of {total[n + "/n"]:5d}  {n}')
    for k in sorted(k for k in total if not k.endswith('/n') and total[k] and k.startswith(('tone: sample', 'tone: no', 'tone: sample rate'))):
        print(f'  {total[k]:5d}        {k}')
    tones = [t for b in banks for t in b['info']['tones']]
    other = collections.Counter()
    for t in tones:
        other[(t['low'], t['high'])] += 1
    print(f'  key ranges other than 0-0 in {sum(v for k, v in other.items() if k != (0, 0))} of {len(tones)} tones '
          f'(0-127 catch-all {other[(0, 127)]})')
    adsr = collections.Counter((t['adsr1'], t['adsr2']) for t in tones)
    print(f'  {len(adsr)} distinct (adsr1, adsr2); the commonest: ' + ', '.join(f'{a:04x}.{b:04x} x{n}' for (a, b), n in adsr.most_common(5)))


# ---------------------------------------------------------------- C converter: driver, comparison and table

C_DRIVER = r'''
/* Compiled at run time by port/arcade/samples.py compare-c: builds samples through arcade_vag_build (vag.c with a generated table).
 * usage: drv CHIP SOUND JOBS OUT; JOBS: sample numbers; OUT: the samples one after the other. */
#include <stdio.h>
#include <stdlib.h>
#include "vag.h"
int main(int argc, char **argv) {
    FILE *f, *o; long n, m; unsigned char *chip, *snd; unsigned id; char err[200]; static unsigned char out[1 << 20];
    size_t size;
    if (argc < 5) return 2;
    f = fopen(argv[1], "rb"); if (!f) return 2;
    fseek(f, 0, SEEK_END); n = ftell(f); rewind(f);
    chip = malloc(n); if (!chip || fread(chip, 1, n, f) != (size_t)n) return 2; fclose(f);
    f = fopen(argv[2], "rb"); if (!f) return 2;
    fseek(f, 0, SEEK_END); m = ftell(f); rewind(f);
    snd = malloc(m); if (!snd || fread(snd, 1, m, f) != (size_t)m) return 2; fclose(f);
    f = fopen(argv[3], "r"); if (!f) return 2;
    o = fopen(argv[4], "wb"); if (!o) return 2;
    while (fscanf(f, "%u", &id) == 1) {
        if (arcade_vag_build(chip, (size_t)n, snd, (size_t)m, id, out, sizeof out, &size, err, sizeof err)) { fprintf(stderr, "%s\n", err); return 1; }
        fwrite(out, 1, size, o);
    }
    fclose(o);
    return 0;
}
'''


def chip_bytes(rom):
    """The chip's view of the sample ROM as bytes (the member pairs swapped)."""
    out = bytearray(len(rom))
    out[0::2], out[1::2] = rom[1::2], rom[0::2]
    return bytes(out)


def build_c(work, table_text):
    here = Path(__file__).resolve().parent
    drv = Path(work) / 'driver.c'
    drv.write_text(C_DRIVER)
    tbl = Path(work) / 'vag_table.c'
    tbl.write_text(table_text)
    exe = Path(work) / 'vag_driver'
    r = subprocess.run(['gcc', '-std=c99', '-O2', '-Wall', '-Wextra', '-Werror', '-I', str(here), '-o', str(exe), str(drv),
                        str(here / 'vag.c'), str(tbl), '-lm'], capture_output=True, text=True)
    if r.returncode:
        raise RuntimeError('cannot build vag.c: ' + r.stderr[-500:])
    return exe


def compare_c(banks, matches, arcade_path, model_blocks, only=None):
    """Build every sample the plan says is built, with vag.c: sizes against the disc's, the first model_blocks blocks against the Python
    model, and the whole decode against the disc's decode (SNR). One line per bank file with new samples."""
    rom = read_rom(arcade_path)
    sound = read_sound(arcade_path)
    instruments = parse_instruments(sound)
    plan = sample_plan(banks, matches, instruments)
    samples = distinct_samples(banks)
    chip = chip_signed(rom)
    tab = kernel_table()
    work = tempfile.mkdtemp(prefix='vag-')
    try:
        table = c_table(banks, plan)
        shipped = Path(__file__).resolve().parent / 'vag_table.c'
        print('vag_table.c in the tree ' + ('is the generated table' if shipped.exists() and shipped.read_text() == table else
                                           'DIFFERS from the generated table' if shipped.exists() else 'is missing'))
        exe = build_c(work, table)
        (Path(work) / 'chip.bin').write_bytes(chip_bytes(rom))
        (Path(work) / 'sound.bin').write_bytes(sound)
        todo = [(i, p) for i, p in enumerate(plan) if p['builds'] and (not only or only.upper() in p['users'][0][0].upper())]
        order = [(i, p, instruments[p['instrument']]) for i, p in todo]
        (Path(work) / 'jobs.txt').write_text(''.join(f'{i}\n' for i, p in todo))
        t0 = time.time()
        r = subprocess.run([str(exe), str(Path(work) / 'chip.bin'), str(Path(work) / 'sound.bin'), str(Path(work) / 'jobs.txt'),
                            str(Path(work) / 'out.bin')], capture_output=True, text=True)
        if r.returncode:
            raise RuntimeError('vag driver failed: ' + r.stderr[-300:])
        took_c = time.time() - t0
        out = (Path(work) / 'out.bin').read_bytes()
        pos, per = 0, collections.OrderedDict()
        t1 = time.time()
        for i, p, rec in order:
            ours = out[pos:pos + 16 * p['blocks']]
            pos += 16 * p['blocks']
            disc = samples[i]['adpcm']
            model = vag_model(chip, rec['start'], rec['end'] - rec['start'] + 1, p['rate'], p['blocks'], model_blocks, tab)
            c_dec, c_flags = decode_adpcm(ours)
            d_dec, _ = decode_adpcm(disc)
            snr = snr_db(d_dec[28:], c_dec[28:len(d_dec)])
            f, sl, _ = p['users'][0]
            row = per.setdefault((f, sl), collections.Counter())
            row['built'] += 1
            row['size'] += int(len(ours) == len(disc))
            row['model'] += int(ours[:len(model)] == model)
            row['flags'] += int(flag_structure(c_flags) == 'oneshot')
            row['rule'] += int(p['rule'])
            row['snr_n'] += 1
            row['snr_sum'] += snr
            row['snr_min'] = min(row['snr_min'], snr) if 'snr_min' in row else snr
        for (f, sl), row in per.items():
            n = row['built']
            print(f'{f:<9} s{sl}: {n:2d} built, size = disc {row["size"]:2d}/{n}, C = model (first {model_blocks} blocks) {row["model"]:2d}/{n}, '
                  f'flags {row["flags"]:2d}/{n}, rule count = disc {row["rule"]:2d}/{n}, decode vs disc SNR mean '
                  f'{row["snr_sum"] / n:4.1f} min {row["snr_min"]:4.1f} dB')
        tot = collections.Counter()
        for row in per.values():
            tot.update({k: v for k, v in row.items() if k not in ('snr_min',)})
        n = tot['built']
        print(f'{n} samples built of {len(plan)} distinct ({sum(1 for p in plan if p["instrument"] is not None)} at an instrument start, '
              f'{sum(1 for p in plan if p["rate"] is None)} with no arcade source): size = disc {tot["size"]}, C = model {tot["model"]}, '
              f'flags ok {tot["flags"]}; C {took_c:.1f} s, model + decode {time.time() - t1:.0f} s')
    finally:
        shutil.rmtree(work, ignore_errors=True)


def c_table(banks, plan):
    """The text of port/arcade/vag_table.c: for each distinct sample (instrument or none, rate, the disc's blocks, flags) and for each bank
    the samples of its VAGs; numbers only, no game bytes."""
    index = {p['key']: i for i, p in enumerate(plan)}
    out = ['/* GENERATED by `python3 port/arcade/samples.py c-table --disc DISC --arcade SET`; do not edit. For each distinct PS1 sound sample: the',
           ' * arcade instrument it starts at (index into the Z80 program\'s table, 0xffff when the arcade has no source for it or the match is',
           ' * not good enough), the rate the PS1 resampled it to, its size in blocks (the disc\'s) and flag bit 0 = that count is not the',
           ' * rule\'s (the PS1 cut or lengthened the sample). Then each bank file (disc PAC name, slot) with the sample numbers of its VAGs',
           ' * 1 .. count. Addresses, sizes and rates only. */',
           '#include "vag.h"', '', 'const struct arcade_vag_sample arcade_vag_samples[] = {']
    for p in plan:
        inst = p['instrument'] if p['builds'] else 0xffff
        out.append(f'    {{{inst}, {p["rate"] or 0}, {p["blocks"]}, {0 if p["rule"] or not p["builds"] else 1}}},')
    out += ['};', f'const unsigned arcade_vag_sample_count = {len(plan)};', '', 'static const uint16_t vag_ids[] = {']
    ids, rows = [], []
    for b in banks:
        keys = [hashlib.md5(a).digest() for a in b['samples']]
        rows.append((b['file'], b['slot'], len(ids), len(keys)))
        ids += [index[k] for k in keys]
    for i in range(0, len(ids), 16):
        out.append('    ' + ', '.join(str(v) for v in ids[i:i + 16]) + ',')
    out += ['};', '', 'const struct arcade_vag_bank arcade_vag_banks[] = {']
    out += [f'    {{"{f}", {sl}, {cnt}, {first}}},' for f, sl, first, cnt in rows]
    out += ['};', f'const unsigned arcade_vag_bank_count = {len(rows)};', '',
            'const uint16_t *arcade_vag_bank_ids(const struct arcade_vag_bank *bank) {', '    return vag_ids + bank->first;', '}', '']
    return '\n'.join(out)


def matches_for(banks, a):
    """The kept matches of --matches FILE, or a quiet run of the match (kept there when FILE names a place)."""
    if a.matches and Path(a.matches).exists():
        return parse_matches(Path(a.matches).read_text())
    if not a.arcade:
        raise SystemExit('--arcade is required when there are no kept matches')
    keep = a.matches or str(Path(tempfile.gettempdir()) / f'samples-matches-{os.getpid()}.tsv')
    match(banks, a.arcade, save=keep, quiet=True)
    out = parse_matches(Path(keep).read_text())
    if not a.matches:
        os.unlink(keep)
    return out


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('command', choices=('inventory', 'match', 'encode-test', 'vh', 'compare-c', 'c-table'))
    ap.add_argument('--disc', required=True)
    ap.add_argument('--arcade')
    ap.add_argument('--only', help='only the archives whose name contains this')
    ap.add_argument('--detail', action='store_true')
    ap.add_argument('--limit', type=int, default=8)
    ap.add_argument('--save', help='match: keep the matches in this file')
    ap.add_argument('--matches', help='vh, compare-c, c-table: the kept matches (written there when absent)')
    ap.add_argument('--model-blocks', type=int, default=8, help='compare-c: blocks of each sample checked against the Python model')
    a = ap.parse_args(argv)
    banks = read_banks(a.disc, a.only)
    if a.command == 'inventory':
        inventory(banks)
        return 0
    if not a.arcade:
        ap.error('--arcade is required')
    if a.command == 'match':
        match(banks, a.arcade, a.detail, a.save)
    elif a.command == 'encode-test':
        encode_test(banks, a.arcade, a.limit)
    else:
        matches = matches_for(banks, a)
        if a.command == 'compare-c':
            compare_c(banks, matches, a.arcade, a.model_blocks, a.only)
        else:
            plan = sample_plan(banks, matches, parse_instruments(read_sound(a.arcade)))
            if a.command == 'vh':
                vh_report(banks, plan)
            else:
                sys.stdout.write(c_table(banks, plan))
    return 0


if __name__ == '__main__':
    sys.exit(main())
