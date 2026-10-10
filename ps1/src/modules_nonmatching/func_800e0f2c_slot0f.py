"""Contract of func_800e0f2c_slot0f: list the files of a memory card.

Choices of the setup:
  - a0 (card) is 0 in two cases of five, 1 in one of five, else a small
    value 2..3 (the slots written start at card * 0x258 from the table);
  - the table data_800f80a4_slot0f holds random bytes over 4 * 0x258 bytes;
  - firstfile finds an entry in nine cases of ten; then nextfile finds 0 to
    14 further entries (so 1 to 15 slots are written) and then none;
  - every entry is 10 words: a name of 1 to 19 nonzero bytes, a 0, then
    random bytes up to 0x28 (a fresh entry per call);
  - the models (see the header of the .c, they stand for the library):
      firstfile(name, entry), 2 arguments: argument 0 is a pointee of 2
        words (the name, 8 bytes), argument 1 (local address) is not
        logged; copies the next entry of its table into the buffer a1
        points at and returns a1, or returns 0 when the table entry says
        not found;
      nextfile(entry), 1 argument (local address, not logged): same, with
        its own table and call counter;
      strcpy(dest, src), 2 arguments: dest is logged, src (local address)
        is not and 10 words behind it are logged; copies bytes up to and
        including the first 0, returns dest.
"""
from contracts import (A0, A1, JR_RA, T0, T2, T3, T4, T5, V0, CallLog, Contract, Setup, addiu, fill, lui, lw, ori,
                       sw)

ENTRY_WORDS = 10


def _move(rd, rs):
    return 0x00000021 | rs << 21 | rd << 11  # addu rd, rs, zero


def _sll(rd, rt, sa):
    return rt << 16 | rd << 11 | sa << 6


def _addu(rd, rs, rt):
    return 0x00000021 | rs << 21 | rt << 16 | rd << 11


def _beq(rs, rt, offset):
    return 0x10000000 | rs << 21 | rt << 16 | offset & 0xFFFF


def _bne(rs, rt, offset):
    return 0x14000000 | rs << 21 | rt << 16 | offset & 0xFFFF


def _lbu(rt, offset, base):
    return 0x90000000 | base << 21 | rt << 16 | offset & 0xFFFF


def _sb(rt, offset, base):
    return 0xA0000000 | base << 21 | rt << 16 | offset & 0xFFFF


def entry_table_tail(state, rng, count, buffer_register):
    """A tail: the n-th call copies the n-th record of a table into the
    buffer in `buffer_register` and returns its address, or returns 0 when
    the record's first word is 0. A record is a flag word and 10 entry words."""
    table = state.alloc(44 * (count + 1))
    cell = state.alloc(4)
    state.w32(cell, 0)
    for index in range(count + 1):
        base = table + 44 * index
        found = index < count
        state.w32(base, 1 if found else 0)
        name_length = rng.randrange(1, 20)
        data = bytearray(rng.getrandbits(8) for _ in range(40))
        for i in range(name_length):
            data[i] = rng.randrange(1, 256)
        data[name_length] = 0
        state.write(base + 4, bytes(data))
    return [
        lui(T0, cell), ori(T0, T0, cell), lw(T2, 0, T0), 0,
        _sll(T4, T2, 5), _sll(T5, T2, 3), _addu(T4, T4, T5), _sll(T5, T2, 2), _addu(T4, T4, T5),
        lui(T3, table), ori(T3, T3, table), _addu(T3, T3, T4),
        addiu(T2, T2, 1), sw(T2, 0, T0),
        lw(T4, 0, T3), addiu(T3, T3, 4),
        _beq(T4, 0, 10), _move(V0, 0),
        addiu(T4, T3, 4 * ENTRY_WORDS), _move(T5, buffer_register),
        lw(T2, 0, T3), addiu(T3, T3, 4), sw(T2, 0, T5), _bne(T3, T4, -4), addiu(T5, T5, 4),
        _move(V0, buffer_register), JR_RA, 0,
        JR_RA, 0,
    ]


def strcpy_tail():
    return [
        _move(T3, A0),
        _lbu(T2, 0, A1), addiu(A1, A1, 1), _sb(T2, 0, A0), _bne(T2, 0, -4), addiu(A0, A0, 1),
        _move(V0, T3), JR_RA, 0,
    ]


def setup(state, rng, sym):
    r = rng.random()
    card = 0 if r < 0.4 else 1 if r < 0.6 else rng.randrange(2, 4)
    fill(state, sym["data_800f80a4_slot0f"], 4 * 0x258, rng)
    log = CallLog(state, 4096)
    first_found = 1 if rng.random() < 0.9 else 0
    extra = rng.randrange(0, 15)
    first = entry_table_tail(state, rng, first_found, A1)
    following = entry_table_tail(state, rng, extra, A0)
    log.replace(sym["firstfile"], 2, tail=first, masks={1: 0, 0: 0}, pointees={0: 2})
    log.replace(sym["nextfile"], 1, tail=following, masks={0: 0})
    log.replace(sym["strcpy"], 2, tail=strcpy_tail(), masks={1: 0}, pointees={1: ENTRY_WORDS})
    return Setup(args=(card,), returns_value=False)


def control(words):
    """Alter the step between table slots: the `addiu reg, reg, 0x28` that
    advances the offset after each copy becomes 0x2c."""
    found = [i for i, w in enumerate(words) if w >> 26 == 9 and w & 0xFFFF == 0x28]
    if len(found) != 1:
        raise ValueError(f"expected one step by 0x28, found {len(found)}")
    i = found[0]
    return i, (words[i] & ~0xFFFF) | 0x2C, "slot step 0x28 becomes 0x2c"


CONTRACT = Contract(setup, control)
