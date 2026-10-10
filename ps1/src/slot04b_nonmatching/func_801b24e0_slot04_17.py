"""Contract of func_801b24e0_slot04_17 (reads and writes: see the header of the .c).

Choices of the setup:
  - field_3a has bit 15 set in five cases of six (otherwise the function
    only calls func_80130efc, which is recorded);
  - field_cd is 0 in half of the cases, a non-zero byte otherwise;
  - field_cf is any byte; the word of data_801ce050_slot04_17 at that index
    is all ones, zero or random, and the seed is random, so that
    func_8014a170 (original code) answers both ways and bit 0 of
    func_80151184 falls both ways;
  - field_130 has bit 0x1000 set in a third of the cases, bit 0x2000 in a
    third (each independent), bit 0x8000 in half; the rest is random;
  - field_0e is any byte and the entry of data_801ce778_slot04_17 at that
    index points to the other object;
  - field_0b is 0 or 1 in four cases of five, else any byte; field_49 is 0
    or non-zero with equal odds;
  - field_12a is 0..5 in nine cases of ten, else any byte; the eight words
    of data_801ce384_slot04_17 that the walk reads (the four at the row of
    each side) are random;
  - every other field of both objects is random, so the speed words, the
    positions and field_70 take any value;
  - func_801307e0 and func_80130efc are recorders; their arguments are
    compared through the log, and each call also copies the whole object
    block (0x100 words), the word ref_other and the word that holds the seed
    data_80190126, so that a store made on the other side of a call differs.
  - ref_other (the global word the function sets to the other object) is
    filled with a random word first, so that the function's store to it is
    seen.
"""

from contracts import CallLog, Contract, Setup


def _address(name: str) -> int:
    return int(name.split("_")[1], 16)


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def _word(rng):
    return rng.choice((0, 0xFFFFFFFF, rng.getrandbits(32), rng.getrandbits(32)))


def _bit(rng, mask, odds):
    value = rng.getrandbits(16)
    return (value | mask) if rng.random() < odds else (value & ~mask)


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
    field_cf = rng.randrange(256)
    state.w8(obj + 0xCF, field_cf)
    field_130 = _bit(rng, 0x1000, 1 / 3)
    field_130 = (field_130 | 0x2000) if rng.random() < 1 / 3 else (field_130 & ~0x2000)
    field_130 = (field_130 | 0x8000) if rng.random() < 0.5 else (field_130 & ~0x8000)
    state.w16(obj + 0x130, field_130)
    state.w8(obj + 0x0B, rng.randrange(2) if rng.random() < 0.8 else rng.randrange(256))
    state.w8(obj + 0x49, rng.choice((0, rng.randrange(256))))
    field_0e = rng.randrange(256)
    state.w8(obj + 0x0E, field_0e)
    state.w32(_address("data_801ce778_slot04_17") + 4 * field_0e, other)
    field_12a = rng.randrange(6) if rng.random() < 0.9 else rng.randrange(256)
    state.w8(obj + 0x12A, field_12a)

    state.w32(_address("data_801ce050_slot04_17") + 4 * field_cf, _word(rng))
    table = _address("data_801ce384_slot04_17")
    for base in (field_12a * 2, field_12a * 2 + 12):
        for entry in range(4):
            state.w32(table + 4 * (base + entry), rng.getrandbits(32))
    state.w16(sym["data_80190126"], rng.getrandbits(16))

    seed = sym["data_80190126"] & ~3
    state.w32(sym["ref_other"], rng.getrandbits(32))
    log = CallLog(state, 1024, watch=((obj, 0x100), (sym["ref_other"], 1), (seed, 1)))
    log.replace(sym["func_801307e0"], 2)
    log.replace(sym["func_80130efc"], 1)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the stop case's field_50: `lui v0,1` (0x10000) becomes 0x20000."""
    found = [i for i, w in enumerate(words) if w >> 26 == 15 and w & 0xFFFF == 1 and (w >> 16) & 31 == 2]
    if len(found) != 1:
        raise ValueError(f"expected one lui v0,1, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 2, "stop-case field_50 0x10000 changed to 0x20000"


CONTRACT = Contract(setup, control)
