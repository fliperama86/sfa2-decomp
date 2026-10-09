#!/usr/bin/env python3
"""Compare the Sony library functions that the game calls with what PsyZ has.

Reads the published library inventory of the resident executable and the
source tree of the pinned PsyZ submodule. No game file is read and nothing
is built. Python 3.11 or later, standard library only.

The game side
-------------

A row of `ps1/inventory/library.tsv` is one library function: address, size,
the name under which a unit of the build declares it or `-`, family, the
number of game functions of the resident executable that call it, and the
rule that gave the family. `ps1/tools/families.py` writes the table and
says what a call is; the count is static and covers the resident
executable only. Calls from overlay modules, calls through a register and
a function handed over as a callback are not in it.

The rows compared are those with at least one caller; with `--all`, every
row.

The library name of a row is the first of:

1. its name in the inventory, unless that is `-` or begins with `func_`;
2. a name that the symbol file gives its address (`NAME = 0xADDRESS;`) and
   that does not begin with `func_`. Of several, the first in order of name
   whose status would not be `absent`, else the first in order of name.

A row without a library name has the status `unnamed`: it cannot be
compared.

The PsyZ side
-------------

Three kinds of `.c` file, by `psyz/CMakeLists.txt`:

- built: the paths between `set(PSYZ_SOURCES` and the `)` that closes it,
  taken relative to `psyz/`. They are compiled for every target.
- target: the paths in a `list(APPEND PSYZ_SOURCES ...)`, compiled for some
  targets.
- other: every other `.c` file under `psyz/src` and `decomp/src`.

A path of the build file that does not exist is an error.

A file is scanned after its comments and its string and character literals
are taken out, with a backslash at the end of a line joining it to the
next.

- Conditionals. `__psyz` counts as defined with the value 1, as the build
  file defines it, and `__psx__`, the mark of a build for the PS1, as not
  defined. Nothing else is known. For X one of the two, `#ifdef X`,
  `#ifndef X`, `#if defined(X)`, `#if !defined(X)`, `#if X` and `#if !X`,
  and also `#if 0` and `#if 1`, each with its `#else`, are decided: the
  text of the branch not taken is not scanned. So is a guard: `#ifndef G`
  whose next directive is `#define G`, as headers have it around their
  whole text, is taken, which is what a compiler sees the first time it
  reads the file. Every other condition, and every `#elif`, is unknown:
  all its branches are scanned and what they hold is conditional.
- Braces. Each later branch of an unknown conditional starts again at the
  brace depth where the conditional began, and the text after its `#endif`
  goes on at the depth where its first branch ended. A definition that a
  later branch begins and does not end inside that branch is therefore not
  found. A file that ends inside a brace, or closes a brace it did not
  open, is an error: the scan of the rest of that file could not be
  trusted.
- A definition is, outside every brace, a name followed by a parameter
  list in parentheses and `{`. Its body runs to the matching `}`. A
  definition that begins with `static` is not visible to a program that
  links the library and is left out.
- A definition is a stub when its body has the identifier
  `NOT_IMPLEMENTED`, which is PsyZ's own mark, outside every unknown
  condition. A definition whose body has the mark only under an unknown
  condition is conditional: a stub for some targets.
- `INCLUDE_ASM(PATH, NAME)` and `WEAK_INCLUDE_ASM(PATH, NAME)` outside
  every brace say that NAME is assembly for the PS1. For a host both
  expand to nothing. Conditions make no difference to them.

The headers under `psyz/include` are read with the same conditionals, for
two things only:

- A rename: `#define NAME OTHER`, where OTHER is an identifier. A source
  that includes the header and calls NAME calls OTHER, so the status of a
  library name that a header renames is the status of OTHER. One step
  only: a rename of OTHER is not followed. A rename under an unknown
  condition gives `some-targets`.
- A macro with parameters, `#define NAME(`. It does not change a status.
  It is reported, because a source that includes the header gets the
  macro and not the function.

The status of a library name is the first of these that applies:

    built          an unconditional definition in a built file that is
                   not a stub
    stub           unconditional definitions in built files, all stubs
    some-targets   a definition only in target files, or only conditional,
                   or a rename under an unknown condition
    assembly       no definition in a built or target file, an
                   INCLUDE_ASM in one
    not-built      a definition or an INCLUDE_ASM only in other files
    absent         neither anywhere in the tree

What a status does not say: whether a function without the mark does all
that Sony's did, or whether PsyZ's version of it takes the same arguments
as the version this game was linked with. Only names are compared.

Output
------

Standard output, in this order, with nothing else:

    compared: N of M library functions (those with a caller)
    built: N
    stub: N
    some-targets: N
    assembly: N
    not-built: N
    absent: N
    unnamed: N
    FAMILY: built N, stub N, some-targets N, assembly N, not-built N, absent N, unnamed N
    ...
    stub: NAME NAME ...
    some-targets: NAME ...
    assembly: NAME ...
    not-built: NAME ...
    absent: NAME ...
    unnamed: ADDRESS ADDRESS ...
    renamed by a header: NAME to OTHER (FILE:LINE)
    macro in a header: NAME (FILE:LINE)
    defined more than once: NAME (FILE:LINE, FILE:LINE)

With `--all` the first line ends `(all)`. One FAMILY line per family that
has a compared row, in order of name. The six lists are in order of name
or address, a name that two rows share is written once, and each list is
left out when it is empty. The last three kinds of line are written once
for each compared name they apply to, in order of name: a rename that was
followed, a macro with parameters of that name, and more than one
unconditional definition in built files, of the name or of what it is
renamed to. The status of a name defined more than once is taken from all
its definitions as above.

`--out FILE` gets one line per compared row, in order of address: address,
library name or `-`, where the name is from (`inventory`, `symbols` or
`-`), family, callers, status, and the places that decided the status, as
`PATH:LINE` relative to the PsyZ tree and joined by `,`, or `-`: the
definitions or INCLUDE_ASM lines of that status, in order of path and
line. For a renamed name the place of the rename comes first.

`--archive FILE` checks the scan against a library that was built from the
same tree. It runs `nm -g FILE` and takes the names with the type `T` or
`W`, without one leading underscore when every such name has one. A
compared name that is `built` or `stub` must be among them, and one that
is `assembly`, `not-built` or `absent` must not; for a renamed name it is
the other name that is looked up. `some-targets` and `unnamed` are not
checked. It prints one line per name that disagrees,
`archive: NAME is STATUS and is (not) defined`, and last
`archive: N names checked, K disagree`.

Exit status: 0; 1 when `--archive` finds a disagreement; 2 when an input is
missing or malformed, with one line on standard error that names it.

usage:
  libgap.py [--inventory TSV] [--symbols FILE] [--psyz DIR] [--all]
            [--out FILE] [--archive FILE]

The defaults are the files of this repository: `ps1/inventory/library.tsv`,
`ps1/src/symbols.ld` and `port/external/psyz`, found from the place of this
script.
"""

from __future__ import annotations

import argparse
import bisect
import os
import re
import subprocess
import sys
from dataclasses import dataclass, field
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
KEYWORDS = {"if", "for", "while", "switch", "return", "sizeof", "do", "else"}
ASM_MACROS = {"INCLUDE_ASM", "WEAK_INCLUDE_ASM"}
KNOWN = {"__psyz": True, "__psx__": False}
STATUSES = ("built", "stub", "some-targets", "assembly", "not-built", "absent", "unnamed")
LISTED = ("stub", "some-targets", "assembly", "not-built", "absent")
IDENT = re.compile(r"[A-Za-z_]\w*\Z")


class Problem(Exception):
    pass


@dataclass
class Site:
    path: str
    line: int
    stub: bool
    cond: bool
    kind: str
    other: str = ""

    def place(self) -> str:
        return f"{self.path}:{self.line}"


# Pass a: comments and literals become spaces, newlines stay.

LITERALS = re.compile(
    r"//(?:\\\n|[^\n])*"
    r"|/\*.*?(?:\*/|\Z)"
    r'|"(?:\\[\s\S]|[^"\\\n])*(?:"|(?=\n)|\Z)'
    r"|'(?:\\[\s\S]|[^'\\\n])*(?:'|(?=\n)|\Z)",
    re.S,
)


def blank_literals(text: str) -> str:
    return LITERALS.sub(lambda m: re.sub(r"[^\n]", " ", m.group(0)), text)


# Pass b: preprocessor conditionals.

DIRECTIVE = re.compile(r"\s*#\s*(\w*)\s*(.*)\Z", re.S)
DEFINED = re.compile(r"(!?)\s*defined\s*(?:\(\s*(\w+)\s*\)|\s+(\w+))\Z")
BARE = re.compile(r"(!?)\s*(\w+)\Z")


def condition_value(name: str, arg: str) -> bool | None:
    arg = arg.strip()
    if name in ("ifdef", "ifndef"):
        value = KNOWN.get(arg)
        return None if value is None else (value if name == "ifdef" else not value)
    if arg in ("0", "1"):
        return arg == "1"
    m = DEFINED.match(arg)
    if m:
        value = KNOWN.get(m.group(2) or m.group(3))
        return None if value is None else (not value if m.group(1) else value)
    m = BARE.match(arg)
    if m and m.group(2) in KNOWN:
        return not KNOWN[m.group(2)] if m.group(1) else KNOWN[m.group(2)]
    return None


def is_guard(logical, n: int, guard: str) -> bool:
    """True when the next directive after logical line n is `#define guard`."""
    if not IDENT.match(guard):
        return False
    for _, _, name, arg in logical[n + 1 :]:
        if name:
            return name == "define" and re.match(r"(\w+)", arg) is not None and re.match(r"(\w+)", arg).group(1) == guard
    return False


def apply_conditionals(text: str):
    """Return lines, the conditional flag of each line, branch events and #define lines.

    An event is (line index, kind) with kind open, next or close, for unknown
    conditionals only. A define is (line number, text after #define, flag).
    """
    lines = text.split("\n")
    logical: list[tuple[int, int, str, str]] = []
    i = 0
    while i < len(lines):
        j = i
        while j < len(lines) - 1 and lines[j].endswith("\\"):
            j += 1
        joined = " ".join(lines[k][:-1] if k < j else lines[k] for k in range(i, j + 1))
        m = DIRECTIVE.match(joined) if joined.lstrip().startswith("#") else None
        logical.append((i, j, m.group(1) if m else "", m.group(2) if m else ""))
        i = j + 1
    has_elif: dict[int, bool] = {}
    pending: list[int] = []
    for n, (_, _, name, _) in enumerate(logical):
        if name in ("if", "ifdef", "ifndef"):
            pending.append(n)
            has_elif[n] = False
        elif name.startswith("elif") and pending:
            has_elif[pending[-1]] = True
        elif name == "endif" and pending:
            pending.pop()
    out = list(lines)
    cond = [False] * len(lines)
    events: list[tuple[int, str]] = []
    defines: list[tuple[int, str, bool]] = []
    stack: list[dict] = []
    live, conditional = True, False
    for n, (a, b, name, arg) in enumerate(logical):
        if name in ("if", "ifdef", "ifndef"):
            value = condition_value(name, arg)
            if value is None and name == "ifndef" and is_guard(logical, n, arg.strip()):
                value = True
            frame = {"plive": live, "pcond": conditional, "unknown": value is None or has_elif[n], "value": value}
            stack.append(frame)
            if live and frame["unknown"]:
                events.append((a, "open"))
            live = live and (True if frame["unknown"] else bool(value))
            conditional = conditional or frame["unknown"]
        elif name.startswith("elif") and stack:
            frame = stack[-1]
            if frame["plive"]:
                events.append((a, "next"))
            live, conditional = frame["plive"], True
        elif name == "else" and stack:
            frame = stack[-1]
            if frame["unknown"]:
                if frame["plive"]:
                    events.append((a, "next"))
                live, conditional = frame["plive"], True
            else:
                live, conditional = frame["plive"] and not frame["value"], frame["pcond"]
        elif name == "endif" and stack:
            frame = stack.pop()
            if frame["plive"] and frame["unknown"]:
                events.append((a, "close"))
            live, conditional = frame["plive"], frame["pcond"]
        elif name == "define" and live:
            defines.append((a + 1, arg, conditional))
        is_directive = bool(name) or lines[a].lstrip().startswith("#")
        for k in range(a, b + 1):
            cond[k] = conditional
            if is_directive or not live:
                out[k] = ""
            elif k < b:
                out[k] = lines[k][:-1] + " "
    return out, cond, events, defines


# Pass c: definitions at brace depth 0.

TOKEN = re.compile(r"[A-Za-z_]\w*|\d\w*|[(){};]")


@dataclass
class Tok:
    text: str
    pos: int


def match_pairs(toks: list[Tok], open_: str, close: str) -> dict[int, int]:
    pairs: dict[int, int] = {}
    stack: list[int] = []
    for n, t in enumerate(toks):
        if t.text == open_:
            stack.append(n)
        elif t.text == close and stack:
            pairs[stack.pop()] = n
    return pairs


def scan_text(raw: str, label: str = "source"):
    """Return definitions (name, line, stub, cond), INCLUDE_ASM names (name, line) and defines."""
    raw = raw.replace("\r\n", "\n").replace("\r", "\n")
    lines, cond, events, defines = apply_conditionals(blank_literals(raw))
    text = "\n".join(lines)
    starts = [0]
    for line in lines[:-1]:
        starts.append(starts[-1] + len(line) + 1)

    def line_of(pos: int) -> int:
        return bisect.bisect_right(starts, pos)

    toks = [Tok(m.group(0), m.start()) for m in TOKEN.finditer(text) if not m.group(0)[0].isdigit()]
    parens = match_pairs(toks, "(", ")")
    marks = [(starts[line], kind) for line, kind in events]
    defs: list[tuple[str, int, bool, bool]] = []
    asms: list[tuple[str, int]] = []
    stack: list[tuple[str, object, int]] = []
    frames: list[list] = []
    e = 0

    def flush(limit: int) -> None:
        nonlocal e
        while e < len(marks) and marks[e][0] < limit:
            kind = marks[e][1]
            e += 1
            if kind == "open":
                frames.append([list(stack), None])
            elif not frames:
                continue
            elif kind == "next":
                if frames[-1][1] is None:
                    frames[-1][1] = list(stack)
                stack[:] = frames[-1][0]
            else:
                first = frames.pop()[1]
                if first is not None:
                    stack[:] = first

    i, stmt = 0, 0
    while i < len(toks):
        flush(toks[i].pos)
        t = toks[i].text
        top = all(k == "ns" for k, _, _ in stack)
        if t == "{":
            line = line_of(toks[i].pos)
            stack.append(("ns" if top and i > 0 and toks[i - 1].text == "extern" else "block", None, line))
            i += 1
            if top:
                stmt = i
        elif t == "}":
            if not stack:
                raise Problem(f"{label}:{line_of(toks[i].pos)}: closes a brace it did not open")
            kind, data, _ = stack.pop()
            if kind == "def":
                name, line, brace, name_cond = data
                body = text[brace : toks[i].pos]
                found = [cond[line_of(brace + m.start()) - 1] for m in re.finditer(r"\bNOT_IMPLEMENTED\b", body)]
                sure = any(not c for c in found)
                defs.append((name, line, sure, name_cond or (bool(found) and not sure)))
            i += 1
            if all(k == "ns" for k, _, _ in stack):
                stmt = i
        elif not top:
            i += 1
        elif t == ";":
            i += 1
            stmt = i
        elif t == "(" and i in parens:
            close = parens[i]
            name = toks[i - 1] if i > 0 and IDENT.match(toks[i - 1].text) else None
            k = close + 1
            while k + 1 < len(toks) and toks[k].text == "__attribute__" and toks[k + 1].text == "(" and (k + 1) in parens:
                k = parens[k + 1] + 1
            if k < len(toks) and toks[k].text == "{" and text[toks[i].pos + 1 :].lstrip().startswith("*"):
                # declarator like void (*NAME(params)) {: the name is inside the group
                for n in range(i + 1, close):
                    if IDENT.match(toks[n].text) and toks[n + 1].text == "(" and (n + 1) in parens:
                        name = toks[n]
                        break
            line = line_of(name.pos) if name else 0
            if name and name.text in ASM_MACROS:
                args = text[toks[i].pos + 1 : toks[close].pos]
                depth, last = 0, 0
                for pos, ch in enumerate(args):
                    depth += ch == "("
                    depth -= ch == ")"
                    if ch == "," and depth == 0:
                        last = pos + 1
                target = args[last:].strip()
                if IDENT.match(target):
                    asms.append((target, line))
            elif name and name.text not in KEYWORDS and k < len(toks) and toks[k].text == "{":
                static = "static" in {x.text for x in toks[stmt:i]}
                brace = toks[k].pos
                if static:
                    stack.append(("block", None, line_of(brace)))
                else:
                    stack.append(("def", (name.text, line, brace, cond[line - 1]), line_of(brace)))
                i = k + 1
                continue
            i = close + 1
        else:
            i += 1
    flush(len(text) + 1)
    if stack:
        raise Problem(f"{label}: ends inside a brace opened at line {stack[0][2]}")
    return defs, asms, defines


def scan_file(path: Path):
    try:
        raw = path.read_text(encoding="utf-8", errors="replace")
    except OSError as err:
        raise Problem(f"cannot read source {path}: {err.strerror}")
    return scan_text(raw, str(path))


# Inputs.

def read_inventory(path: Path) -> list[tuple[int, str, str, int]]:
    try:
        text = path.read_text()
    except OSError as err:
        raise Problem(f"cannot read inventory {path}: {err.strerror}")
    rows = []
    for number, line in enumerate(text.splitlines(), 1):
        if not line.strip():
            continue
        cols = line.split("\t")
        if len(cols) != 6:
            raise Problem(f"inventory {path}:{number}: {len(cols)} columns, wanted 6")
        if not re.fullmatch(r"(0x)?[0-9a-fA-F]+", cols[0]):
            raise Problem(f"inventory {path}:{number}: address {cols[0]!r} is not hexadecimal")
        if not re.fullmatch(r"[0-9]+", cols[4]):
            raise Problem(f"inventory {path}:{number}: caller count {cols[4]!r} is not a number")
        rows.append((int(cols[0], 16), cols[2], cols[3], int(cols[4])))
    return rows


def read_symbols(path: Path) -> dict[int, list[str]]:
    try:
        text = path.read_text()
    except OSError as err:
        raise Problem(f"cannot read symbol file {path}: {err.strerror}")
    text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
    by_address: dict[int, list[str]] = {}
    for m in re.finditer(r"^\s*([A-Za-z_]\w*)\s*=\s*(0[xX][0-9a-fA-F]+)\s*;", text, flags=re.M):
        by_address.setdefault(int(m.group(2), 16), []).append(m.group(1))
    return by_address


def cmake_sources(path: Path) -> tuple[list[str], list[str]]:
    try:
        text = path.read_text()
    except OSError as err:
        raise Problem(f"cannot read build file {path}: {err.strerror}")
    text = re.sub(r"#[^\n]*", "", text)
    first = re.search(r"\bset\s*\(\s*PSYZ_SOURCES\b([^)]*)\)", text, flags=re.I)
    if not first:
        raise Problem(f"build file {path} has no set(PSYZ_SOURCES")
    pick = lambda block: [w for w in block.split() if w.endswith(".c")]
    built = pick(first.group(1))
    target: list[str] = []
    for m in re.finditer(r"\blist\s*\(\s*APPEND\s+PSYZ_SOURCES\b([^)]*)\)", text, flags=re.I):
        target += pick(m.group(1))
    return built, target


@dataclass
class Result:
    status: str
    places: list[Site] = field(default_factory=list)
    other: str = ""
    rename: Site | None = None


class Tree:
    def __init__(self, psyz: Path):
        if not psyz.is_dir():
            raise Problem(f"PsyZ directory {psyz} does not exist")
        built, target = cmake_sources(psyz / "psyz" / "CMakeLists.txt")
        listed: dict[str, str] = {}
        for kind, paths in (("built", built), ("target", target)):
            for rel in paths:
                full = psyz / "psyz" / rel
                if not full.is_file():
                    raise Problem(f"build file {psyz / 'psyz' / 'CMakeLists.txt'} names {rel}, which does not exist")
                listed.setdefault(os.path.relpath(full, psyz).replace(os.sep, "/"), kind)
        files = list(listed.items())
        others = set()
        for top in ("psyz/src", "decomp/src"):
            for full in (psyz / top).rglob("*.c"):
                rel = full.relative_to(psyz).as_posix()
                if rel not in listed:
                    others.add(rel)
        files += [(rel, "other") for rel in sorted(others)]
        self.defs: dict[str, list[Site]] = {}
        self.asms: dict[str, list[Site]] = {}
        for rel, kind in files:
            defs, asms, _ = scan_file(psyz / rel)
            for name, line, stub, cond in defs:
                self.defs.setdefault(name, []).append(Site(rel, line, stub, cond, kind))
            for name, line in asms:
                self.asms.setdefault(name, []).append(Site(rel, line, False, False, kind))
        self.renames: dict[str, list[Site]] = {}
        self.macros: dict[str, list[Site]] = {}
        headers = sorted((psyz / "psyz" / "include").rglob("*.h")) if (psyz / "psyz" / "include").is_dir() else []
        for full in headers:
            rel = full.relative_to(psyz).as_posix()
            _, _, defines = scan_file(full)
            for line, arg, cond in defines:
                m = re.match(r"(\w+)(\()?\s*(.*)\Z", arg, re.S)
                if not m:
                    continue
                name, rest = m.group(1), m.group(3).strip()
                if m.group(2):
                    self.macros.setdefault(name, []).append(Site(rel, line, False, cond, "header"))
                elif IDENT.match(rest) and rest != name:
                    self.renames.setdefault(name, []).append(Site(rel, line, False, cond, "header", rest))

    def status(self, name: str) -> Result:
        defs, asms = self.defs.get(name, []), self.asms.get(name, [])
        order = lambda sites: sorted(sites, key=lambda s: (s.path, s.line))
        sure = [s for s in defs if s.kind == "built" and not s.cond]
        real = [s for s in sure if not s.stub]
        if real:
            return Result("built", order(real))
        if sure:
            return Result("stub", order(sure))
        some = [s for s in defs if s.kind in ("built", "target")]
        if some:
            return Result("some-targets", order(some))
        asm = [s for s in asms if s.kind in ("built", "target")]
        if asm:
            return Result("assembly", order(asm))
        rest = [s for s in defs + asms if s.kind == "other"]
        if rest:
            return Result("not-built", order(rest))
        return Result("absent")

    def resolve(self, name: str) -> Result:
        sites = self.renames.get(name, [])
        if not sites:
            return self.status(name)
        if len({s.other for s in sites}) > 1:
            where = ", ".join(f"{s.other} ({s.place()})" for s in sites)
            raise Problem(f"headers rename {name} to different names: {where}")
        site = sorted(sites, key=lambda s: (s.path, s.line))[0]
        if all(s.cond for s in sites):
            return Result("some-targets", [site], site.other, site)
        inner = self.status(site.other)
        return Result(inner.status, [site] + inner.places, site.other, site)

    def repeated(self, name: str) -> list[Site]:
        sure = [s for s in self.defs.get(name, []) if s.kind == "built" and not s.cond]
        return sorted(sure, key=lambda s: (s.path, s.line)) if len(sure) > 1 else []


# Report.

@dataclass
class Row:
    address: int
    name: str | None
    origin: str
    family: str
    callers: int
    result: Result


def compare(rows, symbols, tree: Tree, show_all: bool) -> list[Row]:
    chosen = []
    for address, inv_name, family, callers in rows:
        if callers == 0 and not show_all:
            continue
        if inv_name != "-" and not inv_name.startswith("func_"):
            name, origin = inv_name, "inventory"
        else:
            options = sorted(n for n in symbols.get(address, []) if not n.startswith("func_"))
            live = [n for n in options if tree.resolve(n).status != "absent"]
            pick = (live or options or [None])[0]
            name, origin = (pick, "symbols") if pick else (None, "-")
        result = tree.resolve(name) if name else Result("unnamed")
        chosen.append(Row(address, name, origin, family, callers, result))
    return chosen


def render(chosen: list[Row], total: int, show_all: bool, tree: Tree) -> str:
    counts = {s: 0 for s in STATUSES}
    families: dict[str, dict[str, int]] = {}
    lists: dict[str, set] = {s: set() for s in LISTED}
    unnamed = []
    for row in chosen:
        status = row.result.status
        counts[status] += 1
        families.setdefault(row.family, {s: 0 for s in STATUSES})[status] += 1
        if status in lists:
            lists[status].add(row.name)
        if status == "unnamed":
            unnamed.append(row.address)
    out = [f"compared: {len(chosen)} of {total} library functions ({'all' if show_all else 'those with a caller'})"]
    out += [f"{s}: {counts[s]}" for s in STATUSES]
    for family in sorted(families):
        out.append(f"{family}: " + ", ".join(f"{s} {families[family][s]}" for s in STATUSES))
    for s in LISTED:
        if lists[s]:
            out.append(f"{s}: " + " ".join(sorted(lists[s])))
    if unnamed:
        out.append("unnamed: " + " ".join(f"{a:08x}" for a in sorted(unnamed)))
    results = {r.name: r.result for r in chosen if r.name}
    names = sorted(results)
    for name in names:
        if results[name].rename:
            out.append(f"renamed by a header: {name} to {results[name].other} ({results[name].rename.place()})")
    for name in names:
        sites = sorted(tree.macros.get(name, []), key=lambda s: (s.path, s.line))
        if sites:
            out.append(f"macro in a header: {name} ({sites[0].place()})")
    for name in names:
        sites = tree.repeated(results[name].other or name)
        if sites:
            out.append(f"defined more than once: {name} (" + ", ".join(s.place() for s in sites) + ")")
    return "\n".join(out) + "\n"


def render_rows(chosen: list[Row]) -> str:
    lines = []
    for row in sorted(chosen, key=lambda r: r.address):
        place = ",".join(s.place() for s in row.result.places) or "-"
        lines.append(f"{row.address:08x}\t{row.name or '-'}\t{row.origin}\t{row.family}\t{row.callers}\t{row.result.status}\t{place}")
    return "\n".join(lines) + ("\n" if lines else "")


def check_archive(archive: Path, chosen: list[Row]) -> tuple[list[str], int]:
    if not archive.is_file():
        raise Problem(f"archive {archive} does not exist")
    try:
        proc = subprocess.run(["nm", "-g", str(archive)], capture_output=True, text=True)
    except OSError as err:
        raise Problem(f"cannot run nm on archive {archive}: {err.strerror}")
    if proc.returncode != 0:
        first = proc.stderr.strip().splitlines()[:1]
        raise Problem(f"nm failed on archive {archive}: {first[0] if first else 'exit ' + str(proc.returncode)}")
    defined = []
    for line in proc.stdout.splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[1] in ("T", "W"):
            defined.append(parts[2])
    if defined and all(n.startswith("_") for n in defined):
        defined = [n[1:] for n in defined]
    have = set(defined)
    checked = {r.name: r.result for r in chosen if r.name and r.result.status not in ("some-targets", "unnamed")}
    lines = []
    for name in sorted(checked):
        result = checked[name]
        should = result.status in ("built", "stub")
        if should != ((result.other or name) in have):
            lines.append(f"archive: {name} is {result.status} and is {'not ' if should else ''}defined")
    lines.append(f"archive: {len(checked)} names checked, {len(lines)} disagree")
    return lines, len(lines) - 1


def main() -> int:
    parser = argparse.ArgumentParser(description="Compare the library functions the game calls with PsyZ.")
    parser.add_argument("--inventory", type=Path, default=REPO / "ps1/inventory/library.tsv")
    parser.add_argument("--symbols", type=Path, default=REPO / "ps1/src/symbols.ld")
    parser.add_argument("--psyz", type=Path, default=REPO / "port/external/psyz")
    parser.add_argument("--all", action="store_true")
    parser.add_argument("--out", type=Path)
    parser.add_argument("--archive", type=Path)
    args = parser.parse_args()
    try:
        rows = read_inventory(args.inventory)
        symbols = read_symbols(args.symbols)
        tree = Tree(args.psyz)
        chosen = compare(rows, symbols, tree, args.all)
        sys.stdout.write(render(chosen, len(rows), args.all, tree))
        if args.out:
            try:
                args.out.write_text(render_rows(chosen))
            except OSError as err:
                raise Problem(f"cannot write {args.out}: {err.strerror}")
        if args.archive:
            lines, bad = check_archive(args.archive, chosen)
            print("\n".join(lines))
            return 1 if bad else 0
    except Problem as err:
        print(f"libgap.py: {err}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main())
