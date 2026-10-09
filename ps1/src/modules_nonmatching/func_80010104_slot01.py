"""Contract of func_80010104_slot01 as code.

No argument. game_state.field_78 points at an object whose field_2ac the
function saves and restores around three setup functions.

Reads and writes are listed in the header comment of func_80010104_slot01.c.
Choices made here:
  - the object, the record behind data_8018f5a0, the players and the
    allocated blocks are filled with random bytes; the words of the globals
    and of game_state that the function writes are random too;
  - data_801abf08 is 0x12 in half the cases (the middle setup function is
    then skipped) and random otherwise; data_80197f10 is one of 0, 1, 2, 3
    or random in equal shares (the second half of the function runs from 2);
  - the three setup functions are recorders (no argument, result 0) that
    each store a new random value into the object's field_2ac at their
    first call;
  - func_8011f1e0 is a recorder that returns, per call, a block of the
    setup or 0 (one case in four for either);
  - func_80157fc4 and func_80157d9c, func_80151020, func_80125dc0 and
    func_80137220 are recorders with their declared numbers of arguments;
    the two memory blocks behind the first two loads (data_80015174_slot01
    and data_80070d48, 0xa0 bytes each) are random;
  - the log copies at every call the object, both blocks, and the words of
    the written globals and fields (see the header comment).
"""

from contracts import CallLog, Contract, Setup, fill, halfword


def setup(state, rng, sym) -> Setup:
    game_state = sym["game_state"]
    other = state.alloc(0x394)
    fill(state, other, 0x394, rng)
    state.w32(game_state + 0x78, other)

    hud = state.alloc(0x64)
    fill(state, hud, 0x64, rng)
    state.w32(sym["data_8018f5a0"], hud)

    words = (0x08, 0x2C, 0x40, 0x4C, 0xA8, 0xC8, 0x64)
    for offset in words:
        state.w32(game_state + offset, rng.getrandbits(32))
    state.w32(game_state + 0x78, other)
    state.w8(sym["data_801abf08"], 0x12 if rng.random() < 0.5 else rng.getrandbits(8))
    state.w8(sym["data_80197f10"], rng.choice((0, 1, 2, 3, rng.getrandbits(8))))
    for name in ("data_80055f3c_slot01", "data_8002ceb0_slot01", "data_80055f44_slot01",
                 "data_801ac6a8", "data_80190568"):
        state.w32(sym[name], rng.getrandbits(32))
    state.w32(sym["data_801aa5ea"] & ~3, rng.getrandbits(32))
    fill(state, sym["player_left"], 0x394, rng)
    fill(state, sym["player_right"], 0x394, rng)
    fill(state, sym["data_80015174_slot01"], 0xA0, rng)
    fill(state, sym["data_80070d48"], 0xA0, rng)

    blocks = [state.alloc(0xAC) for _ in range(2)]
    for block in blocks:
        fill(state, block, 0xAC, rng)
    results = tuple(0 if rng.random() < 0.25 else block for block in blocks)

    watch = [(other, 0x394 // 4), (blocks[0], 0xAC // 4), (blocks[1], 0xAC // 4)]
    watch += [(game_state + offset, 1) for offset in words]
    watch += [(hud + 0x4C, 1), (sym["player_left"], 1), (sym["player_right"], 1)]
    watch += [(sym[name], 1) for name in ("data_80055f3c_slot01", "data_8002ceb0_slot01", "data_80055f44_slot01",
                                           "data_801ac6a8", "data_80190568")]
    watch.append((sym["data_801aa5ea"] & ~3, 1))
    log = CallLog(state, 20000, watch=tuple(watch))

    for name in ("func_8011eb4c", "func_801285e0", "func_8011a744"):
        log.replace(sym[name], 0, 0, stores=((1, other + 0x2AC, rng.getrandbits(32)),))
    log.replace(sym["func_80157fc4"], 2, 0, pointees={0: 2, 1: 40}, masks={0: 3, 1: 3})
    log.replace(sym["func_80157d9c"], 1, 0)
    log.replace(sym["func_8011f1e0"], 0, results=results)
    log.replace(sym["func_80151020"], 1, 0)
    log.replace(sym["func_80125dc0"], 4, 0)
    log.replace(sym["func_80137220"], 2, 0)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the first load of the constant 0x605 (the argument of the sound call): it becomes 0x606."""
    found = [i for i, w in enumerate(words) if w >> 26 in (0x0D, 0x09) and w & 0xFFFF == 0x605]
    if not found:
        raise ValueError("expected a load of the constant 0x605, found none")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x606, "constant 0x605 changed to 0x606"


CONTRACT = Contract(setup, control)
