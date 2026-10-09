#!/usr/bin/env python3
"""Differential test of a nonmatching C function against the original code.

    difftest.py --config ../build.toml [--folder DIR] [--cases N] [--seed S]
                [--control] [--uncovered] (FUNC... | --all)

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

The private inputs (the baseline executable and the module archive that
the build configuration names) are read through the configuration; the
tool stops with a message when they are absent.

Exit status: 0 when every function passes, 1 for a difference (or, with
--control, when the control does not trip), 2 for a configuration or input
problem, 3 when the build fails.
"""

from __future__ import annotations

import argparse
import dataclasses
import importlib.util
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

    def _locate(self, address: int, size: int) -> tuple[bytearray, int]:
        if RAM_BASE <= address and address + size <= RAM_BASE + RAM_SIZE:
            return self.ram, address - RAM_BASE
        if SCRATCH_BASE <= address and address + size <= SCRATCH_BASE + SCRATCH_SIZE:
            return self.scratch, address - SCRATCH_BASE
        raise ValueError(f"address {address:#x} is outside RAM and the scratchpad")

    def write(self, address: int, data: bytes) -> None:
        block, offset = self._locate(address, len(data))
        block[offset : offset + len(data)] = data

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
        """A zero-initialised block of free RAM, word aligned."""
        address = self.arena
        self.arena = (self.arena + size + 3) & ~3
        if self.arena > ARENA_END:
            raise ValueError("the setup needs more free RAM than the arena holds")
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
    discard = " ".join(f"*({s})" for s in matchbuild.DISCARDED_SECTIONS)
    (directory / "link.ld").write_text(
        "OUTPUT_ARCH(mips)\n"
        f'INCLUDE "{cfg.symbols_path}"\n'
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
    return Build(code, sizes.get(".text", 0), entry)


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


def differences(a: dict, b: dict, returns_value: bool) -> list[str]:
    """What differs between two final states, as text lines (empty when equal)."""
    found = []
    if returns_value and a["v0"] != b["v0"]:
        found.append(f"v0: original {a['v0']:#x}, build {b['v0']:#x}")
    for name, x, y in zip(SAVED_NAMES, a["saved"], b["saved"]):
        if x != y:
            found.append(f"{name}: original {x:#x}, build {y:#x}")
    if a["sp"] != b["sp"]:
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


def test_function(cfg, name: str, code: bytes, cases: int, seed: int, ram: bytes, scratch: bytes, entry: int = 0) -> tuple[int, int, int, list, set]:
    """Run the cases. `entry` is the offset of the function in `code`.

    Returns (discarded, equal, different, first difference report, executed): `executed` holds
    the offsets of the original function's instruction slots that the runs of the original
    which reached their end have executed.
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
            lines = differences(reference, built, setup.returns_value)
        if lines:
            different += 1
            if not first:
                first = [f"first difference: case {case}, seed {seed}", *lines[:24]]
                if len(lines) > 24:
                    first.append(f"... and {len(lines) - 24} more")
        else:
            equal += 1
    return discarded, equal, different, first, executed


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
                        help="list the instruction slots of the original that no case executed")
    parser.add_argument("--all", action="store_true", help="every function that has a source in the folder")
    parser.add_argument("functions", nargs="*", metavar="FUNC")
    args = parser.parse_args(argv)
    folder = args.folder.resolve()
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
        with tempfile.TemporaryDirectory(prefix="difftest-") as directory:
            try:
                build = build_function(cfg, name, Path(directory), folder)
            except (matchbuild.StepError, matchbuild.EnvironmentFailure, InputError) as exc:
                print(f"BUILD ERROR: {exc}", file=sys.stderr)
                return 3
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
