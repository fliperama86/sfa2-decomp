"""Contract of func_800796e4_slot2b (reads and writes: see the header of the .c).

Choices of the setup:
  - the object, the parent (its field_3c), and the new object are random
    blocks of 0x394 bytes; the 32 halfwords of data_8007ee64_slot2b and the
    random seed data_80190126 are random;
  - func_8011f1e0 is a recorder that returns the new object in three cases
    of four and 0 otherwise; func_801204f4 is a recorder; the log copies
    the new object, the object and the parent at every call;
  - func_80151184 runs as the original, from the random seed.
"""

from contracts import CallLog, Contract, Setup


def block(state, rng, size):
    address = state.alloc(size)
    state.write(address, rng.randbytes(size))
    return address


def setup(state, rng, sym) -> Setup:
    obj = block(state, rng, 0x394)
    parent = block(state, rng, 0x394)
    child = block(state, rng, 0x394)
    state.w32(obj + 0x3C, parent)
    state.write(sym["data_8007ee64_slot2b"], rng.randbytes(64))
    state.w16(sym["data_80190126"], rng.getrandbits(16))

    log = CallLog(state, 4096, watch=((child, 0x394 // 4), (obj, 0x394 // 4), (parent, 0x394 // 4)))
    log.replace(sym["func_8011f1e0"], 0, child if rng.random() < 0.75 else 0)
    log.replace(sym["func_801204f4"], 3)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the stored size word: `ori v0,zero,0x1e0` becomes 0x1e1."""
    found = [i for i, w in enumerate(words) if w >> 26 == 13 and w & 0xFFFF == 0x1E0]
    if len(found) != 1:
        raise ValueError(f"expected one ori of 0x1e0, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x1E1, "size word 0x1e0 changed to 0x1e1"


CONTRACT = Contract(setup, control)
