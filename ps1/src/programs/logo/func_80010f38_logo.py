"""Contract of func_80010f38_logo: no arguments, no result; the decoder's output routine.

Choices of the setup (reads and writes are listed in func_80010f38_logo.c):
  - the word at 0x8002edb8 is 0 in half the cases, else a random non-zero
    word (StCdInterrupt is then called and the word cleared);
  - the buffer pointer at 0x8002eb00 is the address of a block the setup
    allocated in three cases of four, else a random word; the callees are
    recorders, so the buffer is never dereferenced and only the pointer is
    logged;
  - the rectangle at 0x8002eb24 has random y, w, h, and an x that is, in
    half the cases, one of the values around the limit (x + 0x18 is 0x1df,
    0x1e0 or 0x1e1) or around the signed halfword's wrap (x + 0x18 crosses
    0x8000), in the other half a random halfword;
  - the word at 0x8002eb3c holds a random value before the call, so that its
    store (1 or 0) shows;
  - the bytes around each of these words are random, so that a neighbouring
    read or write would show;
  - the callees are recorders: StCdInterrupt (0 arguments), func_80157fc4 (2
    arguments, the rectangle's two words behind the first), DecDCTout (2
    arguments); the flag, the rectangle and the word at 0x8002eb3c are watched.
"""
from contracts import CallLog, Contract, Setup, fill

EDGES = (0x1DF - 0x18, 0x1E0 - 0x18, 0x1E1 - 0x18, 0x7FFF - 0x18, 0x7FFF - 0x17, 0x8000 - 0x18, 0xFFFF, 0, 0x1E0, 0x8000)


def setup(state, rng, sym):
    flag = sym["data_8002edb8_logo"]
    buf_ptr = sym["data_8002eb00_logo"]
    rect = sym["data_8002eb24_logo"]
    busy = sym["data_8002eb3c_logo"]
    for address, size in ((flag - 8, 20), (buf_ptr - 8, 20), (rect - 8, 24), (busy - 8, 20)):
        fill(state, address, size, rng)
    state.w32(flag, 0 if rng.random() < 0.5 else rng.randrange(1, 1 << 32))
    state.w32(buf_ptr, state.alloc(16) if rng.random() < 0.75 else rng.getrandbits(32))
    x = rng.choice(EDGES) if rng.random() < 0.5 else rng.getrandbits(16)
    state.w16(rect, x & 0xFFFF)
    state.w32(busy, rng.getrandbits(32))
    log = CallLog(state, words=64, watch=((flag, 1), (rect, 2), (busy, 1)))
    log.replace(sym["StCdInterrupt"], 0)
    log.replace(sym["func_80157fc4"], 2, pointees={0: 2})
    log.replace(sym["DecDCTout"], 2)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the step of the rectangle: `addiu rt,rs,0x18` (not on sp) becomes 0x19."""
    found = [i for i, w in enumerate(words) if w >> 26 == 9 and w & 0xFFFF == 0x18 and (w >> 21) & 31 != 29]
    if len(found) != 1:
        raise ValueError(f"expected one add of 0x18, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x19, "the rectangle moves by 25 pixels instead of 24"


CONTRACT = Contract(setup, control)
