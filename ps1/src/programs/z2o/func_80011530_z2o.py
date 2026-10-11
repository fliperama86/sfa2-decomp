"""Contract of func_80011530_z2o: a0 = a choice, the result in v0 is 0; with 1 the disc is paused, with any other value the stream is read on.

Choices of the setup (reads and writes are listed in func_80011530_z2o.c):
  - the argument is 1 in half of the cases (the pause), else a random 32 bit word (not 1), or 0, 2 or 0xffffffff
    now and then, so that only the value 1 is told from the rest;
  - the flag word at 0x8001b348 is 0 in a third of the cases, else a random non-zero word; the bytes around it
    are random, so that a neighbouring read shows;
  - CdControlB and CdRead2 are recorders whose results are scripted: 0 up to three times, then a value that is
    not 0 (1 or a random non-zero word), so that each loop is entered, run more than once and left;
  - func_800118f0_z2o is a recorder (0 arguments);
  - the arguments of CdControlB are literals, so the log holds them as they are (nothing is masked).
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    flag = sym["data_8001b348_z2o"]
    fill(state, flag - 8, 20, rng)
    if rng.random() < 1 / 3:
        state.w32(flag, 0)
    else:
        state.w32(flag, rng.randrange(1, 1 << 32))

    def good():
        return 1 if rng.random() < 0.5 else rng.randrange(1, 1 << 32)

    def script():
        return (0,) * rng.randrange(4) + (good(),)

    log = CallLog(state, words=128)
    log.replace(sym["func_800118f0_z2o"], 0)
    log.replace(sym["CdControlB"], 3, results=script())
    log.replace(sym["CdRead2"], 1, results=script())
    if rng.random() < 0.5:
        a0 = 1
    elif rng.random() < 0.2:
        a0 = rng.choice((0, 2, 0xFFFFFFFF))
    else:
        a0 = rng.getrandbits(32)
        if a0 == 1:
            a0 = 3
    return Setup(args=(a0,), returns_value=True)


def control(words):
    """Alter the mode word: the 0x1e8 (`ori rt,zero,0x1e8`) becomes 0x1e9."""
    found = [i for i, w in enumerate(words) if w >> 26 == 13 and (w >> 21) & 31 == 0 and w & 0xFFFF == 0x1E8]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x1e8, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x1E9, "the wide mode word is 0x1e9 instead of 0x1e8"


CONTRACT = Contract(setup, control)
