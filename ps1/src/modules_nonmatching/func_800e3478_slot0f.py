"""Contract of func_800e3478_slot0f (reads and writes: see the header of the .c).

Choices of the setup:
  - the option block (16 bytes at data_8016e685) is random; the byte
    data_8016e688 is 0 in half of the cases; field_02 equals one of the five
    table bytes in three cases of four (the table data_800df32c_slot0f is
    rewritten with random bytes, with repeats allowed), else it is random;
  - field_00, field_05 and field_07 (the loop limits) are random signed
    bytes from -3 to 10 in three cases of four, else any byte;
  - the previous data_800f8554_slot0f, the cells' other bytes, the copied
    table and the pointer words start random;
  - func_80120374 is a recorder with 1 argument returning 0; the log copies
    data_800f8554_slot0f and data_800f8564_slot0f (8 words), the 20 cells
    (80 words) and the six pointer words (21 words).
"""

from contracts import CallLog, Contract, Setup


def fill(state, address, size, rng):
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup(state, rng, sym) -> Setup:
    options = sym["data_8016e685"]
    fill(state, options, 12, rng)
    for offset in (0, 5, 7):
        if rng.random() < 0.75:
            state.w8(options + offset, rng.randrange(-3, 11))
    state.w8(sym["data_8016e688"], 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    table = sym["data_800df32c_slot0f"]
    fill(state, table, 5, rng)
    if rng.random() < 0.75:
        state.w8(options + 2, state.read(table + rng.randrange(5), 1)[0])
    fill(state, sym["data_800f8554_slot0f"], 0x20, rng)
    fill(state, sym["table_8016e664"], 16, rng)
    fill(state, sym["data_800e953c_slot0f"], 0x140, rng)
    fill(state, sym["data_800e9688_slot0f"], 0x54, rng)

    log = CallLog(state, 512, watch=((sym["data_800f8554_slot0f"], 8), (sym["data_800e953c_slot0f"], 80),
                                     (sym["data_800e9688_slot0f"], 21)))
    log.replace(sym["func_80120374"], 1)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the first threshold: `ori a2,zero,0x16` becomes 0x17."""
    found = [i for i, w in enumerate(words) if w >> 26 == 13 and w & 0xFFFF == 0x16]
    if not found:
        raise ValueError("expected the constant 0x16")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x17, "cell value 0x16 changed to 0x17"


CONTRACT = Contract(setup, control)
