"""Contract of func_8011c724: an object's sprite run list becomes quad records.

Reads and writes are listed in the header comment of func_8011c724.c.
Choices made by this setup:
  - the object's field_94 is an index 0 to 7 (0 returns at once); the three
    table entries it selects (header pointer, tile id, check value) are 0 in
    about one case in twelve each, so that every early return is tried;
  - the header count is 1 to 30; the stream is written run by run until the
    runs cover the count: each run is three words (x step, y step, run word),
    the run word is mostly 0 to 11 and sometimes 0xffff (a run of 0 tiles),
    steps are random; the stream holds a few extra words behind it;
  - the header offset is 0 to 15, odd values included (its low bit is
    cleared by the code);
  - tile ids are 1 to 5 in the table and the next-tile table holds 0 to 5,
    the per-tile byte table holds random bytes;
  - field_02 is 6 and field_08 is 12 in about a third of the cases; field_0b
    is 0 to 3 mostly and random otherwise; field_0f is 0 or random;
  - the quad area (cursor) is a block of 64 quads of random bytes, so that the
    bytes the function leaves alone are tested;
  - the buffer selector data_801a27d0 is 0 to 3 (used only to form an address);
  - func_8015bdd4 and func_8015bf34 are recorders; the first returns a random
    value per case; the log copies, at every call, the object (0xb0 bytes),
    the whole quad area and the cursor word, and for func_8015bf34 the four
    words behind its second argument.
"""

from contracts import CallLog, Contract, Setup


def setup(state, rng, sym):
    index = rng.randrange(8)
    table = sym["data_80184380"] + 8 * index
    tiles = sym["data_80183ffc"] + 4 * index
    check = sym["data_801841bc"] + 4 * index

    count = rng.randrange(1, 31)
    run_words = []
    covered = 0
    while covered < count:
        word = 0xFFFF if rng.random() < 0.05 else rng.randrange(12)
        run_words.append(word)
        covered += (word + 1) & 0xFFFF
    offset = rng.randrange(16)
    stream_size = 16 + 6 * len(run_words) + 16
    stream = state.alloc(stream_size)
    for i in range(0, stream_size, 2):
        state.w16(stream + i, rng.getrandbits(16))
    for n, word in enumerate(run_words):
        state.w16(stream + (offset & 0xFFFE) + 6 * n + 4, word)

    header = state.alloc(16)
    for i in range(0, 16, 2):
        state.w16(header + i, rng.getrandbits(16))
    state.w16(header, count)
    state.w16(header + 8, offset)

    state.w32(table, 0 if rng.random() < 0.08 else header)
    state.w32(table + 4, rng.getrandbits(32))
    state.w16(tiles, rng.getrandbits(16))
    state.w16(tiles + 2, 0 if rng.random() < 0.08 else rng.randrange(1, 6))
    state.w16(check, rng.getrandbits(16))
    state.w16(check + 2, 0 if rng.random() < 0.08 else rng.getrandbits(16))

    for tile in range(6):
        state.w16(sym["data_80183c90"] + 2 * tile, rng.randrange(6))
        for part in range(16):
            state.w8(sym["data_80185504"] + 16 * tile + part, rng.getrandbits(8))

    lists = state.alloc(0x80)
    state.w32(sym["data_801987c8"], lists)
    state.w32(sym["data_801a27d0"], rng.randrange(4))
    quads = state.alloc(16 * 64)
    for i in range(0, 16 * 64, 4):
        state.w32(quads + i, rng.getrandbits(32))
    state.w32(sym["data_801a4fe8"], quads)

    state.w16(sym["box_margin"], rng.getrandbits(16))
    state.w16(sym["data_801aa5ea"], rng.getrandbits(16))
    state.w16(sym["data_8019019a"], rng.getrandbits(16))

    obj = state.alloc(0xB0)
    for i in range(0, 0xB0, 4):
        state.w32(obj + i, rng.getrandbits(32))
    state.w16(obj + 0x94, index)
    state.w32(obj + 0x98, stream)
    if rng.random() < 0.35:
        state.w8(obj + 0x02, 6)
        state.w8(obj + 0x08, 12)
    elif rng.random() < 0.3:
        state.w8(obj + 0x02, 6)
    state.w8(obj + 0x0B, rng.randrange(4) if rng.random() < 0.85 else rng.getrandbits(8))
    state.w8(obj + 0x0F, 0 if rng.random() < 0.5 else rng.randrange(1, 256))

    # Every recorder copies the object, the quad area and the cursor at every
    # call, so that the order of the function's stores against the calls is
    # compared; func_8015bf34 also gets the 16 bytes its second argument points at.
    log = CallLog(state, 40000, watch=((obj, 0xB0 // 4), (quads, 16 * 64 // 4), (sym["data_801a4fe8"], 1)))
    log.replace(sym["func_8015bdd4"], 2, rng.getrandbits(32))
    log.replace(sym["func_8015bf34"], 2, 0, pointees={1: 4})
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of func_8015bdd4's result into the quad: its offset moves by two bytes.

    The build keeps the quad pointer already advanced, so the store of the
    result is the only halfword store with offset 0 (`sh reg, 0(reg)`); every
    call that gets past the early returns executes it at least once, and the
    result is random, so the altered store lands on a neighbouring halfword.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0]
    if len(found) != 1:
        raise ValueError(f"expected one halfword store at offset 0, found {len(found)}")
    index = found[0]
    return index, words[index] | 2, "store of the callee's result into the quad moved by two bytes"


CONTRACT = Contract(setup, control)
