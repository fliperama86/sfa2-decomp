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
    """Alter the frame of the function: `addiu sp,sp,-0x18` becomes -0x20."""
    found = [i for i, w in enumerate(words) if w == 0x27BDFFE8]
    if len(found) != 1:
        raise ValueError(f"expected one frame setup, found {len(found)}")
    i = found[0]
    return i, 0x27BDFFE0, "frame of 0x20 bytes, restored as 0x18"


CONTRACT = Contract(setup, control)
