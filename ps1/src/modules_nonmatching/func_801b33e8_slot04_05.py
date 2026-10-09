"""Contract setup of func_801b33e8_slot04_05 (see the header of the .c file).

Choices of the setup:
  - a0 is an object block of 0x394 bytes filled with random bytes;
  - the low byte of field_46 is 1 in one case of three (the countdown ends),
    0 in one case of eight (wraps to 0xffff), otherwise random; its high
    byte is 0 in one case of three;
  - field_45, field_134 and field_27d are each 0 in half of the cases,
    otherwise random non-zero; field_27c is random, 0xff in one case of
    eight (wrap);
  - func_80130efc, func_80142c70 and func_80142fe8 (1 argument each) are
    recorders returning a random word; each copies the whole object into
    its log entry at every call, so the order of the function's stores
    against the calls is tested.
"""

from contracts import CallLog, Contract, Setup, fill


def zero_or(rng):
    return 0 if rng.random() < 0.5 else rng.randrange(1, 256)


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    log = CallLog(state, watch=((obj, 0x394 // 4),))
    for name in ("func_80130efc", "func_80142c70", "func_80142fe8"):
        log.replace(sym[name], 1, rng.getrandbits(32))
    fill(state, obj, 0x394, rng)
    r = rng.random()
    state.w8(obj + 0x46, 1 if r < 0.33 else 0 if r < 0.46 else rng.randrange(256))
    state.w8(obj + 0x47, 0 if rng.random() < 0.33 else rng.randrange(1, 256))
    for offset in (0x45, 0x134, 0x27D):
        state.w8(obj + offset, zero_or(rng))
    state.w8(obj + 0x27C, 0xFF if rng.random() < 0.125 else rng.randrange(256))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Change the value stored in field_06: the ori of 3."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0xD and w & 0xFFFF == 3]
    if len(found) != 1:
        raise ValueError(f"expected one ori of 3, found {len(found)}")
    return found[0], words[found[0]] + 1, "field_06 value 3 changed to 4"


CONTRACT = Contract(setup, control)
