"""Contract of func_800145f4_slot28.

Choices of the setup:
  - the object (0x394 bytes) is random; its field_f0 is 0 in three cases of
    five and random otherwise;
  - the HUD state (0x64 bytes) is random, so field_52 wraps in some cases;
  - the table data_8005187c_slot28 holds three distinct 16-byte blocks of
    random content;
  - func_80014854_slot28 (two arguments) and func_80128370 (none) are
    recorders; the log watches the object, the HUD state and the three
    table blocks.
"""
from contracts import CallLog, Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    if rng.random() < 0.6:
        state.w8(obj + 0xF0, 0)
    hud = state.alloc(0x64)
    fill(state, hud, 0x64, rng)
    state.w32(sym["data_8018f5a0"], hud)
    blocks = []
    for index in range(3):
        block = state.alloc(16)
        fill(state, block, 16, rng)
        state.w32(sym["data_8005187c_slot28"] + 4 * index, block)
        blocks.append(block)
    watch = ((obj, 0x394 // 4), (hud, 0x64 // 4)) + tuple((b, 4) for b in blocks)
    log = CallLog(state, 4096, watch=watch)
    log.replace(sym["func_80014854_slot28"], 2, 0)
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
