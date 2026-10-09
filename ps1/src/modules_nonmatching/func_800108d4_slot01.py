"""Contract of func_800108d4_slot01 as code.

No argument. game_state.field_78 points at the object whose panel is drawn.

Reads and writes are listed in the header comment of func_800108d4_slot01.c.
Choices made here:
  - the object is 0x394 random bytes; field_cd is 0 in four cases of five;
    field_65 is 0 in half the cases; field_cc is 0 in half the cases;
    field_ce is 0 to 3; game_state.field_85 is 0 in half the cases;
  - data_801a27d0 is 0 or 1;
  - the eight text records of the module image, the queue table_8018d144,
    the arrays data_80190014 and strips and the ordering table head words
    are filled with random bytes;
  - game_state.field_74 is 0 in half the cases (func_801519b4 queues only
    then); the queue counter data_8018d204 is 0 to 0x35, with the values
    near its limit 0x30 in half the cases;
  - func_80010d0c_slot01 and func_80010bf0_slot01 are recorders (two
    arguments each, result 0); the log copies the eight records and the four
    words data_8002ce9c_slot01 to data_8002cea8_slot01 at every call;
  - func_801519b4 runs as the original code.
"""

from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym) -> Setup:
    game_state = sym["game_state"]
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0xCD, 0 if rng.random() < 0.8 else rng.randrange(1, 256))
    state.w8(obj + 0x65, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    state.w8(obj + 0xCC, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    state.w8(obj + 0xCE, rng.randrange(4))
    state.w32(game_state + 0x78, obj)
    state.w8(game_state + 0x85, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    state.w8(game_state + 0x74, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    state.w32(sym["data_801a27d0"], rng.randrange(2))

    records = sym["data_800150cc_slot01"]
    fill(state, records, 8 * 16, rng)
    fill(state, sym["table_8018d144"], 0x30 * 4, rng)
    state.w16(sym["data_8018d204"], rng.choice((rng.randrange(0x36), rng.randrange(0x2C, 0x36))))
    fill(state, sym["data_80190014"], 2 * 4 * 0x1C, rng)
    fill(state, sym["strips"], 4 * 4 * 0x1C, rng)
    fill(state, sym["data_801fc050"], 0x1E0, rng)

    log = CallLog(state, 1024, watch=((records, 8 * 16 // 4), (sym["data_8002ce9c_slot01"], 4)))
    log.replace(sym["func_80010d0c_slot01"], 2, 0)
    log.replace(sym["func_80010bf0_slot01"], 2, 0)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the first load of the constant 0xe0 (the left position): it becomes 0xe1."""
    found = [i for i, w in enumerate(words) if w >> 26 in (0x0D, 0x09) and w & 0xFFFF == 0xE0]
    if not found:
        raise ValueError("expected a load of the constant 0xe0, found none")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0xE1, "constant 0xe0 changed to 0xe1"


CONTRACT = Contract(setup, control)
