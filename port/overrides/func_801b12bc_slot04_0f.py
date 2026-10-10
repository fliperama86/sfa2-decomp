"""Contract of func_801b12bc_slot04_0f in the port's C (header comment of the .c file).

Choices of the setup:
  - the object is a 0x394-byte block of random bytes; its byte field_260
    (offset 0x260) is 0 in four cases of five and otherwise 1 to 255 (the
    early return);
  - func_80141c4c (1 argument) is a recorder; it returns 0 or 1, chosen per
    case, so that both arms after the call are reached; the log watches the
    object whole at every call.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym) -> Setup:
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x260, 0 if rng.random() < 0.8 else rng.randrange(1, 256))
    log = CallLog(state, 256, watch=((obj, 0x394 // 4),))
    log.replace(sym["func_80141c4c"], 1, rng.randrange(2))
    return Setup(args=(obj,), returns_value=True)


def control(words):
    """Alter the argument register at the call: `move a0,s0` in the delay slot of `jal func_80141c4c` becomes `addiu a0,s0,4`.

    The build of this override has no empty delay slot there (the compiler puts the move of the argument into it),
    so the control alters that word instead of an empty one. It alters the build of this C (the tool hands it the
    build's code words, not the original's). The altered build then hands the callee the object plus 4, which the
    recorder logs, while the original code hands it the value the console's code leaves in a0: this is the
    alteration that shows the test sees the value the callee gets.
    """
    jal = 0x0C000000 | ((0x80141C4C >> 2) & 0x03FFFFFF)
    found = [i for i, w in enumerate(words) if w == jal]
    if len(found) != 1 or words[found[0] + 1] != 0x02002021:
        raise ValueError("expected one call of func_80141c4c with `move a0,s0` in its delay slot")
    return found[0] + 1, 0x26040004, "the callee gets its argument + 4"


CONTRACT = Contract(setup, control)
