"""Contract of func_801397d0 (see the header comment of func_801397d0.c).

Choices of the setup:
  - object a has a frame record whose first byte (active) is 0..3 and a
    table of four 0x20-byte boxes; object b, box c, the box at ref_first.p
    and the objects are separate blocks of random bytes;
  - field_08 of each object is 0 in half the cases and a random non-zero
    byte otherwise, so that the three sources of the second box are taken;
  - field_0b of each object is 0 in half the cases and random otherwise;
  - in two cases of three the positions and box fields are small values
    (positions and offsets -40..40, sizes 0..40, as halfwords or bytes
    where the code reads bytes), so that the apart/overlapping decision
    falls on both sides in both axes; in the others they are random;
  - data_80188ed0 is filled with random bytes beforehand.
"""

from contracts import Contract, Setup


def _small(rng):
    return rng.randrange(-40, 41) & 0xFFFF


def setup(state, rng, sym):
    def fill(address, size):
        state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))

    near = rng.random() < 0.67
    a = state.alloc(0x394)
    b = state.alloc(0x394)
    fill(a, 0x394)
    fill(b, 0x394)
    zero = {}
    for obj in (a, b):
        zero[obj] = rng.random() < 0.5
        state.w8(obj + 0x08, 0 if zero[obj] else rng.randrange(1, 256))
        state.w8(obj + 0x0B, 0 if rng.random() < 0.5 else rng.randrange(256))
        if near:
            state.w16(obj + 0x12, _small(rng))
            state.w16(obj + 0x16, _small(rng))

    def box(address, byte_sizes):
        fill(address, 0x20)
        if near:
            state.w16(address + 0, _small(rng))
            state.w16(address + 2, _small(rng))
            if byte_sizes:
                state.w8(address + 4, rng.randrange(0, 41))
                state.w8(address + 5, rng.randrange(0, 41))
            else:
                state.w16(address + 4, rng.randrange(0, 41))
                state.w16(address + 6, rng.randrange(0, 41))

    frame = state.alloc(0x10)
    fill(frame, 0x10)
    state.w8(frame + 0, rng.randrange(4))
    state.w32(a + 0x88, frame)
    table = state.alloc(4 * 0x20)
    for index in range(4):
        box(table + 0x20 * index, False)
    state.w32(a + 0x144, table)

    other = state.alloc(0x20)
    box(other, not zero[a] and not zero[b])  # bytes for width and height only in the third arm
    state.w32(sym["ref_first"], other)

    c = state.alloc(8)
    fill(c, 8)
    if near:
        state.w16(c + 0, _small(rng))
        state.w16(c + 2, _small(rng))
        state.w8(c + 4, rng.randrange(0, 41))
        state.w8(c + 5, rng.randrange(0, 41))
    fill(sym["data_80188ed0"], 0x4C)
    return Setup(args=(a, b, c), returns_value=True)


def control(words):
    """Alter the compare of the y test: the last slt becomes sltu."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0 and w & 0x3F == 0x2A]
    if len(found) < 2:
        raise ValueError("expected two slt instructions")
    index = found[-1]
    return index, (words[index] & ~0x3F) | 0x2B, "last slt changed to sltu"


CONTRACT = Contract(setup, control)
