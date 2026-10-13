#!/usr/bin/env python3
"""Controls for renamedata.py, on made-up symbol files and sources only.

Each case writes a small tree into a temporary directory, runs the command
line and checks the exit status, the output and the files afterwards. The
cases need nothing but Python and take under a second together.
"""

from __future__ import annotations

import subprocess
import sys
import tempfile
from collections.abc import Callable
from pathlib import Path

TOOL = Path(__file__).resolve().parent / "renamedata.py"

SYMBOLS = "data_80010000 = 0x80010000;\ntable_80010010 = 0x80010010;\nkept_name = 0x80010020;\n"
SOURCE = (
    "extern int data_80010000;\n"
    "extern u8 table_80010010[];\n"
    "extern int data_80010000_rows;\n"
    "/* reads data_80010000 */\n"
    "int f(void) { return data_80010000 + xdata_80010000 + table_80010010[0]; }\n"
)
WANTED = (
    "extern int round_timer;\n"
    "extern u8 stage_table[];\n"
    "extern int data_80010000_rows;\n"
    "/* reads round_timer */\n"
    "int f(void) { return round_timer + xdata_80010000 + stage_table[0]; }\n"
)
MAP = "old\tnew\tconfidence\tevidence\ndata_80010000\tround_timer\thigh\tf\ntable_80010010\tstage_table\tmedium\tf\n"


def tree(tmp: Path, files: dict[str, str], table: str = MAP, symbols: str = SYMBOLS) -> None:
    (tmp / "map.tsv").write_text(table)
    (tmp / "src").mkdir()
    (tmp / "src/symbols.ld").write_text(symbols)
    for name, text in files.items():
        path = tmp / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text)


def run(tmp: Path, command: str, *extra: str) -> subprocess.CompletedProcess:
    args = [command, "--map", "map.tsv", "--symbols", "src/symbols.ld", *extra, "src"]
    return subprocess.run([sys.executable, str(TOOL), *args], capture_output=True, text=True, cwd=tmp)


def expect(proc: subprocess.CompletedProcess, status: int, *texts: str) -> str | None:
    output = proc.stdout + proc.stderr
    if proc.returncode != status:
        return f"exit status {proc.returncode}, wanted {status}: {output!r}"
    for text in texts:
        if text not in output:
            return f"output lacks {text!r}: {output!r}"
    return None


def unchanged(tmp: Path, name: str, text: str) -> str | None:
    return None if (tmp / name).read_text() == text else f"{name} was changed: {(tmp / name).read_text()!r}"


def case_whole_words_everywhere(tmp: Path) -> str | None:
    tree(tmp, {"src/a.c": SOURCE, "src/sub/x.py": "CALLS = ['table_80010010']\n"})
    problem = expect(run(tmp, "apply"), 0, "data_80010000 -> round_timer: 4 places", "2 renames, 8 places rewritten in 3 of 3 files")
    if problem:
        return problem
    if (tmp / "src/a.c").read_text() != WANTED:
        return f"the source is {(tmp / 'src/a.c').read_text()!r}"
    if (tmp / "src/sub/x.py").read_text() != "CALLS = ['stage_table']\n":
        return "the contract file was not rewritten as wanted"
    return (
        None
        if (tmp / "src/symbols.ld").read_text() == "round_timer = 0x80010000;\nstage_table = 0x80010010;\nkept_name = 0x80010020;\n"
        else "the symbol file was not rewritten with the address kept"
    )


def case_check_writes_nothing(tmp: Path) -> str | None:
    tree(tmp, {"src/a.c": SOURCE})
    problem = expect(run(tmp, "check"), 0, "2 renames, 7 places to rewrite in 2 of 2 files")
    return problem or unchanged(tmp, "src/a.c", SOURCE)


def case_markdown_only_by_its_path(tmp: Path) -> str | None:
    page = "It reads `data_80010000`.\n"
    tree(tmp, {"src/a.c": SOURCE, "src/README.md": page, "docs/guide.md": page})
    proc = subprocess.run(
        [sys.executable, str(TOOL), "apply", "--map", "map.tsv", "--symbols", "src/symbols.ld", "src", "docs/guide.md"],
        capture_output=True, text=True, cwd=tmp,
    )
    problem = expect(proc, 0)
    if problem:
        return problem
    if (tmp / "src/README.md").read_text() != page:
        return "a page under a folder was rewritten"
    return unchanged(tmp, "docs/guide.md", "It reads `round_timer`.\n")


def case_taken_name_is_refused(tmp: Path) -> str | None:
    source = SOURCE + "static int round_timer;\n"
    tree(tmp, {"src/a.c": source})
    problem = expect(run(tmp, "apply"), 1, "refused", "a.c:6: round_timer is written here already")
    return problem or unchanged(tmp, "src/a.c", source)


def case_bad_names_are_refused(tmp: Path) -> str | None:
    table = "data_80010000\tRoundTimer\ntable_80010010\tdata_80010099\nkept_name\tother\ndata_80010004\tx\n"
    tree(tmp, {"src/a.c": SOURCE}, table=table)
    problem = expect(
        run(tmp, "apply"), 1,
        "RoundTimer: a new name is lower case", "data_80010099: a new name is lower case",
        "kept_name: not a placeholder", "data_80010004: the symbol file defines neither",
    )
    return problem or unchanged(tmp, "src/a.c", SOURCE)


def case_repeats_are_refused(tmp: Path) -> str | None:
    table = "data_80010000\tsame_name\ntable_80010010\tsame_name\ndata_80010000\tother_name\n"
    tree(tmp, {"src/a.c": SOURCE}, table=table)
    problem = expect(run(tmp, "apply"), 1, "data_80010000 is renamed twice", "same_name is the new name of more than one symbol")
    return problem or unchanged(tmp, "src/a.c", SOURCE)


def case_second_run_repairs_a_branch(tmp: Path) -> str | None:
    # A tree that has the new names, and one new file of a branch that still has the old one.
    symbols = "round_timer = 0x80010000;\nstage_table = 0x80010010;\n"
    tree(tmp, {"src/a.c": WANTED, "src/new.c": "int g(void) { return data_80010000; }\n"}, symbols=symbols)
    problem = expect(run(tmp, "apply"), 0, "data_80010000 -> round_timer: 1 places")
    if problem:
        return problem
    problem = unchanged(tmp, "src/a.c", WANTED)
    return problem or unchanged(tmp, "src/new.c", "int g(void) { return round_timer; }\n")


CASES: list[Callable[[Path], str | None]] = [
    case_whole_words_everywhere,
    case_check_writes_nothing,
    case_markdown_only_by_its_path,
    case_taken_name_is_refused,
    case_bad_names_are_refused,
    case_repeats_are_refused,
    case_second_run_repairs_a_branch,
]


def main() -> int:
    failures = 0
    for case in CASES:
        with tempfile.TemporaryDirectory() as directory:
            problem = case(Path(directory))
        print(f"{'ok  ' if problem is None else 'FAIL'} {case.__name__}" + (f": {problem}" if problem else ""))
        failures += problem is not None
    print(f"{len(CASES) - failures}/{len(CASES)} passed")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
