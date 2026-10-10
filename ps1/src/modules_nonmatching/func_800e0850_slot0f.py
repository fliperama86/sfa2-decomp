"""Contract of func_800e0850_slot0f (reads and writes: see the header of the .c).

Choices of the setup:
  - mode is 0 in half of the cases, otherwise a random non-zero word;
  - data_800e8504_slot0f points at a 0x80-byte random block; the name is a
    16-byte random block; the words and halfwords at data_800df0f0 to
    data_800df0fc and data_800df118 are random, and so are the byte at
    data_800df11a (which this C does not read) and the path buffer at
    data_8018ff14 (four words);
  - func_800e0da8_slot0f answers 1, 4, 3, 0 or a random 32-bit word, each
    with probability 1/5;
  - func_800e11e4_slot0f answers in turn, whole words as the callee leaves
    them in the result register: the first open -1 in half of the cases,
    else a word other than -1; the second -1 in a third of the cases, else
    a word other than -1; the third a word other than -1. A word other than
    -1 is drawn from 0, 1, -2, 0xffff, 0xffff0000, 0xff, 0xffffff00,
    0x80000000, 0x7fffffff, a random 30-bit value and a random word with
    its low bit clear, so that a test against -1 meets its near misses;
  - close answers in turn: the first answer is -1, 0 or a word other than
    -1, the second -1, 0, 5 or a word other than -1 (equal odds), so that
    either call may fail;
  - func_800e12cc_slot0f answers -1 in a third of the cases, else a word
    other than -1 as for the opens;
  - strcat, strcpy and func_800e13dc_slot0f are recorders returning 0.
"""

from contracts import CallLog, Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def descriptor(rng):
    return rng.randrange(0, 20)


def not_minus_one(rng):
    """A whole word other than -1; the near misses of a test against -1 (a
    low byte or halfword of ones, other negative values, 0) are drawn often."""
    return rng.choice((0, 1, 0xFFFFFFFE, 0xFFFF, 0xFFFF0000, 0xFF, 0xFFFFFF00, 0x80000000, 0x7FFFFFFF,
                       rng.randrange(2, 1 << 30), rng.getrandbits(32) & 0xFFFFFFFE))


def setup(state, rng, sym) -> Setup:
    mode = 0 if rng.random() < 0.5 else rng.getrandbits(32) | 1
    header = state.alloc(0x80)
    fill(state, header, 0x80, rng)
    state.w32(sym["data_800e8504_slot0f"], header)
    name = state.alloc(16)
    fill(state, name, 16, rng)
    for symbol, size in (("data_800df0f0_slot0f", 4), ("data_800df0f4_slot0f", 2), ("data_800df0f8_slot0f", 4),
                         ("data_800df0fc_slot0f", 2), ("data_800df118_slot0f", 2), ("data_800df11a_slot0f", 1)):
        fill(state, sym[symbol], size, rng)
    fill(state, sym["data_8018ff14"], 16, rng)

    card = rng.choice((1, 4, 3, 0, rng.getrandbits(32)))
    first = -1 if rng.random() < 0.5 else not_minus_one(rng)
    second = -1 if rng.random() < 1 / 3 else not_minus_one(rng)
    third = not_minus_one(rng)
    closes = (rng.choice((-1, 0, not_minus_one(rng))), rng.choice((-1, 0, 5, not_minus_one(rng))))
    write = -1 if rng.random() < 1 / 3 else not_minus_one(rng)

    log = CallLog(state, 4096, watch=((header, 0x60 // 4), (sym["data_8018ff14"], 4)))
    log.replace(sym["strcat"], 2, pointees={0: 2, 1: 4})
    log.replace(sym["func_800e0da8_slot0f"], 1, result=card & 0xFFFFFFFF)
    log.replace(sym["func_800e11e4_slot0f"], 2, results=(first & 0xFFFFFFFF, second & 0xFFFFFFFF, third & 0xFFFFFFFF))
    log.replace(sym["close"], 1, results=tuple(c & 0xFFFFFFFF for c in closes))
    log.replace(sym["strcpy"], 2, pointees={1: 4})
    log.replace(sym["func_800e13dc_slot0f"], 0)
    log.replace(sym["func_800e12cc_slot0f"], 3, result=write & 0xFFFFFFFF)
    return Setup(args=(mode, name), returns_value=True)


def control(words):
    """Alter the length of the write: `ori a2,zero,0x2000` becomes 0x2001."""
    found = [i for i, w in enumerate(words) if w >> 26 == 13 and w & 0xFFFF == 0x2000]
    if len(found) != 1:
        raise ValueError(f"expected one constant 0x2000, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x2001, "write length changed to 0x2001"


CONTRACT = Contract(setup, control)
