"""Contract of func_8011bf70: a packed cell list is unpacked into the cell tables.

Reads and writes are listed in the header comment of func_8011bf70.c. The
callees (func_8011a5f4, func_8011a604, func_8011a6b8, func_8011c568) run as
the original code, on a free list that this setup builds. Choices made here:
  - the entry index of the object (field_94) is 0 to 7; its own chain holds 0
    to 5 blocks, whose first id and length are in the entry tables;
  - block ids are 1 to 60; the next-link table (data_80183c90, ids 0 to 63) is
    random, then the free list and the entry's chain are linked through it;
  - the entry tables (data_80183ffc, data_801841bc, data_8018437c and the four
    data_80183910 group tables) and the cell tables are filled with random
    bytes for ids 0 to 63;
  - n is 0 to 40; the mode is 0, 2, 4 or 6 in nine cases of ten, otherwise
    random (a mode outside the four stores no cells);
  - in the run modes each run word is 0 to 5, and 0xffff in one run of thirty
    (a run of 0 cells); runs are written until they cover n;
  - the free count is chosen so that the work is left to func_8011c568 in one
    case of five (free plus own blocks below (n + 15) / 16 + 1) and otherwise
    covers both that bound and every block the cells need, so that the
    original never runs dry;
  - the object has random bytes, so field_0b takes every value.
"""

from contracts import Contract, Setup


def setup(state, rng, sym):
    mode = rng.choice((0, 2, 4, 6)) if rng.random() < 0.9 else rng.getrandbits(16)
    n = rng.randrange(0, 41) if rng.random() > 0.05 else 0
    data = []
    cells = 0
    if mode == 0:
        data = [rng.getrandbits(16) for _ in range(n)]
        cells = n
    elif mode == 2:
        data = [rng.getrandbits(16) for _ in range(2 * n)]
        cells = n
    elif mode in (4, 6):
        covered = 0
        while covered < n:
            run = 0xFFFF if rng.random() < 0.03 else rng.randrange(6)
            data += [rng.getrandbits(16)]
            if mode == 6:
                data += [rng.getrandbits(16)]
            data += [run]
            covered += (run + 1) & 0xFFFF
            cells += (run + 1) & 0xFFFF
    blocks = (cells + 15) // 16
    gate = (n + 15) // 16 + 1

    if rng.random() < 0.2:
        total = rng.randrange(0, gate)
    else:
        total = max(gate, blocks + 1) + rng.randrange(3)
    own = rng.randrange(0, min(5, total) + 1)
    free = total - own
    ids = rng.sample(range(1, 61), total)
    chain, pool = ids[:own], ids[own:]

    for table, size in (("data_80183ffc", 4), ("data_801841bc", 4), ("data_8018437c", 8),
                        ("data_80183910", 2), ("data_801839f0", 2), ("data_80183ad0", 2),
                        ("data_80183bb0", 2), ("data_80184704", 32), ("data_80185504", 16)):
        for offset in range(0, size * 64, 4):
            state.w32(sym[table] + offset, rng.getrandbits(32))
    for entry in range(64):
        state.w16(sym["data_80183c90"] + 2 * entry, rng.randrange(64))
    for order in (chain, pool):
        for a, b in zip(order, order[1:]):
            state.w16(sym["data_80183c90"] + 2 * a, b)

    j = rng.randrange(8)
    state.w16(sym["data_80183ffc"] + 4 * j + 2, chain[0] if chain else rng.randrange(64))
    state.w16(sym["data_801841bc"] + 4 * j + 2, own)
    state.w16(sym["data_801846fc"], pool[0] if pool else rng.randrange(1, 61))
    state.w16(sym["data_80184700"], free)

    q = state.alloc(10 + 2 * len(data) + 16)
    for offset in range(0, 10 + 2 * len(data) + 16, 2):
        state.w16(q + offset, rng.getrandbits(16))
    state.w16(q, n)
    state.w16(q + 2, mode)
    for index, value in enumerate(data):
        state.w16(q + 10 + 2 * index, value)

    p = state.alloc(0xB0)
    for offset in range(0, 0xB0, 4):
        state.w32(p + offset, rng.getrandbits(32))
    state.w16(p + 0x94, j)
    return Setup(args=(p, q), returns_value=False)


def control(words):
    """Alter the store of the new entry into the object: its offset moves by two bytes.

    `p->field_94 = entry` is the only halfword store at offset 0x94; every
    call past the early return makes it, so the object keeps its old entry
    and the neighbouring halfword changes.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0x94]
    if len(found) != 1:
        raise ValueError(f"expected one store of field_94, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x96, "store of the object's entry index moved by two bytes"


CONTRACT = Contract(setup, control)
