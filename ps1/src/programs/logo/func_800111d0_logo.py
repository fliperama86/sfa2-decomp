"""Contract of func_800111d0_logo: a0 = x, a1 = y, no result; draws a still picture at a place.

Choices of the setup (reads and writes are listed in func_800111d0_logo.c):
  - the pointer p at 0x8002eb10 is the address of a block of 0x30 bytes of
    random content that the setup made; the pointer q at 0x8002eb14 is the
    address of a block of 0x500 * 12 bytes that the setup made (its content is
    not read by any case, it is written whole);
  - x and y are, in 40 cases of 100, random 16-bit values, in 30 random
    32-bit words (the upper half is not stored), in 30 values at the edges of
    the halfword arithmetic (where y + 0x10, y + 0x60 or x + 0x48 wraps at
    0x8000 or 0x10000);
  - the rectangle at 0x8002eb24 and the bytes around it, around the two
    pointers and around the program's other words are random, so that a
    neighbouring read or write would show;
  - the callees are recorders: func_80157fc4 (2 arguments, the two words
    behind the first), DrawSync (1 argument); the rectangle's two words are
    watched, so that the log shows their values at every call.
"""
from contracts import CallLog, Contract, Setup, fill

EDGES = (0, 1, 0x10, 0x7FFF, 0x8000, 0xFFFF, 0x7FFF - 0x48, 0x8000 - 0x48, 0x10000 - 0x48, 0x7FFF - 0x60, 0x8000 - 0x60,
         0x10000 - 0x60, 0x7FFF - 0x100, 0x8000 - 0x100, 0x10000 - 0x100, 0x7FFF - 0xE0, 0x8000 - 0xE0)


def value(rng):
    pick = rng.random()
    if pick < 0.4:
        return rng.getrandbits(16)
    if pick < 0.7:
        return rng.getrandbits(32)
    return (rng.choice(EDGES) + rng.choice((-1, 0, 1))) & 0xFFFFFFFF


def setup(state, rng, sym):
    p_word = sym["data_8002eb10_logo"]
    q_word = sym["data_8002eb14_logo"]
    rect = sym["data_8002eb24_logo"]
    fill(state, p_word - 8, 24, rng)
    fill(state, rect - 8, 24, rng)
    p = state.alloc(0x30)
    fill(state, p, 0x30, rng)
    q = state.alloc(0x500 * 12)
    state.w32(p_word, p)
    state.w32(q_word, q)
    log = CallLog(state, words=320, watch=((rect, 2),))
    log.replace(sym["func_80157fc4"], 2, pointees={0: 2})
    log.replace(sym["DrawSync"], 1)
    return Setup(args=(value(rng), value(rng)), returns_value=False)


def control(words):
    """Alter the number of rows: the compare with 0xf0 (`slti` or `sltiu rt,rs,0xf0`) becomes 0xe0."""
    found = [i for i, w in enumerate(words) if w >> 26 in (0x0A, 0x0B) and w & 0xFFFF == 0xF0]
    if len(found) != 1:
        raise ValueError(f"expected one compare with 0xf0, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0xE0, "one row less is drawn (the limit 0xe0 in place of 0xf0)"


CONTRACT = Contract(setup, control)
