"""Contract setup of func_801b3a70_slot04_05 (see the header of the .c file).

Choices of the setup:
  - a0 is an object block of 0xac8 bytes (room for record indices up to
    255 of the table at 0x2b0), filled with random bytes;
  - a1 is a random byte, a2 a row index 0 to 30; the high bits of both
    registers are random, since the function uses only the low byte;
  - the rows 0 to 30 of the mask table (the module's own data) get random
    halfwords at the offset the function reads (a zero mask in one row of
    eight); then field_134 is made to share a bit with the chosen row's
    mask in half of the cases (when the mask is non-zero) and to share none
    in the other half;
  - the counter byte (record offset 2) is 1 in one case of three, the timer
    byte (offset 4) is 1 in one case of three, otherwise random (0 included,
    which wraps to 0xff);
  - data_801c1700_slot04_05 is a random word before the call.
"""

from contracts import Contract, Setup, fill


def setup(state, rng, sym):
    obj = state.alloc(0xAC8)
    fill(state, obj, 0xAC8, rng)
    table = sym["data_801c1630_slot04_05"]
    for row in range(31):
        mask = 0 if rng.random() < 0.125 else rng.getrandbits(16)
        state.w16(table + 12 + 6 * row, mask)
    state.w32(sym["data_801c1700_slot04_05"], rng.getrandbits(32))
    idx = rng.randrange(256)
    row = rng.randrange(31)
    mask = int.from_bytes(state.read(table + 12 + 6 * row, 2), "little")
    field = rng.getrandbits(16)
    if rng.random() < 0.5 and mask:
        field |= 1 << rng.choice([b for b in range(16) if mask >> b & 1])
    else:
        field &= ~mask & 0xFFFF
    state.w16(obj + 0x134, field)
    rec = obj + 0x2B0 + 8 * idx
    for offset in (2, 4):
        state.w8(rec + offset, 1 if rng.random() < 0.33 else rng.randrange(256))
    a1 = idx | rng.getrandbits(24) << 8
    a2 = row | rng.getrandbits(24) << 8
    return Setup(args=(obj, a1, a2), returns_value=False)


def control(words):
    """Change the value stored in the timer byte: the ori of 0xc."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0xD and w & 0xFFFF == 0xC]
    if len(found) != 1:
        raise ValueError(f"expected one ori of 0xc, found {len(found)}")
    return found[0], words[found[0]] + 1, "timer value 0xc changed to 0xd"


CONTRACT = Contract(setup, control)
