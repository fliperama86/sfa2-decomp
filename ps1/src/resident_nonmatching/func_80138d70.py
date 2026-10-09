"""Contract of func_80138d70 (see the header comment of func_80138d70.c).

Choices of the setup:
  - field_30 is 0 in nine cases of ten and random otherwise (the function
    returns at once when it is not 0);
  - field_2d is 0..15 and field_54 is 0..31; field_42 is in -4..8 in
    four cases of five (the clamp to 3 at 4 and above, and values that make
    the row negative); in the others a multiple of 0x1000 is added to such a
    value, so that field_42 reaches the large positive values (clamped) and
    the negative halfwords (taken as unsigned for the row, whose 16-bit
    index is the same) without leaving the filled table;
  - the table region from 0x200 bytes below data_80176da0 to 0x600 above it
    is filled with random bytes of 0..16, so that the two entry bytes sum to
    at most 32 (the pattern has 32 bytes); a quarter of the entries are 0,
    so that both "no zeros" and "no fills" are taken;
  - field_cd of each player is 0 in half the cases and random otherwise;
    field_220 of both players and the generator's state start random;
  - func_80151184 is not replaced.
"""

from contracts import Contract, Setup


def setup(state, rng, sym):
    game = state.alloc(0x364)
    state.write(game, bytes(rng.getrandbits(8) for _ in range(0x364)))
    state.w8(game + 0x30, 0 if rng.random() < 0.9 else rng.randrange(1, 256))
    state.w8(game + 0x2D, rng.randrange(16))
    state.w16(game + 0x54, rng.randrange(32))
    if rng.random() < 0.8:
        state.w16(game + 0x42, rng.randrange(-4, 9) & 0xFFFF)
    else:
        # a multiple of 0x1000 added: the row times 16 keeps its low 16 bits, so the index stays in the table
        state.w16(game + 0x42, (rng.randrange(-4, 9) + 0x1000 * rng.randrange(16)) & 0xFFFF)
    table = sym["data_80176da0"]
    state.write(table - 0x200, bytes(0 if rng.random() < 0.25 else rng.randrange(17) for _ in range(0x800)))
    for name in ("player_left", "player_right"):
        player = sym[name]
        state.w8(player + 0xCD, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
        state.w8(player + 0x220, rng.getrandbits(8))
    state.w16(sym["data_80190126"], rng.getrandbits(16))
    return Setup(args=(game,), returns_value=False)


def control(words):
    """Alter the pattern byte for the fills: the constant 0x20 becomes 0x21."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0D and w & 0xFFFF == 0x20 and (w >> 21) & 31 == 0]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x20, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x21, "fill byte 0x20 changed to 0x21"


CONTRACT = Contract(setup, control)
