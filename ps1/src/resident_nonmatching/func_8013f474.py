"""Contract of func_8013f474: is the object's "other" object within horizontal reach of a.

Choices of the setup:
  - object and other object are random 0x394-byte blocks; field_40 of the
    object points at the other object; the frame record (16 bytes), the box
    array (256 boxes of 6 bytes) and the configuration block (0x100 bytes)
    are their own blocks;
  - each refusing field (the object's field_49 non-zero, the object's
    field_7e, game_state's config field_4e, field_27b, the three box flags,
    the frame record's field_06, field_45) is in its passing state in five
    cases of six, otherwise random; field_7e is 0 in half of the cases;
  - the frame record's field_07 is a small box number (0 to 3) in three
    cases of four, otherwise any byte;
  - field_0b and field_158 are 0 or random;
  - a is a small signed number in three cases of four, otherwise any s16;
    limit is a small non-negative number in three cases of four, otherwise
    any s16;
  - the other object's pos_x is chosen so that the distance falls within
    3 of limit in two cases of three (in half of those, exactly at the boundary,
    limit plus -1 to 2), otherwise random;
  - func_8013fab4 is a recorder (1 argument) with a random 32-bit result;
    every call copies ref_other, the object and the other object whole.
"""
from contracts import CallLog, Contract, Setup, fill


def s16(value):
    value &= 0xFFFF
    return value - 0x10000 if value & 0x8000 else value


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    other = state.alloc(0x394)
    frame = state.alloc(16)
    boxes = state.alloc(6 * 256)
    config = state.alloc(0x100)
    for block, size in ((obj, 0x394), (other, 0x394), (frame, 16), (boxes, 6 * 256), (config, 0x100)):
        fill(state, block, size, rng)
    log = CallLog(state, words=1024, watch=((sym["ref_other"], 1), (obj, 0x394 // 4), (other, 0x394 // 4)))
    log.replace(sym["func_8013fab4"], 1, rng.getrandbits(32))

    def mostly(passing, other_value):
        return passing if rng.random() < 5 / 6 else other_value

    state.w32(obj + 0x40, other)
    state.w32(other + 0x88, frame)
    state.w32(other + 0x148, boxes)
    state.w32(sym["game_state"] + 0x360, config)
    state.w8(obj + 0x49, mostly(rng.randrange(1, 256), 0))
    state.w8(obj + 0x7E, rng.choice((0, rng.getrandbits(8))))
    state.w8(config + 0x4E, mostly(0, rng.getrandbits(8)))
    state.w8(other + 0x27B, mostly(0, rng.getrandbits(8)))
    state.w8(other + 0x45, mostly(0, rng.getrandbits(8)))
    for offset in (2, 3):
        state.w8(frame + offset, rng.getrandbits(8) if rng.random() < 0.6 else 0)
    state.w8(frame + 1, rng.randrange(1, 256) if rng.random() < 5 / 6 else rng.choice((0, 0, rng.getrandbits(8))))
    state.w8(frame + 6, mostly(0, rng.getrandbits(8)))
    box_number = rng.randrange(4) if rng.random() < 0.75 else rng.getrandbits(8)
    state.w8(frame + 7, box_number)
    flip = rng.choice((0, rng.getrandbits(8)))
    state.w8(obj + 0x0B, flip)
    mirror = rng.choice((0, rng.getrandbits(8)))
    state.w8(other + 0x158, mirror)
    origin, extent, obj_x = rng.getrandbits(16), rng.getrandbits(8), rng.getrandbits(16)
    state.w16(boxes + 6 * box_number, origin)
    state.w8(boxes + 6 * box_number + 4, extent)
    state.w16(obj + 0x12, obj_x)

    a = rng.randrange(-300, 301) if rng.random() < 0.75 else s16(rng.getrandbits(16))
    limit = rng.randrange(0, 200) if rng.random() < 0.75 else s16(rng.getrandbits(16))
    half = s16(origin - extent)
    if mirror:
        half = s16(-half)
    signed_a = s16(-a) if flip else a
    if rng.random() < 2 / 3:
        target = rng.randrange(-(abs(limit) + 3), abs(limit) + 4)
        if rng.random() < 0.5:  # on or next to the boundary
            target = rng.choice((-1, 1)) * (abs(limit) + rng.randrange(-1, 3))
        state.w16(other + 0x12, s16(signed_a + s16(obj_x) - half - target))
    return Setup(args=(obj, a & 0xFFFFFFFF, limit & 0xFFFFFFFF), returns_value=True)


def control(words):
    """Replace the first `beq v0,zero` (the field_49 test, a return to the caller) by a nop: it is never taken."""
    found = [i for i, w in enumerate(words) if w >> 26 == 4 and (w >> 21) & 31 == 2 and (w >> 16) & 31 == 0]
    if not found:
        raise ValueError("no beq v0,zero")
    return found[0], 0, "first branch on v0 == 0 removed"


CONTRACT = Contract(setup, control)
