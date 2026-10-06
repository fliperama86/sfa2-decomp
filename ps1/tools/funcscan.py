#!/usr/bin/env python3
"""Find function boundaries in MIPS code built by the PS1 toolchain of this project.

The method is a linear sweep. A function starts at the first word that is
neither zero nor an impossible instruction. It ends with the delay slot of
the first `jr ra` that lies at or beyond every forward branch target and
jump table case seen so far. A `jr t2` that directly follows `li t2,0xa0`,
`0xb0` or `0xc0` ends it too: that is how the system call stubs leave.
Nothing else ends a function; in the resident executable every function
found ends in one of these two ways except the program entry routine.

A run is cut short, and a new one starts, at:

- a word that is not an instruction of this processor;
- an address that a `jal` in the swept range, or an entry given by the
  caller, calls. This assumes that a function is not called into its
  middle; where one is, the sweep reports two functions.

A run that was cut short before it returned is kept as a function only if
it was cut at a called address and holds a `jal` itself; otherwise it is
reported as data. That is how tables and text around the code inside a
module are set aside. Data whose words all look like instructions and that
sits directly before a function nobody calls is not recognised: it becomes
part of that function.

Jump tables: before a `jr` through another register, the sweep looks in the
current function for the nearest `lw` into that register, up to twelve
instructions back, and for the nearest `lui` into that load's base register,
up to twelve instructions before the load. From the address the pair forms
it takes every consecutive word that is a multiple of four and points at or
after the start of the current function, inside the swept range, as a case
of that function.

Branches are `beq`, `bne`, `blez`, `bgtz`, `bltz` and `bgez`, and `bltzal`
and `bgezal`, which are followed as branches and not counted as calls. A
target outside the swept range is ignored. Coprocessor branches are not
followed.

This is a static estimate. It is checked against an inventory made with
another tool (`compare`), and each function it reports is only established
when the matching build rebuilds it. Both commands end with four counts
that a wrong boundary can disturb: see `census`.

usage:
  funcscan.py scan FILE --base ADDRESS [--offset N] [--start A] [--end A] [--entries FILE] [--out TSV]
  funcscan.py compare FILE --base ADDRESS [--offset N] [--start A] [--end A] [--show N] --inventory TSV

`compare` sweeps from the first address of the inventory to the end of its
last function unless `--start` or `--end` says otherwise.
"""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

JR_RA = 0x03E00008
NOP = 0
# The system call stubs: `li t2,0xa0` (or 0xb0, 0xc0), `jr t2`, and the call number in the delay slot.
STUB_JUMP = 0x01400008
STUB_LOADS = {0x240A00A0, 0x240A00B0, 0x240A00C0}
# Opcodes whose 16-bit field is a branch displacement: beq, bne, blez, bgtz.
BRANCH_OPS = {0x04, 0x05, 0x06, 0x07}
REGIMM_BRANCHES = {0x00, 0x01, 0x10, 0x11}  # bltz, bgez, bltzal, bgezal
TABLE_LOOKBACK = 12  # words searched before a `jr reg` for the table load, and before that load for its `lui`
# MIPS I as this compiler and the hand-written parts use it, with the two coprocessors of the machine.
SPECIAL_FUNCTIONS = {
    0x00, 0x02, 0x03, 0x04, 0x06, 0x07, 0x08, 0x09, 0x0C, 0x0D, 0x10, 0x11, 0x12, 0x13,
    0x18, 0x19, 0x1A, 0x1B, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x2A, 0x2B,
}  # fmt: skip
PRIMARY_OPCODES = {
    0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x12,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2A, 0x2B, 0x2E, 0x30, 0x32, 0x38, 0x3A,
}  # fmt: skip


def is_instruction(word: int) -> bool:
    op = word >> 26
    if op == 0:
        return word & 0x3F in SPECIAL_FUNCTIONS
    if op == 1:
        return (word >> 16) & 0x1F in REGIMM_BRANCHES
    return op in PRIMARY_OPCODES


def signed16(value: int) -> int:
    return value - 0x10000 if value & 0x8000 else value


def branch_target(word: int, index: int) -> int | None:
    """Index of the word a conditional branch at `index` goes to, or None."""
    op = word >> 26
    if op in BRANCH_OPS or (op == 0x01 and (word >> 16) & 0x1F in REGIMM_BRANCHES):
        return index + 1 + signed16(word & 0xFFFF)
    return None


def nearest(words: list[int], index: int, low: int, opcode: int, register: int) -> int | None:
    """Index of the nearest word before `index` with this opcode and target register, or None.

    At most TABLE_LOOKBACK words back, and not before `low`.
    """
    for back in range(index - 1, max(low, index - TABLE_LOOKBACK) - 1, -1):
        if words[back] >> 26 == opcode and (words[back] >> 16) & 0x1F == register:
            return back
    return None


def table_cases(words: list[int], index: int, base: int, low: int, high: int, read) -> list[int]:
    """Case targets of the jump table used by the `jr` at `index`, as word indexes."""
    load = nearest(words, index, low, 0x23, (words[index] >> 21) & 0x1F)  # lw register,lo(base register)
    upper = None if load is None else nearest(words, load, low, 0x0F, (words[load] >> 21) & 0x1F)  # lui base register
    if upper is None:
        return []
    table = ((words[upper] & 0xFFFF) << 16) + signed16(words[load] & 0xFFFF)
    cases: list[int] = []
    while True:
        entry = read(table + 4 * len(cases))
        if entry is None or entry % 4 or not (base + 4 * low <= entry < base + 4 * high):
            return cases
        cases.append((entry - base) // 4)


def scan(
    words: list[int], base: int, low: int, high: int, read=None, entries=(), data: list | None = None
) -> list[tuple[int, int]]:
    """Functions in words[low:high] as (address, size in bytes).

    `entries` are addresses known to be called. Runs set aside as data are
    appended to `data` as (address, size) when a list is given.
    """
    read = read or (lambda address: None)
    called = {(a - base) // 4 for a in entries if a % 4 == 0}
    for position in range(low, high):
        if words[position] >> 26 == 0x03:
            called.add((((base + 4 * position) & 0xF0000000 | (words[position] & 0x03FFFFFF) << 2) - base) // 4)
    functions = []
    index = low
    while index < high:
        if words[index] == NOP or not is_instruction(words[index]):
            index += 1
            continue
        start = far = index
        returned = calls = at_call = False
        while index < high:
            if index > start and index in called:
                at_call = True
                break
            word = words[index]
            if not is_instruction(word):
                break
            target = branch_target(word, index)
            if target is not None and target < high:
                far = max(far, target)
            op = word >> 26
            leaves = False
            if word == JR_RA:
                leaves = True
            elif op == 0x03:
                calls = True
            elif op == 0x02:  # j: a branch inside the function
                goal = (((base + 4 * index) & 0xF0000000 | (word & 0x03FFFFFF) << 2) - base) // 4
                if goal < high:
                    far = max(far, goal)
            elif op == 0 and word & 0x3F == 0x08:  # jr through another register
                for case in table_cases(words, index, base, start, high, read):
                    far = max(far, case)
                leaves = word == STUB_JUMP and index > start and words[index - 1] in STUB_LOADS
            if leaves and index >= far:
                index = min(index + 2, high)
                returned = True
                break
            index += 1
        if returned or (calls and at_call):
            functions.append((base + 4 * start, 4 * (index - start)))
        elif data is not None:
            data.append((base + 4 * start, 4 * (index - start)))
    return functions


CHECKS = (
    ("outside", "`jr ra` words outside every function"),
    ("returns", "functions with more than one `jr ra`"),
    ("frames", "functions that open more than one stack frame"),
    ("open", "functions that end neither with `jr ra` nor with a stub's `jr t2`"),
)


def census(words: list[int], base: int, low: int, high: int, functions: list[tuple[int, int]]) -> dict[str, int]:
    """Counts over words[low:high] and the functions found there that a wrong boundary can disturb: see CHECKS.

    A frame is opened by `addiu sp,sp,-N`. None of the counts proves a
    boundary; a function cut in two or two taken as one shows in them
    unless neither part has a frame or a second return.
    """
    counts = {"outside": words[low:high].count(JR_RA), "returns": 0, "frames": 0, "open": 0}
    for address, size in functions:
        part = words[(address - base) // 4 : (address - base + size) // 4]
        counts["outside"] -= part.count(JR_RA)
        counts["returns"] += part.count(JR_RA) > 1
        counts["frames"] += sum(1 for word in part if word & 0xFFFF8000 == 0x27BD8000) > 1
        counts["open"] += part[-2:-1] not in ([JR_RA], [STUB_JUMP])
    return counts


def census_text(counts: dict[str, int]) -> str:
    return "checks: " + "; ".join(f"{text}: {counts[key]}" for key, text in CHECKS)


def load(path: str, offset: int) -> list[int]:
    data = Path(path).read_bytes()[offset:]
    return list(struct.unpack_from(f"<{len(data) // 4}I", data))


def reader(words: list[int], base: int):
    def read(address: int):
        index = (address - base) // 4
        return words[index] if address % 4 == 0 and 0 <= index < len(words) else None

    return read


def read_entries(path: str | None) -> list[int]:
    """Addresses from a file of `name = 0xADDRESS;` lines or of one hexadecimal address per line."""
    if not path:
        return []
    found = []
    for line in Path(path).read_text().splitlines():
        text = line.split("=")[-1].strip().rstrip(";").split()[0] if line.strip() else ""
        try:
            found.append(int(text, 16))
        except ValueError:
            continue
    return found


def run_scan(args, counts: dict, data: list | None = None) -> list[tuple[int, int]]:
    """The functions of the range the arguments name. `counts` receives the census of that range."""
    words = load(args.file, args.offset)
    low = (args.start - args.base) // 4 if args.start is not None else 0
    high = (args.end - args.base) // 4 if args.end is not None else len(words)
    if not 0 <= low < high <= len(words):
        raise SystemExit("the range to sweep is not inside the file")
    entries = read_entries(getattr(args, "entries", None))
    functions = scan(words, args.base, low, high, reader(words, args.base), entries, data)
    counts.update(census(words, args.base, low, high, functions))
    return functions


def cmd_scan(args) -> int:
    data: list = []
    counts: dict = {}
    functions = run_scan(args, counts, data)
    lines = [f"{address:08x}\t{size}" for address, size in functions]
    if args.out:
        Path(args.out).write_text("".join(line + "\n" for line in lines))
    else:
        print("\n".join(lines))
    print(
        f"{len(functions)} functions, {sum(size for _, size in functions)} bytes;"
        f" {len(data)} runs set aside as data, {sum(size for _, size in data)} bytes",
        file=sys.stderr,
    )
    print(census_text(counts), file=sys.stderr)
    return 0


def read_inventory(path: str) -> dict[int, int]:
    """An inventory with a hexadecimal address in the first column and a size in the third or second."""
    found = {}
    for line in Path(path).read_text().splitlines():
        parts = line.split("\t")
        try:
            found[int(parts[0], 16)] = int(parts[2] if len(parts) > 2 else parts[1])
        except (ValueError, IndexError):
            continue
    return found


def cmd_compare(args) -> int:
    wanted = read_inventory(args.inventory)
    if not wanted:
        print("the inventory is empty")
        return 1
    if args.start is None:
        args.start = min(wanted)
    if args.end is None:
        args.end = max(address + size for address, size in wanted.items())
    counts: dict = {}
    found = dict(run_scan(args, counts))
    words = load(args.file, args.offset)

    def padded(address: int) -> bool:
        """The inventory's size is the found size plus words of zero."""
        first, last = (address + found[address] - args.base) // 4, (address + wanted[address] - args.base) // 4
        return first < last <= len(words) and not any(words[first:last])

    same = sorted(a for a in wanted if found.get(a) == wanted[a])
    padding = sorted(a for a in wanted if a in found and found[a] != wanted[a] and padded(a))
    resized = sorted(a for a in wanted if a in found and found[a] != wanted[a] and not padded(a))
    missing = sorted(a for a in wanted if a not in found)
    extra = sorted(a for a in found if a not in wanted)
    print(f"inventory: {len(wanted)} functions; found: {len(found)}")
    print(
        f"same start and size: {len(same)}; same but for zero padding that the inventory counts: {len(padding)};"
        f" same start, other size: {len(resized)}; not found: {len(missing)}; found only here: {len(extra)}"
    )
    for label, group in (("other size", resized), ("not found", missing), ("found only here", extra)):
        for address in group[: args.show]:
            detail = f" inventory {wanted[address]}" if address in wanted else ""
            detail += f" found {found[address]}" if address in found else ""
            print(f"  {label}: {address:#x}{detail}")
    print(census_text(counts))
    return 0 if len(same) + len(padding) == len(wanted) and not extra else 1


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = parser.add_subparsers(dest="command", required=True)

    def address(text: str) -> int:
        return int(text, 0)

    for name, fn in (("scan", cmd_scan), ("compare", cmd_compare)):
        p = sub.add_parser(name)
        p.add_argument("file")
        p.add_argument("--base", type=address, required=True, help="address of the first word read from the file")
        p.add_argument("--offset", type=address, default=0, help="bytes to skip at the start of the file")
        p.add_argument("--start", type=address, help="first address to sweep")
        p.add_argument("--end", type=address, help="address after the last word to sweep")
        if name == "scan":
            p.add_argument("--entries", help="a file of addresses known to be called")
            p.add_argument("--out", help="write address and size per line")
        else:
            p.add_argument("--inventory", required=True)
            p.add_argument("--show", type=int, default=10, help="differences to print per kind")
        p.set_defaults(fn=fn)
    args = parser.parse_args()
    return args.fn(args)


if __name__ == "__main__":
    sys.exit(main())
