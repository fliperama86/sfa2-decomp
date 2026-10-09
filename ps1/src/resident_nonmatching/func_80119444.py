"""Contract of func_80119444: no argument, no result.

Choices made here:
  - the counter data_801ac310 is random, with 0xffff and 0 chosen often so
    that the wrap is tried;
  - the recorders also copy the word that holds the counter at every call
    (the log shows the counter as each callee would see it);
  - func_80119718 and func_80150638 are recorders with no argument and
    result 0; the log shows that both were called, in order.
"""

from contracts import CallLog, Contract, Setup


def setup(state, rng, sym) -> Setup:
    state.w16(sym["data_801ac310"], rng.choice((0xFFFF, 0, rng.getrandbits(16))))
    log = CallLog(state, watch=((sym["data_801ac310"], 1),))
    log.replace(sym["func_80119718"], 0, 0)
    log.replace(sym["func_80150638"], 0, 0)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the increment of the counter: addiu 1 becomes addiu 2."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x09 and w & 0xFFFF == 1]
    if len(found) != 1:
        raise ValueError(f"expected one increment, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 2, "counter incremented by two"


CONTRACT = Contract(setup, control)
