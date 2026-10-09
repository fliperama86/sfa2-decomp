"""Contract setup of func_801b1884_slot04_08 (see the header of the .c file).

Choices of the setup:
  - a0 is an object block of 0x394 bytes filled with random bytes;
  - the byte at 0x1c8 is 1 in one case of three (the count reaches 0 and
    field_17b is cleared), otherwise random;
  - field_0b is 0 in half of the cases, otherwise random non-zero;
  - field_10 is a random word, field_4c and field_54 are random signed
    30-bit values; in a quarter of the cases field_54 is chosen to make the
    new field_4c fall just below, on or just above zero (edge of the
    signed test, for both senses);
  - field_07 is a random byte, 0xff in one case of eight (wrap);
  - func_80130efc (1 argument) is a recorder returning a random word;
    it copies the whole object (0x394 bytes) into its log entry at the
    call, so the order of the function's stores against the call is tested.
"""

from contracts import CallLog, Contract, Setup, fill


def s30(rng):
    return rng.randrange(-(1 << 30), 1 << 30)


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    log = CallLog(state, watch=((obj, 0x394 // 4),))
    log.replace(sym["func_80130efc"], 1, rng.getrandbits(32))
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x1C8, 1 if rng.random() < 0.33 else rng.randrange(256))
    state.w8(obj + 0x0B, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    state.w32(obj + 0x10, rng.getrandbits(32))
    v = s30(rng)
    a = s30(rng)
    if rng.random() < 0.25:
        a = -v + rng.choice((-1, 0, 1))
    state.w32(obj + 0x4C, v & 0xFFFFFFFF)
    state.w32(obj + 0x54, a & 0xFFFFFFFF)
    state.w8(obj + 0x07, 0xFF if rng.random() < 0.125 else rng.randrange(256))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Change the increment of field_07: the addiu of 1 on the loaded byte."""
    found = [i for i, w in enumerate(words) if w & 0xFFFF == 1 and w >> 26 == 9 and (w >> 21 & 31) == (w >> 16 & 31)]
    if len(found) != 1:
        raise ValueError(f"expected one addiu of 1, found {len(found)}")
    return found[0], words[found[0]] + 1, "increment of field_07 changed to 2"


CONTRACT = Contract(setup, control)
