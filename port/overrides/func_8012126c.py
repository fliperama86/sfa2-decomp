"""Contract of func_8012126c in the port's C (header comment of the .c file).

Choices of the setup:
  - the state is a 0x364-byte block of random bytes (field_70 is one of
    them);
  - data_8018f5a0 points at a 0x60-byte block of random bytes; its halfword
    at 0x4c is 0 in half of the cases (first arm), else 1 to 65535 (second
    arm);
  - the log watches the state and the record whole at every call;
  - func_8014efa8 (1 argument) and func_80013834 (1 argument) are
    recorders that return 0.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym) -> Setup:
    st = state.alloc(0x364)
    fill(state, st, 0x364, rng)
    hud = state.alloc(0x60)
    fill(state, hud, 0x60, rng)
    state.w16(hud + 0x4C, 0 if rng.random() < 0.5 else rng.randrange(1, 0x10000))
    state.w32(sym["data_8018f5a0"], hud)
    log = CallLog(state, 256, watch=((st, 0x364 // 4), (hud, 0x60 // 4)))
    log.replace(sym["func_8014efa8"], 1, 0)
    log.replace(sym["func_80013834"], 1, 0)
    return Setup(args=(st,), returns_value=False)


def control(words):
    """Alter the argument register at the call: the empty delay slot of `jal func_80013834` becomes `addiu a0,a0,4`.

    The control alters the build of this C (the tool hands it the build's code words, not the original's). The
    altered build then hands the callee its parameter plus 4, which the recorder logs, while the original code
    hands it the parameter: this is the alteration that shows the test sees the value the callee gets.
    """
    jal = 0x0C000000 | ((0x80013834 >> 2) & 0x03FFFFFF)
    found = [i for i, w in enumerate(words) if w == jal]
    if len(found) != 1 or words[found[0] + 1] != 0:
        raise ValueError("expected one call of func_80013834 with an empty delay slot")
    return found[0] + 1, 0x24840004, "the callee gets a0 + 4"


CONTRACT = Contract(setup, control)
