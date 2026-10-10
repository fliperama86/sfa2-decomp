"""Contract of func_8013b0c4 as code. The roles are in func_8013b0c4.c.

Choices of the setup:
  - a0 = object, a1 = other, a2 = g, each a block of 0x400 bytes of random
    content;
  - ref_other holds a random word (it is only saved and put back);
  - the counter data_801a27d4 is 0 in one case of ten, 1 in four of ten,
    2 in two of ten and 3, 4 or 5 in one of ten each, so that the second
    effect (needs a counter still not 0 after the first) is tried often;
  - func_8011f1e0 is a recorder with no arguments that returns a block of
    0x400 random bytes, or 0 in one case of five;
  - g->field_17 has bit 0x80 in half of the cases; g->field_10 has bit 0x80
    in one case of five, is 0 to 7 in a third of the rest (index below 16)
    and 0 to 0x7f otherwise (index up to 255, so the reads of
    table_8017736c go into the bytes after it, which are other data of
    the image (inferred) and are filled with random halfwords here);
  - other's field_61 has bit 0x80 in two cases of five; its kind is 6 in 34
    cases of 100 and a random byte otherwise; field_5c is negative in
    half; field_66 is a random byte; the entry of table_80197ef8 for it is
    3 in a third of the cases, so the follow-up step is skipped then;
  - the rectangle data_80188ed0 gets random halfwords at 0x3c to 0x48;
  - the three tables are filled with random halfwords over the range any
    index can reach (table_8017736c up to 0x24 + 512 bytes in one fill,
    which also covers table_80177390 since that starts 0x24 bytes in;
    table_80197ef8, 512 bytes);
  - func_80120554 and func_801204f4 (3 arguments) and func_80120444 (2)
    are recorders that return 0; the second argument of func_80120444 is
    logged as its low byte (the callee masks it, see MaskedLog);
  - the log watches, at every call: other (0x400 bytes), the first 0x100
    bytes of the effect object, data_801a27d4, data_80188f20 and ref_other.
    No recorded callee gets a pointer to memory filled for that call.
"""

from contracts import CallLog, Contract, Setup


class MaskedLog(CallLog):
    """A log whose recorder can AND an argument with a mask before logging it.

    func_80120444 takes a byte as its second argument and masks it itself; the
    original passes a sign-extended halfword, the C passes what its prototype
    says (a byte). The two registers differ only in bits the callee discards,
    so the log records the low byte of that argument. `masks` maps an argument
    index to its mask for the next `replace`.
    """

    masks: dict = {}

    def _argument(self, index, register):
        code = list(CallLog._argument(index, register))
        if index in self.masks:
            code.append(0x30000000 | register << 21 | register << 16 | self.masks[index])  # andi register, register, mask
        return code


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym):
    obj = state.alloc(0x400)
    other = state.alloc(0x400)
    g = state.alloc(0x400)
    for block in (obj, other, g):
        fill(state, block, 0x400, rng)
    fx = 0
    if rng.random() >= 0.2:
        fx = state.alloc(0x400)
        fill(state, fx, 0x400, rng)

    # Everything the function writes and a callee could read.
    watch = [(other, 0x400 // 4), (sym["data_801a27d4"], 1), (sym["data_80188f20"], 1), (sym["ref_other"], 1)]
    if fx:
        watch.append((fx, 0x100 // 4))  # the effect fields written are all below 0x94
    log = MaskedLog(state, 4096, watch=tuple(watch))

    counter = rng.choice((0, 1, 1, 1, 1, 2, 2, 3, 4, 5))
    state.w8(sym["data_801a27d4"], counter)
    state.w32(sym["ref_other"], rng.getrandbits(32))
    state.w32(sym["data_80188f20"], rng.getrandbits(32))
    for offset in (0x3C, 0x40, 0x44, 0x48):
        state.w16(sym["data_80188ed0"] + offset, rng.getrandbits(16))

    fill(state, sym["table_8017736c"], 0x24 + 512, rng)
    fill(state, sym["table_80197ef8"], 512, rng)

    log.replace(sym["func_8011f1e0"], 0, fx)
    log.replace(sym["func_80120554"], 3, 0)
    log.replace(sym["func_801204f4"], 3, 0)
    log.masks = {1: 0xFF}
    log.replace(sym["func_80120444"], 2, 0)
    log.masks = {}

    state.w8(g + 0x17, rng.getrandbits(8) | 0x80 if rng.random() < 0.5 else rng.getrandbits(7))
    if rng.random() < 0.2:
        state.w8(g + 0x10, rng.randrange(0x80, 0x100))
    elif rng.random() < 0.33:
        state.w8(g + 0x10, rng.randrange(0, 8))
    else:
        state.w8(g + 0x10, rng.randrange(0, 0x80))

    flag = rng.getrandbits(8)
    state.w8(other + 0x61, flag | 0x80 if rng.random() < 0.4 else flag & 0x7F)
    state.w8(other + 0xA7, 6 if rng.random() < 0.34 else rng.getrandbits(8))
    state.w16(other + 0x5C, rng.getrandbits(16))
    k = rng.getrandbits(8)
    state.w8(other + 0x66, k)
    if rng.random() < 0.33:
        state.w16(sym["table_80197ef8"] + 2 * k, 3)

    return Setup(args=(obj, other, g), returns_value=False)


def control(words):
    """Alter the store to other's field_17f: its offset moves by one byte.

    The store is `sb reg, 0x17f(reg)`, made once by every call that makes the
    first effect.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x28 and w & 0xFFFF == 0x017F]
    if len(found) != 1:
        raise ValueError(f"expected one store to field_17f, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x017E, "store to field_17f moved by one byte"


CONTRACT = Contract(setup, control)
