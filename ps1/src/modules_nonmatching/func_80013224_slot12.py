"""Contract of func_80013224_slot12 (see the header of the .c file).

Choices of the setup:
  - a (a1) is 0xff in a third of the cases; otherwise it is one of the nine
    table bytes in half of the cases and a random byte else; the upper 24
    bits of a1 are random in half of the cases; x and y (a2, a3) are random
    words;
  - the object is 0x394 random bytes; field_48 is 9 in a third of the cases,
    else random;
  - the primitive pointer lies inside a random block of 0x100 bytes, at
    offset 0x40, so that the primitive before it exists; the word behind
    data_8002b618_slot12 is in a block of its own; data_801a27d0 is a small
    non-negative number in three cases of four, else a random word;
  - both callees are recorders (1 and 2 arguments), the second with a random
    result; the log watches the primitive and the pointer word.
"""
from contracts import CallLog, Contract, Setup, fill

TABLE = (0x4D, 0x4E, 0x4F, 0x5D, 0x5E, 0x5F, 0x69, 0x6A, 0x6B)


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    if rng.random() < 1 / 3:
        state.w8(obj + 0x48, 9)
    block = state.alloc(0x100)
    fill(state, block, 0x100, rng)
    cur = block + 0x40
    state.w32(sym["data_8002b614_slot12"], cur)
    ot = state.alloc(8)
    fill(state, ot, 8, rng)
    state.w32(sym["data_8002b618_slot12"], ot)
    state.w32(sym["data_801a27d0"], rng.randrange(4) if rng.random() < 0.75 else rng.getrandbits(32))
    roll = rng.random()
    if roll < 1 / 3:
        a = 0xFF
    elif roll < 2 / 3:
        a = rng.choice(TABLE)
    else:
        a = rng.randrange(256)
    if rng.random() < 0.5:
        a |= rng.getrandbits(24) << 8
    log = CallLog(state, 4 * 16, watch=((cur, 5), (sym["data_8002b614_slot12"], 1)))
    log.replace(sym["func_8015c100"], 1)
    log.replace(sym["func_8015bdd4"], 2, result=rng.getrandbits(32))
    return Setup(args=(obj, a, rng.getrandbits(32), rng.getrandbits(32)), returns_value=False)


def control(words):
    """Alter the grey level: `ori a0,zero,0x80` becomes 0x81 (stored as red, green and blue)."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0xD and w & 0xFFFF == 0x80]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x80, found {len(found)}")
    index = found[0]
    return index, words[index] + 1, "grey level 0x80 changed to 0x81"


CONTRACT = Contract(setup, control)
