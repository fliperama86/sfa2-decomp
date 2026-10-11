"""Contract of func_80010ae8_z2o: no arguments, no result; sets up the graphics and the two display environments.

Choices of the setup (reads and writes are listed in func_80010ae8_z2o.c):
  - the index word at 0x80031e58 holds a random word before the call, so that
    its store of 0 shows;
  - both environments (40 bytes from 0x80049eac) start as random bytes, and
    so do the bytes around them and around the index word, so that a
    neighbouring read or write, a store of the wrong width, or a store that
    is missing would show;
  - the callees are recorders: ResetGraph (1 argument), SetDispMask (1),
    CdReadCallback (1), SetDefDispEnv (5 arguments), ClearImage (4; the
    pointer is the address of a local, masked to 0, with the two words behind
    it logged), DrawSync (1), VSync (1), func_80158470 (1, the five words
    behind it logged);
  - the environments (ten words) and the index word are watched, so that the
    log shows what the function has stored by each call.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    index = sym["data_80031e58_z2o"]
    env = sym["data_80049eac_z2o"]
    fill(state, index - 8, 24, rng)
    fill(state, env - 8, 56, rng)
    state.w32(index, rng.getrandbits(32))
    log = CallLog(state, words=512, watch=((env, 10), (index, 1)))
    log.replace(sym["ResetGraph"], 1)
    log.replace(sym["SetDispMask"], 1)
    log.replace(sym["CdReadCallback"], 1)
    log.replace(sym["SetDefDispEnv"], 5)
    log.replace(sym["ClearImage"], 4, masks={0: 0}, pointees={0: 2})
    log.replace(sym["DrawSync"], 1)
    log.replace(sym["VSync"], 1)
    log.replace(sym["func_80158470"], 1, pointees={0: 5})
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the second display environment's y: the 0xf0 loaded into a2 (`ori a2,zero,0xf0`) becomes 0xf1."""
    found = [i for i, w in enumerate(words) if w >> 26 in (9, 13) and (w >> 21) & 31 == 0 and (w >> 16) & 31 == 6 and w & 0xFFFF == 0xF0]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0xf0 into a2, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0xF1, "the second environment is set up at y 0xf1 instead of 0xf0"


CONTRACT = Contract(setup, control)
