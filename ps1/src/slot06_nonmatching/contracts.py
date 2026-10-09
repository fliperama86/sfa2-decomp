"""Input contracts of the nonmatching functions, one setup function each.

A setup function builds a random valid input state for one function. It is
given the state to write into (`State` of difftest.py: byte, half and word
accessors on the RAM and the scratchpad, plus `alloc` for blocks in free
RAM), a seeded `random.Random`, and `sym`, the addresses of the names of the
tree's linker symbols and of the functions that the build declares. It returns a `Setup`: the argument registers and
whether the function returns a value in v0.

A contract keeps every pointer inside memory that the setup allocated and
every loop bounded, so that the original code runs to its end. A setup that
cannot keep this promise is a bug of the setup; the test discards a case only
when the ORIGINAL code faults or exceeds its budget.

`CONTRACTS` maps a function name to its `Contract`. `control` of a contract
names one instruction of the nonmatching build that the setup's inputs make
the function execute, and says how to alter it for the negative control.
A contract may also be the value `CONTRACT` of a file `FUNC.py` beside
`FUNC.c`; such a file imports `Contract`, `Setup` and `CallLog` from here.
"""

from __future__ import annotations

import dataclasses
from typing import Callable


@dataclasses.dataclass(frozen=True)
class Setup:
    """The call made for one case."""

    args: tuple[int, ...]  # values for a0, a1, a2, a3
    returns_value: bool  # whether v0 is part of the comparison


@dataclasses.dataclass(frozen=True)
class Contract:
    setup: Callable
    control: Callable  # (words) -> (index of the word, altered word, description)


# ---------------------------------------------------------------------------
# Recorders for callees
#
# A function under test may call a function that cannot run in the test (it
# reaches the library and through it the hardware) or whose inner state the
# contract does not want to set up. `CallLog.replace` puts a recorder in the
# callee's place, in the state of the case, so the original and the build both
# call the recorder: it appends the callee's address and its arguments to a
# log in RAM and returns a value that the setup chose. The log is part of the
# compared RAM: a call that is missing, out of order or made with another
# argument is a difference. Only the arguments that the callee is declared to
# take are recorded; the other argument registers hold leftovers that the two
# codes need not share.

_T0, _T1, _T2, _V0, _A0, _SP = 8, 9, 10, 2, 4, 29


def _lui(rt: int, value: int) -> int:
    return 0x3C000000 | rt << 16 | (value >> 16) & 0xFFFF


def _ori(rt: int, rs: int, value: int) -> int:
    return 0x34000000 | rs << 21 | rt << 16 | value & 0xFFFF


def _lw(rt: int, offset: int, base: int) -> int:
    return 0x8C000000 | base << 21 | rt << 16 | offset & 0xFFFF


def _sw(rt: int, offset: int, base: int) -> int:
    return 0xAC000000 | base << 21 | rt << 16 | offset & 0xFFFF


def _addiu(rt: int, rs: int, value: int) -> int:
    return 0x24000000 | rs << 21 | rt << 16 | value & 0xFFFF


class CallLog:
    """A log of calls in the RAM of one case.

    `words` is the room for entries; an entry takes one word for the callee's
    address and one for each recorded argument. The first word of the block
    points at the next free entry. A run that makes more calls than the room
    holds writes past the block: give the log room for the longest run.
    """

    def __init__(self, state, words: int = 1024):
        self.state = state
        self.cursor = state.alloc(4 + 4 * words)
        self.entries = self.cursor + 4
        state.w32(self.cursor, self.entries)

    def replace(self, address: int, arguments: int, result: int = 0) -> None:
        """Put a recorder in place of the function at `address`.

        `arguments` is how many arguments the callee takes (the fifth and
        later ones are read from the caller's stack, as the calling
        convention places them); `result` is what the recorder returns in v0.
        """
        code = [_lui(_T0, self.cursor), _ori(_T0, _T0, self.cursor), _lw(_T1, 0, _T0),
                _lui(_T2, address), _ori(_T2, _T2, address), _sw(_T2, 0, _T1)]
        for index in range(arguments):
            if index < 4:
                code.append(_sw(_A0 + index, 4 + 4 * index, _T1))
            else:
                code += [_lw(_T2, 16 + 4 * (index - 4), _SP), 0, _sw(_T2, 4 + 4 * index, _T1)]
        code += [_addiu(_T1, _T1, 4 * (1 + arguments)), _sw(_T1, 0, _T0),
                 _lui(_V0, result), 0x03E00008, _ori(_V0, _V0, result)]  # jr ra; the last one is its delay slot
        routine = self.state.alloc(4 * len(code))
        for index, word in enumerate(code):
            self.state.w32(routine + 4 * index, word)
        # j routine; nop
        self.state.w32(address, 0x08000000 | (routine >> 2) & 0x03FFFFFF)
        self.state.w32(address + 4, 0)


def _halfword(rng) -> int:
    return rng.getrandbits(16)


def _fill(state, address: int, size: int, rng) -> None:
    """Fill a block with random bytes."""
    state.write(address, bytes(rng.getrandbits(8) for _ in range(size)))


def setup_func_801e9080_slot06_00(state, rng, sym) -> Setup:
    """a0 is an object that selects a layer, and a grid of cells to draw.

    Reads and writes are listed in the header comment of
    func_801e9080_slot06_00.c. Choices made here:
      - game_state.field_64 is 0 in nine cases of ten and random otherwise
        (the function returns at once when it is not 0);
      - the layer blocks hold random field_12 and field_16;
      - the grid has 1 to 8 columns and 1 to 6 rows (the loops are bounded
        by 48 cells) in four cases of five; otherwise columns or rows may be
        0, so that the loop entries are tried;
      - about a tenth of the cells are 0 (skipped), the others random;
      - the counter is chosen so that counter plus written records is at
        most 85, the selector is 0 or 1;
      - the record table is filled with random bytes, so that the words the
        function reads and rewrites and the bytes it leaves alone are tested;
      - the list array has 16 words of random content, field_09 indexes it.
    """
    game_state = sym["game_state"]
    state.w8(game_state + 0x64, 0 if rng.random() < 0.9 else rng.randrange(1, 256))
    state.w16(game_state + 0x92, _halfword(rng))
    for layer in ("data_801aa5d4", "data_801aa544", "cam_obj"):
        state.w16(sym[layer] + 0x12, _halfword(rng))
        state.w16(sym[layer] + 0x16, _halfword(rng))

    if rng.random() < 0.8:
        columns, rows = rng.randrange(1, 9), rng.randrange(1, 7)
    else:
        columns, rows = rng.randrange(0, 9), rng.randrange(0, 7)
    header = state.alloc(8 + 2 * rows * columns + 2)
    _fill(state, header, 8 + 2 * rows * columns, rng)
    state.w8(header + 0, columns)
    state.w8(header + 2, rows)
    written = 0
    for index in range(rows * columns):
        cell = 0 if rng.random() < 0.1 else _halfword(rng)
        state.w16(header + 8 + 2 * index, cell)
        if cell & 0x3FFF:
            written += 1

    step = state.alloc(12)
    _fill(state, step, 12, rng)
    state.w32(step + 4, header)

    lists = state.alloc(4 * 16)
    _fill(state, lists, 4 * 16, rng)
    state.w32(sym["data_801987c8"], lists)

    obj = state.alloc(0x394)
    _fill(state, obj, 0x394, rng)
    state.w8(obj + 0x09, rng.randrange(16))
    state.w8(obj + 0x0E, rng.choice((8, 4, 0xC, rng.randrange(256))))
    state.w8(obj + 0x0F, rng.choice((0, rng.randrange(256))))
    state.w32(obj + 0x18, step)

    state.w16(sym["data_801adfe4"], rng.randrange(0, 85 - written + 1))
    state.w8(sym["data_801a27d0"], rng.randrange(2))
    _fill(state, sym["data_801f3050_slot06_00"], 2 * 85 * 28, rng)
    return Setup(args=(obj,), returns_value=False)


def control_func_801e9080_slot06_00(words):
    """Alter the store of the record counter: its offset moves by two bytes.

    The store is `sh reg, -0x201c(at)`, the one halfword store to the
    counter's address that every call with game_state.field_64 equal to 0
    and at least one drawn cell makes; the counter then keeps its old value.
    """
    found = [i for i, w in enumerate(words) if w >> 26 == 0x29 and w & 0xFFFF == 0xDFE4]
    if len(found) != 1:
        raise ValueError(f"expected one store of the record counter, found {len(found)}")
    index = found[0]
    return index, (words[index] & ~0xFFFF) | 0xDFE6, "record counter store moved by two bytes"


CONTRACTS = {
    "func_801e9080_slot06_00": Contract(setup_func_801e9080_slot06_00, control_func_801e9080_slot06_00),
}
