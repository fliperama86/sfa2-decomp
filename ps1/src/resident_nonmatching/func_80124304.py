"""Contract of func_80124304: no arguments, no return value.

Choices of the setup (reads and writes are listed in func_80124304.c):
  - game_state.field_17 is 0 in one case of six, else random non-zero;
  - each of the two pad records has a third byte (c) of 0 in one case of
    three, else random non-zero; the other bytes are random;
  - data_8016e800 is one of 1, 2, 3 (each one case of five), 4, 5, 6 (together
    one case of five) or random (one case of five); data_8016e801..803 and
    data_80185fbc, fc0, fc4 are random bytes;
  - data_8018f5a0 points at a random block of 0x64 bytes; the input block
    (data_801a6984, 0x2010 bytes), both players (0x394 bytes each), the
    game state start random, and data_801ae028 starts as a random byte;
  - the six callees are recorders, each copying at every call the game state
    (0x200 bytes), both players, the input block, the HUD block and its
    pointer, the pad records, data_8016e800, data_80185fbc..fc4 and
    data_801ae028 (see the header of the .c) returning 0.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    gs = sym["game_state"]
    fill(state, gs, 0x200, rng)
    state.w8(gs + 0x17, 0 if rng.random() < 1 / 6 else rng.randrange(1, 256))
    pads = sym["data_80185fac"]
    fill(state, pads, 6, rng)
    for k in (0, 1):
        if rng.random() < 1 / 3:
            state.w8(pads + 3 * k + 2, 0)
        else:
            state.w8(pads + 3 * k + 2, rng.randrange(1, 256))
    r = rng.random()
    mode = (1 if r < 0.2 else 2 if r < 0.4 else 3 if r < 0.6 else
            rng.choice((4, 5, 6)) if r < 0.8 else rng.getrandbits(8))
    state.w8(sym["data_8016e800"], mode)
    for name in ("data_8016e801", "data_8016e802", "data_8016e803",
                 "data_80185fbc", "data_80185fc0", "data_80185fc4"):
        state.w8(sym[name], rng.getrandbits(8))
    fill(state, sym["player_left"], 2 * 0x394, rng)
    fill(state, sym["data_801a6984"], 0x2010, rng)
    state.w8(sym["data_801ae028"], rng.getrandbits(8))
    hud = state.alloc(0x64)
    fill(state, hud, 0x64, rng)
    state.w32(sym["data_8018f5a0"], hud)
    watch = ((gs, 0x200 // 4), (sym["player_left"], 2 * 0x394 // 4), (sym["data_801a6984"], 0x2010 // 4),
             (hud, 0x64 // 4), (sym["data_8018f5a0"], 1), (sym["data_80185fac"], 2),
             (sym["data_8016e800"], 1), (sym["data_80185fbc"], 3), (sym["data_801ae028"], 1))
    log = CallLog(state, words=40000, watch=watch)
    for name, count in (("func_801519b4", 1), ("func_801246cc", 2), ("func_801246a4", 3),
                        ("func_8013788c", 1), ("func_80124a7c", 1), ("func_80124ae8", 1)):
        log.replace(sym[name], count, 0)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter `ori v0,zero,0x63` (game_state.field_49 value) to 0x62."""
    found = [i for i, w in enumerate(words) if w == 0x34020063]
    if not found:
        raise ValueError("expected a load of 0x63 into v0")
    i = found[0]
    return i, 0x34020062, "field_49 value 0x63 changed to 0x62"


CONTRACT = Contract(setup, control)
