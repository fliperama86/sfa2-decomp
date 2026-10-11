"""Contract of func_80011a24_z2o: no arguments, the result in v0 is 1 when the disc init succeeded, else 0.

Choices of the setup (reads and writes are listed in func_80011a24_z2o.c):
  - CdInit is a recorder whose results are scripted: in one case of five
    four times 0 (the function gives up, result 0), else 0 to three times 0
    and then a value that is not 0 (one, or a random non-zero word);
  - CdControlB is one recorder for both loops, its results scripted in
    turn: 0 up to three times then a non-zero value (the first loop), then
    the same for the second loop; the non-zero values are 1 or a random
    non-zero word;
  - CdIntToPos is a recorder with a fill: it stores a random word of the
    setup at the pointer in its second argument (the model of what the
    library does to the position, written by the contract's author);
  - the recorders log the arguments (the pointer arguments that are
    addresses of locals are masked, so that only the words behind them are
    compared: for CdControlB the word behind the param pointer).
"""
import struct

from contracts import CallLog, Contract, Setup


def setup(state, rng, sym):
    log = CallLog(state, words=128)
    if rng.random() < 0.2:
        init = (0,) * 4
    else:
        init = (0,) * rng.randrange(4) + (1 if rng.random() < 0.5 else rng.randrange(1, 1 << 32),)
    # The loops of the commands: failures then a success, twice (the same recorder).
    def good():
        return 1 if rng.random() < 0.5 else rng.randrange(1, 1 << 32)
    control = (0,) * rng.randrange(4) + (good(),) + (0,) * rng.randrange(4) + (good(),)
    position = rng.getrandbits(32)
    log.replace(sym["CdInit"], 0, results=init)
    log.replace(sym["CdControlB"], 3, results=control, masks={1: 0}, pointees={1: 1})
    log.replace(sym["CdIntToPos"], 2, masks={1: 0}, fills=((0, 1, 0, struct.pack("<I", position)),))
    return Setup(args=(), returns_value=True)


def control(words):
    """Alter the number of tries: the count of 4 becomes 3 (`ori rt,zero,4` or `addiu rt,zero,4`)."""
    found = [i for i, w in enumerate(words) if (w >> 26) in (9, 13) and (w >> 21) & 31 == 0 and w & 0xFFFF == 4]
    if len(found) != 1:
        raise ValueError(f"expected one load of 4, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 3, "the init is tried 3 times instead of 4"


CONTRACT = Contract(setup, control)
