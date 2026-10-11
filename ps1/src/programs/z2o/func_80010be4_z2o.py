"""Contract of func_80010be4_z2o: no arguments, no result; hands one of two display environments to a library call and flips the index.

Choices of the setup (reads and writes are listed in func_80010be4_z2o.c):
  - the index word is 0 or 1 in four cases of five, otherwise a small int
    from -8 to 63 (the function computes 20 * index whatever the value, and
    the environment it names is then another 20 bytes of the program's data
    area, which the setup makes);
  - the 20 bytes of the environment named by the index are written with
    random bytes by the setup, and so are both environments of the table, so
    that no byte of the program's data is read by a case;
  - the callee func_80158470 (the program's copy, at the address of
    programs.toml) is a recorder of one argument whose pointee is five words;
    the index word is watched, so that the log shows its value at the call.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    table = sym["data_80049eac_z2o"]
    index_word = sym["data_80031e58_z2o"]
    index = rng.randrange(2) if rng.random() < 0.8 else rng.randrange(-8, 64)
    fill(state, table, 40, rng)
    fill(state, table + 20 * index, 20, rng)
    state.w32(index_word, index & 0xFFFFFFFF)
    log = CallLog(state, words=64, watch=((index_word, 1),))
    log.replace(sym["func_80158470"], 1, pointees={0: 5})
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the flip: `xori v0,v0,1` becomes `xori v0,v0,2`."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0E and w & 0xFFFF == 1]
    if len(found) != 1:
        raise ValueError(f"expected one xori with 1, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 2, "the index is flipped with xor 2 instead of xor 1"


CONTRACT = Contract(setup, control)
