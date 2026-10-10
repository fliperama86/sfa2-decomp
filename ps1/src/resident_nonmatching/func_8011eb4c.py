"""Contract of func_8011eb4c: no argument, no result.

Choices made here:
  - every region the function writes or follows is filled with random
    bytes before the call: the three record arrays, the three pointer
    tables, the pool entries and their pointer tables, the counters and
    ref_third itself (a random pointer value, which the function replaces
    before using);
  - the recorder copies, at the call, every region of the list above (the
    one-byte and two-byte ones as the word that holds them);
  - func_8011ef34 is a recorder with no argument, returning 0.
"""

from contracts import CallLog, Contract, Setup

# (name, size in bytes) of the regions to randomise
REGIONS = (
    ("units_2c20", 16 * 0xC0),
    ("data_801a89f4", 40 * 0xAC),
    ("data_801ac888", 16 * 0xAC),
    ("data_801a89b0", 16 * 4),
    ("table_80197f20", 40 * 4),
    ("data_801a68f0", 16 * 4),
    ("data_8018e598", 256 * 16),
    ("data_8018f5e4", 16 * 16),
    ("data_801abf10", 256 * 4),
    ("data_801ac30c", 16 * 4),
    ("data_801ad358", 16 * 4),
    ("data_801a6960", 1),
    ("data_80197f10", 1),
    ("data_801a4fec", 1),
    ("data_801ac618", 2),
    ("data_801abefc", 2),
    ("data_801ad348", 2),
    ("data_801a4fe4", 4),
    ("data_801a6980", 4),
    ("ref_third", 4),
    ("data_8018db14", 1),
    ("data_801a27d4", 1),
    ("data_801a27cc", 1),
)


def setup(state, rng, sym) -> Setup:
    for name, size in REGIONS:
        state.write(sym[name], rng.randbytes(size))
    watch = tuple((sym[name], (size + 3) // 4) for name, size in REGIONS if name != "ref_third")
    watch += ((sym["ref_third"], 1),)
    log = CallLog(state, words=8192, watch=watch)
    log.replace(sym["func_8011ef34"], 0, 0)
    return Setup(args=(), returns_value=False)


def control(words):
    """Alter the type byte of the third array: 0x10 becomes 0x11."""
    found = [i for i, w in enumerate(words) if w >> 26 == 0x0D and w & 0xFFFF == 0x10 and (w >> 21) & 31 == 0]
    if len(found) != 1:
        raise ValueError(f"expected one load of 0x10, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0x11, "type byte of the third array is 0x11"


CONTRACT = Contract(setup, control)
