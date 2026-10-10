"""Contract of func_801e0ce8_slot0b: two passes of quads from a layout.

Choices of the setup (reads and writes are in the header of the .c file):
  - the layout has a count of 1 to 6 in four cases of five; otherwise 0,
    -1, or a random negative halfword (one record per pass is still made);
  - ids are random bytes, coordinates random halfwords (the sums wrap in
    the byte stores, as in the original);
  - tag is a random halfword, base a random word;
  - mirror (the fifth argument, placed at 0x801FF010, which is sp + 0x10 of
    the call) is 0 in half of the cases, otherwise a random non-zero word;
  - the record array holds random bytes, so that the bytes the function
    does not write are tested; it has room for 2 * 6 records (a count that
    makes one entry per pass uses two);
  - the recorder of func_8015c09c copies 10 words of the record passed and
    watches the whole array.
"""
from contracts import CallLog, Contract, Setup, fill

RECORDS = 12
STACK_TOP = 0x801FF000  # initial sp of every call; the fifth argument is at sp + 0x10


def setup(state, rng, sym):
    if rng.random() < 0.8:
        count = rng.randrange(1, 7)
    else:
        count = rng.choice((0, -1, -rng.randrange(1, 0x8000)))
    entries = max(count, 1)

    ids = state.alloc(8)
    fill(state, ids, 8, rng)
    coords = state.alloc(4 * 8)
    fill(state, coords, 4 * 8, rng)
    layout = state.alloc(12)
    fill(state, layout, 12, rng)
    state.w16(layout, count)
    state.w32(layout + 4, ids)
    state.w32(layout + 8, coords)
    step = state.alloc(12)
    fill(state, step, 12, rng)
    state.w32(step + 4, layout)
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w32(obj + 0x18, step)

    buf = state.alloc(RECORDS * 0x28)
    fill(state, buf, RECORDS * 0x28, rng)

    state.w32(STACK_TOP + 0x10, 0 if rng.random() < 0.5 else rng.randrange(1, 1 << 32))

    log = CallLog(state, 2 * RECORDS * (2 + 10 + RECORDS * 10) + 16, watch=((buf, RECORDS * 10),))
    log.replace(sym["func_8015c09c"], 1, pointees={0: 10})
    return Setup(args=(obj, buf, rng.getrandbits(16), rng.getrandbits(32)), returns_value=False)


def control(words):
    """Alter the store of the tile word: the shift of the id by 6 becomes 5.

    The build holds one copy of the loop, so the one shift `sll v0, v0, 6`
    is made for every entry of every call.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0 and w & 0x3F == 0 and (w >> 6) & 0x1F == 6]
    found = [i for i in found if (words[i] >> 11) & 0x1F == (words[i] >> 16) & 0x1F and (words[i] >> 21) == 0]
    if not found:
        raise ValueError("expected a shift of an id by 6")
    index = found[0]
    return index, (words[index] & ~(0x1F << 6)) | (5 << 6), "id shift changed from 6 to 5"


CONTRACT = Contract(setup, control)
