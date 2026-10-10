"""Contract of func_800e1250_slot0f in the port's C (header comment of the .c file).

Choices of the setup:
  - the 3 arguments are random whole words (fd, buf, n); they are only
    passed on, so the recorder of `read` logs them and nothing is read through
    them;
  - the script of `read` (one result per call, the last one repeated) is
    drawn from three kinds in equal shares: the first try answers a word other
    than -1 (kind 0); the first k - 1 tries answer -1 and the k-th a word other
    than -1, k = 2 to 0x78, with k = 2 and k = 0x78 drawn in a third of these
    cases (kind 1); all tries answer -1, so the loop makes its 0x78 tries
    (kind 2);
  - a word other than -1 is drawn from 0, 1, 0xfffffffe, 0xffff, 0xffff0000,
    0xff, 0xffffff00, 0x80000000, 0x7fffffff, a random 30-bit value and a
    random word with its low bit clear, so that the result is returned whole and
    a test against -1 meets its near misses;
  - func_8015fb30 (1 argument) is a recorder that returns 0; both recorders
    log into one log, so the order of the calls and the count of the tries are
    compared.
"""
from contracts import CallLog, Contract, Setup

TRIES = 0x78
JR_RA = 0x03E00008


def not_minus_one(rng):
    return rng.choice((0, 1, 0xFFFFFFFE, 0xFFFF, 0xFFFF0000, 0xFF, 0xFFFFFF00, 0x80000000, 0x7FFFFFFF,
                       rng.randrange(2, 1 << 30), rng.getrandbits(32) & 0xFFFFFFFE))


def setup(state, rng, sym) -> Setup:
    kind = rng.randrange(3)
    if kind == 0:
        script = (not_minus_one(rng),)
    elif kind == 1:
        k = rng.choice((2, TRIES, rng.randrange(2, TRIES + 1)))
        script = (0xFFFFFFFF,) * (k - 1) + (not_minus_one(rng),)
    else:
        script = (0xFFFFFFFF,)
    log = CallLog(state, 1024)
    log.replace(sym["func_8015fb30"], 1, 0)
    log.replace(sym["read"], 3, results=script)
    return Setup(args=tuple(rng.getrandbits(32) for _ in range(3)), returns_value=True)


def control(words):
    """Alter the returned value: the empty delay slot of the function's `jr ra` becomes `addiu v0,v0,1`.

    The control alters the build of this C (the tool hands it the build's code words, not the original's). The
    altered build then returns the result plus 1, while the original code returns the result unchanged: this is
    the alteration that shows the test compares the result register.
    """
    found = [i for i, w in enumerate(words) if w == JR_RA]
    if len(found) != 1 or words[found[0] + 1] != 0:
        raise ValueError("expected one `jr ra` with an empty delay slot")
    return found[0] + 1, 0x24420001, "the function returns its result + 1"


CONTRACT = Contract(setup, control)
