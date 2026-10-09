"""Contract of func_80140cd8 (header comment of the .c file lists reads and writes).

Choices of the setup:
  - the object, its partner (object->other) and the configuration block
    (game_state.config) are separate blocks of random bytes; the two
    configuration bytes are both 0 in three cases of four;
  - a is a random 32-bit value; b is small (0 to 40) in three cases of four
    and random 32-bit otherwise; the partner's field_5c is random 16-bit,
    or small (0 to 40) in a third of the cases so that the subtraction
    crosses zero often;
  - game_state.field_12 is 0 to 3 and table_8017ac34[0..3] point at four
    small blocks, each holding a zero-argument recorder;
  - func_80155d4c is a recorder with two arguments, result 0, no pointee;
    the log watches the whole object, the whole partner, ref_other and
    game_state.cursor at every call;
  - the random state of func_80151184 is random; the tables the function
    reads keep the resident image's contents.
"""
from contracts import CallLog, Contract, Setup, fill, halfword


def setup(state, rng, sym) -> Setup:
    config = state.alloc(0x80)
    fill(state, config, 0x80, rng)
    if rng.random() < 0.75:
        state.w8(config + 0x4D, 0)
        state.w8(config + 0x4E, 0)
    state.w32(sym["game_state"] + 0x360, config)
    partner = state.alloc(0x394)
    fill(state, partner, 0x394, rng)
    if rng.random() < 0.33:
        state.w16(partner + 0x5C, rng.randrange(0, 41))
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w32(obj + 0x40, partner)
    state.w16(sym["game_state"] + 0x1E, halfword(rng))
    state.w8(sym["game_state"] + 0x12, rng.randrange(4))
    state.w32(sym["ref_other"], rng.getrandbits(32))
    log = CallLog(state, 2048, watch=((obj, 0x394 // 4), (partner, 0x394 // 4),
                                      (sym["ref_other"], 1), (sym["game_state"] + 0x304, 1)))
    for i in range(4):
        block = state.alloc(8)
        state.w32(sym["table_8017ac34"] + 4 * i, block)
        log.replace(block, 0)
    log.replace(sym["func_80155d4c"], 2)
    a = rng.getrandbits(32)
    b = rng.randrange(0, 41) if rng.random() < 0.75 else rng.getrandbits(32)
    return Setup(args=(obj, a, b), returns_value=True)


def control(words):
    """Alter the halfword store to field_5c: its offset moves by two bytes (first such store)."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0x5C and (w >> 16) & 0x1F != 0]
    if not found:
        raise ValueError("no store to field_5c found")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0x5E, "store of field_5c moved by two bytes"


CONTRACT = Contract(setup, control)
