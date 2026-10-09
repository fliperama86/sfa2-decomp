"""Contract of func_80012a0c_slot12 (see the header of the .c file).

Choices of the setup:
  - the object is 0x394 bytes of random content; field_03 is 0 in half of the
    cases, else random non-zero (the two arms choose the record table);
  - the offset table (28 halfwords) and both record tables (0x200 + 14 * 16
    bytes each, from the table base) are random, so that the bytes the
    function leaves alone (the tag words) are tested too;
  - both callees are recorders (1 and 2 arguments); the log watches the two
    runs of 14 records of the table the case chooses, at every call.
"""
from contracts import CallLog, Contract, Setup, fill

TILE_WORDS = 14 * 16 // 4


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    selector = 0 if rng.random() < 0.5 else rng.randrange(1, 256)
    state.w8(obj + 3, selector)
    fill(state, sym["data_80022e48_slot12"], 56, rng)
    first = sym["data_8002a394_slot12"]
    second = sym["data_8002a794_slot12"]
    fill(state, first, 0x400 + 0x200 + 14 * 16, rng)
    fill(state, second, 0x200 + 14 * 16, rng)
    base = first if selector == 0 else second
    log = CallLog(state, 56 * (3 + 2 * TILE_WORDS) + 16, watch=((base, TILE_WORDS), (base + 0x200, TILE_WORDS)))
    log.replace(sym["func_8015c150"], 1)
    log.replace(sym["func_8015bfe8"], 2)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the width constant: `ori fp,zero,0xc0` becomes 0xc1 (`sh fp` stores it)."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0xD and w & 0xFFFF == 0xC0 and (w >> 21) & 31 == 0]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0xc0, found {len(found)}")
    index = found[0]
    return index, words[index] + 1, "width constant 0xc0 changed to 0xc1"


CONTRACT = Contract(setup, control)
