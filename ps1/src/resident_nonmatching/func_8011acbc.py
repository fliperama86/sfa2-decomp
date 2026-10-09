"""Contract of func_8011acbc (no argument, no result).

Choices of the setup (details in the header of func_8011acbc.c):
  - the 40 records of data_801a89f4 are filled with random bytes; then per
    record field_00 is 0 in one case of seven, field_01 is 1 in half of the
    cases, 0 in one of five, else random; field_02 is drawn from the five
    special values 3, 0xb, 0x14, 0x27, 0x6d, from 0x16 and 0x18 and from
    random bytes (each group often); field_08 is 0x20 in two cases of five;
    field_81 is 4 in two cases of five;
  - each of the four part pointers of a record is null in one case of four,
    else a block of 0xac random bytes whose field_01 is 0 in one case of
    three;
  - game_state.field_40 is drawn from 0 to 20 (19 and 20 call no handler),
    data_80190568 is 0 in one case of five;
  - the counter data_8019045c is watched at every call and every callee
    records the 0xac bytes behind its argument;
  - the 19 handlers (func_801e9080 ... func_801e9114), func_8011cd68,
    func_80134624, func_801452ec and func_8011bc84 are recorders of one
    argument that return 0.
"""

from contracts import CallLog, Contract, Setup

HANDLERS = (
    "func_801e9080", "func_801e9f90", "func_801e99c8", "func_801e9d04", "func_801e99b4",
    "func_801e8dc8", "func_801e9798", "func_801e9970", "func_801e9b54", "func_801e9840",
    "func_801e8df0", "func_801e96fc", "func_801ea640", "func_801e9f20", "func_801e9d14",
    "func_801e9bb0", "func_801e9ef4", "func_801e8fec", "func_801e9114",
)
OTHERS = ("func_8011cd68", "func_80134624", "func_801452ec", "func_8011bc84")
TYPES = (3, 0xB, 0x14, 0x27, 0x6D, 0x16, 0x18)


def setup(state, rng, sym):
    table = sym["data_801a89f4"]
    for index in range(40):
        rec = table + 172 * index
        for k in range(0, 172, 4):
            state.w32(rec + k, rng.getrandbits(32))
        state.w8(rec + 0, 0 if rng.random() < 1 / 7 else rng.randrange(1, 256))
        r = rng.random()
        state.w8(rec + 1, 1 if r < 0.5 else 0 if r < 0.7 else rng.randrange(256))
        state.w8(rec + 2, rng.choice(TYPES) if rng.random() < 0.8 else rng.randrange(256))
        if rng.random() < 0.4:
            state.w8(rec + 8, 0x20)
        if rng.random() < 0.4:
            state.w8(rec + 0x81, 4)
        for offset in (0x28, 0x2C, 0x30, 0x34):
            if rng.random() < 0.25:
                state.w32(rec + offset, 0)
            else:
                part = state.alloc(0xAC)
                for k in range(0, 0xAC, 4):
                    state.w32(part + k, rng.getrandbits(32))
                if rng.random() < 0.33:
                    state.w8(part + 1, 0)
                state.w32(rec + offset, part)
    state.w16(sym["game_state"] + 0x40, rng.randrange(21))
    state.w8(sym["data_80190568"], 0 if rng.random() < 0.2 else rng.randrange(1, 256))
    state.w8(sym["data_8019045c"], rng.randrange(256))
    # The counter is watched at every call; every callee records the 0xac
    # bytes of the block it is given (a record or a part whose field_09 the
    # function has just set).
    log = CallLog(state, words=12000, watch=((sym["data_8019045c"], 1),))
    for name in HANDLERS + OTHERS:
        log.replace(sym[name], 1, 0, pointees={0: 0xAC // 4})
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the loop limit: the compare with 40 becomes a compare with 41."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0B and w & 0xFFFF == 40]
    if len(found) != 1:
        raise ValueError(f"expected one sltiu with 40, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 41, "loop limit 41"


CONTRACT = Contract(setup, control)
