"""Contract of func_8013245c (the header comment of the .c lists the reads and writes).

Choices made here:
  - the function has no argument; its inputs are globals, all set at random
    before the call: the selectors data_801a8067 and data_801a83fb take
    0x12, 0x13 or 0x14 in three cases of five, otherwise a value in 0 to 0x14
    (the index into table_80172144 stays within the first 34 words);
  - data_80198098 and data_8019842c are 0 or 1 at random (both arms are tried);
  - table_80172144 (34 words), player_left.field_5c and player_right.field_5c
    and the whole block data_801ac6a8 are random;
  - every global the function writes (data_801a6938, data_80188d64 to 6b,
    data_80171c5c/5d, the strips of data_80188d6c, data_80188ebc to ec3) starts
    random, so that a store moved across a call shows in the watched copies;
  - the record area at data_80186004 (2 * 0x1680 bytes) and the table
    table_80188d08 are random, so that the bytes the function leaves alone
    are tested;
  - every recorder copies, at every call, the blocks of the watch list (the
    block data_801ac6a8, data_801a6938, data_80188d64 to 6b, table_80188d08
    up to 0x3c, data_80171c5c/5d, the four strips of data_80188d6c and
    data_80188ebc to ec3); the pointer argument of func_8015c150 is followed
    for 8 words, of func_8015c09c for 10 and of func_80136d1c for 7;
  - the memory that the three last callees write is random before the call:
    strips2 and data_80188e4c (0xe0 bytes), strips (0x1c0 bytes, the first
    two rows of eight records and the rest that func_80132b30 reaches
    through data_80190014) and the two records of data_8018d210 (0x40
    bytes); game_state.field_42, which func_80132b30 tests, is zero in half
    of the cases; the watch list does not hold those three blocks: only
    game code that runs alike in both runs writes them, the function itself
    never does, and the arena has no room for the larger log;
  - recorders replace func_8015c150 (1 argument), func_8015c09c (1),
    func_8015bd0c (4 arguments, a random result per case) and func_80136d1c
    (1); the log has room for the 604 calls the function itself makes (24 of
    func_8015c150, 288 of func_8015c09c, 288 of func_8015bd0c, 4 of
    func_80136d1c) and for the calls to func_80136d1c that its callees make,
    each entry holding the watch blocks (169 words) and its pointees.
"""

from contracts import CallLog, Contract, Setup


def _rand(state, rng, address, size):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def _selector(rng):
    return rng.choice((0x12, 0x13, 0x14, rng.randrange(0, 0x15), rng.randrange(0, 0x15)))


def setup(state, rng, sym):
    state.w8(sym["data_801a8067"], _selector(rng))
    state.w8(sym["data_801a83fb"], _selector(rng))
    state.w8(sym["data_80198098"], rng.randrange(2))
    state.w8(sym["data_8019842c"], rng.randrange(2))
    _rand(state, rng, sym["table_80172144"], 4 * 34)
    _rand(state, rng, sym["data_801ac6a8"], 0x1E0)
    _rand(state, rng, sym["data_80186004"], 2 * 0x1680)
    _rand(state, rng, sym["table_80188d08"], 0x40)
    for name, size in (("data_801a6938", 4), ("data_80188d64", 8), ("data_80171c5c", 4),
                       ("data_80188d6c", 0x70), ("data_80188ebc", 8)):
        _rand(state, rng, sym[name], size)
    # what the three last callees (game code, run as the original) write: the
    # two-by-two strips2 and data_80188e4c (func_80132cf0: 2 * 0x70 bytes),
    # the eight strips and eight more of strips (func_80132b30: 0x1c0 bytes),
    # two Effect records (func_80153088: 0x40 bytes); func_80132b30 reads
    # game_state.field_42, which is zero in half of the cases
    _rand(state, rng, sym["strips2"], 0xE0)
    _rand(state, rng, sym["strips"], 0x1C0)
    _rand(state, rng, sym["data_8018d210"], 0x40)
    state.w8(sym["game_state"] + 0x42, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    state.w16(sym["player_left"] + 0x5C, rng.getrandbits(16))
    state.w16(sym["player_right"] + 0x5C, rng.getrandbits(16))
    watch = ((sym["data_801ac6a8"], 0x1E0 // 4),      # the block the function fills
             (sym["data_801a6938"], 1),
             (sym["data_80188d64"], 2),               # data_80188d64, 65, 68, 69
             (sym["table_80188d08"], 15),
             (sym["data_80171c5c"], 1),               # data_80171c5c and 5d
             (sym["data_80188d6c"], 0x70 // 4),       # the four strips
             (sym["data_80188ebc"], 2))               # data_80188ebc, data_80188ec0
    log = CallLog(state, 160000, watch=watch)
    log.replace(sym["func_8015c150"], 1, pointees={0: 8})
    log.replace(sym["func_8015c09c"], 1, pointees={0: 10})
    log.replace(sym["func_8015bd0c"], 4, rng.getrandbits(16))
    log.replace(sym["func_80136d1c"], 1, pointees={0: 7})
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the constant 0x7807 loaded for data_80188ec0 to 0x7806."""
    found = [i for i, w in enumerate(words) if w == 0x34027807]
    if len(found) != 1:
        raise ValueError(f"expected one ori v0,zero,0x7807, found {len(found)}")
    index = found[0]
    return index, words[index] ^ 0x1, "constant 0x7807 changed to 0x7806"


CONTRACT = Contract(setup, control)
