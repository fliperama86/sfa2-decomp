"""Contract of func_800df654_slot0f: one step of a count-up animation.

Choices of the setup:
  - a0 is a GameState block of 0x364 random bytes; a HudState block of
    0x64 random bytes is made and data_8018f5a0 points at it;
  - field_c4 is 1 in two cases of three (the step runs), else random;
  - field_c2 is chosen among 0, 1, 0x1e, 0x1f, 0x20, 0x7fff, 0x8000, 0xffff
    and random, so that the incremented frame is below 0x20, at 0x20, or
    negative as a signed value;
  - func_80125f5c (4 arguments, the third under mask 0xff) and
    func_80137220 (2 arguments) are recorders returning 0;
  - the log watches the whole GameState and HudState blocks.
"""
from contracts import CallLog, Contract, Setup, fill, halfword

STATE_WORDS = 0x364 // 4
HUD_WORDS = 0x64 // 4


def setup(state, rng, sym):
    game = state.alloc(0x364)
    fill(state, game, 0x364, rng)
    hud = state.alloc(0x64)
    fill(state, hud, 0x64, rng)
    state.w32(sym["data_8018f5a0"], hud)
    state.w16(game + 0xC4, 1 if rng.random() < 0.67 else halfword(rng))
    state.w16(game + 0xC2, rng.choice((0, 1, 0x1E, 0x1F, 0x20, 0x7FFF, 0x8000, 0xFFFF, halfword(rng))))
    log = CallLog(state, 1024, watch=((game, STATE_WORDS), (hud, HUD_WORDS)))
    log.replace(sym["func_80125f5c"], 4, 0, masks={2: 0xFF})
    log.replace(sym["func_80137220"], 2, 0)
    return Setup(args=(game,), returns_value=False)


def control(words):
    """Alter the constant stored into field_c4 on reload: 2 becomes 3.

    The store is `sh reg, 0xc4(s0)` whose register was set by `ori v0,zero,2`;
    the word altered is that `ori`.
    """
    found = [i for i, w in enumerate(words) if w == 0x34020002]
    if len(found) != 1:
        raise ValueError(f"expected one load of 2 into v0, found {len(found)}")
    i = found[0]
    return i, words[i] + 1, "reload value of the tick counter 2 becomes 3"


CONTRACT = Contract(setup, control)
