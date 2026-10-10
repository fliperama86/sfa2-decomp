"""Contract of func_80163794 (volume of the voices of one sequence/program; returns a count).

Reads and writes are listed in the header comment of func_80163794.c.
Choices of the setup:
  - spuVmMaxVoice is 0 to 24; _svm_voice (24 records of 0x30 bytes), the
    two sreg tables (24 * 16 bytes and 24 bytes) and the first 0x20 bytes of
    _svm_cur start as random bytes;
  - seq_sep_no has a low byte of 0 to 31 and a high byte of 0 to 3 (one case
    in ten: 0 to 255, so that the 16-bit value may be negative); vabId is 0 to 15
    or any 16-bit value; prog is 0 to 127 or any 16-bit value; arguments are
    passed sign-extended (a0..a2) and zero-extended (a3, the fifth argument);
  - a voice matches (all three keys equal to the arguments) in 55 cases of
    100; otherwise one of the three keys, chosen at random, is set to a
    random 16-bit value; the halfword at 0x10 is random;
  - the bytes of _svm_cur at 0xa, 0xb, 0xd, 0xe are drawn from 0, 0x3f, 0x40,
    0x7f, 0x80, 0xff and random; the byte at 0x18 of the header that
    _svm_vh points at is drawn from the same list; arg3 is 0 to 0x7f or any
    16-bit value; arg4 is 0 to 0xff or any 16-bit value;
  - the score element has random bytes, and its halfwords at 0x74 and 0x76
    are drawn from 0, 0x7f, 0x80, 0x7fff, 0xffff and random;
  - _svm_stereo_mono is 1 in one case of two, otherwise 0, 2 or random;
  - SpuVmVSetUp is a recorder with two arguments (masked to 16 bits) that
    returns a random value (this function does not use it); CallLog watches
    the first 0x20 bytes of _svm_cur.
"""

from contracts import CallLog, Contract, Setup

RECORDS = 24
RECORD = 0x30
STACK_ARG = 0x801FF010  # the fifth argument: the initial sp is 0x801FF000, the fifth word is at sp + 16
EDGES = (0, 0x3F, 0x40, 0x7F, 0x80, 0xFF)


def byte(rng):
    return rng.choice(EDGES + (rng.getrandbits(8),))


def signed(value):
    return value - 0x10000 if value & 0x8000 else value


def setup(state, rng, sym):
    table = sym["_svm_voice"]
    for offset in range(0, RECORDS * RECORD, 4):
        state.w32(table + offset, rng.getrandbits(32))
    for offset in range(0, RECORDS * 16, 4):
        state.w32(sym["_svm_sreg_buf"] + offset, rng.getrandbits(32))
    for offset in range(0, RECORDS, 4):
        state.w32(sym["_svm_sreg_dirty"] + offset, rng.getrandbits(32))
    for offset in range(0, 0x20, 4):
        state.w32(sym["_svm_cur"] + offset, rng.getrandbits(32))
    state.w8(sym["spuVmMaxVoice"], rng.randrange(0, 25))

    low = rng.randrange(32)
    high = rng.randrange(256) if rng.random() < 0.1 else rng.randrange(4)
    seq = low | high << 8
    vab = rng.randrange(16) if rng.random() < 0.7 else rng.getrandbits(16)
    prog = rng.randrange(128) if rng.random() < 0.7 else rng.getrandbits(16)
    for index in range(RECORDS):
        at = table + index * RECORD
        keys = [seq, prog, vab]
        if rng.random() >= 0.55:
            keys[rng.randrange(3)] = rng.getrandbits(16)
        state.w16(at + 0xA, keys[0])
        state.w16(at + 0xE, keys[1])
        state.w16(at + 0x12, keys[2])
        state.w16(at + 0x10, rng.getrandbits(16))

    cur = sym["_svm_cur"]
    for offset in (0xA, 0xB, 0xD, 0xE):
        state.w8(cur + offset, byte(rng))
    header = state.alloc(0x20)
    for offset in range(0, 0x20, 4):
        state.w32(header + offset, rng.getrandbits(32))
    state.w8(header + 0x18, byte(rng))
    state.w32(sym["_svm_vh"], header)

    row = state.alloc(172 * (high + 1))
    for offset in range(0, 172 * (high + 1), 4):
        state.w32(row + offset, rng.getrandbits(32))
    element = row + 172 * high
    for offset in (0x74, 0x76):
        state.w16(element + offset, rng.choice((0, 0x7F, 0x80, 0x7FFF, 0xFFFF, rng.getrandbits(16))))
    state.w32(sym["_ss_score"] + 4 * low, row)

    if rng.random() < 0.5:
        state.w16(sym["_svm_stereo_mono"], 1)
    else:
        state.w16(sym["_svm_stereo_mono"], rng.choice((0, 2, rng.getrandbits(16))))

    arg3 = rng.randrange(0x80) if rng.random() < 0.5 else rng.getrandbits(16)
    arg4 = rng.randrange(0x100) if rng.random() < 0.7 else rng.getrandbits(16)
    state.w32(STACK_ARG, arg4)

    log = CallLog(state, words=64, watch=((cur, 8),))
    log.replace(sym["SpuVmVSetUp"], 2, rng.getrandbits(32), masks={0: 0xFFFF, 1: 0xFFFF})
    return Setup(args=(signed(seq) & 0xFFFFFFFF, signed(vab) & 0xFFFFFFFF, signed(prog) & 0xFFFFFFFF, arg3),
                 returns_value=True)


def control(words):
    """Alter the constant of a pan test: `sltiu rt, rs, 0x40` becomes 0x41.

    The function has three such tests (the three pans); the first one in the
    build's code is altered. Every matching voice reaches all three.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0xB and w & 0xFFFF == 0x40]
    if not found:
        raise ValueError("expected a sltiu with 0x40, found none")
    index = found[0]
    return index, words[index] + 1, f"first of {len(found)} pan tests compares with 0x41"


CONTRACT = Contract(setup, control)
