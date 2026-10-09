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
  - the tiles that would be drawn are counted and surplus ones are made
    blank, so that no more than 264 records are written (the buffer);
  - the record table is filled with random bytes; the list array has 256
    words of random content, field_8a (a random byte) indexes it; the
    selector is 0 or 1.
"""
from contracts import Contract, Setup

BUFFER = 0x108


def s16(v):
    v &= 0xFFFF
    return v - 0x10000 if v & 0x8000 else v


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

    # count the tiles drawn and blank the surplus
    mask = (width >> 4) - 1
    col0 = (((f12 - 0x60) & 0xFFFF) & (width - 1)) >> 4
    visited = []
    sums = f92 + s16(f16)
    count = 3 if sums == 0 else 4 if sums < 16 else 5 if sums < 32 else 6
    row0 = (n - 0x40) >> 4
    for i in range(count):
        row = row0 + i
        if 0 <= row < height >> 4:
            visited.append(row)
    r0 = int(s16(f4c) / 16)
    for row in range(r0, 15):
        visited.append(row)
    drawn = []
    for row in visited:
        cc = col0
        for _ in range(33):
            drawn.append(cell(row, cc))
            cc = (cc + 1) & mask
    live = [a for a in drawn if int.from_bytes(state.read(a, 2), "little") != blank]
    surplus = len(live) - BUFFER
    if surplus > 0:
        rng.shuffle(live)
        for a in live[:surplus]:
            state.w16(a, blank)

    lists = state.alloc(4 * 256)
    state.write(lists, bytes(rng.getrandbits(8) for _ in range(4 * 256)))
    state.w32(sym["data_801987c8"], lists)
    state.w32(sym["data_801a27d0"], rng.randrange(2))
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
