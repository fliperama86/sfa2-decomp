"""Contract of func_80146794 in the port's C (header comment of the .c file).

Choices of the setup:
  - the object is a 0x394-byte block of random bytes; its byte field_03 is
    0x10 or 0x11 in two cases of five (the pos_x arms), otherwise random; its
    byte at 8 is 0 or 8 in half of the cases (the arm of func_80131094 that
    reads the frame table);
  - three kinds of case in equal shares, for the three paths of
    func_80131094, which runs as the image's own code: kind 0, the halfword at
    0x38 is not 1 (0 or 2 to 65535); kind 1, it is 1 and the halfword at 0x3a
    is 0 to 0x7fff; kind 2, it is 1 and the halfword at 0x3a is 0x8000 to
    0xffff. In kind 0 the halfword at 0x3a is any value;
  - the word at 0x18 (the sequence pointer) leads to a 48-byte block of random
    bytes, four records of 12 bytes, that this setup builds; the halfword at 8
    of the first record is 0 to 3 and the halfword at 0xa of each record is 0
    to 15; the word at 0x8c leads to a block of 16 records of 16 bytes of
    random bytes. These are the memory that func_80131094 reads through the
    two pointers, and it writes nothing of them;
  - func_80120028 (1 argument) is a recorder that returns 0; the log watches
    the object whole at every call. The sequence block is not watched: the
    callee func_80131094 does not write it.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym) -> Setup:
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    if rng.random() < 0.4:
        state.w8(obj + 0x03, rng.choice((0x10, 0x11)))
    if rng.random() < 0.5:
        state.w8(obj + 0x08, rng.choice((0, 8)))

    seq = state.alloc(48)
    fill(state, seq, 48, rng)
    for record in range(4):
        state.w16(seq + 12 * record + 0xA, rng.randrange(16))
    state.w16(seq + 8, rng.randrange(4))
    frames = state.alloc(16 * 16)
    fill(state, frames, 16 * 16, rng)
    state.w32(obj + 0x18, seq)
    state.w32(obj + 0x8C, frames)

    kind = rng.randrange(3)
    if kind == 0:
        counter = rng.choice((0, rng.randrange(2, 0x10000)))
        state.w16(obj + 0x3A, rng.getrandbits(16))
    else:
        counter = 1
        state.w16(obj + 0x3A, rng.randrange(0, 0x8000) if kind == 1 else rng.randrange(0x8000, 0x10000))
    state.w16(obj + 0x38, counter)

    log = CallLog(state, 256, watch=((obj, 0x394 // 4),))
    log.replace(sym["func_80120028"], 1, 0)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the argument register at the call: `move a0,s0` in the delay slot of `jal func_80120028` becomes `addiu a0,s0,4`.

    The build of this override has no empty delay slot there (the compiler puts the move of the argument into it),
    so the control alters that word instead of an empty one. It alters the build of this C (the tool hands it the
    build's code words, not the original's). The altered build then hands the callee its argument plus 4, which the
    recorder logs, while the original code hands it the value the console's code leaves in a0: this is the
    alteration that shows the test sees the value the callee gets.
    """
    jal = 0x0C000000 | ((0x80120028 >> 2) & 0x03FFFFFF)
    found = [i for i, w in enumerate(words) if w == jal]
    if len(found) != 1 or words[found[0] + 1] != 0x02002021:
        raise ValueError("expected one call of func_80120028 with `move a0,s0` in its delay slot")
    return found[0] + 1, 0x26040004, "the callee gets its argument + 4"


CONTRACT = Contract(setup, control)
