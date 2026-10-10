"""Contract of func_800205b4_slot28 in the port's C (header comment of the .c file).

Choices of the setup:
  - the object is a 0x394-byte block of random bytes; its halfword pos_y
    (offset 0x16) is 0xd0 or more in half of the cases (the arm that raises
    field_04) and below it in the other half;
  - func_80020608_slot28 (the leaf that runs before the call) is the image's
    own code and runs as it is; it reads and writes only words of the object
    (offsets 0x10, 0x14, 0x4c, 0x50, 0x54, 0x58), which the block holds;
  - func_80131094 (1 argument) is a recorder that returns 0; the log watches
    the object whole at every call.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym) -> Setup:
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    pos_y = rng.randrange(0xD0, 0x8000) if rng.random() < 0.5 else rng.randrange(-0x8000, 0xD0)
    state.w16(obj + 0x16, pos_y & 0xFFFF)
    log = CallLog(state, 256, watch=((obj, 0x394 // 4),))
    log.replace(sym["func_80131094"], 1, 0)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the argument register at the call: `move a0,s0` in the delay slot of `jal func_80131094` becomes `addiu a0,s0,4`.

    The build of this override has no empty delay slot there (the compiler puts the move of the argument into it),
    so the control alters that word instead of an empty one. It alters the build of this C (the tool hands it the
    build's code words, not the original's). The altered build then hands the callee the object plus 4, which the
    recorder logs, while the original code hands it the value the console's code leaves in a0: this is the
    alteration that shows the test sees the value the callee gets.
    """
    jal = 0x0C000000 | ((0x80131094 >> 2) & 0x03FFFFFF)
    found = [i for i, w in enumerate(words) if w == jal]
    if len(found) != 1 or words[found[0] + 1] != 0x02002021:
        raise ValueError("expected one call of func_80131094 with `move a0,s0` in its delay slot")
    return found[0] + 1, 0x26040004, "the callee gets its argument + 4"


CONTRACT = Contract(setup, control)
