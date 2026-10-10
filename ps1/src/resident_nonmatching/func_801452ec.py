"""Contract of func_801452ec (see the header comment of func_801452ec.c).

Choices of the setup:
  - field_03 is 0, 1, 9 or 10 (the other values read a leftover register;
    read from the original's listing, not tested);
  - the object block is random bytes otherwise (0x100 bytes); field_09 and
    the pointer data_801987c8 are random (the pointer is only passed on);
  - the header has 0 to 8 columns and 0 to 6 rows, mostly 1 to 4 by 1 to 3;
    its bytes 1, 3 and 4 to 7 are random; the sequence step (12 bytes) holds
    the header's address at offset 4 and the object's sequence points at it;
  - a cell is 0 in one case in ten, has a zero low part in one in ten
    (a skipped cell; with a non-zero flag in half of them), and otherwise a
    random low part; its top two bits are 0 half of the time, so both
    vertex arms are driven;
  - when more than 40 cells are non-zero the later ones are set to 0 (the
    side holds 40 pairs);
  - the selector data_801a27d0 is 0 or 1; the table is random bytes;
  - the results of func_8015bd0c and func_8015bdd4 are random 32-bit values
    chosen once per case;
  - the log has room for the longest run, the three quad pointers are
    copied (0x28 bytes, 10 words), and every recorder copies the written
    pairs of the side in use (20 words per pair).
"""

from contracts import CallLog, Contract, Setup, fill

QUAD_WORDS = 0x28 // 4


def setup(state, rng, sym) -> Setup:
    if rng.random() < 0.8:
        columns, rows = rng.randrange(1, 5), rng.randrange(1, 4)
    else:
        columns, rows = rng.randrange(0, 9), rng.randrange(0, 7)
    header = state.alloc(8 + 2 * rows * columns + 2)
    fill(state, header, 8 + 2 * rows * columns, rng)
    state.w8(header + 0, columns)
    state.w8(header + 2, rows)
    written = 0
    for index in range(rows * columns):
        pick = rng.random()
        low = 0 if pick < 0.2 else rng.getrandbits(14)
        flag = 0 if rng.random() < 0.5 else rng.randrange(1, 4)
        cell = flag << 14 | low
        if pick < 0.1:
            cell = 0
        if cell & 0x3FFF:
            if written == 40:
                cell = 0
            else:
                written += 1
        state.w16(header + 8 + 2 * index, cell)

    step = state.alloc(12)
    fill(state, step, 12, rng)
    state.w32(step + 4, header)

    kind = rng.choice((0, 1, 9, 10))
    side = kind & 1
    obj = state.alloc(0x100)
    fill(state, obj, 0x100, rng)
    state.w8(obj + 0x03, kind)
    state.w32(obj + 0x18, step)

    state.w32(sym["data_801a27d0"], rng.randrange(2))
    state.w32(sym["data_801987c8"], rng.getrandbits(32))
    fill(state, sym["data_801a4ff0"], 2 * 0xC80, rng)

    watch = ((sym["data_801a4ff0"] + side * 0xC80, 20 * max(written, 1)),)
    # Room for the longest run: per drawn cell one entry for each callee
    # (address, arguments, pointee, watched pairs).
    watched = 20 * max(written, 1)
    per_cell = (1 + 1 + QUAD_WORDS) + (1 + 4) + (1 + 2) + (1 + 2 + QUAD_WORDS) + (1 + 2 + QUAD_WORDS) + 5 * watched
    log = CallLog(state, per_cell * written + 16, watch=watch)
    log.replace(sym["func_8015c09c"], 1, 0, {0: QUAD_WORDS})
    log.replace(sym["func_8015bd0c"], 4, rng.getrandbits(32))
    log.replace(sym["func_8015bdd4"], 2, rng.getrandbits(32))
    log.replace(sym["func_8015bfe8"], 2, 0, {0: QUAD_WORDS})
    log.replace(sym["func_8015bf34"], 2, 0, {1: QUAD_WORDS})
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of the texture byte at offset 0x25 of the quad.

    The store is the one byte store with that offset; every call with at
    least one drawn cell makes it.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0x25]
    if len(found) != 1:
        raise ValueError(f"expected one store at offset 0x25, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x26, "texture byte store moved by one byte"


CONTRACT = Contract(setup, control)
