"""Contract of func_80011478_z2o: a0 = the source pointer, a1 = x, a2 = y, a3 = a total, the height and the wide flag on the stack; draws bands, no result.

Choices of the setup (reads and writes are listed in func_80011478_z2o.c):
  - the source pointer, x and y are random 32 bit words (the pointer is never dereferenced: it only goes to the
    recorder), so that the halfword stores of x and y and the wrap of x after steps show;
  - the total is 16 times a count of 0 to 8 plus a remainder of 0 to 15 (the count of bands is total >> 4, so the
    remainder must not matter), and the count is 0 in one case of nine;
  - the height (the fifth argument, at sp + 0x10) is a random 32 bit word, so that the stored halfword and the
    stride from the full product are both told apart; the wide flag (sixth, at sp + 0x14) is 0 in half of the cases
    and else a random non-zero word, so that a test of one bit or of the low byte only would show;
  - the words around the two stack words are random;
  - func_80157fc4 is a recorder with 2 arguments, the rectangle's address masked (an address of a local) and the
    two words behind it logged (the rectangle as the function made it); DrawSync is a recorder with 1 argument;
  - the count of bands is kept to 8 or fewer, so the log of a case is bounded; a count above that is not tried.
"""
from contracts import CallLog, Contract, Setup, fill

STACK_TOP = 0x801FF000  # sp at entry (the tool's constant); the fifth and sixth arguments sit at sp + 0x10 and sp + 0x14


def setup(state, rng, sym):
    fill(state, STACK_TOP + 0x10, 8, rng)
    fill(state, STACK_TOP + 0x18, 8, rng)
    height = rng.getrandbits(32)
    wide = 0 if rng.random() < 0.5 else rng.randrange(1, 1 << 32)
    state.w32(STACK_TOP + 0x10, height)
    state.w32(STACK_TOP + 0x14, wide)
    count = 0 if rng.randrange(9) == 0 else rng.randrange(1, 9)
    total = 16 * count + rng.randrange(16)
    log = CallLog(state, words=128)
    log.replace(sym["func_80157fc4"], 2, masks={0: 0}, pointees={0: 2})
    log.replace(sym["DrawSync"], 1)
    return Setup(args=(rng.getrandbits(32), rng.getrandbits(32), rng.getrandbits(32), total), returns_value=False)


def control(words):
    """Alter the count of bands: the shift `srl rd,rt,4` becomes `srl rd,rt,3`."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0 and w & 0x3F == 2 and (w >> 6) & 31 == 4]
    if len(found) != 1:
        raise ValueError(f"expected one srl by 4, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~(31 << 6)) | (3 << 6), "the count of bands is the total >> 3 instead of >> 4"


CONTRACT = Contract(setup, control)
