"""Contract of func_80012c34_slot12 (see the header of the .c file).

Choices of the setup:
  - the object is 0x394 bytes of random content; field_04 is any byte
    (the increment wraps), so the whole object is random;
  - box_margin is a random halfword (the x position is its sum with 0xc0);
  - both callees are recorders taking 3 and 1 arguments; the log watches the
    whole object (229 words) at each of the two calls.
"""
from contracts import CallLog, Contract, Setup, fill, halfword

OBJECT_SIZE = 0x394


def setup(state, rng, sym):
    obj = state.alloc(OBJECT_SIZE)
    fill(state, obj, OBJECT_SIZE, rng)
    state.w16(sym["box_margin"], halfword(rng))
    log = CallLog(state, 4 * 240 + 16, watch=((obj, OBJECT_SIZE // 4),))
    log.replace(sym["func_80130768"], 3)
    log.replace(sym["func_80131094"], 1)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of the y position: 0x78 becomes 0x79 (`ori v0,zero,0x78`)."""
    found = [i for i, w in enumerate(words) if w == 0x34020078]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x78, found {len(found)}")
    return found[0], 0x34020079, "y position constant 0x78 changed to 0x79"


CONTRACT = Contract(setup, control)
