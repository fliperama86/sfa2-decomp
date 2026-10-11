"""Contract of func_80011458_z2o: no arguments, the result in v0 is 1 when one word of the program's data is not below another (unsigned), else 0.

Choices of the setup (reads and writes are listed in func_80011458_z2o.c):
  - the two words (the latest frame number at 0x8002f894 and the frame count at 0x8002f814) are random 32 bit
    words in some cases; in the others they are equal, differ by 1 either way, or one of them is 0 or 0xffffffff,
    so that the edge of the comparison and the unsigned order (a word with its top bit set against one without)
    are both tried;
  - the bytes around both words are random, so that a neighbouring read shows.
"""
from contracts import Contract, Setup, fill


def setup(state, rng, sym):
    latest = sym["data_8002f894_z2o"]
    count = sym["data_8002f814_z2o"]
    fill(state, latest - 8, 20, rng)
    fill(state, count - 8, 20, rng)
    a = rng.getrandbits(32)
    kind = rng.randrange(8)
    if kind == 0:
        b = a
    elif kind == 1:
        b = (a + 1) & 0xFFFFFFFF
    elif kind == 2:
        b = (a - 1) & 0xFFFFFFFF
    elif kind == 3:
        a, b = rng.choice((0, 0xFFFFFFFF)), rng.getrandbits(32)
    elif kind == 4:
        a, b = rng.getrandbits(32), rng.choice((0, 0xFFFFFFFF))
    else:
        b = rng.getrandbits(32)
    state.w32(latest, a)
    state.w32(count, b)
    return Setup(args=(), returns_value=True)


def control(words):
    """Alter the comparison: `sltu` (unsigned) becomes `slt` (signed)."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0 and w & 0x3F == 0x2B]
    if len(found) != 1:
        raise ValueError(f"expected one sltu, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0x3F) | 0x2A, "the comparison is signed instead of unsigned"


CONTRACT = Contract(setup, control)
