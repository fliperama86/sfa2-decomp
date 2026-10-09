"""Contract of func_8013b558: a scan of the object table for colliding pairs.

The reads and writes are listed in the header comment of func_8013b558.c.
Choices made by the setup:
  - count_8018f59c is 0 or 1 in one case of ten (early returns), a negative
    16-bit value in one case of thirty (the loop is then skipped), else 2 to 8;
  - the table data_8018f5e0 takes its entries downward from the symbol's
    address (word 0, then -4, -8, ...), each one picked from a pool of 3 to 5
    distinct objects, so that an object meets itself in many cases;
  - each object has kind (field_02) 0x17 in one case of six, type (field_00)
    1 in five of six, field_65 from 0 to 2, a frame record whose box_a is 0 in
    one case of five and 1 to 3 otherwise, and a box_tables block whose first
    word points at a block of box records; the other bytes are random;
  - func_801397d0 is a recorder with three arguments, returning 0 in about
    three cases of five, else a value with a non-zero low byte, or one whose
    low byte is 0 but higher bits are set (the call's result is a byte);
  - func_8013b7b8 is a recorder without arguments;
  - every recorder copies, at each call, the globals the function writes
    (ref_first to data_80190414 and the word after it, data_80190458,
    ref_other, ref_third); no recorded pointer argument points at memory the
    function filled for the call (the box is a record of a list that exists
    before the call, and not word aligned), so there is no pointee.
"""

from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym) -> Setup:
    table = sym["data_8018f5e0"]
    roll = rng.random()
    if roll < 0.05:
        count = 0
    elif roll < 0.10:
        count = 1
    elif roll < 0.133:
        count = 0x10000 - rng.randrange(1, 40)
    else:
        count = rng.randrange(2, 9)
    state.w16(sym["count_8018f59c"], count)

    objects = []
    for _ in range(rng.randrange(3, 6)):
        box_list = state.alloc(6 * 4)
        fill(state, box_list, 6 * 4, rng)
        tables = state.alloc(0x14)
        fill(state, tables, 0x14, rng)
        state.w32(tables + 0, box_list)
        frame = state.alloc(0x10)
        fill(state, frame, 0x10, rng)
        state.w8(frame + 3, 0 if rng.random() < 0.2 else rng.randrange(1, 4))
        obj = state.alloc(0x394)
        fill(state, obj, 0x394, rng)
        state.w8(obj + 0x02, 0x17 if rng.random() < 1 / 6 else rng.choice((0, 1, 2, 0x16, 0x18)))
        state.w8(obj + 0x00, 1 if rng.random() < 5 / 6 else rng.choice((0, 2)))
        state.w8(obj + 0x65, rng.randrange(3))
        state.w32(obj + 0x6C, tables)
        state.w32(obj + 0x88, frame)
        objects.append(obj)

    for index in range(12):
        state.w32(table - 4 * index, rng.choice(objects))

    watch = ((0x8019040C, 4), (0x80190458, 1), (0x80190460, 1), (0x80190478, 1))
    log = CallLog(state, 4096, watch=watch)
    result = rng.choice((0, 0, 0, 1, 0x100, 0x1FF, 0x80))
    log.replace(sym["func_801397d0"], 3, result)
    log.replace(sym["func_8013b7b8"], 0, 0)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the constant 0x17 that the code compares the objects' kinds with."""
    found = [i for i, w in enumerate(words) if w >> 26 in (0x09, 0x0D) and (w >> 21) & 31 == 0 and w & 0xFFFF == 0x17]
    if not found:
        raise ValueError("no load of the constant 0x17 found")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x16, "kind constant 0x17 changed to 0x16"


CONTRACT = Contract(setup, control)
