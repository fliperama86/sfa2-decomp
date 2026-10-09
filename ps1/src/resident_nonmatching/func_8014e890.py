"""Contract of func_8014e890 (see the header comment of func_8014e890.c).

Choices of the setup:
  - a0 is a random 32-bit value (it is only passed to a recorder);
  - game_state.field_40 is 0 to 19 (the length of the two tables);
  - the table entry for field_40 is a byte count: the strip count (the entry
    shifted right by 7) is 0 in one case in eight, 256 to 300 in one in
    four (so that the row and page wraps of the first loop are reached),
    and 1 to 40 otherwise; the low 7 bits of the entry are random;
  - the other table entries are random but bounded the same way;
  - the second table holds random halfwords;
  - the flag byte and the shared rectangle start with random contents;
  - every recorder also copies the word of the flag byte and the rectangle
    (watch);
  - the eight callees are recorders returning 0, in a log with room for the
    longest run (about 1,300 calls);
    the rectangle behind the first argument of func_80157fc4 and
    func_8015808c is recorded at every call.
"""

from contracts import CallLog, Contract, Setup

# (name, arguments, pointees). The two library routines get the shared
# rectangle by pointer, filled in for each call: the recorder copies its two
# words (x, y, w, h) into the log. The data buffers are only read by the
# callee and are not written by the function, so they have no pointee.
CALLEES = (("func_80119144", 2, None), ("func_801192bc", 1, None), ("func_801203b4", 1, None),
           ("func_8014f3b8", 2, None), ("func_80136bc0", 0, None), ("func_80157fc4", 2, {0: 2}),
           ("func_8015808c", 3, {0: 2}), ("func_801451ac", 1, None))


def _entry(rng) -> int:
    pick = rng.random()
    if pick < 0.125:
        count = 0
    elif pick < 0.375:
        count = rng.randrange(256, 301)
    else:
        count = rng.randrange(1, 41)
    return count << 7 | rng.randrange(128)


def setup(state, rng, sym) -> Setup:
    game_state = sym["game_state"]
    state.w16(game_state + 0x40, rng.randrange(20))
    for index in range(20):
        state.w32(sym["table_8017d96c"] + 4 * index, _entry(rng))
        state.w16(sym["table_8017d9bc"] + 2 * index, rng.getrandbits(16))
    state.w8(sym["data_8019032d"], rng.getrandbits(8))
    for offset in range(0, 8, 2):
        state.w16(sym["data_80189470"] + offset, rng.getrandbits(16))
    # Watched at every call: the word that holds the flag byte (the function
    # writes the byte; the rest of the word is data it must leave alone) and
    # the shared rectangle, so the order of the function's stores against its
    # calls is compared.
    log = CallLog(state, 16384, watch=((sym["data_8019032d"] & ~3, 1), (sym["data_80189470"], 2)))
    for name, arguments, pointees in CALLEES:
        log.replace(sym[name], arguments, 0, pointees)
    return Setup(args=(rng.getrandbits(32),), returns_value=False)


def control(words):
    """Alter the store of the flag byte: its offset moves by one byte.

    The store is the one byte store to data_8019032d (offset 0x32d from its
    base register); every call makes it.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0x032D]
    if len(found) != 1:
        raise ValueError(f"expected one store of the flag byte, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x032E, "flag byte store moved by one byte"


CONTRACT = Contract(setup, control)
