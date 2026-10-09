"""Contract of func_8013902c (see the header comment of func_8013902c.c).

Choices of the setup:
  - player_left and player_right (fixed data) are filled with random bytes;
    each gets its own frame record (field_07 is 0 in one case in twenty,
    otherwise 1..4) and a table of five 6-byte boxes (so the index is in
    range), field_0b and field_45 are 0 in half the cases and random
    otherwise, field_164 is 0, 1, 2 or random;
  - in four cases of five the positions, box x and box y are small (-30..30)
    and the box widths and heights 0..40, so that the y and x overlap tests
    both pass and fail; in the others they are random;
  - the word at offset 0x10 of the players is random, which tests the add
    with carry into pos_x.
"""

from contracts import Contract, Setup


def setup(state, rng, sym):
    def fill(address, size):
        state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))

    def small(span):
        return rng.randrange(-span, span + 1) & 0xFFFF

    near = rng.random() < 0.8
    for player in (sym["player_left"], sym["player_right"]):
        fill(player, 0x394)
        frame = state.alloc(0x10)
        fill(frame, 0x10)
        index = 0 if rng.random() < 0.05 else rng.randrange(1, 5)
        state.w8(frame + 7, index)
        state.w32(player + 0x88, frame)
        table = state.alloc(6 * 5)
        fill(table, 6 * 5)
        state.w32(player + 0x148, table)
        state.w8(player + 0xB, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
        state.w8(player + 0x164, rng.choice((0, 1, 2, rng.randrange(256))))
        state.w8(player + 0x45, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
        if near:
            state.w16(player + 0x12, small(30))
            state.w16(player + 0x16, small(30))
            for entry in range(5):
                box = table + 6 * entry
                state.w16(box + 0, small(30))
                state.w16(box + 2, small(30))
                state.w8(box + 4, rng.randrange(0, 41))
                state.w8(box + 5, rng.randrange(0, 41))
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the overlap test of x: the first lbu of the box widths changes its offset."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x24 and w & 0xFFFF == 0x4]
    if len(found) < 2:
        raise ValueError("expected the two width loads")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x5, "box width load moved to the height byte"


CONTRACT = Contract(setup, control)
