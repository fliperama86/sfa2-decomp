"""Contract of func_8014c9f4 (header comment of the .c file lists reads and writes).

Choices of the setup:
  - the object is a 0x394-byte block of random bytes;
  - field_20a is 0 in half of the cases (the first callee is called) and a
    random byte otherwise;
  - field_22c points at a block of 8 random bytes, 2-byte aligned at a random
    one of its first three halfword positions;
  - func_8014d9ac and func_8014da18 are recorders with one argument each
    (the object; it holds no pointer to memory filled for the call, so no
    pointee); the log watches the whole object and data_80189460 at every call.
"""
from contracts import CallLog, Contract, Setup, fill, halfword


def setup(state, rng, sym) -> Setup:
    script = state.alloc(8)
    fill(state, script, 8, rng)
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    log = CallLog(state, 1024, watch=((obj, 0x394 // 4), (sym["data_80189460"], 1)))
    log.replace(sym["func_8014d9ac"], 1)
    log.replace(sym["func_8014da18"], 1)
    state.w8(obj + 0x20A, rng.choice((0, rng.randrange(256))))
    state.w32(obj + 0x22C, script + 2 * rng.randrange(3))
    state.w32(sym["data_80189460"], rng.getrandbits(32))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the byte store to field_20f: its offset moves by one byte."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0x20F]
    if len(found) != 1:
        raise ValueError(f"expected one store to field_20f, found {len(found)}")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0x210, "store of field_20f moved by one byte"


CONTRACT = Contract(setup, control)
