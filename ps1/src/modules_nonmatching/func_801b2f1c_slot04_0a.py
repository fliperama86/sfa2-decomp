"""Contract of func_801b2f1c_slot04_0a: a character state that starts a jump.

Reads and writes are listed in the header comment of
func_801b2f1c_slot04_0a.c. Choices made here:
  - the first helper returns non-zero (random 16-bit value, with random
    bits above 16 as well) in one case of four;
  - field_50 is above 0x50000 in one case of four, else random up to it,
    with the values around the limit (0x50000, 0x50001, 0x4ffff) often;
  - the halfword at 0x134 has each of the bits 0x80, 0x10, 4, 1 set with
    probability one half, other bits random;
  - the opponent's position x and the object's are random halfwords, and
    equal in one case of five;
  - the object and the opponent are blocks of 0x394 bytes of random content;
  - every callee is a recorder; the log watches both blocks whole.
"""

from contracts import CallLog, Contract, Setup, fill, halfword  # noqa: F401


def setup(state, rng, sym) -> Setup:
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    other = state.alloc(0x394)
    fill(state, other, 0x394, rng)
    state.w32(obj + 0x40, other)

    roll = rng.random()
    if roll < 0.25:
        field_50 = rng.randrange(0x50001, 0x100000)
    elif roll < 0.4:
        field_50 = rng.choice((0x50000, 0x4FFFF, 0x50001))
    else:
        field_50 = rng.randrange(-0x100000, 0x50000) & 0xFFFFFFFF
    state.w32(obj + 0x50, field_50)

    bits = 0
    for bit in (0x80, 0x10, 4, 1):
        if rng.random() < 0.5:
            bits |= bit
    state.w16(obj + 0x134, (halfword(rng) & ~0x95) | bits)

    x = halfword(rng)
    state.w16(obj + 0x12, x)
    state.w16(other + 0x12, x if rng.random() < 0.2 else halfword(rng))

    result = rng.getrandbits(32) | 1 if rng.random() < 0.25 else 0
    log = CallLog(state, 3200, watch=((obj, 0x394 // 4), (other, 0x394 // 4)))
    log.replace(sym["func_801b1e04_slot04_0a"], 1, result)
    log.replace(sym["func_801b30a0_slot04_0a"], 1, 0)
    log.replace(sym["func_801307e0"], 2, 0, masks={1: 0xFFFF})
    log.replace(sym["func_801204f4"], 3, 0, masks={1: 0xFF, 2: 0xFF})
    log.replace(sym["func_80141f28"], 2, 0)
    log.replace(sym["func_80138ae8"], 2, 0)
    log.replace(sym["func_80130efc"], 1, 0)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of the constant 0x38000 into field_50: the build stores another value.

    The upper half 3 is loaded once with a `lui` of 3, in the arm that
    starts the jump.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0F and w & 0xFFFF == 3]
    if len(found) != 1:
        raise ValueError(f"expected one lui of 3, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 4, "field_50 constant 0x38000 changed to 0x48000"


CONTRACT = Contract(setup, control)
