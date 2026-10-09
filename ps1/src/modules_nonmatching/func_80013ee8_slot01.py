"""Contract of func_80013ee8_slot01 as code.

a0 is an object that the function puts into a start state, then calls the
function stored in data_80015cdc_slot01 with it.

Reads and writes are listed in the header comment of func_80013ee8_slot01.c.
Choices made here:
  - the object is 0x394 bytes of random content;
  - game_state.mode is random, so bit 0 is set and clear in about equal shares;
  - data_80015cdc_slot01 holds the address of a block (in RAM that the setup
    allocated) with a recorder: one argument, result 0; the recorder copies
    the whole object into the log at the call.
"""

from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym) -> Setup:
    state.w8(sym["game_state"] + 0x1B, rng.getrandbits(8))
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    log = CallLog(state, 1024, watch=((obj, 0x394 // 4),))
    target = state.alloc(16)
    state.w32(sym["data_80015cdc_slot01"], target)
    log.replace(target, 1, 0)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of the constant 0x54 into pos_y: it becomes 0x55."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0D and w & 0xFFFF == 0x54]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x54, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x55, "pos_y constant 0x54 changed to 0x55"


CONTRACT = Contract(setup, control)
