"""Contract of func_800e0a1c_slot0f: a save-file load from the memory card.

Choices of the setup:
  - a0 (mode) is 0 in half the cases, else one of 0x10, 1 and a random
    word (equal odds);
  - the 16 words from data_8018fef8 hold random bytes (the function
    overwrites 6 bytes at offset 0x1c); the words data_800df0f0..0fc are
    the module's own data and are not changed;
  - data_800e8504_slot0f points at a small block of the setup;
  - func_800e0da8_slot0f answers one of 1, 4, 3, 0, 2 and a random word,
    each with probability 1/6;
  - func_800e11e4_slot0f answers -1 in a quarter of the cases, else a
    whole word other than -1 (0, 1, -2, 0xffff, 0xff, 0x80000000 and other
    near misses of a test against -1, or a random word); func_800e1250_slot0f
    answers one of -1, -2, 0x80000000, a random negative word, 0, 1, 0x2000,
    0xffff, 0x7fffffff and a random non-negative one (equal odds), as the
    function tests it "below 0" as a whole word; close answers -1 in a
    quarter, else a whole word other than -1 as for the open;
  - strcat and func_800e1348_slot0f answer 0;
  - the log watches 16 words from data_8018fef8.
"""
from contracts import CallLog, Contract, Setup, fill


def not_minus_one(rng):
    """A whole word other than -1; the near misses of a test against -1 (a
    low byte or halfword of ones, other negative values, 0) are drawn often."""
    return rng.choice((0, 1, 0xFFFFFFFE, 0xFFFF, 0xFFFF0000, 0xFF, 0xFFFFFF00, 0x80000000, 0x7FFFFFFF,
                       rng.randrange(2, 1 << 30), rng.getrandbits(32) & 0xFFFFFFFE))


def setup(state, rng, sym):
    base = sym["data_8018fef8"]
    fill(state, base, 64, rng)
    block = state.alloc(64)
    state.w32(sym["data_800e8504_slot0f"], block)
    mode = 0 if rng.random() < 0.5 else rng.choice((0x10, 1, rng.getrandbits(32)))
    log = CallLog(state, 512, watch=((base, 16),))
    log.replace(sym["strcat"], 2, 0, pointees={1: 6})
    log.replace(sym["func_800e0da8_slot0f"], 1, rng.choice((1, 4, 3, 0, 2, rng.getrandbits(32))))
    # The recorders of the retry loops answer whole words, the way the
    # callees leave them in the result register.
    log.replace(sym["func_800e11e4_slot0f"], 2,
                0xFFFFFFFF if rng.random() < 0.25 else not_minus_one(rng))
    # The read result is tested "below 0" as a whole word: -1, other negative
    # words, 0 and positive words are all drawn.
    read = rng.choice((0xFFFFFFFF, 0xFFFFFFFE, 0x80000000, rng.getrandbits(31) | 0x80000000,
                       0, 1, 0x2000, 0xFFFF, 0x7FFFFFFF, rng.getrandbits(31)))
    log.replace(sym["func_800e1250_slot0f"], 3, read)
    log.replace(sym["close"], 1,
                0xFFFFFFFF if rng.random() < 0.25 else not_minus_one(rng))
    log.replace(sym["func_800e1348_slot0f"], 0, 0)
    return Setup(args=(mode,), returns_value=True)


def control(words):
    """Alter the result for the card answer 3: `ori v0,zero,2` at the
    return of that arm becomes 3 (the last such word of the function)."""
    found = [i for i, w in enumerate(words) if w == 0x34020002]
    if len(found) < 1:
        raise ValueError("expected a load of 2 into v0")
    i = found[-1]
    return i, words[i] + 1, "a returned 2 becomes 3"


CONTRACT = Contract(setup, control)
