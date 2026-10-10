"""Contract of func_801cb564_slot05_06: a character state with a countdown and a push on the opponent.

Reads and writes are listed in the header comment of
func_801cb564_slot05_06.c. Choices made here:
  - the mode byte (0x3a) is 2 in a fifth of the cases, 0 in a tenth, else
    a random non-zero byte other than 2; x (0x12) is chosen among the
    borders 0x200, 0x201, 0x27f, 0x280, 0x300, 0x301 half of the time,
    else random;
  - field_4c (a word) is negative in a third of the cases, 0 in a tenth,
    else positive, always within 30 bits; field_54 within 30 bits;
  - game_state.field_63, which the function writes, is a random byte before
    the call;
  - field_cd is 0 in half of the cases; game_state.field_1d has random bits;
  - field_1c5 is 0, 1, 2 or 0x80 or above or near (field_12a >> 1) + 3 in
    most cases, else random; field_12a is random; field_0b is 0 or random;
  - each recorder with a result returns 0 in 45 cases of 100, 0x100 (a
    non-zero word with a zero low byte, which tells a test of the whole
    word from a test of the low byte) in 10 of 100, else a non-zero word or
    byte;
  - the opponent (field_40) and the pushed object (ref_other.p) are blocks
    of 0x394 bytes of random content, the opponent's field_5c is negative
    in half of the cases;
  - the table of 256 halfwords is random; the random generator state is
    random;
  - every other callee is a recorder, the log watches the object whole,
    the words at 0x10 and 0x14 of the pushed object and the word at
    game_state offset 0x60.
"""

from contracts import CallLog, Contract, Setup, fill, halfword  # noqa: F401


def s30(rng):
    return rng.randrange(-(1 << 30), 1 << 30)


def word_result(rng, whole_word):
    roll = rng.random()
    if roll < 0.45:
        return 0
    if whole_word and roll < 0.55:
        return 0x100
    return (rng.getrandbits(32) | 1) if roll > 0.55 else rng.randrange(1, 256)


def setup(state, rng, sym) -> Setup:
    game_state = sym["game_state"]
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    other = state.alloc(0x394)
    fill(state, other, 0x394, rng)
    pushed = state.alloc(0x394)
    fill(state, pushed, 0x394, rng)
    state.w32(obj + 0x40, other)
    state.w32(sym["ref_other"], pushed)

    roll = rng.random()
    if roll < 0.2:
        mode = 2
    elif roll < 0.3:
        mode = 0
    else:
        mode = rng.choice([m for m in range(1, 256) if m != 2])
    state.w8(obj + 0x3A, mode)
    if rng.random() < 0.5:
        state.w16(obj + 0x12, rng.choice((0x200, 0x201, 0x27F, 0x280, 0x300, 0x301)))

    roll = rng.random()
    if roll < 0.33:
        field_4c = rng.randrange(-(1 << 30), 0)
    elif roll < 0.43:
        field_4c = 0
    else:
        field_4c = rng.randrange(0, 1 << 30)
    state.w32(obj + 0x4C, field_4c & 0xFFFFFFFF)
    state.w32(obj + 0x54, s30(rng) & 0xFFFFFFFF)
    if rng.random() < 0.5:
        state.w8(obj + 0xCD, 0)
    state.w8(obj + 0x0B, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    state.w8(game_state + 0x1D, rng.getrandbits(8))
    state.w8(game_state + 0x63, rng.getrandbits(8))  # written by the function

    f12a = state.read(obj + 0x12A, 1)[0]
    roll = rng.random()
    if roll < 0.4:
        c5 = rng.choice((0, 1, 1, 2, 3))
    elif roll < 0.6:
        c5 = (f12a >> 1) + 3 + rng.choice((-1, 0, 1))
    elif roll < 0.8:
        c5 = 0x80 | rng.getrandbits(7)
    else:
        c5 = rng.randrange(256)
    state.w8(obj + 0x1C5, c5 & 0xFF)

    state.w16(other + 0x5C, rng.getrandbits(16))  # negative in half of the cases
    table = sym["data_801dd478_slot05_06"]
    for index in range(256):
        state.w16(table + 2 * index, halfword(rng))
    state.w16(sym["game_state"] + 0x1E, halfword(rng))  # the random generator state is the halfword at 0x80190126

    log = CallLog(state, 3000, watch=((obj, 0x394 // 4), (pushed + 0x10, 2), (game_state + 0x60, 1)))
    log.replace(sym["func_80141248"], 1, 0)
    log.replace(sym["func_801410c8"], 1, word_result(rng, True))
    log.replace(sym["func_801cded4_slot05_06"], 1, word_result(rng, True))
    log.replace(sym["func_801468f4"], 1, word_result(rng, True))
    log.replace(sym["func_80148e84"], 1, word_result(rng, True))
    log.replace(sym["func_80148ea8"], 1, word_result(rng, True))
    log.replace(sym["func_80140cd8"], 3, 0)
    log.replace(sym["func_801204f4"], 3, 0)
    log.replace(sym["func_80120554"], 3, 0)
    log.replace(sym["func_801307e0"], 2, 0, masks={1: 0xFFFF})
    log.replace(sym["func_80130efc"], 1, 0)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of the velocity constant 0xa0000 into field_4c: the build stores another value.

    The upper half 0xa is loaded once with a `lui` of 0xa.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0F and w & 0xFFFF == 0xA]
    if len(found) != 1:
        raise ValueError(f"expected one lui of 0xa, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0xB, "field_4c constant 0xa0000 changed to 0xb0000"


CONTRACT = Contract(setup, control)
