"""Contract of func_801e0a78_slot0b: squares of a layout, transformed.

Choices of the setup (reads and writes are in the header of the .c file):
  - the layout has a count of 1 to 6 (0 or less is excluded: the original
    would loop 2^32 times) and random u16 coordinate pairs;
  - field_48 is 0 in half of the cases; field_20, field_22 and field_24
    are each 0 in a third of the cases, otherwise a random halfword (field_24
    also takes values like 0x100, non-zero with a low byte of 0);
  - pos_x and pos_y are random halfwords, field_09 is 0 to 7;
  - the 256 entries of table_801e1fec_slot0b hold random halfwords;
  - the fourth argument is 0 in half of the cases, otherwise a random
    non-zero word; the third argument is random;
  - the record array holds room for 2 * 6 records of random bytes, so that
    the bytes the function leaves alone are tested; the list array has 16
    words of random content.
"""
from contracts import Contract, Setup, fill


def setup(state, rng, sym):
    count = rng.randrange(1, 7)
    coords = state.alloc(4 * count)
    fill(state, coords, 4 * count, rng)
    layout = state.alloc(12)
    fill(state, layout, 12, rng)
    state.w16(layout, count)
    state.w32(layout + 8, coords)
    step = state.alloc(12)
    fill(state, step, 12, rng)
    state.w32(step + 4, layout)

    fill(state, sym["table_801e1fec_slot0b"], 4 * 256, rng)
    lists = state.alloc(4 * 16)
    fill(state, lists, 4 * 16, rng)
    state.w32(sym["data_801987c8"], lists)

    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x09, rng.randrange(8))
    state.w8(obj + 0x48, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    for offset in (0x20, 0x22):
        state.w16(obj + offset, 0 if rng.random() < 1 / 3 else rng.getrandbits(16))
    state.w16(obj + 0x24, rng.choice((0, 0, 0x100, rng.getrandbits(16), rng.getrandbits(16))))
    state.w32(obj + 0x18, step)

    buf = state.alloc(12 * 0x28)
    fill(state, buf, 12 * 0x28, rng)
    flag = 0 if rng.random() < 0.5 else rng.randrange(1, 1 << 32)
    return Setup(args=(obj, buf, rng.getrandbits(32), flag), returns_value=False)


def control(words):
    """Alter the first add of 16 in the build: `addiu ..., 0x10` becomes 0x11.

    The first such add belongs to the corners of every record, so every
    call executes it.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 9 and w & 0xFFFF == 0x10]
    if not found:
        raise ValueError("expected an add of 0x10")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x11, "first +16 changed to +17"


CONTRACT = Contract(setup, control)
