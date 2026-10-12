#!/usr/bin/env python3
"""Counts how many functions, fields and data symbols have a name instead of a placeholder.

    namestats.py [--config FILE] [--fields FILE] [--symbols FILE] [--json FILE] [--svg FILE]

It prints one line per kind and, when asked, writes the same counts as JSON and as a picture for the README:
one bar per kind, filled to its percentage. It reads three text files of the tree and nothing else; no game file is involved.

A placeholder is the generated spelling: `func_` and an address for a function, `field_` and an offset for a
field, `data_`, `table_` or `jtbl_` and an address for a data symbol; a function's or a symbol's placeholder
may end in the suffix of its image (`func_80012dec_slot01`). Anything else counts as a name. Names are
inferred from the code, like every name in this tree: the count says how many things were given a name, not
that the names are right.

What is counted:

- functions: the functions that the build configuration declares, without the units under `sdk/` (Sony's
  library: its names are Sony's and were found, not given). A function declared in several module images is
  counted once per image. A function that has only nonmatching C is not in the configuration and is not
  counted.
- fields: the fields of the field table.
- data symbols: the symbols of the symbol table, without the function placeholders and without a name that
  sits at the address of a function the configuration declares for the resident program (a name of a
  function, kept there for the linker).

Exit status: 0 when done, 1 when a file cannot be read.
"""

from __future__ import annotations

import argparse
import json
import re
import sys
import tomllib
from html import escape
from pathlib import Path

import structgen

FUNCTION_PLACEHOLDER = re.compile(r"func_[0-9a-f]{8}(_[a-z0-9_]+)?\Z")
FIELD_PLACEHOLDER = re.compile(r"field_[0-9a-f]+\Z")
DATA_PLACEHOLDER = re.compile(r"(data|table|jtbl)_[0-9a-f]{8}(_[a-z0-9_]+)?\Z")
SYMBOL_LINE = re.compile(r"^\s*([A-Za-z_]\w*)\s*=\s*([^;]+);", re.M)
COMMENT = re.compile(r"/\*.*?\*/", re.S)


class Problem(Exception):
    pass


def percent(part: int, whole: int) -> float:
    return round(100 * part / whole, 1) if whole else 0.0


def tally(names: list[str], placeholder: re.Pattern) -> dict:
    named = sum(1 for name in names if not placeholder.match(name))
    return {"named": named, "total": len(names), "percent": percent(named, len(names))}


def count(config_path: Path, fields_path: Path, symbols_path: Path) -> dict:
    try:
        config = tomllib.loads(config_path.read_text())
        symbols = COMMENT.sub("", symbols_path.read_text())
    except (OSError, tomllib.TOMLDecodeError) as exc:
        raise Problem(f"cannot read {exc.filename if isinstance(exc, OSError) else config_path}: {exc}") from exc
    try:
        model = structgen.load(fields_path)
    except structgen.FieldsError as exc:
        raise Problem("; ".join(exc.errors)) from exc

    resident: list[str] = []
    modules: list[str] = []
    resident_addresses: set[int] = set()
    for unit in config.get("unit", []):
        image = unit.get("image", "")
        for function in unit.get("functions", []):
            if not image:
                resident_addresses.add(function["address"])
            if not unit["source"].startswith("sdk/"):
                (modules if image else resident).append(function["name"])
    functions = tally(resident + modules, FUNCTION_PLACEHOLDER)
    functions["resident"] = tally(resident, FUNCTION_PLACEHOLDER)
    functions["modules"] = tally(modules, FUNCTION_PLACEHOLDER)

    fields = [field.name for struct in model.structs for field in struct.fields]

    data: list[str] = []
    for name, value in SYMBOL_LINE.findall(symbols):
        if FUNCTION_PLACEHOLDER.match(name):
            continue
        try:
            address = int(value.strip(), 0)
        except ValueError:
            address = None
        if address in resident_addresses and not DATA_PLACEHOLDER.match(name):
            continue
        data.append(name)
    return {"functions": functions, "fields": tally(fields, FIELD_PLACEHOLDER), "data_symbols": tally(data, DATA_PLACEHOLDER)}


def lines(counts: dict) -> list[str]:
    f, fields, data = counts["functions"], counts["fields"], counts["data_symbols"]
    return [
        f"functions: {f['named']} of {f['total']} have a name ({f['percent']}%); resident program {f['resident']['named']} of "
        f"{f['resident']['total']} ({f['resident']['percent']}%), modules {f['modules']['named']} of {f['modules']['total']} "
        f"({f['modules']['percent']}%)",
        f"fields: {fields['named']} of {fields['total']} have a name ({fields['percent']}%)",
        f"data symbols: {data['named']} of {data['total']} have a name ({data['percent']}%)",
    ]


BAR = 300  # width of a full bar
ROWS = (("functions", "Functions"), ("fields", "Fields"), ("data_symbols", "Data symbols"))


def bar_width(entry: dict) -> float:
    """The filled part of a bar: its percentage of the full width, and never under 2 when anything has a name,
    so that a small count can still be seen."""
    return max(2.0, round(BAR * entry["percent"] / 100, 1)) if entry["named"] else 0.0


def indicator(counts: dict) -> str:
    """The picture for the README: one bar per kind, filled to its percentage, with the count beside it."""
    label = "Names given: " + ", ".join(f"{title.lower()} {counts[key]['percent']}%" for key, title in ROWS)
    width, top, step = 640, 34, 26
    height = top + step * len(ROWS) + 6
    font = 'font-family="sans-serif" font-size="13"'
    out = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}" role="img" '
        f'aria-label="{escape(label)}">',
        f"<title>{escape(label)}</title>",
        f'<rect x="0.5" y="0.5" width="{width - 1}" height="{height - 1}" rx="6" fill="#0d1117" stroke="#30363d"/>',
        f'<text x="14" y="22" {font} font-weight="bold" fill="#e6edf3">Names given instead of placeholders</text>',
    ]
    for row, (key, title) in enumerate(ROWS):
        entry = counts[key]
        y = top + step * row
        out += [
            f'<text x="14" y="{y + 11}" {font} fill="#e6edf3">{title}</text>',
            f'<rect x="120" y="{y}" width="{BAR}" height="14" rx="3" fill="#21262d"/>',
            f'<rect id="bar-{key}" x="120" y="{y}" width="{bar_width(entry)}" height="14" rx="3" fill="#2ea043"/>',
            f'<text x="{120 + BAR + 12}" y="{y + 11}" {font} fill="#e6edf3">{entry["percent"]}% '
            f'<tspan fill="#8b949e">({entry["named"]:,} of {entry["total"]:,})</tspan></text>',
        ]
    return "\n".join(out) + "\n</svg>\n"


def main(argv: list[str] | None = None) -> int:
    source = Path(__file__).resolve().parent.parent / "src"
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--config", default=str(source / "build.toml"))
    parser.add_argument("--fields", default=str(source / "types.fields"))
    parser.add_argument("--symbols", default=str(source / "symbols.ld"))
    parser.add_argument("--json", help="also write the counts to this file")
    parser.add_argument("--svg", help="also write the README's picture to this file: one bar per kind")
    args = parser.parse_args(argv)
    try:
        counts = count(Path(args.config), Path(args.fields), Path(args.symbols))
    except Problem as exc:
        print(f"namestats: {exc}", file=sys.stderr)
        return 1
    for target, text in ((args.json, json.dumps(counts, indent=1) + "\n"), (args.svg, indicator(counts))):
        if target:
            Path(target).parent.mkdir(parents=True, exist_ok=True)
            Path(target).write_text(text)
    print("\n".join(lines(counts)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
