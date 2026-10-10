"""Contract of func_8013ed90: an object's flag halfword against a table entry, then a slot.

Choices of the setup:
  - the object is a random block that holds the 256 slots of 8 bytes the
    index can reach; field_150 is a random halfword; field_7e is 0 in half of
    the cases; each slot's field_02 is a small number (0 to 0x3c8) in three
    cases of four, otherwise random (func_8013f2fc picks a byte from it);
  - the table (256 halfwords) is random; the entry the call uses has bits
    0x200, 0x100 and 0x800 each set with probability a half, and a mask byte
    that is, in a half of the cases, a random subset of field_150's low byte
    (so that the mask test passes; in three cases of ten one random bit is
    added), otherwise random;
  - a1 and a2 are random words (only the low byte counts);
  - no callee is replaced; the global byte data_80188f44, which func_8013f2c8
    and func_8013f2fc write (0 or 1), starts as a random byte of 2 to 255, so
    that a call that leaves it alone is seen to leave it alone.
"""
from contracts import Contract, Setup, fill


def setup(state, rng, sym):
    size = 0x2B0 + 8 * 256
    obj = state.alloc(size + 16)
    fill(state, obj, size, rng)
    flags = rng.getrandbits(16)
    state.w16(obj + 0x150, flags)
    if rng.random() < 0.5:
        state.w8(obj + 0x7E, 0)
    for slot in range(256):
        if rng.random() < 0.75:
            state.w16(obj + 0x2B0 + 8 * slot + 2, rng.randrange(0x3C9))
    table = sym["table_8017ab5c"]
    fill(state, table, 512, rng)
    arg_low = rng.randrange(256)
    mask = rng.getrandbits(8)
    if rng.random() < 0.5:
        mask &= flags
        if rng.random() < 0.3:
            mask |= 1 << rng.randrange(8)  # one bit more than field_150 has
    entry = (rng.getrandbits(16) & 0xFF00 & ~0x0B00) | mask
    for bit in (0x200, 0x100, 0x800):
        if rng.random() < 0.5:
            entry |= bit
    state.w16(table + 2 * arg_low, entry)
    state.w8(sym["data_80188f44"], rng.randrange(2, 256))
    index = rng.getrandbits(32)
    arg = (rng.getrandbits(24) << 8) | arg_low
    return Setup(args=(obj, index, arg), returns_value=False)


def control(words):
    """Replace the first `beq v0,zero` (the entry's 0x100 test) by a nop: it is never taken."""
    found = [i for i, w in enumerate(words) if w >> 26 == 4 and (w >> 21) & 31 == 2 and (w >> 16) & 31 == 0]
    if not found:
        raise ValueError("no beq v0,zero")
    return found[0], 0, "branch of the 0x100 test removed"


CONTRACT = Contract(setup, control)
