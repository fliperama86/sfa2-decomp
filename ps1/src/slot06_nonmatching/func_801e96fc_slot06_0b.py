"""Contract of func_801e96fc_slot06_0b: an object that selects a layer, and
a grid of cells to draw (the stage 00 contract with another record table,
the parallax arm and tile pages by cell value).

Reads and writes are listed in the header comment of the .c file. Choices:
  - game_state.field_64 is 0 in nine cases of ten and random otherwise;
  - the layer blocks hold random field_0a, field_12 and field_16, the object
    random field_26 (all random bytes, so negative values occur);
  - the grid has 1 to 8 columns and 1 to 6 rows in four cases of five;
    otherwise columns or rows may be 0, so that the loop entries are tried;
  - about a tenth of the cells are 0 (skipped), two fifths are one of the
    values that select a tile page (with the top two bits random), the others
    random;
  - field_0e is 8, 4, 0xc or random, field_0f is 0 or random, the object's
    field_0d and the header's texture page offset are random bytes;
  - the counter is chosen so that counter plus written records is at most
    128, the selector is 0 or 1;
  - the record table is filled with random bytes;
  - the list array has 16 words of random content, field_09 indexes it.
No callee is called.
"""

from contracts import Contract, Setup


# The cell values that the function gives another texture page.
PAGE_CELLS = (2, 3, 5, 6, 0x11, 0x12, 0x13, 0x14, 0x15, 0x2B, 0x2C, 0x2D, 0x2E,
              0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x47, 0x48, 0x49)


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym):
    game_state = sym["game_state"]
    state.w8(game_state + 0x64, 0 if rng.random() < 0.9 else rng.randrange(1, 256))
    state.w16(game_state + 0x92, rng.getrandbits(16))
    for layer in ("data_801aa5d4", "data_801aa544", "cam_obj"):
        state.w16(sym[layer] + 0x0A, rng.getrandbits(16))
        state.w16(sym[layer] + 0x12, rng.getrandbits(16))
        state.w16(sym[layer] + 0x16, rng.getrandbits(16))

    if rng.random() < 0.8:
        columns, rows = rng.randrange(1, 9), rng.randrange(1, 7)
    else:
        columns, rows = rng.randrange(0, 9), rng.randrange(0, 7)
    header = state.alloc(8 + 2 * rows * columns + 2)
    fill(state, header, 8 + 2 * rows * columns, rng)
    state.w8(header + 0, columns)
    state.w8(header + 2, rows)
    written = 0
    for index in range(rows * columns):
        pick = rng.random()
        if pick < 0.1:
            cell = 0
        elif pick < 0.5:
            cell = rng.choice(PAGE_CELLS) | rng.getrandbits(2) << 14
        else:
            cell = rng.getrandbits(16)
        state.w16(header + 8 + 2 * index, cell)
        if cell & 0x3FFF:
            written += 1

    step = state.alloc(12)
    fill(state, step, 12, rng)
    state.w32(step + 4, header)

    lists = state.alloc(4 * 16)
    fill(state, lists, 4 * 16, rng)
    state.w32(sym["data_801987c8"], lists)

    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x09, rng.randrange(16))
    state.w8(obj + 0x0E, rng.choice((8, 4, 0xC, rng.randrange(256))))
    state.w8(obj + 0x0F, rng.choice((0, rng.randrange(256))))
    state.w32(obj + 0x18, step)

    state.w16(sym["data_801adfe4"], rng.randrange(0, 128 - written + 1))
    state.w8(sym["data_801a27d0"], rng.randrange(2))
    fill(state, sym["data_801f5290_slot06_0b"], 2 * 128 * 28, rng)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of the record counter: its offset moves by two bytes.

    The store is `sh reg, -0x201c(at)`, the one halfword store to the
    counter's address that every call with game_state.field_64 equal to 0
    makes; the counter then keeps its old value.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0xDFE4]
    if len(found) != 1:
        raise ValueError(f"expected one store of the record counter, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0xDFE6, "record counter store moved by two bytes"


CONTRACT = Contract(setup, control)
