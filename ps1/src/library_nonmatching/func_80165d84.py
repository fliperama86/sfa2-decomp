"""Contract of func_80165d84 (prepares one sequence of a group; its byte count, or -1, in v0).

Reads and writes are listed in the header comment of func_80165d84.c.
Choices of the setup:
  - slot is 0 to 31 and index is 0 to 3, each with a random upper half (the
    original uses only the lower halves); vab_id has a random upper half too;
    the row _ss_score[slot] is a block of four 172-byte records of random
    bytes;
  - addr is a block of 64 random bytes. With index 0 the first byte is 'S' or
    'p' in two cases of three and random otherwise; in the 'S'/'p' cases the
    byte at 5 is 0 in four cases of five (the other cases return -1);
  - at the position the function reads from (addr, addr + 2 or addr + 8): the
    resolution is 1 to 100 in one case of two, 1 to 0x7fff in one of four,
    0x8000 to 0xffff in one of four (never 0); the tempo is 1 (half) or 1 to 100 in one
    case of eight, a divisor of 60000000 below 2^24 in one of eight (the edge
    of the constant), 100000 to 2000000 in two of eight, random 1 to 0xffffff
    otherwise (never 0), so that both rounding arms of the division and both
    arms of the final computation occur; the four length bytes are random;
  - VBLANK_MINUS is 50 or 60 in two cases of five, 1 to 200 in two of five,
    random otherwise; it is changed when V * 60 is a multiple of 2^32, and the
    resolution is changed to 1 when the product with the converted tempo
    would be 0 modulo 2^32 (the original traps on a division by zero);
  - printf is a recorder with one argument whose string is logged as six
    words behind its pointer; _SsReadDeltaValue is a recorder with two
    arguments returning a random word, and CallLog watches the whole score
    (43 words) at every call.
"""

from contracts import CallLog, Contract, Setup

RECORD = 172
# The divisors of 60000000 that fit the three tempo bytes: the edge of the constant (a numerator one
# lower or one higher changes the quotient or the remainder only here).
DIVISORS = sorted({d for a in range(1, 7747) if 60000000 % a == 0 for d in (a, 60000000 // a) if d < 0x1000000})


def setup(state, rng, sym):
    slot = rng.randrange(32)
    index = rng.randrange(4)
    row = state.alloc(RECORD * 4)
    for offset in range(0, RECORD * 4, 4):
        state.w32(row + offset, rng.getrandbits(32))
    state.w32(sym["_ss_score"] + 4 * slot, row)
    score = row + RECORD * index

    addr = state.alloc(64)
    buf = bytearray(rng.getrandbits(8) for _ in range(64))
    pos = 2
    if index == 0:
        pos = 0
        if rng.random() < 2 / 3:
            buf[0] = rng.choice((0x53, 0x70))
            if rng.random() < 0.8:
                buf[5] = 0
            elif buf[5] == 0:
                buf[5] = 1
            pos = 8
        elif buf[0] in (0x53, 0x70):
            buf[0] = 0x54
    kind = rng.randrange(4)
    if kind < 2:
        res = rng.randrange(1, 101)
    elif kind == 2:
        res = rng.randrange(1, 0x8000)
    else:
        res = rng.randrange(0x8000, 0x10000)
    kind = rng.randrange(8)
    if kind == 0:
        tempo = rng.choice((1, 1, rng.randrange(1, 101)))
    elif kind == 7:
        tempo = rng.choice(DIVISORS)
    elif kind < 3:
        tempo = rng.randrange(100000, 2000001)
    else:
        tempo = rng.randrange(1, 0x1000000)
    kind = rng.randrange(5)
    if kind < 2:
        vblank = rng.choice((50, 60))
    elif kind < 4:
        vblank = rng.randrange(1, 201)
    else:
        vblank = rng.getrandbits(32)
    if (vblank * 60) & 0xFFFFFFFF == 0:
        vblank = 60
    quot, rem = divmod(60000000, tempo)
    rate = quot + 1 if tempo >> 1 < rem else quot
    if (((res - 0x10000 if res >= 0x8000 else res) * rate) & 0xFFFFFFFF) == 0:
        res = 1
    buf[pos:pos + 5] = bytes((res >> 8, res & 0xFF, tempo >> 16, tempo >> 8 & 0xFF, tempo & 0xFF))
    for offset in range(64):
        state.w8(addr + offset, buf[offset])
    state.w32(sym["VBLANK_MINUS"], vblank)

    log = CallLog(state, words=120, watch=((score, RECORD // 4),))
    log.replace(sym["printf"], 1, 0, masks={0: 0}, pointees={0: 6})
    log.replace(sym["_SsReadDeltaValue"], 2, rng.getrandbits(32), masks={0: 0xFFFF, 1: 0xFFFF})
    args = (rng.getrandbits(16) << 16 | slot, rng.getrandbits(16) << 16 | index, rng.getrandbits(32), addr)
    return Setup(args=args, returns_value=True)


def control(words):
    """Alter the pan value stored for each channel: 0x40 becomes 0x41.

    It is the one `ori rt, zero, 0x40` of the function; every case that gets
    past the header test reaches it.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0xD and (w >> 21) & 31 == 0 and w & 0xFFFF == 0x40]
    if len(found) != 1:
        raise ValueError(f"expected one ori of 0x40, found {len(found)}")
    index = found[0]
    return index, words[index] + 1, "pan value 0x40 becomes 0x41"


CONTRACT = Contract(setup, control)
