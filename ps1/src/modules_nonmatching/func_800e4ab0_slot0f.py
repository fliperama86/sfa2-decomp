"""Contract of func_800e4ab0_slot0f (reads and writes: see the header of the .c).

Choices of the setup:
  - the map width (field_58) is 512 or 1024, the height (field_5c) is 16 to
    256 in steps of 1 (so the height in tiles, field_5c >> 4, is 1 to 16
    and the low bits of field_5c are random);
  - the vertical scroll is chosen through the tile row it gives: the first
    visible row (as a signed halfword) is -25 to height + 10, so that rows
    outside the map on both sides, negative first rows and fully visible
    windows all occur; the fraction bits of field_14 are random, and
    field_12 is random (so negative scroll values and wraparound occur);
  - count is 1 to 40 in most cases; in one case in eight it is 0, a
    negative halfword or has random upper bits (only the low 16 bits count);
  - every cell is field_1e in 40 percent of the cases of a map, otherwise
    random; field_1e is below 0x8000 in three cases of four, otherwise
    0x8000 or more (a value no cell can equal);
  - the record counter is random, the records and list array are filled
    with random bytes (the top byte of each link word is kept by the
    function, so that is tested), field_8a is 0 to 15.
"""

from contracts import Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym) -> Setup:
    width = rng.choice((512, 1024))
    height = rng.randrange(16, 257)
    tiles = height >> 4
    map_size = tiles * width * 4 + 0x1000
    data = state.alloc(map_size)
    key = rng.randrange(0x8000) if rng.random() < 0.75 else rng.randrange(0x8000, 0x10000)
    fill(state, data, map_size, rng)
    share = 0.4 if rng.random() < 0.7 else 0.0
    for offset in range(0, map_size - 4, 4):
        if rng.random() < share:
            state.w16(data + offset, key)

    layer = state.alloc(0x90)
    fill(state, layer, 0x90, rng)
    state.w32(layer + 0x50, data)
    state.w32(layer + 0x58, width)
    state.w32(layer + 0x5C, (height & ~0xF) | rng.randrange(16) if rng.random() < 0.5 else height)
    state.w16(layer + 0x12, rng.getrandbits(16))
    first_row = rng.randrange(-25, tiles * 16 + 10)
    fy = first_row * 16 + rng.randrange(16) if rng.random() < 0.7 else first_row
    state.w32(layer + 0x14, (0x100000 - (fy << 16) - rng.randrange(0x10000)) & 0xFFFFFFFF)
    state.w16(layer + 0x1E, key)
    state.w8(layer + 0x8A, rng.randrange(16))

    lists = state.alloc(64)
    fill(state, lists, 64, rng)
    state.w32(sym["data_801987c8"], lists)
    records = state.alloc(15 * 40 * 0x1C + 16)
    fill(state, records, 15 * 40 * 0x1C + 16, rng)
    state.w16(sym["data_800f7264_slot0f"], rng.getrandbits(16))

    count = rng.randrange(1, 41)
    if rng.random() < 1 / 8:
        count = rng.choice((0, 0xFFFF, 0x8000, 0x8001 + rng.randrange(0x7FFE)))  # zero or a negative halfword
    if rng.random() < 1 / 8:
        count |= rng.getrandbits(16) << 16  # only the low 16 bits count
    return Setup(args=(layer, records, count), returns_value=False)


def control(words):
    """Alter the stored y of a record: the store of the row offset moves by two bytes (sh ...,0x12)."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0x12]
    if len(found) != 1:
        raise ValueError(f"expected one halfword store at offset 0x12, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x10, "y store moved to the x offset"


CONTRACT = Contract(setup, control)
