#!/usr/bin/env python3
"""Analysis tool: rebuild each fighter's PS1 animation steps and frame records from the arcade's sequence records.

What it does. A fighter's PS1 module holds, after the cell lists, a run of 12-byte animation steps
(duration, flags, data pointer, loop offset, frame index; `struct SequenceStep` in ps1/src/types.fields) and a
table of 16-byte frame records (`struct FrameRecord`) that the steps point at through the frame index. Both are a
mechanical conversion of the arcade's sequence records. For every fighter file on the disc (PLnn.PAC and
PLnnX.PAC) this tool finds the step run in the module, finds the arcade's run of records, pairs the two, runs the
conversion rules below and prints one line per file: how many steps, how many are paired, how many of the paired
steps and of the frame records the rules rebuild to the byte, and where the PS1 differs from the arcade.

The arcade side. A record is 24 bytes: a big-endian 16-bit countdown, a 16-bit flags word, a 32-bit pointer to the
art record (the one that fighters.py converts to the PS1 cell list), then 16 bytes of frame data. When bit 15 of
the flags is set, 4 more bytes follow: the absolute address of the record the sequence jumps to (a loop). The next
record follows directly. (A few records in some fighters have the 4 bytes with bit 15 clear; the tool takes them as
a jump too, and counts them.) The records of a fighter lie in one or more groups (Rolento's PL0E has two, Gouki's PL02
too, the second group being the one that PL14 uses), each group in front of its art records or after the tables of
16-bit displacements that select a sequence; tables lie before the first group, and between and after the groups.

The rules (arcade record i, paired with PS1 step j):
  - duration = the countdown; flags = the flags word (the PS1 keeps bit 15 as "jump" and the rest as they are; for the
    records of one range, in PL04, PL11, PL12, PL13 and PL17, the low byte divided by 0x18);
  - data pointer = the byte offset of the art record's cell list from the list base (fighters.py finds it);
  - loop offset = when the jump flag is set, the PS1 step index of the target record minus j (0 otherwise);
  - frame record = the arcade's 16 frame bytes with each group of 4 bytes reversed (a 32-bit byte swap);
  - frame index = 0 for an all-zero frame, else the frames are numbered from 1 in the order of first use down the
    step run, and a frame seen before keeps its number; the steps the PS1 numbered otherwise are listed apart;
  - the PS1's pointer table is the steps' addresses in the order of the arcade's tables, but for the entries of
    dropped records, in an order the PS1 chose for the tables (PL0E), and with tables of its own at the end.
Beyond the rules the tool reports, and does not invent rules for: steps with no arcade record (the PS1's own),
arcade records the PS1 left out (reductions), and paired steps that differ in one field (the PS1's changes).

The runtime C (sequence.c). model_build is the specification of sequence.c, which builds the steps, the frame records
and the pointer table of a file from the arcade program and the cell list offsets that fighter.c's lists give. The
per-file facts it needs are in the generated sequences_table.c (--c-table): where the module keeps the three blocks,
the arcade's groups and tables, which records make which steps (runs), which PS1 steps are its own (spans), which
table slots are which table entries (slices), the steps whose frame number is not the first-use one, the scale range
and the one rule flag. The C builds the runs' steps and leaves the PS1's own to the disc. --compare-c builds the C
with the checked-in table into a temporary folder and prints, for each file the table covers (fighters_table.c's),
whether the C equals the model and how many steps, frame records and table entries equal the disc's.

Inputs: the disc image (raw 2352-byte sectors) and the arcade set's ZIP (MAME's sfa2u), both given as arguments and
both read only. It writes nothing except a temporary folder (fighters.py compiles a small C helper there) that it
removes (--compare-c also compiles and runs the C driver there). It prints sizes, addresses and counts, never game
bytes. It is an analysis and reference tool; the converter that the port runs is the C.

    python3 port/arcade/sequences.py --disc BIN --arcade ZIP [--only NAME] [--detail] [--c-table | --compare-c]
"""
import argparse
import collections
import difflib
import shutil
import struct
import subprocess
import sys
import tempfile
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import fighters as F   # noqa: E402  read-only: the art records, the lists, the arcade views

STEP = 12
FRAME = 16
REC = 24                  # an arcade record without a jump
JUMP = 0x8000
HOLD = 4095               # the longest countdown: a hold
ART_LO, ART_HI = 0x100000, 0x400000   # where the arcade's art records lie (a plausibility bound for a pointer)
LOAD = {1: 0x801b0000, 2: 0x801c8000}   # where the first-side and second-side modules are linked (ps1/docs/overlays.md)
MIN_RUN = 50              # steps in a row that make a step run
SCAN = 0x4000             # how far after the lists the step run is looked for


# ---------------------------------------------------------------- pure functions (tested without game data)

def convert_frame(fr):
    """The PS1 frame record of the arcade's 16 frame bytes: each group of 4 bytes reversed."""
    return b''.join(fr[i:i + 4][::-1] for i in range(0, len(fr) - len(fr) % 4, 4))


def pack_step(dur, flags, ptr, loop, frame):
    return struct.pack('<hhIhH', dur, flags - 65536 if flags >= 32768 else flags, ptr, loop, frame)


def step_at(mod, o):
    dur, fl, ptr, loop, fi = struct.unpack_from('<hhIhH', mod, o)
    return dict(o=o, dur=dur, fl=fl & 0xffff, dp=ptr & ~1, odd=ptr & 1, lo=loop, fi=fi)


def valid_step(mod, o, list_size):
    """Whether the 12 bytes at o can be a step: a countdown up to 4095, a pointer inside the lists, a small loop."""
    if o < 0 or o + STEP > len(mod):
        return False
    dur, _fl, ptr, loop, fi = struct.unpack_from('<hhIhH', mod, o)
    return 0 <= dur <= HOLD and ptr < list_size and fi < 2048 and abs(loop) < 2048


def find_steps(mod, lo, list_size, min_run=MIN_RUN, scan=SCAN):
    """(start, steps) of the first run of at least min_run steps that starts at a 4-aligned offset from lo on."""
    for s in range(lo & ~3, min(len(mod), lo + scan), 4):
        if not valid_step(mod, s, list_size):
            continue
        o = s
        while valid_step(mod, o, list_size):
            o += STEP
        if (o - s) // STEP >= min_run:
            return s, [step_at(mod, s + STEP * k) for k in range((o - s) // STEP)]
    return None, []


def valid_record(prog, a):
    """Whether an arcade record can start at a: an even address, a countdown 1..4095, an even art pointer in range."""
    if a < 0 or a & 1 or a + REC > len(prog):
        return False
    dur, _fl, art = struct.unpack_from('>HHI', prog, a)
    return 1 <= dur <= HOLD and not art & 1 and ART_LO <= art < ART_HI


def record_at(prog, a):
    """(record, next address) of the record at a; the record's 'quirk' is set when a jump has bit 15 clear."""
    dur, fl, art = struct.unpack_from('>HHI', prog, a)
    size, quirk = REC, False
    if fl & JUMP:
        size = REC + 4
    elif not valid_record(prog, a + REC) and valid_record(prog, a + REC + 4):
        size, quirk = REC + 4, True
    tgt = struct.unpack_from('>I', prog, a + REC)[0] if size > REC and a + size <= len(prog) else None
    return dict(a=a, dur=dur, fl=fl | (JUMP if quirk else 0), art=art, tgt=tgt, fr=prog[a + 8:a + 24], quirk=quirk), a + size


def walk_records(prog, a, limit=4000):
    out = []
    while len(out) < limit and valid_record(prog, a):
        rec, a = record_at(prog, a)
        out.append(rec)
    return out


def extend_back(prog, a, limit=400):
    """The address of the first record of the run that holds the record at a (records are walked back from a)."""
    for _ in range(limit):
        for size in (REC, REC + 4):
            p = a - size
            if valid_record(prog, p):
                rec, nxt = record_at(prog, p)
                if nxt == a:
                    a = p
                    break
        else:
            return a
    return a


def find_run(prog, steps, inverse):
    """Address of the arcade record the first steps with a known art record come from, or None.

    inverse maps a cell list offset to the arcade address of its art record."""
    for q in steps[:12]:
        art = inverse.get(q['dp'])
        if art is None:
            continue
        pat = struct.pack('>I', art)
        h = prog.find(pat)
        while h >= 0:
            a = h - 4
            if a >= 0 and not a & 1 and struct.unpack_from('>HH', prog, a) == (q['dur'], q['fl'] & 0xffff):
                return a
            h = prog.find(pat, h + 1)
    return None


def _match_blocks(ka, kb):
    return difflib.SequenceMatcher(None, ka, kb, autojunk=False).get_opcodes()


def pair_records(arc, steps, amap, frame_of=None):
    """[(i, j)]: arcade record i belongs to PS1 step j.

    First by (countdown, flags, list offset) over the whole run. Then, in each gap between matched blocks, by the
    list offset alone (the PS1 changed a countdown or flags), then, when frame_of(j) gives the PS1's frame bytes
    of step j, by (countdown, converted frame bytes), and last, in a gap of the same size on both sides, in order,
    when no pair disagrees in a list offset that both know. A step the PS1 gave another list than the arcade's
    record has is not paired: it is the PS1's own."""
    pairs = []
    unique = iter(range(-1, -10 ** 9, -1))
    # a key function per tier: (arcade record) -> key, (step index) -> key; a key never seen on the other side is harmless
    tiers = [(lambda i: amap.get(arc[i]['art'], next(unique)), lambda j: steps[j]['dp'])]
    if frame_of:
        tiers.append((lambda i: (arc[i]['dur'], convert_frame(arc[i]['fr'])),
                      lambda j: (steps[j]['dur'], frame_of(j) or next(unique))))

    def gap(i1, i2, j1, j2, t):
        if i1 >= i2 or j1 >= j2:
            return
        if t == len(tiers):
            if i2 - i1 == j2 - j1 and all(amap.get(arc[i]['art']) in (None, steps[j]['dp']) for i, j in zip(range(i1, i2), range(j1, j2))):
                pairs.extend(zip(range(i1, i2), range(j1, j2)))
            return
        ka = [tiers[t][0](i) for i in range(i1, i2)]
        kb = [tiers[t][1](j) for j in range(j1, j2)]
        for tag, a1, a2, b1, b2 in _match_blocks(ka, kb):
            if tag == 'equal':
                pairs.extend(zip(range(i1 + a1, i1 + a2), range(j1 + b1, j1 + b2)))
            else:
                gap(i1 + a1, i1 + a2, j1 + b1, j1 + b2, t + 1)

    ka = [(a['dur'], a['fl'], amap.get(a['art'])) for a in arc]
    kb = [(p['dur'], p['fl'], p['dp']) for p in steps]
    for tag, a1, a2, b1, b2 in _match_blocks(ka, kb):
        if tag == 'equal':
            pairs.extend(zip(range(a1, a2), range(b1, b2)))
        else:
            gap(a1, a2, b1, b2, 0)
    pairs.sort()
    return pairs


def frame_numbers(frames, actual, own=()):
    """Expected frame index of each step, given the frame bytes of each step (None where unknown) and the actual indices.

    0 for an all-zero frame; a frame seen before has the index of its first use; a new one gets one more than the
    highest index so far. The actual indices are followed, so one difference does not carry on. Steps in `own` (the
    PS1's own, whose frames the PS1 numbered its own way) are not given an expected index and do not move the count."""
    seen, top, out = {}, 0, []
    for k, (fb, got) in enumerate(zip(frames, actual)):
        if fb is None or k in own:
            out.append(None)
            continue
        if not any(fb):
            out.append(0)
        elif fb in seen:
            out.append(seen[fb])
        else:
            out.append(top + 1)
        if any(fb) and fb not in seen:
            seen[fb] = got
        top = max(top, got)
    return out


def scale_flags(fl):
    """The flags some fighters' PS1 steps have for an arcade flags word whose low byte counts in units of 0x18.

    The low byte divided by 0x18, bits 8 and 15 kept; None when the low byte is not a multiple of 0x18 (or zero)."""
    low = fl & 0xff
    if not low or low % 0x18:
        return None
    return (fl & 0x8100) | low // 0x18


def run_length(prog, recs, p, stop):
    """Entries of the table at p (16-bit displacements from p) that point at a record, up to stop."""
    n, o = 0, p
    while o + 2 <= stop and (p + struct.unpack_from('>h', prog, o)[0]) in recs:
        n += 1
        o += 2
    return n


def record_end(rec):
    """The address after a record (24 bytes, 28 with a jump target)."""
    return rec['a'] + REC + (4 if rec['tgt'] is not None else 0)


def next_group(prog, end, known, window=0x400, min_records=10):
    """(start, records) of the group of records that follows the one ending at end, or None.

    The arcade keeps a character's sequence records in groups with tables between them. The next group starts within
    window bytes of end at the first address where three records in a row are valid; it is taken when it has at
    least min_records records and half of their art records are ones the file's lists are made from (known)."""
    for b in range(end, min(end + window, len(prog)), 2):
        if valid_record(prog, b) and valid_record(prog, b + REC) and valid_record(prog, b + 2 * REC):
            recs = walk_records(prog, b)
            if len(recs) >= min_records and 2 * sum(1 for x in recs if x['art'] in known) >= len(recs):
                return b, recs
    return None


def find_tables(prog, recs, first, end, back=0x900, min_run=1, later=()):
    """The arcade's sequence tables around the run of records: [(start, [record addresses])], by address.

    A table is a list of 16-bit displacements from its own start to a record, ended by a zero word; the tables lie
    one after another, either right before the first record or right after the last. The records may lie in
    several groups: first and end are the first group's, later is [(start, end)] of the groups after it, and the
    tables after each group are looked for up to the next group's start."""
    out = []
    for p in range(max(0, first - back), first, 2):
        q, tabs = p, []
        while q < first:
            n = run_length(prog, recs, q, first)
            if n < min_run:
                break
            tabs.append((q, n))
            q += 2 * (n + 1)
        if tabs and q == first:
            out = tabs
            break
    ends = [end] + [e for _s, e in later]
    stops = [s for s, _e in later] + [len(prog)]
    for q, stop in zip(ends, stops):
        while q + 2 <= stop:
            n = run_length(prog, recs, q, stop)
            if n < min_run:
                break
            out.append((q, n))
            q += 2 * (n + 1)
    return [(q, [q + struct.unpack_from('>h', prog, q + 2 * k)[0] for k in range(n)]) for q, n in out]


def match_slices(flat, mine):
    """[(entry, slot, count)]: runs of the arcade's table entries (flat) that the PS1's pointer table (mine) holds
    in the same order, longest first, by repeated matching of what is left. The PS1 may order the arcade's tables
    differently (PL0E), so a later run may lie before an earlier one. The result is by slot."""
    free_a, free_b, out = list(range(len(flat))), list(range(len(mine))), []
    while free_a and free_b:
        ka, kb = [flat[i] for i in free_a], [mine[j] for j in free_b]
        blocks = [b for b in difflib.SequenceMatcher(None, ka, kb, autojunk=False).get_matching_blocks() if b.size]
        if not blocks:
            break
        used_a, used_b = set(), set()
        for b in blocks:
            ia, ib = free_a[b.a:b.a + b.size], free_b[b.b:b.b + b.size]
            k = 0
            while k < b.size:                     # cut where either side leaves a gap in the original numbering
                m = k
                while m + 1 < b.size and ia[m + 1] == ia[m] + 1 and ib[m + 1] == ib[m] + 1:
                    m += 1
                out.append((ia[k], ib[k], m - k + 1))
                k = m + 1
            used_a.update(ia)
            used_b.update(ib)
        free_a = [i for i in free_a if i not in used_a]
        free_b = [j for j in free_b if j not in used_b]
    return sorted(out, key=lambda x: x[1])


def ps1_table_entries(mod, s0, n, load):
    """The step indices of the pointer table after the n steps that start at s0, for a module linked at load."""
    out, o = [], s0 + STEP * n
    while o + 4 <= len(mod):
        v = struct.unpack_from('<I', mod, o)[0] - load - s0
        if not 0 <= v < STEP * n or v % STEP:
            break
        out.append(v // STEP)
        o += 4
    return out


def find_frame_base(mod, wanted):
    """Offset of the frame table: the offset that most (index, frame bytes) pairs agree on, as (base, votes), or None.

    wanted is {frame index: converted bytes}."""
    votes = collections.Counter()
    for fi, fb in wanted.items():
        if fi == 0 or not any(fb):
            continue
        k = mod.find(fb)
        while k >= 0:
            if not k & 3 and k - FRAME * fi > 0:
                votes[k - FRAME * fi] += 1
            k = mod.find(fb, k + 1)
    return votes.most_common(1)[0] if votes else None


def ranges(idx):
    """'a-b c d-e' from a sorted list of integers."""
    out, k = [], 0
    while k < len(idx):
        m = k
        while m + 1 < len(idx) and idx[m + 1] == idx[m] + 1:
            m += 1
        out.append(str(idx[k]) if m == k else f'{idx[k]}-{idx[m]}')
        k = m + 1
    return out


# ---------------------------------------------------------------- the model of the runtime C (sequence.c)
#
# The specification of sequence.c. The inputs are the arcade program, the cell list offsets of the file (what
# fighter.c's lists give, by art record) and a row: the per-file facts that make_row derives from the disc, in the
# form of sequences_table.c (addresses, counts and indices; no bytes). Nothing else is read: the PS1's own steps,
# frame records and table entries are not built.

JUMP_WITHOUT_BIT15 = 'JUMP_WITHOUT_BIT15'   # the one rule flag: the arcade file has records with a jump target and bit 15 clear
RULE_NAMES = (JUMP_WITHOUT_BIT15,)


def spans(idx):
    """[(first, count)] of the runs of consecutive integers in a sorted list."""
    out = []
    for v in idx:
        if out and out[-1][0] + out[-1][1] == v:
            out[-1][1] += 1
        else:
            out.append([v, 1])
    return [(a, n) for a, n in out]


def model_records(prog, groups, quirks=False):
    """The records of the groups [(address, count)], one group after the other (as walk_records reads each), or
    ValueError. With quirks a record whose flags lack bit 15 but that is followed by a record only 4 bytes later is a
    jump too (the arcade's own slip)."""
    out = []
    for a, count in groups:
        for _ in range(count):
            if not valid_record(prog, a):
                raise ValueError(f'no sequence record at {a:#x}')
            if quirks:
                rec, a = record_at(prog, a)
            else:
                dur, fl, art = struct.unpack_from('>HHI', prog, a)
                size = REC + 4 if fl & JUMP else REC
                if a + size > len(prog):
                    raise ValueError(f'sequence record at {a:#x} runs past the program')
                tgt = struct.unpack_from('>I', prog, a + REC)[0] if size > REC else None
                rec, a = dict(a=a, dur=dur, fl=fl, art=art, tgt=tgt, fr=prog[a + 8:a + 24], quirk=False), a + size
            out.append(rec)
    return out


def model_table_entries(prog, tables):
    """The arcade record address of each entry of the arcade's sequence tables, one after another:
    tables is [(address, count)] and an entry is a 16-bit displacement from the start of its table."""
    out = []
    for q, n in tables:
        out += [q + struct.unpack_from('>h', prog, q + 2 * k)[0] for k in range(n)]
    return out


def model_build(prog, offsets, row):
    """The specification of the runtime C: dict(steps, steps_built, frames, frames_built, table, table_built) for a row of
    sequences_table.c and offsets {art record address: cell list offset}. The built lists hold 0/1 per step, frame
    record or table entry; unbuilt bytes are zero. Raises ValueError when the row does not fit the program.

    A run (record, step, count) makes the steps step.. from the arcade records record..: duration and flags as they
    are (the flags' low byte divided by 0x18 for the records of the scale range, when it is a multiple), the art
    record's cell list offset, for a jump the distance in steps to the target record's step, the frame record
    numbered from 1 in the order of first use down the runs (0 for an all-zero frame; the steps the runs leave out
    are the PS1's own and do not count), except for the steps of the overrides (step, number), which the PS1
    numbered otherwise. A step whose art record has no list or whose jump target has no step is not built. The
    pointer table is the steps' addresses in the order of the arcade's tables, for the slices (entry, slot, count)."""
    ns, nf, ne = row['step_count'], row['frame_count'], row['table_entries']
    steps, sb = bytearray(STEP * ns), [0] * ns
    frames, fb = bytearray(FRAME * nf), [0] * nf
    table, tb = bytearray(4 * ne), [0] * ne
    recs = model_records(prog, row['groups'], JUMP_WITHOUT_BIT15 in row['rules'])
    at = {x['a']: k for k, x in enumerate(recs)}
    step_of = {}
    for i, j, n in row['runs']:
        if i + n > len(recs) or j + n > ns:
            raise ValueError('a run outside the records or the steps')
        for k in range(n):
            if i + k in step_of:
                raise ValueError('runs overlap')
            step_of[i + k] = j + k
    if nf:
        fb[0] = 1                         # frame record 0 is all zero
    seen, number, top = {}, {}, 0
    override = dict(row['frame_overrides'])
    if any(st >= ns for st in override):
        raise ValueError('a frame override outside the steps')
    for i, j, n in row['runs']:
        for k in range(n):
            conv = convert_frame(recs[i + k]['fr'])
            if j + k in override:
                num = override[j + k]
                top = max(top, num)
                if any(conv) and conv not in seen:
                    seen[conv] = num
            elif not any(conv):
                num = 0
            elif conv in seen:
                num = seen[conv]
            else:
                top += 1
                num = seen[conv] = top
            if num >= nf:
                raise ValueError(f'frame {num} beyond the {nf} frame records')
            frames[FRAME * num:FRAME * (num + 1)] = conv
            fb[num] = 1
            number[i + k] = num
    first, count = row['scale']
    for i, j, n in row['runs']:
        for k in range(n):
            rec, jj = recs[i + k], j + k
            fl = rec['fl']
            if first <= i + k < first + count:
                fl = scale_flags(fl) if scale_flags(fl) is not None else fl
            dp = offsets.get(rec['art'])
            if dp is None:
                dp = offsets.get(dict(row.get('aliases', ())).get(rec['art']))
            if dp is None:
                continue
            lo = 0
            if rec['fl'] & JUMP:
                sj = step_of.get(at.get(rec['tgt']))
                if sj is None or not -32768 <= sj - jj <= 32767:
                    continue
                lo = sj - jj
            steps[STEP * jj:STEP * (jj + 1)] = pack_step(rec['dur'], fl, dp, lo, number[i + k])
            sb[jj] = 1
    entries = model_table_entries(prog, row['tables'])
    for e, slot, n in row['slices']:
        if e + n > len(entries) or slot + n > ne:
            raise ValueError('a table slice outside the arcade tables or the PS1 table')
        for t in range(n):
            sj = step_of.get(at.get(entries[e + t]))
            if sj is not None:
                struct.pack_into('<I', table, 4 * (slot + t), row['load'] + row['steps'] + STEP * sj)
                tb[slot + t] = 1
    return dict(steps=bytes(steps), steps_built=sb, frames=bytes(frames), frames_built=fb, table=bytes(table), table_built=tb)


def model_offsets(lists, segments, view, pads=None):
    """{art record address: cell list offset} for lists bytes (fighter.c's, or the model's) and the file's segments
    (address, count, bank): list k belongs to the k-th record of the segments, address 0 being empty lists. The last
    list a record gets is kept. pads {list: zero bytes after it} (the lists the PS1 laid out larger). Raises ValueError
    when the lists and the segments disagree."""
    sizes, o = [], 0
    pads = pads or {}
    while o < len(lists):
        lst = F.parse_list(lists, o)
        if lst is None:
            raise ValueError(f'no list at {o}')
        sizes.append((o, lst['end'] - o))
        o = lst['end'] + pads.get(len(sizes) - 1, 0)
    out, k = {}, 0
    for address, count, _bank in segments:
        a = address
        for _ in range(count):
            if k >= len(sizes):
                raise ValueError('more records than lists')
            if address:
                out[a] = sizes[k][0]
                rec = read_any(view, a)
                if rec is None:
                    raise ValueError(f'no record at {a:#x}')
                a = rec[2]
            k += 1
    return out


# ---------------------------------------------------------------- the analysis of one file

# A second block of steps that some modules keep after the frame records of the first: its own steps, pointer table and frame records (their
# own numbering from 1), made from a second group of the arcade's sequence records. {character: module offset of the block's first step}.
# Evidence (measured 2026-10-24): PL17's steps at 0x14010 (1146 of them) follow 16 zero bytes after the frames of the first block; all 1146
# pair with the arcade's records from 0x1f97b0; the pointer table (272 entries) follows the steps and the 293 frame records follow the table.
SECOND_BLOCKS = {'PL17': 0x14010}


def second_block(name):
    """The module offset of the second block of steps of a file ('PL17.PAC'), or None."""
    return SECOND_BLOCKS.get(name.removesuffix('.PAC').removesuffix('X'))


def find_aliases(view, amap, arc):
    """{art record address: the address of a record that has the same cells and a list in amap} for the art records of the arcade's records `arc` that
    amap lacks: the arcade keeps copies of some art records (another group's sequences point at the copy), and the PS1 gave the copy the list of the
    original."""
    key = lambda a: (lambda rec: None if rec is None else tuple((c[0], c[1], c[2], c[3]) for c in rec[1]))(read_any(view, a))
    by_cells = {}
    for a in sorted(amap):
        k = key(a)
        if k is not None:
            by_cells.setdefault(k, a)
    out = {}
    for rec in arc:
        art = rec['art']
        if art not in amap and art not in out:
            k = key(art)
            if k is not None and k in by_cells:
                out[art] = by_cells[k]
    return out


def run_of_steps(mod, at, lsize):
    """(at, steps) of the steps that follow one another from `at` while they can be steps (valid_step)."""
    out, o = [], at
    while valid_step(mod, o, lsize):
        out.append(step_at(mod, o))
        o += STEP
    return at, out


def analyse(mod, r, prog, view, load=0x801b0000, min_run=MIN_RUN, block=None):
    """Everything the line and the detail need, as a dict. The key 'error' when the file cannot be read.

    r is fighters.analyse's result for the module (lists, list base and end, segments). block: the module offset of a second block of steps
    (SECOND_BLOCKS), whose steps, table, frames and arcade group are analysed in place of the first block's."""
    amap = list_offsets(r, view)
    inverse = {v: k for k, v in amap.items()}
    lsize = r['listsend'] - r['lbase']
    start, steps = run_of_steps(mod, block, lsize) if block is not None else find_steps(mod, r['listsend'], lsize, min_run)
    if not steps:
        return dict(error='no step run after the lists')
    a0 = find_run(prog, steps, inverse)
    if a0 is None:
        return dict(error='no arcade record for the first steps')
    a0 = extend_back(prog, a0)
    arc = walk_records(prog, a0)
    groups = [(a0, len(arc))]
    while True:
        nxt = next_group(prog, record_end(arc[-1]), amap)
        if nxt is None:
            break
        groups.append((nxt[0], len(nxt[1])))
        arc += nxt[1]
    aliases = find_aliases(view, amap, arc)
    amap = {**amap, **{art: amap[orig] for art, orig in aliases.items()}}
    pairs = pair_records(arc, steps, amap)
    # the frame table, from the frames of the paired steps; then the pairing again, with the PS1's frames as a key
    wanted = {}
    for i, j in pairs:
        wanted.setdefault(steps[j]['fi'], convert_frame(arc[i]['fr']))
    fb = find_frame_base(mod, wanted)
    base = fb[0] if fb else None
    if base is not None:
        def frame_of(j):
            o = base + FRAME * steps[j]['fi']
            return mod[o:o + FRAME] if o + FRAME <= len(mod) else None
        pairs = pair_records(arc, steps, amap, frame_of)
    pj = {j: i for i, j in pairs}
    pi = dict(pairs)
    nframes = max(p['fi'] for p in steps) + 1
    frames = []
    for j, p in enumerate(steps):
        if j in pj:
            frames.append(convert_frame(arc[pj[j]]['fr']))
        elif base is not None and base + FRAME * (p['fi'] + 1) <= len(mod):
            frames.append(mod[base + FRAME * p['fi']:base + FRAME * (p['fi'] + 1)])
        else:
            frames.append(None)
    expect_fi = frame_numbers(frames, [p['fi'] for p in steps], set(range(len(steps))) - set(pj))
    # the steps
    c = collections.Counter()
    at = {x['a']: k for k, x in enumerate(arc)}
    first = {}
    exact_steps, scaled_steps = set(), set()
    for i, j in pairs:
        a, p = arc[i], steps[j]
        got = []
        if a['dur'] != p['dur']:
            got.append('dur')
        scaled = False
        if a['fl'] != p['fl']:
            if scale_flags(a['fl']) == p['fl']:
                scaled = True
            else:
                got.append('flags')
        want_dp = amap.get(a['art'])
        decided = True
        want_lo = 0
        if want_dp is None:
            c['dp unknown'] += 1
            decided = False
        elif want_dp != p['dp']:
            got.append('list')
        if a['fl'] & JUMP:
            ti = at.get(a['tgt'])
            if ti is None or ti not in pi:
                c['loop target unpaired'] += 1
                decided = False
            else:
                want_lo = pi[ti] - j
                if want_lo != p['lo']:
                    got.append('loop')
        elif p['lo']:
            got.append('loop')
        if expect_fi[j] is not None and expect_fi[j] != p['fi']:
            got.append('frame index')
        for g in got:
            c[g] += 1
            first.setdefault(g, (i, j))
        # the rules build the 12 bytes; a step is exact when they equal the module's
        if decided and expect_fi[j] is not None:
            want = pack_step(a['dur'], scale_flags(a['fl']) if scaled else a['fl'], want_dp, want_lo, expect_fi[j])
            if want == mod[p['o']:p['o'] + STEP]:
                (scaled_steps if scaled else exact_steps).add(j)
    # the frame records
    ref = {}
    for i, j in pairs:
        ref.setdefault(steps[j]['fi'], set()).add(convert_frame(arc[i]['fr']))
    fr_exact = fr_diff = 0
    if base is not None:
        for fi, variants in ref.items():
            if base + FRAME * (fi + 1) > len(mod):
                continue
            if len(variants) == 1 and mod[base + FRAME * fi:base + FRAME * (fi + 1)] == next(iter(variants)):
                fr_exact += 1
            else:
                fr_diff += 1
    # the sequence tables
    at_group = [0]
    for _a, n in groups:
        at_group.append(at_group[-1] + n)
    bounds = [(arc[at_group[g]]['a'], record_end(arc[at_group[g + 1] - 1])) for g in range(len(groups))]
    tabs = find_tables(prog, {x['a'] for x in arc}, a0, bounds[0][1], later=bounds[1:])
    flat = [pi.get(at[a], ('arcade only', at[a])) for _q, ents in tabs for a in ents]
    mine = ps1_table_entries(mod, start, len(steps), load)
    slices = match_slices(flat, mine)
    tab_match = sum(n for _e, _s, n in slices)
    only_arc = [i for i in range(len(arc)) if i not in pi]
    only_ps1 = [j for j in range(len(steps)) if j not in pj]
    return dict(start=start, aliases=sorted(aliases.items()), steps=steps, a0=a0, groups=groups, arc=arc, pairs=pairs, exact=len(exact_steps), scaled=len(scaled_steps), counts=c, first=first,
                base=base, nframes=nframes, fr_exact=fr_exact, fr_diff=fr_diff, fr_own=nframes - len(set(ref) | {0}),
                tables=[len(e) for _q, e in tabs], table_arcade=len(flat), table_ps1=len(mine), table_match=tab_match,
                table_at=[(q, len(e)) for q, e in tabs], slices=slices,
                only_arc=only_arc, only_ps1=only_ps1, quirks=sum(1 for a in arc if a['quirk']),
                odd=sum(1 for p in steps if p['odd']))


def make_row(name, side, r, block=0):
    """(row for sequences_table.c, None) for a file's analysis r, else (None, why not).

    The runs are the paired records that follow one another in both lists. The frame overrides are the paired steps
    whose frame number is not the one first use gives (found by running the numbering rule of model_build against
    the PS1's numbers; the PS1's own steps do not count). The scale range is the records from the first to the last
    whose flags the PS1 gave the scaled form. The slices are the matching blocks of the arcade's tables and the PS1's
    pointer table, in entries. The PS1's own steps are the ones no run makes."""
    if 'error' in r:
        return None, r['error']
    if r['base'] is None:
        return None, 'frame table not found'
    steps, arc, pairs = r['steps'], r['arc'], r['pairs']
    runs = []
    for i, j in pairs:
        if runs and runs[-1][0] + runs[-1][2] == i and runs[-1][1] + runs[-1][2] == j:
            runs[-1][2] += 1
        else:
            runs.append([i, j, 1])
    seen, top, over = {}, 0, []
    for i, j in pairs:
        conv, got = convert_frame(arc[i]['fr']), steps[j]['fi']
        want = 0 if not any(conv) else seen.get(conv, top + 1)
        if want != got:
            over.append((j, got))
        if got and (want != got or conv not in seen):
            top = max(top, got)
        if any(conv) and conv not in seen:
            seen[conv] = got
    scaled = [i for i, j in pairs if arc[i]['fl'] != steps[j]['fl'] and scale_flags(arc[i]['fl']) == steps[j]['fl']]
    paired = {j for _i, j in pairs}
    row = dict(file=name, block=block, aliases=r['aliases'], load=LOAD[side], steps=r['start'], step_count=len(steps), frames=r['base'], frame_count=r['nframes'],
               groups=r['groups'], rules=[JUMP_WITHOUT_BIT15] if r['quirks'] else [],
               scale=(min(scaled), max(scaled) - min(scaled) + 1) if scaled else (0, 0),
               runs=[tuple(x) for x in runs], own=spans([j for j in range(len(steps)) if j not in paired]),
               frame_overrides=over, tables=r['table_at'], slices=r['slices'], table_entries=r['table_ps1'])
    return row, None


def read_any(view, a):
    """The art record at a, of mode 2 or (when fighters.py reads those) of mode 0: (cells, cells, end address), or None."""
    return getattr(F, 'read_any', F.read_record)(view, a)


def list_offsets(r, view):
    """{arcade address of an art record: cell list offset from the list base} for the lists fighters.py converted."""
    lists, out = r['lists'], {}
    for a, count, k, _bank in r['segments']:
        placed = 0
        j = k
        while placed < count and j < len(lists):
            if F.is_empty(lists[j]):
                j += 1
                continue
            out[a] = lists[j]['at'] - r['lbase']
            rec = read_any(view, a)
            if rec is None:
                break
            a = rec[2]
            j += 1
            placed += 1
    return out


def line(name, r):
    if 'error' in r:
        return f'{name}: {r["error"]}'
    c, n = r['counts'], len(r['steps'])
    diffs = ', '.join(f'{k} {v}' for k, v in c.most_common() if k in ('dur', 'flags', 'list', 'loop', 'frame index'))
    unknown = ', '.join(f'{k} {v}' for k, v in c.most_common() if k in ('dp unknown', 'loop target unpaired'))
    groups = f' in {len(r["groups"])} groups' if len(r['groups']) > 1 else ''
    s = (f'{name}: steps {n} at +{r["start"]:#x} ({STEP * n} B), arcade run {len(r["arc"])} at {r["a0"]:#x}{groups}; paired {len(r["pairs"])}, '
         f'exact {r["exact"]} ({STEP * r["exact"]} B)')
    if r['scaled']:
        s += f' + {r["scaled"]} with scaled flags'
    if diffs:
        s += f'; differ: {diffs}'
    if unknown:
        s += f'; not decidable: {unknown}'
    s += f'; PS1 only {len(r["only_ps1"])}, arcade only {len(r["only_arc"])}'
    if r['base'] is None:
        s += '; frame table not found'
    else:
        s += (f'; frames {r["nframes"]} at +{r["base"]:#x}: exact {r["fr_exact"]} ({FRAME * r["fr_exact"]} B), differ {r["fr_diff"]}, '
              f'PS1 only {r["fr_own"]}')
    s += f'; tables: arcade {r["table_arcade"]} entries in {len(r["tables"])}, PS1 {r["table_ps1"]}, equal in order {r["table_match"]}'
    if r['quirks'] or r['odd']:
        s += f'; jumps without bit 15 {r["quirks"]}, odd pointers {r["odd"]}'
    return s


def details(name, r):
    """A few short lines: the runs of PS1-only steps and arcade-only records, and the first step of each kind of difference."""
    if 'error' in r:
        return []
    out = []
    for label, idx in (('PS1-only steps', r['only_ps1']), ('arcade-only records', r['only_arc'])):
        if idx:
            rs = ranges(idx)
            out.append(f'  {name}: {label} {len(idx)} in {len(rs)} runs: ' + ' '.join(rs[:6]) + (' ...' if len(rs) > 6 else ''))
    for kind, (i, j) in list(r['first'].items())[:5]:
        a, p = r['arc'][i], r['steps'][j]
        out.append(f'  {name}: first {kind} difference: arcade record {i} ({a["a"]:#x}) step {j}: arcade dur {a["dur"]} flags {a["fl"]:#x}, '
                   f'PS1 dur {p["dur"]} flags {p["fl"]:#x} list {p["dp"]} loop {p["lo"]} frame {p["fi"]}')
    return out


def c_table(rows, omitted):
    """The text of sequences_table.c for rows [(row)] (make_row's) and omitted [(name, why)]."""
    out = ['/* GENERATED by `python3 port/arcade/sequences.py --disc DISC --arcade SET --c-table`; do not edit.',
           ' * For each fighter file of fighters_table.c: where the PS1 module keeps its animation steps, frame records and',
           ' * sequence pointer table, where the arcade keeps its sequence records (groups), which records make which steps',
           " * (runs of records and steps), which steps the PS1 added itself (own spans, step indices only), the arcade's",
           ' * sequence tables, which PS1 table slots are which arcade table entries, the steps whose frame number is not the order of first use, the',
           ' * records whose flags the PS1 scaled, and the one rule flag. Only addresses, counts and indices: no byte of',
           ' * either game. A file with a second block of steps has a second row of its name (its own steps, pointer table and frame records).']
    if omitted:
        out += [' *', ' * Left out:']
        out += [f' *   {name}: {why}' for name, why in omitted]
    out += [' */', '#include "sequence.h"', '']
    kinds = (('groups', 'struct arcade_sequence_group', lambda x: f'{{{x[0]:#x}u, {x[1]}u}}'),
             ('runs', 'struct arcade_sequence_run', lambda x: f'{{{x[0]}u, {x[1]}u, {x[2]}u}}'),
             ('own', 'struct arcade_sequence_span', lambda x: f'{{{x[0]}u, {x[1]}u}}'),
             ('tables', 'struct arcade_sequence_table', lambda x: f'{{{x[0]:#x}u, {x[1]}u}}'),
             ('slices', 'struct arcade_sequence_slice', lambda x: f'{{{x[0]}u, {x[1]}u, {x[2]}u}}'),
             ('frame_overrides', 'struct arcade_sequence_frame', lambda x: f'{{{x[0]}u, {x[1]}u}}'),
             ('aliases', 'struct arcade_sequence_alias', lambda x: f'{{{x[0]:#x}u, {x[1]:#x}u}}'))
    short = {'groups': 'groups', 'runs': 'runs', 'own': 'own', 'tables': 'tables', 'slices': 'slices', 'frame_overrides': 'frames', 'aliases': 'aliases'}
    names = {}
    for key, ctype, fmt in kinds:
        for row in rows:
            data = tuple(tuple(x) for x in row[key])
            if data and (key, data) not in names:
                names[(key, data)] = f'{short[key]}_{row["file"].removesuffix(".PAC").lower()}{"_b" if row["block"] else ""}'
                out.append(f'static const {ctype} {names[(key, data)]}[] = {{')
                out += [f'    {fmt(x)},' for x in data]
                out += ['};', '']
    out.append('const struct arcade_sequence arcade_sequences[] = {')
    for row in rows:
        refs = []
        for key, _ctype, _fmt in kinds:
            data = tuple(tuple(x) for x in row[key])
            refs.append(f'{names[(key, data)]}, {len(data)}u' if data else 'NULL, 0u')
        rules = ' | '.join('ARCADE_SEQUENCE_' + n for n in row['rules']) or '0u'
        out.append(f'    {{"{row["file"]}", {row["load"]:#x}u, {row["steps"]:#x}u, {row["frames"]:#x}u, {row["step_count"]}u, '
                   f'{row["frame_count"]}u, {row["table_entries"]}u, {rules}, {row["scale"][0]}u, {row["scale"][1]}u, '
                   + ', '.join(refs) + '},')
    out += ['};', 'const unsigned arcade_sequence_count = sizeof arcade_sequences / sizeof arcade_sequences[0];', '']
    return '\n'.join(out)


def first_diff(a, b):
    return next((i for i, (x, y) in enumerate(zip(a, b)) if x != y), min(len(a), len(b)))


def compile_driver(tmp):
    """The C driver with the checked-in tables, compiled into tmp; its path."""
    here = Path(__file__).resolve().parent
    exe = Path(tmp) / 'sequence_test'
    subprocess.run(['cc', '-std=c99', '-O2', '-Wall', '-Wextra', '-o', str(exe), str(here / 'tests/sequence_test.c'), str(here / 'sequence.c'),
                    str(here / 'sequences_table.c'), str(here / 'fighter.c'), str(here / 'fighters_table.c')],
                   check=True, capture_output=True, timeout=120)
    return exe


def run_c(exe, prog_path, name, tmp):
    """(dict of the C's outputs, milliseconds) for a file of the checked-in tables, or an error string."""
    d = Path(tmp) / 'c-out'
    shutil.rmtree(d, ignore_errors=True)
    d.mkdir()
    r = subprocess.run([str(exe), 'file', str(prog_path), name, str(d)], capture_output=True, text=True, timeout=120)
    if r.returncode:
        return (r.stdout + r.stderr).strip()
    got = {k: (d / f'{k}.bin').read_bytes() for k in ('steps', 'frames', 'table')}
    for k in ('steps', 'frames', 'table'):
        got[k + '_built'] = list((d / f'{k}_built.bin').read_bytes())
    got['lists'] = {int(a): int(b) for a, b in (ln.split() for ln in (d / 'lists.txt').read_text().split('\n') if ln)}
    return got, float(r.stdout.split()[1])


def disc_equal(built, data, mod, base, size):
    """(units built, units equal to the disc's) for built flags, the built bytes and the module, at base, size bytes a unit."""
    n = eq = 0
    for k, b in enumerate(built):
        if b:
            n += 1
            eq += data[size * k:size * (k + 1)] == mod[base + size * k:base + size * (k + 1)]
    return n, eq


def compare_line(name, row, got, ms, model, mod):
    """One line: the C against the model (steps, frames, table, list offsets) and against the disc."""
    if isinstance(got, str):
        return f'{name}: C refused: {got}'
    parts = []
    for what, size in (('steps', STEP), ('frames', FRAME), ('table', 4)):
        same = got[what] == model[what] and got[what + '_built'] == [int(b) for b in model[what + '_built']]
        parts.append(f'{what} ' + ('=' if same else f'!= from +{first_diff(got[what], model[what])}') + 'model')
    parts.append('lists ' + ('=' if got['lists'] == model['offsets'] else '!=') + 'model')
    disc = []
    totals = [0, 0]
    for what, size, base in (('steps', STEP, row['steps']), ('frames', FRAME, row['frames']), ('table', 4, row['steps'] + STEP * row['step_count'])):
        n, eq = disc_equal(got[what + '_built'], got[what], mod, base, size)
        disc.append(f'{what} {eq}/{n} ({size * eq} B)')
        totals[0] += n
        totals[1] += eq
    own = sum(n for _a, n in row['own'])
    return (f'{name}: C ' + ', '.join(parts) + '; equal to the disc: ' + ', '.join(disc) +
            f'; steps of the PS1\'s own {own} in {len(row["own"])} spans; {ms:.1f} ms')


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help='disc image, raw 2352-byte sectors')
    ap.add_argument('--arcade', required=True, help="arcade set ZIP (MAME's sfa2u)")
    ap.add_argument('--only', help='one file, by name (PL00.PAC or PL00)')
    ap.add_argument('--detail', action='store_true', help='extra lines per file: the runs only one side has, the first difference of each kind')
    ap.add_argument('--c-table', action='store_true', help='print port/arcade/sequences_table.c (the rows of the files the runtime C builds)')
    ap.add_argument('--compare-c', action='store_true', help='build sequence.c with the checked-in tables and compare it with the model and the disc, per file')
    args = ap.parse_args(argv)
    started = time.monotonic()
    tmp = tempfile.mkdtemp(prefix='sequences-')
    try:
        prog_be, gfx = F.arcade_views(args.arcade)
        view = F.View(prog_be)
        index = F.Index(view)
        helper = F.Helper(tmp, gfx)
        tot = collections.Counter()
        found = []
        with open(args.disc, 'rb') as f:
            files, sec = F.iso_files(f)
            names = sorted(p for p in files if p.startswith('/PAC/PL') and p.endswith('.PAC'))
            if args.only:
                want = args.only.upper().removesuffix('.PAC') + '.PAC'
                names = [p for p in names if p.endswith('/' + want)]
            for p in names:
                lba, size = files[p]
                data = sec(lba, (size + 2047) // 2048)[:size]
                name = p.split('/')[-1]
                try:
                    parts = F.split_file(data)
                except (F.pac.FormatError, struct.error) as exc:
                    print(f'{name}: {exc}')
                    continue
                if isinstance(parts, str):
                    print(f'{name}: {parts}')
                    continue
                mod, tiles, _side, others = parts
                stem = name.removesuffix('.PAC').removesuffix('X')
                fr = F.analyse(mod, tiles, view, index, helper, others, stem in F.PALETTE_PARTS, stem in F.NUMBER_BY_RUN, F.hand_of(name))
                if 'error' in fr:
                    print(f'{name}: {fr["error"]}')
                    continue
                r = analyse(mod, fr, prog_be, view, LOAD[_side])
                blocks = [(0, name, r)]
                if second_block(name) is not None and 'error' not in r:
                    blocks.append((1, name + ' (second block)', analyse(mod, fr, prog_be, view, LOAD[_side], block=second_block(name))))
                if args.c_table or args.compare_c:
                    found += [(name, _side, mod, fr, rb, blk) for blk, _label, rb in blocks]
                    continue
                for blk, label, r in blocks:
                    print(line(label, r))
                    if 'error' not in r and blk:
                        tot['steps'] += len(r['steps'])
                        tot['exact'] += r['exact']
                        tot['scaled'] += r['scaled']
                        tot['tables'] += r['table_ps1']
                        tot['tables equal'] += r['table_match']
                        tot['frames'] += r['nframes']
                        tot['frames exact'] += r['fr_exact']
                        if args.detail:
                            for d in details(label, r):
                                print(d)
                r = blocks[0][2]
                if 'error' not in r:
                    tot['steps'] += len(r['steps'])
                    tot['exact'] += r['exact']
                    tot['scaled'] += r['scaled']
                    tot['tables'] += r['table_ps1']
                    tot['tables equal'] += r['table_match']
                    tot['frames'] += r['nframes']
                    tot['frames exact'] += r['fr_exact']
                if args.detail:
                    for d in details(name, r):
                        print(d)
        if args.c_table or args.compare_c:
            rows, omitted, context = [], [], {}
            for name, side, mod, fr, r, blk in found:
                drow, why = F.descriptor(name, side, fr)
                if drow is None:
                    omitted.append((name, f'not in fighters_table.c: {why}'))
                    continue
                row, why = make_row(name, side, r, blk)
                if row is None:
                    omitted.append((name + (' (second block)' if blk else ''), why))
                    continue
                rows.append(row)
                context[(name, blk)] = (drow, mod)
            text = c_table(rows, omitted)
            if args.c_table:
                sys.stdout.write(text)
            else:
                compare_c(args, rows, context, text, prog_be, view, tmp)
        elif tot['steps']:
            print(f'all: steps {tot["steps"]} ({STEP * tot["steps"]} B), exact {tot["exact"]} ({STEP * tot["exact"]} B) '
                  f'+ {tot["scaled"]} with scaled flags ({STEP * tot["scaled"]} B); '
                  f'frames {tot["frames"]} ({FRAME * tot["frames"]} B), exact {tot["frames exact"]} ({FRAME * tot["frames exact"]} B); '
                  f'table entries {tot["tables"]}, equal in order {tot["tables equal"]}')
        print(f'{len(names)} files in {time.monotonic() - started:.1f} s', file=sys.stderr if args.c_table else sys.stdout)
    finally:
        shutil.rmtree(tmp, ignore_errors=True)
    return 0


def compare_c(args, rows, context, text, prog_be, view, tmp):
    """--compare-c: the checked-in table against the generated one, then per file the C against the model and the disc."""
    here = Path(__file__).resolve().parent / 'sequences_table.c'
    print('sequences_table.c ' + ('is the generated one' if here.read_text() == text else 'DIFFERS from the generated one'))
    exe = compile_driver(tmp)
    prog_path = Path(tmp) / 'program.bin'
    prog_path.write_bytes(prog_be)
    model_lists = {}
    tot = collections.Counter()
    for row in rows:
        drow, mod = context[(row['file'], row['block'])]
        rules = drow['rules']
        key = (tuple(drow['segments']), tuple(rules), tuple(drow['pads']), tuple(drow['extras']))   # the second block of a file shares its lists
        if key not in model_lists:
            pads = {k: n for k, n in drow['pads']}
            extras = {a: (more, stream) for a, more, stream in drow['extras']}
            lists = F.model_build(view, drow['segments'], 'PALETTE_PARTS' in rules, 'NUMBER_BY_RUN' in rules, 'TERMINATOR' in rules, extras, pads)[0]
            model_lists[key] = model_offsets(lists, drow['segments'], view, pads)
        offsets = model_lists[key]
        model = model_build(prog_be, offsets, row)
        # the C's list offsets are sorted by record; the model's dict has the same content
        model['offsets'] = dict(sorted(offsets.items()))
        label = row['file'] + ('#%d' % row['block'] if row['block'] else '')
        got = run_c(exe, prog_path, label, tmp)
        if isinstance(got, str):
            print(compare_line(label, row, got, 0, model, mod))
            continue
        out, ms = got
        print(compare_line(label, row, out, ms, model, mod))
        for what, size, base in (('steps', STEP, row['steps']), ('frames', FRAME, row['frames']), ('table', 4, row['steps'] + STEP * row['step_count'])):
            n, eq = disc_equal(out[what + '_built'], out[what], mod, base, size)
            tot[what + ' built'] += n * size
            tot[what + ' equal'] += eq * size
            tot[what + ' size'] += len(out[what])
        tot['time'] += ms
    print('all: ' + '; '.join(f'{w} {tot[w + " equal"]} of {tot[w + " built"]} built bytes equal the disc ({tot[w + " size"]} B in all)'
                              for w in ('steps', 'frames', 'table')) + f'; C time {tot["time"]:.1f} ms for {len(rows)} files')


if __name__ == '__main__':
    sys.exit(main())
