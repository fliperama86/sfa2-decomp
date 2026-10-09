"""Contract of func_801b27a0_slot04_17 (reads and writes: see the header of the .c).

Choices of the setup:
  - field_3a has bit 15 set in five cases of six (otherwise the function
    only calls func_80130efc, which is recorded);
  - field_cd is 0 in half of the cases, a non-zero byte otherwise;
  - field_cf is any byte; the word of the table data_801ce050_slot04_17 at
    that index is all ones, zero or random, so that func_8014a170 (original
    code, fed by a random seed word) answers both ways;
  - field_130 has bit 0x1000 set in half of the cases, the rest is random;
  - the other object's field_12 lies within 0x13 of the object's in half of
    the cases (the dead zone), else anywhere;
  - field_0b is 0 or 1 in four cases of five, else any byte; the first two
    words of data_801ce3e4_slot04_17 and the word at the index field_0b are
    random;
  - field_49 is 0 or non-zero with equal odds; every other field of the
    object is random;
  - every recorder copies the object, the other object, ref_other and the
    word of data_80190126 at each call (the watch);
  - func_801204f4, func_801307e0 and func_80130efc are recorders; their
    arguments are compared through the log.
"""

from contracts import CallLog, Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def _word(rng):
    return rng.choice((0, 0xFFFFFFFF, rng.getrandbits(32), rng.getrandbits(32)))


def setup(state, rng, sym) -> Setup:
    other = state.alloc(0x400)
    fill(state, other, 0x400, rng)
    obj = state.alloc(0x400)
    fill(state, obj, 0x400, rng)

    field_3a = rng.getrandbits(16)
    if rng.random() < 5 / 6:
        field_3a |= 0x8000
    else:
        field_3a &= 0x7FFF
    state.w16(obj + 0x3A, field_3a)
    state.w32(obj + 0x40, other)
    state.w8(obj + 0xCD, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    state.w8(obj + 0xCF, rng.randrange(256))
    field_130 = rng.getrandbits(16)
    field_130 = (field_130 | 0x1000) if rng.random() < 0.5 else (field_130 & ~0x1000)
    state.w16(obj + 0x130, field_130)
    state.w8(obj + 0x49, rng.choice((0, rng.randrange(256))))
    field_0b = rng.randrange(2) if rng.random() < 0.8 else rng.randrange(256)
    state.w8(obj + 0x0B, field_0b)

    x = rng.getrandbits(16)
    state.w16(obj + 0x12, x)
    if rng.random() < 0.5:
        state.w16(other + 0x12, (x + rng.randrange(-0x13, 0x14)) & 0xFFFF)
    else:
        state.w16(other + 0x12, rng.getrandbits(16))

    state.w32(sym["data_801ce050_slot04_17"] + 4 * state.read(obj + 0xCF, 1)[0], _word(rng))
    table = sym["data_801ce3e4_slot04_17"]
    for index in (0, 1, field_0b):
        state.w32(table + 4 * index, rng.getrandbits(32))
    state.w16(sym["data_80190126"], rng.getrandbits(16))

    log = CallLog(state, 2048, watch=((obj, 0x100), (other, 0x100), (sym["ref_other"], 1), (sym["data_80190126"] & ~3, 1)))
    log.replace(sym["func_801204f4"], 3)
    log.replace(sym["func_801307e0"], 2)
    log.replace(sym["func_80130efc"], 1)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of the sequence selector's base: the `ori v1,zero,0x2e`-like add.

    The instruction `addiu rt,rs,0x2e` adds the sequence base to the walk
    flag on every call that gets past the stop test.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 9 and w & 0xFFFF == 0x2E]
    if len(found) != 1:
        raise ValueError(f"expected one add of 0x2e, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x2F, "sequence base 0x2e changed to 0x2f"


CONTRACT = Contract(setup, control)
