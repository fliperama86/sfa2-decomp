#!/usr/bin/env python3
"""Rename functions everywhere their names are written.

The build configuration is the one place where a function has its name and
its address together (`functions = [{ name, address, size }]` of a unit in
`ps1/src/build.toml`). This tool compares an old configuration with a new
one: a function that kept its image and address and changed its name is a
rename. It then replaces the old name by the new one, as a whole word, in
every text file under the paths it is given.

Unlike a struct field, a function's name is one word that means one thing
in the whole tree (`func_80130efc`, or `func_801b0128_slot04_00` for a
module's function), so a text replacement is exact: definitions, calls,
prototypes, comments, the configuration's own tables, the inventory, and the
contracts and fixtures of nonmatching functions, which name their callees.
A longer name that begins with an old name is another word and is left.

A function that has a file named after it keeps its name, and the tool
refuses to rename it: a nonmatching function, or an exact one that the port
overrides (`port/overrides/func_ADDRESS.c`). The tools that build and test
such a file read the function's address out of the file's name.

What it does not do: it does not decide a name, and it does not show that
the build still gives the same bytes; the matching build shows that. It
does not rewrite Markdown unless a file is given by its own path: a page
that records a run keeps the names of the day of the run.

Commands:
  pairs OLD NEW                     list the renames between two configurations
  apply --old OLD --new NEW PATH... rewrite the files under PATH

`--fresh` is for the first application of a rename: it refuses when a new
name is already written somewhere under the paths, apart from the new
configuration. Without it the tool can be run again on a tree that has the
new names already, which is how a branch from before a rename is repaired.

Reads: the two configurations and the files under PATH. No game data.
Cost: a second or two for the tree.
Exit status: 0 when done, 1 for a refused rename or a configuration that
does not parse, 2 for a usage error.
"""

from __future__ import annotations

import argparse
import re
import sys
import tomllib
from pathlib import Path

TOOL_NAME = "ps1/tools/renamefuncs.py"
SUFFIXES = (".c", ".h", ".s", ".ld", ".toml", ".py", ".json", ".tsv")
SKIP_DIRS = ("build", "__pycache__", "include")
IDENT_RE = re.compile(r"[A-Za-z_][A-Za-z0-9_]*\Z")


class Refused(Exception):
    pass


def functions(path: Path) -> dict[tuple[str, int], str]:
    """{(image or "", address): name} of every function the configuration declares."""
    try:
        config = tomllib.loads(path.read_text())
    except (OSError, tomllib.TOMLDecodeError) as exc:
        raise Refused(f"{path}: {exc}") from exc
    found: dict[tuple[str, int], str] = {}
    for unit in config.get("unit", []):
        for function in unit.get("functions", []):
            key = (unit.get("image", ""), function["address"])
            if key in found and found[key] != function["name"]:
                raise Refused(f"{path}: {found[key]} and {function['name']} are declared at one address of one image")
            found[key] = function["name"]
    return found


def renames(old: dict[tuple[str, int], str], new: dict[tuple[str, int], str]) -> dict[str, str]:
    """The renames as {old name: new name}. Raises Refused for a rename that cannot be exact."""
    pairs: dict[str, str] = {}
    problems: list[str] = []
    for key, name in old.items():
        other = new.get(key)
        if other is None or other == name:
            continue
        if not IDENT_RE.match(other):
            problems.append(f"{name} -> {other}: the new name is not a C identifier")
        elif pairs.get(name, other) != other:
            # One source is linked into two images that are declared alike: its name stands at two keys.
            problems.append(f"{name} would get two names, {pairs[name]} and {other}")
        else:
            pairs[name] = other
    still = set(new.values())
    before = set(old.values())
    targets: dict[str, str] = {}
    for name, other in pairs.items():
        if name in still:
            problems.append(f"{name} -> {other}: the old name is still a function of the new configuration")
        if other in before and other not in pairs:
            problems.append(f"{name} -> {other}: another function has that name already")
        if targets.setdefault(other, name) != name:
            problems.append(f"{targets[other]} and {name} would both be named {other}")
    if problems:
        raise Refused("\n".join(sorted(set(problems))))
    return pairs


def text_files(paths: list[Path]) -> list[Path]:
    found: list[Path] = []
    for path in paths:
        if path.is_file():
            found.append(path)
            continue
        for item in sorted(path.rglob("*")):
            relative = item.relative_to(path).parts
            if item.is_file() and item.suffix in SUFFIXES and not any(part in SKIP_DIRS for part in relative[:-1]):
                found.append(item)
    return found


def word_pattern(names) -> re.Pattern:
    return re.compile(r"(?<![A-Za-z0-9_])(?:" + "|".join(sorted(map(re.escape, names), key=len, reverse=True)) + r")(?![A-Za-z0-9_])")


def apply(args) -> int:
    pairs = renames(functions(args.old), functions(args.new))
    if not pairs:
        print("no function was renamed between the two configurations")
        return 0
    files = [f for f in text_files(args.paths) if f.resolve() != args.old.resolve()]
    # A file named after a function (nonmatching C, an override of the port, their contracts and fixtures) is
    # found by that name, and the tools that test it read the address out of it.
    held = sorted(f for f in files if f.name.split(".")[0] in pairs)
    if held:
        raise Refused("\n".join(f"{f.name.split('.')[0]} keeps its name: the file {f} is named after it" for f in held))
    if args.fresh:
        taken = word_pattern(pairs.values())
        hits = []
        for file in files:
            if file.resolve() == args.new.resolve():
                continue
            for number, line in enumerate(file.read_text(errors="replace").splitlines(), 1):
                for match in taken.finditer(line):
                    hits.append(f"{file}:{number}: {match.group(0)} is written here already")
        if hits:
            raise Refused("\n".join(hits[:20] + ([f"... and {len(hits) - 20} more"] if len(hits) > 20 else [])))
    pattern = word_pattern(pairs)
    counts: dict[str, int] = {}
    by_suffix: dict[str, int] = {}
    changed = 0
    for file in files:
        try:
            text = file.read_text()
        except UnicodeDecodeError:
            continue

        def replace(match: re.Match) -> str:
            counts[match.group(0)] = counts.get(match.group(0), 0) + 1
            by_suffix[file.suffix] = by_suffix.get(file.suffix, 0) + 1
            return pairs[match.group(0)]

        new_text = pattern.sub(replace, text)
        if new_text != text:
            changed += 1
            if not args.dry_run:
                file.write_text(new_text)
    for name, other in sorted(pairs.items()):
        print(f"{name} -> {other}: {counts.get(name, 0)} places")
    print("by kind of file: " + (", ".join(f"{suffix} {count}" for suffix, count in sorted(by_suffix.items())) or "none"))
    print(
        f"{len(pairs)} renames, {sum(counts.values())} places {'found' if args.dry_run else 'rewritten'} "
        f"in {changed} of {len(files)} files"
    )
    return 0


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(prog=TOOL_NAME, description="Rename functions everywhere their names are written")
    sub = parser.add_subparsers(dest="command", required=True)
    prs = sub.add_parser("pairs", help="list the renames between two build configurations")
    prs.add_argument("old", type=Path)
    prs.add_argument("new", type=Path)
    app = sub.add_parser("apply", help="rewrite the names in the text files under the paths")
    app.add_argument("--old", type=Path, required=True, help="the build configuration before the renames")
    app.add_argument("--new", type=Path, required=True, help="the build configuration with the new names")
    app.add_argument("--fresh", action="store_true", help="refuse when a new name is already written under the paths")
    app.add_argument("--dry-run", action="store_true", help="count the places, write nothing")
    app.add_argument("paths", type=Path, nargs="+", help="folders, and files of any kind given by their own path")
    args = parser.parse_args(argv)
    try:
        if args.command == "pairs":
            pairs = renames(functions(args.old), functions(args.new))
            for name, other in sorted(pairs.items()):
                print(f"{name} -> {other}")
            print(f"{len(pairs)} renames")
            return 0
        return apply(args)
    except Refused as exc:
        print(f"{TOOL_NAME}: refused:\n{exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
