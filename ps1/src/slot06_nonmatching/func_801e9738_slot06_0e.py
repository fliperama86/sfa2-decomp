"""Contract of func_801e9738_slot06_0e: no argument, draws layer data_801aa544[2].

Reads and writes are listed in the header comment of
func_801e9738_slot06_0e.c. Choices made here:
  - field_58 is a power of two from 32 to 4096 in three cases of four and
    any value from 32 to 4000 otherwise (smaller values and larger ones
    would make the column index run over 65536 cells or over the map);
  - field_12, field_14 and game_state.field_92 are random 16-bit and 32-bit
    values, so that every sign and the carry of the row are tried;
  - field_16 is below 0x100, from 0x100 to 0x5ff, or 0x600 and above, in
    equal shares, with the borders tried often;
  - field_1e (blank) is random; the map holds mostly blank cells, about
    half blank cells or no blank cells at all, the others random tile words;
  - the list array has 16 words of random content, field_8a indexes it;
  - the record table is filled with random bytes, so that the words the
    function rewrites and the bytes it leaves alone are tested.
"""

from contracts import Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym):
    game_state = sym["game_state"]
    layer = sym["data_801aa544"] + 2 * 0x90
    state.w16(game_state + 0x92, rng.getrandbits(16))

    if rng.random() < 0.75:
        width = 32 << rng.randrange(0, 8)
    else:
        width = rng.randrange(32, 4001)
    columns = (width >> 5) + 2
    blocks = (columns >> 4) + 1
    mapsize = 2 * 0x400 * blocks  # halfwords: a block of 16 columns takes 256 * 8 rows * 2 halves
    cells = state.alloc(mapsize + 8)

    blank = rng.getrandbits(16)
    density = rng.choice((0.05, 0.5, 1.0, 1.0))
    for index in range(mapsize // 2):
        if index % 2 == 0 and rng.random() < density:
            value = rng.getrandbits(16)
        elif index % 2 == 0:
            value = blank
        else:
            value = rng.getrandbits(16)
        state.w16(cells + 2 * index, value)

    field_16 = rng.choice((rng.randrange(0, 0x100), rng.randrange(0x100, 0x600),
                           rng.randrange(0x600, 0x10000), 0xFF, 0x100, 0x5FF, 0x600))
    state.w16(layer + 0x12, rng.getrandbits(16))
    state.w32(layer + 0x14, rng.getrandbits(32))
    state.w16(layer + 0x16, field_16)
    state.w16(layer + 0x1E, blank)
    state.w32(layer + 0x50, cells)
    state.w32(layer + 0x58, width)
    state.w8(layer + 0x8A, rng.randrange(16))

    lists = state.alloc(4 * 16)
    fill(state, lists, 4 * 16, rng)
    state.w32(sym["data_801987c8"], lists)
    state.w8(sym["data_801a27d0"], rng.randrange(2))
    fill(state, sym["data_801f6734_slot06_0e"], 2 * 117 * 32, rng)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the store of the tile's v coordinate: its offset moves by one.

    The store is `sb reg, 0x15(a1)`, executed once for every drawn cell;
    the byte then lands in the u slot, so records differ.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0x0015]
    if len(found) != 1:
        raise ValueError(f"expected one store of the v coordinate, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x0016, "v coordinate store moved by one byte"


CONTRACT = Contract(setup, control)
