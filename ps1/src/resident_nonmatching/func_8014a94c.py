"""Contract of func_8014a94c (header comment of the .c file lists reads and writes).

Choices of the setup:
  - data_80189460 points at a block of 4 random halfwords (the function
    reads the first); data_80189464 starts random;
  - the object, its partner, the linked object (partner->field_14c) are
    separate 0x394-byte blocks of random bytes; the linked object has a
    frame record (index 0 to 7) and a box table of 8 records, as
    func_8014c4a8 needs;
  - the partner's field_240 is 0 in a fifth of the cases and field_14c is
    0 in a fifth; the linked object's field_04 is 1 in four cases of five;
  - the partner's field_0b is 0 in a third of the cases (func_8014c4a8);
  - half the time the object's pos_x is set near the partner's, so that the
    distance func_8014c4a8 computes is small as well as random;
  - in three cases of ten the first script word is set to the distance that
    func_8014c4a8 will compute, or one off, so the comparison's border is tried;
  - func_8014c914 is a recorder with one argument (the object; no pointee).
    The log watches the whole object, data_80189460 and data_80189464 (as
    one two-word block), ref_other and ref_second at every call.
"""
from contracts import CallLog, Contract, Setup, fill, halfword


def setup(state, rng, sym) -> Setup:
    script = state.alloc(8)
    fill(state, script, 8, rng)
    state.w32(sym["data_80189460"], script)
    state.w16(sym["data_80189464"], halfword(rng))

    boxes = state.alloc(8 * 6)
    fill(state, boxes, 8 * 6, rng)
    frame = state.alloc(16)
    fill(state, frame, 16, rng)
    state.w8(frame, rng.randrange(8))
    linked = state.alloc(0x394)
    fill(state, linked, 0x394, rng)
    state.w32(linked + 0x88, frame)
    state.w32(linked + 0x6C, boxes)
    state.w8(linked + 0x04, 1 if rng.random() < 0.8 else rng.randrange(256))

    other = state.alloc(0x394)
    fill(state, other, 0x394, rng)
    state.w8(other + 0x0B, rng.choice((0, rng.randrange(256))))
    if rng.random() < 0.2:
        state.w8(other + 0x240, 0)
    state.w32(other + 0x14C, 0 if rng.random() < 0.2 else linked)

    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w32(obj + 0x40, other)
    if rng.random() < 0.5:
        state.w16(obj + 0x12, int.from_bytes(state.read(other + 0x12, 2), "little") + rng.randrange(-300, 300))
    if rng.random() < 0.5:
        state.w16(script, rng.randrange(0, 600))
    if rng.random() < 0.3:
        # make the script word equal to the distance func_8014c4a8 will compute
        # (or one off), so that the comparison's border is tried
        box = boxes + 6 * state.read(frame, 1)[0]
        origin = int.from_bytes(state.read(box, 2), "little")
        extent = -state.read(box + 4, 1)[0]
        if state.read(other + 0x0B, 1)[0]:
            origin, extent = -origin, -extent
        dist = (int.from_bytes(state.read(obj + 0x12, 2), "little")
                - (origin + extent + int.from_bytes(state.read(other + 0x12, 2), "little"))) & 0xFFFF
        if dist & 0x8000:
            dist = -dist & 0xFFFF
        state.w16(script, dist + rng.choice((-1, 0, 1)))
    log = CallLog(state, 1024, watch=((obj, 0x394 // 4), (sym["data_80189460"], 2),
                                    (sym["ref_other"], 1), (sym["ref_second"], 1)))
    log.replace(sym["func_8014c914"], 1)
    return Setup(args=(obj,), returns_value=False)


def control(words):
    """Alter the byte store of field_209: its offset moves by one byte."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0x209]
    if len(found) != 1:
        raise ValueError(f"expected one store to field_209, found {len(found)}")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0x20A, "store of field_209 moved by one byte"


CONTRACT = Contract(setup, control)
