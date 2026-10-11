"""Contract of func_800107e8_logo: a0 = base, a1 = str_sector, a2 = bin_sector, a3 = vmode, the fifth argument (sp + 0x10) = stereo; no result.

Sets the program's memory layout from a base, reads the stills' file (the loop of disc calls), relocates 32 words, starts graphics and the stream.

Choices of the setup (reads and writes are listed in func_800107e8_logo.c):
  - the buffer of 32 words (128 bytes) that the file is read into is a block
    that the setup allocates; base is that block's address minus 0x21100, so
    that the pointer the function computes lands on the block; the other
    arguments are random 32-bit words (they are only stored or passed on);
  - the stored words (0x8002eb00 to 0x8002eb5f) and the bytes around the
    position at 0x8002ebb4 start as random bytes, so that a store that is
    missing or of the wrong width would show, and no byte of the program's
    data is read;
  - the loop of the disc calls runs 1 to 4 times: the passes before the last
    end at one of three steps (CdControl returns 0, or CdRead returns 0, or
    CdReadSync returns a value that is not 0), the last pass has all three
    succeed (CdControl and CdRead a value that is not 0, CdReadSync 0);
    the results of each recorder are scripted in the order the calls come in,
    and the setup follows the function's flow while it builds them;
  - CdIntToPos fills four bytes of its own at each call (a different word at
    each call), and CdControl logs the word behind its second argument, so the
    position of each pass is compared; CdRead fills the 128 bytes of the
    buffer with different random bytes at each call, so that the relocation is
    done on what the last read left;
  - the stored words, the stereo and vmode words, and the first four words of
    the buffer are watched, so that the log shows what the function has stored
    and relocated by each call;
  - CdReadSync is a recorder whose second argument is the address of a local
    (masked to 0, nothing is read behind it).
"""
import struct

from contracts import CallLog, Contract, Setup, fill

STACK_TOP = 0x801FF000  # sp at entry (the tool's constant); the fifth argument sits at sp + 0x10


def setup(state, rng, sym):
    words = sym["data_8002eb00_logo"]
    position = sym["data_8002ebb4_logo"]
    stereo_word = sym["data_8002eb34_logo"]
    fill(state, words, 0x60, rng)
    fill(state, position - 4, 12, rng)
    buffer = state.alloc(0x80)
    base = buffer - 0x21100

    def nz():
        return 1 if rng.random() < 0.5 else rng.randrange(1, 1 << 32)

    control, read, sync, position_fills, read_fills = [], [], [], [], []
    passes = 1 + rng.randrange(4)
    for number in range(1, passes + 1):
        position_fills.append((number, 1, 0, struct.pack("<I", rng.getrandbits(32))))
        stage = rng.randrange(1, 4) if number < passes else 4
        if stage == 1:
            control.append(0)
            continue
        control.append(nz())
        read.append(0 if stage == 2 else nz())
        read_fills.append((len(read), 1, 0, bytes(rng.getrandbits(8) for _ in range(0x80))))
        if stage == 2:
            continue
        sync.append(nz() if stage == 3 else 0)

    log = CallLog(state, words=1536, watch=((words, 9), (stereo_word, 2), (sym["data_8002eb48_logo"], 2), (buffer, 4)))
    log.replace(sym["CdIntToPos"], 2, fills=tuple(position_fills))
    log.replace(sym["CdControl"], 3, results=tuple(control), pointees={1: 1})
    log.replace(sym["CdRead"], 3, results=tuple(read), fills=tuple(read_fills))
    log.replace(sym["CdReadSync"], 2, results=tuple(sync), masks={1: 0})
    log.replace(sym["func_80010928_logo"], 0)
    log.replace(sym["func_80010a80_logo"], 1)
    log.replace(sym["func_80010fc4_logo"], 0)
    state.w32(STACK_TOP + 0x10, rng.getrandbits(32))
    return Setup(args=(base & 0xFFFFFFFF, rng.getrandbits(32), rng.getrandbits(32), rng.getrandbits(32)), returns_value=False)


def control(words):
    """Alter the count of the relocation: the `slti rt,rs,0x20` becomes `slti rt,rs,0x1f`."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0A and w & 0xFFFF == 0x20]
    if len(found) != 1:
        raise ValueError(f"expected one slti with 0x20, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x1F, "the relocation covers 31 words instead of 32"


CONTRACT = Contract(setup, control)
