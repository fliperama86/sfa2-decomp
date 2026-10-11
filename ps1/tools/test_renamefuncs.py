#!/usr/bin/env python3
"""Controls for renamefuncs.py, on made-up configurations and files only.

Each case writes a small tree into a temporary directory, runs the command
line and checks the exit status, the output and the files afterwards. The
cases need nothing but Python and take under a second together.
"""

from __future__ import annotations

import dataclasses
import subprocess
import sys
import tempfile
from collections.abc import Callable
from pathlib import Path

TOOL = Path(__file__).resolve().parent / "renamefuncs.py"

OLD = """\
[[unit]]
name = "a"
source = "a.c"
functions = [
  { name = "func_80010000", address = 0x80010000, size = 8 },
  { name = "func_80010008", address = 0x80010008, size = 8 },
]

[[unit]]
name = "m"
image = "slot01"
source = "slot01/m.c"
functions = [{ name = "func_80010000_slot01", address = 0x80010000, size = 8 }]

[[unit]]
name = "m2"
image = "slot02"
source = "slot01/m.c"
functions = [{ name = "func_80010000_slot01", address = 0x80010000, size = 8 }]

[[image]]
name = "slot01"
symbols = { func_80010000 = 0x80010000 }
"""
# The resident function at 0x80010000 gets a name; the module's function at the same address keeps its own.
NEW = OLD.replace('name = "func_80010000", address', 'name = "advance", address').replace(
    "symbols = { func_80010000 = ", "symbols = { advance = "
)
SOURCE = (
    "/* calls func_80010000 */\n"
    "void func_80010000(void);\n"
    "void func_80010000_slot01(void);\n"
    'const char *n = "func_80010000";\n'
    "void func_80010008(void) { func_80010000(); func_80010000_slot01(); xfunc_80010000(); }\n"
)
WANTED = (
    "/* calls advance */\n"
    "void advance(void);\n"
    "void func_80010000_slot01(void);\n"
    'const char *n = "advance";\n'
    "void func_80010008(void) { advance(); func_80010000_slot01(); xfunc_80010000(); }\n"
)


def tree(tmp: Path, files: dict[str, str], old: str = OLD, new: str = NEW) -> None:
    (tmp / "old.toml").write_text(old)
    (tmp / "src").mkdir()
    (tmp / "src/build.toml").write_text(new)
    for name, text in files.items():
        path = tmp / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text)


def run(tmp: Path, *args: str) -> subprocess.CompletedProcess:
    return subprocess.run([sys.executable, str(TOOL), *args], capture_output=True, text=True, cwd=tmp)


def apply(tmp: Path, *extra: str) -> subprocess.CompletedProcess:
    return run(tmp, "apply", "--old", "old.toml", "--new", "src/build.toml", *extra)


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
    return expect(run(tmp, "pairs", "old.toml", "src/build.toml"), 0, "func_80010000 -> advance\n", "1 renames")


def case_whole_words_everywhere(tmp: Path) -> str | None:
    tree(tmp, {"src/a.c": SOURCE, "src/sub/x.json": '{"calls": ["func_80010000", "func_80010008"]}\n'})
    problem = expect(apply(tmp, "--fresh", "src"), 0, "func_80010000 -> advance: 5 places", "by kind of file: .c 4, .json 1", "1 renames, 5 places rewritten in 2 of 3 files")
    if problem:
        return problem
    if (tmp / "src/a.c").read_text() != WANTED:
        return f"the source is {(tmp / 'src/a.c').read_text()!r}"
    if (tmp / "src/sub/x.json").read_text() != '{"calls": ["advance", "func_80010008"]}\n':
        return "the fixtures file was not rewritten as wanted"
    return None if (tmp / "src/build.toml").read_text() == NEW else "the new configuration was changed"


def case_markdown_only_by_its_path(tmp: Path) -> str | None:
    page = "It calls `func_80010000`.\n"
    tree(tmp, {"src/a.c": SOURCE, "src/README.md": page, "docs/guide.md": page})
    problem = expect(apply(tmp, "src", "docs/guide.md"), 0, "by kind of file: .c 4, .md 1")
    if problem:
        return problem
    if (tmp / "src/README.md").read_text() != page:
        return "a page under a folder was rewritten"
    return None if (tmp / "docs/guide.md").read_text() == "It calls `advance`.\n" else "the page given by its path was not rewritten"


def case_fresh_refuses_a_taken_name(tmp: Path) -> str | None:
    source = SOURCE + "static int advance;\n"
    tree(tmp, {"src/a.c": source})
    problem = expect(apply(tmp, "--fresh", "src"), 1, "refused", "a.c:6: advance is written here already")
    if problem:
        return problem
    return None if (tmp / "src/a.c").read_text() == source else "a refused run rewrote a file"


def case_second_run_repairs(tmp: Path) -> str | None:
    # A tree that has the new names already, and one new file of a branch with the old one.
    tree(tmp, {"src/a.c": WANTED, "src/new.c": "void g(void) { func_80010000(); }\n"})
    problem = expect(apply(tmp, "src"), 0, "func_80010000 -> advance: 1 places")
    if problem:
        return problem
    if (tmp / "src/a.c").read_text() != WANTED:
        return "a file that had the new names was changed"
    return None if (tmp / "src/new.c").read_text() == "void g(void) { advance(); }\n" else "the branch's file was not repaired"


def case_skipped_folders_and_old_table(tmp: Path) -> str | None:
    tree(tmp, {"src/a.c": SOURCE, "src/build/cache.c": SOURCE, "src/sdk/include/x.h": SOURCE, "src/__pycache__/x.py": SOURCE, "src/old.toml": OLD})
    problem = expect(run(tmp, "apply", "--old", "src/old.toml", "--new", "src/build.toml", "src"), 0, "func_80010000 -> advance: 4 places", "in 1 of 2 files")
    if problem:
        return problem
    for name in ("src/build/cache.c", "src/sdk/include/x.h", "src/__pycache__/x.py"):
        if (tmp / name).read_text() != SOURCE:
            return f"{name} was rewritten"
    return None if (tmp / "src/old.toml").read_text() == OLD else "the old configuration was rewritten"


def case_dry_run_writes_nothing(tmp: Path) -> str | None:
    tree(tmp, {"src/a.c": SOURCE})
    problem = expect(apply(tmp, "--dry-run", "src"), 0, "4 places found")
    if problem:
        return problem
    return None if (tmp / "src/a.c").read_text() == SOURCE else "a dry run rewrote a file"


def case_name_of_another_function(tmp: Path) -> str | None:
    tree(tmp, {"src/a.c": SOURCE}, new=OLD.replace('name = "func_80010000", address', 'name = "func_80010008", address'))
    problem = expect(apply(tmp, "src"), 1, "refused", "another function has that name already")
    if problem:
        return problem
    return None if (tmp / "src/a.c").read_text() == SOURCE else "a refused run rewrote a file"


def case_two_into_one_name(tmp: Path) -> str | None:
    new = OLD.replace('name = "func_80010000", address', 'name = "same", address').replace('name = "func_80010008", address', 'name = "same", address')
    tree(tmp, {"src/a.c": SOURCE}, new=new)
    return expect(apply(tmp, "src"), 1, "refused", "would both be named same")


def case_one_of_two_placements(tmp: Path) -> str | None:
    # The module's source is linked into two images; a name for one of the two keys only cannot be exact.
    new = OLD.replace('image = "slot01"\nsource = "slot01/m.c"\nfunctions = [{ name = "func_80010000_slot01"', 'image = "slot01"\nsource = "slot01/m.c"\nfunctions = [{ name = "step"')
    tree(tmp, {"src/a.c": SOURCE}, new=new)
    return expect(apply(tmp, "src"), 1, "refused", "the old name is still a function of the new configuration")


def case_both_placements(tmp: Path) -> str | None:
    new = OLD.replace('functions = [{ name = "func_80010000_slot01"', 'functions = [{ name = "step"')
    tree(tmp, {"src/a.c": SOURCE}, new=new)
    problem = expect(apply(tmp, "--fresh", "src"), 0, "func_80010000_slot01 -> step: 2 places", "1 renames")
    if problem:
        return problem
    return None if "void step(void);" in (tmp / "src/a.c").read_text() and "func_80010000();" in (tmp / "src/a.c").read_text() else "the module's function was not renamed alone"


def case_not_an_identifier(tmp: Path) -> str | None:
    tree(tmp, {"src/a.c": SOURCE}, new=OLD.replace('name = "func_80010000", address', 'name = "9lives", address'))
    return expect(apply(tmp, "src"), 1, "refused", "not a C identifier")


def case_no_rename(tmp: Path) -> str | None:
    tree(tmp, {"src/a.c": SOURCE}, new=OLD)
    problem = expect(apply(tmp, "src"), 0, "no function was renamed")
    if problem:
        return problem
    return None if (tmp / "src/a.c").read_text() == SOURCE else "a file changed without a rename"


def case_bad_configuration(tmp: Path) -> str | None:
    tree(tmp, {"src/a.c": SOURCE}, new="[[unit\n")
    return expect(apply(tmp, "src"), 1, "refused", "build.toml")


@dataclasses.dataclass
class Case:
    name: str
    func: Callable[[Path], str | None]


CASES = [
    Case("pairs-lists-renames-by-image-and-address", case_pairs),
    Case("whole-words-in-code-comment-string-fixtures", case_whole_words_everywhere),
    Case("markdown-only-by-its-own-path", case_markdown_only_by_its_path),
    Case("fresh-refuses-a-name-already-written", case_fresh_refuses_a_taken_name),
    Case("second-run-repairs-a-branch-file", case_second_run_repairs),
    Case("skipped-folders-and-the-old-table", case_skipped_folders_and_old_table),
    Case("dry-run-writes-nothing", case_dry_run_writes_nothing),
    Case("name-of-another-function-is-refused", case_name_of_another_function),
    Case("two-functions-into-one-name-is-refused", case_two_into_one_name),
    Case("one-of-two-placements-is-refused", case_one_of_two_placements),
    Case("both-placements-renamed-together", case_both_placements),
    Case("not-an-identifier-is-refused", case_not_an_identifier),
    Case("no-rename-changes-nothing", case_no_rename),
    Case("bad-configuration-is-refused", case_bad_configuration),
]


def main() -> int:
    failed = 0
    for case in CASES:
        with tempfile.TemporaryDirectory(prefix="renamefuncs-test-") as tmp:
            problem = case.func(Path(tmp))
        print(f"{'ok  ' if problem is None else 'FAIL'} {case.name}" + ("" if problem is None else f": {problem}"))
        failed += problem is not None
    count = len(CASES)
    print(f"{failed} case(s) behaved wrongly" if failed else f"all {count} cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
