"""Contract of func_801e0ea8_slot0b: rectangles of a shape into records.

Choices of the setup (reads and writes are in the header of the .c file):
  - the shape has a count of 1 to 6 (0 or less is excluded: the original
    would loop 2^32 times), 4 descriptor bytes and 2 coordinate halfwords
    per entry, all random;
  - the object's pos_x and pos_y are random halfwords, field_09 is 0 to 15;
  - the 16 words of table_801e47a8_slot0b are 0 to 15 (an index into the
    list array), the list array has 16 words of random content;
  - the record array holds random bytes, so that the bytes the function
    does not write are tested.
"""
from contracts import Contract, Setup, fill


def setup(state, rng, sym):
    count = rng.randrange(1, 7)
    desc = state.alloc(4 * count)
    fill(state, desc, 4 * count, rng)
    coords = state.alloc(4 * count)
    fill(state, coords, 4 * count, rng)
    shape = state.alloc(12)
    fill(state, shape, 12, rng)
    state.w16(shape, count)
    state.w32(shape + 4, desc)
    state.w32(shape + 8, coords)
    step = state.alloc(12)
    fill(state, step, 12, rng)
    state.w32(step + 4, shape)

    table = sym["table_801e47a8_slot0b"]
    for i in range(16):
        state.w32(table + 4 * i, rng.randrange(16))
    lists = state.alloc(4 * 16)
    fill(state, lists, 4 * 16, rng)
    state.w32(sym["data_801987c8"], lists)

    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x09, rng.randrange(16))
    state.w32(obj + 0x18, step)

    prims = state.alloc(6 * 0x28)
    fill(state, prims, 6 * 0x28, rng)
    return Setup(args=(obj, prims), returns_value=False)


def control(words):
    """Alter the first halfword store of the build: its offset moves by 0x10.

    Every call stores the corners, so the first `sh` is reached at every
    call; at the other offset it writes a different field and leaves the
    intended one as it was.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29]
    if not found:
        raise ValueError("expected halfword stores")
    index = found[0]
    return index, words[index] + 0x10, "first corner store moved by 0x10 bytes"


CONTRACT = Contract(setup, control)
