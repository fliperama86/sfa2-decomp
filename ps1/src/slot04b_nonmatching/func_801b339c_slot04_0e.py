"""Contract of func_801b339c_slot04_0e: a0 is a character object.

Reads and writes are listed in the header comment of the .c file. Choices
made here:
  - the log watches the whole object (0x394 bytes) at every recorded call,
    so the order of the function's stores against the calls is compared;
    no recorded callee gets a pointer to memory filled for the call;
  - func_801204f4 is a recorder (3 arguments); func_80130184, func_801307e0
    and func_80130efc run as the original code;
  - the object is a random block of 0x394 bytes; the position and the
    target field_70 are set apart by a chosen difference, so that the
    function takes the arm "target reached" or each of the three distances
    (below 0x11, below 0x21, further): the difference is one of -5, 0,
    0x10, 0x11, 0x20, 0x21 or 0x30 in half the cases, 0 to 0x40 in a
    quarter and a random halfword otherwise (func_80130184 then moves
    pos_y by the high half of field_50, so field_50 is a small word in
    three cases of four: -0x30000 to 0x30000; the other velocity words
    are random);
  - the sequence world is built as func_801307e0 and func_80130efc need it
    (four pointer tables of 192 entries in the scratchpad variables
    sequences_left, sequences_right, seqs_9c_left and seqs_14c_right, 16
    sequence steps with duration 1 in half the cases, loop offset -2 to 2,
    frame index 0 to 7, 8 frame records; the entries point at steps 5 to
    10 so that stepping stays inside the block; the object holds a step,
    the frame records and the fields func_80131020 reads with their
    special values likely); field_cd is 0 in half the cases and side is 0
    or 1.
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
    log.replace(sym["func_801204f4"], 3)
    fill(state, obj, 0x394, rng)
    _sequence_object(state, rng, obj, steps, frames)
    state.w8(obj + 0x07, rng.choice((rng.randrange(256), 255)))

    pos_y = rng.getrandbits(16)
    roll = rng.random()
    if roll < 0.5:
        delta = rng.choice((-5, 0, 0x10, 0x11, 0x20, 0x21, 0x30))
    elif roll < 0.75:
        delta = rng.randrange(0x41)
    else:
        delta = rng.getrandbits(16)
    if rng.random() < 0.75:
        state.w32(obj + 0x50, rng.randrange(-0x30000, 0x30000) & 0xFFFFFFFF)
    state.w16(obj + 0x16, pos_y)
    state.w16(obj + 0x70, (pos_y + delta) & 0xFFFF)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the constant 0x1b of the call that plays the landing sound.

    The arm taken when the target is reached loads 0x1b as the third
    argument of func_801204f4; the control makes it 0x1c, and the recorder's
    log shows it.
    """
    found = [i for i, w in enumerate(words) if w >> 26 in (0x09, 0x0D) and (w >> 21) & 31 == 0 and w & 0xFFFF == 0x1B]
    if len(found) != 1:
        raise ValueError(f"expected one load of the constant 0x1b, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x1C, "sound argument 0x1b made 0x1c"


CONTRACT = Contract(setup, control)
