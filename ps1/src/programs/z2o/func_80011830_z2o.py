"""Contract of func_80011830_z2o: no arguments, no result; brings the disc's volume up in steps.

Choices of the setup (reads and writes are listed in func_80011830_z2o.c):
  - the stereo flag word at 0x8001b350 is 0 in half the cases, else a random
    non-zero word (the function tests it against 0 only);
  - the volume word at 0x8001b34c is from 0 to 40 in most cases and from 0 to 300 in others (the loop runs about half that many times; a larger
    value would run for billions of steps, so the setup bounds it);
  - the flag word, the volume word and the four volume bytes start as random
    bytes, and so do the bytes around them, so that a neighbouring read or
    write, a store of the wrong width, or one that is missing would show;
  - CdMix is a recorder of one argument whose pointee is one word (the four
    volume bytes); it returns 0, which the function does not read.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    volume = sym["data_8001b34c_z2o"]
    flag = sym["data_8001b350_z2o"]
    bytes4 = sym["data_8002f8a4_z2o"]
    fill(state, volume - 8, 24, rng)
    fill(state, bytes4 - 8, 20, rng)
    state.w32(flag, 0 if rng.random() < 0.5 else rng.randrange(1, 1 << 32))
    r = rng.random()
    value = rng.randrange(0, 41) if r < 0.7 else rng.randrange(0, 301)
    state.w32(volume, value)
    log = CallLog(state, words=2048)
    log.replace(sym["CdMix"], 1, pointees={0: 1})
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the step of the first loop: the first `addiu rt,rt,2` (not on sp) becomes 3."""
    found = [i for i, w in enumerate(words) if w >> 26 == 9 and w & 0xFFFF == 2 and (w >> 21) & 31 == (w >> 16) & 31 and (w >> 21) & 31 != 29]
    if not found:
        raise ValueError("expected an add of 2 to a register")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 3, "a step is 3 instead of 2 in one loop"


CONTRACT = Contract(setup, control)
