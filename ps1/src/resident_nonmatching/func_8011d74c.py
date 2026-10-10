"""Contract of func_8011d74c as code. Reads and writes are listed in the
header comment of func_8011d74c.c. Choices made here:
  - object field_02 is 0 to 40, side 0 or 1, the column (a1) 0 to 3, the
    buffer selector 0 or 1;
  - the frame distance d is 0 to 9 in nine cases of ten, else 10 or more;
  - the size entry (1 to 110), the header pointer and the header count (1 to
    40; 16, 17, 31 or 32 in a quarter of the cases, so that the row of 16
    ends exactly or leaves a partial row) are 0 in one case of ten each;
  - the flag entry is 0 in half of the cases; then the entries before it
    equal the current ones in three cases of five, else one or both differ;
  - the cache cells hold the three values the function would store in a
    quarter of the cases (it returns at once), otherwise each of the three
    equals the new value in half of the cases and is random otherwise;
  - the row table (16 bytes per size value) and the chain table hold
    random values, the chain entries are 0 to 110;
  - the recorders return random 32-bit values;
  - the cell buffer bytes the function may write are filled with random
    bytes, so that the bytes left alone are tested.
"""

from contracts import CallLog, Contract, Setup, fill, halfword


def setup(state, rng, sym) -> Setup:
    idx = rng.randrange(41)
    side = rng.randrange(2)
    column = rng.randrange(4)
    selector = rng.randrange(2)
    frame = halfword(rng)

    d = rng.randrange(10) if rng.random() < 0.9 else rng.randrange(10, 80)
    r_frame = halfword(rng)
    state.w16(sym["data_801900f8"] + 2 * idx, (d - 1 + r_frame) & 0xFFFF)

    size = 0 if rng.random() < 0.1 else rng.randrange(1, 111)
    if rng.random() < 0.1:
        header = 0
    else:
        header = state.alloc(16)
        fill(state, header, 16, rng)
        if rng.random() < 0.1:
            count = 0
        elif rng.random() < 0.25:
            count = rng.choice((16, 17, 31, 32))
        else:
            count = rng.randrange(1, 41)
        state.w16(header, count)
    flag = 0 if rng.random() < 0.5 else rng.randrange(1, 0x10000)

    if d < 10:
        b, c, ptr = (sym["data_80183d9c"] + 20 * idx, sym["data_80183dc4"] + 20 * idx,
                     sym["data_80183dec"] + 40 * idx)
        state.w16(b + 2 * d, size)
        state.w32(ptr + 4 * d, header)
        state.w16(c + 2 * d, flag)
        if d >= 1:
            pick = rng.randrange(5)
            state.w16(b + 2 * (d - 1), size ^ (1 if pick in (1, 3) else 0))
            state.w32(ptr + 4 * (d - 1), header ^ (0x40 if pick in (2, 3) else 0))
        else:
            state.w16(b - 2, size if rng.random() < 0.5 else halfword(rng))
            state.w32(ptr - 4, header if rng.random() < 0.5 else rng.getrandbits(32))

    other = state.alloc(0xAC)
    fill(state, other, 0xAC, rng)
    byte = rng.randrange(256)
    state.w8(other + 0x0D, byte)

    ptr_cell = sym["data_80185c84"] + 32 * side + 8 * column + 4 * selector
    size_cell = sym["data_80185cc4"] + 16 * side + 4 * column + 2 * selector
    byte_cell = sym["data_80185ce4"] + 8 * side + 2 * column + selector
    same = rng.random() < 0.25
    state.w32(ptr_cell, header if same or rng.random() < 0.5 else rng.getrandbits(32))
    state.w16(size_cell, size if same or rng.random() < 0.5 else halfword(rng))
    state.w8(byte_cell, byte if same or rng.random() < 0.5 else rng.randrange(256))

    fill(state, sym["data_80185504"], 111 * 16, rng)
    for k in range(111):
        state.w16(sym["data_80183c90"] + 2 * k, rng.randrange(111))

    call = state.alloc(16)
    fill(state, call, 16, rng)
    state.w16(call + 0, r_frame)
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    base = sym["data_801987cc"] + (side * 320 + column * 80 + 640) * 16 + selector * 0x5000
    watch = ((obj, 0x394 // 4), (other, 0xAC // 4), (sym["data_80185c84"], 16),
             (sym["data_80185cc4"], 8), (sym["data_80185ce4"], 4), (base, 4 * 41))
    log = CallLog(state, 60000, watch=watch)
    log.replace(sym["func_801250c0"], 2, call)
    log.replace(sym["func_80158150"], 2, 0)
    log.replace(sym["func_8015bdd4"], 2, rng.getrandbits(32))
    log.replace(sym["func_8015bf34"], 2, 0, pointees={1: 4})
    log.replace(sym["func_8015bd0c"], 4, rng.getrandbits(32))
    log.replace(sym["func_80158a2c"], 5, 0)

    state.w8(obj + 0x02, idx)
    state.w8(obj + 0xA6, side)
    state.w32(sym["data_801a27d0"], selector)
    fill(state, base, 16 * 41, rng)
    return Setup(args=(obj, column, frame, other), returns_value=False)


def control(words):
    """Move the byte store of the cell's position byte (`sb` with offset -1,
    the build addresses the cell from a pointer 0xe bytes into it, so this is
    the store at 0xd) by one byte, to offset -2. Every call that gets past the
    cache test and has one cell or more stores it."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0xFFFF]
    if len(found) != 1:
        raise ValueError(f"expected one byte store at offset -1, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0xFFFE, "cell position byte stored one byte lower"


CONTRACT = Contract(setup, control)
