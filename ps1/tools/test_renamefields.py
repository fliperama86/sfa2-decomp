#!/usr/bin/env python3
"""Controls for renamefields.py, on synthetic field tables and C files only.

Each case writes a small tree into a temporary directory, runs the command
line and checks the exit status, the output and the files afterwards. The
cases need a clang; pass `--clang` or have one on the usual path. They need
no game data and take a few seconds together.
"""

from __future__ import annotations

import argparse
import dataclasses
import shutil
import subprocess
import sys
from collections.abc import Callable
from pathlib import Path
import tempfile

TOOL = Path(__file__).resolve().parent / "renamefields.py"
DEFAULT_CLANG = "/opt/homebrew/opt/llvm/bin/clang"
CLANG = DEFAULT_CLANG

OLD = """\
struct Thing size=0x10
0x00 u8 field_00
0x01 u8 field_01
0x04 Thing* field_04
0x08 s16 kept
struct Other size=0x8
0x00 u8 field_00
0x04 s32 field_04
"""
# Thing.field_00 and Thing.field_04 get names; Other keeps both of its own.
NEW = OLD.replace("0x00 u8 field_00\n0x01", "0x00 u8 state\n0x01").replace("Thing* field_04", "Thing* next")
HEADER = '#include "types.gen.h"\n'


def tree(tmp: Path, files: dict[str, str], old: str = OLD, new: str = NEW) -> None:
    (tmp / "old.fields").write_text(old)
    (tmp / "new.fields").write_text(new)
    for name, text in files.items():
        path = tmp / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text)


def apply(tmp: Path, *extra: str, paths: tuple[str, ...] = ("src",)) -> subprocess.CompletedProcess:
    command = [sys.executable, str(TOOL), "apply", "--clang", CLANG, "--old", tmp / "old.fields", "--new", tmp / "new.fields"]
    command += [*extra, *(tmp / p for p in paths)]
    return subprocess.run(list(map(str, command)), capture_output=True, text=True)


def expect(proc: subprocess.CompletedProcess, status: int, *texts: str) -> str | None:
    output = proc.stdout + proc.stderr
    if proc.returncode != status:
        return f"exit status {proc.returncode}, wanted {status}: {output!r}"
    for text in texts:
        if text not in output:
            return f"output lacks {text!r}: {output!r}"
    return None


def case_pairs(tmp: Path) -> str | None:
    tree(tmp, {})
    proc = subprocess.run([sys.executable, str(TOOL), "pairs", str(tmp / "old.fields"), str(tmp / "new.fields")], capture_output=True, text=True)
    problem = expect(proc, 0, "Thing.field_00 -> state", "Thing.field_04 -> next", "2 renames")
    if problem is None and "Other." in proc.stdout:
        return f"a field of the other struct is listed: {proc.stdout!r}"
    return problem


def case_moved_field_is_not_a_rename(tmp: Path) -> str | None:
    # 'kept' leaves offset 8 for offset 0xc and another field takes its place.
    new = OLD.replace("0x08 s16 kept", "0x08 s16 fresh\n0x0c s16 kept")
    tree(tmp, {}, new=new)
    proc = subprocess.run([sys.executable, str(TOOL), "pairs", str(tmp / "old.fields"), str(tmp / "new.fields")], capture_output=True, text=True)
    return expect(proc, 0, "not taken for a rename", "0 renames")


def case_only_the_renamed_struct(tmp: Path) -> str | None:
    source = HEADER + (
        "/* field_00 of a Thing is its state */\n"
        "int f(Thing *t, Other *o) {\n"
        '    const char *s = "field_00";\n'
        "    o->field_00 = 1;\n"
        "    return t->field_00 + o->field_04 + (s != 0);\n"
        "}\n"
    )
    tree(tmp, {"src/a.c": source})
    problem = expect(apply(tmp), 0, "Thing.field_00 -> state: 1 uses", "0 left")
    if problem:
        return problem
    wanted = source.replace("return t->field_00", "return t->state")
    got = (tmp / "src/a.c").read_text()
    return None if got == wanted else f"the file is {got!r}"


def case_chain_and_two_on_a_line(tmp: Path) -> str | None:
    source = HEADER + "int f(Thing *t) { return t->field_04->field_04->field_00 + t->field_00; }\n"
    tree(tmp, {"src/a.c": source})
    problem = expect(apply(tmp), 0, "Thing.field_04 -> next: 2 uses", "Thing.field_00 -> state: 2 uses", "0 left")
    if problem:
        return problem
    got = (tmp / "src/a.c").read_text()
    wanted = HEADER + "int f(Thing *t) { return t->next->next->state + t->state; }\n"
    return None if got == wanted else f"the file is {got!r}"


def case_header_use_once(tmp: Path) -> str | None:
    shared = HEADER + "static int get(Thing *t) { return t->field_00; }\n"
    user = '#include "shared.h"\nint NAME(Thing *t) { return get(t); }\n'
    tree(tmp, {"src/shared.h": shared, "src/a.c": user.replace("NAME", "a"), "src/sub/b.c": user.replace("NAME", "b").replace("shared.h", "../shared.h")})
    problem = expect(apply(tmp), 0, "Thing.field_00 -> state: 1 uses", "0 left")
    if problem:
        return problem
    got = (tmp / "src/shared.h").read_text()
    return None if got == shared.replace("t->field_00", "t->state") else f"the header is {got!r}"


def case_macro_use_is_left(tmp: Path) -> str | None:
    source = HEADER + "#define STATE(t) ((t)->field_00)\nint f(Thing *t) { return STATE(t); }\n"
    tree(tmp, {"src/a.c": source})
    problem = expect(apply(tmp), 1, "LEFT:", "the token there is not the name")
    if problem:
        return problem
    return None if (tmp / "src/a.c").read_text() == source else "a file with a macro's use was rewritten"


def case_unread_file_with_the_name(tmp: Path) -> str | None:
    hidden = '#include "missing.h"\n' + HEADER + "int f(Thing *t) { return t->field_00; }\n"
    harmless = '#include "missing.h"\nint g(void) { return 0; }\n'
    tree(tmp, {"src/hidden.c": hidden, "src/harmless.c": harmless})
    problem = expect(apply(tmp), 1, "hidden.c: not read to the end", "it has the text field_00", "1 left, 2 files not read to the end")
    if problem:
        return problem
    return None if (tmp / "src/hidden.c").read_text() == hidden else "a file that was not read to the end was rewritten"


def case_outside_the_paths(tmp: Path) -> str | None:
    shared = HEADER + "static int get(Thing *t) { return t->field_00; }\n"
    tree(tmp, {"inc/shared.h": shared, "src/a.c": '#include "shared.h"\nint a(Thing *t) { return get(t); }\n'})
    problem = expect(apply(tmp, "-I", str(tmp / "inc")), 1, "is used outside the given paths")
    if problem:
        return problem
    return None if (tmp / "inc/shared.h").read_text() == shared else "a header outside the paths was rewritten"


def case_dry_run_writes_nothing(tmp: Path) -> str | None:
    source = HEADER + "int f(Thing *t) { return t->field_00; }\n"
    tree(tmp, {"src/a.c": source})
    problem = expect(apply(tmp, "--dry-run"), 0, "Thing.field_00 -> state: 1 uses", "1 uses found")
    if problem:
        return problem
    return None if (tmp / "src/a.c").read_text() == source else "a dry run rewrote a file"


def case_no_rename(tmp: Path) -> str | None:
    source = HEADER + "int f(Thing *t) { return t->field_00; }\n"
    tree(tmp, {"src/a.c": source}, new=OLD)
    problem = expect(apply(tmp), 0, "no field was renamed")
    if problem:
        return problem
    return None if (tmp / "src/a.c").read_text() == source else "a file changed without a rename"


def case_foreign_error_is_left(tmp: Path) -> str | None:
    # A member that never existed: not this run's to repair, and it must be said.
    source = HEADER + "int f(Thing *t) { return t->nothing + t->field_00; }\n"
    tree(tmp, {"src/a.c": source})
    problem = expect(apply(tmp), 1, "Thing.nothing is not a rename of this run", "Thing.field_00 -> state: 1 uses")
    if problem:
        return problem
    got = (tmp / "src/a.c").read_text()
    return None if got == source.replace("t->field_00", "t->state") else f"the file is {got!r}"


def case_bad_table(tmp: Path) -> str | None:
    tree(tmp, {"src/a.c": HEADER}, new=NEW + "0x00 u8 stray_overlap\nstruct Thing size=4\n")
    return expect(apply(tmp), 1, "duplicate struct name")


@dataclasses.dataclass
class Case:
    name: str
    func: Callable[[Path], str | None]


CASES = [
    Case("pairs-lists-renames-by-offset", case_pairs),
    Case("moved-field-is-not-a-rename", case_moved_field_is_not_a_rename),
    Case("only-the-renamed-struct-comment-and-string-kept", case_only_the_renamed_struct),
    Case("chain-and-two-uses-on-a-line", case_chain_and_two_on_a_line),
    Case("header-use-rewritten-once", case_header_use_once),
    Case("macro-use-is-left-and-reported", case_macro_use_is_left),
    Case("unread-file-with-the-name-is-left", case_unread_file_with_the_name),
    Case("header-outside-the-paths-is-left", case_outside_the_paths),
    Case("dry-run-writes-nothing", case_dry_run_writes_nothing),
    Case("no-rename-changes-nothing", case_no_rename),
    Case("foreign-member-error-is-left", case_foreign_error_is_left),
    Case("bad-table-is-refused", case_bad_table),
]


def main() -> int:
    global CLANG
    parser = argparse.ArgumentParser(description="Controls for renamefields.py")
    parser.add_argument("--clang", default=DEFAULT_CLANG if Path(DEFAULT_CLANG).exists() else shutil.which("clang"))
    args = parser.parse_args()
    if not args.clang:
        print("no clang found; pass --clang", file=sys.stderr)
        return 2
    CLANG = args.clang
    failed = 0
    for case in CASES:
        with tempfile.TemporaryDirectory(prefix="renamefields-test-") as tmp:
            problem = case.func(Path(tmp))
        print(f"{'ok  ' if problem is None else 'FAIL'} {case.name}" + ("" if problem is None else f": {problem}"))
        failed += problem is not None
    count = len(CASES)
    print(f"{failed} case(s) behaved wrongly" if failed else f"all {count} cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
