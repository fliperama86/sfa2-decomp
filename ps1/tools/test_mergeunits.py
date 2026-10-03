#!/usr/bin/env python3
"""Controls for mergeunits.py, on synthetic configuration directories only.

Each case builds a base directory and unit copies in a temporary location,
runs the command line and checks the exit status, the message, the output
files and, for conflicts, that nothing was written.
"""

from __future__ import annotations

import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import structgen  # noqa: E402

TOOL = Path(__file__).resolve().parent / "mergeunits.py"

BASE_BUILD = """\
# synthetic
[toolchain]
cpp = "clang"

[[unit]]
name = "alpha"
source = "alpha.c"
flags = ["-O2", "-G0"]
functions = [
  { name = "alpha_fn", address = 0x80000100, size = 64 },
]

[types]
fields = "types.fields"
"""

BASE_SYMBOLS = """\
/* Synthetic symbols. */

/* Group one. */
sym_a = 0x1000;
sym_b = 0x1004;
"""

BASE_FIELDS = "struct S size=8\n0x0 u32 a\n"


def unit_text(name: str, functions: list[tuple[str, int, int]]) -> str:
    lines = [f'[[unit]]\nname = "{name}"\nsource = "{name}.c"\nflags = ["-O2", "-G0"]\nfunctions = [']
    for fn, address, size in functions:
        lines.append(f'  {{ name = "{fn}", address = {address:#x}, size = {size} }},')
    lines.append("]\n")
    return "\n".join(lines)


def write(directory: Path, relative: str, text: str) -> None:
    path = directory / relative
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text)


def make_base(root: Path) -> Path:
    base = root / "base"
    write(base, "build.toml", BASE_BUILD)
    write(base, "symbols.ld", BASE_SYMBOLS)
    write(base, "types.fields", BASE_FIELDS)
    write(base, "alpha.c", "int alpha_fn(void) { return 1; }\n")
    write(base, "shared.h", "/* shared */\n")
    write(base, "sub/data.h", "/* data */\n")
    return base


def make_unit(root: Path, base: Path, name: str, *, functions=(), symbols="", fields="", files=None) -> Path:
    """Copy the base, then add one unit the way an agent would."""
    directory = root / name
    for path in base.rglob("*"):
        if path.is_file():
            write(directory, path.relative_to(base).as_posix(), path.read_text())
    if functions:
        with open(directory / "build.toml", "a") as handle:
            handle.write("\n" + unit_text(name, list(functions)))
    write(directory, f"{name}.c", f"/* {name} */\n")
    with open(directory / "symbols.ld", "a") as handle:
        handle.write(symbols)
    with open(directory / "types.fields", "a") as handle:
        handle.write(fields)
    for relative, text in (files or {}).items():
        write(directory, relative, text)
    return directory


def run(*args) -> subprocess.CompletedProcess:
    return subprocess.run([sys.executable, str(TOOL), *map(str, args)], capture_output=True, text=True)


def out(proc) -> str:
    return proc.stdout + proc.stderr


class Case:
    def __init__(self, name, func):
        self.name, self.func = name, func


def setup_two(root: Path, **second):
    base = make_base(root)
    one = make_unit(
        root,
        base,
        "u_one",
        functions=[("one_fn", 0x80000200, 32), ("one_helper", 0x80000220, 16)],
        symbols="sym_c = 0x1008;\none_fn = 0x80000200;\n",
        fields="struct T size=4\n0x0 u16 x\n",
    )
    two = make_unit(
        root,
        base,
        "u_two",
        functions=second.pop("functions", [("two_fn", 0x80000300, 8)]),
        symbols=second.pop("symbols", "sym_d = 0x100c;\nsym_c = 0x1008;\n"),
        fields=second.pop("fields", "struct U size=4\n0x0 u8 y\n"),
        **second,
    )
    return base, one, two


def expect_conflict(proc, root: Path, message: str):
    if proc.returncode != 1:
        return f"expected exit 1, got {proc.returncode}:\n{out(proc)}"
    if message not in out(proc):
        return f"wanted {message!r} in:\n{out(proc)}"
    if (root / "out").exists():
        return "output written despite the conflict"
    return None


def case_clean(d: Path):
    base, one, two = setup_two(d, files={"new/two.h": "x\n"})
    proc = run("--base", base, "--out", d / "out", one, two)
    if proc.returncode != 0:
        return out(proc)
    want_symbols = (
        BASE_SYMBOLS
        + "\n/* Added by u_one. */\nsym_c = 0x1008;\n"
        + "\n/* Added by u_two. */\nsym_d = 0x100c;\n"
    )
    got = (d / "out" / "symbols.ld").read_text()
    if got != want_symbols:
        return f"symbols.ld differs:\n{got}"
    want_build = (
        BASE_BUILD
        + "\n"
        + unit_text("u_one", [("one_fn", 0x80000200, 32), ("one_helper", 0x80000220, 16)])
        + "\n"
        + unit_text("u_two", [("two_fn", 0x80000300, 8)])
    )
    got = (d / "out" / "build.toml").read_text()
    if got != want_build:
        return f"build.toml differs:\n{got}"
    model = structgen.parse((d / "out" / "types.fields").read_text())
    if sorted(s.name for s in model.structs) != ["S", "T", "U"]:
        return "merged types.fields lacks a struct"
    for relative in ("u_one.c", "u_two.c", "new/two.h", "sub/data.h"):
        if not (d / "out" / relative).exists():
            return f"missing {relative} in the output"
    for needle in ("units added: 2", "fields added: 2", "files copied: 3"):
        if needle not in out(proc):
            return f"summary lacks {needle!r}:\n{out(proc)}"
    return None


def case_symbol_removed(d: Path):
    base, one, two = setup_two(d)
    base_ld = base / "symbols.ld"
    # A base statement that a merged unit now defines goes too, the comment stays.
    write(one, "symbols.ld", BASE_SYMBOLS + "one_fn = 0x80000200;\nsym_x = 0x2000;\n")
    write(base, "symbols.ld", BASE_SYMBOLS + "two_fn = 0x80000300;\n")
    write(two, "symbols.ld", BASE_SYMBOLS + "two_fn = 0x80000300;\n")
    proc = run("--base", base, "--out", d / "out", one, two)
    if proc.returncode != 0:
        return out(proc)
    got = (d / "out" / "symbols.ld").read_text()
    want = BASE_SYMBOLS + "\n/* Added by u_one. */\nsym_x = 0x2000;\n"
    if got != want:
        return f"symbols.ld differs:\n{got}"
    return None if base_ld.exists() else "base vanished"


def case_symbol_conflict(d: Path):
    base, one, two = setup_two(d, symbols="sym_c = 0x2008;\n")
    return expect_conflict(run("--base", base, "--out", d / "out", one, two), d, "'sym_c'")


def case_symbol_changes_base_value(d: Path):
    base, one, two = setup_two(d, symbols="")
    write(two, "symbols.ld", BASE_SYMBOLS.replace("0x1000", "0x1111"))
    return expect_conflict(run("--base", base, "--out", d / "out", one, two), d, "'sym_a'")


def case_same_value_warning(d: Path):
    base, one, two = setup_two(d, symbols="sym_alias = 0x1008;\n")
    proc = run("--base", base, "--out", d / "out", one, two)
    if proc.returncode != 0:
        return out(proc)
    if "WARNINGS" not in out(proc) or "sym_alias" not in out(proc):
        return f"no warning:\n{out(proc)}"
    return None


def case_duplicate_unit(d: Path):
    base, one, two = setup_two(d, functions=[("one_fn", 0x80000200, 32)])
    write(two, "build.toml", BASE_BUILD + "\n" + unit_text("u_one", [("other", 0x80000400, 8)]))
    return expect_conflict(run("--base", base, "--out", d / "out", one, two), d, "unit 'u_one' is added by both")


def case_base_unit_changed(d: Path):
    base, one, two = setup_two(d)
    text = (two / "build.toml").read_text().replace("size = 64", "size = 68")
    write(two, "build.toml", text)
    return expect_conflict(run("--base", base, "--out", d / "out", one, two), d, "changes base unit 'alpha'")


def case_base_unit_removed(d: Path):
    base, one, two = setup_two(d)
    write(two, "build.toml", "[types]\nfields = \"types.fields\"\n")
    return expect_conflict(run("--base", base, "--out", d / "out", one, two), d, "removes base unit 'alpha'")


def case_field_conflict(d: Path):
    base, one, two = setup_two(d, fields="struct T size=4\n0x0 u8 x\n")
    return expect_conflict(run("--base", base, "--out", d / "out", one, two), d, "conflict: struct 'T'")


def case_new_file_differs(d: Path):
    base, one, two = setup_two(d)
    write(one, "same.h", "a\n")
    write(two, "same.h", "b\n")
    return expect_conflict(run("--base", base, "--out", d / "out", one, two), d, "new file same.h differs")


def case_new_file_identical(d: Path):
    base, one, two = setup_two(d)
    write(one, "same.h", "a\n")
    write(two, "same.h", "a\n")
    proc = run("--base", base, "--out", d / "out", one, two)
    if proc.returncode != 0:
        return out(proc)
    return None if (d / "out" / "same.h").read_text() == "a\n" else "file not copied"


def case_modified_shared(d: Path):
    base, one, two = setup_two(d)
    write(one, "shared.h", "/* changed */\n")
    return expect_conflict(run("--base", base, "--out", d / "out", one, two), d, "modified shared file shared.h")


def case_take(d: Path):
    base, one, two = setup_two(d)
    write(one, "shared.h", "/* changed */\n")
    proc = run("--base", base, "--out", d / "out", "--take", f"{one}:shared.h", one, two)
    if proc.returncode != 0:
        return out(proc)
    return None if (d / "out" / "shared.h").read_text() == "/* changed */\n" else "taken version not used"


def case_double_take(d: Path):
    base, one, two = setup_two(d)
    write(one, "shared.h", "/* one */\n")
    write(two, "shared.h", "/* two */\n")
    proc = run("--base", base, "--out", d / "out", "--take", f"{one}:shared.h", "--take", f"{two}:shared.h", one, two)
    return expect_conflict(proc, d, "--take names shared.h for both")


def case_missing_base_file(d: Path):
    base, one, two = setup_two(d)
    (two / "sub" / "data.h").unlink()
    return expect_conflict(run("--base", base, "--out", d / "out", one, two), d, "missing base file sub/data.h")


def case_existing_out(d: Path):
    base, one, two = setup_two(d)
    (d / "out").mkdir()
    proc = run("--base", base, "--out", d / "out", one, two)
    if proc.returncode != 2:
        return f"expected exit 2, got {proc.returncode}:\n{out(proc)}"
    return None if "output exists" in out(proc) else f"no message:\n{out(proc)}"


def case_missing_directory(d: Path):
    base, one, _ = setup_two(d)
    proc = run("--base", base, "--out", d / "out", one, d / "nope")
    return None if proc.returncode == 2 else f"expected exit 2, got {proc.returncode}"


def case_conflicts_listed_together(d: Path):
    base, one, two = setup_two(d, symbols="sym_c = 0x2008;\n", fields="struct T size=4\n0x0 u8 x\n")
    write(one, "same.h", "a\n")
    write(two, "same.h", "b\n")
    proc = run("--base", base, "--out", d / "out", one, two)
    lines = [line for line in out(proc).splitlines() if line.startswith("error:")]
    if proc.returncode != 1 or len(lines) != 3 or (d / "out").exists():
        return f"expected three conflicts and no output:\n{out(proc)}"
    return None


CASES = [
    Case("clean-merge", case_clean),
    Case("symbol-removed-when-unit-defines-it", case_symbol_removed),
    Case("symbol-conflict-between-units", case_symbol_conflict),
    Case("symbol-conflict-with-base", case_symbol_changes_base_value),
    Case("same-value-different-name-warns", case_same_value_warning),
    Case("duplicate-unit-name", case_duplicate_unit),
    Case("base-unit-changed", case_base_unit_changed),
    Case("base-unit-removed", case_base_unit_removed),
    Case("field-conflict-from-structgen", case_field_conflict),
    Case("new-file-differs", case_new_file_differs),
    Case("new-file-identical-is-fine", case_new_file_identical),
    Case("modified-shared-file-refused", case_modified_shared),
    Case("modified-shared-file-taken", case_take),
    Case("double-take", case_double_take),
    Case("missing-base-file", case_missing_base_file),
    Case("existing-out", case_existing_out),
    Case("missing-directory", case_missing_directory),
    Case("conflicts-listed-together-nothing-written", case_conflicts_listed_together),
]


def main() -> int:
    failed = 0
    for case in CASES:
        with tempfile.TemporaryDirectory(prefix="mergeunits-test-") as tmp:
            problem = case.func(Path(tmp))
        print(f"{'ok  ' if problem is None else 'FAIL'} {case.name}" + ("" if problem is None else f": {problem}"))
        failed += problem is not None
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
