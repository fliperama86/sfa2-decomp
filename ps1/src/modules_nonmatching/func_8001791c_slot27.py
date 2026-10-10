"""Contract of func_8001791c_slot27: draw 8 rows of a scrolling tile map.

Choices of the setup (reads and writes are in the header of the .c file):
  - the scroll record data_8002f0c8_slot27 holds random bytes except:
    the halfword at 0x12 is -0x48 to 0x188 in nine cases of ten (the
    function then runs on; otherwise a random halfword, which usually
    returns at once) and the one at 0x2a is random (so that the pull by 8
    happens in about half of the cases); word 0x14 puts the vertical value
    at -80 to 200 (rows below zero, past the end, and inside); word 0x5c
    gives 0 to 40 rows in nine cases of ten, otherwise random; word 0x58 is
    0x200, 0x400, 0x800 or 0x1000 or, in one case of eight, a random value
    of 0x200 to 0x1fff; the empty value at 0x1e is random (below 0x8000 with selector 1); byte 0x8a is 0 to 15;
  - the map block is zero except the rows the function will read, which
    the setup computes from the scroll record and fills with cells: a cell
    holds the empty value in a share of cells chosen per case (none, half,
    nine tenths) and random bits otherwise; the second word of a cell is random;
    the selector 1 is used only with the nine-tenths share;
  - the two record buffers hold random bytes, the list array 16 random words.
"""
from contracts import Contract, Setup, fill


def setup(state, rng, sym):
    scroll = sym["data_8002f0c8_slot27"]
    fill(state, scroll, 0x94, rng)
    y = rng.randrange(-0x48, 0x188) if rng.random() < 0.9 else rng.getrandbits(16)
    state.w16(scroll + 0x12, y)
    state.w16(scroll + 0x2A, rng.getrandbits(16))
    vy = rng.randrange(-80, 200)
    state.w32(scroll + 0x14, ((vy + 16) << 16 | rng.getrandbits(16)) & 0xFFFFFFFF)
    lines = rng.randrange(0, 41) if rng.random() < 0.9 else rng.getrandbits(32)
    if rng.random() < 0.9:
        state.w32(scroll + 0x5C, rng.randrange(0, 41) << 3 | rng.randrange(8))
    else:
        state.w32(scroll + 0x5C, rng.getrandbits(32))
    f58 = rng.choice((0x200, 0x400, 0x800, 0x1000)) if rng.random() < 0.875 else rng.randrange(0x200, 0x2000)
    state.w32(scroll + 0x58, f58)
    state.w8(scroll + 0x8A, rng.randrange(16))

    # The empty value is compared as a signed halfword with an unsigned cell, so a value of
    # 0x8000 or more matches no cell: those cases are dense whatever the share, and use selector 0.
    share = rng.choice((0.0, 0.5, 0.9))
    selector = rng.randrange(2) if share >= 0.9 else 0
    empty = rng.getrandbits(15) if selector else rng.getrandbits(16)
    state.w16(scroll + 0x1E, empty)
    state.w32(sym["data_801a27d0"], selector)

    # The map: a strip of 32 rows is (f58 >> 9) * 0x2000 bytes, a row 64 cells of 4 bytes.
    # The rows the function reads are those from (vy >> 3) + 2 on, eight of them.
    stride = (f58 >> 9) << 13
    mapblock = state.alloc(stride + 0x2000)
    state.w32(scroll + 0x50, mapblock)
    for r in range(8):
        line = (vy >> 3) + 2 + r
        if line < 0:
            continue
        base = mapblock + (line >> 5) * stride + (line & 0x1F) * 0x100
        for c in range(64):
            word = empty if rng.random() < share else rng.getrandbits(16)
            state.w16(base + 4 * c, word)
            state.w16(base + 4 * c + 2, rng.getrandbits(16))

    fill(state, sym["data_8002b388_slot27"], 2 * 280 * 28, rng)
    lists = state.alloc(4 * 16)
    fill(state, lists, 4 * 16, rng)
    state.w32(sym["data_801987c8"], lists)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the constant added to the texture page word: 0x37dd becomes 0x37de."""
    found = [i for i, w in enumerate(words) if w >> 26 == 9 and w & 0xFFFF == 0x37DD]
    if len(found) != 1:
        raise ValueError(f"expected one add of the page constant, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x37DE, "texture page constant changed"


CONTRACT = Contract(setup, control)
