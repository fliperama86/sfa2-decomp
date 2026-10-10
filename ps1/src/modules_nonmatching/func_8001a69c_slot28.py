"""Contract of func_8001a69c_slot28.

Choices of the setup:
  - the arena is moved to 0x80060000, past the module's data tables;
  - data_80190949 is non-zero in five cases of six and 0 otherwise; the
    given object (0x394 bytes) is random and its field_f0 is 0 in four
    cases of five;
  - the HUD state (0x64 bytes), the objects of data_800519c0_slot28,
    data_800519c4_slot28 and data_800519c8_slot28 (0x394 bytes each) and
    the two table words data_80051998_slot28, data_8005199c_slot28 are
    random;
  - func_8011f1e0 returns, call by call, a new random object or 0 (the
    chance of an object per case is one of 3/4, 1/4, 1, 0);
  - the six callees are recorders; the log watches the given object, the
    HUD state, the three table objects, the two returned objects and the
    five table words (data_80051998_slot28 to data_800519c8_slot28).
"""
from contracts import CallLog, Contract, Setup

OBJECT = 0x394


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym):
    # The arena of the tool begins at 0x80040000, inside this module's own
    # data (the tables at 0x80035430 to 0x800519c8). Blocks must not cover
    # it, so the arena is moved past the module image first.
    state.alloc(0x80060000 - state.arena)
    obj = state.alloc(OBJECT)
    fill(state, obj, OBJECT, rng)
    state.w8(obj + 0xF0, 0 if rng.random() < 0.8 else rng.randrange(1, 256))
    state.w8(sym["data_80190949"], rng.randrange(1, 256) if rng.random() < 5 / 6 else 0)
    hud = state.alloc(0x64)
    fill(state, hud, 0x64, rng)
    state.w32(sym["data_8018f5a0"], hud)

    low = sym["data_80051998_slot28"]
    fill(state, low, 8, rng)
    table = []
    for name in ("data_800519c0_slot28", "data_800519c4_slot28", "data_800519c8_slot28"):
        block = state.alloc(OBJECT)
        fill(state, block, OBJECT, rng)
        state.w32(sym[name], block)
        table.append(block)

    p_new = rng.choice((0.75, 0.75, 0.25, 1.0, 0.0))
    results, blocks = [], []
    for _ in range(2):
        if rng.random() < p_new:
            block = state.alloc(OBJECT)
            fill(state, block, OBJECT, rng)
            blocks.append(block)
            results.append(block)
        else:
            results.append(0)

    watch = [(obj, OBJECT // 4), (hud, 0x64 // 4), (low, 2), (sym["data_800519c0_slot28"], 3)]
    watch += [(b, OBJECT // 4) for b in table + blocks]
    log = CallLog(state, 12 * (5 + sum(c for _, c in watch)), watch=tuple(watch))
    log.replace(sym["func_8001a9e8_slot28"], 0, 0)
    log.replace(sym["func_80130768"], 3, 0)
    log.replace(sym["func_8011f1e0"], 0, results=tuple(results))
    log.replace(sym["func_8001a988_slot28"], 1, 0)
    log.replace(sym["func_8001a958_slot28"], 2, 0)
    log.replace(sym["func_80128370"], 0, 0)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of field_60 (sh ..., 0x60(reg)): its offset moves by two."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0x60]
    if len(found) != 1:
        raise ValueError(f"expected one store to offset 0x60, found {len(found)}")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0x62, "store of field_60 moved by two bytes"


CONTRACT = Contract(setup, control)
