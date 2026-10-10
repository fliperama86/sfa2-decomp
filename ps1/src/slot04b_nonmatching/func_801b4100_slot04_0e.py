"""Contract of func_801b4100_slot04_0e as code.

Reads and writes are listed in the header comment of
func_801b4100_slot04_0e.c. Choices made here:
  - the object, its partner and the slot block (0x394 bytes each) hold random
    bytes; the object's other points at the partner;
  - the object's side is 0 or 1 in four cases of five, else any byte (the
    record table is read at that index; the entry is filled with random bytes
    so that its field_10 is a random word, negative half the time);
  - every recorder copies the whole object, its partner and the whole slot
    block into the log at every call, so the order of the function's stores
    against its calls is compared; no recorded callee gets a pointer to memory
    filled for the call;
  - func_8011f1e0 returns the slot in four cases of five, else 0 (the arm
    without a slot);
  - func_8011f1e0 (0 arguments), func_80120554 (3 arguments) and
    func_801b420c_slot04_0e (1 argument) are recorders; the log of their
    calls is part of the compared memory.
"""

from contracts import CallLog, Contract, Setup


def setup(state, rng, sym) -> Setup:
    slot = state.alloc(0x394)
    state.write(slot, bytes(rng.getrandbits(8) for _ in range(0x394)))
    other = state.alloc(0x394)
    obj = state.alloc(0x394)
    log = CallLog(state, 4096, watch=((obj, 0x394 // 4), (other, 0x394 // 4), (slot, 0x394 // 4)))
    log.replace(sym["func_8011f1e0"], 0, slot if rng.random() < 0.8 else 0)
    log.replace(sym["func_80120554"], 3, 0)
    log.replace(sym["func_801b420c_slot04_0e"], 1, 0)

    state.write(other, bytes(rng.getrandbits(8) for _ in range(0x394)))
    state.write(obj, bytes(rng.getrandbits(8) for _ in range(0x394)))
    state.w32(obj + 0x40, other)
    side = rng.randrange(2) if rng.random() < 0.8 else rng.randrange(256)
    state.w8(obj + 0xA6, side)
    record = sym["data_801c62b4_slot04_0e"] + 0x1C * side
    state.write(record, bytes(rng.getrandbits(8) for _ in range(0x1C)))
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the constant 0x1e0 stored in the slot's field_7c (an ori from zero)."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0D and (w >> 21) & 31 == 0 and w & 0xFFFF == 0x1E0]
    if len(found) != 1:
        raise ValueError(f"expected one load of the constant 0x1e0, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x1E1, "constant stored to field_7c changed from 0x1e0 to 0x1e1"


CONTRACT = Contract(setup, control)
