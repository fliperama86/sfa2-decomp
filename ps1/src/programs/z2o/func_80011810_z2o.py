"""Contract of func_80011810_z2o: no arguments, no result; adds 1 to a word of the program's data.

Choices of the setup (reads and writes are listed in func_80011810_z2o.c):
  - the counter word is a random 32 bit word, so that the wrap at 0xffffffff is tried now and then;
  - the bytes around it are random, so that a neighbouring read or write shows.
"""
from contracts import Contract, Setup, fill


def setup(state, rng, sym):
    counter = sym["data_8002f86c_z2o"]
    fill(state, counter - 8, 16, rng)
    state.w32(counter, 0xFFFFFFFF if rng.random() < 0.05 else rng.getrandbits(32))
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the step: `addiu v0,v0,1` becomes `addiu v0,v0,2`."""
    found = [i for i, w in enumerate(words) if w >> 26 == 9 and w & 0xFFFF == 1]
    if len(found) != 1:
        raise ValueError(f"expected one addiu with 1, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 2, "the counter is stepped by 2 instead of 1"


CONTRACT = Contract(setup, control)
