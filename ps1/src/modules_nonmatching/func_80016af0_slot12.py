"""Contract of func_80016af0_slot12 (see the header of the .c file).

Choices of the setup:
  - idx is 2 in a third of the cases (the second table), else random below
    76; the upper 16 bits of a2 are random in half of the cases;
  - the pointer tables hold pointers to 16-byte lists of random bytes
    (halfword aligned); the halfword table has 80 random entries; field_48
    is below 61;
  - the object is 0x394 random bytes; the two records are 0x40 random bytes;
    data_801987c8 points at a 0x100-byte random block;
  - the callee is a recorder with a random result, taking 4 arguments; the
    log watches the two records at the call.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    lists = state.alloc(16 * 8)
    fill(state, lists, 16 * 8, rng)
    for table, count in (("data_800287ec_slot12", 76), ("data_8002891c_slot12", 61)):
        for i in range(count):
            state.w32(sym[table] + 4 * i, lists + 16 * rng.randrange(8))
    fill(state, sym["data_80028a10_slot12"], 160, rng)
    block = state.alloc(0x100)
    fill(state, block, 0x100, rng)
    state.w32(sym["data_801987c8"], block)
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x48, rng.randrange(61))
    prims = state.alloc(0x40)
    fill(state, prims, 0x40, rng)
    idx = 2 if rng.random() < 1 / 3 else rng.randrange(76)
    if rng.random() < 0.5:
        idx |= rng.getrandbits(16) << 16
    log = CallLog(state, 16, watch=((prims, 16),))
    log.replace(sym["func_8015bd0c"], 4, result=rng.getrandbits(32))
    return Setup(args=(obj, prims, idx), returns_value=False)


def control(words):
    """Alter the texture page constant: `ori v0,zero,0x7f07` becomes 0x7f08."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0xD and w & 0xFFFF == 0x7F07]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x7f07, found {len(found)}")
    index = found[0]
    return index, words[index] + 1, "texture page constant 0x7f07 changed to 0x7f08"


CONTRACT = Contract(setup, control)
