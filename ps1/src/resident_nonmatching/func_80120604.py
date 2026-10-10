"""Contract of func_80120604: a0 = a, a1 = b, a2 = c, a3 = d, no result.

Choices made here:
  - a is 0 or 1 in a third of the cases, 0x80 in one case in twelve, and
    otherwise 2 to 0x7f; table_8016e6e0[a] is made to point so that the
    first entry sits at the block (c's low 16 bits times 8 bytes below it);
  - the list has 1 to 6 entries; every entry but the last has bit 23 of its
    second word set, the last has it clear; the first word's command is
    drawn from 0 to 11 (all, with 0, 1, 9 and 11 more often) and from 12 to
    15 in one entry of ten; the low 12 bits are 0xfff in a third of the
    entries; byte 2 is 0xff in a third of the entries; the other fields
    are random;
  - b is 0 in half of the cases; c has random upper 16 bits in half of the
    cases (they must not matter); d is random, and in a sixth of the cases
    it is chosen so that the first step by 0xc0 makes its low 16 bits 0;
  - data_80190a44[a] and [a + 8] (one byte each) are random; so are the
    0x1004 bytes from data_80197ed0 (the 36 priority bytes and everything
    behind them that command 11 can reach with a channel of up to
    0xffe + 4), and the two halfwords of table_80197ef8, which lie inside
    that reach;
  - func_8016a7e4 returns 2 in half of the cases (the same for every call
    of that case; the recorder cannot differ between calls), 0 otherwise;
  - every recorder copies, at every call, the priority bytes data_80197ed0
    (36 bytes) and the word of table_80197ef8;
  - the nine callees are recorders, with the argument counts of the header.
"""

from contracts import CallLog, Contract, Setup

CALLEES = (
    ("func_80164140", 8),
    ("func_80164ef0", 4),
    ("func_8016936c", 2),
    ("func_80168f50", 4),
    ("func_80165d34", 2),
    ("func_80164a78", 3),
    ("func_8016452c", 1),
    ("func_80164bbc", 1),
)


def setup(state, rng, sym) -> Setup:
    r = rng.random()
    if r < 0.33:
        a = rng.randrange(2)
    elif r < 0.41:
        a = 0x80
    else:
        a = rng.randrange(2, 0x80)
    b = 0 if rng.random() < 0.5 else rng.randrange(1, 256)
    c = rng.getrandbits(16)
    d = rng.getrandbits(32)
    if rng.random() < 1 / 6:
        d = ((-0xC0) & 0xFFFF) | (rng.getrandbits(16) << 16)

    count = rng.randrange(1, 7)
    block = state.alloc(8 * count)
    for index in range(count):
        command = rng.choice((0, 0, 1, 1, 9, 11, 11, rng.randrange(12), rng.randrange(12), rng.randrange(12, 16)))
        low = 0xFFF if rng.random() < 1 / 3 else rng.getrandbits(12)
        byte2 = 0xFF if rng.random() < 1 / 3 else rng.getrandbits(8)
        word0 = (rng.getrandbits(8) << 24) | (byte2 << 16) | (command << 12) | low
        word1 = rng.getrandbits(32) & ~0x800000
        if index < count - 1:
            word1 |= 0x800000
        state.w32(block + 8 * index, word0)
        state.w32(block + 8 * index + 4, word1)
    state.w32(sym["table_8016e6e0"] + 4 * (a if a != 0x80 else 0), (block - 8 * c) & 0xFFFFFFFF)

    program = sym["data_80190a44"]
    state.w8(program + a, rng.getrandbits(8))
    state.w8(program + a + 8, rng.getrandbits(8))
    # Command 11 clears data_80197ed0[channel] for a channel of up to 0xffe + 4: the whole reach is filled, so
    # that the store of a zero changes a byte and is compared (the image holds zeros there).
    state.write(sym["data_80197ed0"], rng.randbytes(0x1004))
    state.w16(sym["table_80197ef8"], rng.getrandbits(16))
    state.w16(sym["table_80197ef8"] + 2, rng.getrandbits(16))

    log = CallLog(state, watch=((sym["data_80197ed0"], 9), (sym["table_80197ef8"], 1)))
    for name, arguments in CALLEES:
        log.replace(sym[name], arguments, 0)
    log.replace(sym["func_8016a7e4"], 1, 2 if rng.random() < 0.5 else 0)

    if rng.random() < 0.5:
        c |= rng.getrandbits(16) << 16
    return Setup(args=(a, b, c, d), returns_value=False)


def control(words):
    """Alter the first shift right by 6: srl 6 becomes srl 5.

    It is the volume scaling (read from the original's listing, not tested).
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0 and w & 0x3F == 0x02 and (w >> 6) & 31 == 6]
    if not found:
        raise ValueError("expected a shift right by 6")
    index = found[0]
    return index, (words[index] & ~(31 << 6)) | (5 << 6), "volume shifted by 5 instead of 6"


CONTRACT = Contract(setup, control)
