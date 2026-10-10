"""Contract of func_8012fd80: a0 = object a, a1 = object b, returns u8 in v0.

Choices of the setup (reads are listed in func_8012fd80.c):
  - b->frame points at a record whose field_0c has bit 0x80 set in one case
    of six, so that the early return is tried;
  - the sequence has 1 to 6 steps followed by a terminating step with a
    negative flags word; the flags of the earlier steps are non-negative,
    except that in one case of eight one of the first `count` steps (the
    first one included, whose flags the function never tests) is negative,
    so the loop also ends early;
  - the frame table has 8 records; each step's frame_index picks one, and
    a record's active byte is 0 in one case of three, else 1 to 6;
  - the box table has 7 boxes of 32 bytes; in four cases of five the origin
    of every box is from -300 to 299 and its endpoint from -120 to 399,
    otherwise both are random;
  - field_0b is 0 in two cases of three, else random; the positions of a
    and b are from -300 to 299 in four cases of five, else random.
"""
from contracts import Contract, Setup, fill


def setup(state, rng, sym):
    frame = state.alloc(16)
    fill(state, frame, 16, rng)
    state.w8(frame + 0x0C, (rng.getrandbits(8) | 0x80) if rng.random() < 1 / 6 else rng.getrandbits(8) & 0x7F)

    frames = state.alloc(16 * 8)
    fill(state, frames, 16 * 8, rng)
    for i in range(8):
        state.w8(frames + 16 * i, 0 if rng.random() < 1 / 3 else rng.randrange(1, 7))

    boxes = state.alloc(32 * 7)
    fill(state, boxes, 32 * 7, rng)
    small = rng.random() < 0.8
    for i in range(7):
        if small:
            state.w16(boxes + 32 * i + 0, rng.randrange(-300, 300) & 0xFFFF)
            state.w16(boxes + 32 * i + 4, rng.randrange(-120, 400) & 0xFFFF)

    count = rng.randrange(1, 7)
    steps = state.alloc(12 * (count + 1) + 12)
    fill(state, steps, 12 * (count + 1) + 12, rng)
    early = rng.randrange(count) if rng.random() < 1 / 8 else None
    for i in range(count + 1):
        flags = rng.randrange(0, 0x8000)
        if i == count or i == early:
            flags = rng.randrange(0x8000, 0x10000)
        state.w16(steps + 12 * i + 2, flags)
        state.w16(steps + 12 * i + 10, rng.randrange(8))

    def pos():
        return rng.randrange(-300, 300) & 0xFFFF if rng.random() < 0.8 else rng.getrandbits(16)

    a = state.alloc(0x394)
    fill(state, a, 0x394, rng)
    state.w16(a + 0x12, pos())
    b = state.alloc(0x394)
    fill(state, b, 0x394, rng)
    state.w16(b + 0x12, pos())
    state.w8(b + 0x0B, rng.choice((0, 0, rng.getrandbits(8))))
    state.w32(b + 0x88, frame)
    state.w32(b + 0x8C, frames)
    state.w32(b + 0x144, boxes)
    state.w32(b + 0x18, steps)
    return Setup(args=(a, b), returns_value=True)


def control(words):
    """Alter the `ori v0,zero,2` of the return-2 arm into 3 (any reaching case)."""
    found = [i for i, w in enumerate(words) if w & 0xFFFFFFFF == 0x34020002]
    if len(found) != 1:
        raise ValueError(f"expected one load of 2 into v0, found {len(found)}")
    return found[0], 0x34020003, "return value 2 changed to 3"


CONTRACT = Contract(setup, control)
