#!/usr/bin/env python3
"""Controls for namestats.py, on made-up tables only.

Each case writes a small build configuration, field table and symbol table into a temporary directory, runs
the command line and checks the exit status, the printed lines and the files it wrote. The expected counts are
worked out here from each table, not read from the tool. The cases need nothing but Python and take under a
second together.
"""

from __future__ import annotations

import dataclasses
import json
import re
import subprocess
import sys
import tempfile
from collections.abc import Callable
from pathlib import Path

TOOL = Path(__file__).resolve().parent / "namestats.py"

# Resident program: two game units with four functions, one of them named; one library unit, not counted.
# Modules: three functions in two images, one of them named; a placeholder keeps its image's suffix.
CONFIG = """\
[[unit]]
name = "a"
source = "a.c"
functions = [
  { name = "func_80010000", address = 0x80010000, size = 8 },
  { name = "advance_timer", address = 0x80010008, size = 8 },
]

[[unit]]
name = "b"
source = "asm/b.s"
kind = "asm"
functions = [
  { name = "func_80010010", address = 0x80010010, size = 8 },
  { name = "func_80010018", address = 0x80010018, size = 8 },
]

[[unit]]
name = "lib"
source = "sdk/libgpu/sys.c"
functions = [
  { name = "LoadImage", address = 0x80020000, size = 8 },
  { name = "func_80020008", address = 0x80020008, size = 8 },
]

[[unit]]
name = "m1"
source = "slot01/m1.c"
image = "slot01"
functions = [
  { name = "func_80030000_slot01", address = 0x80030000, size = 8 },
  { name = "draw_title", address = 0x80030008, size = 8 },
]

[[unit]]
name = "m2"
source = "slot02/m2.c"
image = "slot02"
functions = [
  { name = "func_80030000_slot02", address = 0x80030000, size = 8 },
]
"""

# Five fields: two named (one an array), three placeholders (one an array).
FIELDS = """\
struct Thing size=0x10
0x000 u16 timer
0x002 u16 field_02
0x004 u32 field_04
0x008 u8 bytes[4]
0x00c u8 field_0c[4]
"""

# Seven data symbols: three named, four placeholders. Not counted: the two function placeholders, the name at
# the address of a resident function (LoadImage), and whatever stands inside a comment.
SYMBOLS = """\
/* Addresses of symbols not defined by C units.
   inside_a_comment = 0x80040000; */
score_left = 0x80040000;
score_right = 0x80040004;
data_80040008 = 0x80040008;
table_8004000c = 0x8004000c;
jtbl_80040010 = 0x80040010;
data_80050000_slot01 = 0x80050000;
menu_rows_slot01 = 0x80050004;
func_80020000 = 0x80020000;
func_80030000_slot01 = 0x80030000;
LoadImage = 0x80020000;
"""

LINES = [
    "functions: 2 of 7 have a name (28.6%); resident program 1 of 4 (25.0%), modules 1 of 3 (33.3%)",
    "fields: 2 of 5 have a name (40.0%)",
    "data symbols: 3 of 7 have a name (42.9%)",
]


def write(tmp: Path, config: str = CONFIG, fields: str = FIELDS, symbols: str = SYMBOLS) -> list[str]:
    (tmp / "build.toml").write_text(config)
    (tmp / "types.fields").write_text(fields)
    (tmp / "symbols.ld").write_text(symbols)
    return ["--config", str(tmp / "build.toml"), "--fields", str(tmp / "types.fields"), "--symbols", str(tmp / "symbols.ld")]


def run(arguments: list[str]) -> subprocess.CompletedProcess:
    return subprocess.run([sys.executable, str(TOOL), *arguments], capture_output=True, text=True)


def case_counts(tmp: Path) -> str | None:
    result = run(write(tmp))
    if result.returncode != 0:
        return f"status {result.returncode}: {result.stderr}"
    if result.stdout.splitlines() != LINES:
        return f"printed {result.stdout!r}"
    return None


def case_files(tmp: Path) -> str | None:
    result = run(write(tmp) + ["--json", str(tmp / "out" / "names.json"), "--svg", str(tmp / "out" / "names.svg")])
    if result.returncode != 0:
        return f"status {result.returncode}: {result.stderr}"
    counts = json.loads((tmp / "out" / "names.json").read_text())
    expected = {
        "functions": {"named": 2, "total": 7, "percent": 28.6, "resident": {"named": 1, "total": 4, "percent": 25.0},
                      "modules": {"named": 1, "total": 3, "percent": 33.3}},
        "fields": {"named": 2, "total": 5, "percent": 40.0},
        "data_symbols": {"named": 3, "total": 7, "percent": 42.9},
    }
    if counts != expected:
        return f"the JSON holds {counts}"
    picture = (tmp / "out" / "names.svg").read_text()
    label = "Names given: functions 28.6%, fields 40.0%, data symbols 42.9%"
    if not picture.startswith("<svg ") or picture.count(label) != 2:
        return f"the picture is {picture!r}"
    # a full bar is 300 wide: 28.6%, 40.0% and 42.9% of it
    if bars(picture) != {"functions": 85.8, "fields": 120.0, "data_symbols": 128.7}:
        return f"the bars are {bars(picture)}"
    for text in ('28.6% <tspan fill="#8b949e">(2 of 7)</tspan>', '40.0% <tspan fill="#8b949e">(2 of 5)</tspan>',
                 '42.9% <tspan fill="#8b949e">(3 of 7)</tspan>'):
        if picture.count(text) != 1:
            return f"the picture lacks {text!r}"
    return None


def bars(picture: str) -> dict[str, float]:
    return {key: float(width) for key, width in re.findall(r'<rect id="bar-(\w+)" x="120" y="\d+" width="([0-9.]+)"', picture)}


def case_small_and_empty_bars(tmp: Path) -> str | None:
    # 1 function of 400 has a name: 0.2%, which would be 0.6 of 300 and is drawn 2 wide; a count of 1,000 and
    # more is written with a comma; no field has a name, so that bar is empty
    functions = "\n".join(f'  {{ name = "func_{0x80010000 + 8 * i:08x}", address = {0x80010000 + 8 * i:#x}, size = 8 }},' for i in range(399))
    config = f'[[unit]]\nname = "a"\nsource = "a.c"\nfunctions = [\n  {{ name = "advance_timer", address = 0x8000fff8, size = 8 }},\n{functions}\n]\n'
    symbols = "".join(f"data_{0x80040000 + 4 * i:08x} = {0x80040000 + 4 * i:#x};\n" for i in range(1000)) + "score_left = 0x80050000;\n"
    result = run(write(tmp, config=config, fields="struct Thing size=0x4\n0x000 u32 field_00\n", symbols=symbols) + ["--svg", str(tmp / "names.svg")])
    if result.returncode != 0:
        return f"status {result.returncode}: {result.stderr}"
    picture = (tmp / "names.svg").read_text()
    if bars(picture) != {"functions": 2.0, "fields": 0.0, "data_symbols": 2.0}:
        return f"the bars are {bars(picture)}"
    for text in ("(1 of 400)", "(0 of 1)", "(1 of 1,001)"):
        if picture.count(text) != 1:
            return f"the picture lacks {text!r}"
    return None


def case_a_new_name_is_one_more(tmp: Path) -> str | None:
    arguments = write(tmp, config=CONFIG.replace("func_80010010", "clear_timer"),
                      fields=FIELDS.replace("field_04", "position"),
                      symbols=SYMBOLS.replace("table_8004000c", "row_heights"))
    result = run(arguments)
    expected = [
        "functions: 3 of 7 have a name (42.9%); resident program 2 of 4 (50.0%), modules 1 of 3 (33.3%)",
        "fields: 3 of 5 have a name (60.0%)",
        "data symbols: 4 of 7 have a name (57.1%)",
    ]
    if result.returncode != 0 or result.stdout.splitlines() != expected:
        return f"status {result.returncode}, printed {result.stdout!r}"
    return None


def case_empty_tables(tmp: Path) -> str | None:
    result = run(write(tmp, config="", fields="", symbols=""))
    expected = [
        "functions: 0 of 0 have a name (0.0%); resident program 0 of 0 (0.0%), modules 0 of 0 (0.0%)",
        "fields: 0 of 0 have a name (0.0%)",
        "data symbols: 0 of 0 have a name (0.0%)",
    ]
    if result.returncode != 0 or result.stdout.splitlines() != expected:
        return f"status {result.returncode}, printed {result.stdout!r}"
    return None


def case_missing_file(tmp: Path) -> str | None:
    arguments = write(tmp)
    (tmp / "symbols.ld").unlink()
    result = run(arguments + ["--json", str(tmp / "names.json")])
    if result.returncode != 1 or "symbols.ld" not in result.stderr or result.stdout:
        return f"status {result.returncode}, printed {result.stdout!r}, said {result.stderr!r}"
    if (tmp / "names.json").exists():
        return "a file was written although the counts could not be made"
    return None


def case_bad_field_table(tmp: Path) -> str | None:
    result = run(write(tmp, fields="0x000 u16 timer\n"))
    if result.returncode != 1 or "field outside a struct" not in result.stderr or result.stdout:
        return f"status {result.returncode}, printed {result.stdout!r}, said {result.stderr!r}"
    return None


@dataclasses.dataclass
class Case:
    name: str
    func: Callable[[Path], str | None]


CASES = [
    Case("counts-of-the-three-kinds", case_counts),
    Case("json-and-picture-hold-the-same-counts", case_files),
    Case("small-count-is-visible-and-none-is-empty", case_small_and_empty_bars),
    Case("a-new-name-is-one-more-of-its-kind", case_a_new_name_is_one_more),
    Case("empty-tables-count-nothing", case_empty_tables),
    Case("missing-file-is-reported", case_missing_file),
    Case("bad-field-table-is-reported", case_bad_field_table),
]


def main() -> int:
    failed = 0
    for case in CASES:
        with tempfile.TemporaryDirectory(prefix="namestats-test-") as tmp:
            problem = case.func(Path(tmp))
        print(f"{'ok  ' if problem is None else 'FAIL'} {case.name}" + ("" if problem is None else f": {problem}"))
        failed += problem is not None
    count = len(CASES)
    print(f"{failed} case(s) behaved wrongly" if failed else f"all {count} cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
