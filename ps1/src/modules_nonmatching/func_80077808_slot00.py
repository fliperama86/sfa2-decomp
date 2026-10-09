"""Contract of func_80077808_slot00 (reads and writes: see the header of the .c).

Choices of the setup:
  - the object, its leader (field_3c), the frame record (field_88), the
    frames block (field_8c) and the box table (field_6c -> +4) are random
    blocks; the frame's active byte is 0 in a quarter of the cases, else
    1 to 3 (the box table block holds 4 entries of 32 bytes);
  - field_60 equals the looked-up byte in a third of the cases; field_67 is
    0 or non-zero with equal odds;
  - game_state.field_4d, field_47 and the leader's field_7e and field_27a
    are each 0 in seven cases of eight, random otherwise;
  - field_a4 is a small value around 0 (-1 to 3) in three cases of four, any
    32-bit value otherwise;
  - field_46 is 0 or 1 in half of the cases (the decrement then goes
    negative or to zero), any halfword otherwise;
  - field_66 is 0 to 15; the 16 rows of table_801ac318 that the function can
    read (384 bytes each), table_801aa4d8 and the 8 bytes of
    data_8007a010_slot00 are random;
  - game_state.field_226 is 0 in half of the cases;
  - func_80120028 is a recorder; the log copies the object and the leader.
"""

from contracts import CallLog, Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def seven_in_eight(rng, size=1):
    return 0 if rng.random() < 7 / 8 else rng.randrange(1, 1 << (8 * size))


def setup(state, rng, sym) -> Setup:
    gs = sym["game_state"]
    state.w8(gs + 0x4D, seven_in_eight(rng))
    state.w8(gs + 0x47, seven_in_eight(rng))
    state.w8(gs + 0x226, 0 if rng.random() < 0.5 else rng.randrange(1, 256))

    leader = state.alloc(0x394)
    fill(state, leader, 0x394, rng)
    state.w8(leader + 0x7E, seven_in_eight(rng))
    state.w8(leader + 0x27A, seven_in_eight(rng))

    frame = state.alloc(0x10)
    fill(state, frame, 0x10, rng)
    active = 0 if rng.random() < 0.25 else rng.randrange(1, 4)
    state.w8(frame, active)
    boxes = state.alloc(4 * 32)
    fill(state, boxes, 4 * 32, rng)
    tables = state.alloc(0x14)
    fill(state, tables, 0x14, rng)
    state.w32(tables + 4, boxes)
    frames = state.alloc(0x40)

    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w32(obj + 0x3C, leader)
    state.w32(obj + 0x88, frame)
    state.w32(obj + 0x8C, frames)
    state.w32(obj + 0x6C, tables)
    wanted = state.read(boxes + 32 * active + 0x12, 1)[0]
    if rng.random() < 1 / 3:
        state.w8(obj + 0x60, wanted)
    state.w8(obj + 0x67, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    if rng.random() < 0.75:
        a4 = rng.randrange(-1, 4)
    else:
        a4 = rng.getrandbits(32) - (1 << 31) if rng.random() < 0.5 else rng.getrandbits(31)
    state.w32(obj + 0xA4, a4 & 0xFFFFFFFF)
    state.w16(obj + 0x46, rng.choice((0, 1)) if rng.random() < 0.5 else rng.getrandbits(16))
    state.w8(obj + 0x66, rng.randrange(16))

    fill(state, sym["table_801ac318"], 16 * 384, rng)
    fill(state, sym["table_801aa4d8"], 32, rng)
    fill(state, sym["data_8007a010_slot00"], 8, rng)

    log = CallLog(state, 1024, watch=((obj, 0x394 // 4), (leader, 0x394 // 4)))
    log.replace(sym["func_80120028"], 1)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the mask of the ring index: `andi v0,v0,7` becomes `andi v0,v0,3`."""
    found = [i for i, w in enumerate(words) if w >> 26 == 12 and w & 0xFFFF == 7 and (w >> 21) & 31 == 2]
    if len(found) != 1:
        raise ValueError(f"expected one andi by 7, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 3, "ring index mask 7 changed to 3"


CONTRACT = Contract(setup, control)
