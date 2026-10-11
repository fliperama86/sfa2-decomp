"""Contract of func_80010c20_logo: no arguments, the result in v0 is 1 when the frame number before the new one is at least 0x118 (else 0); one frame of the movie.

Choices of the setup (reads and writes are listed in func_80010c20_logo.c):
  - the frame number before (the word at 0x8002eb30) and the new frame's
    number (word 2 of a header block the setup makes) are chosen on and
    around each limit of the function: 0xac, 0xad, 0xae to 0xb0, 0xb1,
    0xb2, 0xf3, 0xf4, 0xf5, 0xfe, 0xff, 0x100, 0x117, 0x118, 0x119, 0 and 1,
    a number near 0xffffffff (the comparisons are unsigned), a random
    number from 0 to 0x200, a random word;
  - the index word at 0x8002eb20 is 0, 1, 2 or a random small number
    (0 to 7), the word at 0x8002eb50 and the word at 0x8002eb38 are 0 in
    half the cases and else a random non-zero word;
  - the two tables of pointers (16 words each) are blocks the setup makes and
    fills with random words, and the words at 0x8002eb48 and 0x8002eb4c point
    to them; the table of two buffer pointers at 0x8002eb08, the word at
    0x8002eb00, the rectangle at 0x8002eb24 and the words about them start
    as random bytes, so that a missing store or one of the wrong width shows;
  - StGetNext returns a value that is not 0 up to eight times and then 0.
    The time-out of 0x100000 returns is not tested: see the limit in the .c;
  - the recorder of the flip stores nothing; the others return 0 (a result
    nobody reads);
  - the callees are recorders of the arguments they take: StGetNext (2, both
    masked to 0, with fills of the setup's pointers), DecDCTReset (1),
    func_800110f4_logo (2), func_800111d0_logo (2), DecDCTin (2), DecDCTout
    (2), DecDCTvlc (2), StFreeRing (1), func_80010f20_logo (0), VSync (1),
    func_80010a38_logo (0);
  - the index word, the rectangle (two words) and the two frame words are
    watched, so that the log shows what the function has stored by each call.
"""
import struct

from contracts import CallLog, Contract, Setup, fill

EDGES = [0xAC, 0xAD, 0xAE, 0xB0, 0xB1, 0xB2, 0xF3, 0xF4, 0xF5, 0xFE, 0xFF, 0x100, 0x117, 0x118, 0x119, 0, 1,
         0xFFFFFFFF, 0xFFFFFF53, 0xFFFFFF54, 0x7FFFFFFF, 0x80000000]


def frame_number(rng):
    kind = rng.random()
    if kind < 0.7:
        return rng.choice(EDGES)
    if kind < 0.9:
        return rng.randrange(0, 0x200)
    return rng.getrandbits(32)


def setup(state, rng, sym):
    strip = sym["data_8002eb00_logo"]
    index = sym["data_8002eb20_logo"]
    rect = sym["data_8002eb24_logo"]
    frames = sym["data_8002eb2c_logo"]
    vmode = sym["data_8002eb38_logo"]
    table1 = sym["data_8002eb48_logo"]
    row = sym["data_8002eb50_logo"]
    fill(state, strip, 0x60, rng)
    first = state.alloc(64)
    second = state.alloc(64)
    fill(state, first, 64, rng)
    fill(state, second, 64, rng)
    state.w32(table1, first)
    state.w32(table1 + 4, second)
    state.w32(index, rng.choice((0, 1, 2, rng.randrange(8))))
    state.w32(row, 0 if rng.random() < 0.5 else rng.randrange(1, 1 << 32))
    state.w32(vmode, 0 if rng.random() < 0.5 else rng.randrange(1, 1 << 32))
    state.w32(frames + 4, frame_number(rng))
    header = state.alloc(16)
    fill(state, header, 16, rng)
    state.w32(header + 8, frame_number(rng))

    def nz():
        return 1 if rng.random() < 0.5 else rng.randrange(1, 1 << 32)

    results = tuple(nz() for _ in range(rng.randrange(9))) + (0,)
    last = len(results)
    fills = ((0, 0, 0, struct.pack("<I", rng.getrandbits(32))), (0, 1, 0, struct.pack("<I", rng.getrandbits(32))),
             (last, 0, 0, struct.pack("<I", rng.getrandbits(32))), (last, 1, 0, struct.pack("<I", header)))
    log = CallLog(state, words=512, watch=((index, 1), (rect, 2), (frames, 2)))
    log.replace(sym["StGetNext"], 2, results=results, masks={0: 0, 1: 0}, fills=fills)
    log.replace(sym["DecDCTReset"], 1)
    log.replace(sym["func_800110f4_logo"], 2)
    log.replace(sym["func_800111d0_logo"], 2)
    log.replace(sym["DecDCTin"], 2)
    log.replace(sym["DecDCTout"], 2)
    log.replace(sym["DecDCTvlc"], 2)
    log.replace(sym["StFreeRing"], 1)
    log.replace(sym["func_80010f20_logo"], 0)
    log.replace(sym["VSync"], 1)
    log.replace(sym["func_80010a38_logo"], 0)
    return Setup(args=(), returns_value=True)


def control(words):
    """Alter the limit of the return value: the `sltiu rt,rs,0x118` becomes `sltiu rt,rs,0x119`."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0B and w & 0xFFFF == 0x118]
    if len(found) != 1:
        raise ValueError(f"expected one sltiu with 0x118, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x119, "the last frame is 0x119 instead of 0x118"


CONTRACT = Contract(setup, control)
