#!/usr/bin/env python3
"""Rename struct fields where they are used, located by a compiler.

The field table is the one place where a field gets its name (the "Shared
types" section of ps1/docs/matching-build.md). This tool compares an old
table with a new one, struct by struct and offset by offset: a field that
kept its offset and changed its name is a rename. It writes the header of
the new table into a scratch folder and has clang check each C file against
it. Every use of an old name is then the error

    FILE:LINE:COL: error: no member named 'OLD' in 'struct NAME'

and the tool replaces the token at exactly that place. It repeats until no
such error is left, because an error inside an expression hides the ones
behind it. Nothing is found by text search, so a field of the same name in
another struct, a comment and a string are left alone.

What it does not do: it does not decide a name, it does not touch comments
or prose that mention a field, and it does not show that the build still
gives the same bytes. The matching build shows that. A file that clang
could not read to the end (a missing include) is searched as text for the
old names, and a hit is reported as left, not rewritten.

Commands:
  pairs OLD NEW                     list the renames between two tables
  apply --old OLD --new NEW PATH... rewrite the C files and headers under PATH

Reads: the two tables and the files under PATH. It needs no game data. A
file that includes SDK headers needs their folder as -I.
Cost: about 40 ms per file and pass, eight files at a time by default.
Exit status: 0 when done, 1 when a use is left that it could not rewrite or
the tables do not parse, 2 for a usage error.
"""

from __future__ import annotations

import argparse
import concurrent.futures
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

import structgen

TOOL_NAME = "ps1/tools/renamefields.py"
MEMBER_RE = re.compile(
    r"^(?P<file>.+?):(?P<line>\d+):(?P<col>\d+): error: no member named '(?P<old>\w+)' in '(?:struct )?(?P<struct>\w+)'"
)
FATAL_RE = re.compile(r"^(?P<file>.+?):\d+:\d+: fatal error: ")
# Old C that this compiler refuses by default; none of it ends the reading of a file.
LENIENT = [
    "-std=gnu89",
    "-w",
    "-Wno-incompatible-pointer-types",
    "-Wno-incompatible-function-pointer-types",
    "-Wno-int-conversion",
    "-Wno-return-mismatch",
]
MAX_PASSES = 12
IDENT_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_"


def renames(old: structgen.Model, new: structgen.Model) -> tuple[dict[tuple[str, str], str], list[str]]:
    """The renames as {(struct, old name): new name}, and what was not taken for one."""
    pairs: dict[tuple[str, str], str] = {}
    notes: list[str] = []
    new_structs = {s.name: s for s in new.structs}
    for struct in old.structs:
        other = new_structs.get(struct.name)
        if other is None:
            notes.append(f"struct {struct.name}: not in the new table")
            continue
        by_offset = {f.offset: f for f in other.fields}
        new_names = {f.name for f in other.fields}
        for field in struct.fields:
            twin = by_offset.get(field.offset)
            if twin is None:
                if field.name not in new_names:
                    notes.append(f"struct {struct.name}: {field.name} at {field.offset:#x} has no field at its offset in the new table")
                continue
            if twin.name == field.name:
                continue
            if field.name in new_names:
                notes.append(
                    f"struct {struct.name}: {field.name} moved from {field.offset:#x}; "
                    f"the field now at that offset is {twin.name}: not taken for a rename"
                )
                continue
            pairs[(struct.name, field.name)] = twin.name
    return pairs, notes


def c_files(paths: list[Path]) -> list[Path]:
    found: list[Path] = []
    for path in paths:
        if path.is_dir():
            found += sorted(p for p in path.rglob("*.c") if p.is_file())
        elif path.suffix == ".c":
            found.append(path)
    return found


def check(clang: str, source: Path, flags: list[str]) -> str:
    proc = subprocess.run(
        [clang, "-fsyntax-only", "-ferror-limit=0", "-fno-caret-diagnostics", "-fno-color-diagnostics", *LENIENT, "-x", "c", *flags, source.name],
        cwd=source.parent,
        capture_output=True,
        text=True,
    )
    return proc.stderr


def scan(clang: str, sources: list[Path], flags: list[str], jobs: int):
    """Run clang over the sources. Returns the member errors and the files it did not read to the end."""
    uses: set[tuple[Path, int, int, str, str]] = set()
    other: dict[Path, str] = {}
    with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as pool:
        for source, text in zip(sources, pool.map(lambda s: check(clang, s, flags), sources)):
            for line in text.splitlines():
                match = MEMBER_RE.match(line)
                if match:
                    where = (source.parent / match["file"]).resolve()
                    uses.add((where, int(match["line"]), int(match["col"]), match["struct"], match["old"]))
                elif FATAL_RE.match(line) and source not in other:
                    other[source] = line
    return uses, other


def rewrite(uses, pairs, roots: list[Path], dry_run: bool):
    """Replace the tokens. Returns the count per rename and the uses left alone, with the reason."""
    done: dict[tuple[str, str], int] = {}
    left: list[str] = []
    by_file: dict[Path, list[tuple[int, int, str, str]]] = {}
    for where, line, col, struct, old in uses:
        if (struct, old) not in pairs:
            left.append(f"{where}:{line}:{col}: {struct}.{old} is not a rename of this run (the file did not compile before)")
            continue
        if not any(root == where or root in where.parents for root in roots):
            left.append(f"{where}:{line}:{col}: {struct}.{old} is used outside the given paths")
            continue
        by_file.setdefault(where, []).append((line, col, struct, old))
    for where, items in sorted(by_file.items()):
        lines = where.read_text().split("\n")
        changed = False
        for line, col, struct, old in sorted(items, reverse=True):
            text = lines[line - 1] if 0 < line <= len(lines) else ""
            start = col - 1
            end = start + len(old)
            ok = text[start:end] == old and (end == len(text) or text[end] not in IDENT_CHARS)
            if not ok:
                left.append(f"{where}:{line}:{col}: {struct}.{old}: the token there is not the name (a macro's use?)")
                continue
            lines[line - 1] = text[:start] + pairs[(struct, old)] + text[end:]
            done[(struct, old)] = done.get((struct, old), 0) + 1
            changed = True
        if changed and not dry_run:
            where.write_text("\n".join(lines))
    return done, left


def apply(args) -> int:
    old = structgen.load(args.old)
    new = structgen.load(args.new)
    pairs, notes = renames(old, new)
    for note in notes:
        print(f"note: {note}")
    if not pairs:
        print("no field was renamed between the two tables")
        return 0
    roots = [p.resolve() for p in args.paths]
    sources = c_files(roots)
    totals: dict[tuple[str, str], int] = {}
    left: list[str] = []
    other: dict[Path, str] = {}
    with tempfile.TemporaryDirectory(prefix="renamefields-") as tmp:
        (Path(tmp) / args.header_name).write_text(structgen.generate_header(new, args.header_name))
        flags = ["-target", "mipsel-none-elf", "-nostdinc", "-I", tmp]
        for include in args.include:
            flags += ["-I", str(include.resolve())]
        work = sources
        if args.dry_run:
            # A dry run cannot repeat: its first pass is all it can see.
            uses, other = scan(args.clang, work, flags, args.jobs)
            totals, left = rewrite(uses, pairs, roots, True)
        else:
            for number in range(1, MAX_PASSES + 1):
                uses, other = scan(args.clang, work, flags, args.jobs)
                done, left = rewrite(uses, pairs, roots, False)
                for key, count in done.items():
                    totals[key] = totals.get(key, 0) + count
                print(f"pass {number}: {len(work)} files checked, {sum(done.values())} uses rewritten")
                if not done:
                    break
            else:
                left.append(f"still rewriting after {MAX_PASSES} passes")
    for (struct, old_name), new_name in sorted(pairs.items()):
        print(f"{struct}.{old_name} -> {new_name}: {totals.get((struct, old_name), 0)} uses")
    # A file that clang gave up on may hide a use. Text search can say that it
    # holds none of the old names; it cannot say whose field a name it finds is.
    old_names = re.compile(r"\b(?:" + "|".join(sorted({re.escape(old_name) for _, old_name in pairs})) + r")\b")
    for source, line in sorted(other.items()):
        found = sorted(set(old_names.findall(source.read_text(errors="replace"))))
        if found:
            left.append(f"{source}: not read to the end ({line.split('error: ', 1)[1]}) and it has the text {', '.join(found)}")
    for line in sorted(left):
        print(f"LEFT: {line}")
    print(
        f"{len(pairs)} renames, {sum(totals.values())} uses {'found' if args.dry_run else 'rewritten'} "
        f"in {len(sources)} files, {len(left)} left, {len(other)} files not read to the end"
    )
    return 1 if left else 0


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(prog=TOOL_NAME, description="Rename struct fields where they are used")
    sub = parser.add_subparsers(dest="command", required=True)
    prs = sub.add_parser("pairs", help="list the renames between two field tables")
    prs.add_argument("old", type=Path)
    prs.add_argument("new", type=Path)
    app = sub.add_parser("apply", help="rewrite the uses in C files and the headers they include")
    app.add_argument("--old", type=Path, required=True, help="the field table before the renames")
    app.add_argument("--new", type=Path, required=True, help="the field table with the new names")
    app.add_argument("--header-name", default="types.gen.h", help="the name under which the C files include the generated header")
    app.add_argument("-I", "--include", type=Path, action="append", default=[], help="a further include folder (never rewritten unless under PATH)")
    app.add_argument("--clang", default=shutil.which("clang") or "clang")
    app.add_argument("--jobs", type=int, default=min(8, os.cpu_count() or 1))
    app.add_argument("--dry-run", action="store_true", help="count the uses of the first pass, write nothing")
    app.add_argument("paths", type=Path, nargs="+", help="folders and C files to rewrite")
    args = parser.parse_args(argv)
    try:
        if args.command == "pairs":
            pairs, notes = renames(structgen.load(args.old), structgen.load(args.new))
            for note in notes:
                print(f"note: {note}")
            for (struct, old_name), new_name in sorted(pairs.items()):
                print(f"{struct}.{old_name} -> {new_name}")
            print(f"{len(pairs)} renames")
            return 0
        return apply(args)
    except structgen.FieldsError as exc:
        return structgen.report(exc)


if __name__ == "__main__":
    sys.exit(main())
