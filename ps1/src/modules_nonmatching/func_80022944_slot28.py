"""Contract of func_80022944_slot28.

Choices of the setup:
  - the arena is moved to 0x80060000, past the module's data tables;
  - the given object (0x394 bytes) is random; its field_f0 is 0 in four
    cases of five and random otherwise;
  - the HUD state (0x64 bytes) is random;
  - the object of data_80051bd0_slot28 and the table words at index 0 to 9
    (data_80051ba4_slot28) are random blocks/words;
  - func_8011f1e0 returns, call by call, a new random object in three cases
    of four and 0 otherwise (ten calls, ten results), so that every arm of
    the loop and of the last request is tried;
  - the seven callees are recorders; the log watches the given object, the
    HUD state, the object of data_80051bd0_slot28, the ten objects that
    func_8011f1e0 can return, and the table (index 0 to 9 and the pointer
    behind it).
"""
from contracts import CallLog, Contract, Setup

OBJECT = 0x394


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym):
    # The arena of the tool begins at 0x80040000, inside this module's own
    # data (the tables at 0x80046d34 to 0x80051bd0). Blocks must not cover
    # it, so the arena is moved past the module image first.
    state.alloc(0x80060000 - state.arena)
    obj = state.alloc(OBJECT)
    fill(state, obj, OBJECT, rng)
    state.w8(obj + 0xF0, 0 if rng.random() < 0.8 else rng.randrange(1, 256))
    hud = state.alloc(0x64)
    fill(state, hud, 0x64, rng)
    state.w32(sym["data_8018f5a0"], hud)

    table = sym["data_80051ba4_slot28"]
    fill(state, table, 0x28, rng)
    first = state.alloc(OBJECT)
    fill(state, first, OBJECT, rng)
    state.w32(sym["data_80051bd0_slot28"], first)

    p_new = rng.choice((0.75, 0.75, 0.25, 1.0, 0.0))
    results, blocks = [], []
    for _ in range(10):
        if rng.random() < p_new:
            block = state.alloc(OBJECT)
            fill(state, block, OBJECT, rng)
            blocks.append(block)
            results.append(block)
        else:
            results.append(0)

    watch = [(obj, OBJECT // 4), (hud, 0x64 // 4), (first, OBJECT // 4), (table, 11)]
    watch += [(b, OBJECT // 4) for b in blocks]
    log = CallLog(state, 24 * (4 + sum(c for _, c in watch)), watch=tuple(watch))
    log.replace(sym["func_80022bf0_slot28"], 1, rng.getrandbits(32))
    log.replace(sym["func_80130768"], 3, 0)
    log.replace(sym["func_8011f1e0"], 0, results=tuple(results))
    log.replace(sym["func_80022c6c_slot28"], 1, 0)
    log.replace(sym["func_80128370"], 0, 0)
    log.replace(sym["func_8014f4d4"], 2, 0)
    log.replace(sym["func_80022c3c_slot28"], 2, 0)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of field_60 (sh ..., 0x60(reg)): its offset moves by two."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0x60]
    if len(found) != 1:
        raise ValueError(f"expected one store to offset 0x60, found {len(found)}")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0x62, "store of field_60 moved by two bytes"


CONTRACT = Contract(setup, control)
