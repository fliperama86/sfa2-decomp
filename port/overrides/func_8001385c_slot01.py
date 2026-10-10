"""Contract of func_8001385c_slot01 in the port's C (header comment of the .c file).

Choices of the setup:
  - the object is a 0x394-byte block of random bytes; its byte field_05
    (offset 5) is 0, 1 or 2 in equal shares, so that each of the three
    entries of the table data_80015a64_slot01 runs;
  - the table and its three entries are the image's own and run as they are:
    the setup does not write the table. The entries read and write the object
    (bytes at 4 and 5, halfwords at 0x12 and 0x16, the pointer at 0x3c) and
    read the halfwords at 0x801901dc (entry 0) and 0x801901da (entry 1), which
    this setup writes (small values, -2000 to 1999);
  - entry 0: the object's halfword 0x16 lies so far from the halfword at
    0x801901dc that the difference is 0 or less, 1 to 8, or more than 8, in
    equal shares; entry 1: the object's halfword 0x12 lies -10 to 10 from the
    halfword at 0x801901da (both directions, and the call of the leaf at
    80013a24 is reached);
  - entry 2: the pointer at 0x3c leads to a separate 8-byte block of random
    bytes, whose halfword at 4 is 0x101 in half of the cases;
  - func_8011ffdc (1 argument) is a recorder that returns 0; the log watches
    the object whole and the 8-byte block at every call.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym) -> Setup:
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x05, rng.randrange(3))
    aux = state.alloc(8)
    fill(state, aux, 8, rng)
    if rng.random() < 0.5:
        state.w16(aux + 4, 0x101)
    state.w32(obj + 0x3C, aux)
    limit0 = rng.randrange(-2000, 2000)
    limit1 = rng.randrange(-2000, 2000)
    state.w16(0x801901DC, limit0 & 0xFFFF)
    state.w16(0x801901DA, limit1 & 0xFFFF)
    delta = rng.choice((rng.randrange(-20, 1), rng.randrange(1, 9), rng.randrange(9, 40)))
    state.w16(obj + 0x16, (limit0 - delta) & 0xFFFF)
    state.w16(obj + 0x12, (limit1 + rng.randrange(-10, 11)) & 0xFFFF)
    log = CallLog(state, 256, watch=((obj, 0x394 // 4), (aux, 2)))
    log.replace(sym["func_8011ffdc"], 1, 0)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the argument register at the call: `move a0,s0` in the delay slot of `jal func_8011ffdc` becomes `addiu a0,s0,4`.

    The build of this override has no empty delay slot there (the compiler puts the move of the argument into it),
    so the control alters that word instead of an empty one. It alters the build of this C (the tool hands it the
    build's code words, not the original's). The altered build then hands the callee the object plus 4, which the
    recorder logs, while the original code hands it the value the console's code leaves in a0: this is the
    alteration that shows the test sees the value the callee gets.
    """
    jal = 0x0C000000 | ((0x8011FFDC >> 2) & 0x03FFFFFF)
    found = [i for i, w in enumerate(words) if w == jal]
    if len(found) != 1 or words[found[0] + 1] != 0x02002021:
        raise ValueError("expected one call of func_8011ffdc with `move a0,s0` in its delay slot")
    return found[0] + 1, 0x26040004, "the callee gets its argument + 4"


CONTRACT = Contract(setup, control)
