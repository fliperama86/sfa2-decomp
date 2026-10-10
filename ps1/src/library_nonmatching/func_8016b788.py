"""Contract of func_8016b788 (the sound chip's reset; a0 = 0 cold, not 0 hot; no result is compared).

Reads and writes are listed in the header comment of func_8016b788.c. The
chip cannot be in the test, so the setup gives both runs plain memory:
  - _spu_RXX points at a block of 0x200 bytes of RAM filled with random
    bytes, and D_80033514 at a word of RAM with a random value;
  - the status register at offset 0x1ae gets, in a case of three, low 11 bits
    of 0 (the waits end at the first poll), in a case of three low 11 bits
    that are not 0 with bit 10 clear, and in a case of three a random value
    (the wait of _spu_init runs to its time-out whenever its low 11 bits are
    not 0, and the wait of _spu_writeByIO whenever bit 10 is set; each time
    the printf recorder logs the call);
  - the globals that the function writes (_spu_transMode, _spu_addrMode,
    _spu_tsa, D_800334FC, the four memory-mode words, _spu_inTransfer, the
    two callbacks) start random, so that each store changes something;
  - arg0 is 0 in one case of two and a random nonzero value otherwise;
  - _spu_writeByIO and _spu_setVoiceRegs are not replaced: they run as the
    original code on the plain memory. printf is a recorder with two
    arguments returning 0; its second argument is logged under a mask of 0
    (the build's string sits at another address) and the first 12 characters
    of that string are logged as three words behind the pointer.
"""

from contracts import CallLog, Contract, Setup


def setup(state, rng, sym):
    block = state.alloc(0x200)
    for offset in range(0, 0x200, 4):
        state.w32(block + offset, rng.getrandbits(32))
    kind = rng.randrange(3)
    if kind == 0:
        status = rng.getrandbits(16) & ~0x7FF
    elif kind == 1:
        status = (rng.getrandbits(16) & ~0x7FF) | rng.randrange(1, 0x400)
    else:
        status = rng.getrandbits(16)
    state.w16(block + 0x1AE, status)
    dma = state.alloc(4)
    state.w32(dma, rng.getrandbits(32))
    state.w32(sym["_spu_RXX"], block)
    state.w32(sym["D_80033514"], dma)
    for name in ("_spu_transMode", "_spu_addrMode", "D_800334FC", "_spu_mem_mode", "_spu_mem_mode_plus",
                 "_spu_mem_mode_unit", "_spu_mem_mode_unitM", "_spu_inTransfer", "_spu_transferCallback",
                 "_spu_IRQCallback"):
        state.w32(sym[name], rng.getrandbits(32))
    state.w16(sym["_spu_tsa"], rng.getrandbits(16))
    log = CallLog(state, words=64)
    log.replace(sym["printf"], 2, 0, masks={1: 0}, pointees={1: 3})
    arg0 = 0 if rng.random() < 0.5 else rng.randrange(1, 1 << 32)
    return Setup(args=(arg0,), returns_value=False)


def control(words):
    """Alter the final store of the control register: 0xc000 becomes 0xc001.

    It is the one `ori rt, zero, 0xc000` of the function; both arms reach it.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0xD and (w >> 21) & 31 == 0 and w & 0xFFFF == 0xC000]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0xc000, found {len(found)}")
    index = found[0]
    return index, words[index] + 1, "control register receives 0xc001"


CONTRACT = Contract(setup, control)
