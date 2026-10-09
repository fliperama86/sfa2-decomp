"""Contract of func_801e8dc4_slot06_00: first-frame setup of an object.

Reads and writes are listed in the header comment of
func_801e8dc4_slot06_00.c. Choices made here:
  - the object is a random block of 0x394 bytes; its field_03 is 0 to 2 in
    nine cases of ten (the word table data_801e9f60_slot06_00 holds 12 words
    before the next function of the image, so these three indices read
    table words that the setup filled) and any byte otherwise (the index
    then reads other data of the image, the same in both runs);
  - the 12 table words, the pointer data_801e9f48_slot06_00 and the first
    three records of data_801f3010_slot06_00 are random; the rest of the
    256 records keep the image's content, which both runs rewrite alike;
  - field_04, pos_y and the other fields are random.
"""

from contracts import Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym) -> Setup:
    fill(state, sym["data_801e9f60_slot06_00"], 12 * 4, rng)
    state.w32(sym["data_801e9f48_slot06_00"], rng.getrandbits(32))
    fill(state, sym["data_801f3010_slot06_00"], 3 * 0x20, rng)
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x03, rng.randrange(3) if rng.random() < 0.9 else rng.randrange(256))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the constant of the store to field_78 (0x100 becomes 0x101).

    The store is `ori v0,zero,0x100` followed by `sh v0,0x78(a0)`; every call
    makes it.
    """
    found = [i for i, w in enumerate(words) if w == 0x34020100]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x100, found {len(found)}")
    index = found[0]
    return index, words[index] | 1, "constant 0x100 of field_78 changed to 0x101"


CONTRACT = Contract(setup, control)
