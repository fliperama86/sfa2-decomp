"""Contract of func_800793d4_slot2b (reads and writes: see the header of the .c).

Choices of the setup:
  - the object and the floor object are random blocks of 0x394 bytes;
    data_8007ef34_slot2b points at the floor object, data_8017c850 holds a
    random word (a pointer that the recorder only logs);
  - pos_y is the floor's field_70 within 2 either way in half of the cases
    (so that the signed compare is tried at the boundary), any halfword
    otherwise;
  - func_800795b4_slot2b, func_80130700, func_801204f4 and func_80131094
    are recorders; the log copies the object and the floor object at every
    call.
"""

from contracts import CallLog, Contract, Setup


def block(state, rng, size):
    address = state.alloc(size)
    state.write(address, rng.randbytes(size))
    return address


def setup(state, rng, sym) -> Setup:
    obj = block(state, rng, 0x394)
    floor = block(state, rng, 0x394)
    state.w32(sym["data_8007ef34_slot2b"], floor)
    state.w32(sym["data_8017c850"], rng.getrandbits(32))
    field_70 = state.read(floor + 0x70, 2)
    if rng.random() < 0.5:
        base = int.from_bytes(field_70, "little", signed=True)
        state.w16(obj + 0x16, (base + rng.randrange(-2, 3)) & 0xFFFF)

    log = CallLog(state, 4096, watch=((obj, 0x394 // 4), (floor, 0x394 // 4)))
    log.replace(sym["func_800795b4_slot2b"], 1)
    log.replace(sym["func_80130700"], 2)
    log.replace(sym["func_801204f4"], 3)
    log.replace(sym["func_80131094"], 1)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the third argument of func_801204f4: `ori a2,zero,0x10` becomes 0x11."""
    found = [i for i, w in enumerate(words) if w >> 26 == 13 and w & 0xFFFF == 0x10 and (w >> 16) & 31 == 6]
    if len(found) != 1:
        raise ValueError(f"expected one ori a2 of 0x10, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x11, "third argument 0x10 changed to 0x11"


CONTRACT = Contract(setup, control)
