"""Contract of func_80012990_slot01 as code.

a0 is an object whose sequence step points at a set of sprites; a1 is an
array of primitives, a2 a texture page, a3 a base value, and the flag is
the fifth argument, on the stack.

Reads and writes are listed in the header comment of func_80012990_slot01.c.
Choices made here:
  - the set's count is 1 to 12 in nine cases of ten, otherwise 0 or
    negative (the original runs its loop once then); codes and offsets are
    random bytes, one code and one offset pair for each of max(count, 1)
    sprites;
  - the flag is 0 in half the cases and random (non-zero) otherwise; it is
    written at sp + 0x10, where the fifth argument is read (the tool's
    initial sp is STACK_TOP below, the same for both codes);
  - the texture page and the base are random (the base is 32 bits wide);
  - the primitive array has room for two passes of max(count, 1)
    primitives, filled with random bytes, so that the bytes the function
    leaves alone are tested;
  - func_8015c09c is a recorder (one argument, result 0) that copies the
    whole primitive array into the log at every call.
"""

from contracts import CallLog, Contract, Setup, fill

STACK_TOP = 0x801FF000  # the stack pointer the test starts the call with (STACK_TOP of difftest.py)


def setup(state, rng, sym) -> Setup:
    if rng.random() < 0.9:
        count = rng.randrange(1, 13)
    else:
        count = rng.randrange(-5, 1)
    sprites = max(count, 1)
    codes = state.alloc(sprites + 1)
    fill(state, codes, sprites + 1, rng)
    offsets = state.alloc(4 * sprites)
    fill(state, offsets, 4 * sprites, rng)
    sset = state.alloc(12)
    fill(state, sset, 12, rng)
    state.w16(sset, count & 0xFFFF)
    state.w32(sset + 4, codes)
    state.w32(sset + 8, offsets)
    step = state.alloc(12)
    fill(state, step, 12, rng)
    state.w32(step + 4, sset)
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w32(obj + 0x18, step)

    words = 10 * 2 * sprites + 1
    prims = state.alloc(4 * words)
    fill(state, prims, 4 * words, rng)

    log = CallLog(state, 2 * sprites * (2 + words) + 16, watch=((prims, words),))
    log.replace(sym["func_8015c09c"], 1, 0)

    flag = 0 if rng.random() < 0.5 else rng.randrange(1, 1 << 32)
    state.w32(STACK_TOP + 0x10, flag)
    return Setup(args=(obj, prims, rng.getrandbits(32), rng.getrandbits(32)), returns_value=False)


def control(words):
    """Move the first byte store at one of the primitive's early offsets by one byte."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and (w & 0xFFFF) in (0xFFF6, 0xFFF7, 0xFFF8, 4, 5, 6)]
    if not found:
        raise ValueError("expected a store of a colour byte, found none")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | ((words[index] + 1) & 0xFFFF), "a byte store of the primitive moved by one byte"


CONTRACT = Contract(setup, control)
