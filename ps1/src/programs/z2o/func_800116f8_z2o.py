"""Contract of func_800116f8_z2o: no arguments, the result in v0 is a word of the program's data.

Choices of the setup (reads and writes are listed in func_800116f8_z2o.c):
  - the word data_8002f8cc_z2o is a random 32 bit word, so that a read narrower than a word shows;
  - the bytes around it are random, so that a neighbouring read shows.
"""
from contracts import Contract, Setup, fill


def setup(state, rng, sym):
    target = sym["data_8002f8cc_z2o"]
    fill(state, target - 8, 20, rng)
    return Setup(args=(), returns_value=True)


def control(words):
    """Alter the load: `lw` becomes `lh`."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x23]
    if len(found) != 1:
        raise ValueError(f"expected one lw, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~(0x3F << 26)) | (0x21 << 26), "the load is a halfword instead of a word"


CONTRACT = Contract(setup, control)
