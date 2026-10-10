"""Contract of func_80140b5c (header comment of the .c file lists reads and writes).

Choices of the setup:
  - the object and the target of ref_other are separate 0x394-byte blocks of
    random bytes; the target's field_15b is 0 in half of the cases;
  - `a` is -1 in one case of eight, otherwise a random 16-bit value; the
    upper half of a1 holds random bits; b is a small value (0 to 40) in
    three cases of four and a random 32-bit value otherwise;
  - the random state of func_80151184 (the seed halfword data_80190126, which
    it writes; read from the original's listing, not tested) is random; a
    halfword of game_state at offset 0x1e is random (this C does not read
    it); game_state.cursor, which the function writes, starts as a random
    word (so that a return before the write is seen to leave it);
  - game_state.field_12 is 0 to 3 and table_8017ac34[0..3] point at four
    small blocks, each holding a zero-argument recorder; the log watches the
    target block and game_state.cursor at every call;
  - the tables the function reads keep the resident image's contents.
"""
from contracts import CallLog, Contract, Setup, fill, halfword


def setup(state, rng, sym) -> Setup:
    target = state.alloc(0x394)
    fill(state, target, 0x394, rng)
    state.w8(target + 0x15B, rng.choice((0, rng.randrange(256))))
    state.w32(sym["ref_other"], target)
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w16(sym["game_state"] + 0x1E, halfword(rng))
    state.w8(sym["game_state"] + 0x12, rng.randrange(4))
    state.w16(sym["data_80190126"], halfword(rng))
    state.w32(sym["game_state"] + 0x304, rng.getrandbits(32))
    log = CallLog(state, 1024, watch=((target, 0x394 // 4), (sym["game_state"] + 0x304, 1)))
    for i in range(4):
        block = state.alloc(8)
        state.w32(sym["table_8017ac34"] + 4 * i, block)
        log.replace(block, 0)
    a = 0xFFFF if rng.random() < 0.125 else halfword(rng)
    b = rng.randrange(0, 41) if rng.random() < 0.75 else rng.getrandbits(32)
    return Setup(args=(obj, halfword(rng) << 16 | a, b), returns_value=True)


def control(words):
    """Alter the byte store of the clearing (sb zero,0x64): move it to 0x65."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0x64 and (w >> 16) & 0x1F == 0]
    if len(found) != 1:
        raise ValueError(f"expected one store of zero to field_64, found {len(found)}")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0x65, "clearing store moved by one byte"


CONTRACT = Contract(setup, control)
