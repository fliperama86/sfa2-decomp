"""Contract of func_80011428_z2o: no arguments, no result, waits while a flag word is not 0, then copies one word to another.

Choices of the setup (reads and writes are listed in func_80011428_z2o.c):
  - the flag word (data_8002f890_z2o) is written by the setup and is 0 in
    every case: with another value the function does not end without an
    interrupt, which the test tool does not model (a limit, said in the .c);
  - the source and target words of the copy hold random values, and the
    memory around the flag and around the two words is random, so that a
    read of a neighbouring word or a wider write would show.
"""
from contracts import Contract, Setup, fill


def setup(state, rng, sym):
    flag = sym["data_8002f890_z2o"]
    target = sym["data_8002f818_z2o"]
    fill(state, flag - 16, 32, rng)
    fill(state, target - 8, 24, rng)
    state.w32(flag, 0)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the source of the copy: the load reads the word 4 bytes further."""
    found = [i for i, w in enumerate(words) if w >> 16 == 0x8C42 and w & 0xFFFF == 0xF81C]
    if len(found) != 1:
        raise ValueError(f"expected one lw v0 from 0x8002f81c, found {len(found)}")
    index = found[0]
    return index, words[index] + 4, "the copy takes the word after the source"


CONTRACT = Contract(setup, control)
