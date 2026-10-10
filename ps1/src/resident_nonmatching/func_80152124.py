"""Contract of func_80152124 (see the header comment of the .c file).

Choices made here:
  - c is zero in one case of five (the function then does nothing),
    otherwise random bytes with the high bits of the register set about half
    the time (only the low byte counts);
  - a0, a1, d and e are random; e is placed in the fifth argument slot of the
    caller's stack (sp + 0x10 at the call), as a halfword with random high
    bits;
  - the command buffer pointer points at record 0 or 1 of a block of 3
    records filled with random bytes, data_8018d208 is random;
  - func_8015bf34 is a recorder taking two arguments; the record that its
    second argument points at (5 words) is copied into the log at the call;
    the three records of the buffer and the buffer pointer
    data_8018d13c are watched (copied at every call).
"""

from contracts import CallLog, Contract, Setup

STACK_TOP = 0x801FF000


def setup(state, rng, sym):
    block = state.alloc(0x14 * 3)
    state.write(block, bytes(rng.getrandbits(8) for _ in range(0x14 * 3)))
    state.w32(sym["data_8018d13c"], block + 0x14 * rng.randrange(2))
    state.w16(sym["data_8018d208"], rng.getrandbits(16))
    log = CallLog(state, watch=((block, 0x14 * 3 // 4), (sym["data_8018d13c"], 1)))
    log.replace(sym["func_8015bf34"], 2, rng.getrandbits(32), pointees={1: 5})
    c = 0 if rng.random() < 0.2 else rng.getrandbits(8)
    if rng.random() < 0.5:
        c |= rng.getrandbits(24) << 8
    # the fifth argument e: the caller's stack slot at the entry sp + 0x10
    state.w32(STACK_TOP + 0x10, rng.getrandbits(32))
    return Setup(args=(rng.getrandbits(32), rng.getrandbits(32), c, rng.getrandbits(16)), returns_value=False)


def control(words):
    """Alter the store of the 0x10 constant into the first of its two fields."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0x10]
    if len(found) != 1:
        raise ValueError(f"expected one store at offset 0x10, found {len(found)}")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0x0E, "store of the first constant moved to offset 0xe"


CONTRACT = Contract(setup, control)
