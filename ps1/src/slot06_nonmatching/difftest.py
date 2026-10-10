#!/usr/bin/env python3
"""Differential test of a nonmatching C function against the original code.

    difftest.py --config ../build.toml [--folder DIR] [--cases N] [--seed S]
                [--control | --writes | --edges [--jobs N]] [--uncovered] (FUNC... | --all)

For each FUNC the tool builds `FUNC.c` of the folder (this one, or DIR) with the pinned
toolchain of the matching build (the preprocessing, compiler, maspsx and
assembler steps of `ps1/tools/matchbuild.py`), links it alone at a test
address outside the console's RAM with the tree's `symbols.ld`, and runs the
original function and the build under the Unicorn emulator on identical
random inputs. The final states are compared: the return
register when the contract says there is one, the callee-saved registers,
and all of RAM and the scratchpad except the stack region.

The contract of FUNC is the entry of `contracts.CONTRACTS`, or the value
`CONTRACT` of the file `FUNC.py` beside `FUNC.c`. A second line says how
many instruction slots of the original function the cases executed, and
`--uncovered` lists the others: cases that never reach a part of the
function say nothing about it.

The option --writes audits where the ORIGINAL writes, which the comparison above does not
say: both runs may end alike and both may have written outside what the contract names.
It runs the original only (nothing is built) on the cases of the seed and prints per function

    FUNC writes: cases N, discarded D, outside K (largest B bytes)

K is the number of cases in which the original changed at least one byte of RAM or of the
scratchpad that is neither made nor in the stack region; B is the largest number of such bytes
in one case. D counts the cases that were discarded (a fault, or the instruction budget), as
in the default mode; a discarded case is not counted as outside. With --uncovered and a K above
0 a second line follows, `  first: case C; 0xADDR..0xADDR (NAME+0xOFF); ...`: the first such
case and the address runs of its bytes (bytes less than 17 apart form one run), each run with
the nearest name at or below its first address in the same region (RAM or scratchpad) of the
tree's symbol table, `?` when there is none; at most six runs, then `and M more`. The status is
0 when every K is 0, 1 otherwise, 2 for the errors above, and --writes with --control is one.
The default mode and --control print what they printed before the option existed.

The option --edges looks for the constants whose edge no case tries. It alters the ORIGINAL
code, one constant at a time, and leaves the build alone (--control does the opposite: it alters one
word of the BUILD). Coverage of instruction slots does not show that the edge of a comparison was
tried: random inputs rarely land on the one value where a limit and its neighbour differ. For each FUNC
the build is made once and the default comparison is run first, with the given cases and seed; if it
shows a difference or a discarded case the edges of that function are not swept (one line says so, and
the status is 1). Otherwise the words of the original function are scanned, and a word is a constant of
the sweep when it is one of these (a choice, not a theory: the sweep finds the edges of comparisons and
of added limits, not every constant):

  - `slti` or `sltiu`, with any registers;
  - `addiu rt,rs,imm` with neither rs nor rt `sp` or `gp`, and not the low half of an address or of a
    32-bit constant (it is such a low half when rs == rt and the nearest earlier word of the function
    that writes rt is `lui rt`);
  - `ori rt,zero,imm`. An `ori` whose source is not `zero` is not taken, so `ori rt,zero,imm` cannot be
    the low half of a 32-bit constant, and the `lui` rule is applied to `addiu` only.

Nothing else is a constant of the sweep: no load or store offset, no `andi` or `xori` mask, no shift
amount, no branch, no `lui`. Each constant gets two altered runs, its immediate plus 1 and minus 1,
each taken modulo 0x10000 (0xffff plus 1 is 0, and 0 minus 1 is 0xffff). An altered run is the default
comparison (same cases, same seed, same build) with that one word of the original replaced in the memory
image of every case; the original is put back before the next run. A run ends at its first case that
differs: it is then noticed and it is not run to its end. A run that has found no difference goes on
to the last case, because only then is it unnoticed (or all discarded). The altered runs of a function are
made by `--jobs N` processes (default 8; 1 makes them in this process, with no pool; the processes are
forked, so the option needs a system that has that start method); the lines are printed in the fixed order
below whatever N is. `--jobs` is for `--edges` only, and N below 1 is an input error. Per function it prints

    FUNC edges: constants C, altered runs R, unnoticed U, all discarded A

and, in order of the word's index in the original (the numbering of the coverage line), plus before
minus, a line for every altered run that ends with `different 0`:

    `  slot I: MNEMONIC OPERANDS, immediate 0xOLD -> 0xNEW: different 0 of N, discarded D`

A constant in a slot that no case of the unaltered run executed counts in C, but its two runs are not
made, they are not in R, and it is not in U (the coverage line says that the slot is not executed); with
--uncovered a line `  slot I: not executed by any case` is printed for it, after the lines above. A run in
which every case is discarded (the altered original faults or does not end) is not unnoticed: it counts
in A. The status is 0 when every U is 0, 1 when any U is above 0 or a function was not swept, 2 and 3 as
above; --edges with --control or with --writes is an input error (2).

What an unnoticed line means: no case of this seed tells the constant from its neighbour. That is a gap
of the setup when the contract's inputs can reach the edge, and it is no gap when they cannot (the edge
lies in inputs the contract excludes, or the constant has no effect on what the test compares). The tool
cannot tell the two apart; the author of the contract does, for each line. A sweep with U at 0 is not
equivalence either: it covers the constants of the list above, by one in each direction, for these cases.

"Made" is memory that the setup of the case wrote through `State.write` (so through `w8`, `w16`,
`w32` and the helpers that call them, the recorders of `contracts.CallLog` among them, which
write code and log through `State`), each block that `State.alloc` handed out, whole, and what
the setup marked with `State.owns(address, size)`. `owns` writes nothing: a setup uses it for
memory whose content from the image it wants to keep and that the function may write, and the
header of that function's contract says why. The padding that `alloc` adds to reach a multiple
of 4 bytes is not made. The function's own memory is the stack region [STACK_LOW, STACK_TOP),
the region the comparison leaves out, and the 16 bytes from the initial stack pointer upward,
[STACK_TOP, STACK_TOP + 16): under the calling convention (o32) they are the callee's argument
home area, where it may spill a0 to a3. The comparison still compares those 16 bytes; only the
audit does not count them. Memory that the setup made stays made wherever it lies, in the home
area too. The scratchpad has no such region. `owns` and `read`
refuse a negative size, and `alloc` a negative size or a block that does not fit the arena,
before they change anything; every method refuses a span that does not lie inside the RAM or
the scratchpad; a span of no bytes is valid and makes nothing. The record
of what was made has one mark per byte of RAM and of scratchpad, and the audit refuses a state
whose record has another size.

What the audit does not see: a store of the value that is already there changes no byte, so
it is not counted; reads are not audited; "made" says that the setup touched or allocated the
memory, not that it filled it with varied content; and only the cases of the given seed are
run. A K above 0 means the function runs past a table the setup built, or writes a table or
a global that the setup did not make, or the setup leaves a table unfilled that the function
writes: the setup is to be changed, or the contract's header is to name the exception.

The private inputs (the baseline executable and the module archive that
the build configuration names) are read through the configuration; the
tool stops with a message when they are absent.

Exit status: 0 when every function passes, 1 for a difference (or, with
--control, when the control does not trip), 2 for a configuration or input
problem, 3 when the build fails.
"""

from __future__ import annotations

import argparse
import bisect
import concurrent.futures
import dataclasses
import importlib.util
import multiprocessing
import random
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parents[1] / "tools"))
sys.path.insert(0, str(HERE))

import matchbuild  # noqa: E402
from elftools.elf.elffile import ELFFile  # noqa: E402
import contracts  # noqa: E402
from unicorn import UC_ARCH_MIPS, UC_HOOK_BLOCK, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32, Uc, UcError  # noqa: E402
from unicorn import mips_const as reg  # noqa: E402

RAM_BASE = 0x80000000
RAM_SIZE = 0x200000
SCRATCH_BASE = 0x1F800000
SCRATCH_SIZE = 0x1000  # one page; the console's scratchpad is the first 0x400 bytes
TEST_ADDRESS = 0x80400000  # the build is linked here; physical address 0x400000, outside RAM
TEST_SIZE = 0x10000
STOP_ADDRESS = TEST_ADDRESS + 0xF000  # ra of the calls; never executed
ARENA_BASE = 0x80040000  # free RAM below the resident executable; blocks of the setup
ARENA_END = 0x80100000
STACK_TOP = 0x801FF000  # initial sp
STACK_LOW = 0x801FE000  # the stack region [STACK_LOW, STACK_TOP) is not compared
BUDGET = 2_000_000
HOME_AREA = 16  # the callee's argument home area, sp to sp + 15 on entry (o32): the --writes audit counts it as the function's own
PAGE = 0x1000  # the unit of the first pass of the memory comparison
SAVED = (reg.UC_MIPS_REG_S0, reg.UC_MIPS_REG_S1, reg.UC_MIPS_REG_S2, reg.UC_MIPS_REG_S3,
         reg.UC_MIPS_REG_S4, reg.UC_MIPS_REG_S5, reg.UC_MIPS_REG_S6, reg.UC_MIPS_REG_S7, reg.UC_MIPS_REG_FP)
SAVED_NAMES = ("s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "fp")
ARGS = (reg.UC_MIPS_REG_A0, reg.UC_MIPS_REG_A1, reg.UC_MIPS_REG_A2, reg.UC_MIPS_REG_A3)


class InputError(Exception):
    """A configuration or private input problem."""


class State:
    """RAM and scratchpad of one case, addressed by console addresses."""

    def __init__(self, ram: bytes, scratch: bytes):
        self.ram = bytearray(ram)
        self.scratch = bytearray(scratch)
        self.arena = ARENA_BASE
        self.stop = STOP_ADDRESS  # where a run ends; a recorder that ends the run jumps there
        self.made_ram = bytearray(RAM_SIZE)  # 1 for each byte that the setup made (see `owns`)
        self.made_scratch = bytearray(SCRATCH_SIZE)

    def _locate(self, address: int, size: int) -> tuple[bytearray, int]:
        if size < 0:
            raise ValueError(f"size {size} is negative")
        if RAM_BASE <= address and address + size <= RAM_BASE + RAM_SIZE:
            return self.ram, address - RAM_BASE
        if SCRATCH_BASE <= address and address + size <= SCRATCH_BASE + SCRATCH_SIZE:
            return self.scratch, address - SCRATCH_BASE
        raise ValueError(f"address {address:#x} is outside RAM and the scratchpad")

    def write(self, address: int, data: bytes) -> None:
        block, offset = self._locate(address, len(data))
        block[offset : offset + len(data)] = data
        self._mark(block, offset, len(data))

    def _mark(self, block: bytearray, offset: int, size: int) -> None:
        # `_locate` has refused a negative size and a span outside the memory: a slice with a negative end
        # would shrink or shift the record instead of marking it.
        made = self.made_ram if block is self.ram else self.made_scratch
        made[offset : offset + size] = b"\1" * size

    def owns(self, address: int, size: int) -> None:
        """Mark [address, address + size) as made without writing it (see `--writes`)."""
        block, offset = self._locate(address, size)
        self._mark(block, offset, size)

    def read(self, address: int, size: int) -> bytes:
        block, offset = self._locate(address, size)
        return bytes(block[offset : offset + size])

    def w8(self, address: int, value: int) -> None:
        self.write(address, struct.pack("<B", value & 0xFF))

    def w16(self, address: int, value: int) -> None:
        self.write(address, struct.pack("<H", value & 0xFFFF))

    def w32(self, address: int, value: int) -> None:
        self.write(address, struct.pack("<I", value & 0xFFFFFFFF))

    def alloc(self, size: int) -> int:
        """A zero-initialised block of free RAM, word aligned. The block is made; the padding after it is not."""
        if size < 0:
            raise ValueError(f"size {size} is negative")
        address = self.arena
        end = (address + size + 3) & ~3
        if end > ARENA_END:
            raise ValueError("the setup needs more free RAM than the arena holds")
        self.arena = end
        self.owns(address, size)
        return address


# ---------------------------------------------------------------------------
# Build


@dataclasses.dataclass(frozen=True)
class Build:
    """A linked nonmatching unit."""

    code: bytes  # what is loaded at TEST_ADDRESS: the unit's code, then its read-only data
    size: int  # bytes of code
    entry: int  # offset of the function in the code; a unit may define helpers before it


WRITABLE = (".data", ".sdata", ".bss", ".sbss")
# A line of a linker symbol file that gives a name a value, plain or inside PROVIDE().
ASSIGNMENT = re.compile(r"\s*(?:PROVIDE\s*\(\s*)?([A-Za-z_.$][\w.$]*)\s*=[^=]")


def build_function(cfg, name: str, directory: Path, folder: Path = HERE) -> Build:
    """Compile `name`.c of `folder` and link it at TEST_ADDRESS.

    A unit may hold helper functions and read-only data (a jump table, a
    constant table). It may not define writable data: the game's data lies
    in the image, and data of the unit's own would keep its values from one
    case to the next.
    """
    source = folder / f"{name}.c"
    if not source.is_file():
        raise InputError(f"no source {source.name} in {folder.name}")
    report = {"inputs": {}, "cache": {"units": {}}}
    pipeline, failures = matchbuild.prepare_pipeline(cfg, "difftest", directory, None, report)
    if failures:
        raise matchbuild.StepError("; ".join(failures))
    report["inputs"]["preprocessed"] = {}
    unit = matchbuild.UnitDecl(
        name=name,
        source=str(source),
        flags=("-O2", "-G0"),
        functions=(matchbuild.FunctionDecl(name, TEST_ADDRESS, 0),),
    )
    errors, _ = matchbuild.unit_object(pipeline, unit, directory, report)
    # The object is checked here as a unit of the build that declares no size and no data. Those
    # findings are expected: the code's size is free, and what data a unit may hold is decided below.
    errors = [e for e in errors if "text size mismatch" not in e and "must be declared" not in e]
    if errors:
        raise matchbuild.StepError("; ".join(errors))
    symbols = matchbuild.defined_symbols(directory / f"unit-{name}.o")
    defined = {s.name for s in symbols}
    entry = next((s.offset for s in symbols if s.name == name and s.kind == "text"), None)
    if entry is None:
        raise InputError(f"{source.name} does not define the function {name}")
    sizes = section_sizes(directory / f"unit-{name}.o")
    written = [f"{section} ({sizes[section]} bytes)" for section in WRITABLE if sizes.get(section)]
    if written or any(s.section == "SHN_COMMON" for s in symbols) or sizes.get("COMMON"):
        raise InputError(f"{source.name} defines writable data: {', '.join(written) or 'a common symbol'}")
    # Declared functions of the build that the code may call resolve to their original addresses.
    known = set(cfg.symbol_values)
    lines = [f"{fn.name} = {fn.address:#x};\n" for u in cfg.units for fn in u.functions
             if fn.name not in defined and fn.name not in known]
    (directory / "others.ld").write_text("".join(lines))
    # The tree's symbol file places names at their original addresses, the parked function's
    # own name among them. An assignment there would win over a definition of the unit, and a
    # call by that name would run the ORIGINAL code: every name the unit defines is taken out.
    (directory / "symbols.ld").write_text(without_assignments(Path(cfg.symbols_path).read_text(), defined))
    discard = " ".join(f"*({s})" for s in matchbuild.DISCARDED_SECTIONS)
    (directory / "link.ld").write_text(
        "OUTPUT_ARCH(mips)\n"
        f'INCLUDE "{(directory / "symbols.ld").resolve()}"\n'
        f'INCLUDE "{(directory / "others.ld").resolve()}"\n'
        "SECTIONS {\n"
        f" .text {TEST_ADDRESS:#x} : SUBALIGN(1) {{ unit-{name}.o(.text) }}\n"
        f" .rodata : {{ unit-{name}.o(.rodata*) }}\n"
        f" /DISCARD/ : {{ {discard} *(.note*) }}\n"
        "}\n"
    )
    prefix = cfg.binutils_prefix
    matchbuild.run([prefix + "ld", "-EL", "-T", "link.ld", "-e", f"{TEST_ADDRESS:#x}", "-o", "image.elf", f"unit-{name}.o"],
                   cwd=directory, step="link")
    matchbuild.run([prefix + "objcopy", "-O", "binary", "image.elf", "image.bin"], cwd=directory, step="objcopy")
    code = (directory / "image.bin").read_bytes()
    if len(code) > STOP_ADDRESS - TEST_ADDRESS:
        raise InputError(f"{source.name} builds {len(code)} bytes, more than the test area holds")
    # Fail closed: whatever the link did, a name the unit defines must lie in the unit.
    astray = misplaced(linked_addresses(directory / "image.elf"), defined, TEST_ADDRESS, TEST_ADDRESS + len(code))
    if astray:
        raise InputError(f"{source.name}: the link placed outside the unit what the unit defines: {', '.join(astray)}")
    return Build(code, sizes.get(".text", 0), entry)


def without_assignments(text: str, names: set[str]) -> str:
    """A linker symbol file without the lines that assign one of `names`."""
    kept = []
    for line in text.splitlines(keepends=True):
        match = ASSIGNMENT.match(line)
        if match is None or match.group(1) not in names:
            kept.append(line)
    return "".join(kept)


def linked_addresses(elf_path: Path) -> dict[str, int]:
    """The address of every named symbol of a linked file."""
    with open(elf_path, "rb") as handle:
        symtab = ELFFile(handle).get_section_by_name(".symtab")
        return {sym.name: sym["st_value"] for sym in symtab.iter_symbols() if sym.name} if symtab else {}


def misplaced(linked: dict[str, int], names: set[str], low: int, high: int) -> list[str]:
    """The names of `names` that the link did not place in [low, high), with where it put them."""
    return [f"{name} at {linked[name]:#x}" if name in linked else f"{name} nowhere"
            for name in sorted(names) if not low <= linked.get(name, -1) < high]


def section_sizes(obj_path: Path) -> dict[str, int]:
    """The size of each section of an object, and of its common symbols under the name COMMON."""
    with open(obj_path, "rb") as handle:
        elf = ELFFile(handle)
        sizes = {section.name: section["sh_size"] for section in elf.iter_sections()}
        symtab = elf.get_section_by_name(".symtab")
        common = sum(sym["st_size"] for sym in symtab.iter_symbols() if sym["st_shndx"] == "SHN_COMMON") if symtab else 0
    if common:
        sizes["COMMON"] = common
    return sizes


# ---------------------------------------------------------------------------
# Memory image and emulation


def initial_memory(cfg, image: str | None) -> bytes:
    """RAM with the resident executable and the module image `image` (None: none) at their load addresses.

    Several module images share one address, so exactly the image of the function is loaded.
    """
    ram = bytearray(RAM_SIZE)
    images = [(cfg.load, cfg.payload)] + [(i.address, i.payload) for i in cfg.images if i.name == image]
    for address, data in images:
        offset = address - RAM_BASE
        if not data or offset < 0 or offset + len(data) > RAM_SIZE:
            raise InputError(f"an image of {len(data)} bytes does not fit RAM at {address:#x}")
        ram[offset : offset + len(data)] = data
    return bytes(ram)


def machine(code: bytes) -> Uc:
    uc = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
    # Unicorn translates the console's kseg0 addresses to physical ones.
    uc.mem_map(0, RAM_SIZE)
    uc.mem_map(SCRATCH_BASE, SCRATCH_SIZE)
    uc.mem_map(TEST_ADDRESS & 0x1FFFFFFF, TEST_SIZE)
    # The host side of Unicorn takes physical addresses.
    uc.mem_write(TEST_ADDRESS & 0x1FFFFFFF, code)
    uc.mem_write(STOP_ADDRESS & 0x1FFFFFFF, b"\0\0\0\0")
    return uc


def run_once(uc: Uc, state: State, entry: int, setup: contracts.Setup, blocks: set | None = None) -> dict | str:
    """Run one call on `state`. Returns the final state, or a text naming the failure.

    With `blocks`, every block of code that the run enters is added to it as (address, size).
    """
    uc.mem_write(0, bytes(state.ram))
    uc.mem_write(SCRATCH_BASE, bytes(state.scratch))
    # A setup may write code (a recorder in a callee's place) that differs from case to case.
    # Unicorn keeps translated code across a write to memory: without this the first case's code would run again.
    uc.ctl_flush_tb()
    for number in range(reg.UC_MIPS_REG_0, reg.UC_MIPS_REG_31 + 1):
        uc.reg_write(number, 0)
    for index, number in enumerate(SAVED):
        uc.reg_write(number, 0x5A5A0000 + index)
    for number, value in zip(ARGS, setup.args):
        uc.reg_write(number, value)
    uc.reg_write(reg.UC_MIPS_REG_RA, STOP_ADDRESS)
    uc.reg_write(reg.UC_MIPS_REG_SP, STACK_TOP)
    hook = None
    if blocks is not None:
        hook = uc.hook_add(UC_HOOK_BLOCK, lambda _uc, address, size, _data: blocks.add((address & 0xFFFFFFFF, size)))
    try:
        uc.emu_start(entry, STOP_ADDRESS, count=BUDGET)
    except UcError as exc:
        return f"fault ({exc})"
    finally:
        if hook is not None:
            uc.hook_del(hook)
    if uc.reg_read(reg.UC_MIPS_REG_PC) != STOP_ADDRESS:
        return "instruction budget exceeded"
    return {
        "ram": uc.mem_read(0, RAM_SIZE),
        "scratch": uc.mem_read(SCRATCH_BASE, SCRATCH_SIZE),
        "v0": uc.reg_read(reg.UC_MIPS_REG_V0),
        "saved": [uc.reg_read(n) for n in SAVED],
        "sp": uc.reg_read(reg.UC_MIPS_REG_SP),
    }


def differences(a: dict, b: dict, returns_value: bool, returns: bool = True) -> list[str]:
    """What differs between two final states, as text lines (empty when equal).

    With `returns` false the runs were ended inside the function, and no register is compared.
    """
    found = []
    if returns and returns_value and a["v0"] != b["v0"]:
        found.append(f"v0: original {a['v0']:#x}, build {b['v0']:#x}")
    for name, x, y in zip(SAVED_NAMES, a["saved"], b["saved"]):
        if returns and x != y:
            found.append(f"{name}: original {x:#x}, build {y:#x}")
    if returns and a["sp"] != b["sp"]:
        found.append(f"sp: original {a['sp']:#x}, build {b['sp']:#x}")
    low, high = STACK_LOW - RAM_BASE, STACK_TOP - RAM_BASE
    for label, base, x, y, skip in (("ram", RAM_BASE, a["ram"], b["ram"], (low, high)),
                                    ("scratchpad", SCRATCH_BASE, a["scratch"], b["scratch"], (0, 0))):
        # Whole pages are compared first: a byte loop over all of RAM costs seconds a case.
        for start in range(0, len(x), PAGE):
            if x[start : start + PAGE] == y[start : start + PAGE]:
                continue
            for i in range(start, min(start + PAGE, len(x))):
                if x[i] != y[i] and not skip[0] <= i < skip[1]:
                    found.append(f"{label} {base + i:#x}: original {x[i]:#04x}, build {y[i]:#04x}")
    return found


def image_of(name: str) -> str | None:
    """The module image in the name `func_<address>_<image>`, None for the resident executable."""
    return name.split("_", 2)[2] if name.count("_") >= 2 else None


def original_function(cfg, name: str) -> tuple[int, int]:
    """Address and size of the original function that `name` stands for.

    The name is `func_<address>` for the resident executable or
    `func_<address>_<image>` for a module image. The address is in the name;
    the size comes from the published function inventory (`ps1/inventory/`),
    because a nonmatching function is not declared in the build.
    """
    match = re.fullmatch(r"func_([0-9a-f]{8})(?:_(\w+))?", name)
    if match is None:
        raise InputError(f"{name} is not named func_<address>[_<image>]")
    address = int(match.group(1), 16)
    inventory = HERE.parents[1] / "inventory"
    if match.group(2) is None:
        rows = [line.split("\t") for line in (inventory / "game.tsv").read_text().splitlines()]
        sizes = [int(r[1]) for r in rows if int(r[0], 16) == address]
    else:
        image = next((i for i in cfg.images if i.name == match.group(2)), None)
        if image is None:
            raise InputError(f"{name}: the build declares no module image {match.group(2)!r}")
        rows = [line.split("\t") for line in (inventory / "modules.tsv").read_text().splitlines()]
        sizes = [int(r[3]) for r in rows if r[0] == image.archive.name and int(r[1], 16) == image.slot and int(r[2], 16) == address]
    if len(sizes) != 1:
        raise InputError(f"{name}: the inventory lists {len(sizes)} functions at {address:#x}")
    return address, sizes[0]


def slots_in(blocks: set, start: int, size: int) -> set[int]:
    """The instruction slots of [start, start + size) that lie in one of the entered blocks, as offsets."""
    slots: set[int] = set()
    for address, length in blocks:
        for at in range(max(address, start), min(address + length, start + size), 4):
            slots.add(at - start)
    return slots


def ranges_text(offsets: list[int]) -> str:
    """Sorted instruction offsets as text, runs joined: `+0x40..+0x4c, +0x88`."""
    parts = []
    for offset in offsets:
        if parts and parts[-1][1] + 4 == offset:
            parts[-1][1] = offset
        else:
            parts.append([offset, offset])
    return ", ".join(f"+{a:#x}" if a == b else f"+{a:#x}..+{b:#x}" for a, b in parts)


def addresses(cfg) -> dict[str, int]:
    """What a contract's setup is given as `sym`: the names of the tree's linker symbols
    and of the functions that the build declares, each with its address."""
    table = {fn.name: fn.address for unit in getattr(cfg, "units", ()) for fn in unit.functions}
    table.update(cfg.symbol_values)
    return table


def test_function(cfg, name: str, code: bytes, cases: int, seed: int, ram: bytes, scratch: bytes, entry: int = 0,
                  stop_at_first_difference: bool = False) -> tuple[int, int, int, list, set]:
    """Run the cases. `entry` is the offset of the function in `code`.

    Returns (discarded, equal, different, first difference report, executed): `executed` holds
    the offsets of the original function's instruction slots that the runs of the original
    which reached their end have executed. With `stop_at_first_difference` the loop ends after the
    first case that differs (only `--edges` asks for it; the counts are then those of the cases run).
    """
    contract = contracts.CONTRACTS[name]
    original, size = original_function(cfg, name)
    uc = machine(code)
    sym = addresses(cfg)
    discarded = equal = different = 0
    first: list[str] = []
    executed: set[int] = set()
    for case in range(cases):
        rng = random.Random(f"{seed}:{name}:{case}")
        state = State(ram, scratch)
        setup = contract.setup(state, rng, sym)
        blocks: set = set()
        reference = run_once(uc, state, original, setup, blocks)
        if isinstance(reference, str):
            discarded += 1
            continue
        executed |= slots_in(blocks, original, size)
        built = run_once(uc, state, TEST_ADDRESS + entry, setup)
        if isinstance(built, str):
            lines = [f"build: {built}"]
        else:
            lines = differences(reference, built, setup.returns_value, getattr(setup, "returns", True))
        if lines:
            different += 1
            if not first:
                first = [f"first difference: case {case}, seed {seed}", *lines[:24]]
                if len(lines) > 24:
                    first.append(f"... and {len(lines) - 24} more")
            if stop_at_first_difference:
                break
        else:
            equal += 1
    return discarded, equal, different, first, executed


def outside_addresses(state: State, final: dict) -> list[int]:
    """The addresses of the bytes that the run changed and that are neither made nor in the stack region."""
    low, high = STACK_LOW - RAM_BASE, STACK_TOP + HOME_AREA - RAM_BASE
    if len(state.made_ram) != RAM_SIZE or len(state.made_scratch) != SCRATCH_SIZE:
        raise InputError("the record of what the setup made has another size than the memory it describes")
    found = []
    for base, before, after, made, skip in ((RAM_BASE, state.ram, final["ram"], state.made_ram, (low, high)),
                                            (SCRATCH_BASE, state.scratch, final["scratch"], state.made_scratch, (0, 0))):
        for start in range(0, len(before), PAGE):
            if before[start : start + PAGE] == after[start : start + PAGE]:
                continue
            for i in range(start, min(start + PAGE, len(before))):
                if before[i] != after[i] and not made[i] and not skip[0] <= i < skip[1]:
                    found.append(base + i)
    return found


def runs_text(found: list[int], sym: dict) -> str:
    """Addresses (RAM ones, then scratchpad ones, each ascending) as `0xA..0xB (NAME+0xOFF); ...`:
    runs of bytes less than 17 apart, six at most, then `and M more`."""
    names = sorted((v, k) for k, v in sym.items() if isinstance(v, int))
    runs = []
    for address in found:
        if runs and 0 <= address - runs[-1][1] <= 16:
            runs[-1][1] = address
        else:
            runs.append([address, address])
    parts = []
    for first, last in runs[:6]:
        region = RAM_BASE if first >= RAM_BASE else SCRATCH_BASE
        size = RAM_SIZE if first >= RAM_BASE else SCRATCH_SIZE
        i = bisect.bisect_right(names, (first, "~")) - 1
        near = f"{names[i][1]}+{first - names[i][0]:#x}" if i >= 0 and region <= names[i][0] < region + size else "?"
        parts.append(f"{first:#x}..{last:#x} ({near})")
    if len(runs) > 6:
        parts.append(f"and {len(runs) - 6} more")
    return "; ".join(parts)


def audit_writes(cfg, name: str, cases: int, seed: int, ram: bytes, scratch: bytes) -> tuple[int, int, int, str]:
    """Run the original alone on the cases and count those that change a byte outside what the setup made.

    Returns (discarded, outside, largest, first): `largest` is the most such bytes in one case;
    `first` is the text `case C; <runs>` of the first such case, empty when there is none.
    """
    contract = contracts.CONTRACTS[name]
    original, _size = original_function(cfg, name)
    uc = machine(b"\0" * 16)
    sym = addresses(cfg)
    discarded = outside = largest = 0
    first = ""
    for case in range(cases):
        rng = random.Random(f"{seed}:{name}:{case}")
        state = State(ram, scratch)
        setup = contract.setup(state, rng, sym)
        final = run_once(uc, state, original, setup)
        if isinstance(final, str):
            discarded += 1
            continue
        found = outside_addresses(state, final)
        if found:
            outside += 1
            largest = max(largest, len(found))
            if not first:
                first = f"case {case}; {runs_text(found, sym)}"
    return discarded, outside, largest, first


# ---------------------------------------------------------------------------
# Edges of the constants (--edges)

REGISTER_NAMES = ("zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
                  "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra")
SP, GP, RA_NUMBER = 29, 28, 31
# opcode -> mnemonic of the immediate instructions that the sweep may take
OP_ADDIU, OP_SLTI, OP_SLTIU, OP_ORI, OP_LUI = 0x09, 0x0A, 0x0B, 0x0D, 0x0F
EDGE_MNEMONICS = {OP_SLTI: "slti", OP_SLTIU: "sltiu", OP_ADDIU: "addiu", OP_ORI: "ori"}
# SPECIAL functions that write no register (jr, mult, div, mthi, mtlo, syscall, break)
SPECIAL_NO_WRITE = frozenset({0x08, 0x11, 0x13, 0x18, 0x19, 0x1A, 0x1B, 0x0C, 0x0D})
# SPECIAL functions that write rd
SPECIAL_WRITES_RD = frozenset({0x00, 0x02, 0x03, 0x04, 0x06, 0x07, 0x09, 0x10, 0x12, 0x20, 0x21, 0x22, 0x23,
                               0x24, 0x25, 0x26, 0x27, 0x2A, 0x2B})


def register_written(word: int) -> int | None:
    """The register that the instruction `word` writes, None when it writes none.

    Raises InputError for a word whose opcode or function this table does not know: the rule for the
    low half of a constant needs to know what a word writes, and it is not guessed.
    """
    op, rs, rt, rd, funct = word >> 26, (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31, word & 63
    if op == 0:
        if funct in SPECIAL_WRITES_RD:
            return rd
        if funct in SPECIAL_NO_WRITE:
            return None
    elif op == 1:  # REGIMM: bltzal and bgezal write ra
        return RA_NUMBER if rt in (0x10, 0x11) else None
    elif op == 3:  # jal
        return RA_NUMBER
    elif op in (2, 4, 5, 6, 7):  # j and the branches
        return None
    elif 0x08 <= op <= 0x0F or 0x20 <= op <= 0x26:  # arithmetic immediates, lui; loads
        return rt
    elif 0x28 <= op <= 0x2E or op in (0x32, 0x3A):  # stores; lwc2, swc2
        return None
    elif op == 0x10:  # COP0: mfc0 writes rt
        return rt if rs == 0 else None
    elif op == 0x12:  # COP2 (the GTE): mfc2 and cfc2 write rt
        return rt if rs in (0, 2) else None
    raise InputError(f"word {word:#010x}: the tool does not know which register it writes")


def is_low_half(words: list[int], index: int, reg_number: int) -> bool:
    """True when the nearest earlier word of `words` that writes `reg_number` is `lui reg_number`."""
    for earlier in range(index - 1, -1, -1):
        written = register_written(words[earlier])
        if written == reg_number:
            return words[earlier] >> 26 == OP_LUI
    return False


def edge_constant(words: list[int], index: int) -> tuple[str, int, int, int] | None:
    """(mnemonic, rs, rt, immediate) when word `index` of `words` is a constant of the sweep, else None."""
    word = words[index]
    op, rs, rt, imm = word >> 26, (word >> 21) & 31, (word >> 16) & 31, word & 0xFFFF
    if op in (OP_SLTI, OP_SLTIU):
        return EDGE_MNEMONICS[op], rs, rt, imm
    if op == OP_ADDIU:
        if SP in (rs, rt) or GP in (rs, rt):
            return None
        if rs == rt and is_low_half(words, index, rt):
            return None
        return "addiu", rs, rt, imm
    if op == OP_ORI and rs == 0:
        return "ori", rs, rt, imm
    return None


def edge_constants(words: list[int]) -> list[tuple[int, str, int, int, int]]:
    """The constants of the sweep in a function's words, as (index, mnemonic, rs, rt, immediate)."""
    found = []
    for index in range(len(words)):
        constant = edge_constant(words, index)
        if constant is not None:
            found.append((index, *constant))
    return found


def edge_neighbours(immediate: int) -> tuple[int, int]:
    """The immediate plus one and minus one, each modulo 0x10000."""
    return (immediate + 1) & 0xFFFF, (immediate - 1) & 0xFFFF


def operands_text(mnemonic: str, rs: int, rt: int, immediate: int) -> str:
    return f"{REGISTER_NAMES[rt]},{REGISTER_NAMES[rs]},{immediate:#x}"


def edge_line(index: int, mnemonic: str, rs: int, rt: int, old: int, new: int, different_of: int, discarded: int) -> str:
    return (f"  slot {index}: {mnemonic} {operands_text(mnemonic, rs, rt, old)}, immediate {old:#x} -> {new:#x}: "
            f"different 0 of {different_of}, discarded {discarded}")


# What the workers of an `--edges` pool (or the one process, with `--jobs 1`) share: set by `edge_init`.
EDGE_CONTEXT: dict = {}


def edge_init(context: dict) -> None:
    EDGE_CONTEXT.clear()
    EDGE_CONTEXT.update(context)


def edge_run(task: tuple[int, int, int]) -> tuple[int, int, int, int]:
    """One altered run: the default comparison with word `index` of the original replaced by one with
    the immediate `new`, ended at its first differing case. Returns (index, which, discarded, different).

    The image is altered in place and put back, whatever happens, before the next run of this process.
    """
    index, which, new = task
    c = EDGE_CONTEXT
    at = c["base"] + 4 * index
    word = c["words"][index]
    image = c["image"]
    image[at : at + 4] = struct.pack("<I", (word & 0xFFFF0000) | new)
    try:
        gone, _equal, changed, _first, _executed = test_function(
            c["cfg"], c["name"], c["code"], c["cases"], c["seed"], bytes(image), c["scratch"], c["entry"],
            stop_at_first_difference=True)
    finally:
        image[at : at + 4] = struct.pack("<I", word)
    return index, which, gone, changed


def edge_runs(context: dict, tasks: list[tuple[int, int, int]], jobs: int) -> list[tuple[int, int, int, int]]:
    """The results of `edge_run` for the tasks, in the order they finish (the caller sorts them)."""
    if not tasks:
        return []
    if jobs == 1:
        edge_init(context)
        try:
            return [edge_run(task) for task in tasks]
        finally:
            EDGE_CONTEXT.clear()
    try:
        fork = multiprocessing.get_context("fork")
    except ValueError as exc:
        raise InputError("--jobs above 1 needs the fork start method of this system; use --jobs 1") from exc
    with concurrent.futures.ProcessPoolExecutor(max_workers=min(jobs, len(tasks)), mp_context=fork,
                                                initializer=edge_init, initargs=(context,)) as pool:
        futures = [pool.submit(edge_run, task) for task in tasks]
        return [future.result() for future in concurrent.futures.as_completed(futures)]


def edge_report(name: str, cases: int, constants: list, executed: set, results: list, uncovered: bool) -> tuple[list[str], int]:
    """The lines of one function and the status, from the results of its altered runs in any order.

    `constants` are (index, mnemonic, rs, rt, immediate); a result is (index, which, discarded, different),
    `which` 0 for the immediate plus one and 1 for minus one.
    """
    by_constant = {c[0]: c for c in constants}
    runs = unnoticed = all_discarded = 0
    lines: list[str] = []
    for index, which, gone, changed in sorted(results):
        _i, mnemonic, rs, rt, immediate = by_constant[index]
        runs += 1
        if changed:
            continue
        if gone == cases:
            all_discarded += 1
        else:
            unnoticed += 1
            lines.append(edge_line(index, mnemonic, rs, rt, immediate, edge_neighbours(immediate)[which], cases, gone))
    skipped = [f"  slot {c[0]}: not executed by any case" for c in constants if 4 * c[0] not in executed]
    head = (f"{name} edges: constants {len(constants)}, altered runs {runs}, unnoticed {unnoticed}, "
            f"all discarded {all_discarded}")
    return [head, *lines, *(skipped if uncovered else [])], 1 if unnoticed else 0


def edge_sweep(cfg, name: str, build: Build, cases: int, seed: int, ram: bytes, scratch: bytes,
               uncovered: bool, jobs: int = 1) -> tuple[list[str], int]:
    """The edges sweep of one function: (lines to print, status 0 or 1).

    The default comparison runs first. The words of the original are scanned in `ram`; the altered runs
    are made by `jobs` processes (in this one with 1) and printed in a fixed order.
    """
    address, size = original_function(cfg, name)
    discarded, _equal, different, _first, executed = test_function(cfg, name, build.code, cases, seed, ram, scratch, build.entry)
    if different or discarded:
        return [f"{name} edges: not swept, the comparison without alteration shows different {different}, "
                f"discarded {discarded}"], 1
    base = address - RAM_BASE
    image = bytearray(ram)
    words = list(struct.unpack(f"<{size // 4}I", bytes(image[base : base + size - size % 4])))
    constants = edge_constants(words)
    tasks = [(index, which, new) for index, _m, _rs, _rt, immediate in constants if 4 * index in executed
             for which, new in enumerate(edge_neighbours(immediate))]
    context = {"cfg": cfg, "name": name, "code": build.code, "entry": build.entry, "cases": cases, "seed": seed,
               "scratch": scratch, "base": base, "words": words, "image": image}
    return edge_report(name, cases, constants, executed, edge_runs(context, tasks, jobs), uncovered)


def load_contracts(folder: Path, names: list[str]) -> None:
    """Take the contract of each name that has a file `NAME.py` in `folder` into `contracts.CONTRACTS`."""
    for name in names:
        path = folder / f"{name}.py"
        if name in contracts.CONTRACTS or not path.is_file():
            continue
        spec = importlib.util.spec_from_file_location(f"contract_{name}", path)
        module = importlib.util.module_from_spec(spec)
        try:
            spec.loader.exec_module(module)
        except Exception as exc:  # a contract file is input: whatever it raises is reported, not a traceback
            raise InputError(f"{path.name} cannot be loaded: {type(exc).__name__}: {exc}") from exc
        if not isinstance(getattr(module, "CONTRACT", None), contracts.Contract):
            raise InputError(f"{path.name} does not define CONTRACT, a contracts.Contract")
        contracts.CONTRACTS[name] = module.CONTRACT


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Differential test of nonmatching C against the original code")
    parser.add_argument("--config", type=Path, required=True, help="the build configuration (build.toml)")
    parser.add_argument("--folder", type=Path, default=HERE,
                        help="the folder of the sources and of their contract files (default: the tool's own)")
    parser.add_argument("--cases", type=int, default=1000)
    parser.add_argument("--seed", type=int, default=1)
    parser.add_argument("--control", action="store_true",
                        help="negative control: alter one instruction of the build and require differences")
    parser.add_argument("--uncovered", action="store_true",
                        help="list the instruction slots of the original that no case executed "
                             "(with --writes: where the first case outside wrote)")
    parser.add_argument("--writes", action="store_true",
                        help="audit where the original writes, without building the C: cases that change "
                             "bytes the setup did not make")
    parser.add_argument("--edges", action="store_true",
                        help="alter one constant of the original at a time (plus and minus one) and list the "
                             "alterations that no case notices")
    parser.add_argument("--jobs", type=int, default=None,
                        help="with --edges: processes that make the altered runs (default 8; 1 runs them here)")
    parser.add_argument("--all", action="store_true", help="every function that has a source in the folder")
    parser.add_argument("functions", nargs="*", metavar="FUNC")
    args = parser.parse_args(argv)
    folder = args.folder.resolve()
    if args.writes and args.control:
        print("INPUT ERROR: --writes and --control do not combine", file=sys.stderr)
        return 2
    if args.edges and args.control:
        print("INPUT ERROR: --edges and --control do not combine", file=sys.stderr)
        return 2
    if args.edges and args.writes:
        print("INPUT ERROR: --edges and --writes do not combine", file=sys.stderr)
        return 2
    if args.jobs is not None and not args.edges:
        print("INPUT ERROR: --jobs applies to --edges only", file=sys.stderr)
        return 2
    if args.jobs is not None and args.jobs < 1:
        print("INPUT ERROR: --jobs needs 1 or more", file=sys.stderr)
        return 2
    if args.all == bool(args.functions):
        print("INPUT ERROR: name the functions, or give --all and none", file=sys.stderr)
        return 2
    if args.all:
        args.functions = sorted(path.stem for path in folder.glob("func_*.c"))
        if not args.functions:
            print(f"INPUT ERROR: no func_*.c in {folder.name}", file=sys.stderr)
            return 2

    try:
        cfg = matchbuild.load_config(args.config.resolve())
    except matchbuild.ConfigError as exc:
        for error in exc.errors:
            print(f"CONFIG ERROR: {error}", file=sys.stderr)
        return 2
    except (InputError, OSError) as exc:
        print(f"INPUT ERROR: {exc}", file=sys.stderr)
        return 2
    scratch = bytes(SCRATCH_SIZE)
    try:
        load_contracts(folder, args.functions)
    except (InputError, OSError) as exc:
        print(f"INPUT ERROR: {exc}", file=sys.stderr)
        return 2

    status = 0
    for name in args.functions:
        if name not in contracts.CONTRACTS:
            print(f"INPUT ERROR: no contract for {name}", file=sys.stderr)
            return 2
        try:
            original_size = original_function(cfg, name)[1]
            ram = initial_memory(cfg, image_of(name))
        except (InputError, OSError) as exc:
            print(f"INPUT ERROR: {exc}", file=sys.stderr)
            return 2
        if args.writes:
            discarded, outside, largest, first = audit_writes(cfg, name, args.cases, args.seed, ram, scratch)
            print(f"{name} writes: cases {args.cases}, discarded {discarded}, outside {outside} (largest {largest} bytes)")
            if outside and args.uncovered:
                print(f"  first: {first}")
            if outside:
                status = 1
            continue
        with tempfile.TemporaryDirectory(prefix="difftest-") as directory:
            try:
                build = build_function(cfg, name, Path(directory), folder)
            except (matchbuild.StepError, matchbuild.EnvironmentFailure, InputError) as exc:
                print(f"BUILD ERROR: {exc}", file=sys.stderr)
                return 3
        if args.edges:
            try:
                lines, swept = edge_sweep(cfg, name, build, args.cases, args.seed, ram, scratch, args.uncovered,
                                         8 if args.jobs is None else args.jobs)
            except (InputError, OSError) as exc:
                print(f"INPUT ERROR: {exc}", file=sys.stderr)
                return 2
            for line in lines:
                print(line)
            status = max(status, swept)
            continue
        code = build.code
        if args.control:
            # The control sees the unit's code, not its read-only data, and may alter one word of it.
            words = list(struct.unpack(f"<{build.size // 4}I", code[: build.size]))
            index, word, what = contracts.CONTRACTS[name].control(words)
            if not 0 <= index < len(words):
                print(f"INPUT ERROR: the control of {name} names word {index} of {len(words)}", file=sys.stderr)
                return 2
            code = code[: 4 * index] + struct.pack("<I", word) + code[4 * index + 4 :]
        discarded, equal, different, first, executed = test_function(cfg, name, code, args.cases, args.seed, ram, scratch, build.entry)
        if args.control:
            print(f"{name} control: different {different} of {args.cases} (expected more than 0)")
            print(f"  altered: {what}, instruction slot {index}")
            if different == 0:
                status = 1
            continue
        print(f"{name}: built {build.size} bytes, original {original_size} bytes; "
              f"cases {args.cases}, discarded {discarded}, equal {equal}, different {different}")
        for line in first:
            print(f"  {line}")
        slots = original_size // 4
        print(f"{name} coverage: {len(executed)} of {slots} instruction slots of the original executed")
        if args.uncovered and len(executed) < slots:
            print(f"  not executed: {ranges_text([o for o in range(0, original_size, 4) if o not in executed])}")
        if different or not equal:
            status = 1
    return status


if __name__ == "__main__":
    sys.exit(main())
