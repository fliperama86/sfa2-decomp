"""Contract of func_8011f06c: no argument, no result.

Choices made here:
  - the first 0x400 bytes of game_state are random, so that the bytes the
    function clears (0x40 to 0xbf and 0x226) and the ones it must leave
    alone (everything else, the borders included) are both tested.
"""

from contracts import Contract, Setup


def setup(state, rng, sym) -> Setup:
    game_state = sym["game_state"]
    state.write(game_state, bytes(rng.getrandbits(8) for _ in range(0x400)))
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the store of the byte at field_226: its offset moves by one."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0x032E]
    if len(found) != 1:
        raise ValueError(f"expected one store to field_226, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x032F, "store of field_226 moved by one byte"


CONTRACT = Contract(setup, control)
