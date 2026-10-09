"""Contract of func_801b3f38_slot04_sel: the update of a two-player selection screen.

Reads and writes are listed in the header comment of
func_801b3f38_slot04_sel.c. Choices made here:
  - the handler table has four entries, each a recorder of its own; the
    records' first bytes are random in 0 to 3;
  - the records are 0x2a bytes of random content (first bytes as above);
    field_0d of each is 0 in half of the cases, random otherwise;
  - game_state.field_07 is 0 in a quarter of the cases, else random; the
    mode byte random;
  - the two comparison bytes data_801b9d68_slot04_sel and
    data_801b9d6c_slot04_sel equal game_state.field_07 and mode in half of
    the cases each, else random;
  - data_8018f5a0 points to a hud block of 0x64 bytes of random content;
  - func_801519b4 is a recorder; the log watches the records, the hud block
    and the comparison bytes.
"""

from contracts import CallLog, Contract, Setup, fill, halfword  # noqa: F401


def setup(state, rng, sym) -> Setup:
    game_state = sym["game_state"]
    state.w8(game_state + 0x07, 0 if rng.random() < 0.25 else rng.randrange(1, 256))
    state.w8(game_state + 0x1B, rng.randrange(256))

    records = sym["data_801b9d38_slot04_sel"]
    fill(state, records, 0x2A, rng)
    state.w8(records, rng.randrange(4))
    state.w8(records + 0x15, rng.randrange(4))
    for offset in (0x0D, 0x15 + 0x0D):
        state.w8(records + offset, 0 if rng.random() < 0.5 else rng.randrange(1, 256))

    for name, value in (("data_801b9d68_slot04_sel", state.read(game_state + 0x07, 1)[0]),
                        ("data_801b9d6c_slot04_sel", state.read(game_state + 0x1B, 1)[0])):
        state.w8(sym[name], value if rng.random() < 0.5 else rng.randrange(256))

    hud = state.alloc(0x64)
    fill(state, hud, 0x64, rng)
    state.w32(sym["data_8018f5a0"], hud)

    log = CallLog(state, 400, watch=((records, 11), (hud, 0x64 // 4), (sym["data_801b9d68_slot04_sel"], 1)))
    table = sym["data_801b7df0_slot04_sel"]
    for index in range(4):
        entry = state.alloc(16)
        state.w32(table + 4 * index, entry)
        log.replace(entry, 2, 0)
    log.replace(sym["func_801519b4"], 1, 0)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the increment of the hud state's field_4e: the build adds 2.

    The function has one `addiu reg, reg, 1` after loading the halfword at
    0x4e of the hud state; the control changes the 1 to 2.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x25 and w & 0xFFFF == 0x4E]
    if len(found) != 1:
        raise ValueError(f"expected one load of the halfword at 0x4e, found {len(found)}")
    index = found[0]
    for follow in range(index + 1, min(index + 6, len(words))):
        if words[follow] >> 26 == 0x09 and words[follow] & 0xFFFF == 1:
            return follow, (words[follow] & ~0xFFFF) | 2, "increment of field_4e changed to 2"
    raise ValueError("no increment after the load of field_4e")


CONTRACT = Contract(setup, control)
