"""Contract of func_80011a14_slot01 as code.

a0 is an object that the function sets up as one of three shapes, chosen by
its field_03; game_state.field_78 points at a second object that the first
shape reads.

Reads and writes are listed in the header comment of func_80011a14_slot01.c.
Choices made here:
  - field_03 is, in equal shares, 0, a value with bit 7 set, or a value from
    1 to 0x7f; field_48 is 0 in half the cases;
  - the other object (0x394 random bytes): kind is 0 to 15; field_2ac is -8
    to -1 or 0 to 7 in equal shares; field_5c is a random halfword, a value
    below 0xf, or one from 0xf to 0x7f in equal shares;
  - game_state.field_0c is 0 in half the cases and random otherwise;
    game_state.field_13a, the generator state at game_state + 0x1e and
    box_margin[0] are random;
  - the table entries at the other object's kind are replaced for the case:
    data_8002ce3c_slot01[kind] points at 8 pointers to 256 random bytes each,
    data_800192dc_slot01[kind] at 256 random words, and the two words
    data_80021170_slot01[2 * kind] are random; data_8002cbfc_slot01 stays as
    the module holds it (its byte only picks an index in 0 to 255);
  - data_80021224_slot01[0] and data_800212ac_slot01[0] point at blocks that
    hold a step (frame index 0 to 7, random other bytes), and the object's
    field_8c points at a block of 8 frame records (the object's field_08 is
    0, 8 or random in equal shares), so that func_80130768 runs;
  - data_8002121c_slot01[0], which the first shape writes, holds random
    bytes before the call;
  - game_state.field_112, which the first shape writes, is a random byte
    before the call;
  - the cursor words that func_80011fe8_slot01 writes (a pointer and two
    halfwords from data_80055e94_slot01, 8 bytes) hold random bytes before
    the call;
  - no recorder: the callees run as the original code in both runs.
"""

from contracts import Contract, Setup, fill, halfword


def setup(state, rng, sym) -> Setup:
    game_state = sym["game_state"]
    kind = rng.randrange(16)

    other = state.alloc(0x394)
    fill(state, other, 0x394, rng)
    state.w8(other + 0xA7, kind)
    state.w32(other + 0x2AC, (rng.randrange(-8, 0) if rng.random() < 0.5 else rng.randrange(8)) & 0xFFFFFFFF)
    state.w16(other + 0x5C, rng.choice((halfword(rng), rng.randrange(0xF), rng.randrange(0xF, 0x80))))
    state.w32(game_state + 0x78, other)
    state.w16(game_state + 0x0C, 0 if rng.random() < 0.5 else halfword(rng))
    state.w8(game_state + 0x13A, rng.getrandbits(8))
    state.w8(game_state + 0x112, rng.getrandbits(8))
    state.w16(game_state + 0x1E, halfword(rng))
    state.w16(sym["box_margin"], halfword(rng))

    fill(state, sym["data_8002121c_slot01"], 4, rng)
    fill(state, sym["data_80055e94_slot01"], 8, rng)

    rows = state.alloc(4 * 8)
    for index in range(8):
        row = state.alloc(256)
        fill(state, row, 256, rng)
        state.w32(rows + 4 * index, row)
    state.w32(sym["data_8002ce3c_slot01"] + 4 * kind, rows)
    words = state.alloc(4 * 256)
    fill(state, words, 4 * 256, rng)
    state.w32(sym["data_800192dc_slot01"] + 4 * kind, words)
    state.w32(sym["data_80021170_slot01"] + 8 * kind, rng.getrandbits(32))
    state.w32(sym["data_80021170_slot01"] + 8 * kind + 4, rng.getrandbits(32))

    for table in ("data_80021224_slot01", "data_800212ac_slot01"):
        step = state.alloc(12)
        fill(state, step, 12, rng)
        state.w16(step + 0x0A, rng.randrange(8))
        state.w32(sym[table], step)

    frames = state.alloc(16 * 8 + 16)
    fill(state, frames, 16 * 8 + 16, rng)

    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x03, rng.choice((0, rng.randrange(0x80, 0x100), rng.randrange(1, 0x80))))
    state.w8(obj + 0x08, rng.choice((0, 8, rng.randrange(256))))
    state.w8(obj + 0x48, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    state.w32(obj + 0x8C, frames)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the first load of the constant 0xa0 (the y position of the first shape): it becomes 0xa1."""
    found = [i for i, w in enumerate(words) if w >> 26 in (0x0D, 0x09) and w & 0xFFFF == 0xA0]
    if not found:
        raise ValueError("expected a load of the constant 0xa0, found none")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0xA1, "constant 0xa0 changed to 0xa1"


CONTRACT = Contract(setup, control)
