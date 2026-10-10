"""Contract of func_80078628_slot00 (reads and writes: see the header of the .c).

Choices of the setup:
  - game_state.field_4e is 0 in a third of the cases, a random non-zero byte
    otherwise;
  - the byte at offset 0x3a of the block that field_3c points at is 0 in
    half of the cases, a random non-zero byte otherwise (the rest of both
    blocks is random);
  - func_80078b50_slot00, func_80078b94_slot00, func_80078d78_slot00,
    func_80131094 and func_80078458_slot00 are recorders returning 0; the
    log copies the whole object at every call.
"""

from contracts import CallLog, Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym) -> Setup:
    target = state.alloc(0x394)
    fill(state, target, 0x394, rng)
    state.w8(target + 0x3A, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w32(obj + 0x3C, target)
    state.w8(sym["game_state"] + 0x4E, 0 if rng.random() < 1 / 3 else rng.randrange(1, 256))

    log = CallLog(state, 4096, watch=((obj, 0x394 // 4),))
    log.replace(sym["func_80078b50_slot00"], 1)
    log.replace(sym["func_80078b94_slot00"], 1)
    log.replace(sym["func_80078d78_slot00"], 1)
    log.replace(sym["func_80131094"], 1)
    log.replace(sym["func_80078458_slot00"], 2)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the increment of field_06: `addiu v0,v0,1` becomes `addiu v0,v0,2`."""
    found = [i for i, w in enumerate(words) if w >> 26 == 9 and (w >> 21) & 31 == 2 and (w >> 16) & 31 == 2 and w & 0xFFFF == 1]
    if len(found) != 1:
        raise ValueError(f"expected one increment of v0 by 1, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 2, "field_06 increment changed from 1 to 2"


CONTRACT = Contract(setup, control)
