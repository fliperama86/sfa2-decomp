"""Contract of func_801b0ea0_slot04_sel: the update of a two-player selection screen.

Reads and writes are listed in the header comment of
func_801b0ea0_slot04_sel.c. Choices made here:
  - the side bytes of the two players are 0 or 1 in three cases of four,
    else any byte (the tables are read at that index);
  - the handler table has four entries, each a recorder of its own; the
    records' first bytes are random in 0 to 3;
  - the records are 0x32 bytes of random content; game_state.field_07 is
    0 in a fifth of the cases, else random; in half of the cases the
    records' field_09 are chosen so that their OR equals it; field_0b of
    each is 0 in half of the cases;
  - the two comparison bytes equal game_state.field_07 and mode in three
    cases of four each, else random;
  - data_8018f5a0 points to a hud block of 0x64 bytes of random content;
  - the 16 bytes at data_801b7978_slot04_sel are random;
  - func_801519b4 is a recorder; the log watches the records, the hud block
    and the text block.
"""

from contracts import CallLog, Contract, Setup, fill, halfword  # noqa: F401


def setup(state, rng, sym) -> Setup:
    game_state = sym["game_state"]
    field_07 = 0 if rng.random() < 0.2 else rng.randrange(1, 256)
    mode = rng.randrange(256)
    state.w8(game_state + 0x07, field_07)
    state.w8(game_state + 0x1B, mode)

    for player in ("player_left", "player_right"):
        state.w8(sym[player] + 0xA6, rng.randrange(2) if rng.random() < 0.75 else rng.randrange(256))

    records = sym["data_801b9cf0_slot04_sel"]
    fill(state, records, 0x32, rng)
    state.w8(records, rng.randrange(4))
    state.w8(records + 0x19, rng.randrange(4))
    if rng.random() < 0.5:
        first = field_07 & rng.getrandbits(8)
        state.w8(records + 0x09, first)
        state.w8(records + 0x19 + 0x09, field_07 & ~first if rng.random() < 0.5 else field_07)
    for offset in (0x0B, 0x19 + 0x0B):
        state.w8(records + offset, 0 if rng.random() < 0.5 else rng.randrange(1, 256))

    state.w8(sym["data_801b9d28_slot04_sel"], field_07 if rng.random() < 0.75 else rng.randrange(256))
    state.w8(sym["data_801b9d2c_slot04_sel"], mode if rng.random() < 0.75 else rng.randrange(256))

    hud = state.alloc(0x64)
    fill(state, hud, 0x64, rng)
    state.w32(sym["data_8018f5a0"], hud)
    fill(state, sym["data_801b7978_slot04_sel"], 16, rng)

    log = CallLog(state, 600, watch=((records, 13), (hud, 0x64 // 4), (sym["data_801b7978_slot04_sel"], 4)))
    table = sym["data_801b6e70_slot04_sel"]
    for index in range(4):
        entry = state.alloc(16)
        state.w32(table + 4 * index, entry)
        log.replace(entry, 2, 0)
    log.replace(sym["func_801519b4"], 1, 0)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the store of 1 into the hud state's field_50: the build stores 2.

    The function has one `ori reg, zero, 1` whose value is then stored as
    the halfword at 0x50 of the hud state, in the last test's arm.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0D and w >> 21 & 31 == 0 and w & 0xFFFF == 1]
    if len(found) != 1:
        raise ValueError(f"expected one load of 1, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 2, "field_50 set to 2 instead of 1"


CONTRACT = Contract(setup, control)
