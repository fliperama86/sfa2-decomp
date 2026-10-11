"""Contract of func_800108e8_z2o: no arguments, no result; the program's main (set up, a loop of frames and pad reads, the end).

Choices of the setup (reads and writes are listed in func_800108e8_z2o.c):
  - the index word at 0x80031e58 starts as 0 or 1 in four cases of five, else
    a random word; the pad words from 0x80031e6c to 0x80031e74, the words at
    0x8003de88 and 0x80049e8c, the rectangle at 0x80049ea4 and the scratchpad
    word at 0x1f80000c start as random words, and so do the bytes around
    them, so that a store that is missing, a neighbouring write or a store of
    the wrong width would show;
  - the loop runs 1 to 5 times; the results are scripted in the order the
    calls come in, and the setup follows the function's flow while it builds
    them: the function stores 0 in the word at 0x80031e6c at the start and
    nothing changes it, so the buttons newly pressed are the pad word read.
    In every turn but the last, the frame returns 0 and the pad word has no
    bit of 0x820 or of 0x08200000 (the high half's mask 0x820 is the bit
    pattern 0x08200000 of the whole word), so the loop goes on; in the last
    turn the pad word has a bit of one of those masks (or both), with the
    frame returning 0 or not 0, or has none and the frame returns not 0 (the
    exit without the scratchpad store);
  - the results of the frame and of the pad end with one more turn than the
    function makes (the frame returns 1 and the pad word is random): the
    function never asks for it, but a variant that missed a press or a result
    would, and its log would then differ instead of the run going on without
    end;
  - the flip (func_80010be4_z2o) is a recorder with a model: at each call it
    stores the opposite of the index word's truth (0 if the word is not 0,
    else 1); this equals the real flip (an exclusive-or with 1) for the
    indices 0 and 1 only (index 2 becomes 3 in the real flip, 0 in the model),
    so that the frame's second
    argument and the rectangle's y change between turns; the stores are given
    for calls 1 to 9 (at most 5 in the loop and 2 at the end);
  - the other callees are recorders of the arguments they take:
    func_80011afc_z2o (0), ResetCallback (0), ResetGraph (1),
    func_80011a24_z2o (0), PadInit (1), InitGeom (0), func_80010ae8_z2o (0),
    func_800119b8_z2o (1), func_800115a8_z2o (1), func_80011630_z2o (1),
    func_800116c8_z2o (1), func_80011018_z2o (10; the six stack arguments are
    read from the function's frame), func_800110d0_z2o (2),
    func_80011428_z2o (0), VSync (1), PadRead (1), func_8001139c_z2o (0),
    ClearImage (4, the two words behind the rectangle pointer logged),
    DrawSync (1), PadStop (0), StopCallback (0);
  - the index word, the three words from 0x80031e6c, the words at 0x8003de88
    and 0x80049e8c, the rectangle and the scratchpad word are watched, so that
    the log shows what the function has stored by each call.
"""
from contracts import CallLog, Contract, Setup, fill

PRESS = 0x820 | (0x820 << 16)  # a bit of 0x820 in either half of the pad word
SCRATCH = 0x1F80000C


def setup(state, rng, sym):
    index = sym["data_80031e58_z2o"]
    pads = sym["data_80031e6c_z2o"]
    low = sym["data_8003de88_z2o"]
    high = sym["data_80049e8c_z2o"]
    rect = sym["data_80049ea4_z2o"]
    fill(state, index - 8, 24, rng)
    fill(state, pads - 8, 28, rng)
    fill(state, low - 8, 24, rng)
    fill(state, high - 8, 24, rng)
    fill(state, rect - 8, 24, rng)
    fill(state, SCRATCH - 8, 24, rng)
    cur = rng.randrange(2) if rng.random() < 0.8 else rng.getrandbits(32)
    state.w32(index, cur)

    turns = 1 + rng.randrange(5)
    frames, pad_words = [], []
    for turn in range(turns):
        last = turn == turns - 1
        pad = rng.getrandbits(32)
        if not last or rng.random() < 0.3:
            pad &= ~PRESS & 0xFFFFFFFF  # no press: the loop goes on, or ends through the frame's result
            if rng.random() < 0.3:
                pad &= rng.getrandbits(32)
        else:
            press = rng.choice((0x820, 0x800, 0x20, 0x20, 0x08200000, 0x08000000, 0x00200000, 0x00200000, 0x08200820))
            pad |= press
            if rng.random() < 0.5:
                pad &= ~PRESS | press & 0xFFFFFFFF  # only the chosen bits of the two masks
        if pad & PRESS == 0 and not last:
            frame = 0
        elif last and pad & PRESS == 0:
            frame = 1
        else:
            frame = 0 if (not last or rng.random() < 0.5) else 1
        frames.append(frame)
        pad_words.append(pad)
    # one more turn than the function makes
    frames.append(1)
    pad_words.append(rng.getrandbits(32))

    # the flip's model: the index word's truth is turned over at each call
    stores = []
    for call in range(1, 10):
        cur = 0 if cur else 1
        stores.append((call, index, cur))

    log = CallLog(state, words=1536, watch=((index, 1), (pads, 3), (low, 1), (high, 1), (rect, 2), (SCRATCH, 1)))
    log.replace(sym["func_80011afc_z2o"], 0)
    log.replace(sym["ResetCallback"], 0)
    log.replace(sym["ResetGraph"], 1)
    log.replace(sym["func_80011a24_z2o"], 0)
    log.replace(sym["PadInit"], 1)
    log.replace(sym["InitGeom"], 0)
    log.replace(sym["func_80010ae8_z2o"], 0)
    log.replace(sym["func_800119b8_z2o"], 1)
    log.replace(sym["func_800115a8_z2o"], 1)
    log.replace(sym["func_80011630_z2o"], 1)
    log.replace(sym["func_800116c8_z2o"], 1)
    log.replace(sym["func_80011018_z2o"], 10)
    log.replace(sym["func_800110d0_z2o"], 2, results=tuple(frames))
    log.replace(sym["func_80011428_z2o"], 0)
    log.replace(sym["VSync"], 1)
    log.replace(sym["func_80010be4_z2o"], 0, stores=tuple(stores))
    log.replace(sym["PadRead"], 1, results=tuple(pad_words))
    log.replace(sym["func_8001139c_z2o"], 0)
    log.replace(sym["ClearImage"], 4, pointees={0: 2})
    log.replace(sym["DrawSync"], 1)
    log.replace(sym["PadStop"], 0)
    log.replace(sym["StopCallback"], 0)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the mask of the low half: the `andi rt,rs,0x820` becomes `andi rt,rs,0x800`."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0C and w & 0xFFFF == 0x820]
    if len(found) != 2:
        raise ValueError(f"expected two andi with 0x820, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x800, "the mask of the first pair of buttons is 0x800 instead of 0x820"


CONTRACT = Contract(setup, control)
