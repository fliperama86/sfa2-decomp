"""Contract of func_80139928 (see the header comment of func_80139928.c).

Choices of the setup:
  - the two objects, b's box and the cursor box are separate blocks of
    random bytes; game_state.cursor points at the cursor box;
  - field_0b of each object is 0 in half the cases and random otherwise (the
    mirror of the box x);
  - the box and cursor values are random halfwords in a third of the cases,
    and small values (-40..40 for positions and offsets, 0..40 for sizes) in
    the others, so that the apart/overlapping decision falls on both sides
    in both axes (random 16-bit values nearly always say "apart" in x);
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
    for obj in (a, b):
        state.w8(obj + 0x0B, 0 if rng.random() < 0.5 else rng.randrange(256))
        if near:
            state.w16(obj + 0x12, _small(rng))
            state.w16(obj + 0x16, _small(rng))
    box = state.alloc(0x20)
    cursor = state.alloc(8)
    fill(box, 0x20)
    fill(cursor, 8)
    if near:
        for offset in (0, 2):
            state.w16(box + offset, _small(rng))
            state.w16(cursor + offset, _small(rng))
        for offset in (4, 6):
            state.w16(box + offset, rng.randrange(0, 41))
            state.w16(cursor + offset, rng.randrange(0, 41))
    state.w32(sym["game_state"] + 0x304, cursor)
    fill(sym["data_80188ed0"], 0x4C)
    return Setup(args=(a, b, box), returns_value=True)


def control(words):
    """Alter the compare of the y test: slt of the last result becomes sltu."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0 and w & 0x3F == 0x2A]
    if len(found) < 2:
        raise ValueError("expected two slt instructions")
    index = found[-1]
    return index, (words[index] & ~0x3F) | 0x2B, "last slt changed to sltu"


CONTRACT = Contract(setup, control)
