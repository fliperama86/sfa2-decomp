"""Contract of func_801b34e4_slot04_17 (reads and writes: see the header of the .c).

Choices of the setup:
  - field_3a: the low byte is 0 in a sixth of the cases, has bit 0x80 set
    in a third, and is 1 to 0x7f otherwise (values 1 to 7 included, which
    read the halfword table below its start); the high byte is random;
  - field_0b is 0 or random non-zero with equal odds;
  - field_4c and field_54 are random words, so field_4c + field_54 is
    negative in about half of the cases; field_10 is a random word;
  - field_12a is any byte; the byte table data_801ce44c_slot04_17 and the
    halfword table data_801ce454_slot04_17 are overwritten with random
    bytes (the range 0x801ce444 to 0x801ce554, which nothing in this
    function's run reads as anything else);
  - the other object's field_5c has its sign set in half of the cases, and
    every other field of both objects is random;
  - every recorder copies the object, the other object and ref_other at
    each call (the watch);
  - func_801465b0, func_80140770, func_80120554, func_801b3784_slot04_17 and
    func_80130efc are recorders (results unused); func_80140cd8 is a
    recorder whose result is 0 in half of the cases and a random word
    otherwise. The log holds their arguments, compared in order.
  - ref_other (the global word the function sets to the other object) is
    filled with a random word first, so that the function's store to it is
    seen.
"""

from contracts import CallLog, Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym) -> Setup:
    other = state.alloc(0x400)
    fill(state, other, 0x400, rng)
    state.w16(other + 0x5C, (rng.getrandbits(16) | 0x8000) if rng.random() < 0.5 else rng.getrandbits(16) & 0x7FFF)
    obj = state.alloc(0x400)
    fill(state, obj, 0x400, rng)
    state.w32(obj + 0x40, other)

    kind = rng.random()
    if kind < 1 / 6:
        low = 0
    elif kind < 1 / 2:
        low = rng.randrange(0x80, 0x100)
    else:
        low = rng.randrange(1, 0x80)
    state.w16(obj + 0x3A, rng.getrandbits(8) << 8 | low)
    state.w8(obj + 0x0B, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    fill(state, sym["data_801ce44c_slot04_17"] - 8, 0x110, rng)

    result = 0 if rng.random() < 0.5 else rng.getrandbits(32)
    state.w32(sym["ref_other"], rng.getrandbits(32))
    log = CallLog(state, 4096, watch=((obj, 0x100), (other, 0x100), (sym["ref_other"], 1)))
    log.replace(sym["func_801465b0"], 4)
    log.replace(sym["func_80140770"], 7)
    log.replace(sym["func_80120554"], 3)
    log.replace(sym["func_80140cd8"], 3, result)
    log.replace(sym["func_801b3784_slot04_17"], 1)
    log.replace(sym["func_80130efc"], 1)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the clamp of field_4c: the branch on its sign.

    The one `bgez` of the build tests the sum field_4c + field_54; every
    call executes it. Making it `bltz` swaps the two outcomes.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 1 and (w >> 16) & 0x1F == 1]
    if not found:
        raise ValueError("no bgez found")
    index = found[0]
    return index, (words[index] & ~(0x1F << 16)), "bgez changed to bltz"


CONTRACT = Contract(setup, control)
