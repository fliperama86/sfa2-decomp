"""Contract of func_80010e28_logo: no arguments, no result; ends the movie (the volume down, the disc commands, the picture cleared twice).

Choices of the setup (reads and writes are listed in func_80010e28_logo.c):
  - the index word at 0x8002eb50 is 0 in half the cases, else a random
    non-zero word; the recorder of the flip stores a second word of the same
    kind in it at its first call (the flip changes the index), so that the
    second turn of the last loop reads another value than the first;
  - the word at 0x8002eb08 (the pointer handed to CdGetSector) is a random
    word, and so are the bytes around it, around the index word and around
    the mode byte at 0x8002eafc (the three bytes after the mode byte are
    inside the watched word), so that a neighbouring read or write, a store
    of the wrong width, or one that is missing would show;
  - CdControlB is one recorder for both loops, its results scripted in turn:
    0 up to three times then a value that is not 0 (1 or a random non-zero
    word), then the same for the second loop;
  - the callees are recorders: func_80011060_logo (0 arguments), StUnSetRing
    (0), CdReady (2), CdGetSector (2), CdControlB (3), DecDCToutCallback (1),
    ClearImage (4; the pointer is the address of a local, masked to 0, with
    the two words behind it logged: they are described against zeros),
    DrawSync (1), VSync (1), func_80010a38_logo (0);
  - the mode byte's word and the index word are watched, so that the log
    shows what the function has stored by each call.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    mode = sym["data_8002eafc_logo"]
    buffer = sym["data_8002eb08_logo"]
    index = sym["data_8002eb50_logo"]
    fill(state, mode - 8, 28, rng)
    fill(state, index - 8, 20, rng)
    state.w32(buffer, rng.getrandbits(32))
    state.w32(index, 0 if rng.random() < 0.5 else rng.randrange(1, 1 << 32))
    flipped = 0 if rng.random() < 0.5 else rng.randrange(1, 1 << 32)

    def good():
        return 1 if rng.random() < 0.5 else rng.randrange(1, 1 << 32)

    control = (0,) * rng.randrange(4) + (good(),) + (0,) * rng.randrange(4) + (good(),)
    log = CallLog(state, words=256, watch=((mode, 1), (index, 1)))
    log.replace(sym["func_80011060_logo"], 0)
    log.replace(sym["StUnSetRing"], 0)
    log.replace(sym["CdReady"], 2)
    log.replace(sym["CdGetSector"], 2)
    log.replace(sym["CdControlB"], 3, results=control)
    log.replace(sym["DecDCToutCallback"], 1)
    log.replace(sym["ClearImage"], 4, masks={0: 0}, pointees={0: 2})
    log.replace(sym["DrawSync"], 1)
    log.replace(sym["VSync"], 1)
    log.replace(sym["func_80010a38_logo"], 0, stores=((1, index, flipped),))
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the count of the last loop: the `slti rt,rs,2` becomes `slti rt,rs,3`."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0A and w & 0xFFFF == 2]
    if len(found) != 1:
        raise ValueError(f"expected one slti with 2, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 3, "the last loop runs 3 times instead of 2"


CONTRACT = Contract(setup, control)
