"""Contract of func_800110d0_z2o: a0 = x, a1 = y, the result in v0 is 1 when the previous frame number is not below the word at 0x8002f814 (unsigned), else 0 (or 1 at the time-out); one frame of the movie.

Choices of the setup (reads and writes are listed in func_800110d0_z2o.c):
  - x and y are random words (the low 16 bits are stored);
  - the words from 0x8002f800 to 0x8002f8cf (all the data the function
    writes, reads or passes on) start as random bytes, so that a store that
    is missing or of the wrong width shows and no byte of the program's data
    is read; the data in the 0x8001b3xx area is not read by this function;
  - the previous frame number (word 2 of the header copy at 0x8002f84c) is,
    in most cases, at an edge of the comparison with the word at 0x8002f814:
    that word minus 1, that word, that word plus 1, 0, 1, or a number near
    0xffffffff (the comparison is unsigned), else random; the word at
    0x8002f814 is a small number, a number near the unsigned limit, or
    random;
  - the two halfwords of the copy at +0x10 and +0x12 (the width and the
    height) are chosen at the edges of the rounding and of the shifts (0, 7,
    8, 9, 15, 16, 17, 23, 24, 0x7fff, 0x8000, 0xfff7, 0xfff8, 0xffff) or
    random, and so are those of the new header block (32 bytes of the
    setup's, with random other bytes);
  - the words at 0x8002f818 and 0x8002f870 are 0 in half the cases and else a
    random non-zero word; the index word at 0x8002f820 is 0, 1, 2 or a random
    number from 0 to 7 (the buffer pointer table is read at that index, and
    the words from 0x8002f804 to 0x8002f823 are made);
  - StGetNext returns a value that is not 0 up to eight times and then 0, its
    fills store the pointers through the two locals (garbage before the last
    call, at the last a random data pointer and the pointer to the header
    block);
  - in half of the cases the recorder of DecDCTReset stores a random value in
    the word at 0x8002f818 (0 in a quarter of them) and in the word at
    0x8002f820 (0 to 7), and in half of the cases the recorder of DecDCTin
    stores a random word in the words at 0x8002f80c and 0x8002f884 and a
    number from 0 to 7 in the word at 0x8002f820, so that the re-reads of the
    function show (an interrupt could change them);
  - the callees are recorders of the arguments they take: StGetNext (2, both
    masked to 0), DecDCTReset (1), DecDCTin (2), DecDCTout (2), DecDCTvlc (2),
    StFreeRing (1); the others return 0 (a result nobody reads);
  - the words from 0x8002f804 to 0x8002f823, the 32 bytes at 0x8002f84c and
    the word at 0x8002f86c are watched, so that the log shows what the
    function has stored by each call. The time-out of 0x100000 returns is not
    tested: see the limit in the .c.
"""
import struct

from contracts import CallLog, Contract, Setup, fill

HALF_EDGES = [0, 7, 8, 9, 15, 16, 17, 23, 24, 0x7FFF, 0x8000, 0xFFF7, 0xFFF8, 0xFFFF]


def setup(state, rng, sym):
    data = sym["data_8002f804_z2o"]
    total = sym["data_8002f814_z2o"]
    flag = sym["data_8002f818_z2o"]
    index = sym["data_8002f820_z2o"]
    copy = sym["data_8002f84c_z2o"]
    count_word = sym["data_8002f86c_z2o"]
    param = sym["data_8002f870_z2o"]
    strip = sym["data_8002f80c_z2o"]
    size_word = sym["data_8002f884_z2o"]
    fill(state, data - 4, 0xD0, rng)

    def nz():
        return 1 if rng.random() < 0.5 else rng.randrange(1, 1 << 32)

    def half():
        return rng.choice(HALF_EDGES) if rng.random() < 0.7 else rng.getrandbits(16)

    kind = rng.random()
    if kind < 0.4:
        frames = rng.randrange(0, 0x200)
    elif kind < 0.6:
        frames = rng.choice((0xFFFFFFFF, 0xFFFFFFFE, 0x80000000, 0x7FFFFFFF, 0))
    else:
        frames = rng.getrandbits(32)
    state.w32(total, frames)
    kind = rng.random()
    if kind < 0.6:
        prev = (frames + rng.choice((-1, 0, 1))) & 0xFFFFFFFF
    elif kind < 0.8:
        prev = rng.choice((0, 1, 0xFFFFFFFF, 0x80000000, 0x7FFFFFFF))
    else:
        prev = rng.getrandbits(32)
    state.w32(copy + 8, prev)
    state.w16(copy + 0x10, half())
    state.w16(copy + 0x12, half())
    state.w32(flag, 0 if rng.random() < 0.5 else nz())
    state.w32(param, 0 if rng.random() < 0.5 else nz())
    state.w32(index, rng.choice((0, 1, 2, rng.randrange(8))))

    next_results = tuple(nz() for _ in range(rng.randrange(9))) + (0,)
    last = len(next_results)
    header = state.alloc(32)
    fill(state, header, 32, rng)
    state.w16(header + 0x10, half())
    state.w16(header + 0x12, half())
    fills = ((0, 0, 0, struct.pack("<I", rng.getrandbits(32))), (0, 1, 0, struct.pack("<I", rng.getrandbits(32))),
             (last, 0, 0, struct.pack("<I", rng.getrandbits(32))), (last, 1, 0, struct.pack("<I", header)))
    reset_stores = ()
    if rng.random() < 0.5:
        reset_stores = ((1, flag, 0 if rng.random() < 0.25 else rng.getrandbits(32)), (1, index, rng.randrange(8)))
    in_stores = ()
    if rng.random() < 0.5:
        in_stores = ((1, strip, rng.getrandbits(32)), (1, size_word, rng.getrandbits(32)), (1, index, rng.randrange(8)))

    log = CallLog(state, words=1024, watch=((data, 9), (copy, 8), (count_word, 1)))
    log.replace(sym["StGetNext"], 2, results=next_results, masks={0: 0, 1: 0}, fills=fills)
    log.replace(sym["DecDCTReset"], 1, stores=reset_stores)
    log.replace(sym["DecDCTin"], 2, stores=in_stores)
    log.replace(sym["DecDCTout"], 2)
    log.replace(sym["DecDCTvlc"], 2)
    log.replace(sym["StFreeRing"], 1)
    return Setup(args=(rng.getrandbits(32), rng.getrandbits(32)), returns_value=True)


def control(words):
    """Alter the width of the rectangle: the 0x10 loaded into t0 (`ori t0,zero,0x10`) becomes 0x11."""
    found = [i for i, w in enumerate(words) if w >> 26 == 13 and (w >> 21) & 31 == 0 and (w >> 16) & 31 == 8 and w & 0xFFFF == 0x10]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x10 into t0, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x11, "the rectangle's width is 0x11 instead of 0x10"


CONTRACT = Contract(setup, control)
