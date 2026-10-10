"""Contract setup of func_801b1934_slot04_02 (see the header of the .c file).

Choices of the setup:
  - a0 is an object block of 0x394 bytes filled with random bytes;
  - field_12a is random (0..255) so that the table index wraps in the
    higher half; field_49 is 0 in half of the cases; field_cd is 0 in
    three cases of four (func_80138ae8 acts only when it is 0);
  - game_state.mode is 3 in half of the cases and game_state.field_2f is 0
    in half (both arms of the test in func_80138ae8);
  - data_80188ec4, the counter that func_80138ae8 raises, is a random
    halfword, so that the clamps of func_8013886c are tried too;
  - func_80141f28 and func_801307e0 are recorders (2 arguments each) that
    return a random word;
  - every recorder copies the whole object (0x394 bytes) into its log entry at
    each call, so the order of the function's stores against the calls is
    tested. No recorded callee gets a pointer to memory filled for the call.
"""

from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    log = CallLog(state, watch=((obj, 0x394 // 4),))
    log.replace(sym["func_80141f28"], 2, rng.getrandbits(32))
    log.replace(sym["func_801307e0"], 2, rng.getrandbits(32))

    game_state = sym["game_state"]
    state.w8(game_state + 0x1B, 3 if rng.random() < 0.5 else rng.randrange(256))
    state.w8(game_state + 0x2F, 0 if rng.random() < 0.5 else rng.randrange(256))
    state.w16(sym["data_80188ec4"], rng.getrandbits(16))

    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x12A, rng.randrange(256))
    state.w8(obj + 0x49, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    state.w8(obj + 0xCD, 0 if rng.random() < 0.75 else rng.randrange(1, 256))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Change the offset of the store of the second table word.

    The instruction is `sw a0,0x50(s0)` (field_50); every call executes it,
    and the word then lands in field_5c instead.
    """
    found = [i for i, w in enumerate(words) if w == 0xAE040050]
    if len(found) != 1:
        raise ValueError(f"expected one store to field_50, found {len(found)}")
    return found[0], 0xAE04005C, "store of the second table word moved from field_50 to field_5c"


CONTRACT = Contract(setup, control)
