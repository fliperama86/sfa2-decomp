"""Contract of func_80010840_slot27: the end-of-step handler that acts when the hud's field_52 matches.

Reads and writes are listed in the header of func_80010840_slot27.c. Choices of the setup:
  - game_state and the hud block are filled with random bytes;
  - mode and field_07 are chosen so that their OR is 3 in two cases of five (from
    pairs that make 3), otherwise random;
  - the hud's field_52 equals mode | field_07 in four cases of five, otherwise random;
  - field_b0 is 0 in one case of four; field_b1 has bit 7 set in one case of three;
    field_b2 is 0, 10, 11 or random in equal shares;
  - the recorder of func_801257f4 stands for a callee that rewrites the words
    of game_state holding mode and field_07 in half of the cases, with a new
    pair that makes the OR 3 in half of those (so the second test after the
    call sees a changed value, equal or not to 3); field_b0 and the words
    around are left to the random fill;
  - the log watches the whole game state, the whole hud block and
    data_8002f158_slot27, and has room for the one call.
"""
from contracts import CallLog, Contract, Setup, fill

GAME_STATE_SIZE = 0x364
HUD_SIZE = 0x64


def setup(state, rng, sym):
    gs = sym["game_state"]
    fill(state, gs, GAME_STATE_SIZE, rng)
    if rng.random() < 0.4:
        mode, f07 = rng.choice(((3, 0), (0, 3), (1, 2), (2, 1), (3, 1), (3, 3), (2, 3)))
    else:
        mode, f07 = rng.randrange(256), rng.randrange(256)
    state.w8(gs + 0x1B, mode)
    state.w8(gs + 0x07, f07)
    state.w8(gs + 0xB0, 0 if rng.random() < 0.25 else rng.randrange(1, 256))
    state.w8(gs + 0xB1, rng.randrange(128, 256) if rng.random() < 1 / 3 else rng.randrange(128))
    state.w8(gs + 0xB2, rng.choice((0, 10, 11, rng.randrange(256))))

    hud = state.alloc(HUD_SIZE)
    fill(state, hud, HUD_SIZE, rng)
    state.w16(hud + 0x52, (mode | f07) if rng.random() < 0.8 else rng.getrandbits(16))
    state.w32(sym["data_8018f5a0"], hud)
    flag = sym["data_8002f158_slot27"]
    state.w8(flag, rng.randrange(1, 256))

    log = CallLog(state, 4 + GAME_STATE_SIZE // 4 + HUD_SIZE // 4 + 1 + 8,
                  watch=((gs, GAME_STATE_SIZE // 4), (hud, HUD_SIZE // 4), (flag & ~3, 1)))
    if rng.random() < 0.5:
        if rng.random() < 0.5:
            mode2, f072 = rng.choice(((3, 0), (0, 3), (1, 2), (2, 1)))
        else:
            mode2, f072 = rng.randrange(256), rng.randrange(256)
        rest = rng.getrandbits(24)
        stores = ((1, gs + 0x18, mode2 << 24 | rest), (1, gs + 0x04, f072 << 24 | rng.getrandbits(24)))
    else:
        stores = ()
    log.replace(sym["func_801257f4"], 1, stores=stores)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the store of the constant 1 into game_state.field_09.

    The store is `sb v0, 0x111(at)`; moved to 0x112 it leaves field_09 alone.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0x111]
    if len(found) != 1:
        raise ValueError(f"expected one store to field_09, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x112, "field_09 store moved by one byte"


CONTRACT = Contract(setup, control)
