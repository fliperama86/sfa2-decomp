"""Contract of func_801364a0 (see the header comment of func_801364a0.c).

Choices of the setup:
  - the sprite is a 0x4c-byte block of random bytes; the two players' blocks
    are filled with random bytes, then each player is "ready" (bit 7 of
    field_73 set and field_261 0) in a third of the cases, has bit 7 clear in
    a third and bit 7 set with field_261 not 0 in a third, so that the three
    arms of the span are all taken;
  - in three cases of four the span and the sprite's field_22 are steered to
    the decisions: the span's middle c and half width w are chosen with w
    around 0x60 (so that the width 2w falls on both sides of 0xc0), the
    players' pos_x and field_154 are c and w (the second player's differ from
    them by -3..3, so that the union takes its lowest and highest from either
    player), and field_22 is chosen to put one of the three tested amounts
    (the middle one, the high edge, the low edge) within -3..3 of its
    threshold; in the others all of them are random halfwords;
  - func_80136668, func_801366d4 and func_80136898 are not replaced.
"""

from contracts import Contract, Setup


def setup(state, rng, sym):
    def fill(address, size):
        state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))

    sprite = state.alloc(0x4C)
    fill(sprite, 0x4C)
    left, right = sym["player_left"], sym["player_right"]
    fill(left, 0x394)
    fill(right, 0x394)
    for player in (left, right):
        kind = rng.randrange(3)  # 0: ready, 1: bit 7 clear, 2: bit 7 set but field_261 not 0
        if kind == 0:
            state.w8(player + 0x73, 0x80 | rng.randrange(0x80))
            state.w8(player + 0x261, 0)
        elif kind == 1:
            state.w8(player + 0x73, rng.randrange(0x80))
        else:
            state.w8(player + 0x73, 0x80 | rng.randrange(0x80))
            state.w8(player + 0x261, rng.randrange(1, 256))
    if rng.random() < 0.75:
        c = rng.getrandbits(16)
        w = rng.randrange(0x5C, 0x65)
        state.w16(left + 0x12, c)
        state.w16(left + 0x154, w)
        state.w16(right + 0x12, (c + rng.randrange(-3, 4)) & 0xFFFF)
        state.w16(right + 0x154, (w + rng.randrange(-3, 4)) & 0xFFFF)
        goal = rng.randrange(3)
        if goal == 0:
            f22 = c - 0xC0
        elif goal == 1:
            f22 = c + w - 0x120
        else:
            f22 = c - w - 0x60
        state.w16(sprite + 0x22, (f22 + rng.randrange(-3, 4)) & 0xFFFF)
    return Setup(args=(sprite,), returns_value=False)


def control(words):
    """Alter the large-span threshold: slti 0xc0 becomes slti 0xc1."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0A and w & 0xFFFF == 0xC0]
    if len(found) != 1:
        raise ValueError(f"expected one slti 0xc0, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0xC1, "span threshold 0xc0 changed to 0xc1"


CONTRACT = Contract(setup, control)
