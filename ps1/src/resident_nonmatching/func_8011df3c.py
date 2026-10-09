"""Contract of func_8011df3c as code. Reads and writes are listed in the
header comment of func_8011df3c.c. Choices made here:
  - the flags: bit 15 and bit 14 random, the table index (bits 0 to 13) 0 to
    7, the upper 16 bits random in a quarter of the cases;
  - the data block is 36 KiB of random bytes; the eight table entries point
    into its last 2 KiB (even offsets) and the stream of the case starts at
    the entry the flags choose, with the first halfword n written by the
    setup; everything after it (flag halfwords, literals, references) is
    random, so a reference may point back up to 32 KiB into random data;
  - n: 0 in 12 cases of 100; a small count (high byte 0) in 46 of 100, of
    which 15 of 100 are below 16 and 20 of 100 a multiple of 16; the form
    0x80xx in 20 of 100; any other value (whose high byte is neither 0 nor
    0x80) in 22 of 100, with a low byte below 41 in four cases of five;
  - the destination block is 68 KiB of random bytes and the destination
    starts at an even offset of 256 to 382 bytes into it, so that the mask
    and the largest output (65,280 bytes of the callees) stay inside.
"""

from contracts import Contract, Setup


def setup(state, rng, sym) -> Setup:
    flags = (rng.getrandbits(16) & 0xC000) | rng.randrange(8)
    if rng.random() < 0.25:
        flags |= rng.getrandbits(16) << 16

    base_size = 0x9000
    base = state.alloc(base_size)
    state.write(base, rng.randbytes(base_size))
    for k in range(8):
        state.w32(base + 4 * k, 0x8000 + 2 * rng.randrange(0x400))
    start = base + int.from_bytes(state.read(base + 4 * (flags & 0x3FFF), 4), "little")

    kind = rng.random()
    if kind < 0.12:
        n = 0
    elif kind < 0.58:
        sub = rng.random()
        if sub < 0.15 / 0.46:
            n8 = rng.randrange(1, 16)
        elif sub < 0.35 / 0.46:
            n8 = 16 * rng.randrange(1, 16)
        else:
            n8 = rng.randrange(1, 256)
        n = n8
    elif kind < 0.78:
        n = 0x8000 | rng.randrange(256)
    else:
        high = rng.choice([h for h in range(1, 256) if h != 0x80])
        low = rng.randrange(41) if rng.random() < 0.8 else rng.randrange(256)
        n = high << 8 | low
    state.w16(start, n)

    dst_size = 0x11000
    block = state.alloc(dst_size)
    state.write(block, rng.randbytes(dst_size))
    dst = block + 256 + 2 * rng.randrange(64)
    return Setup(args=(base, flags, dst), returns_value=False)


def control(words):
    """Turn the first exclusive-or of the build (the destination address with
    the mask) into an or. Each cell that is stored passes through one."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0 and w & 0x7FF == 0x26]
    if not found:
        raise ValueError("no xor in the build")
    index = found[0]
    return index, (words[index] & ~0x3F) | 0x25, "address mask combined with or instead of xor"


CONTRACT = Contract(setup, control)
