"""Contract of func_8014362c (header comment of the .c file lists reads and writes).

Choices of the setup:
  - object and other are separate 0x394-byte blocks of random bytes;
  - other->unknown_148 points at a block of four 6-byte box records of
    random bytes (the function uses index 1);
  - object->field_158 is 1 in a third of the cases, 0 in a third and a
    random byte otherwise, so that the mirrored and plain arms are both
    taken;
  - pos_x of both objects is random 16-bit, so the sign of the difference
    takes both values.
"""
from contracts import CallLog, Contract, Setup, fill, halfword


def setup(state, rng, sym) -> Setup:
    boxes = state.alloc(4 * 6)
    fill(state, boxes, 4 * 6, rng)
    other = state.alloc(0x394)
    fill(state, other, 0x394, rng)
    state.w32(other + 0x148, boxes)
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x158, rng.choice((0, 1, 1, rng.randrange(256))))
    # pos_x close to the box edge half the time, so the difference is small
    if rng.random() < 0.5:
        state.w16(obj + 0x12, int.from_bytes(state.read(other + 0x12, 2), "little") + rng.randrange(-300, 300))
    return Setup(args=(obj, other), returns_value=False)


def control(words):
    """Alter the store to field_21e: its offset moves by two bytes."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0x21E]
    if len(found) != 1:
        raise ValueError(f"expected one store to field_21e, found {len(found)}")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0x220, "store of field_21e moved by two bytes"


CONTRACT = Contract(setup, control)
