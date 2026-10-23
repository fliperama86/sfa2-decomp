#!/usr/bin/env python3
"""Analysis tool: the PS1's sprite tile compression, as a decoder and an encoder, checked against the disc.

What it does. A fighter's sprite chunk (table 0 slot 1 or 2 of PLnn.PAC and PLnnX.PAC) is a table of one 32-bit offset per tile
and then one block per tile, in tile order, with no padding between blocks or after the last one (the chunk's size is the end of the
last block). A tile is 64 little-endian 16-bit words (16 rows of 4 words, 4 bits a pixel, low nibble first). The reader is
`decode_compressed_block` (ps1/src/resident/disc/); a block starts with a 16-bit n:

  n = 0              64 literal words (130 bytes with n);
  n >> 8 == 0        LZ: n items, the flag bits of the items (LSB first, 16 a word, ceil(n/16) words), then one word an item:
                     a literal, or a back reference w: length (w & 3) + 2 words copied from `block start - (w >> 2)` words,
                     that is from the stream that precedes the block (the previous blocks' words as stored, their flag words
                     and n words included, the offset table not);
  n >> 8 == 0x80     64 flag bits (4 words), then one word for each bit set; a bit clear is a zero word;
  anything else      RLE (decode_rle): n & 0xff items, flag bits as above, then bytes: a flag set is a (value, count) pair,
                     a flag clear is one literal byte. The 22 sprite chunks of the disc contain no RLE block.
The flip variants (bit 15 of the call's flags: the nibbles of each word reversed) belong to the reader, not to the stored data.

What the disc's data shows (all 22 distinct chunks; the numbers are in `report` below):
  - the order of tiles is the order of the table; blocks are adjacent; offsets are the running sum of the block sizes;
  - form: zero form when 14 or more of the 64 words are zero (its size, 10 + 2 * non-zero words, is then at most 110 bytes);
    LZ when its size (2 + 2 * flag words + 2 * items) is at most 108 bytes and strictly less than the zero form's; otherwise 130
    bytes of literals. A tie between LZ and zero goes to the zero form. (Inferred from the sizes: no LZ block is 110 or more, no
    zero block 112 or more, and no tile with fewer than 14 zero words is a zero block.)
  - LZ references: length 2 to 5, the longest match against the stream before the block within 16,383 words, and among equally
    long ones the FARTHEST (24,270 of the 24,314 references of Ryu's chunk are the finder's choice; the other 44 are at the last
    positions of a tile, where the source is in the longest set but not the farthest). A match at position p is cut to
    (65 - p) // 2 words after it is found, so 61 is the last position that can start a reference (2 words) and the last word of
    a tile is always a literal.
  - the flag words: the encoder writes only the bit of each item into four flag words that it never clears, and it attempts the LZ
    form on every tile, whichever form it keeps. The unused high bits of the last flag word of a block therefore hold what the
    previous tile's attempt left (868 of the 1,293 LZ blocks of Ryu's chunk that have unused bits have some of them set, and later tiles match against them).
    `Stream.flags` models this; it is exact only when the attempts before it were.
  - the first and the unnumbered tiles: tile 0 is the all-zero tile (a 10-byte zero block); the last few tiles of a chunk (0 to 13)
    are not in the fighters' numbering: they are the PS1's own art and a loader has no arcade source for them.
  - what is NOT reproduced, and why the byte-exact chunk is out of reach: the disc's encoder leaves about 8% of the references that
    the rules above would make as literals (Ryu's chunk: 24,314 references made, 2,081 more would have been made), in runs: when it
    skips a reference, 80% of the references that continue the same match right after it are skipped too. Per file between 43%
    and 71% of the blocks come out byte-equal (Ryu's: 62%, forms equal for 4,021 of 4,288 tiles); the result is a valid chunk,
    within 1.5% of the disc's size either way, that the reader decodes to the same tiles. The skips are not causal: in PL10 and
    PL16 (tile 12) and in PL11 and PL17 (tile 41) the same tile after the same stream is encoded differently, and 84% of the
    skipped references in Ryu's chunk would have replaced a literal word that a LATER tile copies (14% of the other literals are
    copied later). `--oracle-pins` keeps exactly those words literal, using the disc's later references (a diagnostic: it reads
    disc bytes and is circular); blocks equal then rise only to 66% for Ryu's chunk, so that is a clue and not the rule. No
    simple rule tried explains the rest (lazy matching, a chain or time budget, hash collisions, the source's block, form or
    alignment, popularity of the content, parity and position in the tile all fail; measured on Ryu's chunk and, for the causality, on the PL0D/PL15, PL10/PL16 and PL11/PL17 pairs).
    The reader looks tiles up through the offset table only, so a differing chunk is functionally the same.

Inputs: the disc image (raw 2352-byte sectors) and, for the arcade step, the arcade set's ZIP (MAME's sfa2u), both read only. It
writes nothing. It prints sizes, indexes and counts, never game bytes. It is an analysis and reference tool; the runtime converter
will be C. The fighters' tile numbering and flips (`fighters.py`, the C `fighter.c`) are used as they are.

    python3 port/arcade/tilepack.py --disc BIN [--arcade ZIP] [--only NAME] [--oracle-pins]
"""
import argparse
import bisect
import collections
import struct
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

TILE_WORDS = 64
MAX_BACK = 16383          # 14 bits of the reference word
MIN_LEN, MAX_LEN = 2, 5
ZERO_LIMIT = 110          # bytes: the largest zero-form block seen (14 zero words)
LZ_LIMIT = 108            # bytes: the largest LZ block seen
LIT_SIZE = 2 + 2 * TILE_WORDS


# ---------------------------------------------------------------- the reader's side

def decode_block(chunk, offset):
    """(form, 64 words) of the block at `offset` of the chunk. Forms: 'lit', 'lz', 'zero', 'rle'. Raises ValueError when a
    block does not decode to exactly 64 words (the reader does not check; this tool does)."""
    (n,) = struct.unpack_from('<H', chunk, offset)
    p = offset + 2
    if n == 0:
        return 'lit', list(struct.unpack_from('<64H', chunk, p))
    if n >> 8 == 0:
        flags, p = _flags(chunk, p, n)
        out = []
        for i in range(n):
            (w,) = struct.unpack_from('<H', chunk, p)
            p += 2
            if flags >> i & 1:
                s = offset - 2 * (w >> 2)
                if s < 0:
                    raise ValueError('reference before the chunk')
                out += struct.unpack_from('<%dH' % ((w & 3) + 2), chunk, s)
            else:
                out.append(w)
        if len(out) != TILE_WORDS:
            raise ValueError('LZ block of %d words' % len(out))
        return 'lz', out
    if n >> 8 == 0x80:
        flags = int.from_bytes(chunk[p:p + 8], 'little')
        p += 8
        out = []
        for i in range(TILE_WORDS):
            if flags >> i & 1:
                out.append(struct.unpack_from('<H', chunk, p)[0])
                p += 2
            else:
                out.append(0)
        return 'zero', out
    k = n & 0xff
    flags, p = _flags(chunk, p, k)
    data = bytearray()
    for i in range(k):
        if flags >> i & 1:
            data += bytes([chunk[p]]) * chunk[p + 1]
            p += 2
        else:
            data.append(chunk[p])
            p += 1
    if len(data) != 2 * TILE_WORDS:
        raise ValueError('RLE block of %d bytes' % len(data))
    return 'rle', list(struct.unpack('<64H', bytes(data)))


def _flags(chunk, p, k):
    """(flag bits as one integer, offset after them) for k items: ceil(k / 16) words."""
    nw = (k - 1 >> 4) + 1 if k else 0
    return int.from_bytes(chunk[p:p + 2 * nw], 'little'), p + 2 * nw


def decode_chunk(chunk):
    """[(form, 64 words)] of every tile of a sprite chunk, in table order."""
    first = struct.unpack_from('<I', chunk, 0)[0]
    return [decode_block(chunk, struct.unpack_from('<I', chunk, 4 * i)[0]) for i in range(first // 4)]


def block_sizes(chunk):
    """[(offset, size)] of the blocks; the last one ends at the chunk's end."""
    first = struct.unpack_from('<I', chunk, 0)[0]
    offs = struct.unpack_from('<%dI' % (first // 4), chunk, 0)
    return [(o, (offs[i + 1] if i + 1 < len(offs) else len(chunk)) - o) for i, o in enumerate(offs)]


# ---------------------------------------------------------------- the encoder

class Stream:
    """The words of the blocks written so far (the offset table is not part of it), and an index of every adjacent pair of
    words, as the position list of that pair in increasing order."""

    def __init__(self):
        self.words = []
        self.pairs = collections.defaultdict(list)
        self.flags = [0, 0, 0, 0]      # the encoder's flag words, which are not cleared between tiles (see lz_block)

    def append(self, block):
        base = len(self.words)
        self.words += block
        w, pairs = self.words, self.pairs
        for i in range(max(0, base - 1), len(w) - 1):
            pairs[(w[i], w[i + 1])].append(i)


def find_match(stream, tile, pos, start, longest):
    """(length, distance) of the longest match of tile[pos:] against the stream words before `start` (within 16,383 words),
    at most `longest` words, at least 2; among equally long ones the farthest. None when there is none."""
    longest = min(longest, MAX_LEN, TILE_WORDS - pos)
    if longest < MIN_LEN:
        return None
    lst = stream.pairs.get((tile[pos], tile[pos + 1]))
    if not lst:
        return None
    w = stream.words
    best, dist = 0, 0
    for s in lst[bisect.bisect_left(lst, start - MAX_BACK):]:
        if s + 2 > start:
            break
        n = 2
        while n < longest and s + n < start and w[s + n] == tile[pos + n]:
            n += 1
        if n > best:
            best, dist = n, start - s
            if n == longest:
                break
    return (best, dist) if best >= MIN_LEN else None


def lz_parse(stream, tile, start, pins=()):
    """[(position, length, distance)] of the greedy forward parse: length 1 is a literal (distance 0). At each position the
    longest farthest match is taken, cut to (65 - position) // 2 words; a position in `pins` stays a literal and no reference
    covers one."""
    items, pos = [], 0
    while pos < TILE_WORDS:
        if pos not in pins:
            room = next((q for q in range(pos + 1, TILE_WORDS) if q in pins), TILE_WORDS) - pos if pins else TILE_WORDS
            m = find_match(stream, tile, pos, start, room)
            if m:
                length = min(m[0], (65 - pos) // 2)
                if length >= MIN_LEN:
                    items.append((pos, length, m[1]))
                    pos += length
                    continue
        items.append((pos, 1, 0))
        pos += 1
    return items


def lz_block(tile, items, flags):
    """The words of an LZ block for a parse. `flags` is the encoder's four flag words, updated here: an item writes its own
    bit (set for a reference, clear for a literal) and nothing else, so the unused high bits of the last flag word of a block
    keep what the previous tile's attempt left there (the encoder attempts the LZ form on every tile, whichever form it keeps;
    868 of the 1,293 LZ blocks of Ryu's chunk that have unused bits have some of them set, and the stream the later tiles match against contains them)."""
    k = len(items)
    nw = (k - 1 >> 4) + 1
    body = []
    for i, (pos, length, dist) in enumerate(items):
        bit = 1 << (i & 15)
        if length > 1:
            flags[i >> 4] |= bit
            body.append(dist << 2 | length - 2)
        else:
            flags[i >> 4] &= ~bit
            body.append(tile[pos])
    return [k] + flags[:nw] + body


def zero_block(tile):
    flags = sum(1 << i for i, w in enumerate(tile) if w)
    return [0x8000] + [flags >> (16 * j) & 0xffff for j in range(4)] + [w for w in tile if w]


def lit_block(tile):
    return [0] + list(tile)


def encode_tile(stream, tile, pins=()):
    """(block words, form) for a tile after the stream: the rule of the module docstring."""
    zeros = sum(1 for w in tile if not w)
    zero_size = 10 + 2 * (TILE_WORDS - zeros)
    items = lz_parse(stream, tile, len(stream.words), pins)
    lz_size = 2 + 2 * ((len(items) - 1 >> 4) + 1) + 2 * len(items)
    zero_ok = zero_size <= ZERO_LIMIT
    attempt = lz_block(tile, items, stream.flags)        # the attempt writes the flag words whether or not it is kept
    if lz_size <= LZ_LIMIT and (not zero_ok or lz_size < zero_size):
        return attempt, 'lz'
    if zero_ok:
        return zero_block(tile), 'zero'
    return lit_block(tile), 'lit'


def encode_chunk(tiles, pins=None):
    """(chunk bytes, forms) for a list of 64-word tiles. `pins` maps a tile index to the set of positions that must stay
    literal (the diagnostic of `--oracle-pins`); None encodes without them."""
    stream, blocks, forms = Stream(), [], []
    for i, tile in enumerate(tiles):
        block, form = encode_tile(stream, tile, pins.get(i, ()) if pins else ())
        stream.append(block)
        blocks.append(block)
        forms.append(form)
    offsets, at = [], 4 * len(tiles)
    for block in blocks:
        offsets.append(at)
        at += 2 * len(block)
    words = [w for block in blocks for w in block]
    return struct.pack('<%dI' % len(tiles), *offsets) + struct.pack('<%dH' % len(words), *words), forms


def encode_rle(tile):
    """An RLE block for a tile (runs of 3 or more equal bytes as pairs). No sprite chunk uses this form, `encode_tile` never
    picks it, and its high byte of n (here 1) is only known to be neither 0 nor 0x80: it exists to test the reader's path."""
    data = struct.pack('<64H', *tile)
    items, i = [], 0
    while i < len(data):
        j = i
        while j < len(data) and data[j] == data[i] and j - i < 255:
            j += 1
        if j - i >= 3:
            items.append((True, data[i], j - i))
            i = j
        else:
            items.append((False, data[i], 1))
            i += 1
    flags = sum(1 << k for k, it in enumerate(items) if it[0])
    nw = (len(items) - 1 >> 4) + 1
    body = bytearray(flags.to_bytes(2 * nw, 'little'))
    for run, value, count in items:
        body += bytes([value, count]) if run else bytes([value])
    return struct.pack('<H', 0x100 | len(items)) + bytes(body)


# ---------------------------------------------------------------- tiles from the arcade's graphics

def _spread():
    """spread[b]: the 8 bits of b, bit 7 first, one to each nibble of a 32-bit value (pixel x in nibble x)."""
    return [sum((b >> (7 - i) & 1) << (4 * i) for i in range(8)) for b in range(256)]


SPREAD = _spread()


def arcade_tile(gfx, code):
    """The 128 bytes of arcade tile `code` as the PS1 stores a tile: 4 bits a pixel in planes there, the colour index plus one
    (modulo 16) here, 16 rows of 8 bytes, pixel x of a row in nibble x (low nibble first)."""
    q = gfx[code * 128:code * 128 + 128]
    out = bytearray()
    for y in range(16):
        for half in range(2):
            b = q[y * 8 + half * 4:y * 8 + half * 4 + 4]
            v = SPREAD[b[0]] | SPREAD[b[1]] << 1 | SPREAD[b[2]] << 2 | SPREAD[b[3]] << 3
            v = ((v & 0x77777777) + 0x11111111) ^ ((v ^ 0x11111111) & 0x88888888)     # +1 in every nibble, modulo 16
            out += v.to_bytes(4, 'little')
    return bytes(out)


def flip_tile(data, flip):
    """The 128 bytes flipped across (flip & 1) and/or down (flip & 2)."""
    px = [b >> (4 * i) & 15 for b in data for i in (0, 1)]
    out = [0] * 256
    for y in range(16):
        for x in range(16):
            out[y * 16 + x] = px[(15 - y if flip & 2 else y) * 16 + (15 - x if flip & 1 else x)]
    return bytes(out[2 * k] | out[2 * k + 1] << 4 for k in range(128))


def tile_words(data):
    return list(struct.unpack('<64H', data))


# ---------------------------------------------------------------- diagnostics

def disc_pins(chunk, tiles):
    """{tile: set of positions} whose literal word in the chunk a LATER tile's reference covers (the pins of the module docstring).
    Reads the disc's own references: a diagnostic, not an input a loader has."""
    sizes = block_sizes(chunk)
    cover = collections.Counter()
    owner = {}
    for i, (o, _size) in enumerate(sizes):
        (n,) = struct.unpack_from('<H', chunk, o)
        p = o // 2 + 1
        if n == 0:
            for j in range(TILE_WORDS):
                owner[p + j] = (i, j)
        elif n >> 8 == 0:
            flags, _ = _flags(chunk, o + 2, n)
            p += (n - 1 >> 4) + 1
            pos = 0
            for k in range(n):
                (w,) = struct.unpack_from('<H', chunk, 2 * (p + k))
                if flags >> k & 1:
                    for t in range((w & 3) + 2):
                        cover[o // 2 - (w >> 2) + t] += 1
                    pos += (w & 3) + 2
                else:
                    owner[p + k] = (i, pos)
                    pos += 1
        elif n >> 8 == 0x80:
            flags = int.from_bytes(chunk[o + 2:o + 10], 'little')
            q = p + 4
            for j in range(TILE_WORDS):
                if flags >> j & 1:
                    owner[q] = (i, j)
                    q += 1
    pins = collections.defaultdict(set)
    for word, (i, j) in owner.items():
        if cover[word]:
            pins[i].add(j)
    return pins


def compare_chunks(ours, disc):
    """(tiles equal, count, first different tile or None, its disc form and our form)."""
    a, b = block_sizes(ours), block_sizes(disc)
    equal, first = 0, None
    for i, ((oa, sa), (ob, sb)) in enumerate(zip(a, b)):
        if ours[oa:oa + sa] == disc[ob:ob + sb]:
            equal += 1
        elif first is None:
            first = i
    return equal, len(b), first


def form_name(chunk, i):
    o = block_sizes(chunk)[i][0]
    (n,) = struct.unpack_from('<H', chunk, o)
    return 'lit' if n == 0 else 'lz' if n >> 8 == 0 else 'zero' if n >> 8 == 0x80 else 'rle'


def report(name, disc, oracle, numbers=None, gfx=None):
    """The lines for one distinct sprite chunk."""
    started = time.monotonic()
    decoded = decode_chunk(disc)
    tiles = [w for _f, w in decoded]
    forms = collections.Counter(f for f, _w in decoded)
    ours, mine_forms = encode_chunk(tiles)
    equal, count, first = compare_chunks(ours, disc)
    agree = sum(1 for i, f in enumerate(mine_forms) if f == decoded[i][0])
    lz_total = forms['lz']
    lines = [f'{name}: {count} tiles ({", ".join(f"{k} {v}" for k, v in sorted(forms.items()))}); disc {len(disc)} B, ours {len(ours)} B '
             f'({100.0 * (len(ours) - len(disc)) / len(disc):+.1f}%); forms agree {agree}/{count}; blocks equal {equal}/{count}'
             + ('' if first is None else f'; first difference tile {first} (disc {decoded[first][0]}, ours {mine_forms[first]})')
             + f'; chunk {"EQUAL" if ours == disc else "differs"}']
    if oracle:
        pins = disc_pins(disc, tiles)
        ours2, forms2 = encode_chunk(tiles, pins)
        equal2, _count, first2 = compare_chunks(ours2, disc)
        lines.append(f'  with the disc\'s own later references as pins: blocks equal {equal2}/{count}, ours {len(ours2)} B'
                     + ('' if first2 is None else f'; first difference tile {first2} (disc {decoded[first2][0]}, ours {forms2[first2]})')
                     + f'; chunk {"EQUAL" if ours2 == disc else "differs"}')
    if numbers is not None:
        eq = bad = 0
        first_bad = None
        mine = list(tiles)
        for n, (code, flip) in sorted(numbers.items()):
            if n >= len(tiles):
                continue
            data = arcade_tile(gfx, code)
            if flip:
                data = flip_tile(data, flip)
            words = tile_words(data)
            if words == tiles[n]:
                eq += 1
            else:
                bad += 1
                first_bad = first_bad if first_bad is not None else n
            mine[n] = words
        extra = count - 1 - len(numbers)
        ours3, _f3 = encode_chunk(mine)
        lines.append(f'  tiles from the arcade (fighters numbering): {eq} of {eq + bad} numbered tiles equal the disc\'s'
                     + ('' if first_bad is None else f'; first different tile {first_bad}')
                     + f'; tile 0 and {extra} unnumbered tiles taken from the disc; chunk {"EQUAL to the disc" if ours3 == disc else "differs from the disc"}'
                     + f', {"equal to" if ours3 == ours else "differs from"} the encoding of the disc\'s tiles')
    lines[-1] += f'   [{time.monotonic() - started:.1f} s]'
    return lines


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--disc', required=True, help='disc image, raw 2352-byte sectors')
    ap.add_argument('--arcade', help="arcade set ZIP (MAME's sfa2u): also build the tiles from the arcade graphics")
    ap.add_argument('--only', help='one file, by name (PL00 or PL00.PAC)')
    ap.add_argument('--oracle-pins', action='store_true', help="also encode with the disc's own later references as pins (diagnostic)")
    args = ap.parse_args(argv)
    sys.path.insert(0, str(ROOT / 'port/arcade'))
    sys.path.insert(0, str(ROOT / 'ps1/tools'))
    import fighters
    import pac
    started = time.monotonic()
    gfx = rows = clib = prog = None
    import tempfile
    tmp = tempfile.mkdtemp(prefix='tilepack-')
    try:
        if args.arcade:
            prog, gfx = fighters.arcade_views(args.arcade)
            clib = fighters.CLib(tmp)
        seen = {}
        with open(args.disc, 'rb') as f:
            files, sec = fighters.iso_files(f)
            names = sorted(p for p in files if p.startswith('/PAC/PL') and p.endswith('.PAC'))
            if args.only:
                want = args.only.upper().removesuffix('.PAC') + '.PAC'
                names = [p for p in names if p.endswith('/' + want)]
            for p in names:
                lba, size = files[p]
                data = sec(lba, (size + 2047) // 2048)[:size]
                name = p.split('/')[-1]
                chunks = [c for c in pac.chunk_table(data) if c['table'] == 0 and c['slot'] in (1, 2)]
                if len(chunks) != 1:
                    print(f'{name}: {len(chunks)} sprite chunks')
                    continue
                chunk = data[chunks[0]['offset']:chunks[0]['offset'] + chunks[0]['size']]
                if chunk in seen:
                    print(f'{name}: same chunk as {seen[chunk]}')
                    continue
                seen[chunk] = name
                numbers = None
                if clib is not None:
                    built = clib.build(prog, name)
                    if built is not None and not isinstance(built, str):
                        numbers = {n: (code, built[3][n]) for n, code in built[2].items()}
                    elif isinstance(built, str):
                        print(f'{name}: the C builder refused: {built}')
                for line in report(name, chunk, args.oracle_pins, numbers, gfx):
                    print(line)
        print(f'{len(seen)} distinct chunks in {time.monotonic() - started:.1f} s')
    finally:
        import shutil
        shutil.rmtree(tmp, ignore_errors=True)
    return 0


if __name__ == '__main__':
    sys.exit(main())
