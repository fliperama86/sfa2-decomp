"""Contract of func_801b2e8c_slot04_0e as code.

Reads and writes are listed in the header comment of
func_801b2e8c_slot04_0e.c. Choices made here:
  - the object is a 0x394-byte block of random bytes;
  - the walk of the heights is steered: after the motion step
    (func_801b4914_slot04_0e subtracts field_50 from the word at 0x14,
    which holds field_14 and pos_y) pos_y is field_70 plus a chosen
    distance, taken from 1, 0, -0x2f, -0x30, -0x31 and near values, so that
    each of the three arms is reached often and the borders are tried, with
    field_70 sometimes near the bottom of the 16-bit range (the wrap of the
    subtraction); the word at 0x14 is set so that it holds that pos_y after
    the step;
  - field_164 equals the selector of the facing in half the cases, else a
    small or random value; field_0b and field_49 are 0 in half the cases;
  - every recorder copies the whole object (0x394 bytes) into the log at
    every call, so the order of the function's stores against its calls is
    compared; no recorded callee gets a pointer to memory filled for the call;
  - func_80130efc, func_801307e0 and func_801209c4 are recorders returning
    0; func_801b4914_slot04_0e and func_801b2f58_slot04_0e run as original.
"""

from contracts import CallLog, Contract, Setup

Q = 0xFFFFFFFF


def setup(state, rng, sym) -> Setup:
    obj = state.alloc(0x394)
    log = CallLog(state, 2048, watch=((obj, 0x394 // 4),))
    log.replace(sym["func_80130efc"], 1)
    log.replace(sym["func_801307e0"], 2)
    log.replace(sym["func_801209c4"], 1)

    state.write(obj, bytes(rng.getrandbits(8) for _ in range(0x394)))
    height = rng.choice((rng.randrange(-0x8000, 0x8000), rng.randrange(-0x8000, -0x7FC0)))
    distance = rng.choice((1, 0, -0x2F, -0x30, -0x31, rng.randrange(1, 40), rng.randrange(-0x60, 0),
                           rng.randrange(-0x8000, 0x8000)))
    pos_y = (height + distance) & 0xFFFF
    field_14 = rng.getrandbits(16)
    step = rng.getrandbits(32)
    state.w16(obj + 0x70, height & 0xFFFF)
    state.w32(obj + 0x50, step)
    state.w32(obj + 0x14, ((pos_y << 16 | field_14) + step) & Q)
    state.w8(obj + 0x0B, rng.choice((0, rng.randrange(256))))
    state.w8(obj + 0x49, rng.choice((0, rng.randrange(256))))
    facing = 2 if state.read(obj + 0x0B, 1)[0] == 0 else 1
    state.w8(obj + 0x164, rng.choice((facing, facing, rng.randrange(4), rng.randrange(256))))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the clear of field_50: the store of zero at offset 0x50 moves to 0x54."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x2B and (w >> 16) & 31 == 0 and w & 0xFFFF == 0x50]
    if len(found) != 1:
        raise ValueError(f"expected one store of zero to field_50, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x54, "store of zero to field_50 moved to field_54"


CONTRACT = Contract(setup, control)
