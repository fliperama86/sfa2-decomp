#!/usr/bin/env python3
"""Merge several unit directories back into one configuration directory.

Each UNITDIR started as a copy of the base directory and adds one unit: a
`[[unit]]` table in build.toml, a source file, lines in symbols.ld and lines
in types.fields. The merge is mechanical: every disagreement is reported and
nothing is resolved silently. The tool holds no game data.

Exit status: 0 for a clean merge, 1 for conflicts (nothing is written),
2 for usage errors.
"""

from __future__ import annotations

import argparse
import dataclasses
import os
import re
import shutil
import sys
import tomllib
from pathlib import Path

import matchbuild
import structgen

TYPES_FILE = "types.fields"
SYMBOLS_FILE = "symbols.ld"
BUILD_FILE = "build.toml"
SPECIAL_FILES = (TYPES_FILE, SYMBOLS_FILE, BUILD_FILE)
COMMENT_RE = re.compile(r"/\*.*?\*/", re.DOTALL)
STATEMENT_RE = re.compile(r"[^;]*;")


class UsageError(Exception):
    pass


@dataclasses.dataclass
class Statement:
    name: str
    value: int
    text: str  # `name = value;` as written, stripped
    start: int  # span in the source text, for removal
    end: int


@dataclasses.dataclass
class Unit:
    directory: Path
    label: str  # directory name, used in messages and comments


# ---------------------------------------------------------------------------
# symbols.ld


def parse_statements(text: str, label: str, conflicts: list[str]) -> list[Statement]:
    """Statements of a symbols.ld text, comments ignored, in source order."""
    masked = COMMENT_RE.sub(lambda m: re.sub(r"[^\n]", " ", m.group()), text)
    statements: list[Statement] = []
    seen: dict[str, Statement] = {}
    end = 0
    for match in STATEMENT_RE.finditer(masked):
        end = match.end()
        body = match.group()[:-1]
        stripped = body.strip()
        if not stripped:
            continue
        start = match.start() + len(body) - len(body.lstrip())
        parsed = matchbuild.SYMBOL_STMT_RE.match(stripped)
        if not parsed:
            conflicts.append(f"{label}: {SYMBOLS_FILE}: unsupported statement {stripped!r}")
            continue
        value_text = parsed.group(2)
        value = int(value_text, 16) if value_text[:2].lower() == "0x" else int(value_text, 10)
        statement = Statement(parsed.group(1), value, stripped + ";", start, match.end())
        if statement.name in seen:
            conflicts.append(f"{label}: {SYMBOLS_FILE}: duplicate assignment of {statement.name!r}")
            continue
        seen[statement.name] = statement
        statements.append(statement)
    if masked[end:].strip():
        conflicts.append(f"{label}: {SYMBOLS_FILE}: unsupported trailing text {masked[end:].strip()!r}")
    return statements


def remove_statements(text: str, doomed: list[Statement]) -> str:
    """Delete statements from the text, taking their line along when it holds nothing else."""
    for statement in sorted(doomed, key=lambda s: s.start, reverse=True):
        start, end = statement.start, statement.end
        line_start = text.rfind("\n", 0, start) + 1
        if text[line_start:start].strip() == "":
            start = line_start
            tail = end
            while text[tail : tail + 1] in (" ", "\t"):
                tail += 1
            if tail >= len(text) or text[tail] == "\n":
                end = min(tail + 1, len(text))
        text = text[:start] + text[end:]
    return text


def merge_symbols(
    base_text: str,
    units: list[Unit],
    unit_texts: dict[str, str],
    function_names: set[str],
    conflicts: list[str],
) -> tuple[str, dict[str, int], int, list[str]]:
    """Return the merged text, symbols added per unit, symbols removed, and warnings."""
    base = parse_statements(base_text, "base", conflicts)
    base_values = {s.name: s.value for s in base}
    introduced: dict[str, tuple[str, int]] = {s.name: ("base", s.value) for s in base}
    added_by: dict[str, list[Statement]] = {}
    for unit in units:
        statements = parse_statements(unit_texts[unit.label], unit.label, conflicts)
        added: list[Statement] = []
        for statement in statements:
            known = introduced.get(statement.name)
            if known is None:
                introduced[statement.name] = (unit.label, statement.value)
                added.append(statement)
            elif known[1] != statement.value:
                conflicts.append(
                    f"conflict: {SYMBOLS_FILE}: symbol {statement.name!r} is {known[1]:#x} in {known[0]} "
                    f"but {statement.value:#x} in {unit.label}"
                )
        added_by[unit.label] = added

    doomed = [s for s in base if s.name in function_names]
    removed = len(doomed)
    text = remove_statements(base_text, doomed)
    if text and not text.endswith("\n"):
        text += "\n"
    counts: dict[str, int] = {}
    for unit in units:
        kept = [s for s in added_by[unit.label] if s.name not in function_names]
        removed += len(added_by[unit.label]) - len(kept)
        counts[unit.label] = len(kept)
        if kept:
            text += f"\n/* Added by {unit.label}. */\n" + "".join(s.text + "\n" for s in kept)

    by_value: dict[int, list[str]] = {}
    for name, (_, value) in introduced.items():
        if name not in function_names:
            by_value.setdefault(value, []).append(name)
    warnings = []
    for value, names in sorted(by_value.items()):
        for index, first in enumerate(names):
            for second in names[index + 1 :]:
                if introduced[first][0] != introduced[second][0]:
                    warnings.append(
                        f"{value:#x} is named {first!r} ({introduced[first][0]}) "
                        f"and {second!r} ({introduced[second][0]})"
                    )
    return text, counts, removed, warnings


# ---------------------------------------------------------------------------
# build.toml


def quote(value: str) -> str:
    return '"' + value.replace("\\", "\\\\").replace('"', '\\"') + '"'


def format_value(key: str, value) -> str:
    if isinstance(value, bool):
        return "true" if value else "false"
    if isinstance(value, str):
        return quote(value)
    if isinstance(value, int):
        return f"{value:#x}" if key == "address" else str(value)
    if isinstance(value, list):
        return "[" + ", ".join(format_value(key, item) for item in value) + "]"
    raise ValueError(f"unsupported value for {key!r}: {value!r}")


def format_unit(unit: dict) -> str:
    """One `[[unit]]` table in the form the configuration files use."""
    lines = ["[[unit]]"]
    for key, value in unit.items():
        if key == "functions":
            lines.append("functions = [")
            for function in value:
                fields = ", ".join(f"{k} = {format_value(k, v)}" for k, v in function.items())
                lines.append(f"  {{ {fields} }},")
            lines.append("]")
        elif isinstance(value, dict):
            fields = ", ".join(f"{k} = {format_value(k, v)}" for k, v in value.items())
            lines.append(f"{key} = {{ {fields} }}")
        else:
            lines.append(f"{key} = {format_value(key, value)}")
    text = "\n".join(lines) + "\n"
    if tomllib.loads(text) != {"unit": [unit]}:
        raise ValueError(f"unit {unit.get('name')!r} does not survive being written back out")
    return text


def load_toml(text: str, label: str, conflicts: list[str]) -> dict | None:
    try:
        return tomllib.loads(text)
    except tomllib.TOMLDecodeError as exc:
        conflicts.append(f"{label}: {BUILD_FILE}: {exc}")
        return None


def merge_build(
    base_text: str, units: list[Unit], unit_texts: dict[str, str], conflicts: list[str]
) -> tuple[str, dict[str, list[dict]]]:
    """Return the merged text and the new unit tables per unit directory."""
    base = load_toml(base_text, "base", conflicts)
    if base is None:
        return base_text, {}
    base_units = {u["name"]: u for u in base.get("unit", [])}
    new_by_unit: dict[str, list[dict]] = {}
    owner: dict[str, str] = {}
    for unit in units:
        parsed = load_toml(unit_texts[unit.label], unit.label, conflicts)
        if parsed is None:
            continue
        mine = {u["name"]: u for u in parsed.get("unit", [])}
        new_by_unit[unit.label] = []
        for name, table in mine.items():
            if name in base_units:
                if table != base_units[name]:
                    conflicts.append(f"conflict: {unit.label} changes base unit {name!r} in {BUILD_FILE}")
            elif name in owner:
                conflicts.append(f"conflict: unit {name!r} is added by both {owner[name]} and {unit.label}")
            else:
                owner[name] = unit.label
                new_by_unit[unit.label].append(table)
        for name in base_units:
            if name not in mine:
                conflicts.append(f"conflict: {unit.label} removes base unit {name!r} from {BUILD_FILE}")
        rest = {k: v for k, v in parsed.items() if k != "unit"}
        base_rest = {k: v for k, v in base.items() if k != "unit"}
        for key in sorted(set(rest) | set(base_rest)):
            if rest.get(key) != base_rest.get(key):
                conflicts.append(f"conflict: {unit.label} changes [{key}] in {BUILD_FILE}")
    text = base_text if base_text.endswith("\n") or not base_text else base_text + "\n"
    for unit in units:
        for table in new_by_unit.get(unit.label, []):
            text += "\n" + format_unit(table)
    return text, new_by_unit


# ---------------------------------------------------------------------------
# types.fields


def merge_fields(
    base_text: str, units: list[Unit], unit_texts: dict[str, str], conflicts: list[str]
) -> tuple[str, int]:
    """Return the merged text and the number of fields added."""
    models = []
    try:
        models.append(structgen.parse(base_text, "base/" + TYPES_FILE))
    except structgen.FieldsError as exc:
        conflicts += [f"base: {e}" for e in exc.errors]
        return base_text, 0
    for unit in units:
        try:
            models.append(structgen.parse(unit_texts[unit.label], f"{unit.label}/{TYPES_FILE}"))
        except structgen.FieldsError as exc:
            conflicts += [f"{unit.label}: {e}" for e in exc.errors]
    if len(models) != len(units) + 1:
        return base_text, 0
    for unit, model in zip(units, models[1:]):
        if (unit.directory / TYPES_FILE).is_file():
            conflicts += [f"conflict: {unit.label} {e}" for e in _removed_declarations(models[0], model)]
    try:
        merged = structgen.merge_models(models)
    except structgen.FieldsError as exc:
        conflicts += exc.errors
        return base_text, 0
    count = lambda model: sum(len(s.fields) for s in model.structs)
    added = count(merged) - count(models[0])
    # Compare the whole model (types, empty structs, padding-only structs), not a
    # count. Nothing new: keep the base text, with its comments, as it is.
    changed = structgen.format_fields(merged) != structgen.format_fields(models[0])
    return (structgen.format_fields(merged) if changed else base_text), added


def _removed_declarations(base: structgen.Model, unit: structgen.Model) -> list[str]:
    """Base declarations that a unit copy no longer has; the union would hide that."""
    removed = []
    for name in base.types:
        if name not in unit.types:
            removed.append(f"removes base type {name!r} from {TYPES_FILE}")
    unit_structs = {s.name: s for s in unit.structs}
    for struct in base.structs:
        other = unit_structs.get(struct.name)
        if other is None:
            removed.append(f"removes base struct {struct.name!r} from {TYPES_FILE}")
            continue
        # A field that is still there but different is the merge's conflict to report.
        offsets = {f.offset for f in other.fields}
        names = {f.name for f in other.fields}
        for f in struct.fields:
            if f.offset not in offsets and f.name not in names:
                removed.append(f"removes base field {struct.name}.{f.name} from {TYPES_FILE}")
    return removed


# ---------------------------------------------------------------------------
# Other files


def list_files(root: Path) -> dict[str, Path]:
    files = {}
    for path in sorted(root.rglob("*")):
        relative = path.relative_to(root)
        if any(part == "__pycache__" or part.startswith(".") for part in relative.parts):
            continue
        if path.is_file():
            files[relative.as_posix()] = path
    return files


def merge_files(
    base_dir: Path, units: list[Unit], takes: dict[str, Unit], conflicts: list[str]
) -> dict[str, Path]:
    """Return relative path -> source file for everything copied over the base."""
    base_files = list_files(base_dir)
    chosen: dict[str, tuple[Unit, Path]] = {}
    for unit in units:
        files = list_files(unit.directory)
        for relative, base_path in base_files.items():
            if relative in SPECIAL_FILES:
                continue
            path = files.get(relative)
            if path is None:
                conflicts.append(f"conflict: {unit.label} is missing base file {relative}")
            elif path.read_bytes() != base_path.read_bytes():
                if takes.get(relative) is unit:
                    chosen[relative] = (unit, path)
                else:
                    conflicts.append(f"conflict: modified shared file {relative} in {unit.label}")
        for special in SPECIAL_FILES:
            if special not in files:
                conflicts.append(f"conflict: {unit.label} is missing base file {special}")
        for relative, path in files.items():
            if relative in base_files:
                continue
            earlier = chosen.get(relative)
            if earlier is None:
                chosen[relative] = (unit, path)
            elif earlier[1].read_bytes() != path.read_bytes():
                conflicts.append(f"conflict: new file {relative} differs between {earlier[0].label} and {unit.label}")
    for relative, unit in takes.items():
        if relative not in chosen or chosen[relative][0] is not unit:
            conflicts.append(f"conflict: --take {unit.label}:{relative} names a file that unit does not modify")
    owners = {relative: "base" for relative in base_files}
    owners.update({relative: unit.label for relative, (unit, _) in chosen.items()})
    conflicts += prefix_collisions(owners)
    return {relative: path for relative, (_, path) in chosen.items()}


def prefix_collisions(owners: dict[str, str]) -> list[str]:
    """A path that is a file in one place and a directory in another cannot both exist."""
    found = []
    for relative in sorted(owners):
        parts = relative.split("/")
        for end in range(1, len(parts)):
            parent = "/".join(parts[:end])
            if parent in owners:
                found.append(
                    f"conflict: {parent} is a file in {owners[parent]} but a directory in "
                    f"{owners[relative]} (holding {relative})"
                )
    return found


# ---------------------------------------------------------------------------
# Command line


def parse_takes(specs: list[str], units: list[Unit], conflicts: list[str]) -> dict[str, Unit]:
    takes: dict[str, Unit] = {}
    for spec in specs:
        directory, sep, relative = spec.rpartition(":")
        if not sep or not directory or not relative:
            raise UsageError(f"--take needs UNITDIR:RELPATH, got {spec!r}")
        unit = next((u for u in units if u.directory.resolve() == Path(directory).resolve()), None)
        if unit is None:
            raise UsageError(f"--take names {directory!r}, which is not one of the unit directories")
        relative = Path(relative).as_posix()
        if relative in takes:
            conflicts.append(
                f"conflict: --take names {relative} for both {takes[relative].label} and {unit.label}"
            )
        else:
            takes[relative] = unit
    return takes


def read_inputs(base_dir: Path, units: list[Unit], name: str, conflicts: list[str]) -> tuple[str, dict[str, str]]:
    try:
        base_text = (base_dir / name).read_text()
    except OSError:
        raise UsageError(f"base directory has no {name}")
    texts = {}
    for unit in units:
        try:
            texts[unit.label] = (unit.directory / name).read_text()
        except OSError:
            texts[unit.label] = ""  # reported as a missing base file by merge_files
    return base_text, texts


def publish(base: Path, out: Path, fields: str, symbols: str, build: str, copied: dict[str, Path]) -> None:
    """Write the whole result next to `out`, then publish it with one rename.

    Any failure removes the staging directory and leaves `out` absent.
    """
    out.parent.mkdir(parents=True, exist_ok=True)
    staging = out.parent / f".{out.name}.staging-{os.getpid()}"
    if os.path.lexists(staging):
        raise UsageError(f"staging directory exists: {staging}")
    try:
        shutil.copytree(base, staging, ignore=shutil.ignore_patterns("__pycache__", ".*"))
        (staging / TYPES_FILE).write_text(fields)
        (staging / SYMBOLS_FILE).write_text(symbols)
        (staging / BUILD_FILE).write_text(build)
        for relative, source in copied.items():
            target = staging / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(source, target)
        if os.path.lexists(out):
            raise UsageError(f"output exists: {out}")
        os.rename(staging, out)
    except BaseException:
        shutil.rmtree(staging, ignore_errors=True)
        raise


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Merge unit directories into one configuration directory")
    parser.add_argument("--base", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--take", action="append", default=[], metavar="UNITDIR:RELPATH")
    parser.add_argument("unitdirs", type=Path, nargs="+", metavar="UNITDIR")
    args = parser.parse_args(argv)

    try:
        for directory in [args.base, *args.unitdirs]:
            if not directory.is_dir():
                raise UsageError(f"not a directory: {directory}")
        if os.path.lexists(args.out):
            raise UsageError(f"output exists: {args.out}")
        units = [Unit(d, d.resolve().name) for d in args.unitdirs]
        labels = [u.label for u in units]
        if len(set(labels)) != len(labels):
            raise UsageError("unit directories must have distinct names")
        conflicts: list[str] = []
        takes = parse_takes(args.take, units, conflicts)

        fields_base, fields_units = read_inputs(args.base, units, TYPES_FILE, conflicts)
        symbols_base, symbols_units = read_inputs(args.base, units, SYMBOLS_FILE, conflicts)
        build_base, build_units = read_inputs(args.base, units, BUILD_FILE, conflicts)
    except UsageError as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2

    fields_text, fields_added = merge_fields(fields_base, units, fields_units, conflicts)
    build_text, new_units = merge_build(build_base, units, build_units, conflicts)
    function_names = {f["name"] for tables in new_units.values() for t in tables for f in t.get("functions", [])}
    base_parsed = load_toml(build_base, "base", [])
    for table in (base_parsed or {}).get("unit", []):
        function_names |= {f["name"] for f in table.get("functions", [])}
    symbols_text, symbol_counts, symbols_removed, warnings = merge_symbols(
        symbols_base, units, symbols_units, function_names, conflicts
    )
    copied = merge_files(args.base, units, takes, conflicts)

    if warnings:
        print("WARNINGS")
        for warning in warnings:
            print(f"  {warning}")
    if conflicts:
        for conflict in conflicts:
            print(f"error: {conflict}", file=sys.stderr)
        print(f"RESULT: FAIL ({len(conflicts)} conflict(s)); nothing written", file=sys.stderr)
        return 1

    try:
        publish(args.base, args.out, fields_text, symbols_text, build_text, copied)
    except UsageError as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2

    added_units = sum(len(tables) for tables in new_units.values())
    print(f"units added: {added_units}")
    for unit in units:
        print(f"  {unit.label}: {len(new_units.get(unit.label, []))} unit(s), {symbol_counts[unit.label]} symbol(s)")
    print(f"symbols removed (now defined by a unit): {symbols_removed}")
    print(f"fields added: {fields_added}")
    print(f"files copied: {len(copied)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
