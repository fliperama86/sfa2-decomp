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
    """Alter the argument register at the call: the empty delay slot of `jalr v0` becomes `addiu a0,a0,4`.

    The control alters the build of this C (the tool hands it the build's code words, not the original's). The
    altered build then hands the function in the pointer the object plus 4, which the recorder logs, while the
    original code hands it the object: this is the alteration that shows the test sees the value the callee gets.
    """
    found = [i for i, w in enumerate(words) if w == 0x0040F809]
    if len(found) != 1 or words[found[0] + 1] != 0:
        raise ValueError("expected one call through v0 with an empty delay slot")
    return found[0] + 1, 0x24840004, "the callee gets a0 + 4"


CONTRACT = Contract(setup, control)
