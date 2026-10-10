"""Contract of func_8013bfa4: table objects hitting a player's boxes.

The reads and writes are listed in the header comment of func_8013bfa4.c.
Choices made by the setup:
  - count_8018f59c is 0 in one case of twenty, a negative 16-bit value in one
    case of thirty (the loop is skipped), else 1 to 6;
  - the table data_8018f5e0 takes its entries downward from the symbol's
    address, each picked from a pool of 2 to 4 table objects;
  - a table object has type (field_00) 1 in nine cases of ten, field_65 0 or
    1, kind (field_02) 0x17 in one case of four and a random value 0 to 40
    otherwise (shift amounts above 23 are tried), field_49 zero or not,
    field_08 zero or not, field_5c from a short list of values around the
    countdown limit, a frame record with active zero in one case of eight and
    field_04 a small number, a box_tables block whose boxes_b word points at a
    list of 32-byte box records, and a wide_boxes list of the same kind;
  - the two players have field_263, field_27b and field_163 zero in nine
    cases of ten each, field_295 zero in half of the cases, field_69 small,
    field_45 zero in half, field_61 255 in one case of four, field_5c from a
    short list around zero, field_08 zero or not, a frame record whose box_a,
    box_b and box_c are each zero in one case of five, and three lists of
    6-byte box records;
  - positions, origins, extents are small numbers (so that boxes overlap) in
    nine cases of ten and random otherwise;
  - data_80190468 points at a block of its own;
  - the six pointer words the function writes (ref_first, ref_second,
    data_80190458, ref_other, ref_third, data_801904c0) start as random words;
  - every recorder copies, at each call, whole: the table objects, both
    players, the block at data_80190468, data_80188ed0, data_80188f30,
    ref_first to data_80190414 and its next word, data_80190458,
    data_801904c0, ref_other and ref_third; no pointer argument is filled for
    the call (the box is a record of an existing list, not word aligned), so
    there is no pointee;
  - the four replaced callees return 0 and the overlap record starts with
    random content (func_80139a1c clears it).
"""

from contracts import CallLog, Contract, Setup, fill


def _near(rng, bound):
    return rng.randrange(-bound, bound + 1) & 0xFFFF if rng.random() < 0.9 else rng.getrandbits(16)


def _byte_near(rng, bound):
    return rng.randrange(0, bound) if rng.random() < 0.9 else rng.getrandbits(8)


def _list6(state, rng):
    block = state.alloc(6 * 4)
    for index in range(4):
        base = block + 6 * index
        state.w16(base + 0, _near(rng, 20))
        state.w16(base + 2, _near(rng, 20))
        state.w8(base + 4, _byte_near(rng, 30))
        state.w8(base + 5, _byte_near(rng, 30))
    return block


def _list32(state, rng):
    block = state.alloc(32 * 4)
    fill(state, block, 32 * 4, rng)
    for index in range(4):
        base = block + 32 * index
        state.w16(base + 0, _near(rng, 20))
        state.w16(base + 2, _near(rng, 20))
        state.w16(base + 4, _byte_near(rng, 30))
        state.w16(base + 6, _byte_near(rng, 30))
    return block


def _frame(state, rng):
    frame = state.alloc(0x10)
    fill(state, frame, 0x10, rng)
    return frame


def _common(state, rng, obj):
    fill(state, obj, 0x394, rng)
    state.w16(obj + 0x12, _near(rng, 20))
    state.w16(obj + 0x16, _near(rng, 20))
    state.w8(obj + 0x0B, rng.choice((0, 0, rng.getrandbits(8))))
    state.w8(obj + 0x08, rng.choice((0, rng.getrandbits(8))))
    state.w16(obj + 0x5C, rng.choice((0, 1, 2, 0x7FFF, 0x8000, 0xFFFF, rng.getrandbits(16))))


def setup(state, rng, sym) -> Setup:
    table = sym["data_8018f5e0"]
    roll = rng.random()
    if roll < 0.05:
        count = 0
    elif roll < 0.083:
        count = 0x10000 - rng.randrange(1, 40)
    else:
        count = rng.randrange(1, 7)
    state.w16(sym["count_8018f59c"], count)
    fill(state, sym["data_80188ed0"], 0x4C, rng)
    state.w8(sym["data_80188f30"], rng.getrandbits(8))

    objects = []
    for _ in range(rng.randrange(2, 5)):
        obj = state.alloc(0x394)
        _common(state, rng, obj)
        state.w8(obj + 0x00, 1 if rng.random() < 0.9 else rng.getrandbits(8))
        state.w8(obj + 0x02, 0x17 if rng.random() < 0.25 else rng.randrange(0, 41))
        state.w8(obj + 0x49, rng.choice((0, rng.randrange(1, 256))))
        state.w8(obj + 0x65, rng.randrange(2))
        frame = _frame(state, rng)
        state.w8(frame + 0, 0 if rng.random() < 1 / 8 else rng.randrange(1, 4))
        state.w8(frame + 4, rng.randrange(0, 4))
        state.w32(obj + 0x88, frame)
        tables = state.alloc(0x14)
        fill(state, tables, 0x14, rng)
        state.w32(tables + 4, _list32(state, rng))
        state.w32(obj + 0x6C, tables)
        state.w32(obj + 0x144, _list32(state, rng))
        objects.append(obj)

    for name in ("player_left", "player_right"):
        player = sym[name]
        _common(state, rng, player)
        for offset in (0x263, 0x27B, 0x163):
            state.w8(player + offset, 0 if rng.random() < 0.9 else rng.randrange(1, 256))
        state.w8(player + 0x295, rng.choice((0, rng.randrange(1, 256))))
        state.w8(player + 0x69, rng.randrange(0, 4) if rng.random() < 0.9 else rng.getrandbits(8))
        state.w8(player + 0x45, rng.choice((0, rng.randrange(1, 256))))
        state.w8(player + 0x61, 0xFF if rng.random() < 0.25 else rng.getrandbits(8))
        frame = _frame(state, rng)
        for offset in (1, 2, 3):
            state.w8(frame + offset, 0 if rng.random() < 0.2 else rng.randrange(1, 4))
        state.w32(player + 0x88, frame)
        for offset in (0x138, 0x13C, 0x140):
            state.w32(player + offset, _list6(state, rng))

    block = state.alloc(0x394)
    fill(state, block, 0x394, rng)
    state.w32(sym["data_80190468"], block)

    for index in range(12):
        state.w32(table - 4 * index, rng.choice(objects))

    # the pointer words the function writes start random
    for name in ("ref_first", "ref_second", "data_80190458", "ref_other", "ref_third", "data_801904c0"):
        state.w32(sym[name], rng.getrandbits(32))

    watch = [(sym["ref_first"], 4), (sym["data_80190458"], 1), (sym["ref_other"], 1), (sym["ref_third"], 1),
             (sym["data_801904c0"], 1),
             (sym["data_80188ed0"], 0x4C // 4), (sym["data_80188f30"], 1), (block, 0x394 // 4)]
    watch += [(obj, 0x394 // 4) for obj in objects]
    watch += [(sym["player_left"], 0x394 // 4), (sym["player_right"], 0x394 // 4)]
    log = CallLog(state, 60000, watch=watch)
    log.replace(sym["func_80139a78"], 2, 0)
    log.replace(sym["func_8013a3a8"], 3, 0)
    log.replace(sym["func_8013af1c"], 3, 0)
    log.replace(sym["func_80155de0"], 2, 0)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the constant 0x17 that the code compares the object's kind with."""
    found = [i for i, w in enumerate(words) if w >> 26 in (0x09, 0x0D) and (w >> 21) & 31 == 0 and w & 0xFFFF == 0x17]
    if not found:
        raise ValueError("no load of the constant 0x17 found")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x16, "kind constant 0x17 changed to 0x16"


CONTRACT = Contract(setup, control)
