"""Contract of func_80166144 (opens a group of sequences; the slot, or -1, in v0).

Reads and writes are listed in the header comment of func_80166144.c.
Choices of the setup:
  - _snd_openflag is all ones in one case of eight; otherwise its lowest
    clear bit is a number 0 to 31 (the bits below it are set, the bit itself
    clear, the bits above random);
  - arg0 (the address) is a random word, not dereferenced; arg1 (vab_id) and
    arg2 (count) have random upper halves, and their lower halves are the
    values that matter: count is 0 to 8 in seven cases of eight and 0x8000 to
    0xffff (negative as an s16, so no call) in one of eight;
  - func_80165d84 is a recorder with four arguments, and each case gives it a
    list of results: lengths of 0 to 200 bytes, and in one case of three a -1
    at a random position of the list (the list has 8 results; a count of 8 is the
    longest run); printf is a recorder with one argument whose string is logged
    as nine words behind its pointer.
"""

from contracts import CallLog, Contract, Setup


def setup(state, rng, sym):
    if rng.random() < 0.125:
        flag = 0xFFFFFFFF
    else:
        bit = rng.randrange(32)
        flag = ((1 << bit) - 1) | (rng.getrandbits(32) & ~((1 << (bit + 1)) - 1) & 0xFFFFFFFF)
    state.w32(sym["_snd_openflag"], flag)
    if rng.random() < 0.125:
        count = rng.randrange(0x8000, 0x10000)
    else:
        count = rng.randrange(9)
    results = [rng.randrange(0, 201) for _ in range(8)]
    if rng.random() < 1 / 3:
        results[rng.randrange(len(results))] = 0xFFFFFFFF
    log = CallLog(state, words=64)
    log.replace(sym["printf"], 1, 0, masks={0: 0}, pointees={0: 9})
    log.replace(sym["func_80165d84"], 4, 0, masks={0: 0xFFFF, 1: 0xFFFF, 2: 0xFFFF}, results=tuple(results))
    addr = rng.getrandbits(32)
    vab = rng.getrandbits(32)
    return Setup(args=(addr, vab, rng.getrandbits(16) << 16 | count), returns_value=True)


def control(words):
    """Alter the bit mask of the search: `ori a3, zero, 1` becomes `ori a3, zero, 2`.

    It is the one such word of the function; every case with a free bit
    reaches it.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0xD and (w >> 21) & 31 == 0 and (w >> 16) & 31 == 7 and w & 0xFFFF == 1]
    if len(found) != 1:
        raise ValueError(f"expected one ori of 1 into a3, found {len(found)}")
    index = found[0]
    return index, words[index] + 1, "bit mask 1 becomes 2"


CONTRACT = Contract(setup, control)
