"""Contract of func_800e68e8_slot0f (reads and writes: see the header of the .c).

Choices of the setup:
  - n is 2 in a third of the cases, otherwise a random value below 76; the
    upper half of a2's register for n is random, since only its low 16 bits
    are used;
  - every entry of data_800efdc8_slot0f and data_800efef8_slot0f is the
    address of its own block of 8 random halfwords; data_800eff58_slot0f is
    random for its first 76 entries;
  - the object is random (field_48 below 24); the data_801987c8 block is
    random;
  - the two records are filled with random bytes, so that the bytes the
    function leaves alone are tested;
  - func_8015bd0c is a recorder with 4 arguments returning a random word;
    the log copies both records at the call.
"""

from contracts import CallLog, Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym) -> Setup:
    for index in range(76):
        block = state.alloc(16)
        fill(state, block, 16, rng)
        state.w32(sym["data_800efdc8_slot0f"] + 4 * index, block)
        state.w16(sym["data_800eff58_slot0f"] + 2 * index, rng.getrandbits(16))
    for index in range(24):
        block = state.alloc(16)
        fill(state, block, 16, rng)
        state.w32(sym["data_800efef8_slot0f"] + 4 * index, block)
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x48, rng.randrange(24))
    records = state.alloc(0x40)
    fill(state, records, 0x40, rng)
    other = state.alloc(0x394)
    fill(state, other, 0x394, rng)
    state.w32(sym["data_801987c8"], other)
    n = 2 if rng.random() < 1 / 3 else rng.randrange(76)
    n |= rng.getrandbits(16) << 16 if rng.random() < 0.5 else 0

    log = CallLog(state, 256, watch=((records, 0x10),))
    log.replace(sym["func_8015bd0c"], 4, result=rng.getrandbits(32))
    return Setup(args=(obj, records, n), returns_value=False)


def control(words):
    """Alter the texture page: `ori v0,zero,0x7f07` becomes 0x7f06."""
    found = [i for i, w in enumerate(words) if w >> 26 == 13 and w & 0xFFFF == 0x7F07]
    if not found:
        raise ValueError("expected the constant 0x7f07")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x7F06, "texture page constant changed"


CONTRACT = Contract(setup, control)
