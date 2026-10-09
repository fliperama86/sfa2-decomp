"""Contract of func_80137e38 (see the header comment of func_80137e38.c).

Choices of the setup:
  - the object is a 0x394-byte block of random bytes, so field_06, field_0b,
    field_46 (both bytes) and field_80 take random values, including the
    wrap of the counter at 255;
  - func_801380f0 and func_80131094 are recorders that take one argument and
    return 0.
  - the whole object is watched: each recorder copies its 0x394 bytes at the
    call, so the order of the stores against the calls is compared.
"""

from contracts import CallLog, Contract, Setup


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    state.write(obj, bytes(rng.getrandbits(8) for _ in range(0x394)))
    if rng.random() < 0.2:
        state.w8(obj + 0x06, 0xFF)
    log = CallLog(state, 1024, watch=((obj, 0x394 // 4),))
    log.replace(sym["func_801380f0"], 1, 0)
    log.replace(sym["func_80131094"], 1, 0)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of field_80: its offset moves by one byte (0x80 to 0x81)."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0x80]
    if len(found) != 1:
        raise ValueError(f"expected one store of field_80, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x81, "field_80 store moved by one byte"


CONTRACT = Contract(setup, control)
