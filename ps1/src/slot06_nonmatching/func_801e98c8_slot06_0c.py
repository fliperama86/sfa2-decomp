"""Contract of func_801e98c8_slot06_0c (a palette sequence start).

Reads and writes are listed in the header comment of the .c file. Choices
made here:
  - pal is a 16-byte block of random bytes; field_08 and field_0a are random
    halfwords, so every mask and the signed division by 16 are tried;
  - rec is an 8-byte record: a random header word, and a pointer to a data
    block that starts with a width of 0 to 16 and a height of 0 to 8
    (halfwords; zero is chosen about one case in eight each, so that the
    loop entries are tried), then random halfwords;
  - data_801aa624 points 0x9000 bytes into a block of 0x16000 bytes of
    random content, so that base plus any 17-bit offset the code can form
    (-0x8000 to about +0xbf00) and the copied area stay inside the block;
  - func_8011fae0 is not replaced; it runs as the original in both runs.
"""

from contracts import Contract, Setup, fill, halfword

BEFORE = 0x9000
AFTER = 0xD000


def setup(state, rng, sym):
    pal = state.alloc(16)
    fill(state, pal, 16, rng)
    state.w16(pal + 8, halfword(rng))
    state.w16(pal + 0xA, halfword(rng))

    width = 0 if rng.random() < 0.12 else rng.randrange(1, 17)
    height = 0 if rng.random() < 0.12 else rng.randrange(1, 9)
    data = state.alloc(4 + 4 * width * height + 4)
    fill(state, data, 4 + 4 * width * height, rng)
    state.w16(data, width)
    state.w16(data + 2, height)

    rec = state.alloc(8)
    state.w32(rec, rng.getrandbits(32))
    state.w32(rec + 4, data)

    area = state.alloc(BEFORE + AFTER)
    fill(state, area, BEFORE + AFTER, rng)
    state.w32(sym["data_801aa624"], area + BEFORE)
    return Setup(args=(pal, rec), returns_value=False)


def control(words):
    """Alter the store of the header's low half: it goes to field_04 instead of field_06.

    The code stores the low half with a halfword store at offset 6 from the entry, once in every call.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 6]
    if len(found) != 1:
        raise ValueError(f"expected one store of the low half at offset 6, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 4, "low half stored at field_04"


CONTRACT = Contract(setup, control)
