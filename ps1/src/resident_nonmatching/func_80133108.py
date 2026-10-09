"""Contract of func_80133108 (the header comment of the .c lists the reads and writes).

Choices made here:
  - a0 is a block of 0x800 random bytes (the table of records), a1 and a2 are
    two blocks of 0x394 random bytes (the objects);
  - data_801a27d0 holds 0, 1 or 2 (the row index); the high half is zero;
  - each object's field_d8 is 0 in two cases of five and 1 or a random byte
    otherwise; field_165 is 0 in nine cases of ten (a non-zero value makes
    the first callee return at once);
  - field_c6 is steered: in the regions below 0x30, 0x30 to 0x5f, 0x60 to
    0x8f, 0x90 and above, negative, and random, so that every segment arm and
    every step of the state bytes is tried; with field_d8 set the value is
    chosen about a third of the target, so that the tripled value lands there;
  - the globals the function writes and the callees read are filled with
    random bytes before the call.
"""

from contracts import Contract, Setup


def _object(state, rng):
    obj = state.alloc(0x394)
    state.write(obj, bytes(rng.getrandbits(8) for _ in range(0x394)))
    d8 = rng.choice((0, 0, 1, rng.randrange(256)))
    state.w8(obj + 0xD8, d8)
    state.w8(obj + 0x165, 0 if rng.random() < 0.9 else rng.randrange(1, 256))
    target = rng.choice((rng.randrange(-8, 0x30), rng.randrange(0x30, 0x60), rng.randrange(0x60, 0x90),
                         rng.randrange(0x90, 0xC0), rng.randrange(0x60, 0x90), rng.randrange(0x30, 0x60),
                         rng.randrange(-32768, 32768)))
    if d8 and -0x1000 < target < 0x1000:
        target = target // 3 + rng.randrange(0, 2)
    state.w16(obj + 0xC6, target & 0xFFFF)
    return obj


def setup(state, rng, sym):
    base = state.alloc(0x800)
    state.write(base, bytes(rng.getrandbits(8) for _ in range(0x800)))
    left = _object(state, rng)
    right = _object(state, rng)
    state.w32(sym["data_801a27d0"], rng.randrange(3))
    for name in ("data_80171c5c", "data_80171c5d", "data_80171c5e", "data_80171c5f",
                 "data_80188d4c", "data_80188d50", "data_80188d54",
                 "data_80188d58", "data_80188d5c", "data_80188d60"):
        state.w8(sym[name], rng.getrandbits(8))
    return Setup(args=(base, left, right), returns_value=False)


def control(words):
    """Alter the store of the constant 0x76 into the left bar's halfword at 0xcc.

    The word is `ori v0,zero,0x76`, which the arm taken by a left value of
    0x30 or more executes.
    """
    found = [i for i, w in enumerate(words) if w == (0x34000000 | 0 << 21 | 2 << 16 | 0x76)]
    if not found:
        raise ValueError("no ori 0x76 found")
    index = found[0]
    return index, words[index] ^ 0x1, "constant 0x76 changed to 0x77"


CONTRACT = Contract(setup, control)
