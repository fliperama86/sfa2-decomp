#!/usr/bin/env python3
"""Controls for funcscan.py on synthetic code.

Each case builds a few MIPS words by hand, runs the sweep and requires the
functions and the runs set aside as data that the construction implies.
"""

from __future__ import annotations

import struct
import subprocess
import sys
import tempfile
from pathlib import Path

import funcscan

TOOL = Path(__file__).resolve().parent / "funcscan.py"
BASE = 0x80200000
OPEN, CLOSE, RETURN, NOP = 0x27BDFFE8, 0x27BD0018, 0x03E00008, 0
ONE, TWO = 0x24020001, 0x24020002  # li v0,1 and li v0,2
TEXT = struct.unpack("<3I", b"NAMEPLAYZONE")  # no word of it is an instruction: see is-instruction-rejects-text


def jal(index: int) -> int:
    return 0x0C000000 | ((BASE + 4 * index) >> 2) & 0x03FFFFFF


def jump(index: int) -> int:
    return 0x08000000 | ((BASE + 4 * index) >> 2) & 0x03FFFFFF


def always(to: int, at: int) -> int:
    """`beq zero,zero` at word `at` going to word `to`."""
    return 0x10000000 | (to - at - 1) & 0xFFFF


def sweep(words, entries=(), low=0, high=None, base=BASE):
    data: list = []
    high = len(words) if high is None else high
    found = funcscan.scan(list(words), base, low, high, funcscan.reader(list(words), base), entries, data)
    relative = lambda rows: [((a - base) // 4, s // 4) for a, s in rows]  # noqa: E731
    return relative(found), relative(data)


def checks(words, entries=(), low=0, high=None):
    """The four counts of funcscan.census for a sweep of these words, in the order of funcscan.CHECKS."""
    high = len(words) if high is None else high
    found = funcscan.scan(list(words), BASE, low, high, funcscan.reader(list(words), BASE), entries)
    counts = funcscan.census(list(words), BASE, low, high, found)
    return [counts[key] for key, _ in funcscan.CHECKS]


PLAIN = [OPEN, NOP, RETURN, CLOSE]
LUI_AT, LW_V0, JR_V0, ADD_AT = 0x3C010000, 0x8C220000, 0x00400008, 0x00220821  # ADD_AT is addu at,at,v0


def switched(entries, upper_gap=(ADD_AT,), load_gap=(NOP,), base=BASE):
    """A function that jumps through a table, a second function, and the table: the functions the sweep finds.

    The function reads the table with `lui at` and `lw v0,lo(at)` and has two
    cases that only the table reaches. `upper_gap` are the words between the
    `lui` and the `lw`, `load_gap` those between the `lw` and the `jr`.
    `entries` gets the word indexes of the two cases and of the end of the
    range and returns the table. Returns what the sweep found, what it finds
    when it takes the two cases, and what it finds when it takes none.
    """
    head = [OPEN, LUI_AT, *upper_gap, LW_V0, *load_gap, JR_V0, NOP]
    one, two = len(head), len(head) + 3
    code = head + [ONE, RETURN, NOP, TWO, RETURN, CLOSE] + PLAIN + [NOP]
    address = base + 4 * len(code)
    code[1] |= ((address + 0x8000) >> 16) & 0xFFFF
    code[2 + len(upper_gap)] |= address & 0xFFFF
    table = entries(one, two, len(code) + 8)
    code += table + [NOP] * (8 - len(table))
    return sweep(code, base=base)[0], [(0, two + 3), (two + 3, 4)], [(0, one + 3), (two, 3), (two + 3, 4)]


def sweep_cases():
    plain = [OPEN, NOP, RETURN, CLOSE]
    yield "two-functions", sweep(plain + plain), ([(0, 4), (4, 4)], [])
    yield "padding-between", sweep(plain + [NOP, NOP] + plain), ([(0, 4), (6, 4)], [])

    # A return in the middle: the branch at word 1 goes past it, so the function goes on.
    early = [OPEN, always(6, 1), NOP, RETURN, NOP, ONE, TWO, RETURN, CLOSE]
    yield "return-before-a-branch-target", sweep(early + plain), ([(0, 9), (9, 4)], [])
    # Without the branch the same words are two functions.
    yield "return-without-a-pending-target", sweep([OPEN, NOP, NOP, RETURN, NOP, ONE, TWO, RETURN, CLOSE]), (
        [(0, 5), (5, 4)],
        [],
    )
    # A forward `j` inside a function is a branch too.
    yield "jump-forward-inside", sweep([OPEN, jump(6), NOP, RETURN, NOP, ONE, TWO, RETURN, CLOSE]), ([(0, 9)], [])
    # A `j` that goes elsewhere does not end a function: nothing in the resident code ends that way.
    yield "jump-away-is-not-an-end", sweep([OPEN, jump(0x4000), NOP, ONE, RETURN, CLOSE]), ([(0, 6)], [])

    # The end is the first return at or beyond every target. A target on the return itself ends the
    # function there; a target on its delay slot does not.
    tight = lambda to: [OPEN, always(to, 1), NOP, RETURN, NOP, ONE, RETURN, CLOSE]  # noqa: E731
    yield "target-on-the-return", sweep(tight(3)), ([(0, 5), (5, 3)], [])
    yield "target-on-the-delay-slot", sweep(tight(4)), ([(0, 8)], [])
    # The farthest target counts, not the last one seen: of two branches, of a branch and a `j`.
    for name, second in (("branch", always(5, 3)), ("jump", jump(5))):
        both = [OPEN, always(8, 1), NOP, second, NOP, RETURN, NOP, ONE, RETURN, CLOSE]
        yield f"nearer-{name}-after-a-farther-one", sweep(both), ([(0, 10)], [])
    # Every conditional branch of the processor carries a function past an early return.
    for name, word in (
        ("beq", 0x10000000), ("bne", 0x14000000), ("blez", 0x18000000), ("bgtz", 0x1C000000),
        ("bltz", 0x04000000), ("bgez", 0x04010000), ("bltzal", 0x04100000), ("bgezal", 0x04110000),
    ):  # fmt: skip
        through = [OPEN, word | 4, NOP, RETURN, NOP, ONE, TWO, RETURN, CLOSE]
        yield f"branch-{name}", sweep(through + plain), ([(0, 9), (9, 4)], [])
    # A coprocessor branch (`bc2f` here) is an instruction and is not followed.
    yield "coprocessor-branch-not-followed", sweep([OPEN, 0x49000004, NOP, RETURN, NOP, ONE, TWO, RETURN, CLOSE]), (
        [(0, 5), (5, 4)],
        [],
    )
    # A target at the end of the swept range or beyond is ignored: the function still ends at its return.
    yield "branch-to-the-end-of-the-range", sweep([OPEN, always(5, 1), NOP, RETURN, CLOSE]), ([(0, 5)], [])
    yield "jump-to-the-end-of-the-range", sweep([OPEN, jump(5), NOP, RETURN, CLOSE]), ([(0, 5)], [])
    yield "branch-target-forward", funcscan.branch_target(always(9, 5), 5), 9
    yield "branch-target-backward", funcscan.branch_target(always(1, 5), 5), 1
    yield "branch-target-of-another-instruction", funcscan.branch_target(0x24010001, 5), None  # li at,1
    # A return in the last word of the range: the function does not reach beyond the range.
    yield "return-at-the-end-of-the-range", sweep([OPEN, RETURN]), ([(0, 2)], [])

    # A jump table: the two cases are only reached through the table. After them the table holds text.
    # The two table words look like instructions and never return: they are set aside as data.
    code = [OPEN, LUI_AT | 0x8020, ADD_AT, LW_V0 | 72, NOP, JR_V0, NOP, ONE, RETURN, NOP, TWO, RETURN, CLOSE]
    table = [BASE + 4 * 7, BASE + 4 * 10, *TEXT]
    yield "jump-table-cases", sweep(code + plain + [NOP] + table), ([(0, 13), (13, 4)], [(18, 2)])
    found, taken, none = switched(lambda one, two, end: [BASE + 4 * one, BASE + 4 * two, *TEXT])
    yield "jump-table-taken", found, taken
    # Without a reader no table is read, and without a list the runs set aside are dropped.
    yield "scan-without-reader-or-list", funcscan.scan(code + plain + [NOP] + table, BASE, 0, 23), [
        (BASE, 40),
        (BASE + 40, 12),
        (BASE + 52, 16),
    ]
    # The farthest case counts, not the last one in the table.
    found, taken, none = switched(lambda one, two, end: [BASE + 4 * two, BASE + 4 * one, *TEXT])
    yield "jump-table-cases-in-another-order", found, taken
    # A case on the last return itself: the function ends with that return.
    found, taken, none = switched(lambda one, two, end: [BASE + 4 * one, BASE + 4 * two + 4, *TEXT])
    yield "jump-table-case-on-the-return", found, taken
    # A table that runs to the end of the file ends there.
    found, taken, none = switched(lambda one, two, end: [NOP] * 6 + [BASE + 4 * one, BASE + 4 * two])
    yield "jump-table-before-the-end", found, none
    words = [OPEN, LUI_AT | 0x8020, LW_V0 | 44, JR_V0, NOP, ONE, RETURN, NOP, TWO, RETURN, CLOSE, BASE + 20, BASE + 32]
    yield "jump-table-at-the-end-of-the-file", sweep(words)[0], [(0, 11)]
    # The table ends at the first word that is not a case: text, an address that is not a multiple of
    # four, the end of the swept range or beyond, or an address before the function.
    found, taken, none = switched(lambda one, two, end: [*TEXT, BASE + 4 * one, BASE + 4 * two])
    yield "jump-table-absent", found, none
    found, taken, none = switched(lambda one, two, end: [BASE + 4 * one + 2, BASE + 4 * two])
    yield "jump-table-entry-not-a-multiple-of-four", found, none
    found, taken, none = switched(lambda one, two, end: [BASE + 4 * end, BASE + 4 * two])
    yield "jump-table-entry-at-the-end-of-the-range", found, none
    found, taken, none = switched(lambda one, two, end: [BASE - 4, BASE + 4 * two])
    yield "jump-table-entry-before-the-range", found, none
    # The function of this construction starts at word 4, after another one.
    lead = [OPEN, NOP, RETURN, CLOSE]
    behind = lead + [OPEN, LUI_AT | 0x8020, LW_V0 | 64, JR_V0, NOP, ONE, RETURN, NOP, TWO, RETURN, CLOSE, NOP]
    yield "jump-table-entry-before-the-function", sweep(behind + [BASE + 8, BASE + 48, *TEXT])[0], [(0, 4), (4, 8), (12, 3)]
    yield "jump-table-entry-at-the-function", sweep(behind + [BASE + 16, BASE + 48, *TEXT])[0], [(0, 4), (4, 11)]
    # The pair that reads the table: a load into another register and the upper half of another
    # register in between are passed over.
    found, taken, none = switched(lambda one, two, end: [BASE + 4 * one, BASE + 4 * two, *TEXT], load_gap=(0x8FA30000,))
    yield "jump-table-load-of-another-register-between", found, taken
    found, taken, none = switched(lambda one, two, end: [BASE + 4 * one, BASE + 4 * two, *TEXT], upper_gap=(0x3C030000,))
    yield "jump-table-upper-half-of-another-register-between", found, taken
    # So is another instruction that writes the base register (`ori at,at,0`).
    found, taken, none = switched(lambda one, two, end: [BASE + 4 * one, BASE + 4 * two, *TEXT], upper_gap=(0x34210000,))
    yield "jump-table-other-instruction-into-the-base-between", found, taken
    # The load may sit directly before the jump, and the upper half directly before the load.
    found, taken, none = switched(lambda one, two, end: [BASE + 4 * one, BASE + 4 * two, *TEXT], load_gap=())
    yield "jump-table-load-directly-before", found, taken
    found, taken, none = switched(lambda one, two, end: [BASE + 4 * one, BASE + 4 * two, *TEXT], upper_gap=())
    yield "jump-table-upper-half-directly-before", found, taken
    # The nearest load into the register decides: here it is `lw v0,16(sp)`, not the table.
    found, taken, none = switched(lambda one, two, end: [BASE + 4 * one, BASE + 4 * two, *TEXT], load_gap=(0x8FA20010,))
    yield "jump-table-register-reloaded", found, none
    # Another kind of load into the register (`lh v0,lo(at)`) does not read a table.
    found, taken, none = switched(lambda one, two, end: [BASE + 4 * one, BASE + 4 * two, *TEXT], load_gap=(0x84220000,))
    yield "jump-table-other-load-between", found, taken
    # Twelve words back is searched, thirteen is not: for the load before the jump, for the upper half before the load.
    for gap, want in ((11, "taken"), (12, "none")):
        found, taken, none = switched(lambda one, two, end: [BASE + 4 * one, BASE + 4 * two, *TEXT], load_gap=(NOP,) * gap)
        yield f"jump-table-load-{gap + 1}-back", found, taken if want == "taken" else none
        found, taken, none = switched(lambda one, two, end: [BASE + 4 * one, BASE + 4 * two, *TEXT], upper_gap=(NOP,) * gap)
        yield f"jump-table-upper-half-{gap + 1}-back", found, taken if want == "taken" else none
    # The pair is searched inside the function only: here the load, then only the upper half, sits in the function before.
    before = [LUI_AT | 0x8020, LW_V0 | 56, RETURN, NOP, OPEN, JR_V0, NOP, ONE, RETURN, NOP, TWO, RETURN, CLOSE, NOP]
    yield "jump-table-load-before-the-function", sweep(before + [BASE + 40, *TEXT])[0], [(0, 4), (4, 6), (10, 3)]
    before = [LUI_AT | 0x8020, RETURN, NOP, LW_V0 | 48, JR_V0, NOP, ONE, RETURN, NOP, TWO, RETURN, CLOSE]
    yield "jump-table-upper-half-before-the-function", sweep(before + [BASE + 36, *TEXT])[0], [(0, 3), (3, 6), (9, 3)]
    # A table whose address has a negative low half.
    high_base = 0x80207FC0
    found, taken, none = switched(lambda one, two, end: [high_base + 4 * one, high_base + 4 * two, *TEXT], base=high_base)
    yield "jump-table-negative-low-half", found, taken
    # A `jr` through a register without a table is not an end by itself.
    yield "register-jump-is-not-an-end", sweep([OPEN, JR_V0, NOP, ONE, RETURN, CLOSE]), ([(0, 6)], [])

    # The system call stubs, and the same jump without one of the three loads directly before it.
    stub = [0x240A00A0, 0x01400008, 0x24090005]
    for load in (0x240A00A0, 0x240A00B0, 0x240A00C0):
        yield f"system-call-stub-{load & 0xFF:x}", sweep([load, *stub[1:]] * 2), ([(0, 3), (3, 3)], [])
    yield "jump-through-t2-alone", sweep([ONE, 0x01400008, TWO, RETURN, NOP]), ([(0, 5)], [])
    yield "jump-through-another-register-after-the-load", sweep([0x240A00A0, JR_V0, NOP, ONE, RETURN, NOP]), ([(0, 6)], [])
    yield "jump-through-t2-after-another-load", sweep([0x240A00D0, 0x01400008, TWO, RETURN, NOP]), ([(0, 5)], [])
    yield "jump-through-t2-load-not-directly-before", sweep([0x240A00A0, ONE, 0x01400008, TWO, RETURN, NOP]), ([(0, 6)], [])
    # The load before the swept range is not looked at.
    yield "stub-load-before-the-range", sweep(stub + [ONE, RETURN, NOP], low=1), ([(1, 5)], [])

    # Text before the code is not swept as code.
    yield "text-before-code", sweep([*TEXT, *plain]), ([(3, 4)], [])
    # Words after the last return that look like instructions and never return are data.
    yield "instruction-like-words-after-code", sweep(plain + [ONE, TWO, ONE]), ([(0, 4)], [(4, 3)])
    # The same in front of a function nobody calls: not recognised, as the tool's text says.
    yield "instruction-like-words-before-uncalled-code", sweep([ONE, TWO] + plain), ([(0, 6)], [])
    # Called, the function starts where it is called and the words before it are data.
    caller = [OPEN, jal(8), NOP, RETURN, CLOSE, NOP]
    yield "instruction-like-words-before-called-code", sweep(caller + [ONE, TWO] + plain), (
        [(0, 5), (8, 4)],
        [(6, 2)],
    )
    # An entry given from outside does the same. One that is not a multiple of four is not an entry.
    yield "entry-from-outside", sweep([ONE, TWO] + plain, [BASE + 8]), ([(2, 4)], [(0, 2)])
    yield "entry-not-a-multiple-of-four", sweep([ONE, TWO] + plain, [BASE + 10]), ([(0, 6)], [])
    # Only a `jal` inside the swept range counts: here the caller lies before it, then after it.
    yield "call-from-before-the-range", sweep(caller + [ONE, TWO] + plain, low=6), ([(6, 6)], [])
    after = [ONE, TWO] + plain + [OPEN, jal(2), NOP, RETURN, CLOSE]
    yield "call-from-after-the-range", sweep(after, high=6), ([(0, 6)], [])
    yield "call-inside-the-range", sweep(after), ([(2, 4), (6, 5)], [(0, 2)])
    # A run that calls something and is cut at a called address is kept: the entry routine's case.
    yield "unreturned-run-that-calls", sweep([OPEN, jal(4), NOP, ONE] + plain, [BASE + 16]), ([(0, 4), (4, 4)], [])
    # One that calls something and just runs into text is not.
    yield "unreturned-run-into-text", sweep([OPEN, jal(0x4000), NOP, ONE, *TEXT]), ([], [(0, 4)])

    yield "is-instruction-accepts-code", all(funcscan.is_instruction(w) for w in early + code + stub), True
    yield "is-instruction-rejects-text", [funcscan.is_instruction(w) for w in TEXT], [False, False, False]
    # The instructions taken as possible, written out by name: MIPS I and the two coprocessors of the machine.
    # sll srl sra sllv srlv srav / jr jalr / syscall break / mfhi mthi mflo mtlo / mult multu div divu /
    # add addu sub subu and or xor nor / slt sltu
    special = [0, 2, 3, 4, 6, 7, 8, 9, 12, 13, 16, 17, 18, 19, 24, 25, 26, 27, 32, 33, 34, 35, 36, 37, 38, 39, 42, 43]
    yield "is-instruction-special", [f for f in range(64) if funcscan.is_instruction(f)], special
    # bltz bgez bltzal bgezal
    regimm = [0, 1, 16, 17]
    yield "is-instruction-regimm", [r for r in range(32) if funcscan.is_instruction(0x04000000 | r << 16)], regimm
    # j jal beq bne blez bgtz / addi addiu slti sltiu andi ori xori lui / cop0 cop2 /
    # lb lh lwl lw lbu lhu lwr / sb sh swl sw swr / lwc0 lwc2 swc0 swc2
    primary = [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 18, 32, 33, 34, 35, 36, 37, 38, 40, 41, 42, 43, 46, 48, 50, 56, 58]
    yield "is-instruction-primary", [op for op in range(2, 64) if funcscan.is_instruction(op << 26)], primary
    # The other fields of a word do not matter.
    yield "is-instruction-other-fields", [funcscan.is_instruction(w) for w in (0x03FFFFC1, 0x07FFFFFF, 0x07E2FFFF)], [
        False,
        False,
        False,
    ]

    # The counts that a wrong boundary can disturb: returns outside every function, functions with more
    # than one return, with more than one frame opened, and without a return or stub jump at their end.
    yield "checks-plain", checks(plain + plain), [0, 0, 0, 0]
    yield "checks-stubs", checks(stub + stub), [0, 0, 0, 0]
    yield "checks-two-returns", checks(early + plain), [0, 1, 0, 0]
    twice = [OPEN, always(6, 1), NOP, RETURN, NOP, OPEN, TWO, RETURN, CLOSE]
    yield "checks-two-frames", checks(twice + twice), [0, 2, 2, 0]
    # A return in a run that is set aside: the branch target behind it was never reached.
    yield "checks-return-outside", checks([OPEN, always(5, 1), NOP, RETURN, NOP, *TEXT] + plain), [1, 0, 0, 0]
    # A run kept without a return, and one that is a single word.
    yield "checks-open-end", checks([OPEN, jal(4), NOP, ONE] + plain, [BASE + 16]), [0, 0, 0, 1]
    yield "checks-open-single-word", checks([jal(1)] + plain), [0, 0, 0, 1]
    # Only the swept range is counted.
    yield "checks-range-from", checks(plain + plain, low=4), [0, 0, 0, 0]
    yield "checks-range-to", checks(plain + plain, high=4), [0, 0, 0, 0]
    yield "checks-text", funcscan.census_text({"outside": 1, "returns": 2, "frames": 3, "open": 4}), (
        "checks: `jr ra` words outside every function: 1; functions with more than one `jr ra`: 2;"
        " functions that open more than one stack frame: 3;"
        " functions that end neither with `jr ra` nor with a stub's `jr t2`: 4"
    )

    read = funcscan.reader([ONE, TWO], BASE)
    yield "reader", [read(BASE), read(BASE + 4), read(BASE + 8), read(BASE - 4), read(BASE + 2)], [ONE, TWO, None, None, None]


def tool(*args) -> subprocess.CompletedProcess:
    return subprocess.run([sys.executable, str(TOOL), *map(str, args)], capture_output=True, text=True)


def command_cases(root: Path):
    plain = [OPEN, NOP, RETURN, CLOSE]
    words = [*TEXT, *plain, NOP, *plain]
    code = root / "code.bin"
    code.write_bytes(bytes(16) + struct.pack(f"<{len(words)}I", *words))
    base = ("--base", hex(BASE), "--offset", 16)
    out = root / "found.tsv"
    proc = tool("scan", code, *base, "--out", out)
    yield "scan-summary", proc, 0, "2 functions, 32 bytes; 0 runs set aside as data, 0 bytes"
    want = f"{BASE + 12:08x}\t16\n{BASE + 32:08x}\t16\n"
    yield "scan-output-file", (out.read_text() == want, out.read_text()), True, ""
    yield "scan-output-file-only", (proc.stdout == "", proc.stdout), True, ""
    listed = tool("scan", code, *base)
    yield "scan-output-listed", (listed.stdout == want, listed.stdout), True, ""
    yield "scan-start", tool("scan", code, *base, "--start", hex(BASE + 28)), 0, f"{BASE + 32:08x}\t16\n1 functions, 16 bytes"
    yield "scan-end", tool("scan", code, *base, "--end", hex(BASE + 28)), 0, f"{BASE + 12:08x}\t16\n1 functions, 16 bytes"
    # The four counts end the summary, over the swept range only.
    clean = "checks: `jr ra` words outside every function: 0; functions with more than one `jr ra`: 0;"
    yield "scan-checks", proc, 0, clean
    yield "scan-checks-from-start", tool("scan", code, *base, "--start", hex(BASE + 28)), 0, clean
    yield "scan-checks-to-end", tool("scan", code, *base, "--end", hex(BASE + 28)), 0, clean
    end = BASE + 4 * len(words)
    yield "scan-whole-range-named", tool("scan", code, *base, "--start", hex(BASE), "--end", hex(end)), 0, "2 functions, 32 bytes"
    for label, limits in (
        ("start-beyond-the-file", ("--start", hex(BASE + 0x1000))),
        ("start-before-the-file", ("--start", hex(BASE - 4))),
        ("end-beyond-the-file", ("--end", hex(end + 4))),
        ("end-three-bytes-beyond-the-file", ("--end", hex(end + 3))),
        ("end-one-byte-beyond-the-file", ("--end", hex(end + 1))),
        ("empty-range", ("--start", hex(BASE + 12), "--end", hex(BASE + 12))),
    ):
        yield f"scan-{label}", tool("scan", code, *base, *limits), 1, "not inside the file"
    # A range inside the file must start and end on a word.
    for label, limits in (("start", ("--start", hex(BASE + 13))), ("end", ("--end", hex(end - 3)))):
        yield f"scan-{label}-not-on-a-word", tool("scan", code, *base, *limits), 1, "does not start and end on a word"
    yield "scan-range-named-in-the-message", tool("scan", code, *base, "--end", hex(end + 3)), 1, (
        f"the range to sweep, {BASE:#x} to {end + 3:#x}, is not inside the file"
    )
    yield "scan-base-not-a-multiple-of-four", tool("scan", code, "--base", hex(BASE + 2), "--offset", 16), 1, (
        "the base is not a multiple of four"
    )
    yield "scan-negative-offset", tool("scan", code, "--base", hex(BASE), "--offset", -16), 1, "the offset is negative"
    # A jump table is read from the file: the function goes on past its first return to the cases.
    switch = [OPEN, 0x3C018020, 0x8C220030, 0x00400008, NOP, ONE, RETURN, NOP, TWO, RETURN, CLOSE, NOP]
    jumping = root / "jumping.bin"
    jumping.write_bytes(struct.pack("<15I", *switch, BASE + 20, BASE + 32, TEXT[0]))
    yield "scan-jump-table", tool("scan", jumping, "--base", hex(BASE)), 0, (
        "1 functions, 44 bytes; 1 runs set aside as data, 8 bytes"
    )
    yield "scan-checks-counted", tool("scan", jumping, "--base", hex(BASE)), 0, (
        "outside every function: 0; functions with more than one `jr ra`: 1; functions that open more than one stack frame: 0;"
    )
    whole = root / "whole.tsv"
    whole.write_text(f"{BASE:08x}\t44\n")
    yield "compare-checks", tool("compare", jumping, "--base", hex(BASE), "--inventory", whole), 0, (
        "found only here: 0\nchecks: `jr ra` words outside every function: 0; functions with more than one `jr ra`: 1;"
    )
    # Bytes after the last whole word are not read, and a range cannot reach into them.
    ragged = root / "ragged.bin"
    ragged.write_bytes(code.read_bytes() + b"\x01\x02")
    yield "scan-file-not-a-multiple-of-four", tool("scan", ragged, *base), 0, "2 functions, 32 bytes"
    yield "scan-end-in-the-bytes-not-read", tool("scan", ragged, *base, "--end", hex(end + 2)), 1, "not inside the file"

    glued = root / "glued.bin"
    glued.write_bytes(struct.pack("<6I", ONE, TWO, *plain))
    yield "scan-without-entries", tool("scan", glued, "--base", hex(BASE)), 0, "1 functions, 24 bytes"
    # An address is written with eight digits.
    yield "scan-low-address", tool("scan", glued, "--base", "0x1000"), 0, "00001000\t24\n"
    # The two forms of a line of entries, each alone, and lines that hold no address.
    for label, text in (
        ("assignment", f"second = {BASE + 8:#x};\n"),
        ("address", f"{BASE + 8:x}\n"),
        ("among-other-lines", f"\n/* entries */\n   \nsecond = {BASE + 8:#x};\n\n"),
    ):
        entries = root / f"{label}.ld"
        entries.write_text(text)
        yield f"scan-with-entries-{label}", tool("scan", glued, "--base", hex(BASE), "--entries", entries), 0, (
            "1 functions, 16 bytes; 1 runs set aside as data, 8 bytes"
        )
    nothing = root / "nothing.ld"
    nothing.write_text("\n/* entries */\nsecond\n")
    yield "scan-with-entries-none", tool("scan", glued, "--base", hex(BASE), "--entries", nothing), 0, "1 functions, 24 bytes"

    # Lines without an address or without a size are not functions of the inventory.
    same = root / "same.tsv"
    same.write_text(f"address\tname\tsize\n{BASE + 12:08x}\tfirst\t16\n\n{BASE:08x}\n{BASE + 32:08x}\tsecond\t16\n")
    proc = tool("compare", code, *base, "--inventory", same)
    yield "compare-same", proc, 0, "same start and size: 2; same but for zero padding that the inventory counts: 0;"
    yield "compare-counts", proc, 0, "inventory: 2 functions; found: 2\n"
    yield "compare-same-rest", proc, 0, "same start, other size: 0; not found: 0; found only here: 0\n"
    # The inventory counts the padding word after the first function.
    padded = root / "padded.tsv"
    padded.write_text(f"{BASE + 12:08x}\t20\n{BASE + 32:08x}\t16\n")
    yield "compare-padding", tool("compare", code, *base, "--inventory", padded), 0, (
        "same but for zero padding that the inventory counts: 1;"
    )
    # A size that is not explained by padding, and a function the sweep does not find.
    wrong = root / "wrong.tsv"
    wrong.write_text(f"{BASE + 12:08x}\t12\n{BASE + 32:08x}\t16\n")
    yield "compare-other-size", tool("compare", code, *base, "--inventory", wrong), 1, "same start, other size: 1;"
    yield "compare-names-the-difference", tool("compare", code, *base, "--inventory", wrong), 1, (
        f"  other size: {BASE + 12:#x} inventory 12 found 16\n"
    )
    # A larger size in the inventory is padding only when every word of the difference is zero and in the file.
    longer = root / "longer.tsv"
    longer.write_text(f"{BASE + 12:08x}\t24\n{BASE + 32:08x}\t16\n")
    yield "compare-larger-not-padding", tool("compare", code, *base, "--inventory", longer), 1, (
        "same but for zero padding that the inventory counts: 0; same start, other size: 1;"
    )
    beyond = root / "beyond.tsv"
    beyond.write_text(f"{BASE + 12:08x}\t16\n{BASE + 32:08x}\t20\n")
    yield "compare-larger-than-the-file", tool("compare", code, *base, "--inventory", beyond, "--end", hex(end)), 1, (
        "same but for zero padding that the inventory counts: 0; same start, other size: 1;"
    )
    yield "compare-range-beyond-the-file", tool("compare", code, *base, "--inventory", beyond), 1, "not inside the file"
    # Near the end of a file: one function and one word of zero after it, 20 bytes. A size that claims
    # bytes the file does not have is not padding, with the range of the inventory or with a given one.
    last = ("--end", hex(BASE + 20))

    def claim(name: str, data: bytes, size: int, *limits) -> subprocess.CompletedProcess:
        (root / f"{name}.bin").write_bytes(data)
        (root / f"{name}.tsv").write_text(f"{BASE:08x}\t{size}\n")
        return tool("compare", root / f"{name}.bin", "--base", hex(BASE), "--inventory", root / f"{name}.tsv", *limits)

    tail = struct.pack("<5I", *plain, NOP)
    is_padding = "same but for zero padding that the inventory counts: 1; same start, other size: 0;"
    not_padding = "same but for zero padding that the inventory counts: 0; same start, other size: 1;"
    yield "compare-padding-at-the-end-of-the-file", claim("tail20", tail, 20), 0, is_padding
    yield "compare-three-bytes-beyond-the-file", claim("tail23", tail, 23), 1, "not inside the file"
    yield "compare-three-bytes-beyond-the-file-range-given", claim("tail23g", tail, 23, *last), 1, not_padding
    yield "compare-one-byte-beyond-the-file-range-given", claim("tail21g", tail, 21, *last), 1, not_padding
    # A size that is not a multiple of four: its range is refused, and with a range given every claimed byte counts.
    yield "compare-size-not-a-multiple-of-four", claim("tail18", tail, 18), 1, "does not start and end on a word"
    yield "compare-padding-part-of-a-word", claim("tail18g", tail, 18, *last), 0, is_padding
    mixed = struct.pack("<5I", *plain, 0x00010000)  # two bytes of zero, then a byte that is not
    yield "compare-padding-up-to-a-byte-that-is-not-zero", claim("mixed18", mixed, 18, *last), 0, is_padding
    yield "compare-padding-over-a-byte-that-is-not-zero", claim("mixed19", mixed, 19, *last), 1, not_padding
    # Bytes after the last whole word of the file are not read: padding before them stays padding, a size
    # that reaches into them does not.
    yield "compare-padding-before-bytes-not-read", claim("ragged20", tail + bytes(2), 20), 0, is_padding
    yield "compare-size-into-bytes-not-read", claim("ragged22", tail + bytes(2), 22), 1, "not inside the file"
    yield "compare-size-into-bytes-not-read-range-given", claim("ragged22g", tail + bytes(2), 22, *last), 1, not_padding
    # An inventory whose first address is not on a word.
    odd = root / "odd.tsv"
    odd.write_text(f"{BASE + 13:08x}\t16\n")
    yield "compare-inventory-start-not-on-a-word", tool("compare", code, *base, "--inventory", odd, "--end", hex(end)), 1, (
        "does not start and end on a word"
    )
    shifted = root / "shifted.tsv"
    shifted.write_text(f"{BASE + 12:08x}\t16\n{BASE + 36:08x}\t12\n")
    proc = tool("compare", code, *base, "--inventory", shifted)
    yield "compare-not-found", proc, 1, "same start and size: 1;"
    yield "compare-not-found-rest", proc, 1, "same start, other size: 0; not found: 1; found only here: 1\n"
    yield "compare-names-the-missing", proc, 1, f"  not found: {BASE + 36:#x} inventory 12\n"
    yield "compare-names-the-extra", proc, 1, f"  found only here: {BASE + 32:#x} found 16\n"
    # `--show` limits the differences named per kind.
    both = root / "both.tsv"
    both.write_text(f"{BASE + 12:08x}\t12\n{BASE + 32:08x}\t12\n")
    proc = tool("compare", code, *base, "--inventory", both, "--end", hex(end))
    yield "compare-names-all", (proc.stdout.count("  other size: ") == 2, proc.stdout), True, ""
    proc = tool("compare", code, *base, "--inventory", both, "--end", hex(end), "--show", 1)
    yield "compare-show-one", (proc.stdout.count("  other size: ") == 1, proc.stdout), True, ""
    yield "compare-show-the-first", proc, 1, f"  other size: {BASE + 12:#x} inventory 12 found 16\n"
    # The sweep covers the inventory's range unless a limit is given: an inventory of one of the two
    # functions agrees, and the other function is reported once the range reaches it.
    for label, address, limit in (("first", BASE + 12, ("--end", hex(end))), ("second", BASE + 32, ("--start", hex(BASE)))):
        partial = root / f"{label}.tsv"
        partial.write_text(f"{address:08x}\t16\n")
        yield f"compare-range-of-the-inventory-{label}", tool("compare", code, *base, "--inventory", partial), 0, (
            "inventory: 1 functions; found: 1\n"
        )
        proc = tool("compare", code, *base, "--inventory", partial, *limit)
        yield f"compare-found-only-here-{label}", proc, 1, "same start and size: 1;"
        yield f"compare-found-only-here-{label}-rest", proc, 1, "not found: 0; found only here: 1\n"
    empty = root / "empty.tsv"
    empty.write_text("address\tname\tsize\n")
    yield "compare-empty-inventory", tool("compare", code, *base, "--inventory", empty), 1, "the inventory is empty"


def main() -> int:
    failed = 0
    for name, got, want in sweep_cases():
        ok = got == want
        print(f"{'ok  ' if ok else 'FAIL'} {name}" + ("" if ok else f": got {got}, wanted {want}"))
        failed += not ok
    with tempfile.TemporaryDirectory() as tmp:
        for name, proc, want_status, want_text in command_cases(Path(tmp)):
            if isinstance(proc, tuple):
                ok, output = proc[0] is want_status, proc[1]
            else:
                output = proc.stdout + proc.stderr
                ok = proc.returncode == want_status and want_text in output
            print(f"{'ok  ' if ok else 'FAIL'} {name}" + ("" if ok else f": wanted {want_status} with {want_text!r}\n{output}"))
            failed += not ok
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
