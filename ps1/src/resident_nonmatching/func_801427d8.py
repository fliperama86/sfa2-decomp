"""Contract of func_801427d8 (header comment of the .c file lists reads and writes).

Choices of the setup:
  - game_state.field_354 is one of the ten listed modes in four cases of five, and a random
    16-bit value otherwise;
  - object->field_c6 is random 16-bit in half the cases and near one of the
    thresholds (0x60, 0x90, plus or minus 2) otherwise; negative values
    occur through the random half;
  - the object and the block at its field_40 are 0x394-byte blocks of random
    bytes (field_7e and field_66 are random, so that the callee's branches on
    them both run; read from the original's listing, not tested);
  - ref_other, which func_80142c04 writes (read from the original's
    listing, not tested), starts as a random word;
  - func_80142c04 is not replaced.
"""
from contracts import CallLog, Contract, Setup, fill, halfword

MODES = (1, 2, 0x94, 0x68, 0x90, 0x84, 0x14, 0x60, 0x48, 0x28)


def setup(state, rng, sym) -> Setup:
    mode = rng.choice(MODES) if rng.random() < 0.8 else halfword(rng)
    state.w16(sym["game_state"] + 0x354, mode)
    target = state.alloc(0x394)
    fill(state, target, 0x394, rng)
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w32(obj + 0x40, target)
    state.w32(sym["ref_other"], rng.getrandbits(32))
    if rng.random() < 0.5:
        c6 = halfword(rng)
    else:
        c6 = rng.choice((0x60, 0x90)) + rng.randrange(-2, 3)
    state.w16(obj + 0xC6, c6)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the byte store to field_255: its offset moves by one byte."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0x255]
    if len(found) != 1:
        raise ValueError(f"expected one store to field_255, found {len(found)}")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0x256, "store of field_255 moved by one byte"


CONTRACT = Contract(setup, control)
