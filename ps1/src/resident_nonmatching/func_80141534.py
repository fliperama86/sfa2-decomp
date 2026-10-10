"""Contract of func_80141534 (header comment of the .c file lists reads and writes).

Choices of the setup:
  - table_8017179c[0..7] are overwritten with pointers to eight blocks of
    two random 16-bit entries; the object's kind is 0 to 7;
  - the object is a 0x394-byte block of random bytes; field_130 has each of
    the bits 0x8000, 0x4000 and 0x2000 set with probability one half, the
    low 13 bits random, so the early return (neither 0x8000 nor 0x2000) is
    taken one case in four;
  - the pair `dir` is two random bytes, the second one 0 in half of the
    cases; the lower half of a1 is random.
"""
from contracts import CallLog, Contract, Setup, fill, halfword


def setup(state, rng, sym) -> Setup:
    table = sym["table_8017179c"]
    for index in range(8):
        entry = state.alloc(4)
        fill(state, entry, 4, rng)
        state.w32(table + 4 * index, entry)
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0xA7, rng.randrange(8))
    flags = halfword(rng) & 0x1FFF
    for bit in (0x8000, 0x4000, 0x2000):
        if rng.random() < 0.5:
            flags |= bit
    state.w16(obj + 0x130, flags)
    first = rng.randrange(256)
    second = rng.choice((0, rng.randrange(256)))
    a1 = second << 24 | first << 16 | halfword(rng)
    return Setup(args=(obj, a1), returns_value=True)


def control(words):
    """Alter the final xori 1 (invert the tested bit) to xori 0."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0E and w & 0xFFFF == 1]
    if not found:
        raise ValueError("no xori 1 found")
    i = found[-1]
    return i, words[i] & ~0xFFFF, "final inversion xori 1 replaced by xori 0"


CONTRACT = Contract(setup, control)
