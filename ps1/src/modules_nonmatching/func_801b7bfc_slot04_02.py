"""Contract setup of func_801b7bfc_slot04_02 (see the header of the .c file).

Choices of the setup:
  - a0 and the object at field_3c are distinct 0x394-byte blocks filled with
    random bytes;
  - game_state.field_64 is 0 in one case of four (the idle arm);
  - the other object's field_5c is 0x90 in half of the cases, its side is
    random, and field_03 of the first object is 0, 0xff or random so that the
    palette argument is 0 both by itself and by the wrap of the increment;
  - box_margin and data_801aa5ea are random halfwords (x wraps; the
    registration test in func_8011ffdc takes both arms);
  - the scratchpad words 0x1f8000b4 and 0x1f800164 point to two tables of 16
    pointers to sequence steps; each step has random fields and a frame index
    below 32; the object's `frames` pointer (field 0x8c) is a block of 32
    frame records and the object's field_08 is 0, 8 or random (the arms of
    func_80130768 and of func_80120028);
  - the stacks that func_80120080 and its helpers push on hold 32 words and
    the stack pointers start at their last word; their counters are random;
  - func_80137220 is a recorder (2 arguments) that returns a random word;
  - the recorder copies the whole object (0x394 bytes) and the palette array
    data_801a2bc4 (16 halfwords) into its entry at its call, so the stores of
    the function before func_801b7e34 are seen in order. Its arguments are
    plain values, so no pointee.
"""

from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    log = CallLog(state, watch=((obj, 0x394 // 4), (sym["data_801a2bc4"], 8)))
    log.replace(sym["func_80137220"], 2, rng.getrandbits(32))

    game_state = sym["game_state"]
    state.w8(game_state + 0x64, 0 if rng.random() < 0.25 else rng.randrange(1, 256))
    state.w16(sym["box_margin"], rng.getrandbits(16))
    state.w16(sym["data_801aa5ea"], rng.getrandbits(16))

    for stack in ("stack_801ad354", "stack_8018db00", "stack_8018db04"):
        block = state.alloc(4 * 32)
        state.w32(sym[stack], block + 4 * 31)
    for counter in ("count_8018f59c", "count_80190100", "count_80190104"):
        state.w16(sym[counter], rng.getrandbits(16))

    frames = state.alloc(16 * 32)
    fill(state, frames, 16 * 32, rng)

    for scratch in ("data_1f8000b4", "data_1f800164"):
        table = state.alloc(4 * 16)
        for index in range(16):
            step = state.alloc(12)
            fill(state, step, 12, rng)
            state.w16(step + 10, rng.randrange(32))
            state.w32(table + 4 * index, step)
        state.w32(sym[scratch], table)

    other = state.alloc(0x394)
    fill(state, other, 0x394, rng)
    state.w16(other + 0x5C, 0x90 if rng.random() < 0.5 else rng.getrandbits(16))
    state.w8(other + 0xA6, rng.randrange(2) if rng.random() < 0.9 else rng.randrange(256))

    fill(state, obj, 0x394, rng)
    state.w32(obj + 0x3C, other)
    state.w32(obj + 0x8C, frames)
    state.w8(obj + 0x03, rng.choice((0, 0xFF, rng.randrange(256))))
    state.w8(obj + 0x08, rng.choice((0, 8, rng.randrange(256))))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Change the constant of the store to field_46 (`ori v0,zero,0x15`).

    Every active-arm call executes it; the halfword stored to field_46 then
    differs.
    """
    found = [i for i, w in enumerate(words) if w == 0x34020015]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x15, found {len(found)}")
    return found[0], 0x34020016, "constant stored in field_46 changed from 0x15 to 0x16"


CONTRACT = Contract(setup, control)
