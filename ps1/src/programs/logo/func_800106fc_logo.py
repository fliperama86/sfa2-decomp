"""Contract of func_800106fc_logo: no arguments, no result; the program's main (set up, a loop of frames and pad reads, the end).

Choices of the setup (reads and writes are listed in func_800106fc_logo.c):
  - the three pad words at 0x8002eb80 to 0x8002eb88 start as random words,
    and so do the bytes around them;
  - the loop runs 1 to 5 times; the results are scripted in the order the
    calls come in, and the setup follows the function's flow while it builds
    them (it computes the pressed buttons the way the function does, and
    draws pad words until the turn has the outcome it wants): in every turn
    but the last, the frame returns 0 and the buttons newly pressed have no
    bit of 0x820 or of 0x08200000, so the loop goes on; in the last turn the
    frame returns 1, or the buttons newly pressed have a bit of 0x820 or of
    0x08200000 (or of both) with the frame returning 0. The drawn pad words
    are random words, some with fewer bits set and some that keep the bits
    of the old word in the two masks (held buttons are not new ones);
  - the results of the frame and of the pad end with one more turn than the
    function makes (the frame returns 1): the function never asks for it, but a
    variant that missed a press would, and its log would then differ instead of
    the run going on without end;
  - func_80011758_logo returns two random words (the stream's sector, then
    the stills' file's sector); its second argument is the address of a local
    (masked to 0);
  - the callees are recorders of the arguments they take: func_80011838_logo
    (0), ResetCallback (0), func_80011794_logo (0), PadInit (1),
    func_80011758_logo (2), func_800107e8_logo (5), func_80010c20_logo (0),
    PadRead (1), func_80010e28_logo (0), PadStop (0), StopCallback (0);
  - the three pad words are watched, so that the log shows what the function
    has stored by each call.
"""
from contracts import CallLog, Contract, Setup, fill

PRESS_A = 0x820
PRESS_B = 0x08200000


def setup(state, rng, sym):
    words = sym["data_8002eb80_logo"]
    fill(state, words - 4, 20, rng)
    previous = state.read(words, 4)
    previous = int.from_bytes(previous, "little")
    turns = 1 + rng.randrange(5)
    frames, pads = [], []

    def outcome(frame, pad, old):
        pressed = (pad ^ old) & pad
        total = frame + (1 if pressed & PRESS_A else 0) + (1 if pressed & PRESS_B else 0)
        return total

    for turn in range(turns):
        last = turn == turns - 1
        for _ in range(64):
            pad = rng.getrandbits(32)
            if rng.random() < 0.3:
                pad &= rng.getrandbits(32) | previous  # fewer set bits
            if rng.random() < 0.3:
                pad |= previous & (PRESS_A | PRESS_B)  # held buttons
            frame = 1 if last and rng.random() < 0.3 else 0
            total = outcome(frame, pad, previous)
            if (total != 0) == last:
                break
        else:
            pad = previous & ~(PRESS_A | PRESS_B) if not last else previous | PRESS_A
            frame = 0 if not last else 1
        frames.append(frame)
        pads.append(pad)
        previous = pad

    frames.append(1)  # one more turn than the function makes: a variant that ignored a press would take it and end there, and the log would differ
    pads.append(rng.getrandbits(32))
    log = CallLog(state, words=256, watch=((words, 3),))
    log.replace(sym["func_80011838_logo"], 0)
    log.replace(sym["ResetCallback"], 0)
    log.replace(sym["func_80011794_logo"], 0)
    log.replace(sym["PadInit"], 1)
    log.replace(sym["func_80011758_logo"], 2, results=(rng.getrandbits(32), rng.getrandbits(32)), masks={1: 0})
    log.replace(sym["func_800107e8_logo"], 5)
    log.replace(sym["func_80010c20_logo"], 0, results=tuple(frames))
    log.replace(sym["PadRead"], 1, results=tuple(pads))
    log.replace(sym["func_80010e28_logo"], 0)
    log.replace(sym["PadStop"], 0)
    log.replace(sym["StopCallback"], 0)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the first mask: the `andi rt,rs,0x820` becomes `andi rt,rs,0x800`."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0C and w & 0xFFFF == 0x820]
    if len(found) != 1:
        raise ValueError(f"expected one andi with 0x820, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x800, "the mask of the first pair of buttons is 0x800 instead of 0x820"


CONTRACT = Contract(setup, control)
