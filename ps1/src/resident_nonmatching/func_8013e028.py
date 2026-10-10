"""Contract of func_8013e028: step one slot of an object against a table entry.

Choices of the setup:
  - data_80188f44, the global byte that the leaf callees set (to 0 or 1;
    read from the original's listing, not tested), starts random, so that a
    call that leaves it alone is seen;
  - the object is a random block that holds the 256 slots of 8 bytes the
    index can reach; the slot used has field_04 equal to 1 in a fifth of the
    cases (it reaches 0), otherwise 2 to 255; its field_01 is random;
  - the table is filled whole (256 arguments of 14 bytes, and the reach of
    field_01 past them) with random bytes; the entry the call uses is built
    to steer: bits 0x200, 0x100 and 0x800 each set with probability a half,
    and a mask byte chosen from 0x94, 0x68, 0x03, 0xfc, 0 or random;
  - field_134 and field_136 are random halfwords, steered in a half of the
    cases so that the pad shares no bit of 0xfc with the mask, and in a
    quarter so that the pad has every bit of the mask's 0xfc part (pad &
    mask is then 0x94 when the mask is 0x94, 0x68 when it is 0x68); the
    pad's low two bits are set to 1 or 2 in a half of the cases;
  - a1 and a2 are random words (only the low byte counts);
  - no callee is replaced.
"""
from contracts import Contract, Setup, fill


def setup(state, rng, sym):
    size = 0x2B0 + 8 * 256
    obj = state.alloc(size + 16)
    fill(state, obj, size, rng)
    index_low = rng.randrange(256)
    arg_low = rng.randrange(256)
    slot = obj + 0x2B0 + 8 * index_low
    state.w8(slot + 4, 1 if rng.random() < 0.2 else rng.randrange(2, 256))
    step = rng.getrandbits(8)
    state.w8(slot + 1, step)
    table = sym["table_8017a8cc"]
    fill(state, table, 14 * 256 + 2 * 256, rng)
    mask = rng.choice((0x94, 0x68, 0x03, 0xFC, 0, rng.getrandbits(8)))
    pad = rng.getrandbits(16)
    roll = rng.random()
    if roll < 0.5:
        pad &= ~(mask & 0xFC)
    elif roll < 0.75:
        pad |= mask & 0xFC
    if rng.random() < 0.5:
        pad = (pad & ~3) | rng.choice((1, 2))
    split = rng.getrandbits(16)
    state.w16(obj + 0x134, pad & split)
    state.w16(obj + 0x136, pad & ~split)
    entry = (rng.getrandbits(16) & 0xFF00 & ~0x0B00) | mask
    for bit in (0x200, 0x100, 0x800):
        if rng.random() < 0.5:
            entry |= bit
    state.w16(table + 2 * (arg_low * 7 + step), entry)
    index = (rng.getrandbits(24) << 8) | index_low
    arg = (rng.getrandbits(24) << 8) | arg_low
    # the global byte that func_8013f2a8, func_8013f2c8 and func_8013f2d8
    # set (0 or 1; read from the original's listing, not tested) starts
    # random, so that a call that leaves it alone is seen
    state.w8(sym["data_80188f44"], rng.getrandbits(8))
    return Setup(args=(obj, index, arg), returns_value=False)


def control(words):
    """Replace the first `beq v0,zero` of the function by a nop (never taken)."""
    found = [i for i, w in enumerate(words) if w >> 26 == 4 and (w >> 21) & 31 == 2 and (w >> 16) & 31 == 0]
    if not found:
        raise ValueError("no beq v0,zero")
    return found[0], 0, "first branch on v0 == 0 removed"


CONTRACT = Contract(setup, control)
