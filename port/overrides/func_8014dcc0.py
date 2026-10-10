"""Contract of func_8014dcc0 in the port's C (header comment of the .c file).

Choices of the setup:
  - the object is a 0x394-byte block of random bytes; its byte at 0xa6 (the
    side) is 0 in half of the cases and otherwise 1 to 255;
  - scr_d4_left and scr_184_right (scratchpad words) each hold the address
    of its own small block, and a recorder of one argument sits in each
    block; both return 0;
  - data_801ad398 is a random byte;
  - the log watches the object whole and data_801ad398 at every call.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym) -> Setup:
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0xA6, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    state.w8(sym["data_801ad398"], rng.getrandbits(8))
    log = CallLog(state, 256, watch=((obj, 0x394 // 4), (sym["data_801ad398"] & ~3, 1)))
    for cell in ("scr_d4_left", "scr_184_right"):
        block = state.alloc(8)
        state.w32(sym[cell], block)
        log.replace(block, 1, 0)
    return Setup(args=(obj,), returns_value=True)


def control(words):
    """Alter the load of the side: `lbu v0,0xa6(a0)` reads 0xa7."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x24 and (w >> 21) & 31 == 4 and (w >> 16) & 31 == 2 and w & 0xFFFF == 0xA6]
    if len(found) != 1:
        raise ValueError(f"expected one load of the side, found {len(found)}")
    i = found[0]
    return i, words[i] + 1, "side read from the byte after it"


CONTRACT = Contract(setup, control)
