"""Contract of func_801e84cc_slot06_0e: no argument, draws layer data_801aa544[0].

Reads and writes are listed in the header comment of
func_801e84cc_slot06_0e.c. Choices made here:
  - field_58 is a power of two from 16 to 2048 in three cases of four and
    any value from 16 to 4000 otherwise (smaller values and larger ones
    would make the column index run over 65536 cells or over the map);
  - field_12, field_14 and the game_state.field_92 are random 16-bit and
    32-bit values, so that every sign and the carry of the row are tried;
  - field_1e (blank) is negative in one case of three, so that the unsigned
    compare of the map's tile against the signed blank is tried both ways;
  - the map holds, per case, mostly blank cells, about half blank cells or
    no blank cells at all; the others are random tile words;
  - the table holds 2 buffers of 208 records and the function writes one
    record per drawn cell from the start of the selected buffer, without
    a check. The setup counts the cells that the function will draw, with
    the function's own walk over the map and its own comparison (a tile
    read as unsigned against the blank value read as signed, so a
    negative blank value draws all 400 cells), and keeps the last record
    inside the table: buffer 1 is selected only when at most 208 cells
    are drawn, and in half of those cases cells are blanked until exactly
    208 are, the last record of the table. An assertion states the bound;
  - the list array has 16 words of random content, field_8a indexes it;
  - the record table is filled with random bytes, so that the words the
    function rewrites and the bytes it leaves alone are tested.
"""

from contracts import Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


RECORDS = 208          # records in one buffer of the table; the table has two buffers


def u16(state, address):
    return int.from_bytes(state.read(address, 2), "little")


def u32(state, address):
    return int.from_bytes(state.read(address, 4), "little")


def visited(state, layer, game_state):
    """The addresses of the 400 map cells in the order the function reads them (a cell may come twice)."""
    position = (0x100000 - u32(state, layer + 0x14)) & 0xFFFFFFFF
    y = ((position >> 16) - u16(state, game_state + 0x92)) & 0xFFFF
    row = (y & 0x1F0) >> 4
    width = u32(state, layer + 0x58)
    mask = ((width >> 4) - 1) & 0xFFFF
    column = ((u16(state, layer + 0x12) & (width - 1)) >> 4) & 0xFFFF
    cells = u32(state, layer + 0x50)
    out = []
    for i in range(16):
        line = cells + 2 * (((row + i) & 0xF) << 6)
        cc = column
        for _ in range(25):
            out.append(line + 2 * (((cc >> 5) << 10) + ((cc & 0x1F) << 1)))
            cc = mask & (cc + 1)
    return out


def drawn(state, cells, blank):
    """How many records the function writes: one per visit of a cell whose tile is not the blank value.
    The tile is an unsigned halfword and the blank value a signed one, so a negative blank equals no tile."""
    if blank >= 0x8000:
        return len(cells)
    return sum(1 for address in cells if u16(state, address) != blank)


def setup(state, rng, sym):
    game_state = sym["game_state"]
    layer = sym["data_801aa544"]
    state.w16(game_state + 0x92, rng.getrandbits(16))

    if rng.random() < 0.75:
        width = 16 << rng.randrange(0, 8)
    else:
        width = rng.randrange(16, 4001)
    columns = (width >> 4) + 2
    blocks = (columns >> 5) + 1
    mapsize = 2048 * blocks
    cells = state.alloc(mapsize + 8)

    if rng.random() < 0.33:
        blank = rng.randrange(0x8000, 0x10000)
    else:
        blank = rng.randrange(0, 0x8000)
    density = rng.choice((0.05, 0.5, 1.0, 1.0))
    for index in range(mapsize // 2):
        if index % 2 == 0 and rng.random() < density:
            value = rng.getrandbits(16)
        elif index % 2 == 0:
            value = blank
        else:
            value = rng.getrandbits(16)
        state.w16(cells + 2 * index, value)

    state.w16(layer + 0x12, rng.getrandbits(16))
    state.w32(layer + 0x14, rng.getrandbits(32))
    state.w16(layer + 0x1E, blank)
    state.w32(layer + 0x50, cells)
    state.w32(layer + 0x58, width)
    state.w8(layer + 0x8A, rng.randrange(16))

    walk = visited(state, layer, game_state)
    assert all(cells <= address < cells + mapsize for address in walk), "a visited cell lies outside the map"
    selector = rng.randrange(2)
    if selector == 1 and blank >= 0x8000:
        selector = 0                     # all 400 cells are drawn: they fit only from the table's start
    elif selector == 1 and drawn(state, walk, blank) > RECORDS:
        if rng.random() < 0.5:
            selector = 0
        else:                            # blank cells until the second buffer is exactly full
            order = list(dict.fromkeys(walk))
            rng.shuffle(order)
            for address in order:
                if drawn(state, walk, blank) <= RECORDS:
                    break
                if u16(state, address) != blank and drawn(state, walk, blank) - walk.count(address) >= RECORDS:
                    state.w16(address, blank)
            if drawn(state, walk, blank) > RECORDS:
                selector = 0
    assert selector * RECORDS + drawn(state, walk, blank) <= 2 * RECORDS, "the function would write past its table"

    lists = state.alloc(4 * 16)
    fill(state, lists, 4 * 16, rng)
    state.w32(sym["data_801987c8"], lists)
    state.w8(sym["data_801a27d0"], selector)
    fill(state, sym["data_801ef554_slot06_0e"], 2 * 208 * 28, rng)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the store of the tile's v coordinate: its offset moves by one.

    The store is `sb reg, 0x15(a1)`, executed once for every drawn cell;
    the byte then lands in the u slot, which the other store overwrites
    first or last, so records differ.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0x0015]
    if len(found) != 1:
        raise ValueError(f"expected one store of the v coordinate, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x0016, "v coordinate store moved by one byte"


CONTRACT = Contract(setup, control)
