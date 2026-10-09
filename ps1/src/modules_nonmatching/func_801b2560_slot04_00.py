"""Contract setup of func_801b2560_slot04_00 (see the header of the .c file).

Choices of the setup:
  - a0 is an object block of 0x394 bytes filled with random bytes;
  - the byte at 0x3a is 0 in three cases of four, otherwise random (so the
    else arm is also taken, including with a nonzero byte);
  - field_50 and field_58 are random 30-bit signed values, so that the sum
    is negative about half of the time and never overflows; in a quarter of
    the cases field_58 is chosen to make the sum fall just below or just on
    zero (edge of the signed test);
  - field_07 and field_12a are random bytes, with 0xff in one case of eight
    (wrap of the increments);
  - func_801307e0 (2 arguments) and func_80130efc (1 argument) are
    recorders returning a random word;
  - every recorder copies the whole object (0x394 bytes) into its log entry
    at each call, so the order of the function's stores against the calls is
    tested. No recorded callee gets a pointer to memory filled for the call.
"""

from contracts import CallLog, Contract, Setup, fill


def s30(rng):
    return rng.randrange(-(1 << 30), 1 << 30)


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    log = CallLog(state, watch=((obj, 0x394 // 4),))
    log.replace(sym["func_801307e0"], 2, rng.getrandbits(32))
    log.replace(sym["func_80130efc"], 1, rng.getrandbits(32))
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x3A, 0 if rng.random() < 0.75 else rng.randrange(256))
    v = s30(rng)
    a = s30(rng)
    if rng.random() < 0.25:
        a = -v + rng.choice((-1, 0, 1))
    state.w32(obj + 0x50, v & 0xFFFFFFFF)
    state.w32(obj + 0x58, a & 0xFFFFFFFF)
    for offset in (0x07, 0x12A):
        state.w8(obj + offset, 0xFF if rng.random() < 0.125 else rng.randrange(256))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Change the base of the sequence number: the addiu of 0x32."""
    found = [i for i, w in enumerate(words) if w == 0x24A50032]
    if len(found) != 1:
        raise ValueError(f"expected one addiu of 0x32, found {len(found)}")
    return found[0], 0x24A50033, "sequence base 0x32 changed to 0x33"


CONTRACT = Contract(setup, control)
