"""Contract of func_801b5e34_slot04_0e: a0 is a character object.

Reads and writes are listed in the header comment of the .c file. Choices
made here:
  - the log watches the whole object (0x394 bytes) at every recorded call,
    so the order of the function's stores against the calls is compared;
    no recorded callee gets a pointer to memory filled for the call;
  - func_801204f4 and func_80120554 are recorders (3 arguments each);
    func_80141f28 and func_801307e0 run as the original code;
  - the object is a random block of 0x394 bytes; field_48 is below 8 in
    half the cases (the range the tables are meant for) and any byte
    otherwise, since the tables are read from the module image either way;
    field_0b is 0 in half the cases; field_07 may be 255;
  - func_80141f28 is given its paths: field_74 and field_2a2 are 0 in
    three cases of four, field_d8 is 0 or random, field_c6 is near a limit
    (0, 0x2f, 0x30, 0x8f, 0x90) in half the cases, game_state.field_47 is
    0 in three cases of four;
  - the sequence world is built as func_801307e0 and the step functions it
    reaches need it: four pointer tables of 192 entries in the scratchpad
    variables sequences_left, sequences_right, seqs_9c_left and
    seqs_14c_right, 16 sequence steps (duration 1 in half the cases, loop
    offset -2 to 2, frame index 0 to 7), 8 frame records; the entries point
    at steps 5 to 10 so that stepping stays inside the block; the object
    holds a step, the frame records and the fields func_80131020 reads
    (field_7e, field_04..07, field_29a) with their special values likely;
    field_cd is 0 in half the cases and side is 0 or 1.
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
    log.replace(sym["func_80120554"], 3)
    fill(state, obj, 0x394, rng)
    _sequence_object(state, rng, obj, steps, frames)
    state.w8(obj + 0x0B, rng.choice((0, rng.randrange(256))))
    state.w8(obj + 0x48, rng.randrange(8) if rng.random() < 0.5 else rng.randrange(256))
    state.w8(obj + 0x07, rng.choice((rng.randrange(256), 255)))

    # func_80141f28
    state.w8(obj + 0x74, 0 if rng.random() < 0.75 else rng.randrange(256))
    state.w8(obj + 0x2A2, 0 if rng.random() < 0.75 else rng.randrange(256))
    state.w8(obj + 0xD8, rng.choice((0, rng.randrange(256))))
    state.w16(obj + 0xC6, rng.choice((0, 0x2F, 0x30, 0x8F, 0x90, rng.getrandbits(16), rng.randrange(0x98))))
    state.w8(sym["game_state"] + 0x47, 0 if rng.random() < 0.75 else rng.randrange(256))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of the constant 7 into field_46.

    The function always executes `sb reg, 0x46(s0)`; the stored byte then
    comes from a register that holds 7 (the build loads it with an ori).
    The control moves the store to field_47's neighbour, offset 0x45.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0x46]
    if len(found) != 1:
        raise ValueError(f"expected one store to field_46, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x45, "store of the state byte moved by one byte"


CONTRACT = Contract(setup, control)
