"""a0 is a character object; game_state.config points at a block.

Reads and writes are listed in the header comment of
func_801b248c_slot04_0f.c. Choices made here:
  - the object and the config block are filled with random bytes;
  - field_74 and game_state.field_47 are 0 in three cases of four, field_d8
    is 0 in half, field_2a2 is random;
  - field_c6 is chosen near the limits 0 and 0x30 / 0x90 (below, at and
    above them) in three cases of four, otherwise random, so that the
    clamps and the sound request of func_80141f28 are reached;
  - the config block has mode 3 and field_2f 0 in a third of the cases,
    the object's field_cd is 0 in half (the two arms of func_80138ae8);
  - field_49 is 0 in half;
  - the log watches the whole object and the word holding data_80188ec4 at
    every recorded call;
  - func_80120554 (3 arguments) and func_801307e0 (2 arguments) are
    recorders that return 0.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    game_state = sym["game_state"]
    fill(state, game_state, 0x400, rng)
    cfg = state.alloc(0x400)
    fill(state, cfg, 0x400, rng)
    state.w32(game_state + 0x360, cfg)
    if rng.random() < 1 / 3:
        state.w8(cfg + 0x1B, 3)
        state.w8(cfg + 0x2F, 0)
    state.w8(game_state + 0x47, 0 if rng.random() < 0.75 else rng.randrange(1, 256))
    state.w16(sym["data_80188ec4"], rng.getrandbits(16))

    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x74, 0 if rng.random() < 0.75 else rng.randrange(1, 256))
    state.w8(obj + 0xCD, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    d8 = 0 if rng.random() < 0.5 else rng.randrange(1, 256)
    state.w8(obj + 0xD8, d8)
    state.w8(obj + 0x49, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    if rng.random() < 0.75:
        top = 0x30 if d8 else 0x90
        state.w16(obj + 0xC6, rng.choice((0, 1, 2, 3, 4, top - 5, top - 4, top - 3, top - 1, top, top + 1,
                                           0xFFFC, 0xFFFF, 0x7FFF, 0x8000, 0x8F, 0x2F)))

    log = CallLog(state, watch=((obj, 0x394 // 4), (sym["data_80188ec4"], 1)))
    log.replace(sym["func_80120554"], 3, 0)
    log.replace(sym["func_801307e0"], 2, 0)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the addiu that adds 1 to field_07: it adds 2."""
    found = [i for i, w in enumerate(words) if w >> 26 == 9 and w & 0xFFFF == 1 and (w >> 21 & 31) == (w >> 16 & 31)]
    if not found:
        raise ValueError("no addiu of 1 on one register found")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 2, "field_07 increment changed to 2"


CONTRACT = Contract(setup, control)
