"""a0 is a character object, a1 is ignored, a2 is held at 0 (the contract's restriction).

Reads and writes are listed in the header comment of
func_801b21f8_slot04_0f.c. Choices made here:
  - the object is filled with random bytes; field_0b is 0 in half;
  - the 128 halfwords of the table are random per case, with the values
    0x8000, 0x7fff, 0 and 0xffff put into some entries, so that the negation
    of the most negative halfword and the sign handling are reached;
  - field_338 is random over the byte (its lowest bit is ignored);
  - field_332 and field_12 are random halfwords, sometimes chosen so that
    the sum overflows 16 bits;
  - a1 is a random 32-bit value; a2 is 0 in every case: the original reads
    its low half, which the C does not have (the header's stated difference).
"""
from contracts import Contract, Setup, fill


def setup(state, rng, sym):
    table = sym["data_801c5ae4_slot04_0f"]
    fill(state, table, 0x100, rng)
    for _ in range(4):
        state.w16(table + 2 * rng.randrange(128), rng.choice((0x8000, 0x7FFF, 0, 0xFFFF)))
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x0B, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    state.w16(obj + 0x332, rng.choice((rng.getrandbits(16), 0x7FFF, 0x8000, 0xFFFF, 0)))
    state.w16(obj + 0x12, rng.choice((rng.getrandbits(16), 0x7FFF, 0x8000, 0xFFFF, 0)))
    return Setup(args=(obj, rng.getrandbits(32), 0), returns_value=False)


def control(words):
    """Alter the `sra v0,a2,4` that makes field_4c: it shifts by 3."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0 and w & 0x3F == 3 and (w >> 6 & 31) == 4]
    if len(found) != 1:
        raise ValueError(f"expected one arithmetic shift by 4, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~(31 << 6)) | (3 << 6), "final shift 4 changed to 3"


CONTRACT = Contract(setup, control)
