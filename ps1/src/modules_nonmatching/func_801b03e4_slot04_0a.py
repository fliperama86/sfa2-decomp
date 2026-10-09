"""Contract of func_801b03e4_slot04_0a: a character state that picks a random variant.

Reads and writes are listed in the header comment of
func_801b03e4_slot04_0a.c. Choices made here:
  - game_state.field_64 and field_5c are both 0 in four cases of five (the
    arm that runs the function's body); otherwise one or both are random
    non-zero bytes, so that each of the two tests and the other arm run;
  - the object is a block of 0x394 bytes of random content (func_80125734
    reads fields 0xa7, 0xcd, 0x130 of it, so both its early exit and its
    search loop run), field_06 random, field_46 and field_48 random;
  - the random generator state is random;
  - func_80130678 and func_80130efc are recorders returning 0, the log
    watches the whole object and game_state.field_76.
"""

from contracts import CallLog, Contract, Setup, fill, halfword  # noqa: F401


def setup(state, rng, sym) -> Setup:
    game_state = sym["game_state"]
    if rng.random() < 0.8:
        state.w8(game_state + 0x64, 0)
        state.w8(game_state + 0x5C, 0)
    else:
        state.w8(game_state + 0x64, rng.choice((0, rng.randrange(1, 256))))
        state.w8(game_state + 0x5C, rng.choice((0, rng.randrange(1, 256))))
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    # func_80125734 tests bit 0x100 of the halfword at 0x130, its early exit needs byte 0xcd non-zero.
    if rng.random() < 0.3:
        state.w8(obj + 0xCD, rng.randrange(1, 256))
    elif rng.random() < 0.5:
        state.w8(obj + 0xCD, 0)
    if rng.random() < 0.7:
        state.w16(obj + 0x130, 0x100 | (rng.getrandbits(16) & 0xFF))
    log = CallLog(state, 300, watch=((obj, 0x394 // 4), (game_state + 0x74, 1)))
    log.replace(sym["func_80130678"], 2, 0, masks={1: 0xFFFF})
    log.replace(sym["func_80130efc"], 1, 0)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of the 0x3c00 constant: the build stores another halfword.

    The immediate 0x3c00 is loaded once, in the arm that runs when the game
    is not paused; the store of field_46 writes it.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0D and w & 0xFFFF == 0x3C00]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x3c00, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x3C01, "constant stored in field_46 changed by one"


CONTRACT = Contract(setup, control)
