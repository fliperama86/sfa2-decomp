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
    """Alter the mask of the argument of func_8014efa8: 0x7f becomes 0x3f."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0C and w & 0xFFFF == 0x7F]
    if len(found) != 1:
        raise ValueError(f"expected one andi with 0x7f, found {len(found)}")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0x3F, "mask 0x7f becomes 0x3f"


CONTRACT = Contract(setup, control)
