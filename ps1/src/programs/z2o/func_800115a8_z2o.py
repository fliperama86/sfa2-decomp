"""Contract of func_800115a8_z2o: a0 = the volume word, no result; stores it and sets the four volume bytes from it.

Choices of the setup (reads and writes are listed in func_800115a8_z2o.c):
  - the argument is a random 32 bit word (small in a third of the cases);
  - the stereo flag word at 0x8001b350 is 0 in half the cases, else a random
    non-zero word (the function tests it against 0 only);
  - the volume word, the flag word and the four volume bytes start as random
    bytes, and so do the bytes around them, so that a neighbouring read or
    write, a store of the wrong width, or one that is missing would show;
  - CdMix is a recorder of one argument whose pointee is one word (the four
    volume bytes).
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    volume = sym["data_8001b34c_z2o"]
    flag = sym["data_8001b350_z2o"]
    bytes4 = sym["data_8002f8a4_z2o"]
    fill(state, volume - 8, 24, rng)
    fill(state, bytes4 - 8, 20, rng)
    state.w32(flag, 0 if rng.random() < 0.5 else rng.randrange(1, 1 << 32))
    arg = rng.getrandbits(32) if rng.random() < 2 / 3 else rng.randrange(300)
    log = CallLog(state, words=64, watch=((volume, 1), (bytes4, 1)))
    log.replace(sym["CdMix"], 1, pointees={0: 1})
    return Setup(args=(arg,), returns_value=False)


def control(words):
    """Alter the shift: the `srl rd,rt,1` becomes a shift by 2."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0 and w & 0x3F == 2 and (w >> 6) & 31 == 1]
    if len(found) != 1:
        raise ValueError(f"expected one srl by 1, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~(31 << 6)) | (2 << 6), "the volume is halved by a shift of 2 instead of 1"


CONTRACT = Contract(setup, control)
