"""Contract of func_80010a80_logo: a0 = sector, the result in v0 is 0; starts the movie's stream at a sector and takes the first frame.

Choices of the setup (reads and writes are listed in func_80010a80_logo.c):
  - the sector is, in most cases, a number of the order of the disc's
    sectors (100 to 0xffffff), else a small number (0 to 99, so that
    sector - 100 is negative) or a small negative number; the position
    CdPosToInt returns at the last pass is one at an edge of the accepted
    range (sector - 100, sector - 99, sector + 4, sector + 5) or inside it,
    chosen so that the function's own test accepts it (the setup computes
    the test itself); the passes before it return a position outside
    (sector - 101 and lower, sector + 6 and higher);
  - the list of positions ends with one more, the sector itself (which the
    function's test accepts): the function never asks for it, a variant that
    refused the last position would, and then the log differs rather than
    the run going on without end;
  - the loop is entered 1 to 4 times: before the last pass, 0 to 3 passes
    that end at one of the four steps (the filter call returns 0, the
    second call returns 0, CdRead2 returns 0, or all pass and the position is
    wrong); the results of each recorder are scripted in the order the
    calls come in, so the setup follows the function's flow while it builds
    them;
  - CdControlB is one recorder for both loops: its results are 0 up to
    twice and then a value that is not 0, for every wait of the first loop
    and for the position request; the byte its fill stores in res[0] has bit
    0x20 clear except in the call that ends the first loop in the last
    turn of that loop (the other bits are random, so a test for any bit
    other than 0x20 would show);
  - StGetNext returns a value that is not 0 up to twice and then 0; its two
    fills store the pointers through the two locals: garbage at the calls
    before the last, and at the last call the data pointer (a random word)
    and the pointer to a 16 byte header block that the setup made, whose word
    at +8 is a random number; the recorder also stores 0 or 1 in the index
    word at 0x8002eb20 at one of its calls (or none, in one case of five),
    so that the re-read of the index shows (the function's own store of 0
    is then replaced);
  - the words at 0x8002eb04 to 0x8002eb5f and the four bytes at 0x8002ebb4
    start as random bytes, the pointer words at 0x8002eb08 and 0x8002eb0c
    among them (the buffers are only passed on), so that a store that is
    missing or of the wrong width shows, and no byte of the program's data is
    read;
  - CdIntToPos fills the four bytes at its second argument with a random word;
    CdControl logs the word behind its second argument (the filter bytes at
    the first call, the position at the second);
  - the words at 0x8002eb20, 0x8002eb2c and 0x8002eb30, 0x8002eb3c and
    0x8002eb44 are watched, so that the log shows what the function has
    stored by each call.
"""
import struct

from contracts import CallLog, Contract, Setup, fill


def signed(value):
    value &= 0xFFFFFFFF
    return value - (1 << 32) if value & 0x80000000 else value


def accepts(sector, pos):
    """The function's own test of the position: True when the loop ends."""
    if signed(pos) < signed(sector - 100):
        return False
    return not ((sector + 5) & 0xFFFFFFFF) < (pos & 0xFFFFFFFF)


def setup(state, rng, sym):
    base = sym["data_8002eb04_logo"]
    buffers = sym["data_8002eb08_logo"]
    index = sym["data_8002eb20_logo"]
    frame = sym["data_8002eb2c_logo"]
    flag = sym["data_8002eb3c_logo"]
    filt = sym["data_8002eb44_logo"]
    position = sym["data_8002ebb4_logo"]
    kind = rng.random()
    if kind < 0.8:
        sector = rng.randrange(100, 0x1000000)
    elif kind < 0.95:
        sector = rng.randrange(0, 100)
    else:
        sector = -rng.randrange(1, 9)
    fill(state, base, 0x5C, rng)
    fill(state, position - 4, 12, rng)

    def nz():
        return 1 if rng.random() < 0.5 else rng.randrange(1, 1 << 32)

    def status(bit):
        value = rng.getrandbits(8)
        return (value | 0x20) if bit else (value & ~0x20 & 0xFF)

    control_results, read_results, wait_results, pos_results, statuses = [], [], [], [], {}

    def first_loop():
        turns = 1 + rng.randrange(3)
        for turn in range(turns):
            for _ in range(rng.randrange(3)):
                wait_results.append(0)
                statuses[len(wait_results)] = status(rng.random() < 0.5)
            wait_results.append(nz())
            statuses[len(wait_results)] = status(turn == turns - 1)

    def position_request():
        for _ in range(rng.randrange(3)):
            wait_results.append(0)
            statuses[len(wait_results)] = rng.getrandbits(8)
        wait_results.append(nz())
        statuses[len(wait_results)] = rng.getrandbits(8)

    def good_position():
        choices = [sector - 100, sector - 99, sector + 4, sector + 5, sector, sector - rng.randrange(0, 100), sector + rng.randrange(0, 6)]
        rng.shuffle(choices)
        for pos in choices:
            if accepts(sector, pos):
                return pos & 0xFFFFFFFF
        return (sector + 5) & 0xFFFFFFFF

    def bad_position():
        choices = [sector - 101, sector + 6, sector - 102 - rng.randrange(0, 1 << 20), sector + 7 + rng.randrange(0, 1 << 20), sector + (1 << 31)]
        rng.shuffle(choices)
        for pos in choices:
            if not accepts(sector, pos):
                return pos & 0xFFFFFFFF
        raise ValueError("no position outside the range")

    for _ in range(rng.randrange(4)):
        stage = rng.randrange(1, 5)
        if stage == 1:
            control_results += [0]
        elif stage == 2:
            control_results += [nz(), 0]
        elif stage == 3:
            control_results += [nz(), nz()]
            read_results += [0]
        else:
            control_results += [nz(), nz()]
            read_results += [nz()]
            first_loop()
            position_request()
            pos_results.append(bad_position())
    control_results += [nz(), nz()]
    read_results += [nz()]
    first_loop()
    position_request()
    pos_results.append(good_position())
    pos_results.append(sector & 0xFFFFFFFF)  # never reached by the function; an original that refused the last position would take it

    waits_count = rng.randrange(3)
    next_results = tuple(nz() for _ in range(waits_count)) + (0,)
    last = len(next_results)
    header = state.alloc(16)
    fill(state, header, 16, rng)
    address = rng.getrandbits(32)
    stores = ()
    if rng.random() < 0.8:
        stores = ((1 + rng.randrange(last), index, rng.randrange(2)),)
    fills = ((0, 0, 0, struct.pack("<I", rng.getrandbits(32))), (0, 1, 0, struct.pack("<I", rng.getrandbits(32))),
             (last, 0, 0, struct.pack("<I", address)), (last, 1, 0, struct.pack("<I", header)))

    log = CallLog(state, words=1200, watch=((index, 1), (frame, 2), (flag, 1), (filt, 1)))
    log.replace(sym["DecDCTReset"], 1)
    log.replace(sym["DecDCToutCallback"], 1)
    log.replace(sym["StSetRing"], 2)
    log.replace(sym["StSetStream"], 5)
    log.replace(sym["CdIntToPos"], 2, fills=((0, 1, 0, struct.pack("<I", rng.getrandbits(32))),))
    log.replace(sym["CdControl"], 3, results=tuple(control_results), pointees={1: 1})
    log.replace(sym["CdRead2"], 1, results=tuple(read_results))
    log.replace(sym["CdControlB"], 3, results=tuple(wait_results),
                fills=tuple((number, 2, 0, bytes([byte])) for number, byte in sorted(statuses.items())))
    log.replace(sym["CdPosToInt"], 1, results=tuple(pos_results))
    log.replace(sym["StGetNext"], 2, results=next_results, masks={0: 0, 1: 0}, stores=stores, fills=fills)
    log.replace(sym["DecDCTvlc"], 2)
    log.replace(sym["StFreeRing"], 1)
    return Setup(args=(sector & 0xFFFFFFFF,), returns_value=True)


def control(words):
    """Alter the mask of the status test: the `andi rt,rs,0x20` becomes `andi rt,rs,0x10`."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0C and w & 0xFFFF == 0x20]
    if len(found) != 1:
        raise ValueError(f"expected one andi with 0x20, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x10, "the status test is bit 0x10 instead of bit 0x20"


CONTRACT = Contract(setup, control)
