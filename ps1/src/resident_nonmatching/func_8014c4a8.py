"""Contract of func_8014c4a8 (header comment of the .c file lists reads and writes).

Choices of the setup:
  - the object, its partner and the object ref_second points at are separate
    0x394-byte blocks of random bytes;
  - the frame record is a block of random bytes whose first byte (the index)
    is 0 to 7; the box table has 8 records of random bytes;
  - the partner's field_0b is 0 in a third of the cases, random otherwise
    (so both arms are taken);
  - half the time the object's pos_x is set near the partner's, so that the
    difference is small as well as random;
  - data_80189464 starts as random bytes.
"""
from contracts import CallLog, Contract, Setup, fill, halfword


def setup(state, rng, sym) -> Setup:
    boxes = state.alloc(8 * 6)
    fill(state, boxes, 8 * 6, rng)
    frame = state.alloc(16)
    fill(state, frame, 16, rng)
    state.w8(frame, rng.randrange(8))
    ref = state.alloc(0x394)
    fill(state, ref, 0x394, rng)
    state.w32(ref + 0x88, frame)
    state.w32(ref + 0x6c, boxes)
    state.w32(sym["ref_second"], ref)
    other = state.alloc(0x394)
    fill(state, other, 0x394, rng)
    state.w8(other + 0x0B, rng.choice((0, rng.randrange(256))))
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w32(obj + 0x40, other)
    if rng.random() < 0.5:
        state.w16(obj + 0x12, int.from_bytes(state.read(other + 0x12, 2), "little") + rng.randrange(-300, 300))
    state.w16(sym["data_80189464"], halfword(rng))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the halfword store to data_80189464: its offset moves by two bytes."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0x9464]
    if len(found) != 1:
        raise ValueError(f"expected one store to data_80189464, found {len(found)}")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0x9466, "store of data_80189464 moved by two bytes"


CONTRACT = Contract(setup, control)
