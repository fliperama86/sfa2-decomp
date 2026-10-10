"""Contract of func_8013ec50: an object's flag halfword against a table entry.

Choices of the setup:
  - the object is a random block of 0x394 bytes; field_150 is a random
    halfword;
  - the table (256 halfwords) is random; the entry the call uses has bits
    0x200, 0x100 and 0x800 each set with probability a half, and a mask byte
    that is, in a half of the cases, a random subset of field_150's low byte
    (so that the mask test passes; in three cases of ten one random bit is
    added, which may already be set in field_150), otherwise random;
  - a1 and a2 are random words (only the low byte counts);
  - func_8013ed1c (2 arguments) and func_8013f2a8 (3) are recorders (result 0). Every recorder copies the object whole (0x394 bytes)
    into its log entry at the call.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    log = CallLog(state, words=512, watch=((obj, 0x394 // 4),))
    log.replace(sym["func_8013ed1c"], 2, 0)
    log.replace(sym["func_8013f2a8"], 3, 0)
    fill(state, obj, 0x394, rng)
    flags = rng.getrandbits(16)
    state.w16(obj + 0x150, flags)
    table = sym["table_8017ab5c"]
    fill(state, table, 512, rng)
    arg_low = rng.randrange(256)
    mask = rng.getrandbits(8)
    if rng.random() < 0.5:
        mask &= flags
        if rng.random() < 0.3:
            mask |= 1 << rng.randrange(8)  # one more bit; it may already be set in field_150
    entry = (rng.getrandbits(16) & 0xFF00 & ~0x0B00) | mask
    for bit in (0x200, 0x100, 0x800):
        if rng.random() < 0.5:
            entry |= bit
    state.w16(table + 2 * arg_low, entry)
    index = rng.getrandbits(32)
    arg = (rng.getrandbits(24) << 8) | arg_low
    return Setup(args=(obj, index, arg), returns_value=False)


def control(words):
    """Replace the first `beq v0,zero` by a nop: it is never taken.

    It is the entry's 0x100 test (read from the original's listing, not tested).
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 4 and (w >> 21) & 31 == 2 and (w >> 16) & 31 == 0]
    if not found:
        raise ValueError("no beq v0,zero")
    return found[0], 0, "branch of the 0x100 test removed"


CONTRACT = Contract(setup, control)
