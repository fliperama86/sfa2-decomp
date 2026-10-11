"""Contract of func_80010c38_z2o: ten arguments (four in registers, six on the stack), the result in v0 is 0; opens the movie (the stream at a sector, the buffers, the first frame).

Choices of the setup (reads and writes are listed in func_80010c38_z2o.c):
  - the arguments are random words; the six stack arguments are written at
    the caller's stack slots (the entry sp + 0x10 to sp + 0x24); `wide`
    (the argument stored at 0x8001b348) is 0 in half the cases and else a
    random non-zero word;
  - the words from 0x8002f800 to 0x8002f8cf (all the data the function
    writes, reads or passes on, the two bytes for the filter, the position,
    the result bytes of the disc command and the 32 byte copy among them)
    and the words from 0x8001b340 to 0x8001b34f start as random bytes, so
    that a store that is missing or of the wrong width shows and no byte of
    the program's data is read;
  - CdControl is one recorder for all its calls, its results scripted in
    turn: 0 up to twice then a value that is not 0 (the filter command), the
    same again (the position command), and then, for each return of 0 of
    CdRead2 (0 to 2 of them), 0 up to twice and a value that is not 0; the
    non-zero values are 1 or a random non-zero word;
  - in four cases of five the recorder of CdControl also stores a random word
    (0 in a quarter of them) in the word at 0x8001b348 at one of the calls
    before the function reads it again, so that the re-read shows;
  - CdControlB is one recorder for the mode command and for the status loop:
    0 up to twice then a non-zero value for the mode command, then the
    status loop runs 1 to 3 turns, each 0 up to twice and then a non-zero
    value; the byte its fill stores in res[0] (the byte at 0x8002f8c4) at
    each call of the status loop has bit 0x20 clear except at the call that
    ends the last turn (the other bits are random, so a test for another
    bit would show). The pointer of the mode command is the address of a
    local and is logged as it is, so the test also compares where that
    local lies: it passes only while the C's frame puts the local at the
    address the original uses;
  - CdRead2 returns 0 up to twice and then a non-zero value;
  - StGetNext returns a non-zero value up to twice and then 0; its fills
    store the pointers through the two locals: garbage at the calls before
    the last, and at the last call a random data pointer and the pointer to
    a 32 byte header block of the setup's; in four cases of five the
    recorder also stores a small index (0 to 7) in the word at 0x8002f820
    at one of its calls, and in half of the cases a random word in the word
    at 0x8002f86c, so that the re-reads show;
  - CdIntToPos fills the four bytes at its second argument with a random
    word; the halfwords of the header block at +0x10 and +0x12 are chosen
    at the edges of the rounding (0, 7, 8, 9, 15, 16, 23, 24, 0xfff7, 0xfff8,
    0xffff, 0x8000 and random);
  - the callees are recorders of the arguments they take: DecDCTReset (1),
    DecDCToutCallback (1), StSetRing (2), StSetStream (5),
    func_800118f0_z2o (0), CdIntToPos (2), CdControl (3, one word behind
    the second logged), CdControlB (3), VSync (1), CdRead2 (1),
    func_80011830_z2o (0), StGetNext (2, both masked to 0), DecDCTvlc (2),
    StFreeRing (1);
  - the words from 0x8002f804 to 0x8002f823, the 32 bytes at 0x8002f84c, the
    word at 0x8002f86c and the word at 0x8001b348 are watched, so that the
    log shows what the function has stored by each call.
"""
import struct

from contracts import CallLog, Contract, Setup, fill

STACK_TOP = 0x801FF000
HALF_EDGES = [0, 7, 8, 9, 15, 16, 17, 23, 24, 0x7FFF, 0x8000, 0xFFF7, 0xFFF8, 0xFFFF]


def setup(state, rng, sym):
    data = sym["data_8002f804_z2o"]
    wide_word = sym["data_8001b348_z2o"]
    index = sym["data_8002f820_z2o"]
    copy = sym["data_8002f84c_z2o"]
    count_word = sym["data_8002f86c_z2o"]
    fill(state, data - 4, 0xD0, rng)
    fill(state, wide_word - 8, 16, rng)

    def nz():
        return 1 if rng.random() < 0.5 else rng.randrange(1, 1 << 32)

    def half():
        return rng.choice(HALF_EDGES) if rng.random() < 0.7 else rng.getrandbits(16)

    # CdControl: the filter command, the position command, then one run for each failure of CdRead2.
    control = [0] * rng.randrange(3) + [nz()]
    control += [0] * rng.randrange(3) + [nz()]
    before_reread = len(control)
    read_results = [0] * rng.randrange(3) + [nz()]
    for _ in range(len(read_results) - 1):
        control += [0] * rng.randrange(3) + [nz()]
    # CdControlB: the mode command, then the status loop.
    wait_results = [0] * rng.randrange(3) + [nz()]
    statuses = {}
    turns = 1 + rng.randrange(3)
    for turn in range(turns):
        for _ in range(rng.randrange(3)):
            wait_results.append(0)
            value = rng.getrandbits(8) & ~0x20 & 0xFF
            statuses[len(wait_results)] = value
        wait_results.append(nz())
        value = rng.getrandbits(8)
        statuses[len(wait_results)] = (value | 0x20) if turn == turns - 1 else (value & ~0x20 & 0xFF)
    next_results = tuple(nz() for _ in range(rng.randrange(3))) + (0,)
    last = len(next_results)
    header = state.alloc(32)
    fill(state, header, 32, rng)
    state.w16(header + 0x10, half())
    state.w16(header + 0x12, half())
    address = rng.getrandbits(32)
    stores = ()
    if rng.random() < 0.8:
        stores += ((1 + rng.randrange(last), index, rng.randrange(8)),)
    if rng.random() < 0.5:
        stores += ((1 + rng.randrange(last), count_word, rng.getrandbits(32)),)
    fills = ((0, 0, 0, struct.pack("<I", rng.getrandbits(32))), (0, 1, 0, struct.pack("<I", rng.getrandbits(32))),
             (last, 0, 0, struct.pack("<I", address)), (last, 1, 0, struct.pack("<I", header)))
    control_stores = ()
    if rng.random() < 0.8:
        control_stores = ((1 + rng.randrange(before_reread), wide_word, 0 if rng.random() < 0.25 else rng.getrandbits(32)),)

    log = CallLog(state, words=4096, watch=((data, 9), (copy, 8), (count_word, 1), (wide_word, 1)))
    log.replace(sym["DecDCTReset"], 1)
    log.replace(sym["DecDCToutCallback"], 1)
    log.replace(sym["StSetRing"], 2)
    log.replace(sym["StSetStream"], 5)
    log.replace(sym["func_800118f0_z2o"], 0)
    log.replace(sym["CdIntToPos"], 2, fills=((0, 1, 0, struct.pack("<I", rng.getrandbits(32))),))
    log.replace(sym["CdControl"], 3, results=tuple(control), pointees={1: 1}, stores=control_stores)
    log.replace(sym["CdControlB"], 3, results=tuple(wait_results),
                fills=tuple((number, 2, 0, bytes([byte])) for number, byte in sorted(statuses.items())))
    log.replace(sym["VSync"], 1)
    log.replace(sym["CdRead2"], 1, results=tuple(read_results))
    log.replace(sym["func_80011830_z2o"], 0)
    log.replace(sym["StGetNext"], 2, results=next_results, masks={0: 0, 1: 0}, stores=stores, fills=fills)
    log.replace(sym["DecDCTvlc"], 2)
    log.replace(sym["StFreeRing"], 1)
    stack = [rng.getrandbits(32) for _ in range(6)]
    stack[4] = 0 if rng.random() < 0.5 else nz()
    for number, value in enumerate(stack):
        state.w32(STACK_TOP + 0x10 + 4 * number, value)
    return Setup(args=tuple(rng.getrandbits(32) for _ in range(4)), returns_value=True)


def control(words):
    """Alter the count of the wait: the 0x32 loaded into s0 (`ori s0,zero,0x32`) becomes 0x33."""
    found = [i for i, w in enumerate(words) if w >> 26 == 13 and (w >> 21) & 31 == 0 and (w >> 16) & 31 == 16 and w & 0xFFFF == 0x32]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x32 into s0, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x33, "the wait is 52 VSyncs instead of 51"


CONTRACT = Contract(setup, control)
