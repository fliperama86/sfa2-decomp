"""Contract of func_80135a10, as code. Header comment of the .c has the text.

Choices of the setup:
  - data_80190474 is 1 in two cases of five, 2 in two of five, and random
    otherwise (so that the 'neither' arm is tried);
  - game_state.field_78 is null in one case of six; otherwise it points at
    player_left, the object after it, or a separate random block, equally;
    in the selected object (a player or the separate block) field_65 is
    set to 0 or 1, one case of two each;
  - field_ce is random (any byte) in a block that is the selected object;
    field_167 is random with its high bit forced on in half of the
    cases (so it is set in three cases of four);
  - both player objects (2 * 0x394 bytes) start with random bytes, so that
    the byte the function writes at an index up to 255 (field_ce is any
    byte) and the bytes it must leave alone are all tested;
  - the byte tables bytes_d0[0..3] of both players hold 8 in each entry
    with probability one half, otherwise a random byte;
  - func_80120554 and func_80135c0c are recorders returning 0; both take
    three arguments; the first is given the whole player object it is
    passed as a pointee (229 words); every recorder copies both player
    objects, whole, at every call.
"""

from contracts import CallLog, Contract, Setup


def setup(state, rng, sym):
    left = sym["player_left"]
    right = left + 0x394
    log = CallLog(state, 8192, watch=((left, 0x394 // 4), (right, 0x394 // 4)))
    log.replace(sym["func_80120554"], 3, 0, pointees={0: 0x394 // 4})
    log.replace(sym["func_80135c0c"], 3, 0)
    state.write(left, bytes(rng.getrandbits(8) for _ in range(2 * 0x394)))
    mode = rng.choice((1, 1, 2, 2, rng.randrange(256)))
    state.w8(sym["data_80190474"], mode)
    for player in (left, right):
        for k in range(4):
            state.w8(player + 0xD0 + k, 8 if rng.random() < 0.5 else rng.getrandbits(8))
    game_state = sym["game_state"]
    which = rng.randrange(3)
    if rng.random() < 1 / 6:
        state.w32(game_state + 0x78, 0)
    else:
        if which == 0:
            obj = left
        elif which == 1:
            obj = right
        else:
            obj = state.alloc(0x394)
            state.write(obj, bytes(rng.getrandbits(8) for _ in range(0x394)))
        state.w8(obj + 0x65, rng.randrange(2))
        state.w8(obj + 0xCE, rng.getrandbits(8))
        state.w8(obj + 0x167, rng.getrandbits(8) | (0x80 if rng.random() < 0.5 else 0))
        state.w32(game_state + 0x78, obj)
    return Setup(args=(), returns_value=False)


def control(words):
    """Change the code stored for the two strips: the constant 0x20 of mode 2.

    The one `ori`-form constant 0x20 (s2 in the original, an immediate in the
    build) is found in the build as the immediate 0x20 of an `addiu`/`ori` that
    loads the third argument of the calls in mode 2.
    """
    found = [i for i, w in enumerate(words) if (w >> 26) in (0x09, 0x0D) and w & 0xFFFF == 0x20 and (w >> 16) & 0x1F in (6, 4)]
    if not found:
        raise ValueError("no load of the constant 0x20 found")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x21, "mode 2 code 0x20 changed to 0x21"


CONTRACT = Contract(setup, control)
