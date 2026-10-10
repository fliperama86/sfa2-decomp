"""Contract of func_801347b4, as code. Header comment of the .c has the text.

Choices of the setup:
  - the map has 0 to 12 columns and 0 to 12 rows (rows are at least 1 when
    the kind is 2 or 3: see the exclusion in the .c); about one cell in six
    is 0 (skipped), one in six has tile 0x3b or 0x3e, the others are
    random; the top two bits of a cell are random;
  - the origin halfwords and the texture page offset of the header are
    random;
  - the kind a is 0 to 7 in four cases of five (with random bits above
    the low byte in a quarter of them), otherwise random; c and d are
    random words; e and f are random words on the stack (the code reads
    their low halfword and low byte);
  - the buffer selector word has 0, 1 or 0xffff in its low halfword and
    random bits above it;
  - the record counter is random from 0 to 144 minus the number of tiles
    to be drawn;
  - the variables that func_80134e94 and func_801350c0 read (the function
    itself names none of them except data_80188d28; the callees take them by
    address, inferred: a stretch of data from data_80188d28 - 0x20, the byte
    table at data_80171bf8 + 0x60 (0x108 bytes), and the halfword at offset
    0xc6 and the byte at 0xd8 of each of the two player objects, player_left
    and the object 0x394 bytes after it, which is player_right) are set at
    random: the stretch is 0x38 bytes, each random in one case of two, else
    from 0 to 11; the byte table is random; the halfword at 0xc6 is, in 60
    cases of 100, from low - 8 to low + count + 7 with (low, count) =
    (0x60, 0x30) or (0x30, 0x30), one case of two each, else random; the
    byte at 0xd8 is 0 in two cases of three, else random;
  - func_8015bf34 is a recorder (2 arguments, returns 0); the record it is
    given is a pointee (10 words); every recorder copies the words of
    data_80188d04 and data_80188d28 and the first 48 records of the run
    (the record table and the 5760 bytes before it hold random bytes).
"""

from contracts import CallLog, Contract, Setup

STACK_TOP = 0x801FF000  # sp at entry (the tool's constant); the 5th and 6th arguments sit at sp + 0x10 and + 0x14


def _near_range(rng, low, count):
    """A halfword from low - 8 to low + count + 7 in 60 cases of 100, else random."""
    if rng.random() < 0.6:
        return low + rng.randrange(-8, count + 8) & 0xFFFF
    return rng.getrandbits(16)


def setup(state, rng, sym):
    kind = rng.randrange(8)
    if rng.random() < 0.2:
        kind = rng.getrandbits(32)
    elif rng.random() < 0.25:
        kind |= rng.getrandbits(24) << 8
    low = kind & 0xFF
    cols = rng.randrange(0, 13)
    rows = rng.randrange(0, 13)
    if rows == 0 and low in (2, 3):
        rows = rng.randrange(1, 13)
    header = state.alloc(8 + 2 * rows * cols + 2)
    state.write(header, bytes(rng.getrandbits(8) for _ in range(8 + 2 * rows * cols)))
    state.w8(header + 0, cols)
    state.w8(header + 2, rows)
    tiles = 0
    for index in range(rows * cols):
        roll = rng.random()
        if roll < 1 / 6:
            tile = 0
        elif roll < 2 / 6:
            tile = rng.choice((0x3B, 0x3E))
        else:
            tile = rng.getrandbits(14)
        state.w16(header + 8 + 2 * index, (rng.getrandbits(2) << 14) | tile)
        if tile:
            tiles += 1

    state.w32(sym["data_801987c8"], state.alloc(0x40))
    selector = rng.choice((0, 1, 0, 1, 0xFFFF))
    state.w32(sym["data_801a27d0"], (rng.getrandbits(16) << 16) | selector)
    count = rng.randrange(0, 144 - tiles + 1)
    state.w16(sym["data_80188d04"], count)

    # The record table (and the buffer before it, for selector -1) holds random bytes;
    # every recorder copies the data_80188d04 and data_80188d28 words and the first 48
    # records this call may write (counter on); the record handed over is a pointee.
    table = sym["data_80186004"]
    state.write(table - 5760, bytes(rng.getrandbits(8) for _ in range(3 * 5760)))
    first = table + (selector - (0x10000 if selector == 0xFFFF else 0)) * 5760 + count * 40
    watched = min(tiles, 48) * 10
    words = 64 + tiles * (16 + watched + 2 + 1)
    log = CallLog(state, words, watch=((sym["data_80188d04"], 1), (sym["data_80188d28"], 1), (first, watched or 1)))
    log.replace(sym["func_8015bf34"], 2, 0, pointees={1: 10})

    # What func_80134e94 and func_801350c0 read.
    var = sym["data_80188d28"] - 0x20
    state.write(var, bytes(rng.getrandbits(8) if rng.random() < 0.5 else rng.randrange(0, 12) for _ in range(0x38)))
    table = sym["data_80171bf8"] + 0x60
    state.write(table, bytes(rng.getrandbits(8) for _ in range(0x108)))
    for player in (sym["player_left"], sym["player_left"] + 0x394):
        state.w16(player + 0xC6, _near_range(rng, 0x60, 0x30) if rng.random() < 0.5 else _near_range(rng, 0x30, 0x30))
        state.w8(player + 0xD8, rng.choice((0, 0, rng.getrandbits(8))))

    state.w32(STACK_TOP + 0x10, rng.getrandbits(32))
    state.w32(STACK_TOP + 0x14, rng.getrandbits(32))
    return Setup(args=(kind, header, rng.getrandbits(32), rng.getrandbits(32)), returns_value=False)


def control(words):
    """Change the constant 0x7807 of the page word for kinds 0 to 3."""
    found = [i for i, w in enumerate(words) if (w >> 26) == 0x09 and w & 0xFFFF == 0x7807]
    if len(found) != 1:
        raise ValueError(f"expected one addiu of 0x7807, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x7809, "page word constant 0x7807 changed to 0x7809"


CONTRACT = Contract(setup, control)
