"""Contract of func_80077868_slot2b (reads and writes: see the header of the .c).

Choices of the setup:
  - the object is a random block of 0x394 bytes; field_219 is 0 in four
    cases of five, a random non-zero byte otherwise;
  - field_48 and field_129 are each 0 in half of the cases, non-zero
    otherwise; field_12a is any byte;
  - func_80077908_slot2b, func_80141f28 and func_801307e0 are recorders; the
    log copies the whole object at every call.
"""

from contracts import CallLog, Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def maybe_zero(rng, odds=0.5):
    return 0 if rng.random() < odds else rng.randrange(1, 256)


def setup(state, rng, sym) -> Setup:
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x219, maybe_zero(rng, 0.8))
    state.w8(obj + 0x48, maybe_zero(rng))
    state.w8(obj + 0x129, maybe_zero(rng))

    log = CallLog(state, 1024, watch=((obj, 0x394 // 4),))
    log.replace(sym["func_80077908_slot2b"], 1)
    log.replace(sym["func_80141f28"], 2)
    log.replace(sym["func_801307e0"], 2)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the small base: `ori v1,zero,0xc` becomes `ori v1,zero,0xd`."""
    found = [i for i, w in enumerate(words) if w >> 26 == 13 and w & 0xFFFF == 0xC]
    if len(found) != 1:
        raise ValueError(f"expected one ori of 0xc, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0xD, "base 0xc changed to 0xd"


CONTRACT = Contract(setup, control)
