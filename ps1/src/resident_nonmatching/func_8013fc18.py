"""Contract of func_8013fc18: is the object's "other" object within horizontal reach.

Choices of the setup:
  - object and other object are random 0x394-byte blocks; field_40 of the
    object points at the other object; the frame record (16 bytes), the box
    (8 bytes) and the configuration block (0x100 bytes) are their own blocks;
  - each of field_249, field_27b, the halfword at 4 (passing state 1),
    game_state's config field_4e and field_45 is in its passing state in
    five cases of six, otherwise random (the halfword at 4 is then 1 or
    random); each of the three box flags is a random byte in three cases of
    five and 0 otherwise, and box_c is then forced non-zero in five cases of
    six;
  - the frame record's field_06 is 0, 4 or random; field_d8 and field_157 are
    0 in two cases of three; field_0b and field_158 are 0 or random;
  - a is a small signed number in three cases of four, otherwise any s16; c is
    a small non-negative number in three cases of four, otherwise any s16;
  - the other object's pos_x is chosen in two cases of three so that the
    signed difference (before it is made positive) is a random value from
    -(|c| + 3) to |c| + 3 or, in half of those cases instead, plus or minus
    (|c| + k) with k from -1 to 2; otherwise it stays random;
  - ref_other, which the function always writes (the other object's address,
    before any check), starts as a random word, so that the write is seen;
  - func_8013fab4 is a recorder (1 argument, result 0); every call copies
    ref_other, the object and the other object whole.
"""
from contracts import CallLog, Contract, Setup, fill


def s16(value):
    value &= 0xFFFF
    return value - 0x10000 if value & 0x8000 else value


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    other = state.alloc(0x394)
    frame = state.alloc(16)
    box = state.alloc(8)
    config = state.alloc(0x100)
    for block, size in ((obj, 0x394), (other, 0x394), (frame, 16), (box, 8), (config, 0x100)):
        fill(state, block, size, rng)
    log = CallLog(state, words=1024, watch=((sym["ref_other"], 1), (obj, 0x394 // 4), (other, 0x394 // 4)))
    log.replace(sym["func_8013fab4"], 1, 0)

    def mostly(passing, other_value):
        return passing if rng.random() < 5 / 6 else other_value

    state.w32(sym["ref_other"], rng.getrandbits(32))
    state.w32(obj + 0x40, other)
    state.w32(other + 0x88, frame)
    state.w32(other + 0x148, box)
    state.w32(sym["game_state"] + 0x360, config)
    state.w8(other + 0x249, mostly(0, rng.getrandbits(8)))
    state.w8(other + 0x27B, mostly(0, rng.getrandbits(8)))
    state.w16(other + 4, mostly(1, rng.getrandbits(16)))
    state.w8(config + 0x4E, mostly(0, rng.getrandbits(8)))
    state.w8(other + 0x45, mostly(0, rng.getrandbits(8)))
    for offset in (1, 2, 3):
        state.w8(frame + offset, rng.getrandbits(8) if rng.random() < 0.6 else 0)
    if rng.random() < 5 / 6:
        state.w8(frame + 1, rng.randrange(1, 256))
    state.w8(frame + 6, rng.choice((0, 4, rng.getrandbits(8))))
    for offset in (0xD8, 0x157):
        state.w8(other + offset, 0 if rng.random() < 2 / 3 else rng.getrandbits(8))
    flip = rng.choice((0, rng.getrandbits(8)))
    state.w8(obj + 0x0B, flip)
    mirror = rng.choice((0, rng.getrandbits(8)))
    state.w8(other + 0x158, mirror)
    origin, extent, obj_x = rng.getrandbits(16), rng.getrandbits(8), rng.getrandbits(16)
    state.w16(box, origin)
    state.w8(box + 4, extent)
    state.w16(obj + 0x12, obj_x)

    a = rng.randrange(-300, 301) if rng.random() < 0.75 else s16(rng.getrandbits(16))
    c = rng.randrange(0, 200) if rng.random() < 0.75 else s16(rng.getrandbits(16))
    half = s16(origin - extent)
    if mirror:
        half = s16(-half)
    signed_a = s16(-a) if flip else a
    if rng.random() < 2 / 3:
        target = rng.randrange(-(abs(c) + 3), abs(c) + 4)
        if rng.random() < 0.5:  # on or next to the boundary
            target = rng.choice((-1, 1)) * (abs(c) + rng.randrange(-1, 3))
        state.w16(other + 0x12, s16(signed_a + s16(obj_x) - half - target))
    return Setup(args=(obj, a & 0xFFFFFFFF, c & 0xFFFFFFFF), returns_value=True)


def control(words):
    """Replace the first `bne v0,zero` by a nop: it is never taken.

    It is the field_249 refusal (read from the original's listing, not tested).
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 5 and (w >> 21) & 31 == 2 and (w >> 16) & 31 == 0]
    if not found:
        raise ValueError("no bne v0,zero")
    return found[0], 0, "first refusal branch removed"


CONTRACT = Contract(setup, control)
