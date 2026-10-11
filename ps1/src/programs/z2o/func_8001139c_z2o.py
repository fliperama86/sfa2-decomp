"""Contract of func_8001139c_z2o: no arguments, no result; ends the movie (the volume down, the disc commands, the decoder's callback cleared).

Choices of the setup (reads and writes are listed in func_8001139c_z2o.c):
  - the pointer word at 0x8002f804 is a random word, and so are the bytes
    around it and around the mode byte at 0x8002f800 (the three bytes after
    the mode byte are inside the watched word, and so is the pointer word),
    so that a neighbouring read or write, a store of the wrong width, or one
    that is missing would show;
  - CdControlB is one recorder for both loops, its results scripted in turn:
    0 up to three times then a value that is not 0 (1 or a random non-zero
    word), then the same for the second loop;
  - the callees are recorders: func_800118f0_z2o (0 arguments), StUnSetRing
    (0), CdReady (2), CdGetSector (2), CdControlB (3), DecDCToutCallback (1);
  - the mode byte's word is watched, so that the log shows what the function
    has stored by each call.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    mode = sym["data_8002f800_z2o"]
    pointer = sym["data_8002f804_z2o"]
    fill(state, mode - 8, 28, rng)
    state.w32(pointer, rng.getrandbits(32))

    def good():
        return 1 if rng.random() < 0.5 else rng.randrange(1, 1 << 32)

    control = (0,) * rng.randrange(4) + (good(),) + (0,) * rng.randrange(4) + (good(),)
    log = CallLog(state, words=256, watch=((mode, 1),))
    log.replace(sym["func_800118f0_z2o"], 0)
    log.replace(sym["StUnSetRing"], 0)
    log.replace(sym["CdReady"], 2)
    log.replace(sym["CdGetSector"], 2)
    log.replace(sym["CdControlB"], 3, results=control)
    log.replace(sym["DecDCToutCallback"], 1)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the mode: the 0x80 stored in the mode byte (`ori rt,zero,0x80`) becomes 0x81."""
    found = [i for i, w in enumerate(words) if w >> 26 == 13 and (w >> 21) & 31 == 0 and w & 0xFFFF == 0x80]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x80, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x81, "the mode byte is 0x81 instead of 0x80"


CONTRACT = Contract(setup, control)
