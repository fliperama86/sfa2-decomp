"""Contract of func_8011db78 as code. Reads and writes are listed in the
header comment of func_8011db78.c. Choices made here:
  - object field_02 is 0 to 40, side is 0 or 1, the column (a1) is 0 to 3,
    the buffer selector is 0 or 1 (these keep the record buffer inside the
    free part of RAM and clear of the globals);
  - the frame distance d is 0 to 9 in nine cases of ten, else 10 or more;
  - the size entry and the header pointer are 0 in one case of ten each,
    the header count is 0 in one case of ten, otherwise 1 to 8;
  - the flag entry is 0 in half of the cases; then the entries before it
    equal the current ones in three cases of five, else the size entry or
    the header pointer differs;
  - the mode byte is 0 to 3 in nine cases of ten, otherwise random (4 or
    more store nothing); its upper byte is random;
  - the stream has x step, y step and a group count per group; the group
    count value is 0 to 5 in nine cases of ten, else 0xffff (a group of
    zero records); steps are random halfwords; the stream offset in the
    header is random in 0 to 0x40 (odd values included, the code clears
    bit 0);
  - field_0f is 0 in half of the cases; all globals read are random;
  - the record buffer cells that the cases write are filled with random
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

    size = 0 if rng.random() < 0.1 else rng.randrange(1, 0x10000)
    if rng.random() < 0.1:
        header = 0
    else:
        header = state.alloc(16)
        fill(state, header, 16, rng)
        count = 0 if rng.random() < 0.1 else rng.randrange(1, 9)
        state.w16(header + 0, count)
        offset = rng.randrange(0x41)
        state.w16(header + 8, offset | (0 if rng.random() < 0.8 else 0xFF80))
    flag = 0 if rng.random() < 0.5 else rng.randrange(1, 0x10000)

    if d < 10:
        b, c, ptr, f = (sym["data_80183d9c"] + 20 * idx, sym["data_80183dc4"] + 20 * idx,
                        sym["data_80183dec"] + 40 * idx, sym["data_80183d74"] + 20 * idx)
        state.w16(b + 2 * d, size)
        state.w32(ptr + 4 * d, header)
        state.w16(c + 2 * d, flag)
        state.w16(f + 2 * d, rng.randrange(4) | (rng.getrandbits(8) << 8)
                  if rng.random() < 0.9 else halfword(rng))
        if d >= 1:
            same = rng.random() < 0.6
            state.w16(b + 2 * (d - 1), size if same or rng.random() < 0.5 else halfword(rng))
            state.w32(ptr + 4 * (d - 1), header if same or rng.random() < 0.5
                      else rng.getrandbits(32))
            if not same:
                # exactly one of the two differs, or both
                pick = rng.randrange(3)
                if pick == 0:
                    state.w16(b + 2 * (d - 1), size)
                    state.w32(ptr + 4 * (d - 1), header ^ 0x40)
                elif pick == 1:
                    state.w16(b + 2 * (d - 1), size ^ 1)
                    state.w32(ptr + 4 * (d - 1), header)
        else:
            state.w16(b - 2, size if rng.random() < 0.5 else halfword(rng))
            state.w32(ptr - 4, header if rng.random() < 0.5 else rng.getrandbits(32))

    stream = state.alloc(2 * 3 * 140)
    for k in range(140):
        state.w16(stream + 6 * k, halfword(rng))
        state.w16(stream + 6 * k + 2, halfword(rng))
        state.w16(stream + 6 * k + 4, rng.randrange(6) if rng.random() < 0.9 else 0xFFFF)
    if header:
        offset = state.read(header + 8, 2)
        off = (offset[0] | offset[1] << 8) & 0xFFFE
        # off may reach 0xFFFE with the high bits set: keep the pointer in RAM
        field_98 = stream - off
    else:
        field_98 = stream

    call = state.alloc(16)
    fill(state, call, 16, rng)
    state.w16(call + 0, r_frame)
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    other = state.alloc(0xAC)
    fill(state, other, 0xAC, rng)
    base = sym["data_801987cc"] + (side * 320 + column * 80 + 640) * 16 + selector * 0x5000
    log = CallLog(state, 3000, watch=((obj, 0x394 // 4), (other, 0xAC // 4), (base, 4 * 20)))
    log.replace(sym["func_801250c0"], 2, call)
    log.replace(sym["func_8015bf70"], 3, 0)

    state.w8(obj + 0x02, idx)
    state.w8(obj + 0x0F, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    state.w8(obj + 0xA6, side)
    state.w32(obj + 0x98, field_98 & 0xFFFFFFFF)

    state.w16(sym["box_margin"], halfword(rng))
    state.w16(sym["data_801aa5ea"], halfword(rng))
    state.w16(sym["game_state"] + 0x92, halfword(rng))
    state.w32(sym["data_801a27d0"], selector)
    state.w32(sym["data_801987c8"], rng.getrandbits(32))
    fill(state, base, 16 * 20, rng)
    return Setup(args=(obj, column, frame, other), returns_value=False)


def control(words):
    """Move the first halfword store with offset 0 of the build by two bytes.
    It is the store of a record's y value (the build addresses the record
    from a pointer that lies 0xa bytes into it, so the store has offset 0);
    a case with a mode below 4 and one record or more reaches it."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0]
    if not found:
        raise ValueError("no halfword store with offset 0")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 2, "first halfword store of a y value moved by two bytes"


CONTRACT = Contract(setup, control)
