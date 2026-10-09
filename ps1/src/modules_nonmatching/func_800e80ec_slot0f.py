"""Contract of func_800e80ec_slot0f (reads and writes: see the header of the .c).

Choices of the setup:
  - the object is random; field_03 is 0 to 7, field_5c is 0 to 4 (as an
    unsigned 16-bit value, so the sign extension is not exercised; see the
    header), game_state.field_2bd is 0 in half of the cases;
  - the second argument is 1 in half of the cases, otherwise a random value
    other than 1 (small, or a random word);
  - the record table (8 records) and the flag array start random;
  - func_80130768 is a recorder with 3 arguments returning 0; the log
    copies the object, the object's record and the flag bytes.
"""

from contracts import CallLog, Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym) -> Setup:
    records = sym["data_800f7d50_slot0f"]
    flags = sym["data_800f7d48_slot0f"]
    fill(state, records, 8 * 0x40, rng)
    fill(state, flags, 8, rng)
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    index = rng.randrange(8)
    state.w8(obj + 0x03, index)
    state.w16(obj + 0x5C, rng.randrange(5))
    state.w8(sym["game_state"] + 0x2BD, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    a = 1 if rng.random() < 0.5 else rng.choice((0, 2, 0xFF, rng.getrandbits(32)))
    log = CallLog(state, 512, watch=((obj, 0x394 // 4), (records + 0x40 * index, 16), (flags, 2)))
    log.replace(sym["func_80130768"], 3)
    return Setup(args=(obj, a), returns_value=False)


def control(words):
    """Alter the position: `addiu v0,v0,8` (obj pos_x = field_76 + 8) becomes +9."""
    found = [i for i, w in enumerate(words) if w >> 26 == 9 and (w >> 21) & 31 == (w >> 16) & 31 and w & 0xFFFF == 8]
    if not found:
        raise ValueError("expected an increment by 8")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 9, "increment by 8 changed to 9"


CONTRACT = Contract(setup, control)
