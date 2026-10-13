#!/usr/bin/env python3
"""Rename data symbols everywhere their names are written.

A data symbol has its name and its address together in one place, its line in
`ps1/src/symbols.ld` (`data_8018f5a0 = 0x8018f5a0;`). Its first name is
`data_` or `table_` and the address. A name is one word that means one thing
in the whole tree, so, like a function's, it is renamed by a replacement of
the whole word: the declaration in `externs.h`, the uses in every unit, the
contracts and fixtures of nonmatching functions and the port's overrides. A
longer name that begins with the old one (`data_801a27e4_rows`) is another
word and is left.

Unlike renamefuncs.py, this tool is given the renames, not two
configurations: a table of lines `OLD<TAB>NEW`, further columns ignored, a
first line that begins with `old` and lines that begin with `#` skipped.

It refuses, before writing anything:

- an old name that is not a placeholder (`data_`, `table_` or `jtbl_` and
  eight hex digits, then an image suffix if it has one) or that the symbol
  file does not define, unless the new name is defined there and the old one
  is not, which is a tree that has the rename already (see below);
- a new name that is not lower case snake case, is a placeholder, a C
  keyword, or is the new name of another line of the table;
- a new name that is written anywhere under the paths already, when the
  old name is still defined (the first application). That is the clash
  check; the build of every image is the other one.

When the old name is no longer defined and the new one is, the tool rewrites
only what still has the old name: this repairs a branch from before the
rename, as renamefuncs.py's second run does.

What it does not do: decide a name, change an address or a type, or show
that the build still gives the same bytes; the matching build shows that.
Markdown is rewritten only where it is given by its own path.

Commands:
  apply --map TABLE [--symbols FILE] PATH...   rewrite the files under PATH
  check --map TABLE [--symbols FILE] PATH...   the same checks, writes nothing

Reads: the table, the symbol file and the files under PATH. No game data.
Cost: a second or two for the tree.
Exit status: 0 when done, 1 for a refused rename or a table that cannot be read.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

SUFFIXES = (".c", ".h", ".s", ".ld", ".toml", ".py", ".json", ".tsv")
SKIP_DIRS = ("build", "__pycache__", "include")
PLACEHOLDER = re.compile(r"(data|table|jtbl)_[0-9a-f]{8}(_[a-z0-9_]+)?\Z")
ANY_PLACEHOLDER = re.compile(r"(func|data|table|jtbl)_[0-9a-f]{8}(_[a-z0-9_]+)?\Z|field_[0-9a-f]+\Z")
NAME = re.compile(r"[a-z][a-z0-9_]*\Z")
KEYWORDS = frozenset(
    "auto break case char const continue default do double else enum extern float for goto if int long register "
    "return short signed sizeof static struct switch typedef union unsigned void volatile while".split()
)


class Refused(Exception):
    pass


def read_table(path: Path) -> list[tuple[str, str]]:
    try:
        lines = path.read_text().splitlines()
    except OSError as error:
        raise Refused(f"cannot read {path}: {error}") from error
    rows: list[tuple[str, str]] = []
    for number, line in enumerate(lines, 1):
        if not line.strip() or line.startswith("#") or (number == 1 and line.split("\t")[0] == "old"):
            continue
        columns = line.split("\t")
        if len(columns) < 2 or not columns[0].strip() or not columns[1].strip():
            raise Refused(f"{path}:{number}: a line is OLD<TAB>NEW")
        rows.append((columns[0].strip(), columns[1].strip()))
    return rows


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


def word(name: str) -> re.Pattern:
    return re.compile(r"(?<![A-Za-z0-9_])" + re.escape(name) + r"(?![A-Za-z0-9_])")


def defined(symbols: str, name: str) -> bool:
    return re.search(r"(?m)^\s*" + re.escape(name) + r"\s*=", symbols) is not None


def plan(rows: list[tuple[str, str]], symbols_text: str, files: list[Path], texts: dict[Path, str]) -> list[tuple[str, str]]:
    """The renames to apply, or Refused. Nothing is written here."""
    problems: list[str] = []
    olds = [old for old, _ in rows]
    news = [new for _, new in rows]
    for old in sorted({o for o in olds if olds.count(o) > 1}):
        problems.append(f"{old} is renamed twice")
    for new in sorted({n for n in news if news.count(n) > 1}):
        problems.append(f"{new} is the new name of more than one symbol")
    work: list[tuple[str, str]] = []
    for old, new in rows:
        if not NAME.match(new) or new in KEYWORDS or ANY_PLACEHOLDER.match(new):
            problems.append(f"{new}: a new name is lower case snake case, not a keyword, not a placeholder")
            continue
        if new == old:
            problems.append(f"{old}: the new name is the old name")
            continue
        if not PLACEHOLDER.match(old):
            problems.append(f"{old}: not a placeholder name of a data symbol")
            continue
        if defined(symbols_text, old):
            pattern = word(new)
            for file in files:
                for number, line in enumerate(texts[file].splitlines(), 1):
                    if pattern.search(line):
                        problems.append(f"{file}:{number}: {new} is written here already")
                        break
            work.append((old, new))
        elif defined(symbols_text, new):
            work.append((old, new))  # the tree has the rename; only leftovers of the old name are rewritten
        else:
            problems.append(f"{old}: the symbol file defines neither {old} nor {new}")
    if problems:
        raise Refused("\n".join(problems))
    return work


def run(args: argparse.Namespace, write: bool) -> int:
    try:
        rows = read_table(Path(args.map))
        symbols_path = Path(args.symbols)
        try:
            symbols_text = symbols_path.read_text()
        except OSError as error:
            raise Refused(f"cannot read {symbols_path}: {error}") from error
        files = text_files([Path(p) for p in args.paths])
        texts = {file: file.read_text(errors="surrogateescape") for file in files}
        work = plan(rows, symbols_text, files, texts)
    except Refused as error:
        print(f"refused:\n{error}", file=sys.stderr)
        return 1
    places = {file: 0 for file in files}
    counts: dict[str, int] = {}
    for old, new in work:
        pattern = word(old)
        counts[old] = 0
        for file in files:
            texts[file], n = pattern.subn(new, texts[file])
            places[file] += n
            counts[old] += n
        print(f"{old} -> {new}: {counts[old]} places")
    changed = [file for file in files if places[file]]
    if write:
        for file in changed:
            file.write_text(texts[file], errors="surrogateescape")
    total = sum(counts.values())
    verb = "rewritten" if write else "to rewrite"
    print(f"{len(work)} renames, {total} places {verb} in {len(changed)} of {len(files)} files")
    return 0


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = parser.add_subparsers(dest="command", required=True)
    for name in ("apply", "check"):
        command = sub.add_parser(name)
        command.add_argument("--map", required=True, help="table of lines OLD<TAB>NEW")
        command.add_argument("--symbols", default="ps1/src/symbols.ld")
        command.add_argument("paths", nargs="+")
    args = parser.parse_args(argv)
    return run(args, write=args.command == "apply")


if __name__ == "__main__":
    sys.exit(main())
