"""Contract of func_80011060_logo: no arguments, no result; brings the disc's volume down in 65 steps.

Choices of the setup (reads and writes are listed in func_80011060_logo.c):
  - the flag word at 0x8002eb34 is 0 in half the cases, else a random
    non-zero word (the function tests it against 0 only);
  - the four volume bytes at 0x8002eb40 start as random bytes, and so do the
    bytes around the flag and around the volume bytes (a neighbouring read or
    write would show);
  - CdMix is a recorder of one argument whose pointee is one word (the four
    volume bytes); it returns 0, which the function does not read.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    flag = sym["data_8002eb34_logo"]
    volume = sym["data_8002eb40_logo"]
    fill(state, flag - 8, 20, rng)
    fill(state, volume - 8, 20, rng)
    state.w32(flag, 0 if rng.random() < 0.5 else rng.randrange(1, 1 << 32))
    log = CallLog(state, words=256)
    log.replace(sym["CdMix"], 1, pointees={0: 1})
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the step of the first loop: the first `addiu rt,rt,-2` (not on sp) becomes -3."""
    found = [i for i, w in enumerate(words) if w >> 26 == 9 and w & 0xFFFF == 0xFFFE and (w >> 21) & 31 == (w >> 16) & 31 and (w >> 21) & 31 != 29]
    if not found:
        raise ValueError("expected an add of -2 to a register")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0xFFFD, "a step is -3 instead of -2 in one loop"


CONTRACT = Contract(setup, control)
