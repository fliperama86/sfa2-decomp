"""Contract of func_801545cc (see the header comment of the .c file).

Choices made here:
  - game_state.field_30 is 0 in three cases of four (the arm with the
    callees), otherwise random;
  - game_state.field_31 is 0, 1 or 2 about equally, in one case in six
    random;
  - the halfwords field_c2 of both players are random with bit 0x100 set
    in half the cases;
  - the HUD block that data_8018f5a0 points at is a block of its own with
    random content (its halfword at 0x52 gets all values, including 0xffff);
  - the 16 ids read from table_8016e664 + 0x10 are random bytes, a half of
    them taken from 0x93 to 0x98 (the borders of the special range);
  - the tables the function writes (the bytes from data_80181094 to the end
    of data_8018128a's eight 5-byte entries, 0x801812b2) and the cursor bytes
    data_8018d250 to data_8018d264 get random content first; so do the two
    bytes game_state + 0x32 and + 0x33, which the recorder of func_801519b4
    overwrites with its stored word;
  - the log watches the hud block (25 words), data_8018d250 (6 words) and
    the tables from data_80181094 (136 words); no pointer argument of a
    recorded callee points at memory this function fills (the blocks passed
    to func_801519b4 are in the image and not written here);
  - func_801519b4 (one argument) and func_80120554 (three arguments) are
    recorders; their results are not used.
"""

from contracts import CallLog, Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym):
    game_state = sym["game_state"]
    state.w8(game_state + 0x30, 0 if rng.random() < 0.75 else rng.getrandbits(8))
    state.w8(game_state + 0x31, rng.choice((0, 1, 2, rng.getrandbits(8) if rng.random() < 0.5 else 1)))
    for player in ("player_left", "player_right"):
        value = rng.getrandbits(16)
        value = (value | 0x100) if rng.random() < 0.5 else (value & ~0x100)
        state.w16(sym[player] + 0xC2, value)

    hud = state.alloc(0x64)
    fill(state, hud, 0x64, rng)
    if rng.random() < 0.1:
        state.w16(hud + 0x52, 0xFFFF)
    state.w32(sym["data_8018f5a0"], hud)

    ids = sym["table_8016e664"] + 0x10
    for index in range(16):
        state.w8(ids + index, rng.randrange(0x93, 0x99) if rng.random() < 0.5 else rng.getrandbits(8))

    fill(state, sym["data_80181094"], 0x12B2 - 0x1094, rng)
    state.w16(game_state + 0x32, rng.getrandbits(16))
    fill(state, sym["data_8018d250"], 0x18, rng)

    # The hud block, the cursor bytes and the tables are watched: the function
    # writes them, and a callee could read them.
    log = CallLog(state, watch=((hud, 0x64 // 4), (sym["data_8018d250"], 6),
                                (sym["data_80181094"] & ~3, (0x12B4 - 0x1094) // 4)))
    # The first callee may change game_state.field_31 (inferred): in half of the
    # cases the recorder stores a new word at game_state + 0x30 on its first call,
    # keeping field_30 (0) and giving field_31 a new value, so that the reread
    # after the call is tested.
    stores = ()
    if rng.random() < 0.5:
        stores = ((1, game_state + 0x30, rng.choice((1, 2, rng.getrandbits(8))) << 8 | rng.getrandbits(16) << 16),)
    log.replace(sym["func_801519b4"], 1, rng.getrandbits(32), stores=stores)
    log.replace(sym["func_80120554"], 3, rng.getrandbits(32))
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the store of 0x1b into the first of the cursor tables' loops."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0D and w & 0xFFFF == 0x1B]
    if not found:
        raise ValueError("no ori with 0x1b")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0x1C, "the constant 0x1b of the cursor loop becomes 0x1c"


CONTRACT = Contract(setup, control)
