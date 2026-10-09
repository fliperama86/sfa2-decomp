"""Contract of func_8013db48: step one slot of an object against the table entry at step 1.

Choices of the setup:
  - the object is a random block that holds the 256 slots of 8 bytes the
    index can reach; the slot used has field_04 equal to 1 in a fifth of the
    cases (it reaches 0), otherwise 2 to 255;
  - the table is filled whole (256 arguments of 14 bytes) with random bytes;
    the entry the call uses (halfword 1 of the argument's 14 bytes) is built
    to steer: bits 0x200, 0x100 and 0x800 each set with probability a half,
    and a mask byte chosen from 0x94, 0x68, 0x03, 0xfc, 0 or random;
  - field_134 and field_136 are random halfwords ORed to a pad that is, in a
    third of the cases, exactly 1 or 2; otherwise steered in a half of the
    cases so that the pad shares no bit of 0xfc with the mask, and in a
    quarter so that pad & mask has the mask's bits 0xfc;
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
    table = sym["table_8017a8cc"]
    fill(state, table, 14 * 256 + 2, rng)
    mask = rng.choice((0x94, 0x68, 0x03, 0xFC, 0, rng.getrandbits(8)))
    pad = rng.getrandbits(16)
    roll = rng.random()
    if roll < 0.5:
        pad &= ~(mask & 0xFC)
    elif roll < 0.75:
        pad |= mask & 0xFC
    if rng.random() < 1 / 3:
        pad = rng.choice((1, 2))
    split = rng.getrandbits(16)
    state.w16(obj + 0x134, pad & split)
    state.w16(obj + 0x136, pad & ~split)
    entry = (rng.getrandbits(16) & 0xFF00 & ~0x0B00) | mask
    for bit in (0x200, 0x100, 0x800):
        if rng.random() < 0.5:
            entry |= bit
    state.w16(table + 2 * (arg_low * 7 + 1), entry)
    index = (rng.getrandbits(24) << 8) | index_low
    arg = (rng.getrandbits(24) << 8) | arg_low
    return Setup(args=(obj, index, arg), returns_value=False)


def control(words):
    """Replace the first `beq v0,zero` of the function by a nop (never taken)."""
    found = [i for i, w in enumerate(words) if w >> 26 == 4 and (w >> 21) & 31 == 2 and (w >> 16) & 31 == 0]
    if not found:
        raise ValueError("no beq v0,zero")
    return found[0], 0, "first branch on v0 == 0 removed"


CONTRACT = Contract(setup, control)
