"""Contract of func_8001188c_slot27: copy a strip into two rows of a table.

Choices of the setup (reads and writes are in the header of the .c file):
  - the object has an `other` object (a block of 0x100 random bytes);
  - its field_d4 is 0 to 3 and its kind 0 to 7 in nine cases of ten (the
    source strip then lies in the 0x2000 random bytes the setup writes at
    data_800e2000), otherwise both are random bytes (the strip is then
    whatever the memory holds there, the same in both runs);
  - its side is 0 to 9, 9 in one case of three (the exact fit: the strip
    written into row 5 ends at the last halfword of the table); a larger
    side would write past the end of the table, which the original does not
    check, and is excluded;
  - the six rows of data_801a27e4_rows hold random bytes, so that the
    halfwords the function does not write are tested too;
  - the recorder of func_80137220 watches all six rows.
"""
from contracts import CallLog, Contract, Setup, fill

ROWS = 6 * 0x200 * 2


def setup(state, rng, sym):
    fill(state, sym["data_800e2000"], 0x2000, rng)
    fill(state, sym["data_801a27e4_rows"], ROWS, rng)

    other = state.alloc(0x100)
    fill(state, other, 0x100, rng)
    near = rng.random() < 0.9
    state.w8(other + 0xD4, rng.randrange(4) if near else rng.randrange(256))
    state.w8(other + 0xA7, rng.randrange(8) if near else rng.randrange(256))
    # side 0 to 9: the original does not check it; the strip written into
    # row 5 ends at halfword 0x20 + side * 0x30 + 0x2f, which is the last
    # halfword of the row (0x1ff) when side is 9 (the exact fit, one case in
    # three); a larger side leaves the table and is not part of the contract.
    side = 9 if rng.random() < 0.33 else rng.randrange(10)
    assert 0x20 + side * 0x30 + 0x2F < 0x200  # the strip ends inside row 5
    state.w8(other + 0xA6, side)

    obj = state.alloc(0x100)
    fill(state, obj, 0x100, rng)
    state.w32(obj + 0x40, other)

    log = CallLog(state, 4 + ROWS // 4 + 16, watch=((sym["data_801a27e4_rows"], ROWS // 4),))
    log.replace(sym["func_80137220"], 2)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the halfword store into row 5: it lands one halfword later.

    Every case runs the loop, so the last `sh` of the loop body is reached
    at every call; its offset 0 becomes 2.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0]
    if not found:
        raise ValueError("expected a halfword store with offset 0")
    index = found[-1]
    return index, words[index] | 2, "last halfword store of the loop moved by two bytes"


CONTRACT = Contract(setup, control)

