"""Contract of func_801e8bd8_slot06_08: no argument, draws a tile layer in perspective.

Reads and writes are listed in the header comment of func_801e8bd8_slot06_08.c.
Choices made here:
  - the layer block data_801aa5d4 gets random words for the two slopes
    (field_08, field_10; half of the cases small values), random field_12,
    field_1c, field_1e, field_4e; the other bytes of the block keep what the
    image holds;
  - the map is 256, 512 or 1024 pixels wide and has 0 to 40 rows of tiles,
    in 1 to 3 groups of 16 rows (as the function indexes it); the tile
    cells are the blank value (field_1e) in a share of the cases chosen per
    case (nearly all blank, half, a few), the others random 32-bit cells;
  - the first row of the first pass is steered: in nine cases of ten it is
    between -8 and the map's height plus 2, so that the rows skipped above
    and below the map and the rows drawn are all reached; one in ten uses
    a random field_3e;
  - the sum field_16 + game_state.field_92 is steered onto 0, 1..15, 16..31,
    32 and more, and negative values, one in nine each, one in ten random;
  - field_4c is mostly 144 to 271 (the second pass runs 0 to 6 rows), in
    one case of eight 0 to 143 or -15 to -1 (up to 15 rows), in one of
    twenty a larger value (no second pass);
  - field_4e - field_1c is never 0 in its low 16 bits (the second pass
    divides by it; the original traps);
  - field_4c below -15 is not generated: the second pass would then index
    the map with negative rows, outside any block;
  - the function writes one record per drawn tile from the start of the
    selected buffer, without a check. The setup counts the tiles that the
    function will draw with the function's own walk (rows of the first pass
    skipped outside the map, the second pass from field_4c / 16 to row 14,
    33 columns wrapping at the width, a tile drawn when it differs from
    field_1e), and keeps the last record inside the table: buffer 1 is
    kept only when at most 264 tiles are drawn, otherwise (or when the
    drawn tiles exceed the capacity) tiles are blanked, in most cases
    until the capacity is exactly full, so the last record of the table
    is written. An assertion states the bound;
  - the record table is filled with random bytes; the list array has 256
    words of random content, field_8a (a random byte) indexes it; the
    selector is 0 or 1.
"""
from contracts import Contract, Setup

BUFFER = 0x108


def s16(v):
    v &= 0xFFFF
    return v - 0x10000 if v & 0x8000 else v


def u16(state, address):
    return int.from_bytes(state.read(address, 2), "little")


def u32(state, address):
    return int.from_bytes(state.read(address, 4), "little")


def trunc16(v):
    """Division by 16 rounding toward zero, as the compiled code does."""
    return -((-v) >> 4) if v < 0 else v >> 4


def visited(state, layer, game_state):
    """The addresses of the map cells in the order the function reads them (a cell may come twice).

    Mirrors the function: the first pass takes 3 to 6 rows from the row of
    field_3e (rows outside 0..field_5c/16 are skipped), the second pass takes
    the rows from field_4c / 16 up to 14; each row is 33 cells, the column
    wrapping with the map's width."""
    n = s16(u16(state, layer + 0x3E))
    width = u32(state, layer + 0x58)
    rows_total = s16(u32(state, layer + 0x5C) >> 4)
    f4c = s16(u16(state, layer + 0x4C))
    mask = ((width >> 4) - 1) & 0xFFFF
    col0 = (((s16(u16(state, layer + 0x12)) - 0x60) & (width - 1)) >> 4) & 0xFFFF
    cells = u32(state, layer + 0x50)
    stride = (width >> 9) << 11
    total = s16(u16(state, layer + 0x16)) + s16(u16(state, game_state + 0x92))
    count = 3 if total == 0 else 4 if total < 0x10 else 5 if total < 0x20 else 6
    rows = []
    row = (n - 0x40) >> 4
    for _ in range(count):
        if 0 <= s16(row) < rows_total:
            rows.append(row)
        row += 1
    row = trunc16(f4c)
    for _ in range(max(0, 15 - trunc16(f4c))):
        rows.append(row)
        row += 1
    out = []
    for row in rows:
        line = cells + ((row & 0xFFFF) >> 4) * stride + (row & 0xF) * 128
        cc = col0
        for _ in range(33):
            out.append(line + (cc >> 5) * 2048 + (cc & 0x1F) * 4)
            cc = mask & (cc + 1)
    return out


def drawn(state, cells, blank):
    """How many records the function writes: one per visit of a cell whose tile is not the blank value."""
    return sum(1 for address in cells if u16(state, address) != blank)


def setup(state, rng, sym):
    r = rng.random
    layer = sym["data_801aa5d4"]
    game_state = sym["game_state"]

    width = rng.choice((256, 512, 512, 1024, 1024))
    rows = rng.choice((0, 1, 5, 12, 16, 17, rng.randrange(0, 41), rng.randrange(0, 41)))
    height = rows * 16 + rng.randrange(16)
    groups = max(1, -(-rows // 16))
    stride = (width >> 9) << 11  # bytes between groups of 16 rows
    blocks = max(1, width >> 9)

    # sum of field_16 and game_state.field_92
    f92 = rng.randrange(-1000, 1001)
    pick = rng.randrange(10)
    target = (0, rng.randrange(1, 16), rng.randrange(16, 32), rng.randrange(32, 200),
              rng.randrange(-200, 0), 0, rng.randrange(1, 16), rng.randrange(16, 32),
              rng.randrange(32, 100), None)[pick]
    f16 = rng.getrandbits(16) if target is None else (target - f92) & 0xFFFF
    state.w16(game_state + 0x92, f92 & 0xFFFF)
    state.w16(layer + 0x16, f16)

    if r() < 0.9:
        row0 = rng.randrange(-8, rows + 3)
        n = 0x40 + row0 * 16 + rng.randrange(16)
    else:
        n = s16(rng.getrandbits(16))
    state.w16(layer + 0x3E, n & 0xFFFF)

    q = r()
    if q < 0.7:
        f4c = rng.randrange(144, 272)
    elif q < 0.8:
        f4c = rng.randrange(0, 144)
    elif q < 0.875:
        f4c = rng.randrange(-15, 0)
    elif q < 0.95:
        f4c = rng.randrange(144, 272)
    else:
        f4c = rng.randrange(272, 32768)
    f4c &= 0xFFFF
    state.w16(layer + 0x4C, f4c)

    f1c = rng.getrandbits(16)
    d2 = rng.choice((rng.randrange(1, 40), -rng.randrange(1, 40), s16(rng.getrandbits(16)) or 1))
    state.w16(layer + 0x1C, f1c)
    state.w16(layer + 0x4E, (f1c + d2) & 0xFFFF)
    blank = rng.getrandbits(16)
    state.w16(layer + 0x1E, blank)
    f12 = rng.getrandbits(16)
    state.w16(layer + 0x12, f12)
    if r() < 0.5:
        state.w32(layer + 0x08, rng.getrandbits(32))
        state.w32(layer + 0x10, rng.getrandbits(32))
    else:
        state.w32(layer + 0x08, rng.randrange(0, 1 << 20))
        state.w32(layer + 0x10, rng.randrange(0, 1 << 22))
    state.w32(layer + 0x58, width)
    state.w32(layer + 0x5C, height)
    field_8a = rng.randrange(256)
    state.w8(layer + 0x8A, field_8a)

    # tile map
    size = groups * stride if stride else 2048
    size = max(size, 2048 * blocks)
    tmap = state.alloc(size + 16)
    share = rng.choice((1.0, 0.97, 0.9, 0.5, 0.1, 0.0))

    def cell(row, cc):
        return tmap + (row >> 4) * stride + (row & 15) * 128 + (cc >> 5) * 2048 + (cc & 31) * 4

    for g in range(groups):
        for off in range(0, 2048 * blocks, 4):
            base = tmap + g * stride + off
            if r() < share:
                state.w16(base, blank)
            else:
                state.w16(base, rng.getrandbits(16))
            state.w16(base + 2, rng.getrandbits(16))
    state.w32(layer + 0x50, tmap)

    # keep the last record inside the table (the function does not check)
    walk = visited(state, layer, game_state)
    assert all(tmap <= address < tmap + size for address in walk), "a visited cell lies outside the map"
    selector = rng.randrange(2)
    capacity = 2 * BUFFER - selector * BUFFER
    if drawn(state, walk, blank) > capacity:
        if rng.random() < 0.5 and selector == 1:
            selector = 0
            capacity = 2 * BUFFER
        if drawn(state, walk, blank) > capacity:
            # blank cells until the buffer is full or, when that cannot be hit, just below
            order = list(dict.fromkeys(walk))
            rng.shuffle(order)
            for address in order:
                total = drawn(state, walk, blank)
                if total <= capacity:
                    break
                if u16(state, address) != blank:
                    if total - walk.count(address) >= capacity or rng.random() < 0.3:
                        state.w16(address, blank)
    elif rng.random() < 0.7:
        # fewer tiles than the capacity: draw more until the buffer is exactly full, when that can be hit
        order = list(dict.fromkeys(walk))
        rng.shuffle(order)
        for address in order:
            total = drawn(state, walk, blank)
            if total == capacity:
                break
            if u16(state, address) == blank and total + walk.count(address) <= capacity:
                state.w16(address, (blank + 1 + rng.randrange(0xFFFF)) & 0xFFFF)
    total = drawn(state, walk, blank)
    assert selector * BUFFER + total <= 2 * BUFFER, "the function would write past its table"

    lists = state.alloc(4 * 256)
    state.write(lists, bytes(rng.getrandbits(8) for _ in range(4 * 256)))
    state.w32(sym["data_801987c8"], lists)
    state.w32(sym["data_801a27d0"], selector)
    table = sym["data_801f2504_slot06_08"]
    state.write(table, bytes(rng.getrandbits(8) for _ in range(2 * BUFFER * 40)))
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the comparison that picks five rows rather than six for the first pass.

    The first pass draws 3 rows when the sum of field_16 and game_state.field_92
    is 0, 4 below 16, 5 below 32 and 6 otherwise. The one `slti reg, reg, 0x20`
    of the build is that test; its limit becomes 0x10, so that sums from 16 to 31
    draw six rows instead of five.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0A and w & 0xFFFF == 0x20]
    if len(found) != 1:
        raise ValueError(f"expected one slti with 0x20, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x10, "row count limit 0x20 changed to 0x10"


CONTRACT = Contract(setup, control)
