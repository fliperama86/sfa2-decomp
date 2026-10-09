"""Contract of func_800e0b48_slot0f: a memory-card check that runs once.

Choices of the setup:
  - scratch_word_08 is 0 in three cases of four, else random non-zero or 0;
  - data_8016e686 is a random byte (half of them with the high bit set,
    so that the signed argument of func_80120374 differs from the byte);
  - game_state.field_10 starts random;
  - func_800e0bf4_slot0f answers 0, 1, 2, 3 or a random word, each often
    (two arguments, the second a pointee of 6 words);
  - func_800e0a1c_slot0f answers 0 in half the cases, else a random
    non-zero word (one argument);
  - func_800e1348_slot0f (no argument) and func_80120374 (one argument)
    are recorders answering 0;
  - the log watches scratch_word_08 and the word at game_state + 0x10.
"""
from contracts import CallLog, Contract, Setup, halfword


def setup(state, rng, sym):
    flag = sym["scratch_word_08"]
    if rng.random() < 0.75:
        state.w32(flag, 0)
    else:
        state.w32(flag, rng.choice((0, rng.getrandbits(32) | 1)))
    state.w8(sym["data_8016e686"], rng.getrandbits(8))
    game_state = sym["game_state"]
    state.w32(game_state + 0x10, rng.getrandbits(32))
    log = CallLog(state, 256, watch=((flag, 1), (game_state + 0x10, 1)))
    first = rng.choice((0, 1, 2, 3, rng.getrandbits(32)))
    second = 0 if rng.random() < 0.5 else rng.getrandbits(32) | 1
    log.replace(sym["func_800e0bf4_slot0f"], 2, first, pointees={1: 6})
    log.replace(sym["func_800e0a1c_slot0f"], 1, second)
    log.replace(sym["func_800e1348_slot0f"], 0, 0)
    log.replace(sym["func_80120374"], 1, 0)
    return Setup(args=(), returns_value=True)


def control(words):
    """Alter the mode loaded when the card answer is neither 1 nor 3:
    0x10 becomes 0x11 (the `ori a0,zero,0x10` in the delay slot)."""
    found = [i for i, w in enumerate(words) if w == 0x34040010]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x10 into a0, found {len(found)}")
    i = found[0]
    return i, words[i] + 1, "mode 0x10 becomes 0x11"


CONTRACT = Contract(setup, control)
