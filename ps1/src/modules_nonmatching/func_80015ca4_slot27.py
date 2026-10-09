"""Contract of func_80015ca4_slot27: an object that may spawn one helper.

Choices of the setup (reads and writes are in the header of the .c file):
  - the object is a 0x394-byte block of random bytes; field_60 is 0 in
    three cases of eight, field_05 is 0 in half of the rest, so that each
    of the four arms is common;
  - game_state.field_04 is 0 in half of the cases; game_state.field_40 is
    random;
  - data_8002b386_slot27 is 0, 0x8000, 0xFFFF or random (the sign test's borders);
  - func_8011f1e0 returns the address of a block of 0x394 random bytes, or 0
    in one case of ten;
  - the four callees are recorders; the log watches the object and the new
    block whole and has room for the longest run (two calls).
"""
from contracts import CallLog, Contract, Setup, fill

SIZE = 0x394


def setup(state, rng, sym):
    game_state = sym["game_state"]
    state.w8(game_state + 0x04, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    state.w8(game_state + 0x40, rng.randrange(256))
    state.w16(sym["data_8002b386_slot27"], rng.choice((0, 0x8000, 0xFFFF, rng.getrandbits(16))))

    obj = state.alloc(SIZE)
    fill(state, obj, SIZE, rng)
    state.w8(obj + 0x60, 0 if rng.random() < 0.625 else rng.randrange(1, 256))
    state.w8(obj + 0x05, 0 if rng.random() < 0.5 else rng.randrange(1, 256))

    block = state.alloc(SIZE)
    fill(state, block, SIZE, rng)
    new = 0 if rng.random() < 0.1 else block

    log = CallLog(state, 2 * (2 + 2 * SIZE // 4) + 16, watch=((obj, SIZE // 4), (block, SIZE // 4)))
    log.replace(sym["func_80131094"], 1)
    log.replace(sym["func_8011ffdc"], 1)
    log.replace(sym["func_80016810_slot27"], 1)
    log.replace(sym["func_8011f1e0"], 0, new)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of the constant 0x1e0 into field_7c of the new object.

    The store is `sh v0, 0x7c(a1)`; moved to 0x7e it leaves field_7c alone.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0x7C]
    if len(found) != 1:
        raise ValueError(f"expected one store to field_7c, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x7E, "field_7c store moved by two bytes"


CONTRACT = Contract(setup, control)
