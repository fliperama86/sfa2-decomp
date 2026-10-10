"""Contract of func_80141cec (header comment of the .c file lists reads and writes).

Choices of the setup:
  - the object, the partner block (object->field_40) and the configuration
    block (game_state.config) are separate blocks of random bytes;
  - bytes 4 to 7 of the object: exactly 1, 1, 2, 0 in a third of the cases,
    1, 0 and a byte 6 from {2, 7, 8, 9, random} with byte 7 random in
    another third, random otherwise; field_7e and field_45 are 0 in four
    cases of five;
  - game_state.field_30 is 0 in a third of the cases; game_state.mode (byte)
    equals the object's side plus 1 in half the cases;
  - the three configuration bytes are all 0 in two cases of three;
  - the partner block's 16-bit field_04 has 1 in its low byte in half the
    cases (func_8012f56c then returns 0); the object's field_cd is 0 in
    half of them;
  - ref_other, which a callee writes (func_8012f56c or func_80142c04), starts
    as a random word;
  - func_80155d4c is a recorder with two arguments, result 0, no pointee;
    the log watches the whole object and ref_other at every call.
"""
from contracts import CallLog, Contract, Setup, fill, halfword


def setup(state, rng, sym) -> Setup:
    config = state.alloc(0x80)
    fill(state, config, 0x80, rng)
    if rng.random() < 0.67:
        for off in (0x4D, 0x4E, 0x04):
            state.w8(config + off, 0)
    state.w32(sym["game_state"] + 0x360, config)
    partner = state.alloc(0x394)
    fill(state, partner, 0x394, rng)
    if rng.random() < 0.5:
        state.w8(partner + 4, 1)
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w32(obj + 0x40, partner)
    pick = rng.random()
    if pick < 0.33:
        bytes4 = (1, 1, 2, 0)
    elif pick < 0.66:
        bytes4 = (1, 0, rng.choice((2, 7, 8, 9, rng.randrange(256))), rng.randrange(256))
    else:
        bytes4 = tuple(rng.randrange(256) for _ in range(4))
    for i, b in enumerate(bytes4):
        state.w8(obj + 4 + i, b)
    if rng.random() < 0.8:
        state.w8(obj + 0x7E, 0)
        state.w8(obj + 0x45, 0)
    side = rng.randrange(256)
    state.w8(obj + 0xA6, side)
    state.w8(obj + 0xCD, rng.choice((0, rng.randrange(256))))
    state.w8(sym["game_state"] + 0x30, 0 if rng.random() < 0.33 else rng.randrange(1, 256))
    state.w8(sym["game_state"] + 0x1B, (side + 1) & 0xFF if rng.random() < 0.5 else rng.randrange(256))
    state.w32(sym["ref_other"], rng.getrandbits(32))
    log = CallLog(state, 1024, watch=((obj, 0x394 // 4), (sym["ref_other"], 1)))
    log.replace(sym["func_80155d4c"], 2)
    return Setup(args=(obj,), returns_value=True)


def control(words):
    """Alter the byte store to field_157: its offset moves by one byte."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0x157]
    if len(found) != 1:
        raise ValueError(f"expected one store to field_157, found {len(found)}")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0x158, "store of field_157 moved by one byte"


CONTRACT = Contract(setup, control)
