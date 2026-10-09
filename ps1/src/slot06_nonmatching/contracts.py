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
    # False for a function that does not return in the test: its run is ended by a recorder
    # (`ends_run_at` of `CallLog.replace`), in the middle of the function. The registers are then
    # not compared, because each code keeps other things in them at that point; the log and the
    # memory are compared as always.
    returns: bool = True


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

_AT, _T0, _T2, _T3, _T4, _T5, _V0, _A0, _SP = 1, 8, 10, 11, 12, 13, 2, 4, 29


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


# Copy words from t3 up to t4 to where t5 points, moving t5 along:
#   lw t2, 0(t3); addiu t3, t3, 4; sw t2, 0(t5); bne t3, t4, back to the lw; addiu t5, t5, 4 (delay slot)
_COPY = [_lw(_T2, 0, _T3), _addiu(_T3, _T3, 4), _sw(_T2, 0, _T5),
         0x14000000 | _T3 << 21 | _T4 << 16 | (-4 & 0xFFFF), _addiu(_T5, _T5, 4)]


# The encoders and register numbers, for a contract that writes a tail (see `CallLog.replace`).
lui, ori, lw, sw, addiu = _lui, _ori, _lw, _sw, _addiu
JR_RA = 0x03E00008
AT, V0, A0, A1, A2, A3, T0, T1, T2, T3, T4, T5, SP = 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 29


class CallLog:
    """A log of calls in the RAM of one case.

    `words` is the room for entries. An entry holds the callee's address,
    then each recorded argument, then the words behind the pointer arguments
    named for that callee, then the words of every watched block. The first
    word of the log's block points at the next free entry. A run that makes
    more calls than the room holds writes past the block: give the log room
    for the longest run.

    `watch` names memory as (address, words) that every recorder copies into
    its entry at every call: what the function under test has written there
    by the time of the call. A recorder returns at once, so without this a
    function that writes a field after the call instead of before it, where
    the real callee would have read it, ends in the same state and passes.
    Watch what the function writes and a callee could read: the objects it
    is given, the globals it sets.
    """

    def __init__(self, state, words: int = 1024, watch: tuple[tuple[int, int], ...] = ()):
        for address, count in watch:
            if count < 1 or address % 4:
                raise ValueError(f"a watched block needs a word-aligned address and at least one word, not {address:#x}, {count}")
        self.state = state
        self.watch = tuple(watch)
        self.cursor = state.alloc(4 + 4 * words)
        self.entries = self.cursor + 4
        state.w32(self.cursor, self.entries)

    def replace(self, address: int, arguments: int, result: int = 0, pointees: dict[int, int] | None = None,
                masks: dict[int, int] | None = None, results: tuple[int, ...] = (), ends_run_at: int = 0,
                stores: tuple[tuple[int, int, int], ...] = (), counts: tuple[int, ...] = (),
                tail: tuple[int, ...] | None = None) -> None:
        """Put a recorder in place of the function at `address`.

        `arguments` is how many arguments the callee takes (the fifth and
        later ones are read from the caller's stack, as the calling
        convention places them); `result` is what the recorder returns in v0.

        `masks` names arguments of which the callee uses only some low bits,
        as {argument index: mask} with a mask of at most 16 bits: the
        recorder logs the argument under the mask. A mask of 0 logs nothing
        of the argument's value: for the address of a local of the function
        under test, which the two codes place differently (give such an
        argument a pointee when what it points at matters). Use it where the callee
        takes a byte or a halfword and the two codes extend it differently
        in the bits the callee does not read.

        `results` gives a result for each call in turn; after the last one
        that value is returned for every further call. It is for a function
        that loops until a callee says something else. With `results`,
        `result` is not used.

        `ends_run_at` N ends the run at the N-th call of this recorder, after
        it is logged: for a function that never returns. The contract's
        setup then returns `Setup(..., returns=False)`.

        `tail` is machine code of the contract's own that stands for what
        the callee does to the caller's memory and to `v0`, for a callee
        that the fixed recorder cannot stand for (one that fills a buffer
        the caller hands it and returns that buffer's address, say). It
        runs after the entry is written, in place of the recorder's own
        return, with the argument registers and `sp` as the caller left
        them, and must end the call itself (`jr ra` and its delay slot).
        Build it with the encoders of this module (`lui`, `ori`, `lw`,
        `sw`, `addiu`, `JR_RA`) and keep to the registers `t0` to `t5`,
        `at` and `v0`. A tail is a model written by the contract's author:
        the header of the `.c` says what it models and from what that is
        known. With a tail, `result` and `results` are not used.

        `stores` and `counts` stand for what an interrupt does to memory
        while the function runs, which nothing else in a test can do:
        `stores` is ((N, address, value), ...), a word stored at the N-th
        call of this recorder; `counts` names words that every call of this
        recorder adds 1 to. Both happen after the call is logged, so the
        entry shows memory as it was when the call was made.

        `pointees` names arguments that are pointers to something the caller
        filled in for this call, as {argument index: words}: the recorder
        copies that many words from where the argument points into the entry.
        The pointer must be word aligned and valid in every call.
        """
        pointees = dict(sorted((pointees or {}).items()))
        for index, count in pointees.items():
            if not 0 <= index < arguments or count < 1:
                raise ValueError(f"argument {index} of {arguments} cannot have a pointee of {count} words")
        masks = masks or {}
        for index, mask in masks.items():
            if not 0 <= index < arguments or not 0 <= mask <= 0xFFFF:
                raise ValueError(f"argument {index} of {arguments} cannot have the mask {mask:#x}")
        if ends_run_at < 0:
            raise ValueError("ends_run_at counts calls from 1")
        for number, target, _ in stores:
            if number < 1 or target % 4:
                raise ValueError(f"a store needs a call number from 1 and a word-aligned address, not {number}, {target:#x}")
        for target in counts:
            if target % 4:
                raise ValueError(f"a counted word needs a word-aligned address, not {target:#x}")
        code = [_lui(_T0, self.cursor), _ori(_T0, _T0, self.cursor), _lw(_T5, 0, _T0),
                _lui(_T2, address), _ori(_T2, _T2, address), _sw(_T2, 0, _T5)]
        for index in range(arguments):
            code += self._argument(index, _T2)
            if index in masks:
                code.append(0x30000000 | _T2 << 21 | _T2 << 16 | masks[index])  # andi t2, t2, mask
            code.append(_sw(_T2, 4 + 4 * index, _T5))
        code.append(_addiu(_T5, _T5, 4 * (1 + arguments)))
        for index, count in pointees.items():
            code += self._argument(index, _T3)
            code.append(_addiu(_T4, _T3, 4 * count))
            code += _COPY
        for block, count in self.watch:
            end = block + 4 * count
            code += [_lui(_T3, block), _ori(_T3, _T3, block), _lui(_T4, end), _ori(_T4, _T4, end)]
            code += _COPY
        code.append(_sw(_T5, 0, _T0))
        for target in counts:
            code += [_lui(_T3, target), _ori(_T3, _T3, target), _lw(_T2, 0, _T3), 0, _addiu(_T2, _T2, 1), _sw(_T2, 0, _T3)]
        if ends_run_at or stores:
            # A cell counts this recorder's calls; t2 holds the number of this call from here on.
            cell = self.state.alloc(4)
            code += [_lui(_T3, cell), _ori(_T3, _T3, cell), _lw(_T2, 0, _T3), 0, _addiu(_T2, _T2, 1), _sw(_T2, 0, _T3)]
            for number, target, value in stores:
                code += [_addiu(_T4, 0, number),
                         0x14000000 | _T2 << 21 | _T4 << 16 | 5,  # bne t2, t4, past this store
                         _lui(_T3, target), _ori(_T3, _T3, target), _lui(_T4, value), _ori(_T4, _T4, value),
                         _sw(_T4, 0, _T3)]
            if ends_run_at:
                # At the N-th call the run jumps to where a returning function would end.
                code += [_addiu(_T4, 0, ends_run_at),
                         0x14000000 | _T2 << 21 | _T4 << 16 | 2,  # bne t2, t4, past the jump
                         0, 0x08000000 | (self.state.stop >> 2) & 0x03FFFFFF, 0]  # nop; j stop; nop
        if tail is not None:
            code += list(tail)
        elif results:
            # A block holds the index of the next result, the last index, and the results.
            table = self.state.alloc(8 + 4 * len(results))
            self.state.w32(table + 4, len(results) - 1)
            for index, value in enumerate(results):
                self.state.w32(table + 8 + 4 * index, value)
            code += [_lui(_T3, table), _ori(_T3, _T3, table), _lw(_T2, 0, _T3), _lw(_T4, 4, _T3),
                     0x00000080 | _T2 << 16 | _AT << 11,  # sll at, t2, 2
                     0x00000021 | _AT << 21 | _T3 << 16 | _AT << 11,  # addu at, at, t3
                     _lw(_V0, 8, _AT),
                     0x0000002B | _T2 << 21 | _T4 << 16 | _AT << 11,  # sltu at, t2, t4
                     0x00000021 | _T2 << 21 | _AT << 16 | _T2 << 11,  # addu t2, t2, at
                     _sw(_T2, 0, _T3), 0x03E00008, 0]  # jr ra; nop
        else:
            code += [_lui(_V0, result), 0x03E00008, _ori(_V0, _V0, result)]  # jr ra; the last one is its delay slot
        routine = self.state.alloc(4 * len(code))
        for index, word in enumerate(code):
            self.state.w32(routine + 4 * index, word)
        # j routine; nop
        self.state.w32(address, 0x08000000 | (routine >> 2) & 0x03FFFFFF)
        self.state.w32(address + 4, 0)

    @staticmethod
    def _argument(index: int, register: int) -> list[int]:
        """Code that puts argument `index` of the call into `register`."""
        if index < 4:
            return [0x00000021 | (_A0 + index) << 21 | register << 11]  # move register, argument
        return [_lw(register, 16 + 4 * (index - 4), _SP), 0]


def halfword(rng) -> int:
    return rng.getrandbits(16)


def fill(state, address: int, size: int, rng) -> None:
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
    state.w16(game_state + 0x92, halfword(rng))
    for layer in ("data_801aa5d4", "data_801aa544", "cam_obj"):
        state.w16(sym[layer] + 0x12, halfword(rng))
        state.w16(sym[layer] + 0x16, halfword(rng))

    if rng.random() < 0.8:
        columns, rows = rng.randrange(1, 9), rng.randrange(1, 7)
    else:
        columns, rows = rng.randrange(0, 9), rng.randrange(0, 7)
    header = state.alloc(8 + 2 * rows * columns + 2)
    fill(state, header, 8 + 2 * rows * columns, rng)
    state.w8(header + 0, columns)
    state.w8(header + 2, rows)
    written = 0
    for index in range(rows * columns):
        cell = 0 if rng.random() < 0.1 else halfword(rng)
        state.w16(header + 8 + 2 * index, cell)
        if cell & 0x3FFF:
            written += 1

    step = state.alloc(12)
    fill(state, step, 12, rng)
    state.w32(step + 4, header)

    lists = state.alloc(4 * 16)
    fill(state, lists, 4 * 16, rng)
    state.w32(sym["data_801987c8"], lists)

    obj = state.alloc(0x394)
    fill(state, obj, 0x394, rng)
    state.w8(obj + 0x09, rng.randrange(16))
    state.w8(obj + 0x0E, rng.choice((8, 4, 0xC, rng.randrange(256))))
    state.w8(obj + 0x0F, rng.choice((0, rng.randrange(256))))
    state.w32(obj + 0x18, step)

    state.w16(sym["data_801adfe4"], rng.randrange(0, 85 - written + 1))
    state.w8(sym["data_801a27d0"], rng.randrange(2))
    fill(state, sym["data_801f3050_slot06_00"], 2 * 85 * 28, rng)
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
