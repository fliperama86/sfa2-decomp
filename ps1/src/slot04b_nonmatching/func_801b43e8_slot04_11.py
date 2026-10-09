"""Contract of func_801b43e8_slot04_11 (reads and writes: see the header of the .c).

Choices of the setup:
  - field_3a has bit 15 set in five cases of six (otherwise the function
    only calls func_80130efc, which is recorded);
  - field_cd is 0 in half of the cases, a non-zero byte otherwise;
  - field_1c0 is zero, a single key bit (8, 0x20, 0x40) or random, so that
    the collector (func_801b4970, original code) both keeps and replaces it;
    field_134 is zero in a fifth of the cases, else random;
  - field_cf is any byte; the word of data_801c719c_slot04_11 at that index
    is all ones, zero or random, and the seed is random, so that
    func_8014a170 (original code) answers both ways;
  - field_0e is any byte and the entry of data_801c7844_slot04_11 at that
    index points to the other object; the sixteen bytes of
    data_801c77a8_slot04_11 and the thirty-two of data_801c7798_slot04_11
    are random picks among the key bits 8, 0x20, 0x40 and 0;
  - the other object's field_12 lies from -0x40 to +0x180 from the
    object's, or exactly at one of the cutoffs 0x7f, 0x80, 0xff, 0x100 (two
    cases in five), so that the distance falls in each of the three ranges
    and on both sides of each cutoff;
  - field_0b is 0 or 1 in four cases of five, else any byte;
  - field_12a is 0..3 in nine cases of ten, else any byte; field_12e is any
    byte; the words of data_801c7648_slot04_11 that the function reads (the
    four of the row entry of each variant and the sequence word) are random;
  - every other field is random;
  - func_801307e0 and func_80130efc are recorders; their arguments are
    compared through the log, and each call also copies the whole object
    block (0x100 words), the word ref_other and the word that holds the seed
    data_80190126, so that a store made on the other side of a call differs.
"""

from contracts import CallLog, Contract, Setup


def _address(name: str) -> int:
    return int(name.split("_")[1], 16)


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def _word(rng):
    return rng.choice((0, 0xFFFFFFFF, rng.getrandbits(32), rng.getrandbits(32)))


def _key(rng):
    return rng.choice((0, 8, 0x20, 0x40, rng.randrange(256)))


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
    state.w16(obj + 0x1C0, rng.choice((0, 0, _key(rng), rng.getrandbits(16))))
    state.w16(obj + 0x134, 0 if rng.random() < 0.2 else rng.getrandbits(16))
    field_0b = rng.randrange(2) if rng.random() < 0.8 else rng.randrange(256)
    state.w8(obj + 0x0B, field_0b)
    field_0e = rng.randrange(256)
    state.w8(obj + 0x0E, field_0e)
    state.w32(_address("data_801c7844_slot04_11") + 4 * field_0e, other)
    field_12a = rng.randrange(4) if rng.random() < 0.9 else rng.randrange(256)
    field_12e = rng.randrange(256)
    state.w8(obj + 0x12A, field_12a)
    state.w8(obj + 0x12E, field_12e)

    x = rng.getrandbits(16)
    state.w16(obj + 0x12, x)
    distance = rng.choice((0x7F, 0x80, 0xFF, 0x100, rng.randrange(-0x40, 0x180)))
    state.w16(other + 0x12, (x + distance) & 0xFFFF)

    state.w32(_address("data_801c719c_slot04_11") + 4 * field_cf, _word(rng))
    for index in range(16):
        state.w8(_address("data_801c77a8_slot04_11") + index, _key(rng))
    for index in range(32):
        state.w8(_address("data_801c7798_slot04_11") + index, _key(rng))
    table = _address("data_801c7648_slot04_11")
    for variant in range(3):
        for entry in range(4):
            state.w32(table + 4 * (field_12a * 8 + variant * 4 + entry), rng.getrandbits(32))
    state.w32(table + 4 * (field_12a * 8 + field_12e + 12), rng.getrandbits(32))
    state.w16(sym["data_80190126"], rng.getrandbits(16))

    seed = sym["data_80190126"] & ~3
    log = CallLog(state, 1024, watch=((obj, 0x100), (sym["ref_other"], 1), (seed, 1)))
    log.replace(sym["func_801307e0"], 2)
    log.replace(sym["func_80130efc"], 1)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the cutoff of the distance test: `slti v0,a3,0x100` becomes 0x101."""
    found = [i for i, w in enumerate(words) if w >> 26 == 10 and w & 0xFFFF == 0x100]
    if len(found) != 1:
        raise ValueError(f"expected one slti 0x100, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x101, "distance cutoff 0x100 changed to 0x101"


CONTRACT = Contract(setup, control)
