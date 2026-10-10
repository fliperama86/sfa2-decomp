"""Contract of func_8012f9b0: a0 = object, no return value.

Choices of the setup (reads and writes are listed in func_8012f9b0.c):
  - the unit table units_2c20 (16 units of 0xc0 bytes) is filled with random
    bytes; then for each unit field_00 is 0 in one case of five, bit 0x80 of
    field_74 is set in one case of five, field_65 equals the object's in one
    case of five (else random), and field_12 is set near the object's pos_x
    (within 0x60, either side, so that the boundary 0x50 is crossed) with a
    probability per unit that is chosen per case among 0, 0.05, 0.25 and
    0.5, else it stays random;
  - the object is a random block of 0x394 bytes; field_150 has bit 0x2000
    set in one case of two; field_d8 is 0 in one case of three;
  - ref_other.p starts random (the function sets it first);
  - func_8012faf8 is a recorder with one argument (the object), returning a random value
    (the function stores it in a byte); it copies the object, ref_other and
    data_80186000 at the call.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    units = sym["units_2c20"]
    fill(state, units, 16 * 0xC0, rng)
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    pos_x = rng.getrandbits(16)
    state.w16(obj + 0x12, pos_x)
    side = rng.getrandbits(8)
    state.w8(obj + 0x65, side)
    state.w16(obj + 0x150, rng.getrandbits(16) | 0x2000 if rng.random() < 0.5 else rng.getrandbits(16) & ~0x2000)
    state.w8(obj + 0xD8, 0 if rng.random() < 1 / 3 else rng.randrange(1, 256))
    # Fewer hits when the whole table qualifies often: scale by a per-case rate.
    near_rate = rng.choice((0.0, 0.05, 0.25, 0.5))
    for i in range(16):
        u = units + 0xC0 * i
        state.w8(u + 0x00, 0 if rng.random() < 0.2 else rng.randrange(1, 256))
        state.w8(u + 0x74, (rng.getrandbits(8) | 0x80) if rng.random() < 0.2 else rng.getrandbits(8) & 0x7F)
        state.w8(u + 0x65, side if rng.random() < 0.2 else rng.getrandbits(8))
        if rng.random() < near_rate:
            state.w16(u + 0x12, (pos_x + rng.randrange(-0x60, 0x61)) & 0xFFFF)
    state.w32(sym["ref_other"], rng.getrandbits(32))
    state.w8(sym["data_80186000"], rng.getrandbits(8))
    log = CallLog(state, watch=((obj, 0x394 // 4), (sym["ref_other"], 1), (sym["data_80186000"], 1)))
    log.replace(sym["func_8012faf8"], 1, rng.getrandbits(32))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Change the stride 0xc0 of the cursor to 0xc4."""
    found = [i for i, w in enumerate(words) if w >> 26 == 9 and w & 0xFFFF == 0xC0]
    if len(found) != 1:
        raise ValueError(f"expected one addiu of 0xc0, found {len(found)}")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0xC4, "cursor stride 0xc0 changed to 0xc4"


CONTRACT = Contract(setup, control)
