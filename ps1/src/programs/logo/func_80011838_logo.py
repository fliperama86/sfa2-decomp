"""Contract of func_80011838_logo: no arguments, no effect.

Choices of the setup (reads and writes are listed in func_80011838_logo.c):
  - there is nothing to set up; the function reads and writes nothing;
  - the test compares v0 after the call (returns_value=True), so that the
    function is also shown to leave the result register alone, and so that
    the negative control (an instruction put into the delay slot that
    changes v0) has something to show;
  - all registers and memory are the test's own starting state, which the
    two runs share.
"""
from contracts import Contract, Setup


def setup(state, rng, sym):
    return Setup(args=(), returns_value=True)


def control(words):
    """Alter the delay slot of the return (a nop) into `ori v0,zero,1`."""
    found = [i for i, w in enumerate(words) if w == 0x03E00008]
    if len(found) != 1 or found[0] + 1 >= len(words) or words[found[0] + 1] != 0:
        raise ValueError("expected one return followed by a nop")
    return found[0] + 1, 0x34020001, "the delay slot of the return sets v0 to 1"


CONTRACT = Contract(setup, control)
