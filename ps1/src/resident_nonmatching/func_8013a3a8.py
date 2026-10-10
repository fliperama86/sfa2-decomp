"""Contract of func_8013a3a8 as code. The roles are in func_8013a3a8.c.

Choices of the setup:
  - a0 (attacker), a1 (target) and a2 (attack record, 0x40 bytes) are blocks
    of random bytes, with the fields the function branches on steered so that
    each arm is common: a0->field_49, field_08 (0, 8 or random),
    field_28a, field_29c (0 to 4), field_0b and field_5c (below 0x28 in half
    of the cases); a1->field_159, field_29c, field_61 (0xff in half of the
    cases, else a random byte), field_d8, field_5c (a random halfword in 14
    cases of 100, else 0 in one case of twenty, else 0 to 0x8f), field_cd,
    field_6a (below 0x1f in 66 cases of 100), field_295, and field_15e and
    field_162 (left as random bytes); a2->field_14, field_15 (5 in a
    quarter), field_16 (0 to 4, or a random byte), field_17 (0, 1, 2 or
    random), field_1b, field_1c (a2->field_08 stays a random byte); the
    frame record of a1 is a block of 16 random bytes;
  - game_state.config points at a block of 0xb0 random bytes with field_12
    in 0 to 4, field_4c and field_4d usually 0, and field_4e usually 0;
  - the two players, data_80188f34 (0 or 1), the seed data_80190126 (random,
    not 0) and the global pointers hold random values; each player's
    field_167 is 0 in three cases of five;
  - the tables from table_801777d4 to 0x300 bytes behind table_8017984c are
    filled with random bytes made small often (a byte shifted right by a
    random 0 to 7 bits), so that the sums and the clamps on both sides are
    reached; the bit 0x80 of table_80179834 at a1's kind is set in half of
    the cases;
  - func_8013ae00 (3 arguments), func_8013a154 (3), func_8013ad64 (1) and
    func_80147000 (1) are recorders returning 0. func_80151184 and
    func_8013ad2c run as the original code.
The log watches at every recorded call: a1 (0x400 bytes), both players
(0x394 bytes each), the config block, the pointer and counter globals the
function writes (ref_first, ref_third, ref_other, data_80188f20, data_80188f24,
data_80188f40) and the seed data_80190126. No recorded callee gets a pointer
to memory filled for that call.
"""

from contracts import CallLog, Contract, Setup, fill


def _small(rng):
    return rng.getrandbits(8) >> rng.randrange(8)


def setup(state, rng, sym):
    a0 = state.alloc(0x400)
    a1 = state.alloc(0x400)
    a2 = state.alloc(0x40)
    frame = state.alloc(0x10)
    config = state.alloc(0xB0)
    for block, size in ((a0, 0x400), (a1, 0x400), (a2, 0x40), (frame, 0x10), (config, 0xB0)):
        fill(state, block, size, rng)
    players = (sym["player_left"], sym["player_right"])
    for player in players:
        fill(state, player, 0x394, rng)
        if rng.random() < 0.6:
            state.w8(player + 0x167, 0)

    watch = [(a1, 0x400 // 4), (players[0], 0x394 // 4), (players[1], 0x394 // 4), (config, 0xB0 // 4)]
    for name in ("ref_first", "ref_third", "ref_other", "data_80188f20", "data_80188f24", "data_80188f40"):
        watch.append((sym[name], 1))
    watch.append((sym["data_80190126"] - 2, 1))  # the word that holds the halfword seed
    log = CallLog(state, 4096, watch=tuple(watch))
    log.replace(sym["func_8013ae00"], 3, 0)
    log.replace(sym["func_8013a154"], 3, 0)
    log.replace(sym["func_8013ad64"], 1, 0)
    log.replace(sym["func_80147000"], 1, 0)

    state.w32(sym["game_state"] + 0x360, config)
    state.w8(config + 0x12, rng.randrange(5))
    state.w8(config + 0x4C, 0 if rng.random() < 0.8 else rng.getrandbits(8))
    state.w8(config + 0x4D, 0 if rng.random() < 0.8 else rng.getrandbits(8))
    state.w8(config + 0x4E, 0 if rng.random() < 0.6 else rng.getrandbits(8))

    state.w8(sym["data_80188f34"], rng.randrange(2))
    state.w16(sym["data_80190126"], rng.randrange(1, 0x10000))
    for name in ("ref_first", "ref_third", "ref_other", "data_80188f20", "data_80188f24", "data_80188f40"):
        state.w32(sym[name], rng.getrandbits(32))

    start = sym["table_801777d4"]
    end = sym["table_8017984c"] + 0x300
    state.write(start, bytes(_small(rng) for _ in range(end - start)))

    # a0
    state.w8(a0 + 0x65, rng.randrange(2))
    state.w8(a0 + 0x49, 1 if rng.random() < 0.35 else 0)
    state.w16(a0 + 0x5C, rng.randrange(-100, 0x28) if rng.random() < 0.5 else rng.randrange(0x28, 0x200))
    state.w8(a0 + 0x08, rng.choice((0, 0, 0, 8, 8, rng.getrandbits(8))))
    state.w8(a0 + 0x28A, 0 if rng.random() < 0.5 else rng.getrandbits(8))
    state.w8(a0 + 0x29C, rng.randrange(5))
    state.w8(a0 + 0x0B, 0 if rng.random() < 0.5 else rng.getrandbits(8))
    # a1
    state.w8(a1 + 0x159, 0 if rng.random() < 0.4 else rng.getrandbits(8))
    state.w8(a1 + 0x29C, rng.randrange(5))
    state.w32(a1 + 0x88, frame)
    state.w8(a1 + 0x61, 0xFF if rng.random() < 0.5 else rng.getrandbits(8))
    state.w8(a1 + 0xD8, 0 if rng.random() < 0.4 else rng.getrandbits(8))
    if rng.random() < 0.14:
        state.w16(a1 + 0x5C, rng.getrandbits(16))
    else:
        state.w16(a1 + 0x5C, 0 if rng.random() < 0.05 else rng.randrange(0, 0x90))
    state.w8(a1 + 0xCD, 0 if rng.random() < 0.5 else rng.getrandbits(8))
    state.w8(a1 + 0x6A, rng.randrange(0x1F) if rng.random() < 0.66 else rng.randrange(0x1F, 256))
    state.w8(a1 + 0x295, rng.randrange(2))
    # a2
    state.w8(a2 + 0x14, 0 if rng.random() < 0.4 else rng.getrandbits(8))
    state.w8(a2 + 0x15, 5 if rng.random() < 0.25 else rng.getrandbits(8))
    state.w8(a2 + 0x16, rng.choice((0, 1, 2, 3, 4, rng.getrandbits(8))))
    state.w8(a2 + 0x17, rng.choice((0, 1, 2, rng.getrandbits(8))))
    state.w8(a2 + 0x1B, 0 if rng.random() < 0.5 else rng.getrandbits(8))
    state.w8(a2 + 0x1C, 0 if rng.random() < 0.5 else rng.getrandbits(8))
    # the table byte at a1's kind
    kind = state.read(a1 + 0xA7, 1)[0]
    entry = sym["table_80179834"] + kind
    value = state.read(entry, 1)[0]
    state.w8(entry, value | 0x80 if rng.random() < 0.5 else value & 0x7F)

    return Setup(args=(a0, a1, a2), returns_value=False)


def control(words):
    """Alter the store of the 0xff the function writes to a1's field_260.

    The store is `sb reg, 0x260(reg)`; it is the only byte store with that
    offset in the build.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0x0260]
    if len(found) != 1:
        raise ValueError(f"expected one store to field_260, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x025F, "store to field_260 moved by one byte"


CONTRACT = Contract(setup, control)
