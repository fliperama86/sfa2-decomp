"""Contract of func_80011708_z2o: no arguments, no result; the decoder's completion routine (draws a strip, starts the next).

Choices of the setup (reads and writes are listed in func_80011708_z2o.c):
  - the count word at 0x8002f888 is a random word, and in a quarter of the
    cases the word just below the limit (so that the count after the add is
    the limit minus 1, the limit, or the limit plus 1, or the wrap through
    0xffffffff); the limit at 0x8002f88c is then set around the count after
    the add (below it, equal, above it) in three cases of five, else random;
    the comparison is unsigned;
  - the word at 0x80031e68 is 0 in half the cases, else a random non-zero
    word (StCdInterrupt is then called and the word cleared);
  - the word at 0x8002f870 is 0 in half the cases (the copy to the picture
    is then made), else a random non-zero word;
  - the word at 0x8002f818 is 0 or a random non-zero word, one case in two;
  - the rectangle's x (the halfword at 0x8002f874) is one of the values
    around the signed halfword's wrap (x + 0x10 or x + 0x18 crosses 0x8000)
    in half the cases, else a random halfword; its other six bytes are random;
  - the buffer pointer, the length word and the size word (0x8002f87c,
    0x8002f880, 0x8002f884) are random words (the callees are recorders, so
    the buffer is never dereferenced); the flag word at 0x8002f890 holds a
    random word before the call, so that its store (1 or 0) shows;
  - the bytes around each of these words are random, so that a neighbouring
    read or write, a store of the wrong width, or a store that is missing
    would show;
  - the callees are recorders: StCdInterrupt (0 arguments), func_80157fc4 (2
    arguments, the rectangle's two words behind the first), DecDCTout (2
    arguments); the eight words from the rectangle to the flag, and the
    word at 0x80031e68, are watched.
"""
from contracts import CallLog, Contract, Setup, fill

EDGES = (0x7FFF - 0x18, 0x7FFF - 0x17, 0x8000 - 0x18, 0x7FFF - 0x10, 0x7FFF - 0x0F, 0x8000 - 0x10, 0xFFFF, 0xFFF0, 0xFFE8, 0, 0x7FFF, 0x8000)


def setup(state, rng, sym):
    count = sym["data_8002f888_z2o"]
    limit = sym["data_8002f88c_z2o"]
    flag31 = sym["data_80031e68_z2o"]
    param = sym["data_8002f870_z2o"]
    rect = sym["data_8002f874_z2o"]
    step = sym["data_8002f818_z2o"]
    done = limit + 4  # 0x8002f890
    fill(state, flag31 - 8, 20, rng)
    fill(state, step - 8, 20, rng)
    fill(state, param - 8, 16 + 8 * 4 + 24, rng)  # from 0x8002f868 up to 0x8002f8a8
    state.w32(flag31, 0 if rng.random() < 0.5 else rng.randrange(1, 1 << 32))
    state.w32(param, 0 if rng.random() < 0.5 else rng.randrange(1, 1 << 32))
    state.w32(step, 0 if rng.random() < 0.5 else rng.randrange(1, 1 << 32))
    x = rng.choice(EDGES) if rng.random() < 0.5 else rng.getrandbits(16)
    state.w16(rect, x)
    for offset in (4, 8, 12, 16):  # 0x8002f878, 0x8002f87c, 0x8002f880, 0x8002f884
        state.w32(rect + offset, rng.getrandbits(32))
    if rng.random() < 0.25:
        before = (0xFFFFFFFF - rng.randrange(3)) & 0xFFFFFFFF  # the add wraps, or lands on the top
    else:
        before = rng.getrandbits(32)
    state.w32(count, before)
    after = (before + 1) & 0xFFFFFFFF
    if rng.random() < 0.6:
        state.w32(limit, (after + rng.choice((-2, -1, 0, 1, 2))) & 0xFFFFFFFF)
    else:
        state.w32(limit, rng.getrandbits(32))
    state.w32(done, rng.getrandbits(32))
    log = CallLog(state, words=128, watch=((rect, 8), (flag31, 1)))
    log.replace(sym["StCdInterrupt"], 0)
    log.replace(sym["func_80157fc4"], 2, pointees={0: 2})
    log.replace(sym["DecDCTout"], 2)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the step of the rectangle: the `addiu rt,rs,0x18` (not on sp) becomes 0x19."""
    found = [i for i, w in enumerate(words) if w >> 26 == 9 and w & 0xFFFF == 0x18 and (w >> 21) & 31 != 29]
    if len(found) != 1:
        raise ValueError(f"expected one add of 0x18, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x19, "the rectangle moves by 25 pixels instead of 24"


CONTRACT = Contract(setup, control)
