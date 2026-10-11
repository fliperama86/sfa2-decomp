"""Contract of func_80011afc_z2o: no effect on memory or on the result register.

Choices of the setup (reads and writes are listed in func_80011afc_z2o.c):
  - a0 to a3 are random 32 bit words, so that a function whose result or stores depended on an argument would
    show; the argument registers themselves are not compared after the call;
  - the bytes around a word of the program's data are random, so that a store there would show;
  - the test compares v0 after the call (returns_value=True). The tool starts every run with v0 at 0, so what
    is shown is that the function leaves it 0, and the negative control (an instruction put into the delay slot
    that sets v0 to 1) has something to show.
"""
from contracts import Contract, Setup, fill


def setup(state, rng, sym):
    fill(state, sym["data_8002f86c_z2o"] - 8, 16, rng)
    return Setup(args=tuple(rng.getrandbits(32) for _ in range(4)), returns_value=True)


def control(words):
    """Alter the delay slot of the return (a nop) into `ori v0,zero,1`."""
    found = [i for i, w in enumerate(words) if w == 0x03E00008]
    if len(found) != 1 or found[0] + 1 >= len(words) or words[found[0] + 1] != 0:
        raise ValueError("expected one return followed by a nop")
    return found[0] + 1, 0x34020001, "the delay slot of the return sets v0 to 1"


CONTRACT = Contract(setup, control)
