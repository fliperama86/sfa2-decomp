"""Contract of func_80136744 (see the header comment of func_80136744.c).

Choices of the setup:
  - the sprite is a 0x4c-byte block of random bytes; field_26 is random and
    the players' pos_y are chosen, in two cases of three, so that the target
    minus 0x39 differs from field_26 by -2..2 (both signs and zero, and the
    sign of the 16-bit difference at its edges); in the others they are
    random;
  - the two players' blocks are filled with random bytes, then field_73 bit
    7 and field_261 zero-ness are steered: each player is "ready" (bit 7 set
    and field_261 0) in a third of the cases, has bit 7 clear in a third and
    bit 7 set with field_261 not 0 in a third, so that the three arms of the
    target are all taken; pos_y of both players is equal in a quarter of the
    steered cases (the max of the third arm);
  - func_801368ac is not replaced.
"""

from contracts import Contract, Setup


def setup(state, rng, sym):
    def fill(address, size):
        state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))

    sprite = state.alloc(0x4C)
    fill(sprite, 0x4C)
    f26 = rng.getrandbits(16)
    state.w16(sprite + 0x26, f26)
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
    if rng.random() < 0.67:
        target = (f26 + 0x39 + rng.randrange(-2, 3)) & 0xFFFF
        y = (0xD0 - target) & 0xFFFF
        state.w16(left + 0x16, y)
        state.w16(right + 0x16, y if rng.random() < 0.25 else (y + rng.randrange(-3, 4)) & 0xFFFF)
    return Setup(args=(sprite,), returns_value=False)


def control(words):
    """Alter the store of field_26 after the increment: offset 0x26 becomes 0x28."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0x26]
    if not found:
        raise ValueError("expected a store of field_26")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x28, "first field_26 store moved by two bytes"


CONTRACT = Contract(setup, control)
