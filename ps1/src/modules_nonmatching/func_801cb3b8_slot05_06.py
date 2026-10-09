"""Contract setup of func_801cb3b8_slot05_06 (see the header of the .c file).

Choices of the setup:
  - a0 is an object block of 0x394 bytes filled with random bytes, and
    field_40 points to a second block of 0x394 bytes ("other"), also random;
  - field_4c is a random signed 30-bit value, negative in half of the cases, 0 in a tenth
    (the move adds field_54, also 30-bit, so the sum does not overflow);
    field_0b is 0 in half of the cases, otherwise non-zero (the move's two
    signs);
  - other's field_6b is non-zero in three cases of four, its field_61 is 1
    in three cases of four, its field_5c is random with the sign taken at
    random (zero and -1 in one case of eight each);
  - field_07 and field_12a are random bytes, 0xff in one case of eight
    (wrap of the increments and of the sum of 3);
  - func_801307e0 (2 arguments) and func_80130efc (1 argument) are
    recorders returning a random word; each copies the whole object and
    the whole of other into its log entry at every call, so the order of
    the function's stores against the calls is tested.
"""

from contracts import CallLog, Contract, Setup, fill


def s30(rng):
    return rng.randrange(-(1 << 30), 1 << 30)


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    other = state.alloc(0x394)
    log = CallLog(state, watch=((obj, 0x394 // 4), (other, 0x394 // 4)))
    log.replace(sym["func_801307e0"], 2, rng.getrandbits(32))
    log.replace(sym["func_80130efc"], 1, rng.getrandbits(32))
    fill(state, obj, 0x394, rng)
    fill(state, other, 0x394, rng)
    state.w32(obj + 0x40, other)
    state.w32(obj + 0x4C, s30(rng) & 0xFFFFFFFF)
    state.w32(obj + 0x54, s30(rng) & 0xFFFFFFFF)
    if rng.random() < 0.5:
        state.w32(obj + 0x4C, abs(s30(rng)) & 0xFFFFFFFF)
    if rng.random() < 0.1:
        state.w32(obj + 0x4C, 0)
    state.w8(obj + 0x0B, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    for offset in (0x07, 0x12A):
        state.w8(obj + offset, 0xFF if rng.random() < 0.125 else rng.randrange(256))
    state.w8(other + 0x6B, rng.randrange(1, 256) if rng.random() < 0.75 else 0)
    state.w8(other + 0x61, 1 if rng.random() < 0.75 else rng.randrange(256))
    r = rng.random()
    state.w16(other + 0x5C, 0 if r < 0.125 else 0xFFFF if r < 0.25 else rng.getrandbits(16))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Change the base of the sequence number: the addiu of 0x45."""
    found = [i for i, w in enumerate(words) if w >> 26 == 9 and w & 0xFFFF == 0x45]
    if len(found) != 1:
        raise ValueError(f"expected one addiu of 0x45, found {len(found)}")
    return found[0], words[found[0]] + 1, "sequence base 0x45 changed to 0x46"


CONTRACT = Contract(setup, control)
