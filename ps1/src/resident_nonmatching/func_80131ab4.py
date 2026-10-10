"""Contract of func_80131ab4: a0 = object, no return value.

Choices of the setup (reads and writes are listed in func_80131ab4.c):
  - input_log.index is 0xffd to 0xffff in one case of ten (the function
    does nothing then), 0xff8 to 0xffc in 18 cases of 100, and otherwise
    random from 0 to 0xffc;
  - the log buffer, last and flag start random (flag 0 in one case of
    three, else random non-zero);
  - the controller words data_801a6966 and data_801a6972 are random, equal
    in one case of eight; input_log.last equals data_801a6966 in 40 cases
    of 100, data_801a6972 in 30 of 100, else random;
  - the object's field_02 is 0 or 1 in four cases of five, else random; its
    side is 0 in one case of two; data_80171b34 is 0xffff in one case of ten,
    else random;
  - the object block is 0xb0 bytes of random content.
"""
from contracts import Contract, Setup, fill


def setup(state, rng, sym):
    log = sym["input_log"]
    r = rng.random()
    if r < 0.1:
        index = rng.randrange(0xFFD, 0x10000)
    elif r < 0.28:
        index = rng.randrange(0xFF8, 0xFFD)
    else:
        index = rng.randrange(0, 0xFFD)
    fill(state, log, 0x2008, rng)
    state.w32(log, index)
    state.w8(log + 0x2006, 0 if rng.random() < 1 / 3 else rng.randrange(1, 256))
    w66 = rng.getrandbits(16)
    w72 = w66 if rng.random() < 1 / 8 else rng.getrandbits(16)
    state.w16(sym["data_801a6966"], w66)
    state.w16(sym["data_801a6972"], w72)
    r = rng.random()
    state.w16(log + 0x2004, w66 if r < 0.4 else (w72 if r < 0.7 else rng.getrandbits(16)))
    state.w16(sym["data_80171b34"], 0xFFFF if rng.random() < 0.1 else rng.getrandbits(16))
    state.w16(sym["data_80171b32"], rng.getrandbits(16))
    obj = state.alloc(0xB0)
    fill(state, obj, 0xB0, rng)
    state.w8(obj + 2, rng.choice((0, 1)) if rng.random() < 0.8 else rng.getrandbits(8))
    state.w8(obj + 0xA6, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter `ori v0,zero,1` (the flag set by the repeat arms) to store 2."""
    found = [i for i, w in enumerate(words) if w == 0x34020001]
    if not found:
        raise ValueError("expected a load of 1 into v0")
    i = found[0]
    return i, 0x34020002, "flag value 1 changed to 2"


CONTRACT = Contract(setup, control)
