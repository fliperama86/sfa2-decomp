"""Contract of func_800119e8_z2o: a0 = index, a1 = pointer to a word, returns u32 in v0.

Choices of the setup (reads and writes are listed in func_800119e8_z2o.c):
  - the entry the function reads, at table + 12 * index, is written by the
    setup (three random words), so that no byte of the program's own data
    is read by a case;
  - the index is from 0 to 15 in two cases of three (the table of the disc's
    files is short), from 0 to 0x0fff in the others; a negative index is
    not tried, because its entry would lie among the function's own words;
  - the pointer is a block of one word that holds a random value before the
    call, so that the store through it shows;
  - the table's name comes from the program's symbols in programs.toml.
"""
from contracts import Contract, Setup, fill


def setup(state, rng, sym):
    table = sym["data_8001b354_z2o"]
    index = rng.randrange(16) if rng.random() < 2 / 3 else rng.randrange(0x1000)
    fill(state, table + 12 * index, 12, rng)
    out = state.alloc(4)
    fill(state, out, 4, rng)
    return Setup(args=(index & 0xFFFFFFFF, out), returns_value=True)


def control(words):
    """Alter the load of the returned word (`lw v0,0(at)`) into the load of the entry's second word."""
    found = [i for i, w in enumerate(words) if w >> 16 == 0x8C22 and w & 0xFFFF == 0]
    if len(found) != 1:
        raise ValueError(f"expected one load into v0 at offset 0, found {len(found)}")
    index = found[0]
    return index, words[index] | 0x0004, "the returned word is the second word of the entry instead of the first"


CONTRACT = Contract(setup, control)
