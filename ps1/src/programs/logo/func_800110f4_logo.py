"""Contract of func_800110f4_logo: a0 = table of 3-byte entries, a1 = stream of 16-bit codes ended by 0xffff, no result.

Choices of the setup (the format and the reads and writes are in func_800110f4_logo.c):
  - the table has 16 entries of 3 random bytes; a literal code is an index
    from 0 to 15 (the largest index tried is 15, so no byte beyond the table is read);
  - the stream has 0 to 12 codes before the terminator (0 in about one case
    in twelve, so the check before the first code is tried), each a literal
    in about two cases of three, else a repeat with a count from 0 to 6 (0 in
    one of seven); the counts are small, so that the output stays under
    12 * 6 * 3 = 216 bytes and a case runs a few hundred instructions;
  - the stream is aligned, and the halfwords after the terminator are random
    (the function must not read them);
  - the output block is 216 bytes plus 16 of slack on each side, all random,
    and the pointer word at 0x8002eb10 points at the start of the 216; the
    slack shows a write beyond the data's end or before its start;
  - the pointer word's own neighbours are random.
"""
from contracts import Contract, Setup, fill

MAX_CODES = 12
MAX_COUNT = 6


def setup(state, rng, sym):
    pointer = sym["data_8002eb10_logo"]
    fill(state, pointer - 8, 20, rng)
    table = state.alloc(48)
    fill(state, table, 48, rng)
    count = 0 if rng.random() < 1 / 12 else rng.randrange(1, MAX_CODES + 1)
    codes = []
    for _ in range(count):
        if rng.random() < 2 / 3:
            codes.append(rng.randrange(16))
        else:
            codes.append(0x8000 | (0 if rng.random() < 1 / 7 else rng.randrange(1, MAX_COUNT + 1)))
    stream = state.alloc(2 * (count + 1) + 8)
    fill(state, stream, 2 * (count + 1) + 8, rng)
    for index, code in enumerate(codes):
        state.w16(stream + 2 * index, code)
    state.w16(stream + 2 * count, 0xFFFF)
    block = state.alloc(216 + 32)
    fill(state, block, 216 + 32, rng)
    state.w32(pointer, block + 16)
    return Setup(args=(table, stream), returns_value=False)


def control(words):
    """Alter the test of the repeat bit: `andi rt,rs,0x8000` becomes `andi rt,rs,0x4000`."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0C and w & 0xFFFF == 0x8000]
    if len(found) != 1:
        raise ValueError(f"expected one andi with 0x8000, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x4000, "the repeat bit is bit 14 instead of bit 15"


CONTRACT = Contract(setup, control)
