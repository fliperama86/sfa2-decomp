"""Contract of func_8011a880 (no argument, no result).

Choices of the setup (details in the header of func_8011a880.c):
  - player_left and player_right are filled with random bytes; then field_00
    and field_01 of each are non-zero in three cases of four each, field_73
    is 0 in two cases of five, 1 in one of three of the rest, else random;
    for the tie arms field_17a of both fighters is drawn from 0 to 3, the
    frame field_04 values from 0 to 3, so that less, equal and greater all
    occur often;
  - the frame pointers (at 0x88 of each fighter and frames_left and
    frames_right in the scratchpad) point at blocks of the setup;
  - each of the four part pointers of a fighter is null in one case of four,
    else a block of 0xac random bytes whose field_01 is 0 in one case of
    three;
  - CallLog watches player_left and player_right whole (0x394 bytes each) at
    every call, and func_8011bc84 records the 0xac bytes of its argument;
  - func_8011bc84, func_8011cbb8 (1 argument) and func_80151324 (2
    arguments) are recorders that return 0.
"""

from contracts import CallLog, Contract, Setup


def _fighter(state, rng, base, sym):
    for offset in range(0, 0x394, 4):
        state.w32(base + offset, rng.getrandbits(32))
    state.w8(base + 0x00, rng.choice((0, 1, rng.randrange(256))) if rng.random() < 0.25 else rng.randrange(1, 256))
    state.w8(base + 0x01, rng.choice((0, 1, rng.randrange(256))) if rng.random() < 0.25 else rng.randrange(1, 256))
    r = rng.random()
    state.w8(base + 0x73, 0 if r < 0.4 else 1 if r < 0.7 else rng.randrange(256))
    state.w8(base + 0x17A, rng.randrange(4))
    parts = []
    frame = state.alloc(16)
    for offset in range(16):
        state.w8(frame + offset, rng.getrandbits(8))
    state.w8(frame + 4, rng.randrange(4))
    state.w32(base + 0x88, frame)
    for offset in (0x28, 0x2C, 0x30, 0x34):
        if rng.random() < 0.25:
            state.w32(base + offset, 0)
        else:
            part = state.alloc(0xAC)
            for k in range(0, 0xAC, 4):
                state.w32(part + k, rng.getrandbits(32))
            if rng.random() < 0.33:
                state.w8(part + 1, 0)
            state.w32(base + offset, part)
            parts.append(part)
    return parts


def setup(state, rng, sym):
    _fighter(state, rng, sym["player_left"], sym)
    _fighter(state, rng, sym["player_right"], sym)
    state.w32(sym["frames_left"], state.alloc(16))
    state.w32(sym["frames_right"], state.alloc(16))
    # Both fighters are watched whole at every call, and func_8011bc84 gets
    # the 43 words of the block it is passed (a part, whose field_09 the
    # function sets just before the call).
    log = CallLog(state, words=16384, watch=((sym["player_left"], 0x394 // 4), (sym["player_right"], 0x394 // 4)))
    log.replace(sym["func_8011bc84"], 1, 0, pointees={0: 0xAC // 4})
    log.replace(sym["func_8011cbb8"], 1, 0)
    log.replace(sym["func_80151324"], 2, 0)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the constant of the part marker: `addiu v0,v0,4` becomes 5.

    The addition of 4 to the fighter's field_09 is made before every
    call of func_8011bc84, which the setup reaches in nearly every case.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x09 and (w >> 21) & 31 == 2 and (w >> 16) & 31 == 2 and w & 0xFFFF == 4]
    if not found:
        raise ValueError("no addiu v0,v0,4 found")
    index = found[0]
    return index, words[index] + 1, "part field_09 offset 4 becomes 5"


CONTRACT = Contract(setup, control)
