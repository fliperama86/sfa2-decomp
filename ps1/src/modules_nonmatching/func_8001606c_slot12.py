"""Contract of func_8001606c_slot12 (see the header of the .c file).

Choices of the setup:
  - the object is 0x394 random bytes with field_03 below 8 and field_5c from
    -2 to 4 (as a halfword; the upper bits are the sign);
  - game_state.field_2bd is 0 in half of the cases, else random non-zero;
  - a1 is 1 in half of the cases, else one of 0, 2, a random 32-bit word,
    0x101 and 0x80000000 (chosen with equal probability);
  - the record table (8 records), the flag bytes, the byte table and the
    pointer table (window of -2 to 4 entries) are random;
  - the callee is a recorder with 3 arguments; the log watches the object,
    the selected record and the 8 flag bytes at every call.
"""
from contracts import CallLog, Contract, Setup, fill

REC = 0x40


def setup(state, rng, sym):
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    field_03 = rng.randrange(8)
    state.w8(obj + 3, field_03)
    state.w16(obj + 0x5C, rng.randrange(-2, 5) & 0xFFFF)
    state.w8(sym["game_state"] + 0x2BD, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    fill(state, sym["data_8002bc8c_slot12"], 8, rng)
    fill(state, sym["data_8002bc94_slot12"], 8 * REC, rng)
    fill(state, sym["data_800284ec_slot12"] - 8, 0x20, rng)
    fill(state, sym["data_80028500_slot12"] - 4, 12, rng)
    if rng.random() < 0.5:
        mode = 1
    else:
        mode = rng.choice((0, 2, rng.getrandbits(32), 1 | 0x100, 1 << 31))
    record = sym["data_8002bc94_slot12"] + REC * field_03
    log = CallLog(state, 2 * (4 + 229 + 16 + 2) + 16,
                  watch=((obj, 229), (record, REC // 4), (sym["data_8002bc8c_slot12"], 2)))
    log.replace(sym["func_80130768"], 3)
    return Setup(args=(obj, mode), returns_value=False)


def control(words):
    """Alter the second list: the 16-bit immediate 0x8454 (the low half of
    the address of entry 4, as a signed value -0x7bac) becomes 0x8455
    (-0x7bab)."""
    found = [i for i, w in enumerate(words) if w >> 26 == 9 and w & 0xFFFF == 0x8444 + 0x10]
    if len(found) != 1:
        raise ValueError(f"expected one address of the entry-4 list, found {len(found)}")
    index = found[0]
    return index, words[index] + 1, "address of the second list moved by one byte"


CONTRACT = Contract(setup, control)
