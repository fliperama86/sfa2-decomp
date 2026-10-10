"""Contract of func_80012714_slot01 as code.

a0 is an object whose sequence step points at a set of sprite offsets; a1 is
an array of primitives and a3 a flag (a2 is not used).

Reads and writes are listed in the header comment of func_80012714_slot01.c.
Choices made here:
  - the set's count is 1 to 8 (never 0 or less: the original would loop
    about 2^32 times); the offsets are random u16 pairs;
  - the flag is 0 in half the cases and random (non-zero) otherwise; the
    primitive array has room for 2 * count primitives either way, filled
    with random bytes so that the bytes the function leaves alone are tested;
  - field_48 is 0 in half the cases; field_20, field_22 and field_24 are
    each 0, a random halfword, or (field_24 only) a halfword with a zero
    low byte, in about a third of the cases each; the table
    data_800212d8_slot01 is read as the module image holds it;
  - the list array has 8 + 256 words of random content and data_801987c8
    points at it; field_09 is random, so the head word is any of the 256;
  - pos_x and pos_y are random.
"""

from contracts import Contract, Setup, fill


def _field(rng, low_byte_zero=False) -> int:
    pick = rng.randrange(3)
    if pick == 0:
        return 0
    if pick == 1 and low_byte_zero:
        return rng.randrange(1, 256) << 8
    return rng.getrandbits(16)


def setup(state, rng, sym) -> Setup:
    count = rng.randrange(1, 9)
    offsets = state.alloc(4 * count)
    fill(state, offsets, 4 * count, rng)
    sset = state.alloc(12)
    fill(state, sset, 12, rng)
    state.w16(sset, count)
    state.w32(sset + 8, offsets)
    step = state.alloc(12)
    fill(state, step, 12, rng)
    state.w32(step + 4, sset)

    lists = state.alloc(4 * (8 + 256))
    fill(state, lists, 4 * (8 + 256), rng)
    state.w32(sym["data_801987c8"], lists)

    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w32(obj + 0x18, step)
    state.w8(obj + 0x09, rng.randrange(256))
    state.w8(obj + 0x48, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    state.w16(obj + 0x20, _field(rng))
    state.w16(obj + 0x22, _field(rng))
    state.w16(obj + 0x24, _field(rng, low_byte_zero=True))

    prims = state.alloc(0x28 * 2 * count + 4)
    fill(state, prims, 0x28 * 2 * count + 4, rng)
    flag = 0 if rng.random() < 0.5 else rng.randrange(1, 1 << 32)
    return Setup(args=(obj, prims, rng.getrandbits(32), flag), returns_value=False)


def control(words):
    """Alter the first arithmetic right shift by 4 (the stretch of x or y): it becomes a shift by 3."""
    found = [i for i, w in enumerate(words) if w & 0xFFE0003F == 3 and (w >> 6) & 31 == 4]
    if not found:
        raise ValueError("expected an arithmetic shift right by 4, found none")
    index = found[0]
    return index, (words[index] & ~(31 << 6)) | (3 << 6), "a shift right by 4 changed to 3"


CONTRACT = Contract(setup, control)
