"""Contract of func_80079144_slot2b (reads and writes: see the header of the .c).

Choices of the setup:
  - the object, parent, target and the parent's frame record are random
    blocks; the parent's side is 0 or non-zero with equal odds;
  - the target's kind and the frame's field_0b are any byte; on each side
    the scratchpad word points at an array of 256 pointers, whose entry for
    kind k points at one of four random record blocks of 256 records of 6
    bytes (k modulo 4), so that every index is valid;
  - the target's field_0b is 0 in half of the cases (the sum is negated
    otherwise); every other field is random.
"""

from contracts import Contract, Setup


def block(state, rng, size):
    address = state.alloc(size)
    state.write(address, rng.randbytes(size))
    return address


def setup(state, rng, sym) -> Setup:
    obj = block(state, rng, 0x394)
    parent = block(state, rng, 0x394)
    target = block(state, rng, 0x394)
    frame = block(state, rng, 0x10)
    state.w32(obj + 0x3C, parent)
    state.w32(parent + 0x40, target)
    state.w32(parent + 0x88, frame)
    state.w8(parent + 0xA6, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    state.w8(target + 0x0B, 0 if rng.random() < 0.5 else rng.randrange(1, 256))

    for word in ("boxes_74_left", "boxes_124_right"):
        records = [block(state, rng, 256 * 6) for _ in range(4)]
        array = state.alloc(256 * 4)
        for kind in range(256):
            state.w32(array + 4 * kind, records[kind % 4])
        state.w32(sym[word], array)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the negation: `negu a2,v1` (subu a2,zero,v1) becomes `addu a2,v1,v1`."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0 and w & 0x3F == 0x23 and (w >> 21) & 31 == 0]
    if len(found) != 1:
        raise ValueError(f"expected one negation, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0x3F & ~(31 << 21)) | 0x21 | ((words[index] >> 16 & 31) << 21), "negation turned into a doubling"


CONTRACT = Contract(setup, control)
