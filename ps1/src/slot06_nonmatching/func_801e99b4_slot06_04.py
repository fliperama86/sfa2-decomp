"""Contract of func_801e99b4_slot06_04: an object with a grid of cells to draw.

Reads and writes are listed in the header comment of
func_801e99b4_slot06_04.c. Choices made here:
  - game_state.field_64 is 0 in nine cases of ten and random otherwise;
  - the layer blocks hold random field_0a, field_12 and field_16, and the
    object a random field_26 (the parallax term of layer 8);
  - the object's field_0e is 8, 4 or 0xc (other values are excluded: the
    original then uses registers its caller left), field_0f is 0 or random;
  - the grid has 1 to 8 columns and 1 to 6 rows in four cases of five;
    otherwise columns or rows may be 0, so that the loop entries are tried;
  - the header mode (byte 1) is 5 or 6 in three cases of four, so that both
    texture page switches are met, random otherwise; six rows reach row 2;
  - about a tenth of the cells are 0 (skipped), the others random;
  - the counter is chosen so that counter plus written records is at most
    304, the selector is 0 or 1;
  - the record table is filled with random bytes, so that the words the
    function reads and rewrites and the bytes it leaves alone are tested;
  - the list array has 16 words of random content, field_09 indexes it.
"""

from contracts import Contract, Setup


def halfword(rng) -> int:
    return rng.getrandbits(16)


def fill(state, address: int, size: int, rng) -> None:
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym) -> Setup:
    game_state = sym["game_state"]
    state.w8(game_state + 0x64, 0 if rng.random() < 0.9 else rng.randrange(1, 256))
    state.w16(game_state + 0x92, halfword(rng))
    for layer in ("data_801aa5d4", "data_801aa544", "cam_obj"):
        state.w16(sym[layer] + 0x0A, halfword(rng))
        state.w16(sym[layer] + 0x12, halfword(rng))
        state.w16(sym[layer] + 0x16, halfword(rng))

    if rng.random() < 0.8:
        columns, rows = rng.randrange(1, 9), rng.randrange(1, 7)
    else:
        columns, rows = rng.randrange(0, 9), rng.randrange(0, 7)
    header = state.alloc(8 + 2 * rows * columns + 2)
    fill(state, header, 8 + 2 * rows * columns, rng)
    state.w8(header + 0, columns)
    state.w8(header + 1, rng.choice((5, 6)) if rng.random() < 0.75 else rng.randrange(256))
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
    state.w8(obj + 0x0E, rng.choice((8, 4, 0xC)))
    state.w8(obj + 0x0F, rng.choice((0, rng.randrange(256))))
    state.w32(obj + 0x18, step)

    state.w16(sym["data_801adfe4"], rng.randrange(0, 304 - written + 1))
    state.w8(sym["data_801a27d0"], rng.randrange(2))
    fill(state, sym["data_801f6ce0_slot06_04"], 2 * 304 * 28, rng)
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
