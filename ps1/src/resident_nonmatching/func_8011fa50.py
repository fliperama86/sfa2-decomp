"""Contract of func_8011fa50: a0 the object, a1 a record, no result.

Choices made here:
  - the object is 0x394 random bytes (pos_x and pos_y random 16-bit values);
  - the record is two words: a random header word and a pointer to a data
    block; the data block starts with a column count and a row count (each
    0 to 6, with 0 chosen often enough to try the empty loops) followed by
    random cells;
  - cam_f50 points into the middle of a 0x24000-byte random buffer, 0x12000
    bytes from its start, so that every destination the signed 17-bit
    offset can make (plus the copied grid) stays inside the buffer;
  - func_8011fb70 runs as the original code in both runs.
"""

from contracts import Contract, Setup

BUFFER = 0x24000


def setup(state, rng, sym) -> Setup:
    buffer = state.alloc(BUFFER)
    state.write(buffer, rng.randbytes(BUFFER))
    state.w32(sym["cam_f50"], buffer + 0x12000)

    columns = rng.choice((0, rng.randrange(0, 7), rng.randrange(1, 7)))
    rows = rng.choice((0, rng.randrange(0, 7), rng.randrange(1, 7)))
    data = state.alloc(4 + 2 * (2 * columns * rows) + 16)
    state.write(data, rng.randbytes(4 + 4 * columns * rows + 16))
    state.w16(data, columns)
    state.w16(data + 2, rows)

    rec = state.alloc(8)
    state.w32(rec, rng.getrandbits(32))
    state.w32(rec + 4, data)

    obj = state.alloc(0x394)
    state.write(obj, rng.randbytes(0x394))
    return Setup(args=(obj, rec), returns_value=False)


def control(words):
    """Alter the store of field_3a: its offset moves by two bytes."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0x3A]
    if len(found) != 1:
        raise ValueError(f"expected one store of field_3a, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x3C, "store of field_3a moved by two bytes"


CONTRACT = Contract(setup, control)
