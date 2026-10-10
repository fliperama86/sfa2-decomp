"""Contract of func_80011688_slot12 (see the header of the .c file).

Choices of the setup:
  - the current palette is 16 random halfwords; the target is, per entry,
    equal to it in two cases of five (so that the skip is tried), otherwise
    built channel by channel: each of the three channels is the current one
    plus a delta of -4 to 4 (so that the clamp at the target and the step of
    2 both occur), or random; the blue channel uses 6 bits as the function
    reads it;
  - the pointer word data_80028b40_slot12 points at the current palette;
  - both callees are recorders (2 and 1 arguments). The first copies the 2
    words of the rectangle and the 8 words of the palette into the log.
"""
from contracts import CallLog, Contract, Setup, halfword


def channel(rng, base, bits):
    if rng.random() < 0.7:
        return (base + rng.randrange(-4, 5)) & ((1 << bits) - 1)
    return rng.getrandbits(bits)


def setup(state, rng, sym):
    cur = state.alloc(32)
    current = [halfword(rng) for _ in range(16)]
    for i, value in enumerate(current):
        state.w16(cur + 2 * i, value)
    target = sym["data_80028aa0_slot12"]
    for i, value in enumerate(current):
        if rng.random() < 0.4:
            t = value
        else:
            blue = channel(rng, value >> 10, 6)
            green = channel(rng, (value >> 5) & 0x1F, 5)
            red = channel(rng, value & 0x1F, 5)
            t = (blue << 10) | (green << 5) | red
        state.w16(target + 2 * i, t)
    state.w32(sym["data_80028b40_slot12"], cur)
    log = CallLog(state, 64)
    log.replace(sym["func_80157fc4"], 2, pointees={0: 2, 1: 8})
    log.replace(sym["func_80157d9c"], 1)
    return Setup(args=(), returns_value=True)


def control(words):
    """Alter one channel step: the last `addiu rt,rs,2` of the build becomes `addiu rt,rs,3`.

    The function has three of them (blue, green, red), each executed by every
    entry that differs from its target (the two pointer steps of the loop,
    which also add 2, have the same register on both sides and are skipped).
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 9 and w & 0xFFFF == 2 and (w >> 21) & 31 != (w >> 16) & 31]
    if len(found) != 3:
        raise ValueError(f"expected three channel steps, found {len(found)}")
    index = found[-1]
    return index, words[index] + 1, "last channel step 2 changed to 3"


CONTRACT = Contract(setup, control)
