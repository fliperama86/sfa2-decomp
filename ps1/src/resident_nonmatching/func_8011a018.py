"""Contract of func_8011a018 (no argument, no result).

Choices of the setup (details in the header of func_8011a018.c):
  - the table region 0x80183900 to 0x80184710 (every table the function
    clears or fills, and the words between them), the 0xa000 bytes of the
    two cell buffers and the primitive arrays are filled with random bytes,
    so that every store of the function changes something and a store that
    strays outside its table is seen;
  - func_8015c0ec and func_80158a2c are recorders (1 and 5 arguments) that
    return 0; func_8015bdd4 (2 arguments) and func_8015bd0c (4 arguments)
    return one random 32-bit value per case, kept for the whole case;
  - CallLog watches, at every call, data_801846fc and data_80184700 and
    one word at the end of each of three of the tables the function writes
    (the calls number 5,728 and more blocks exhaust the instruction
    budget), and func_8015c0ec records the 16 bytes of the cell it is
    given; the cell buffers themselves are too large to watch whole (see
    setup);
  - the log has room for the words of the longest run (about 60,000).
"""

from contracts import CallLog, Contract, Setup


def fill(state, base, size, rng):
    for offset in range(0, size, 4):
        state.w32(base + offset, rng.getrandbits(32))


def setup(state, rng, sym):
    fill(state, 0x80183900, 0xE10, rng)
    fill(state, sym["data_801987cc"], 0xA000, rng)
    fill(state, sym["data_8018db18"], 0xA80, rng)
    fill(state, sym["data_8019056c"], 0x3C0, rng)
    # Watched at every call: the two counters and one word at the end of
    # each of three of the tables written before the first call (more blocks
    # take the original past the instruction budget, the calls number
    # 5,728). Cell writes cannot be watched whole (the cell buffers are
    # 0xa000 bytes and the calls number 5,728), so the cell passed to func_8015c0ec is recorded: its 16
    # bytes as they are at the call.
    watch = ((0x801846FC, 2), (0x80183C90 + 0xDC, 1), (0x80183DEC + 0x4C, 1), (0x80183910 + 0xDC, 1))
    log = CallLog(state, words=60000, watch=watch)
    log.replace(sym["func_8015c0ec"], 1, 0, pointees={0: 4})
    log.replace(sym["func_8015bdd4"], 2, rng.getrandbits(32))
    log.replace(sym["func_8015bd0c"], 4, rng.getrandbits(32))
    log.replace(sym["func_80158a2c"], 5, 0)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the constant stored into data_80184700: 0x6f becomes 0x70.

    It is the one `ori v0,zero,0x6f` of the function, executed in every case.
    """
    found = [i for i, w in enumerate(words) if w == 0x3402006F]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x6f, found {len(found)}")
    index = found[0]
    return index, 0x34020070, "data_80184700 receives 0x70"


CONTRACT = Contract(setup, control)
