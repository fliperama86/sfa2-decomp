"""Contract of func_80078d78_slot00 (reads and writes: see the header of the .c).

Choices of the setup:
  - the low three bits of game_state.field_1d are 0 in four cases of five
    (the function does nothing otherwise); the rest of the byte is random;
  - the parent and the new object are random blocks of 0x394 bytes;
  - func_8011f1e0 is a recorder that returns the new object in three cases
    of four and 0 in the others; the log copies the new object and the
    parent at the call.
"""

from contracts import CallLog, Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym) -> Setup:
    parent = state.alloc(0x394)
    fill(state, parent, 0x394, rng)
    child = state.alloc(0x394)
    fill(state, child, 0x394, rng)
    low = 0 if rng.random() < 0.8 else rng.randrange(1, 8)
    state.w8(sym["game_state"] + 0x1D, (rng.randrange(32) << 3) | low)

    log = CallLog(state, 1024, watch=((child, 0x394 // 4), (parent, 0x394 // 4)))
    log.replace(sym["func_8011f1e0"], 0, child if rng.random() < 0.75 else 0)
    return Setup(args=(parent,), returns_value=False)


def control(words):
    """Alter the store of the child's field_7c: the halfword 0x1e0 becomes 0x1e1."""
    found = [i for i, w in enumerate(words) if w >> 26 == 13 and w & 0xFFFF == 0x1E0]
    if len(found) != 1:
        raise ValueError(f"expected one ori of 0x1e0, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x1E1, "size word 0x1e0 changed to 0x1e1"


CONTRACT = Contract(setup, control)
