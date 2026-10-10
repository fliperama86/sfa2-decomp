"""Contract setup of func_801b36f8_slot04_09 (see the header of the .c file).

Choices of the setup:
  - a0 is an object block of 0x394 bytes filled with random bytes, and
    field_40 points to a second block of 0x394 bytes ("other"), also random;
  - the halfword field_3a is negative in half of the cases; field_a3, field_0b
    and field_67 are each 0 in half of the cases, otherwise random non-zero;
    other's field_61 is 0xff with probability 0.33, else a random byte;
  - the word at field_10 is a random word and the velocity field_4c a random
    signed 20-bit value; the setup computes
    the object's pos_x after the move, and gives other a pos_x within
    -0x30 to +0x30 of it in half of the cases (so the distance test
    "below 0x48" goes both ways, edges included), a random halfword
    otherwise;
  - field_07 is a random byte, 0xff in one case of eight (wrap);
  - func_801307e0 (2 arguments) and func_80130efc (1 argument) are
    recorders returning a random word; each copies the whole object and
    the whole of other into its log entry at every call, so the order of
    the function's stores against the calls is tested.
"""

from contracts import CallLog, Contract, Setup, fill


def zero_or(rng):
    return 0 if rng.random() < 0.5 else rng.randrange(1, 256)


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    other = state.alloc(0x394)
    log = CallLog(state, watch=((obj, 0x394 // 4), (other, 0x394 // 4)))
    log.replace(sym["func_801307e0"], 2, rng.getrandbits(32))
    log.replace(sym["func_80130efc"], 1, rng.getrandbits(32))
    fill(state, obj, 0x394, rng)
    fill(state, other, 0x394, rng)
    state.w32(obj + 0x40, other)
    half = rng.getrandbits(15)
    state.w16(obj + 0x3A, half | (0x8000 if rng.random() < 0.5 else 0))
    for offset in (0xA3, 0x0B, 0x67):
        state.w8(obj + offset, zero_or(rng))
    state.w8(other + 0x61, 0xFF if rng.random() < 0.33 else rng.randrange(256))
    state.w8(obj + 0x07, 0xFF if rng.random() < 0.125 else rng.randrange(256))
    w10 = rng.getrandbits(32)
    v = rng.randrange(-(1 << 19), 1 << 19)
    state.w32(obj + 0x10, w10)
    state.w32(obj + 0x4C, v & 0xFFFFFFFF)
    after = ((w10 + v) & 0xFFFFFFFF) >> 16
    if rng.random() < 0.5:
        ox = (after + rng.randrange(-0x30, 0x31)) & 0xFFFF
    else:
        ox = rng.getrandbits(16)
    state.w16(other + 0x12, ox)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Change the sequence number: the ori of 0x33 (first of the two)."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0xD and w & 0xFFFF == 0x33]
    if not found:
        raise ValueError("no ori of 0x33")
    return found[0], words[found[0]] + 1, "sequence 0x33 changed to 0x34"


CONTRACT = Contract(setup, control)
