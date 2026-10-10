"""a0 is a character object.

Reads and writes are listed in the header comment of
func_801b2270_slot04_0f.c. Choices made here:
  - the object is filled with random bytes;
  - field_339 equals field_3a in a quarter of the cases (the early end), and
    otherwise differs from it;
  - field_3a has bit 7 set in half of the cases (the other arm), and the
    values 0 and 0x7f and 0x80 and 0xff are picked often;
  - field_d4 is random over the byte, with 0 and 255 picked often;
  - the log watches the whole object at every recorded call;
  - func_801b22ec (1 argument) and func_801b2340 (2 arguments) are recorders
    that return 0.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    wanted = rng.choice((0, 0x7F, 0x80, 0xFF, rng.randrange(0x80), rng.randrange(0x80, 0x100), rng.randrange(256)))
    state.w8(obj + 0x3A, wanted)
    if rng.random() < 0.25:
        state.w8(obj + 0x339, wanted)
    else:
        state.w8(obj + 0x339, (wanted + rng.randrange(1, 256)) & 0xFF)
    state.w8(obj + 0xD4, rng.choice((0, 255, rng.randrange(256), rng.randrange(256))))
    log = CallLog(state, watch=((obj, 0x394 // 4),))
    log.replace(sym["func_801b22ec_slot04_0f"], 1, 0)
    log.replace(sym["func_801b2340_slot04_0f"], 2, 0)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the `andi v0,v0,0x80` that tests bit 7: it tests bit 6."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0C and w & 0xFFFF == 0x80]
    if len(found) != 1:
        raise ValueError(f"expected one test of bit 7, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x40, "bit test 0x80 changed to 0x40"


CONTRACT = Contract(setup, control)
