"""Contract of func_8015ffc0 (the voice search; a0 unused; the voice number in v0).

Reads and writes are listed in the header comment of func_8015ffc0.c.
Choices of the setup:
  - spuVmMaxVoice is 0 to 24 (24 records of 0x30 bytes in _svm_voice);
  - the whole table is random bytes first, then the fields the function
    reads are set from small ranges so that the compares tie often: the
    byte at 0x17 is 0 in three cases of eight, 1 or 2 in two of eight each, or random; the halfword at 6 is 0 to 3,
    0xffff or random; the halfword at 0x14 is within 2 of the starting
    priority when that is below 0x80 (otherwise within 2 of a random value 0
    to 8), or random; values below 0 and above 0x8000 occur; the halfword at
    2 is 0 to 3, 0x7ff0 to 0x7fff, 0x8000 to 0x8003, or random;
  - the byte _svm_cur.field_F_prior is 0 to 8 (in half of the cases), 0xff or random;
  - arg0 is random;
  - SpuSetNoiseVoice is a recorder with two arguments returning 0, and
    CallLog watches the whole voice table (288 words) at the call.
"""

from contracts import CallLog, Contract, Setup

RECORDS = 24
RECORD = 0x30


def setup(state, rng, sym):
    table = sym["_svm_voice"]
    for offset in range(0, RECORDS * RECORD, 4):
        state.w32(table + offset, rng.getrandbits(32))
    count = rng.randrange(0, 25)
    prior = rng.choice((rng.randrange(0, 9), rng.randrange(0, 9), 0xFF, rng.getrandbits(8)))
    state.w8(sym["spuVmMaxVoice"], count)
    state.w8(sym["_svm_cur"] + 0xF, prior)
    base = prior if prior < 0x80 else rng.randrange(0, 9)
    for index in range(RECORDS):
        at = table + index * RECORD
        state.w8(at + 0x17, rng.choice((0, 0, 0, 1, 1, 2, 2, rng.getrandbits(8))))
        state.w16(at + 6, rng.choice((0, 1, 2, 3, 0xFFFF, rng.getrandbits(16))))
        state.w16(at + 0x14, rng.choice((base - 2, base - 1, base, base, base, base + 1, base + 2,
                                         rng.getrandbits(16))) & 0xFFFF)
        state.w16(at + 2, rng.choice((0, 1, 2, 3, 0x7FF0 + rng.randrange(0x10), 0x8000 + rng.randrange(4),
                                      rng.getrandbits(16))))
    log = CallLog(state, words=400, watch=((table, RECORDS * RECORD // 4),))
    log.replace(sym["SpuSetNoiseVoice"], 2, 0)
    return Setup(args=(rng.getrandbits(32),), returns_value=True)


def control(words):
    """Alter the argument of the noise call: 0xffffff becomes 0xfeffff.

    It is the one `lui a1, 0xff` of the function; every case that makes the
    call reaches it.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0xF and w & 0xFFFF == 0xFF]
    if len(found) != 1:
        raise ValueError(f"expected one lui with 0xff, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0xFE, "noise call argument 0xfeffff"


CONTRACT = Contract(setup, control)
