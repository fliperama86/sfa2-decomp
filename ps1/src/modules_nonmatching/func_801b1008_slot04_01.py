"""Contract setup of func_801b1008_slot04_01 (see the header of the .c file).

Choices of the setup:
  - a0 is an object block of 0x394 bytes filled with random bytes;
  - field_40 points to a second object block of 0x394 bytes, also random;
  - func_801307e0 is a recorder (2 arguments) returning a random word;
  - the recorder copies the whole object and the whole other object into its
    log entry at the call, so the order of the stores against the call is
    tested. No recorded callee gets a pointer to memory filled for the call.
"""

from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    other = state.alloc(0x394)
    log = CallLog(state, watch=((obj, 0x394 // 4), (other, 0x394 // 4)))
    log.replace(sym["func_801307e0"], 2, rng.getrandbits(32))
    fill(state, obj, 0x394, rng)
    fill(state, other, 0x394, rng)
    state.w32(obj + 0x40, other)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Change the sequence constant 0x41 passed to func_801307e0."""
    found = [i for i, w in enumerate(words) if w == 0x34050041]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x41, found {len(found)}")
    return found[0], 0x34050042, "sequence constant 0x41 changed to 0x42"


CONTRACT = Contract(setup, control)
