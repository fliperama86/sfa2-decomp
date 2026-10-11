"""Contract of func_80011018_z2o: a0 = index, a1 to a3 and six stack words are passed on to the open of a movie; returns its result.

Choices of the setup (reads and writes are listed in func_80011018_z2o.c):
  - the index and the three register arguments are random words; the six
    stack words (the fifth to the tenth arguments, at sp + 0x10 to
    sp + 0x24 of the call) are random words, and so are the words around
    them, so that a stack word read from the neighbouring slot, passed in
    the wrong place or not at all would show in the log of the open;
  - func_800119e8_z2o is a recorder (2 arguments) that returns a random word
    (the first word of the entry); its second argument is the address of a
    local, masked to 0, and the function does not read what the callee would
    write there;
  - func_80010c38_z2o is a recorder (10 arguments) that returns a random word
    (a model: the open returns whatever the contract chooses, so that the
    function's passing of the result on shows);
  - the function reads no memory of the program, so the setup makes none of
    it.
"""
from contracts import CallLog, Contract, Setup, fill

STACK_TOP = 0x801FF000  # sp at entry (the tool's constant); the fifth to the tenth arguments sit at sp + 0x10 to sp + 0x24


def setup(state, rng, sym):
    fill(state, STACK_TOP + 0x10, 24, rng)
    fill(state, STACK_TOP + 0x28, 8, rng)
    log = CallLog(state, words=128)
    log.replace(sym["func_800119e8_z2o"], 2, result=rng.getrandbits(32), masks={1: 0})
    log.replace(sym["func_80010c38_z2o"], 10, result=rng.getrandbits(32))
    return Setup(args=tuple(rng.getrandbits(32) for _ in range(4)), returns_value=True)


def control(words):
    """Alter the load of the seventh argument: `lw s2,0x70(sp)` becomes the load of the sixth, at 0x6c."""
    found = [i for i, w in enumerate(words) if w >> 16 == 0x8FB2 and w & 0xFFFF == 0x70]
    if len(found) != 1:
        raise ValueError(f"expected one lw s2 from 0x70(sp), found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x6C, "the seventh argument is passed as a copy of the sixth"


CONTRACT = Contract(setup, control)
