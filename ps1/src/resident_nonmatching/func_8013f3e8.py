"""Contract of func_8013f3e8: push an object's field_150 into a side ring.

Choices of the setup:
  - the object is a random block of 0x394 bytes; the side byte (offset 0xa6)
    is 0 in half of the cases and a random non-zero value otherwise;
  - both rings (256 halfwords each) are filled with random words;
  - the head (word 0 of each ring) is in 0 to 255, with 255 (the wrap to 1)
    and 0 chosen in one case of four each.
"""
from contracts import CallLog, Contract, Setup, fill


def setup(state, rng, sym):
    for name in ("ring_left", "ring_right"):
        fill(state, sym[name], 512, rng)
        state.w16(sym[name], rng.choice((0, 255, rng.randrange(256), rng.randrange(256))))
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0xA6, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Move the last halfword store at offset 0 (the write-back of the head) to offset 2.

    Every call executes it; word 0 of the ring then keeps its old value.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0]
    if not found:
        raise ValueError("no halfword store at offset 0")
    index = found[-1]
    return index, words[index] | 2, "head store moved by two bytes"


CONTRACT = Contract(setup, control)
