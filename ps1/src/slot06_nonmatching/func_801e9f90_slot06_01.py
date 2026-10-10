"""Contract of func_801e9f90_slot06_01: an object of kind 8 and a grid of cells to draw.

Reads and writes are listed in the header comment of
func_801e9f90_slot06_01.c. Choices made here:
  - game_state.field_64 is 0 in nine cases of ten and random otherwise
    (the function returns at once when it is not 0);
  - the object's field_0e is always 8 (any other value is excluded, see the
    header comment);
  - the layer block holds random field_0a, field_12 and field_16, the object
    a random field_26 (so the parallax term takes both signs);
  - the grid has 1 to 8 columns and 1 to 6 rows (the loops are bounded by 48
    cells) in four cases of five; otherwise columns or rows may be 0, so that
    the loop entries are tried;
  - about a tenth of the cells are 0 (skipped), the others random;
  - the counter is chosen so that counter plus written records is at most
    108, the selector is 0 or 1;
  - the record table is filled with random bytes, so that the words the
    function reads and rewrites and the bytes it leaves alone are tested;
  - the list array has 16 words of random content, field_09 indexes it.
"""

from contracts import CallLog, Contract, Setup, fill, halfword  # noqa: F401


def setup(state, rng, sym) -> Setup:
    game_state = sym["game_state"]
    state.w8(game_state + 0x64, 0 if rng.random() < 0.9 else rng.randrange(1, 256))
    state.w16(game_state + 0x92, halfword(rng))
    layer = sym["data_801aa5d4"]
    for offset in (0x0A, 0x12, 0x16):
        state.w16(layer + offset, halfword(rng))

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
        cell = 0 if rng.random() < 0.1 else halfword(rng)
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
    state.w8(obj + 0x0E, 8)
    state.w8(obj + 0x0F, rng.choice((0, rng.randrange(256))))
    state.w32(obj + 0x18, step)

    state.w16(sym["data_801adfe4"], rng.randrange(0, 108 - written + 1))
    state.w8(sym["data_801a27d0"], rng.randrange(2))
    fill(state, sym["data_801fa57c_slot06_01"], 2 * 108 * 28, rng)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of the record counter: its offset moves by two bytes.

    The store is `sh reg, -0x201c(at)`, the one halfword store to the
    counter's address that every call with game_state.field_64 equal to 0
    and at least one drawn cell makes; the counter then keeps its old value.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0xDFE4]
    if len(found) != 1:
        raise ValueError(f"expected one store of the record counter, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0xDFE6, "record counter store moved by two bytes"


CONTRACT = Contract(setup, control)
