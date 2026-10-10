"""Contract of func_801ea3b4_slot06_08: an object that follows another one.

Reads and writes are listed in the header comment of
func_801ea3b4_slot06_08.c. Choices made here:
  - game_state.field_65 and field_74 are both 0 in three cases of four, so
    that the body is reached; otherwise one or both are random non-zero;
  - the object's field_4c is 0 in three cases of four;
  - the low byte of field_3a is 0 in one case of three, the high byte is
    random;
  - the followed object's field_06 is 8 in two cases of five, and the
    object's field_54 low byte equals it in one case of four, so that all
    three comparisons take both arms;
  - func_80131094 and func_80120028 are replaced by recorders taking one
    argument (they reach game objects and the library); both return 0;
  - every recorder copies the whole object (0x394 bytes) into the log at each
    call, so a store made on the other side of a call is a difference; the
    followed object and the globals are not written, so they are not watched;
    the callees' argument is the object itself, so no pointee is named;
  - the object and the followed object are random blocks of 0x394 bytes.
"""

from contracts import CallLog, Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym) -> Setup:
    gs = sym["game_state"]
    if rng.random() < 0.75:
        a65 = a74 = 0
    else:
        a65, a74 = rng.choice(((0, 1), (1, 0), (1, 1)))
        a65 *= rng.randrange(1, 256)
        a74 *= rng.randrange(1, 256)
    state.w8(gs + 0x65, a65)
    state.w8(gs + 0x74, a74)

    target = state.alloc(0x394)
    fill(state, target, 0x394, rng)
    field_06 = 8 if rng.random() < 0.4 else rng.randrange(256)
    state.w8(target + 0x06, field_06)

    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w32(obj + 0x3C, target)
    state.w32(obj + 0x4C, 0 if rng.random() < 0.75 else rng.getrandbits(32))
    low = 0 if rng.random() < 1 / 3 else rng.randrange(1, 256)
    state.w16(obj + 0x3A, rng.randrange(256) << 8 | low)
    if rng.random() < 0.25:
        state.w32(obj + 0x54, rng.getrandbits(24) << 8 | field_06)

    log = CallLog(state, words=1024, watch=((obj, 0x394 // 4),))
    log.replace(sym["func_80131094"], 1, 0)
    log.replace(sym["func_80120028"], 1, 0)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the store of field_54: its offset moves by four bytes.

    The store is `sw v0,0x54(s0)`, made by every call that reaches the end
    of the body with field_4c equal to 0 or the game_state test failed.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x2B and w & 0xFFFF == 0x54]
    if len(found) != 1:
        raise ValueError(f"expected one store at offset 0x54, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x58, "field_54 store moved by four bytes"


CONTRACT = Contract(setup, control)
