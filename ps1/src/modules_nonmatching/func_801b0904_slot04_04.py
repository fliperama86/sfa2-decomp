"""Contract setup of func_801b0904_slot04_04 (see the header of the .c file).

Choices of the setup:
  - a0 is an object block of 0x394 bytes filled with random bytes;
  - field_12a is 2 in half of the cases, otherwise random (the else arm of
    the first condition), small values 0..15 in a third of those so that the
    table of func_801b09ac is indexed within itself and its odd/even forms
    are tried;
  - the top nibble of field_130 is 4 in four cases of five when field_12a is 2,
    otherwise random;
  - field_48 and field_129 are 0 in a third of the cases, random otherwise;
  - func_80141f28 and func_801307e0 are recorders (2 arguments each) that
    return a random word;
  - every recorder copies the whole object (0x394 bytes) into its log entry at
    each call, so the order of the function's stores against the calls is
    tested. No recorded callee gets a pointer to memory filled for the call.
"""

from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    log = CallLog(state, watch=((obj, 0x394 // 4),))
    log.replace(sym["func_80141f28"], 2, rng.getrandbits(32))
    log.replace(sym["func_801307e0"], 2, rng.getrandbits(32))
    fill(state, obj, 0x394, rng)
    if rng.random() < 0.5:
        a = 2
    elif rng.random() < 0.33:
        a = rng.randrange(16)
    else:
        a = rng.randrange(256)
    state.w8(obj + 0x12A, a)
    word = rng.getrandbits(16)
    if a == 2 and rng.random() < 0.8:
        word = (word & 0x0FFF) | 0x4000
    state.w16(obj + 0x130, word)
    for offset in (0x48, 0x129):
        state.w8(obj + offset, 0 if rng.random() < 0.33 else rng.randrange(1, 256))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Change the constant of the sequence started when field_48 is not 0.

    The instruction is `ori a1,zero,0x1d`; the then arm executes it whenever
    field_48 is not 0, and the recorded argument then differs.
    """
    found = [i for i, w in enumerate(words) if w == 0x3405001D]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x1d, found {len(found)}")
    return found[0], 0x3405001E, "sequence constant 0x1d changed to 0x1e"


CONTRACT = Contract(setup, control)
