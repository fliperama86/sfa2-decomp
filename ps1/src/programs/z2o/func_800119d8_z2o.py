"""Contract of func_800119d8_z2o: a0 = a value, no result; stores it in a word of the program's data.

Choices of the setup (reads and writes are listed in func_800119d8_z2o.c):
  - the argument is a random 32 bit word, so that a store narrower than a word shows;
  - the bytes around the word are random before the call, so that a neighbouring write shows.
"""
from contracts import Contract, Setup, fill


def setup(state, rng, sym):
    target = sym["data_8002f81c_z2o"]
    fill(state, target - 8, 20, rng)
    return Setup(args=(rng.getrandbits(32),), returns_value=False)


def control(words):
    """Alter the store: `sw` becomes `sh`."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x2B]
    if len(found) != 1:
        raise ValueError(f"expected one sw, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~(0x3F << 26)) | (0x29 << 26), "the store is a halfword instead of a word"


CONTRACT = Contract(setup, control)
