"""Contract of func_80120cf0: a0 = &game_state, no result.

Choices made here:
  - a0 is &game_state in every case (see the header comment: the original
    reads a0, the C uses the global, so another pointer is outside the
    contract); game_state (0x364
    bytes) and the two players (2 * 0x394 bytes) start with random bytes,
    the 0x2b8 and 0x2ba halfwords of game_state included;
  - data_8018f5a0 points at a 0x60-byte random block;
  - the bytes read from the tables (table_8016e664 offsets 0x25, 0x26, 0x28
    and data_8016e685, 87, 88) and the data bytes and halfwords are random;
    data_8016e688 is 0 in half of the cases, so that both arms of the
    field_13 choice are tried;
  - game_state.field_17 is chosen so that its bits 0 and 1 are all four
    combinations, equally often;
  - every recorder copies, at every call, game_state (0x364 bytes), both
    players, the status block and the words holding data_801a6938 and
    data_801a6984 and data_801a6985;
  - the seed word data_80190126 of func_80151184, the counters counter_a,
    counter_b and counter_c that func_80138358 sets, and the 256 bytes of
    table_6cf0 that it reads are random (the seed and the counters are
    written by callees that run as original code; the C never names them);
  - func_8011eb14 and func_80120374 are recorders (0 and 1 arguments, both
    returning 0); the log shows the value passed to func_80120374.
"""

from contracts import CallLog, Contract, Setup


def fill(state, address, size, rng):
    state.write(address, rng.randbytes(size))


def setup(state, rng, sym) -> Setup:
    game_state = sym["game_state"]
    fill(state, game_state, 0x364, rng)
    state.w8(game_state + 0x17, (rng.getrandbits(8) & 0xFC) | rng.randrange(4))
    fill(state, sym["player_left"], 2 * 0x394, rng)

    hud = state.alloc(0x60)
    fill(state, hud, 0x60, rng)
    state.w32(sym["data_8018f5a0"], hud)

    table = sym["table_8016e664"]
    for offset in (0x25, 0x26, 0x28):
        state.w8(table + offset, rng.getrandbits(8))
    state.w8(sym["data_8016e685"], rng.getrandbits(8))
    state.w8(sym["data_8016e687"], rng.getrandbits(8))
    state.w8(sym["data_8016e688"], 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    state.w8(sym["data_801a8067"], rng.getrandbits(8))
    state.w8(sym["data_801a83fb"], rng.getrandbits(8))
    state.w16(sym["data_801a6966"], rng.getrandbits(16))
    state.w16(sym["data_801a6972"], rng.getrandbits(16))
    for name in ("data_801a6938", "data_801a6984", "data_801a6985"):
        state.w8(sym[name], rng.getrandbits(8))
    # the seed word of func_80151184: the callee reads and rewrites it
    # (the C does not name it; the callee runs as original code)
    state.w16(sym["data_80190126"], rng.getrandbits(16))
    # func_80138358 (original code) writes the three counters and reads
    # table_6cf0 at an index that is a random byte: all 256 entries are random
    for name in ("counter_a", "counter_b", "counter_c"):
        state.w16(sym[name], rng.getrandbits(16))
    fill(state, sym["table_6cf0"], 256, rng)

    watch = (
        (game_state, 0x364 // 4),
        (sym["player_left"], 2 * 0x394 // 4),
        (hud, 0x60 // 4),
        (sym["data_801a6938"], 1),
        (sym["data_801a6984"], 1),
    )
    log = CallLog(state, words=4096, watch=watch)
    log.replace(sym["func_8011eb14"], 0, 0)
    log.replace(sym["func_80120374"], 1, 0)
    return Setup(args=(game_state,), returns_value=False)


def control(words):
    """Alter the mask of the random byte: 0x78 becomes 0x70."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0C and w & 0xFFFF == 0x78]
    if len(found) != 1:
        raise ValueError(f"expected one mask 0x78, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x70, "random byte masked with 0x70"


CONTRACT = Contract(setup, control)
