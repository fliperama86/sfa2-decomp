"""Contract of func_801212c8 in the port's C (header comment of the .c file).

Choices of the setup:
  - the parameter is a 0x364-byte block of random bytes;
  - func_8001365c (1 argument) is a recorder that returns 0; the log watches the block
    at every call.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym) -> Setup:
    st = state.alloc(0x364)
    fill(state, st, 0x364, rng)
    log = CallLog(state, 256, watch=((st, 0x364 // 4),))
    log.replace(sym["func_8001365c"], 1, 0)
    return Setup(args=(st,), returns_value=False)


def control(words):
    """Alter the argument register at the call: the empty delay slot of `jal func_8001365c` becomes `addiu a0,a0,4`.

    The original code then hands the callee its parameter plus 4, which the recorder logs: this is the
    alteration that shows the test sees the value the callee gets.
    """
    jal = 0x0C000000 | ((0x8001365c >> 2) & 0x03FFFFFF)
    found = [i for i, w in enumerate(words) if w == jal]
    if len(found) != 1 or words[found[0] + 1] != 0:
        raise ValueError("expected one call of func_8001365c with an empty delay slot")
    return found[0] + 1, 0x24840004, "the callee gets a0 + 4"


CONTRACT = Contract(setup, control)
