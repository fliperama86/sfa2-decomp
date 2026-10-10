"""Contract setup of func_801b37b0_slot04_03 (see the header of the .c file).

Choices of the setup:
  - a0 and the object at field_40 are distinct 0x394-byte blocks filled with
    random bytes; ref_other starts as a random word;
  - the low byte of field_3a is 0 in half of the cases;
  - field_1cf is 0 in half of the cases;
  - the results of func_80140cd8 and func_801410c8 have a zero low byte in
    half of the cases (the upper bytes stay random, since only the low byte
    is used);
  - field_46 is 0 in a sixth of the cases and 0xffff in a sixth (the
    increment then makes it 0), otherwise random;
  - the other object's field_15b is 0 in half of the cases;
  - all callees are recorders with the argument counts of the header;
  - every recorder copies the whole object, the whole other object and the
    word ref_other into its log entry at each call, so the order of the
    function's stores against the calls is tested. The log has room for
    every call of the longest path. No recorded callee gets a pointer to
    memory filled for the call.
"""

from contracts import CallLog, Contract, Setup, fill

RECORDED = (
    ("func_801204f4", 3),
    ("func_80120554", 3),
    ("func_80146478", 4),
    ("func_80140770", 7),
    ("func_80140fe0", 1),
    ("func_801307e0", 2),
    ("func_80130efc", 1),
)


def result_word(rng):
    word = rng.getrandbits(32)
    if rng.random() < 0.5:
        word &= 0xFFFFFF00
    elif word & 0xFF == 0:
        word |= 1
    return word


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    other = state.alloc(0x394)
    ref = sym["ref_other"]
    log = CallLog(state, words=4096, watch=((obj, 0x394 // 4), (other, 0x394 // 4), (ref, 1)))
    for name, count in RECORDED:
        log.replace(sym[name], count, rng.getrandbits(32))
    log.replace(sym["func_80140cd8"], 3, result_word(rng))
    log.replace(sym["func_801410c8"], 1, result_word(rng))

    fill(state, obj, 0x394, rng)
    fill(state, other, 0x394, rng)
    state.w32(ref, rng.getrandbits(32))
    state.w32(obj + 0x40, other)
    half = rng.getrandbits(16)
    if rng.random() < 0.5:
        half &= 0xFF00
    elif half & 0xFF == 0:
        half |= 1
    state.w16(obj + 0x3A, half)
    state.w8(obj + 0x1CF, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    roll = rng.random()
    if roll < 1 / 6:
        state.w16(obj + 0x46, 0)
    elif roll < 2 / 6:
        state.w16(obj + 0x46, 0xFFFF)
    if rng.random() < 0.5:
        state.w8(other + 0x15B, 0)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Change the sound number 0x30d of the advance (`ori a2,zero,0x30d`)."""
    found = [i for i, w in enumerate(words) if w == 0x3406030D]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x30d, found {len(found)}")
    return found[0], 0x3406030E, "sound number 0x30d changed to 0x30e"


CONTRACT = Contract(setup, control)
