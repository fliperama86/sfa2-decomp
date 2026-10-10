"""Contract of func_80012dec_slot01 as code.

a0 is an object, a1 an array of primitives; the function fills one
primitive per part of the object's description and links each into a list.

Reads and writes are listed in the header comment of func_80012dec_slot01.c.
Choices made here:
  - the description holds 1 to 6 parts (never 0: the original would loop
    2^32 times); texture records (4 bytes each) and position records
    (4 bytes each) are random bytes;
  - the primitive array has as many primitives as parts, filled with random
    bytes so that the bytes the function leaves alone are tested;
  - the object is 0x394 random bytes with a random field_09; the table
    data_80015780_slot01 gets, at index field_09, a random index into the
    list array of 16 words, which is filled with random words;
  - data_801987c8 points at the list array.
"""

from contracts import Contract, Setup, fill


def setup(state, rng, sym) -> Setup:
    n = rng.randrange(1, 7)
    tex = state.alloc(4 * n)
    fill(state, tex, 4 * n, rng)
    pts = state.alloc(4 * n)
    fill(state, pts, 4 * n, rng)
    desc = state.alloc(12)
    fill(state, desc, 12, rng)
    state.w16(desc, n)
    state.w32(desc + 4, tex)
    state.w32(desc + 8, pts)
    step = state.alloc(12)
    fill(state, step, 12, rng)
    state.w32(step + 4, desc)

    lists = state.alloc(4 * 16)
    fill(state, lists, 4 * 16, rng)
    state.w32(sym["data_801987c8"], lists)

    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    index = rng.randrange(256)
    state.w8(obj + 0x09, index)
    state.w32(obj + 0x18, step)
    state.w32(sym["data_80015780_slot01"] + 4 * index, rng.randrange(16))

    prims = state.alloc(0x28 * n + 4)
    fill(state, prims, 0x28 * n + 4, rng)
    return Setup(args=(obj, prims), returns_value=False)


def control(words):
    """Alter the first left shift by 4 (the scale of a texture byte): it becomes a shift by 3."""
    found = [i for i, w in enumerate(words) if w & 0xFFE0003F == 0 and (w >> 6) & 31 == 4 and w != 0]
    if not found:
        raise ValueError("expected a shift left by 4, found none")
    index = found[0]
    return index, (words[index] & ~(31 << 6)) | (3 << 6), "a shift left by 4 changed to 3"


CONTRACT = Contract(setup, control)
