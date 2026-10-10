"""Contract setup of func_801b20b0_slot04_06 (see the header of the .c file).

Choices of the setup:
  - a0 is an object block of 0x394 bytes filled with random bytes;
  - field_12a is a random byte (0 and 0xff in one case of eight each), field_07
    a random byte with 0xff in one case of eight;
  - the module's two tables, which are one array of words read one word
    apart, get 257 random words (indices 0 to 256);
  - func_80141f28, func_80138ae8 and func_801307e0 (2 arguments each) are
    recorders returning a random word; each copies the whole object into its
    log entry at every call, so the order of the function's stores against
    the calls is tested.
"""

from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    log = CallLog(state, watch=((obj, 0x394 // 4),))
    for name in ("func_80141f28", "func_80138ae8", "func_801307e0"):
        log.replace(sym[name], 2, rng.getrandbits(32))
    fill(state, obj, 0x394, rng)
    table = sym["data_801c5398_slot04_06"]
    for i in range(257):
        state.w32(table + 4 * i, rng.getrandbits(32))
    r = rng.random()
    state.w8(obj + 0x12A, 0 if r < 0.125 else 0xFF if r < 0.25 else rng.randrange(256))
    state.w8(obj + 0x07, 0xFF if rng.random() < 0.125 else rng.randrange(256))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Change the base of the sequence number: the addiu of 0x1b."""
    found = [i for i, w in enumerate(words) if w >> 26 == 9 and w & 0xFFFF == 0x1B]
    if len(found) != 1:
        raise ValueError(f"expected one addiu of 0x1b, found {len(found)}")
    return found[0], words[found[0]] + 1, "sequence base 0x1b changed to 0x1c"


CONTRACT = Contract(setup, control)
