"""Contract of func_8001389c_slot28.

Choices of the setup:
  - the object (0x394 bytes), the HUD state (0x64 bytes) and the 16-byte
    table, with 16 bytes of random content on each side of it (these
    neighbours must stay as they are), are random;
  - func_8011eae4 (no argument) and func_80013c38_slot28 (one argument) are
    recorders returning 0; the log watches the object, the HUD state and
    the table.
"""
from contracts import CallLog, Contract, Setup

TABLE = 0x80051EA8


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    hud = state.alloc(0x64)
    fill(state, hud, 0x64, rng)
    state.w32(sym["data_8018f5a0"], hud)
    table = sym["data_80051ea8_slot28"]
    fill(state, table - 16, 48, rng)
    log = CallLog(state, 2048, watch=((obj, 0x394 // 4), (hud, 0x64 // 4), (table, 4)))
    log.replace(sym["func_8011eae4"], 0, 0)
    log.replace(sym["func_80013c38_slot28"], 1, 0)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of field_ad (sb ..., 0xad(reg)): its offset moves by one."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0xAD]
    if len(found) != 1:
        raise ValueError(f"expected one store to offset 0xad, found {len(found)}")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0xAE, "store of field_ad moved by one byte"


CONTRACT = Contract(setup, control)
