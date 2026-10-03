#!/usr/bin/env python3
"""Struct-layout generator: a field table in, a C header out.

Contract: the "Shared types" section of ps1/docs/matching-build.md. The tool
holds no game data. Commands: generate, check, merge.

Exit status: 0 on success, 1 for any reported error.
"""

from __future__ import annotations

import argparse
import dataclasses
import re
import subprocess
import sys
import tempfile
from pathlib import Path

TOOL_NAME = "ps1/tools/structgen.py"
# name -> (size, alignment, C spelling)
SCALARS = {
    "u8": (1, 1, "unsigned char"),
    "s8": (1, 1, "signed char"),
    "u16": (2, 2, "unsigned short"),
    "s16": (2, 2, "short"),
    "u32": (4, 4, "unsigned int"),
    "s32": (4, 4, "int"),
}
# Written as one word in the field file, already a C type.
NATIVE_ALIASES = {"int": (4, 4)}
POINTER_SIZE = 4
IDENT_RE = re.compile(r"[A-Za-z_][A-Za-z0-9_]*\Z")
TYPE_RE = re.compile(r"([A-Za-z_][A-Za-z0-9_]*)(\**)\Z")
FIELD_RE = re.compile(
    r"(0[xX][0-9a-fA-F]+|[0-9]+)\s+(\S+)\s+([A-Za-z_][A-Za-z0-9_]*)(?:\[(0[xX][0-9a-fA-F]+|[0-9]+)\])?\Z"
)
NUMBER_RE = re.compile(r"(0[xX][0-9a-fA-F]+|[0-9]+)\Z")


class FieldsError(Exception):
    """Every problem found in the input, one message per entry."""

    def __init__(self, errors: list[str]):
        super().__init__("; ".join(errors))
        self.errors = errors


@dataclasses.dataclass
class TypeDecl:
    name: str
    size: int
    align: int
    loc: str


@dataclasses.dataclass
class Field:
    offset: int
    type: str
    name: str
    count: int | None
    loc: str
    size: int = 0  # total size, set by validation
    align: int = 1  # element alignment, set by validation

    def same_as(self, other: "Field") -> bool:
        return (self.offset, self.type, self.name, self.count) == (
            other.offset,
            other.type,
            other.name,
            other.count,
        )


@dataclasses.dataclass
class StructDecl:
    name: str
    size: int
    loc: str
    fields: list[Field] = dataclasses.field(default_factory=list)
    align: int = 1  # set by validation


@dataclasses.dataclass
class Model:
    types: dict[str, TypeDecl] = dataclasses.field(default_factory=dict)
    structs: list[StructDecl] = dataclasses.field(default_factory=list)


def parse_number(text: str) -> int | None:
    if not NUMBER_RE.match(text):
        return None
    return int(text, 16) if text[:2].lower() == "0x" else int(text, 10)


def parse_attrs(tokens: list[str]) -> dict[str, int] | None:
    attrs: dict[str, int] = {}
    for token in tokens:
        key, sep, value = token.partition("=")
        number = parse_number(value) if sep else None
        if number is None or key in attrs:
            return None
        attrs[key] = number
    return attrs


# ---------------------------------------------------------------------------
# Parsing and validation


def parse(text: str, source: str = "fields") -> Model:
    """Parse and validate a field file. Raises FieldsError listing every problem."""
    errors: list[str] = []
    model = Model()
    names: dict[str, str] = {}  # struct and type names -> location
    current: StructDecl | None = None
    for number, raw in enumerate(text.splitlines(), 1):
        loc = f"{source}:{number}"
        line = raw.split("#", 1)[0].strip()
        if not line:
            continue
        tokens = line.split()
        keyword = tokens[0]
        if keyword in ("struct", "type"):
            want = {"struct": {"size"}, "type": {"size", "align"}}[keyword]
            attrs = parse_attrs(tokens[2:]) if len(tokens) >= 2 else None
            ok = (
                attrs is not None
                and set(attrs) == want
                and IDENT_RE.match(tokens[1]) is not None
            )
            # A throwaway target keeps the fields of a bad header from cascading.
            current = StructDecl("", 0, loc)
            if not ok:
                errors.append(f"{loc}: malformed line: {line!r}")
                continue
            name = tokens[1]
            if name in names or name in SCALARS or name in NATIVE_ALIASES or name == "void":
                previous = f" (first at {names[name]})" if name in names else ""
                errors.append(f"{loc}: duplicate {keyword} name {name!r}{previous}")
                continue
            names[name] = loc
            if keyword == "type":
                model.types[name] = TypeDecl(name, attrs["size"], attrs["align"], loc)
                current = None
            else:
                current = StructDecl(name, attrs["size"], loc)
                model.structs.append(current)
            continue
        match = FIELD_RE.match(line)
        if not match or not TYPE_RE.match(match.group(2)):
            errors.append(f"{loc}: malformed line: {line!r}")
            continue
        if current is None:
            errors.append(f"{loc}: field outside a struct: {line!r}")
            continue
        count = parse_number(match.group(4)) if match.group(4) else None
        current.fields.append(Field(parse_number(match.group(1)), match.group(2), match.group(3), count, loc))
    errors += validate(model)
    if errors:
        raise FieldsError(errors)
    return model


def validate(model: Model) -> list[str]:
    """Check the model and fill in field sizes and struct alignments."""
    errors: list[str] = []
    for decl in model.types.values():
        if decl.size <= 0 or decl.align <= 0 or decl.align & (decl.align - 1):
            errors.append(f"{decl.loc}: type {decl.name!r} needs a positive size and a power-of-two align")
        elif decl.size % decl.align:
            errors.append(f"{decl.loc}: type {decl.name!r} size {decl.size} is not a multiple of its align {decl.align}")
    struct_names = {s.name for s in model.structs}
    done: dict[str, tuple[int, int]] = {}  # complete structs: size, align

    def known(base: str) -> bool:
        return base in SCALARS or base in NATIVE_ALIASES or base in model.types or base in struct_names or base == "void"

    def resolve(f: Field, struct: StructDecl) -> tuple[int, int] | None:
        base, stars = TYPE_RE.match(f.type).groups()
        if stars:
            if known(base):
                return POINTER_SIZE, POINTER_SIZE
        elif base in SCALARS:
            return SCALARS[base][:2]
        elif base in NATIVE_ALIASES:
            return NATIVE_ALIASES[base]
        elif base in model.types:
            decl = model.types[base]
            return decl.size, decl.align
        elif base in done:
            return done[base]
        elif base in struct_names:
            errors.append(f"{f.loc}: struct {base!r} is used by value before it is declared")
            return None
        errors.append(f"{f.loc}: unknown type {f.type!r}")
        return None

    for struct in model.structs:
        if struct.size <= 0:
            errors.append(f"{struct.loc}: struct {struct.name!r} needs a size greater than zero")
        placed: list[Field] = []
        seen: dict[str, Field] = {}
        align = 1
        for f in struct.fields:
            if f.count is not None and f.count <= 0:
                errors.append(f"{f.loc}: array length of {f.name!r} must be greater than zero")
                continue
            resolved = resolve(f, struct)
            if resolved is None:
                continue
            element, f.align = resolved
            f.size = element * (f.count or 1)
            align = max(align, f.align)
            if f.name in seen:
                errors.append(f"{f.loc}: duplicate field name {f.name!r} in struct {struct.name!r} (first at {seen[f.name].loc})")
            else:
                seen[f.name] = f
            if f.offset % f.align:
                errors.append(
                    f"{f.loc}: field {f.name!r} offset {f.offset:#x} is not a multiple of its alignment {f.align}"
                )
            if f.offset + f.size > struct.size:
                errors.append(
                    f"{f.loc}: field {f.name!r} ends past the declared size "
                    f"({f.offset + f.size:#x} > {struct.size:#x}) of struct {struct.name!r}"
                )
            placed.append(f)
        struct.align = align
        done[struct.name] = (struct.size, align)
        if struct.size % align:
            errors.append(
                f"{struct.loc}: struct {struct.name!r} size {struct.size:#x} is not a multiple of its alignment {align}"
            )
        reach: Field | None = None  # the placed field that extends furthest so far
        for f in sorted(placed, key=lambda x: (x.offset, x.loc)):
            if reach is not None and f.offset < reach.offset + reach.size:
                errors.append(
                    f"{f.loc}: field {f.name!r} at {f.offset:#x} overlaps field {reach.name!r} "
                    f"({reach.offset:#x}-{reach.offset + reach.size:#x}, {reach.loc}) in struct {struct.name!r}"
                )
            if reach is None or f.offset + f.size > reach.offset + reach.size:
                reach = f
        generated = {gap_name(offset) for offset, _ in _gaps(struct)} if not errors else set()
        for f in struct.fields:
            if f.name in generated:
                errors.append(f"{f.loc}: field name {f.name!r} collides with generated padding in struct {struct.name!r}")
    return errors


def gap_name(offset: int) -> str:
    return f"unknown_{offset:02x}"


def _gaps(struct: StructDecl) -> list[tuple[int, int]]:
    """(offset, length) of every padding run in a validated struct."""
    gaps = []
    cursor = 0
    for f in sorted(struct.fields, key=lambda x: x.offset):
        if f.offset > cursor:
            gaps.append((cursor, f.offset - cursor))
        cursor = f.offset + f.size
    if cursor < struct.size:
        gaps.append((cursor, struct.size - cursor))
    return gaps


# ---------------------------------------------------------------------------
# Output


def declaration(model: Model, f: Field) -> str:
    base, stars = TYPE_RE.match(f.type).groups()
    suffix = f"[{f.count}]" if f.count is not None else ""
    if stars:
        is_struct = any(s.name == base for s in model.structs)
        return f"{'struct ' if is_struct else ''}{base} {stars}{f.name}{suffix};"
    return f"{base} {f.name}{suffix};"


def guard_name(header_name: str) -> str:
    return re.sub(r"[^A-Za-z0-9]", "_", header_name).upper()


def generate_header(model: Model, header_name: str = "types.gen.h") -> str:
    guard = guard_name(header_name)
    lines = [
        f"/* Generated by {TOOL_NAME} from a field table. Do not edit. */",
        f"#ifndef {guard}",
        f"#define {guard}",
        "",
    ]
    for name, (_, _, spelling) in SCALARS.items():
        lines.append(f"typedef {spelling} {name};")
    for struct in model.structs:
        lines += ["", f"typedef struct {struct.name} {{"]
        entries: list[tuple[int, str]] = []
        for f in struct.fields:
            entries.append((f.offset, declaration(model, f)))
        for offset, length in _gaps(struct):
            suffix = f"[{length}]" if length != 1 else ""
            entries.append((offset, f"u8 {gap_name(offset)}{suffix};"))
        for _, text in sorted(entries, key=lambda e: e[0]):
            lines.append(f"    {text}")
        lines.append(f"}} {struct.name};")
    lines += ["", f"#endif /* {guard} */", ""]
    return "\n".join(lines)


def generate_check(model: Model, header_name: str) -> str:
    lines = ["/* Layout assertions for a generated header. */"]
    for decl in model.types.values():
        lines.append(f"typedef struct {{ unsigned char b[{decl.size}]; }} __attribute__((aligned({decl.align}))) {decl.name};")
    lines.append(f'#include "{header_name}"')
    for struct in model.structs:
        n = struct.name
        lines.append(f'_Static_assert(sizeof({n}) == {struct.size}, "sizeof {n}");')
        lines.append(f'_Static_assert(_Alignof({n}) == {struct.align}, "alignof {n}");')
        for f in struct.fields:
            lines.append(
                f'_Static_assert(__builtin_offsetof({n}, {f.name}) == {f.offset}, "offset {n}.{f.name}");'
            )
            lines.append(
                f'_Static_assert(sizeof((({n} *)0)->{f.name}) == {f.size}, "size {n}.{f.name}");'
            )
    return "\n".join(lines) + "\n"


def format_fields(model: Model) -> str:
    """Canonical field file: types, then structs in order, fields by offset."""
    lines = []
    for decl in model.types.values():
        lines.append(f"type {decl.name} size={decl.size:#x} align={decl.align}")
    for struct in model.structs:
        if lines:
            lines.append("")
        lines.append(f"struct {struct.name} size={struct.size:#x}")
        for f in sorted(struct.fields, key=lambda x: x.offset):
            suffix = f"[{f.count}]" if f.count is not None else ""
            lines.append(f"{f.offset:#05x} {f.type} {f.name}{suffix}")
    return "\n".join(lines) + "\n"


# ---------------------------------------------------------------------------
# Compiler check


def check_layout(model: Model, cpp: str, header_name: str, directory: Path) -> None:
    """Compile assertions about the generated header with a real compiler.

    The header must already exist as `directory/header_name`.
    """
    check = directory / "types.check.c"
    check.write_text(generate_check(model, header_name))
    argv = [cpp, "-target", "mipsel-none-elf", "-nostdinc", "-fsyntax-only", "-I", str(directory), str(check)]
    try:
        result = subprocess.run(argv, capture_output=True)
    except OSError as exc:
        raise FieldsError([f"layout check: cannot execute {cpp}: {exc}"])
    if result.returncode != 0:
        output = (result.stderr + result.stdout).decode(errors="replace").strip()
        raise FieldsError([f"layout check failed (exit {result.returncode}): {output}"])


# ---------------------------------------------------------------------------
# Merge


def merge_models(models: list[Model]) -> Model:
    """Union of validated models. Any disagreement is reported; nothing is picked."""
    conflicts: list[str] = []
    merged = Model()
    by_name: dict[str, StructDecl] = {}
    for model in models:
        for decl in model.types.values():
            clash = by_name.get(decl.name)
            if clash is not None:
                conflicts.append(
                    f"conflict: name {decl.name!r} is a struct ({clash.loc}) and a type ({decl.loc})"
                )
                continue
            old = merged.types.get(decl.name)
            if old is None:
                merged.types[decl.name] = TypeDecl(decl.name, decl.size, decl.align, decl.loc)
            elif (old.size, old.align) != (decl.size, decl.align):
                conflicts.append(
                    f"conflict: type {decl.name!r}: size={old.size:#x} align={old.align} ({old.loc}) "
                    f"vs size={decl.size:#x} align={decl.align} ({decl.loc})"
                )
        for struct in model.structs:
            clash = merged.types.get(struct.name)
            if clash is not None:
                conflicts.append(
                    f"conflict: name {struct.name!r} is a type ({clash.loc}) and a struct ({struct.loc})"
                )
                continue
            target = by_name.get(struct.name)
            if target is None:
                target = StructDecl(struct.name, struct.size, struct.loc)
                by_name[struct.name] = target
                merged.structs.append(target)
            elif target.size != struct.size:
                conflicts.append(
                    f"conflict: struct {struct.name!r}: size {target.size:#x} ({target.loc}) vs {struct.size:#x} ({struct.loc})"
                )
            for f in struct.fields:
                at_offset = next((g for g in target.fields if g.offset == f.offset), None)
                same_name = next((g for g in target.fields if g.name == f.name), None)
                if at_offset is not None and at_offset.same_as(f):
                    continue
                if at_offset is not None:
                    conflicts.append(
                        f"conflict: struct {struct.name!r} offset {f.offset:#x}: "
                        f"{at_offset.type} {at_offset.name} ({at_offset.loc}) vs {f.type} {f.name} ({f.loc})"
                    )
                    continue
                if same_name is not None:
                    conflicts.append(
                        f"conflict: struct {struct.name!r} field {f.name!r}: offset {same_name.offset:#x} "
                        f"({same_name.loc}) vs {f.offset:#x} ({f.loc})"
                    )
                    continue
                target.fields.append(dataclasses.replace(f))
    if conflicts:
        raise FieldsError(conflicts)
    merged.structs = _dependency_order(merged)
    errors = validate(merged)
    if errors:
        raise FieldsError(errors)
    # Fail closed: the text that would be written must pass the tool's own parser.
    try:
        parse(format_fields(merged), "merged result")
    except FieldsError as exc:
        raise FieldsError([f"merged result is not valid: {e}" for e in exc.errors])
    return merged


def _dependency_order(model: Model) -> list[StructDecl]:
    """First-seen order, except that a struct used by value follows what it embeds."""
    by_name = {s.name: s for s in model.structs}
    ordered: list[StructDecl] = []
    state: dict[str, int] = {}

    def visit(struct: StructDecl) -> None:
        if state.get(struct.name):
            return
        state[struct.name] = 1
        for f in sorted(struct.fields, key=lambda x: x.offset):
            dep = by_name.get(f.type)
            if dep is not None:
                visit(dep)
        ordered.append(struct)

    for struct in model.structs:
        visit(struct)
    return ordered


# ---------------------------------------------------------------------------
# Command line


def load(path: Path) -> Model:
    try:
        text = path.read_text()
    except OSError as exc:
        raise FieldsError([f"cannot read {path}: {exc}"])
    return parse(text, str(path))


def report(exc: FieldsError) -> int:
    for error in exc.errors:
        print(f"error: {error}", file=sys.stderr)
    print(f"RESULT: FAIL ({len(exc.errors)} error(s))", file=sys.stderr)
    return 1


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Struct-layout generator")
    sub = parser.add_subparsers(dest="command", required=True)
    gen = sub.add_parser("generate", help="write the C header")
    gen.add_argument("fields", type=Path)
    gen.add_argument("-o", "--output", type=Path, required=True)
    chk = sub.add_parser("check", help="verify the layout with a real compiler")
    chk.add_argument("fields", type=Path)
    chk.add_argument("--cpp", required=True, help="clang executable")
    mrg = sub.add_parser("merge", help="union of several field files")
    mrg.add_argument("inputs", type=Path, nargs="+")
    mrg.add_argument("-o", "--output", type=Path, required=True)
    args = parser.parse_args(argv)

    try:
        if args.command == "generate":
            model = load(args.fields)
            args.output.write_text(generate_header(model, args.output.name))
        elif args.command == "check":
            model = load(args.fields)
            with tempfile.TemporaryDirectory(prefix="structgen-") as tmp:
                directory = Path(tmp)
                (directory / "types.gen.h").write_text(generate_header(model, "types.gen.h"))
                check_layout(model, args.cpp, "types.gen.h", directory)
            print(f"layout check passed: {len(model.structs)} struct(s)")
        else:
            models = [load(path) for path in args.inputs]
            merged = merge_models(models)
            args.output.write_text(format_fields(merged))
    except FieldsError as exc:
        return report(exc)
    return 0


if __name__ == "__main__":
    sys.exit(main())
