#!/usr/bin/env python3
"""Show where a unit's compiled code differs from the baseline.

Reads the unit object left in the build directory by the last matchbuild run
for the given tag, links it alone at the unit's start address, and prints an
aligned instruction diff against the baseline bytes of the unit's declared
range. This works when the unit does not match and the whole-image link was
never reached. It is a diagnostic: matchbuild remains the only authority on
whether a build is exact.

Exit status: 0 identical, 1 different, 2 unusable input.
"""

from __future__ import annotations

import argparse
import difflib
import os
import shutil
import subprocess
import sys
from pathlib import Path

from capstone import CS_ARCH_MIPS, CS_MODE_LITTLE_ENDIAN, CS_MODE_MIPS32, Cs
from elftools.elf.elffile import ELFFile

sys.path.insert(0, str(Path(__file__).resolve().parent))
from matchbuild import (  # noqa: E402
    BSS_SECTIONS,
    DISCARDED_SECTIONS,
    LOADED_SECTIONS,
    ConfigError,
    EnvironmentFailure,
    StepError,
    cache_directory,
    defined_symbols,
    load_config,
    named,
    prepare_pipeline,
    symbol_address,
    unit_object,
)


def words(data: bytes) -> list[bytes]:
    return [data[i : i + 4] for i in range(0, len(data) - len(data) % 4, 4)]


def link_alone(cfg, unit, build: Path) -> Path:
    """Link one unit object at its start address with every other symbol of its image as an address."""
    obj = build / f"unit-{unit.name}.o"
    if not obj.is_file():
        raise SystemExit(named(unit.image, f"no object {obj}: run matchbuild with the same --tag first"))
    neighbours = cfg.units_of(unit.image)
    others = [fn for other in neighbours if other.name != unit.name for fn in other.functions]
    assigned = {fn.name for fn in others}
    siblings = []
    for other in neighbours:
        sibling = build / f"unit-{other.name}.o"
        if other.name == unit.name or not sibling.is_file():
            continue
        # Variables and tables of a sibling sit at its declared ranges plus their offset in its object.
        for sym in defined_symbols(sibling):
            address = symbol_address(other, sym)
            if address is not None and sym.name not in assigned:
                assigned.add(sym.name)
                siblings.append((sym.name, address))
    script = build / f"unit-{unit.name}.fndiff.ld"
    placed = ""
    for kind, decl in unit.loaded():
        # Addresses of tables and variables in the code depend on where they sit.
        placed += f" .{kind} {decl.address:#x} : SUBALIGN(1) {{ " + " ".join(f"*({s})" for s in LOADED_SECTIONS[kind]) + " }\n"
    if unit.bss is not None:
        placed += f" .bss {unit.bss.address:#x} (NOLOAD) : SUBALIGN(1) {{ " + " ".join(f"*({s})" for s in BSS_SECTIONS) + " }\n"
    script.write_text(
        f'INCLUDE "{cfg.symbols_path}"\n'
        + "".join(f"{fn.name} = {fn.address:#x};\n" for fn in others)
        + "".join(f"{name} = {address:#x};\n" for name, address in siblings)
        + "SECTIONS {\n"
        + f" .text {unit.start:#x} : SUBALIGN(1) {{ *(.text) }}\n"
        + placed
        + " /DISCARD/ : { " + " ".join(f"*({s})" for s in DISCARDED_SECTIONS) + " *(.note*) }\n"
        + "}\n"
    )
    elf = build / f"unit-{unit.name}.fndiff.elf"
    proc = subprocess.run(
        [cfg.binutils_prefix + "ld", "-EL", "-T", script, "-e", f"{unit.start:#x}", "-o", elf, obj],
        capture_output=True,
        text=True,
    )
    if proc.returncode != 0:
        raise SystemExit(named(unit.image, f"cannot link unit {unit.name!r} alone:\n{proc.stderr}{proc.stdout}"))
    return elf


def built_code(elf_path: Path) -> tuple[int, bytes, dict[str, tuple[int, int]]]:
    """Return (text address, text bytes, {function: (address, size)}) of a linked unit."""
    with open(elf_path, "rb") as handle:
        elf = ELFFile(handle)
        text = elf.get_section_by_name(".text")
        symbols = {
            s.name: (s["st_value"], s["st_size"])
            for s in elf.get_section_by_name(".symtab").iter_symbols()
            if s["st_info"]["type"] == "STT_FUNC" and s["st_size"]
        }
        return text["sh_addr"], text.data(), symbols


UNIT_FILES = (".i", ".s", ".gnu.s", ".o", ".compiler.log")
# What a whole build leaves to say what it found. They describe the objects of that build.
BUILD_RESULTS = ("report.json", "summary.txt")


def publish(scratch: Path, build: Path, unit_name: str, replace=os.replace) -> None:
    """Move the files of one unit from `scratch` into the build directory.

    The report and the summary of the last whole build go first: once one
    file of the unit is replaced they no longer describe the directory, and
    an interruption between two replacements must not leave them behind. If
    they cannot be removed, nothing is replaced. `replace` moves one file;
    the controls pass one that fails.
    """
    for name in BUILD_RESULTS:
        (build / name).unlink(missing_ok=True)
    for ext in UNIT_FILES:
        if (scratch / f"unit-{unit_name}{ext}").exists():
            replace(scratch / f"unit-{unit_name}{ext}", build / f"unit-{unit_name}{ext}")


def rebuild(cfg, unit, build: Path, tag: str, cache_dir: Path | None) -> int | None:
    """Run the pipeline of one unit and put its object and listings into the build directory.

    The pipeline of a whole build runs here for this unit alone, in a scratch
    directory inside the build directory, so that a failed step leaves the
    previous object and the report as they were. Once the pipeline has
    succeeded the report is removed before the first file is replaced: see
    `publish`. Prints what fails in the object checks and which way the
    object came. Returns an exit status when the unit cannot go on, else None.
    """
    scratch = build / f".rebuild-{os.getpid()}"
    report = {
        "tag": tag,
        "cache": {"units": {}},
        "inputs": {"preprocessed": {}},
    }
    try:
        scratch.mkdir()
        pipeline = None
        try:
            pipeline, failures = prepare_pipeline(cfg, tag, scratch, cache_dir, report)
            if pipeline is not None:
                failures, built = unit_object(pipeline, unit, scratch, report)
                failures = [named(unit.image, reason) for reason in failures]
                if not built:
                    pipeline = None
        except StepError as exc:
            # A failed step of a module unit names its image, as in a whole build. A failure of the shared setup does not.
            failed_unit = pipeline is not None
            pipeline, failures = None, [named(unit.image, str(exc)) if failed_unit else str(exc)]
        except EnvironmentFailure as exc:
            print(f"ENVIRONMENT ERROR: {exc}")
            return 3
        if pipeline is None:
            for reason in failures:
                print(f"FAIL: {reason}")
            print("RESULT: FAIL (the unit's previous object and the report are unchanged)")
            return 1
        try:
            publish(scratch, build, unit.name)
        except OSError as exc:
            print("FAIL: " + named(unit.image, f"cannot put the files of unit {unit.name!r} into {build}: {exc}"))
            print("RESULT: FAIL (run matchbuild.py again: the build directory may hold files of two builds)")
            return 1
    finally:
        shutil.rmtree(scratch, ignore_errors=True)
    print(f"object of unit {unit.name!r}: cache {report['cache']['units'][unit.name]['cache']}")
    for reason in failures:
        print(f"FAIL: {reason}")
    return None


def main() -> int:
    root = Path(__file__).resolve().parents[2]
    parser = argparse.ArgumentParser(description="Instruction diff of one unit against the baseline")
    parser.add_argument("unit")
    parser.add_argument("function", nargs="?", help="limit the diff to one function of the unit")
    parser.add_argument("--config", type=Path, default=root / "ps1/src/build.toml")
    parser.add_argument("--tag", default="default")
    parser.add_argument("--all", action="store_true", help="print matching instructions too")
    parser.add_argument("--rebuild", action="store_true", help="compile this unit again first, with the object cache")
    parser.add_argument("--reference", action="store_true", help="with --rebuild: use [toolchain.cc1_reference]")
    parser.add_argument("--cache", type=Path, help="with --rebuild: object cache directory (default as matchbuild.py)")
    parser.add_argument("--no-cache", action="store_true", help="with --rebuild: do not read or write the object cache")
    parser.add_argument("--context", type=int, default=3, help="matching instructions shown around a difference")
    args = parser.parse_args()
    if not args.rebuild and (args.reference or args.cache or args.no_cache):
        parser.error("--reference, --cache and --no-cache need --rebuild")
    if args.cache and args.no_cache:
        parser.error("--cache and --no-cache exclude each other")

    config_path = Path(os.path.abspath(args.config))
    try:
        cfg = load_config(config_path, use_reference=args.reference)
    except ConfigError as exc:
        print("\n".join(f"CONFIG ERROR: {e}" for e in exc.errors))
        return 2
    unit = next((u for u in cfg.units if u.name == args.unit), None)
    if unit is None:
        print(f"no unit named {args.unit!r}")
        return 2
    build = config_path.parent.parent / "build" / args.tag
    if args.rebuild:
        if not build.is_dir():
            print(f"no build directory {build}: run matchbuild.py with the same --tag first")
            return 2
        status = rebuild(cfg, unit, build, args.tag, cache_directory(args.cache, args.no_cache, config_path))
        if status is not None:
            return status
    text_address, text, built_functions = built_code(link_alone(cfg, unit, build))

    if args.function:
        declared = next((f for f in unit.functions if f.name == args.function), None)
        if declared is None or args.function not in built_functions:
            print(named(unit.image, f"function {args.function!r} is not declared in the unit or not present in the object"))
            return 2
        want_address, want_size = declared.address, declared.size
        got_address, got_size = built_functions[args.function]
    else:
        want_address, want_size = unit.start, unit.end - unit.start
        got_address, got_size = text_address, len(text)
    # The baseline of a module unit is the payload of its own image.
    image = next((i for i in cfg.images if i.name == unit.image), None)
    base, payload = (cfg.load, cfg.payload) if image is None else (image.address, image.payload)
    offset = want_address - base
    want = payload[offset : offset + want_size]
    got = text[got_address - text_address : got_address - text_address + got_size]

    disassembler = Cs(CS_ARCH_MIPS, CS_MODE_MIPS32 + CS_MODE_LITTLE_ENDIAN)

    def render(word: bytes, address: int) -> str:
        decoded = next(disassembler.disasm(word, address), None)
        text_form = f"{decoded.mnemonic} {decoded.op_str}".strip() if decoded else ".word"
        return f"{address:08x} {int.from_bytes(word, 'little'):08x} {text_form}"

    want_words, got_words = words(want), words(got)
    matcher = difflib.SequenceMatcher(None, want_words, got_words, autojunk=False)
    different = 0
    width = 46
    print(f"{'baseline':<{width}}   built")
    for tag, i1, i2, j1, j2 in matcher.get_opcodes():
        if tag == "equal":
            span = range(i2 - i1)
            shown = span if args.all else [k for k in span if k < args.context or k >= len(span) - args.context]
            previous = -1
            for k in shown:
                if k != previous + 1:
                    print("  ...")
                left = render(want_words[i1 + k], want_address + 4 * (i1 + k))
                right = render(got_words[j1 + k], got_address + 4 * (j1 + k))
                print(f"  {left:<{width}} {right}")
                previous = k
            continue
        different += max(i2 - i1, j2 - j1)
        for k in range(max(i2 - i1, j2 - j1)):
            left = render(want_words[i1 + k], want_address + 4 * (i1 + k)) if i1 + k < i2 else ""
            right = render(got_words[j1 + k], got_address + 4 * (j1 + k)) if j1 + k < j2 else ""
            mark = "|" if left and right else ("<" if left else ">")
            print(f"{mark} {left:<{width}} {right}")
    identical = want == got and want_address == got_address
    print(
        f"baseline {len(want)} bytes at {want_address:#x}, built {len(got)} bytes at {got_address:#x}, "
        f"{different} differing instruction slots: {'IDENTICAL' if identical else 'DIFFERENT'}"
    )
    return 0 if identical else 1


if __name__ == "__main__":
    sys.exit(main())
