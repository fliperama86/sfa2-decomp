"""Contract of func_80151324 (see the header comment of the .c file).

Choices made here:
  - game_state.field_64 is 0 in nine cases of ten, otherwise a random
    non-zero byte;
  - the table holds 4 frame records of random bytes; the object's sequence
    step names one of them (halfword at 0xa, 0 to 3); the record's field_05
    is 0 in one case of eight (the early return), otherwise 1 to 4;
  - data_8017ff68 gets four pointers, to four maps of this case; the maps
    have width 0 to 6 and height 0 to 5 (width or height 0 in one case of
    eight), tile bytes that are 0 in a third of the cells and otherwise
    random (so that flip bit, tile column and row vary);
  - object.field_02 is 0 to 3, field_0b, the halfword at 0x12, box_margin
    and data_801aa5ea are random, the buffer selector data_801a27d0 is 0 or 1;
  - the record buffer data_8018947c is filled with random bytes for the
    range the function can reach;
  - data_801987c8 is the address of a block of the setup;
  - func_8015bf34 is a recorder taking two arguments; the 10 words of the
    record that its second argument points at are copied into the log, and the
    record buffer (2900 words from data_8018947c) is watched at every call.
"""

from contracts import CallLog, Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym):
    game_state = sym["game_state"]
    state.w8(game_state + 0x64, 0 if rng.random() < 0.9 else rng.randrange(1, 256))

    maps = []
    for _ in range(4):
        if rng.random() < 0.125:
            width, height = rng.choice(((0, rng.randrange(6)), (rng.randrange(7), 0)))
        else:
            width, height = rng.randrange(1, 7), rng.randrange(1, 6)
        block = state.alloc(4 + width * height + 4)
        fill(state, block, 4 + width * height + 4, rng)
        state.w8(block + 0, width)
        state.w8(block + 1, height)
        for index in range(width * height):
            tile = 0 if rng.random() < 0.33 else rng.getrandbits(8)
            state.w8(block + 4 + index, tile)
        maps.append(block)
    for index, block in enumerate(maps):
        state.w32(sym["data_8017ff68"] + 4 * index, block)

    table = state.alloc(4 * 16)
    fill(state, table, 4 * 16, rng)
    for index in range(4):
        state.w8(table + 16 * index + 5, 0 if rng.random() < 0.125 else rng.randrange(1, 5))

    step = state.alloc(12)
    fill(state, step, 12, rng)
    state.w16(step + 0xA, rng.randrange(4))

    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x02, rng.randrange(4))
    state.w32(obj + 0x18, step)

    state.w16(sym["box_margin"], rng.getrandbits(16))
    state.w16(sym["data_801aa5ea"], rng.getrandbits(16))
    state.w32(sym["data_801a27d0"], rng.randrange(2))
    state.w32(sym["data_801987c8"], state.alloc(0x80))
    fill(state, sym["data_8018947c"], 4 * 65 * 32 + 40 * 80 + 80, rng)

    # 30 cells at most, each entry holds 3 + 10 + 2900 words.
    watched = (4 * 65 * 32 + 40 * 80 + 80) // 4
    log = CallLog(state, words=31 * (13 + watched), watch=((sym["data_8018947c"], watched),))
    # The callee may change the buffer selector and the object's field_0b (a
    # callee's effect is inferred, not known): in half of the cases the recorder
    # stores new words there at its first call. The original reads the flags
    # once, before the loops, and the selector for each cell.
    stores = ()
    if rng.random() < 0.5:
        stores = ((1, sym["data_801a27d0"], rng.randrange(2)), (1, obj + 8, rng.getrandbits(32)))
    log.replace(sym["func_8015bf34"], 2, rng.getrandbits(32), pointees={1: 10}, stores=stores)
    return Setup(args=(obj, table), returns_value=False)


def control(words):
    """Alter the store of the field at record offset 0x25 (a byte store)."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0x25]
    if not found:
        raise ValueError("no byte store at offset 0x25")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0x26, "a byte store at record offset 0x25 moves to 0x26"


CONTRACT = Contract(setup, control)
