"""Contract of func_8013e1e4: one slot of an object against a table entry.

Choices of the setup:
  - data_80188f44, the global byte that the leaf callees set (to 0 or 1),
    starts random, so that a call that leaves it alone is seen;
  - the object is a random block that holds the 256 slots of 8 bytes the
    index can reach; field_130 and field_134 are random halfwords;
  - arguments a1 and a2 are random words (only the low byte counts; the upper
    bytes are random so that the masking is tested);
  - the table has its 256 entries of 14 bytes filled with random bytes; the
    first halfword of the entry the call uses is built to steer: bit 0x400 and
    bit 1 are each set in half of the cases, and the mask part (0xf0ff) is
    either random, or chosen to have no bit in common with field_134, or to
    equal field_130's part, in a third of the cases each;
  - func_8013f2a8 and func_8013f2d8 (short leaf functions of game code) run as
    the original code in both runs; func_8013f0c8 is a recorder (3 arguments,
    result 0). Every recorder copies the object whole (0x2b0 + 8 * 256
    bytes, the reachable slots included) into its log entry at the call.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    obj = state.alloc(0x2B0 + 8 * 256 + 16)
    log = CallLog(state, words=2048, watch=((obj, (0x2B0 + 8 * 256) // 4),))
    log.replace(sym["func_8013f0c8"], 3, 0)
    fill(state, obj, 0x2B0 + 8 * 256, rng)
    f130, f134 = rng.getrandbits(16), rng.getrandbits(16)
    state.w16(obj + 0x130, f130)
    state.w16(obj + 0x134, f134)
    table = sym["table_8017a8cc"]
    fill(state, table, 256 * 14, rng)
    arg_low = rng.randrange(256)
    mask = rng.choice((rng.getrandbits(16) & 0xF0FF,
                       rng.getrandbits(16) & 0xF0FF & ~f134,
                       f130 & 0xF0FF,
                       f130 & 0xF0FF & ~f134))
    entry = (mask & ~0x0401) | (rng.getrandbits(1) << 10) | rng.getrandbits(1)
    state.w16(table + arg_low * 14, entry & 0xFFFF)
    index = rng.getrandbits(32)
    arg = (rng.getrandbits(24) << 8) | arg_low
    # the global byte that func_8013f2a8, func_8013f2c8 and func_8013f2d8 set
    # (0 or 1) starts random, so that a call that leaves it alone is seen
    state.w8(sym["data_80188f44"], rng.getrandbits(8))
    return Setup(args=(obj, index, arg), returns_value=False)


def control(words):
    """Replace the first conditional branch `beq v0,zero` on the mask test by a nop (never taken)."""
    found = [i for i, w in enumerate(words) if w >> 26 == 4 and (w >> 21) & 31 == 2 and (w >> 16) & 31 == 0]
    if not found:
        raise ValueError("no beq v0,zero")
    return found[0], 0, "branch to the first arm removed"


CONTRACT = Contract(setup, control)
