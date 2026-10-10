"""Contract setup of func_801b3564_slot04_06 (see the header of the .c file).

Choices of the setup:
  - a0 is an object block of 0x394 bytes filled with random bytes; field_40
    points to a second block ("other") and ref_other.p to a third one, both
    of 0x394 bytes and random;
  - field_3a is 2 in one case of four, 0 in one case of eight, otherwise a
    random byte other than 0 and 2; pos_x is, in four cases of ten, one of
    the edge values 0x1ff to 0x302 around 0x200, 0x280 and 0x300, in three of
    ten a value of 0x201 to 0x300, otherwise random;
  - field_4c and field_54 are random signed 30-bit values; in half of the
    cases field_4c is then replaced by the absolute value of another such
    value (so it is negative in about a quarter of the cases); field_0b is
    0 in half; field_cd is 0 in
    two cases of five; the word at field_10 and field_14 are random;
  - field_1c5 is 1 in one case of four, otherwise from a mix of small values
    (0 to 7), values with bit 7 set, and random; field_12a is random;
    field_07 is random with 0xff in one case of eight;
  - game_state.field_1d is random with its two low bits taken uniformly;
    the other game_state bytes keep what the executable image holds;
  - other's field_5c is random with its sign taken at random;
  - game_state.field_63, which the function writes, is a random byte
    before the call;
  - the table of halfwords at data_801c547c_slot04_06 gets 256 random values;
  - the results of func_801410c8, func_801b5ed8_slot04_06, func_801468f4,
    func_80148e84 and func_80148ea8 are whole words, chosen per case and per
    callee so that a test of the low byte and a test of the word disagree
    often: 0 in one case of four, a word with a zero low byte and other bits
    set (0x100, or random upper bits) in one of four, a byte from 1 to 255 in
    one of four, and a random word with a non-zero low byte otherwise;
    func_80151184 returns a
    different random word at each of its calls (twelve are given);
  - every recorder copies the three objects and game_state (0x364 bytes)
    into its log entry at every call.
"""

from contracts import CallLog, Contract, Setup, fill

EDGES = (0x1FF, 0x200, 0x201, 0x27F, 0x280, 0x281, 0x2FF, 0x300, 0x301, 0x302)


def s30(rng):
    return rng.randrange(-(1 << 30), 1 << 30)


def flag(rng):
    """A byte for a field of the object: 0 in half of the cases, else 1 to 255."""
    return 0 if rng.random() < 0.5 else rng.randrange(1, 256)


def result(rng):
    """A callee's result as a whole word. The original tests some results by their low byte only, so the
    values with a zero low byte and other bits set are the ones that tell a byte test from a word test."""
    kind = rng.randrange(4)
    if kind == 0:
        return 0
    if kind == 1:
        return 0x100 if rng.random() < 0.5 else rng.randrange(1, 1 << 24) << 8
    if kind == 2:
        return rng.randrange(1, 256)
    return (rng.getrandbits(24) << 8) | rng.randrange(1, 256)


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    other = state.alloc(0x394)
    target = state.alloc(0x394)
    gs = sym["game_state"]
    log = CallLog(state, words=12000,
                  watch=((obj, 0x394 // 4), (other, 0x394 // 4), (target, 0x394 // 4), (gs, 0x364 // 4)))
    for name, args in (("func_80130efc", 1), ("func_801307e0", 2), ("func_80140cd8", 3),
                       ("func_801204f4", 3), ("func_80120554", 3), ("func_80141248", 1)):
        log.replace(sym[name], args, rng.getrandbits(32))
    for name in ("func_801410c8", "func_801b5ed8_slot04_06", "func_801468f4",
                 "func_80148e84", "func_80148ea8"):
        log.replace(sym[name], 1, result(rng))
    log.replace(sym["func_80151184"], 0, results=tuple(rng.getrandbits(32) for _ in range(12)))
    for block in (obj, other, target):
        fill(state, block, 0x394, rng)
    state.w8(gs + 0x63, rng.getrandbits(8))  # written by the function
    state.w32(sym["ref_other"], target)
    state.w32(obj + 0x40, other)
    r = rng.random()
    state.w8(obj + 0x3A, 2 if r < 0.25 else 0 if r < 0.375 else rng.choice([b for b in range(256) if b not in (0, 2)]))
    state.w32(obj + 0x10, rng.getrandbits(32))
    r = rng.random()
    state.w16(obj + 0x12, rng.choice(EDGES) if r < 0.4 else rng.randrange(0x201, 0x301) if r < 0.7 else rng.getrandbits(16))
    state.w32(obj + 0x4C, s30(rng) & 0xFFFFFFFF)
    if rng.random() < 0.5:
        state.w32(obj + 0x4C, abs(s30(rng)) & 0xFFFFFFFF)
    state.w32(obj + 0x54, s30(rng) & 0xFFFFFFFF)
    state.w8(obj + 0x0B, flag(rng))
    state.w8(obj + 0xCD, 0 if rng.random() < 0.4 else rng.randrange(1, 256))
    r = rng.random()
    c5 = 1 if r < 0.25 else rng.randrange(8) if r < 0.5 else 0x80 | rng.randrange(128) if r < 0.75 else rng.randrange(256)
    state.w8(obj + 0x1C5, c5)
    state.w8(obj + 0x12A, rng.randrange(256))
    state.w8(obj + 0x07, 0xFF if rng.random() < 0.125 else rng.randrange(256))
    state.w8(gs + 0x1D, (rng.getrandbits(8) & 0xFC) | rng.randrange(4))
    state.w16(other + 0x5C, rng.getrandbits(16))
    table = sym["data_801c547c_slot04_06"]
    for i in range(256):
        state.w16(table + 2 * i, rng.getrandbits(16))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Change the knock-back distance of the second test: the ori of 0x60."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0xD and w & 0xFFFF == 0x60]
    if len(found) != 1:
        raise ValueError(f"expected one ori of 0x60, found {len(found)}")
    return found[0], words[found[0]] + 1, "knock-back distance 0x60 changed to 0x61"


CONTRACT = Contract(setup, control)
