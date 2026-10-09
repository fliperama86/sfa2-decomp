"""Contract of func_801189c4, the game's main, which never returns.

Choices of the setup (the contract itself is in the header of func_801189c4.c):
  - Every callee is a recorder (CallLog.replace); no game or library code runs.
  - The log copies at every call: all of game_state, scratchpad words 0 to 0x18,
    and the words of data_801ac310, data_801903b0 and data_801ac620.
  - The run ends by `ends_run_at` on func_80157784, the first call of an
    iteration of the frame loop: at its K-th call, K from 2 to 5, so that 1 to
    4 whole iterations run before it.
  - Interrupts are stood in for by the recorders:
    * data_801ac310 is the low halfword of the word at its address (the other
      halfword, data_801ac310 + 2, has no reader in the resident image and is
      random). `counts` on func_80157d9c adds 1 to the word at every call, so
      the halfword counts one per turn of the wait loop; it never carries into
      the other halfword (main clears it every iteration and a case has fewer
      than 65,535 turns).
    * in 2 cases of 5 the wait loop is entered (data_801abf0c 0, data_801ac314
      0, data_801abef8 from 1 to 6, which is the number of turns it takes);
      elsewhere it is skipped: data_801abf0c is not 0 or data_801abef8 is 0.
    * in 2 cases of 5 scratchpad word 4 is set to 1 at the N-th call of
      func_80157784 (N from 1 to K-1), and in half of those cleared again at
      the next call, so that the restart branch runs once or on every frame
      from N on.
  - func_8014f0bc says 0 zero to three times, then 1.
  - func_80118fc8 (two calls per iteration when game_state.field_225 is 0)
    says 0 in three calls of four, a random non-zero value otherwise, in a
    list of 12 whose last value stays. game_state.field_225 is 0 in 4 cases
    of 5.
  - func_80157d9c says 1, 2, 5, 0, -1 or an extreme value, per call.
  - data_801ac620 is random, with the values around 0x4000 and the multiples
    of 0x20 chosen often.
  - game_state and the other watched blocks are filled with random bytes;
    data_801ac314 is 0 or random non-zero.
  - Not generated: data_801ac314 not 0 with the wait loop entered. No
    recorder is called inside the loop in that case, so nothing counts
    data_801ac310 up and the original does not leave it.
"""

from __future__ import annotations

from contracts import CallLog, Contract, Setup

GAME_STATE_WORDS = 0x364 // 4
CLEARED_WORDS = 7


def setup(state, rng, sym) -> Setup:
    def fill(address, size):
        state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))

    game_state = sym["game_state"]
    fill(game_state, 0x364)
    state.w8(game_state + 0x225, 0 if rng.random() < 0.8 else rng.randrange(1, 256))
    for name in ("data_801ac310", "data_801903b0", "data_801ac620"):
        fill(sym[name], 4)
    fill(sym["scratch_word_00"], 4 * CLEARED_WORDS)

    wait_case = rng.random() < 0.4
    if wait_case:
        state.w16(sym["data_801abf0c"], 0)
        state.w16(sym["data_801abef8"], rng.randrange(1, 7))
        state.w16(sym["data_801ac314"], 0)
    else:
        if rng.random() < 0.5:
            state.w16(sym["data_801abf0c"], rng.randrange(1, 0x10000))
            state.w16(sym["data_801abef8"], rng.getrandbits(16))
        else:
            state.w16(sym["data_801abf0c"], 0 if rng.random() < 0.5 else rng.randrange(1, 0x10000))
            state.w16(sym["data_801abef8"], 0)
        state.w16(sym["data_801ac314"], 0 if rng.random() < 0.5 else rng.randrange(1, 0x10000))

    pick = rng.random()
    if pick < 0.25:
        counter = rng.randrange(0x4001)
    elif pick < 0.45:
        counter = rng.choice((0x4000, 0x4001, 0x4020, 0x3FFF, 0x4002))
    elif pick < 0.7:
        counter = rng.randrange(0x4001, 0x10000)
    else:
        counter = (rng.randrange(0x800) * 0x20 + rng.choice((0, 0, 0, 1, 0x10, 0x1F))) & 0xFFFF
    state.w16(sym["data_801ac620"], counter)

    pads = tuple(0 if rng.random() < 0.75 else rng.choice((1, 2, 0x100, rng.randrange(1, 1 << 32)))
                 for _ in range(12))
    waits = rng.randrange(0, 4)

    log = CallLog(state, words=60000, watch=(
        (game_state, GAME_STATE_WORDS),
        (sym["scratch_word_00"], CLEARED_WORDS),
        (sym["data_801ac310"], 1),
        (sym["data_801903b0"], 1),
        (sym["data_801ac620"], 1),
    ))
    wait_results = tuple(rng.choice((1, 2, 5, 0, -1, 0x7FFFFFFF, -0x80000000)) for _ in range(5))
    resets_at = rng.randrange(2, 6)
    stores = ()
    if rng.random() < 0.4:
        first = rng.randrange(1, resets_at)
        stores = ((first, sym["scratch_word_04"], 1),)
        if rng.random() < 0.5:
            stores += ((first + 1, sym["scratch_word_04"], 0),)
    log.replace(sym["func_80118900"], 0)
    log.replace(sym["func_8015efc0"], 0)
    log.replace(sym["func_8015f0b4"], 0)
    log.replace(sym["func_8014f0bc"], 0, results=(0,) * waits + (1,))
    log.replace(sym["func_80150cd0"], 1)
    log.replace(sym["func_80155c90"], 1)
    log.replace(sym["func_80118d10"], 2)
    log.replace(sym["func_80118e58"], 4)
    log.replace(sym["func_80119030"], 0)
    log.replace(sym["func_80119144"], 2)
    log.replace(sym["func_80157784"], 1, ends_run_at=resets_at, stores=stores)
    log.replace(sym["func_80118fc8"], 2, results=pads)
    log.replace(sym["func_80120408"], 0)
    log.replace(sym["func_8014f4d4"], 2)
    log.replace(sym["func_80157d00"], 1)
    log.replace(sym["func_80119340"], 1)
    log.replace(sym["func_8011907c"], 0)
    log.replace(sym["func_8014f59c"], 0)
    log.replace(sym["func_80151a04"], 0)
    log.replace(sym["func_801576e8"], 1, result=rng.getrandbits(32))
    log.replace(sym["func_80157d9c"], 1, results=wait_results,
                counts=(sym["data_801ac310"],))
    log.replace(sym["func_801578fc"], 1)
    log.replace(sym["func_801194f4"], 0)
    log.replace(sym["func_8011948c"], 0)
    return Setup(args=(), returns_value=False, returns=False)


def control(words):
    """Alter the constant stored in game_state.field_1e: 0x1c3 becomes 0x1c4.

    The build loads it with an `ori` or `addiu` from zero; the store of it
    is made on every run, before the call that takes &game_state, so the
    first watched copy of game_state in the log differs.
    """
    found = [i for i, w in enumerate(words)
             if w >> 26 in (0x09, 0x0D) and (w >> 21) & 31 == 0 and w & 0xFFFF == 0x1C3]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x1c3, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x1C4, "game_state.field_1e constant 0x1c3 changed to 0x1c4"


CONTRACT = Contract(setup, control)
