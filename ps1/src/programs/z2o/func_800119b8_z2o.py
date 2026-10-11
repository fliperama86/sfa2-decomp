"""Contract of func_800119b8_z2o: a0 = a value, no result; stores it at a word of the program's data.

Choices of the setup (reads and writes are listed in func_800119b8_z2o.c):
  - the argument is a random 32 bit word;
  - the bytes around the target are random before the call, so that a narrower or a wider store shows.
"""
from contracts import Contract, Setup, fill


def setup(state, rng, sym):
    target = sym["data_8002f870_z2o"]
    fill(state, target - 8, 16, rng)
    return Setup(args=(rng.getrandbits(32),), returns_value=False)


def control(words):
    """Alter the store: `sw` becomes `sh`."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x2B]
    if len(found) != 1:
        raise ValueError(f"expected one sw, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~(0x3F << 26)) | (0x29 << 26), "the store is a halfword instead of a word"


CONTRACT = Contract(setup, control)
