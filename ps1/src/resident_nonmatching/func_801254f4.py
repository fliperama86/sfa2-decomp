"""Contract of func_801254f4: no arguments, no return value.

Choices of the setup (reads and writes are listed in func_801254f4.c):
  - game_state.field_78 points at a random object block whose side byte is 0
    in one case of two;
  - the code word data_801a6966 and data_801a6972 each get a low byte
    chosen among the known values (0x94, 0x90, 0x68, 0x60, 0xc0, 0x30, 6, 9)
    in three cases of four, 0 in one case of twenty, else random, and a
    random top byte (the nibble is the top four bits);
  - the signed byte table data_8016e9a8 (16 entries) is random, with about
    a quarter of the entries negative;
  - game_state.field_138, field_139, field_13a, data_80185fc8..fd4 start
    random, so that what the function leaves alone and what it rewrites
    are both tested.
"""
from contracts import Contract, Setup, fill

KNOWN = (0x94, 0x90, 0x68, 0x60, 0xC0, 0x30, 6, 9)


def setup(state, rng, sym):
    gs = sym["game_state"]
    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0xA6, 0 if rng.random() < 0.5 else rng.randrange(1, 256))
    state.w32(gs + 0x78, obj)
    for name in ("data_801a6966", "data_801a6972"):
        r = rng.random()
        low = rng.choice(KNOWN) if r < 0.75 else (0 if r < 0.8 else rng.getrandbits(8))
        state.w16(sym[name], (rng.getrandbits(8) << 8) | low)
    table = sym["data_8016e9a8"]
    for i in range(16):
        state.w8(table + i, rng.randrange(128, 256) if rng.random() < 0.25 else rng.randrange(0, 128))
    for off in (0x138, 0x139, 0x13A):
        state.w8(gs + off, rng.getrandbits(8))
    for name in ("data_80185fc8", "data_80185fcc", "data_80185fd0", "data_80185fd4"):
        state.w32(sym[name], rng.getrandbits(32))
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the store of field_139 on the failure path: 0x3c becomes 0x3d."""
    found = [i for i, w in enumerate(words) if w == 0x3402003C]
    if len(found) < 1:
        raise ValueError("expected a load of 0x3c into v0")
    i = found[0]
    return i, 0x3402003D, "failure value 0x3c changed to 0x3d"


CONTRACT = Contract(setup, control)
