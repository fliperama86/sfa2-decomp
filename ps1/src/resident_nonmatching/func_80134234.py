"""Contract of func_80134234, as code. Header comment of the .c has the text.

Choices of the setup:
  - each of the four gates (data_8019016c, data_801a6938, first byte of
    data_801ac6a8, data_8018db10) is set to a value that lets the function go
    on in nine cases of ten (0 for the first and the last, non-zero for the
    other two) and to a random byte in the other tenth (which may also let
    it go on), so that each early return is tried;
  - data_801987c8 is the address of a block of the setup (it is only added
    to); data_801a27d0 is a random word, a small number half of the time;
  - the words data_801ac86c to data_801ac884 are random;
  - the counters data_8019808e and data_80198422 each take one of six
    choices with equal probability: 0, 1, 2, a value from 1 to 7, a value
    from 0 to 255, or 255; the flags data_80198098 and data_8019842c
    are zero or random;
  - all five callees are recorders returning 0 (func_8015bf34 takes 2
    arguments, func_801347b4 six, the other three none); the log has room
    for 8192 words; one run makes at most 255 + 255 + 26 calls (26 are the
    fixed calls, counting both optional pairs); every recorder copies
    the word that holds data_80188d04 (the only thing the function writes).
    None of the recorded callees is passed memory the function fills.
"""

from contracts import CallLog, Contract, Setup


def _count(rng):
    return rng.choice((0, 1, 2, rng.randrange(1, 8), rng.randrange(256), 255))


def setup(state, rng, sym):
    log = CallLog(state, 8192, watch=((sym["data_80188d04"], 1),))
    log.replace(sym["func_8015bf34"], 2, 0)
    log.replace(sym["func_801347b4"], 6, 0)
    log.replace(sym["func_80153154"], 0, 0)
    log.replace(sym["func_80153f88"], 0, 0)
    log.replace(sym["func_80135c88"], 0, 0)

    def gate(zero):
        if rng.random() < 0.9:
            return 0 if zero else rng.randrange(1, 256)
        return rng.getrandbits(8)

    state.w8(sym["data_8019016c"], gate(True))
    state.w8(sym["data_801a6938"], gate(False))
    state.w8(sym["data_801ac6a8"], gate(False))
    state.w8(sym["data_8018db10"], gate(True))
    state.w32(sym["data_801987c8"], state.alloc(0x40))
    state.w32(sym["data_801a27d0"],
              rng.randrange(-4, 5) & 0xFFFFFFFF if rng.random() < 0.5 else rng.getrandbits(32))
    state.w16(sym["data_80188d04"], rng.getrandbits(16))
    for name in ("data_801ac86c", "data_801ac870", "data_801ac874", "data_801ac878",
                 "data_801ac87c", "data_801ac880", "data_801ac884"):
        state.w32(sym[name], rng.getrandbits(32))
    state.w8(sym["data_8019808e"], _count(rng))
    state.w8(sym["data_80198422"], _count(rng))
    state.w8(sym["data_80198098"], rng.choice((0, rng.getrandbits(8))))
    state.w8(sym["data_8019842c"], rng.choice((0, rng.getrandbits(8))))
    return Setup(args=(), returns_value=False)


def control(words):
    """Change the constant 0x124 of the second record of the first series."""
    found = [i for i, w in enumerate(words) if (w >> 26) == 0x09 and w & 0xFFFF == 0x124]
    if len(found) != 1:
        raise ValueError(f"expected one addiu of 0x124, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x128, "record offset 0x124 changed to 0x128"


CONTRACT = Contract(setup, control)
