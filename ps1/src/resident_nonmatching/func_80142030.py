"""Contract of func_80142030 (header comment of the .c file lists reads and writes).

Choices of the setup:
  - the object and ref_other's object are separate 0x394-byte blocks of
    random bytes;
  - the vertical distance is, by case: small (0 to 0x90, either sign) in
    about 60 percent, exactly 0x8000 in 5 percent, random otherwise;
  - field_21e is, by case: in the grid (-0x20 to 0xF0, wrapping, so that
    columns 0 to 8 and the first columns below zero occur) in 70 percent,
    at the borders (0xFFF0, 0xFFEF, 0x00F0, 0x00F1) in 10 percent, random otherwise;
  - table_8017ac54's 40 cell bytes are rewritten with random indices below
    0x80, a quarter of them with bit 7 set; table_8017ac7c is left as the
    resident image has it (its 4-byte records are read by index below 0x80).
"""
from contracts import CallLog, Contract, Setup, fill, halfword


def setup(state, rng, sym) -> Setup:
    cells = sym["table_8017ac54"]
    for i in range(40):
        value = rng.randrange(0x80)
        if rng.random() < 0.25:
            value |= 0x80
        state.w8(cells + i, value)
    other = state.alloc(0x394)
    fill(state, other, 0x394, rng)
    state.w32(sym["ref_other"], other)
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    pick = rng.random()
    if pick < 0.6:
        dist = rng.randrange(-0x90, 0x91)
    elif pick < 0.65:
        dist = 0x8000
    else:
        dist = halfword(rng)
    other_y = int.from_bytes(state.read(other + 0x16, 2), "little")
    state.w16(obj + 0x16, other_y + dist)
    pick = rng.random()
    if pick < 0.7:
        x = rng.randrange(-0x20, 0xF1) & 0xFFFF
    elif pick < 0.8:
        x = rng.choice((0xFFF0, 0x00F0, 0x00F1, 0xFFEF))
    else:
        x = halfword(rng)
    state.w16(obj + 0x21E, x)
    return Setup(args=(obj,), returns_value=True)


def control(words):
    """Alter the store to field_219: its offset moves by one byte."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0x219]
    if len(found) != 1:
        raise ValueError(f"expected one store to field_219, found {len(found)}")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0x21A, "store of field_219 moved by one byte"


CONTRACT = Contract(setup, control)
