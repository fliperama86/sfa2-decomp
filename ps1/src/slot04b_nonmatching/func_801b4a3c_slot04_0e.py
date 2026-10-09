"""Contract of func_801b4a3c_slot04_0e as code.

Reads and writes are listed in the header comment of
func_801b4a3c_slot04_0e.c. Choices made here:
  - the object and the slot block (0x394 bytes each) hold random bytes;
  - game_state.field_1d has its low three bits zero in half the cases, else
    random;
  - the object's side is 0 or 1 in four cases of five, else any byte (the
    record table is read at that index; the entry is filled with random bytes
    so that its field_10 is a random word, negative half the time);
  - every recorder copies the whole object and the whole slot block into the
    log at every call, so the order of the function's stores against its calls
    is compared; no recorded callee gets a pointer to memory filled for the
    call;
  - func_8011f1e0 returns the slot in four cases of five, else 0; it is a
    recorder (0 arguments) and the log of its calls is part
    of the compared memory.
"""

from contracts import CallLog, Contract, Setup


def setup(state, rng, sym) -> Setup:
    slot = state.alloc(0x394)
    state.write(slot, bytes(rng.getrandbits(8) for _ in range(0x394)))
    obj = state.alloc(0x394)
    log = CallLog(state, 2048, watch=((obj, 0x394 // 4), (slot, 0x394 // 4)))
    log.replace(sym["func_8011f1e0"], 0, slot if rng.random() < 0.8 else 0)

    counter = rng.getrandbits(8)
    state.w8(sym["game_state"] + 0x1D, counter & ~7 if rng.random() < 0.5 else counter)
    state.write(obj, bytes(rng.getrandbits(8) for _ in range(0x394)))
    side = rng.randrange(2) if rng.random() < 0.8 else rng.randrange(256)
    state.w8(obj + 0xA6, side)
    record = sym["data_801c62b4_slot04_0e"] + 0x1C * side
    state.write(record, bytes(rng.getrandbits(8) for _ in range(0x1C)))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the constant 0x4e stored in the slot's field_48 (an ori from zero)."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0D and (w >> 21) & 31 == 0 and w & 0xFFFF == 0x4E]
    if len(found) != 1:
        raise ValueError(f"expected one load of the constant 0x4e, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x4F, "constant stored to field_48 changed from 0x4e to 0x4f"


CONTRACT = Contract(setup, control)
