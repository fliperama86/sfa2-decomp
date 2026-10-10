"""Contract of func_8013c6ac: the table objects of kind 9 push the players.

The reads and writes are listed in the header comment of func_8013c6ac.c.
Choices made by the setup:
  - count_8018f59c is 0 in one case of twenty, a negative 16-bit value in one
    case of thirty (the loop is skipped), else 1 to 8;
  - the table data_8018f5e0 takes its entries downward from the symbol's
    address, each picked from a pool of table objects, player_left and
    player_right;
  - table objects have kind (field_02) 9 in five cases of six and type
    (field_00) 1 in five of six, field_65 0 or 1, a frame record with
    field_07 zero in one case of eight, a box_tables block whose boxes_c word
    points at a list of 6-byte box records;
  - the two players are filled with random bytes, then given field_45 zero in
    five cases of six, a frame (field_07 zero in one case of eight), an
    unknown_148 list of box records, field_164 zero or not, and an "other"
    object (a block of its own, or a table object);
  - the six pointer words the function writes (ref_first, ref_second,
    data_80190414, data_80190458, ref_other, ref_third) start as random words;
  - positions, box origins and extents are small numbers (so that the boxes
    overlap) in four cases of five and random otherwise; field_0b is 0 or a
    random byte.
"""

from contracts import Contract, Setup, fill


def _small(rng, bound):
    return rng.randrange(-bound, bound + 1) & 0xFFFF


def _near(rng, bound):
    return _small(rng, bound) if rng.random() < 0.8 else rng.getrandbits(16)


def _box_list(state, rng):
    block = state.alloc(6 * 4)
    for index in range(4):
        base = block + 6 * index
        state.w16(base + 0, _near(rng, 30))
        state.w16(base + 2, _near(rng, 30))
        state.w8(base + 4, rng.randrange(0, 40) if rng.random() < 0.8 else rng.getrandbits(8))
        state.w8(base + 5, rng.randrange(0, 40) if rng.random() < 0.8 else rng.getrandbits(8))
    return block


def _frame(state, rng):
    frame = state.alloc(0x10)
    fill(state, frame, 0x10, rng)
    state.w8(frame + 7, 0 if rng.random() < 1 / 8 else rng.randrange(1, 4))
    return frame


def _common(state, rng, obj):
    fill(state, obj, 0x394, rng)
    state.w16(obj + 0x12, _near(rng, 30))
    state.w16(obj + 0x16, _near(rng, 30))
    state.w8(obj + 0x0B, rng.choice((0, 0, rng.getrandbits(8))))
    state.w32(obj + 0x88, _frame(state, rng))


def setup(state, rng, sym) -> Setup:
    table = sym["data_8018f5e0"]
    roll = rng.random()
    if roll < 0.05:
        count = 0
    elif roll < 0.083:
        count = 0x10000 - rng.randrange(1, 40)
    else:
        count = rng.randrange(1, 9)
    state.w16(sym["count_8018f59c"], count)
    # the six pointer words the function writes start random
    for name in ("ref_first", "ref_second", "data_80190414", "data_80190458", "ref_other", "ref_third"):
        state.w32(sym[name], rng.getrandbits(32))

    objects = []
    for _ in range(rng.randrange(2, 5)):
        obj = state.alloc(0x394)
        _common(state, rng, obj)
        state.w8(obj + 0x02, 9 if rng.random() < 5 / 6 else rng.randrange(256))
        state.w8(obj + 0x00, 1 if rng.random() < 5 / 6 else rng.randrange(256))
        state.w8(obj + 0x65, rng.randrange(2))
        tables = state.alloc(0x14)
        fill(state, tables, 0x14, rng)
        state.w32(tables + 8, _box_list(state, rng))
        state.w32(obj + 0x6C, tables)
        objects.append(obj)

    players = []
    for name in ("player_left", "player_right"):
        player = sym[name]
        _common(state, rng, player)
        state.w8(player + 0x45, 0 if rng.random() < 5 / 6 else rng.getrandbits(8))
        state.w32(player + 0x148, _box_list(state, rng))
        state.w8(player + 0x164, rng.choice((0, rng.randrange(1, 256))))
        players.append(player)
    for player in players:
        other = state.alloc(0x394)
        fill(state, other, 0x394, rng)
        state.w32(player + 0x40, rng.choice((other, other, rng.choice(objects), rng.choice(players))))

    pool = objects + players
    for index in range(12):
        state.w32(table - 4 * index, rng.choice(objects * 2 + pool))
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the constant 9 that the code compares the objects' kinds with."""
    found = [i for i, w in enumerate(words) if w >> 26 in (0x09, 0x0D) and (w >> 21) & 31 == 0 and w & 0xFFFF == 0x9]
    if not found:
        raise ValueError("no load of the constant 9 found")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0xA, "kind constant 9 changed to 10"


CONTRACT = Contract(setup, control)
