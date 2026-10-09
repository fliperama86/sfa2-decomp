"""Contract of func_80119694: a0 is an address, no result.

Choices made here:
  - a block of 64 words is allocated and filled; the start address is the
    block's word 29 to 63 (so the 29 words read stay inside it), given with
    a random top byte (the function uses only the low 24 bits; the block's
    address is below 0x100000, so the low 24 bits address the same RAM);
  - each word is, with a probability chosen per case (from 0.1 to 0.9), a
    link to the previous word (its own address minus 4, as the function
    compares it), and otherwise a random word; so that runs of links of
    every length occur, a case sometimes starts with a long run;
  - the counter is run to 0 in the middle of a run, of a non-link, and at
    each of the function's decrements, by the random lengths of the runs
    and the start position.
"""

from contracts import Contract, Setup

WORDS = 64


def setup(state, rng, sym) -> Setup:
    block = state.alloc(4 * WORDS)
    p_link = rng.choice((0.1, 0.3, 0.5, 0.7, 0.9, 0.97))
    for index in range(WORDS):
        address = block + 4 * index
        if rng.random() < p_link:
            value = (address & 0xFFFFFF) - 4
        else:
            value = rng.getrandbits(32)
        state.w32(address, value)
    start = block + 4 * rng.randrange(29, WORDS)
    start |= rng.getrandbits(8) << 24
    return Setup(args=(start,), returns_value=False)


def control(words):
    """Alter the first counter reload: the counter starts at 28, not 29."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0D and w & 0xFFFF == 0x1D]
    if len(found) != 1:
        raise ValueError(f"expected one load of 29, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x1C, "counter starts at 28"


CONTRACT = Contract(setup, control)
