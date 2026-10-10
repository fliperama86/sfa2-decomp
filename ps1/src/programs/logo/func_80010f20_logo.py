"""Contract of func_80010f20_logo: no arguments, no result, waits while a flag word is not 0.

Choices of the setup (reads and writes are listed in func_80010f20_logo.c):
  - the flag word (data_8002eb3c_logo) is written by the setup and is 0 in
    every case: with another value the function does not end without an
    interrupt, which the test tool does not model (a limit, said in the .c);
  - the memory around the flag is random, so that a read of a neighbouring
    word or a write would show.
"""
from contracts import Contract, Setup, fill


def setup(state, rng, sym):
    flag = sym["data_8002eb3c_logo"]
    fill(state, flag - 16, 32, rng)
    state.w32(flag, 0)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the exit test: the loop leaves on a flag of 1 instead of 0 (`bnez` into `beqz`)."""
    found = [i for i, w in enumerate(words) if w >> 26 == 5 and (w >> 21) & 31 == 2 and (w >> 16) & 31 == 0]
    if len(found) != 1:
        raise ValueError(f"expected one bnez on v0, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~(0x3F << 26)) | (4 << 26), "the wait leaves on a flag that is not 0 (bnez into beqz)"


CONTRACT = Contract(setup, control)
