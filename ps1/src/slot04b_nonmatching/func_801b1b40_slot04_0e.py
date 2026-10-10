"""Contract of func_801b1b40_slot04_0e: a0 is a character object.

Reads and writes are listed in the header comment of the .c file. Choices
made here:
  - the log watches the whole object (0x394 bytes) at every recorded call,
    so the order of the function's stores against the calls is compared;
    no recorded callee gets a pointer to memory filled for the call;
  - func_80120554 is a recorder (3 arguments; only func_80141f28 reaches
    it); func_80130504, func_80141f28, func_801307e0, func_801b6a7c_slot04_0e
    and func_801b6afc_slot04_0e run as the original code;
  - the object is a random block of 0x394 bytes; func_80130504 overwrites
    field_129 and field_12a from the input word field_134, so that word is
    what steers: in half the cases it is one of the single bits that
    func_8013054c tests (0x80, 0x10, 0x4, 0x40, 0x20, 0x8, 0x1, 0x2) or 0,
    in a quarter it is 0x20 with random lower bits other than 0x10, 0x4
    and 0x40 (the pair field_12a = 2, field_129 = 2 that func_801b6a7c
    needs), the rest random;
  - field_cd is 0 in half the cases and random otherwise, field_219 is 0
    in half the cases, bit 14 of field_130 is set in half the cases;
  - field_48 is 0 in half the cases;
  - func_80141f28 is given its paths: field_74 and field_2a2 are 0 in
    three cases of four, field_d8 is 0 or random, field_c6 is near a limit
    in half the cases, game_state.field_47 is 0 in three cases of four;
  - the sequence world is built as func_801307e0 needs it: four pointer
    tables of 192 entries in the scratchpad variables sequences_left,
    sequences_right, seqs_9c_left and seqs_14c_right, 16 sequence steps
    (duration 1 in half the cases, loop offset -2 to 2, frame index 0 to
    7), 8 frame records; the entries point at steps 5 to 10 so that
    stepping stays inside the block; the object holds a step, the frame
    records and the fields func_80131020 reads with their special values
    likely; side is 0 or 1.
"""

from contracts import CallLog, Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def _sequence_world(state, rng, sym):
    """Pointer tables, steps and frame records of the sequence functions.

    Returns (first step, frame records).
    """
    steps = state.alloc(16 * 12)
    for index in range(16):
        base = steps + 12 * index
        state.w16(base + 0, 1 if rng.random() < 0.5 else rng.randrange(1, 6))
        state.w16(base + 2, rng.getrandbits(16))
        state.w32(base + 4, rng.choice((1, 0x03030001, rng.getrandbits(32))))
        state.w16(base + 8, rng.randrange(-2, 3) & 0xFFFF)
        state.w16(base + 10, rng.randrange(8))
    frames = state.alloc(8 * 16)
    fill(state, frames, 8 * 16, rng)
    for name in ("sequences_left", "sequences_right", "seqs_9c_left", "seqs_14c_right"):
        table = state.alloc(4 * 192)
        for index in range(192):
            state.w32(table + 4 * index, steps + 12 * rng.randrange(5, 11))
        state.w32(sym[name], table)
    return steps, frames


def _sequence_object(state, rng, obj, steps, frames):
    """The fields of an object that the sequence functions read."""
    state.w32(obj + 0x18, steps + 12 * rng.randrange(5, 11))  # sequence
    state.w32(obj + 0x8C, frames)  # frames
    state.w32(obj + 0x88, frames + 16 * rng.randrange(8))  # frame
    state.w16(obj + 0x38, rng.choice((1, 2, rng.getrandbits(16))))
    state.w8(obj + 0xCD, rng.choice((0, rng.randrange(256))))
    state.w8(obj + 0xA6, rng.randrange(2))
    state.w8(obj + 0x7E, rng.choice((0, 1, rng.randrange(256))))
    state.w32(obj + 0x04, rng.choice((1, 0x03030001, 0x00000001, rng.getrandbits(32))))
    state.w8(obj + 0x06, rng.choice((5, rng.randrange(256))))
    state.w8(obj + 0x29A, rng.choice((0, 0, rng.randrange(256))))


def setup(state, rng, sym):
    steps, frames = _sequence_world(state, rng, sym)
    obj = state.alloc(0x394)
    log = CallLog(state, words=4096, watch=((obj, 0x394 // 4),))
    log.replace(sym["func_80120554"], 3)
    fill(state, obj, 0x394, rng)
    _sequence_object(state, rng, obj, steps, frames)

    roll = rng.random()
    if roll < 0.5:
        pad = rng.choice((0x80, 0x10, 0x4, 0x40, 0x20, 0x8, 0x1, 0x2, 0))
    elif roll < 0.75:
        pad = 0x20 | (rng.getrandbits(16) & ~0x80 & ~0x10 & ~0x4 & ~0x40)
    else:
        pad = rng.getrandbits(16)
    state.w16(obj + 0x134, pad)
    state.w8(obj + 0x219, rng.choice((0, rng.randrange(256))))
    state.w16(obj + 0x130, (rng.getrandbits(16) & ~0x4000) | (0x4000 if rng.random() < 0.5 else 0))
    state.w8(obj + 0x48, rng.choice((0, rng.randrange(256))))

    # func_80141f28
    state.w8(obj + 0x74, 0 if rng.random() < 0.75 else rng.randrange(256))
    state.w8(obj + 0x2A2, 0 if rng.random() < 0.75 else rng.randrange(256))
    state.w8(obj + 0xD8, rng.choice((0, rng.randrange(256))))
    state.w16(obj + 0xC6, rng.choice((0, 0x2F, 0x30, 0x8F, 0x90, rng.getrandbits(16), rng.randrange(0x98))))
    state.w8(sym["game_state"] + 0x47, 0 if rng.random() < 0.75 else rng.randrange(256))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the constant 3 stored into field_07 at the start.

    The first statement of the function stores the byte 3 into field_07 on
    every call; the control makes it 4.
    """
    found = [i for i, w in enumerate(words) if w >> 26 in (0x09, 0x0D) and (w >> 21) & 31 == 0 and w & 0xFFFF == 3]
    if len(found) != 1:
        raise ValueError(f"expected one load of the constant 3, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 4, "state byte 3 made 4"


CONTRACT = Contract(setup, control)
