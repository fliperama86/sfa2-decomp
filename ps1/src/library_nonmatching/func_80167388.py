"""Contract of func_80167388 (a data entry value of a sequence; no result).

Reads and writes are listed in the header comment of func_80167388.c.
Choices of the setup:
  - arg0 is 0 to 31 and arg1 is 0 to 3; the row _ss_score[arg0] is a block of
    four 172-byte records of random bytes and the record is the arg1-th;
  - in the record: the channel byte is 0 to 15; the byte at 0x27 is 1 in two
    cases of five (otherwise 0, 2 or random) and the byte at 0x10 is 0 in one
    case of two (otherwise 1 or random); the byte at 0x16 is 0x14, 0x1e or
    0x10 in four cases of six and random in two; the bytes at 0x29 and 0x2a
    are 2 in one case of two (otherwise 0, 1, 3 or random); the byte at 0x13
    is 0, 1, 2 or random; the byte at 0x14 is 0 in seven cases of ten;
  - arg2 is 0 to 255;
  - the tone count is 0 to 6; the first word of the program header is that
    count and three random bytes;
  - the 32 bytes that the tone-record recorder hands out are random;
  - all five callees are recorders (arguments 3, 4, 4, 2, 18); the program
    header and tone-record recorders have a fill each (data of the contract,
    stored through the pointer argument at every call; models of the
    contract's author); the log watches the whole record (43 words) and has
    room for 2,500 words;
  - SsUtSetVagAtr logs the 8 words behind its tone-record pointer, and the
    pointer itself is not logged (mask 0); _SsSndSetVabAttr logs the three
    s16 arguments, idx and attr and no word of the two records passed by
    value (masks 0 on those words), _SsReadDeltaValue returns a random word.
"""

import struct

from contracts import CallLog, Contract, Setup

RECORD = 172


def setup(state, rng, sym):
    arg0 = rng.randrange(32)
    arg1 = rng.randrange(4)
    row = state.alloc(RECORD * 4)
    for offset in range(0, RECORD * 4, 4):
        state.w32(row + offset, rng.getrandbits(32))
    state.w32(sym["_ss_score"] + 4 * arg0, row)
    score = row + RECORD * arg1

    def pick(*choices):
        return rng.choice(choices + (rng.getrandbits(8),))

    state.w8(score + 0x12, rng.randrange(16))
    state.w8(score + 0x27, 1 if rng.random() < 0.4 else pick(0, 2))
    state.w8(score + 0x10, 0 if rng.random() < 0.5 else pick(1))
    state.w8(score + 0x16, rng.choice((0x14, 0x1E, 0x10, 0x10, rng.getrandbits(8), rng.getrandbits(8))))
    state.w8(score + 0x29, 2 if rng.random() < 0.5 else pick(0, 1, 3))
    state.w8(score + 0x2A, 2 if rng.random() < 0.5 else pick(0, 1, 3))
    state.w8(score + 0x13, pick(0, 1, 2))
    state.w8(score + 0x14, 0 if rng.random() < 0.7 else rng.getrandbits(8))
    attr = rng.getrandbits(8)

    tones = rng.randrange(7)
    word = tones | rng.getrandbits(24) << 8
    vag = struct.pack("<8I", *(rng.getrandbits(32) for _ in range(8)))

    log = CallLog(state, words=2500, watch=((score, RECORD // 4),))
    log.replace(sym["SsUtGetProgAtr"], 3, 0, masks={0: 0xFFFF, 1: 0xFFFF, 2: 0},
                fills=((0, 2, 0, struct.pack("<I", word)),))
    log.replace(sym["SsUtGetVagAtr"], 4, 0, masks={0: 0xFFFF, 1: 0xFFFF, 2: 0xFFFF, 3: 0},
                fills=((0, 3, 0, vag),))
    log.replace(sym["SsUtSetVagAtr"], 4, 0, masks={0: 0xFFFF, 1: 0xFFFF, 2: 0xFFFF, 3: 0}, pointees={3: 8})
    log.replace(sym["_SsReadDeltaValue"], 2, rng.getrandbits(32), masks={0: 0xFFFF, 1: 0xFFFF})
    masks = {index: 0 for index in range(3, 16)}
    masks.update({0: 0xFFFF, 1: 0xFFFF, 2: 0xFFFF, 16: 0xFFFF, 17: 0xFF})
    log.replace(sym["_SsSndSetVabAttr"], 18, 0, masks=masks)
    return Setup(args=(arg0, arg1, attr), returns_value=False)


def control(words):
    """Alter the constant 0x1e of the test of the byte at 0x16: it becomes 0x1f.

    It is the one `ori rt, zero, 0x1e` of the function; every case that gets
    past the first test reaches it.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0xD and (w >> 21) & 31 == 0 and w & 0xFFFF == 0x1E]
    if len(found) != 1:
        raise ValueError(f"expected one ori of 0x1e, found {len(found)}")
    index = found[0]
    return index, words[index] + 1, "constant 0x1e becomes 0x1f"


CONTRACT = Contract(setup, control)
