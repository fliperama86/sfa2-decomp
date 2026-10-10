"""Contract of func_801e1d6c_slot0b: draw a grid of tile cells as records.

Choices of the setup (reads and writes are in the header of the .c file):
  - the grid has 1 to 4 rows and 1 to 6 columns in four cases of five;
    otherwise rows or columns may be 0 (the loop entries are tried), up to
    the same limits; about a fifth of the cells are 0, the others random
    halfwords (the top two bits are masked by the function);
  - the header's origin, texture page offset and the object's field_0d,
    pos_x, pos_y, and the globals box_margin and data_801aa5ea are random;
  - data_801a27d0 is 0 in half of the cases, otherwise a random non-zero
    byte (the second record buffer);
  - the list array has 16 words of random content, field_09 indexes it;
  - both record buffers hold random bytes, data_801e7668_slot0b a random
    word, so that the words the function rewrites and the bytes it leaves
    alone are tested;
  - the recorder of func_80136d1c copies the 7 words of the record, the
    first 24 records of the buffer in use, the list array and
    data_801e7668_slot0b at every call.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    if rng.random() < 0.8:
        cols, rows = rng.randrange(1, 7), rng.randrange(1, 5)
    else:
        cols, rows = rng.randrange(0, 7), rng.randrange(0, 5)
    header = state.alloc(8 + 2 * rows * cols + 2)
    fill(state, header, 8 + 2 * rows * cols, rng)
    state.w8(header + 0, cols)
    state.w8(header + 2, rows)
    for index in range(rows * cols):
        state.w16(header + 8 + 2 * index, 0 if rng.random() < 0.2 else rng.getrandbits(16))
    step = state.alloc(12)
    fill(state, step, 12, rng)
    state.w32(step + 4, header)

    lists = state.alloc(4 * 16)
    fill(state, lists, 4 * 16, rng)
    state.w32(sym["data_801987c8"], lists)

    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x09, rng.randrange(16))
    state.w32(obj + 0x18, step)

    state.w16(sym["box_margin"], rng.getrandbits(16))
    state.w16(sym["data_801aa5ea"], rng.getrandbits(16))
    second = rng.random() < 0.5
    state.w32(sym["data_801a27d0"], rng.randrange(1, 256) if second else 0)
    records = sym["data_8019d7cc"]
    fill(state, records, 2 * 0x24C0, rng)
    state.w32(sym["data_801e7668_slot0b"], rng.getrandbits(32))

    start = records + (0x24C0 if second else 0)
    watch = ((start, 24 * 7), (lists, 16), (sym["data_801e7668_slot0b"], 1))
    log = CallLog(state, 24 * (2 + 7 + 24 * 7 + 16 + 1) + 16, watch=watch)
    log.replace(sym["func_80136d1c"], 1, pointees={0: 7})
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of the command word's constant: 0xe1000006 becomes 0xe1000007.

    The constant is made by `ori v0, v0, 6` after `lui v0, 0xe100` in the
    original; in the build the same `ori` with 6 is the one that follows a
    `lui` of 0xe100. Every drawn cell reaches it.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0xD and w & 0xFFFF == 6]
    if len(found) != 1:
        raise ValueError(f"expected one ori with 6, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 7, "command word constant changed"


CONTRACT = Contract(setup, control)
