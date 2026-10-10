"""Contract of func_8013172c: a0 = object, no return value.

Choices of the setup (reads and writes are listed in func_8013172c.c):
  - game_state.field_30 is 0 in one case of five, else random non-zero;
  - game_state.mode is the object's side + 1 cut to a byte in one case of
    two (for side 0xff that is 0, which differs from the C's side + 1),
    else random (so both arms of the side test are common);
  - data_801a6985 and data_801a6987 are 0 in one case of three, else
    random non-zero bytes 1 to 255 (signed bytes, so negative values are
    tried);
  - the object is a random block of 0x394 bytes; field_0b and field_158 are
    0 in one case of three, else random non-zero; the button words
    (field_130, field_132, field_150) are random 16-bit values, and with
    probability one half each is instead one of 0, 0x2000, 0x8000, 0xa000
    plus, in one case of two, a random part of the bits 0x5fff, so that the
    swap of bits is tried with every combination of bits 0x2000 and 0x8000;
  - func_80131ab4 is a recorder taking one argument and returning 0; it
    copies the whole object (0x394 bytes) at the call.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    gs = sym["game_state"]
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    side = rng.getrandbits(8)
    state.w8(obj + 0xA6, side)
    state.w8(gs + 0x30, 0 if rng.random() < 0.2 else rng.randrange(1, 256))
    state.w8(gs + 0x1B, (side + 1) & 0xFF if rng.random() < 0.5 else rng.getrandbits(8))
    for name in ("data_801a6985", "data_801a6987"):
        state.w8(sym[name], 0 if rng.random() < 1 / 3 else rng.randrange(1, 256))
    for off in (0x0B, 0x158):
        state.w8(obj + off, 0 if rng.random() < 1 / 3 else rng.randrange(1, 256))
    for off in (0x130, 0x132, 0x150):
        if rng.random() < 0.5:
            value = rng.choice((0, 0x2000, 0x8000, 0xA000)) | (rng.getrandbits(16) & 0x5FFF if rng.random() < 0.5 else 0)
        else:
            value = rng.getrandbits(16)
        state.w16(obj + off, value)
    log = CallLog(state, watch=((obj, 0x394 // 4),))
    log.replace(sym["func_80131ab4"], 1, 0)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of field_134: its offset moves from 0x134 to 0x138."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0x134]
    if len(found) != 1:
        raise ValueError(f"expected one store to field_134, found {len(found)}")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0x138, "store of field_134 moved by four bytes"


CONTRACT = Contract(setup, control)
