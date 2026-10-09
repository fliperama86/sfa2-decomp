#!/usr/bin/env python3
"""Controls of difftest.py that need the pinned toolchain and the private inputs of the matching build.

    python test_difftest_build.py --config ../build.toml

The units are made from this folder's func_801e9080_slot06_00.c by text edits (each edit must
apply exactly once) and written to a folder of this run's own under ps1/build/, which the
repository ignores; their includes are pointed at ps1/src from there. The run removes its own
folder in every outcome and touches no other: two runs side by side do not disturb each other,
and what a killed run leaves behind stays in the ignored place. Output as in test_difftest.py: one
line per case, then `all cases behaved as required` (status 0), a count of failures (status 1),
or one line and status 2 when the configuration, the toolchain or the private inputs are missing.
"""

from __future__ import annotations

import argparse
import contextlib
import os
import subprocess
import gc
import io
import re
import shutil
import signal
import struct
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import difftest  # noqa: E402
import matchbuild  # noqa: E402

D = difftest
NAME = "func_801e9080_slot06_00"
ORIGINAL_ADDRESS = 0x801E9080
PREFIX = "run-"
WORKSPACES = HERE.parents[1] / "build" / "difftest-controls"  # ps1/build/ is ignored by the repository
CASES_RUN = 30

_machine = D.machine


def machine_after_collection(code):
    """Dead emulators keep large address ranges until collected; a run under an address-space limit must not pile them up."""
    gc.collect()
    return _machine(code)


D.machine = machine_after_collection

SOURCE = (HERE / f"{NAME}.c").read_text()


def edit(text: str, old: str, new: str) -> str:
    """Replace `old` by `new`, which must apply exactly once."""
    if text.count(old) != 1:
        raise AssertionError(f"edit applies {text.count(old)} times: {old!r}")
    return text.replace(old, new)


ANCHOR_DECL = "extern Slot06Prim data_801f3050_slot06_00[2][85];\n"
ANCHOR_END = "    data_801adfe4 = cnt;\n}"
ANCHOR_RETURN = "    if (game_state.field_64 != 0) return;\n"


def unit(declarations: str = "", before_end: str = "", after_return: str = "") -> str:
    text = edit(SOURCE, ANCHOR_DECL, ANCHOR_DECL + declarations)
    text = edit(text, ANCHOR_END, before_end + ANCHOR_END)
    return edit(text, ANCHOR_RETURN, ANCHOR_RETURN + after_return)


def make_workspace(root: Path = WORKSPACES) -> Path:
    """A folder of this run's own for its throwaway sources. Nothing of another run is touched."""
    root.mkdir(parents=True, exist_ok=True)
    return Path(tempfile.mkdtemp(prefix=PREFIX, dir=root))


def remove_workspace(folder: Path) -> None:
    """Remove this run's folder, and only it."""
    shutil.rmtree(folder, ignore_errors=True)


def rebased(text: str, folder: Path) -> str:
    """The unit's text with its `../NAME` includes pointed at ps1/src from `folder`."""
    source = os.path.relpath(HERE.parent, folder).replace(os.sep, "/")
    lines = [re.sub(r'^(\s*#\s*include\s+")\.\./', lambda m: f"{m.group(1)}{source}/", line)
             for line in text.splitlines(keepends=True)]
    return "".join(lines)


class Rig:
    """The configuration, the memory image and this run's own source folder."""

    def __init__(self, cfg, folder: Path):
        self.cfg, self.folder = cfg, folder
        self.ram = D.initial_memory(cfg, D.image_of(NAME))
        self.address, self.original_size = D.original_function(cfg, NAME)
        self.counter = 0

    def build(self, text: str, hide: Path | None = None):
        """Build a unit made from `text`. Returns D.Build; raises what build_function raises."""
        (self.folder / f"{NAME}.c").write_text(rebased(text, self.folder))
        with tempfile.TemporaryDirectory(prefix="difftest-") as directory:
            return D.build_function(self.cfg, NAME, Path(directory), self.folder)

    def run(self, build, cases: int = CASES_RUN):
        discarded, equal, different, first, executed = D.test_function(
            self.cfg, NAME, build.code, cases, 1, self.ram, bytes(D.SCRATCH_SIZE), build.entry)
        return discarded, equal, different

    def jals(self, build) -> list[int]:
        """Targets of the jal instructions in the unit's code."""
        words = struct.unpack(f"<{build.size // 4}I", build.code[: build.size])
        return [(D.TEST_ADDRESS & 0xF0000000) | (w & 0x03FFFFFF) << 2 for w in words if w >> 26 == 3]


def expect_refused(rig: Rig, text: str, what: str) -> str | None:
    try:
        rig.build(text)
    except D.InputError as exc:
        return None if what in str(exc) else f"refused for another reason: {exc}"
    except matchbuild.StepError as exc:
        return f"the build failed instead of being refused: {exc}"
    return "the unit was built"


# ---------------------------------------------------------------------------
# Cases


def case_baseline(rig: Rig):
    build = rig.build(SOURCE)
    result = rig.run(build)
    return None if result[2] == 0 and result[1] > 0 else f"the unedited function gives {result}"


def case_colliding_helper_runs(rig: Rig):
    # A global helper named like a linker symbol of the tree: the test must run it.
    helper = "void func_801e9080(Slab172 *p) { game_state.field_92 ^= 1; }\n"
    build = rig.build(unit(declarations=helper, before_end="    func_801e9080(0);\n"))
    discarded, equal, different = rig.run(build)
    if different == 0:
        return f"different 0 ({equal} equal): the original code ran in the helper's place"
    return None if rig.address not in rig.jals(build) and ORIGINAL_ADDRESS not in rig.jals(build) else "the call goes to the original address"


def case_colliding_helper_fail_closed(rig: Rig):
    helper = "void func_801e9080(Slab172 *p) { game_state.field_92 ^= 1; }\n"
    text = unit(declarations=helper, before_end="    func_801e9080(0);\n")
    saved = D.without_assignments
    D.without_assignments = lambda text, names: text
    try:
        return expect_refused(rig, text, "outside the unit")
    finally:
        D.without_assignments = saved


def case_self_call(rig: Rig):
    base = rig.run(rig.build(SOURCE))
    build = rig.build(unit(before_end=f"    if (object == 0) {NAME}(object + 1);\n"))
    result = rig.run(build)
    if result != base:
        return f"result {result} differs from the unedited function's {base}"
    targets = rig.jals(build)
    if D.TEST_ADDRESS + build.entry not in targets:
        return f"no jal to {D.TEST_ADDRESS + build.entry:#x}; jal targets {[hex(t) for t in targets]}"
    if rig.address in targets:
        return f"a jal goes to the original address {rig.address:#x}"
    return None


def case_helper_and_table(rig: Rig):
    declarations = ("const int nb_table[4] = {1, 2, 3, 4};\n"
                    "int nb_helper(int x) { return x + nb_table[x & 3]; }\n")
    text = unit(declarations=declarations, before_end="    if (object == 0) cnt = nb_helper(nb_table[cnt & 3]);\n")
    base = rig.run(rig.build(SOURCE))
    build = rig.build(text)
    if build.entry == 0 or build.entry % 4:
        return f"entry {build.entry}"
    if len(build.code) < build.size + 16:
        return f"code {len(build.code)} bytes, size {build.size}: the table is not after the code"
    if rig.run(build) != base:
        return f"result {rig.run(build)} differs from the unedited function's {base}"
    (rig.folder / f"{NAME}.c").write_text(rebased(text, rig.folder))
    out = io.StringIO()
    with contextlib.redirect_stdout(out):
        status = D.main(["--config", str(rig.config_path), "--folder", str(rig.folder), "--cases", "10", NAME])
    line = out.getvalue().splitlines()[0] if out.getvalue() else ""
    want = f"{NAME}: built {build.size} bytes, original {rig.original_size} bytes;"
    return None if status == 0 and line.startswith(want) else f"status {status}, line {line!r}, wanted it to start {want!r}"


def case_writable_data(rig: Rig):
    # Each kind is refused, and for its own reason: the first two by the tool's check, the third by the
    # build's check of sections that a unit may own (a StepError that names the section).
    kinds = (("initialized", "int nb_init = 5;\n", "nb_init", D.InputError, "writable data"),
             ("uninitialized", "int nb_zero;\n", "nb_zero", D.InputError, "writable data"),
             ("own section", 'int nb_sect __attribute__((section(".nbdata"))) = 3;\n', "nb_sect", matchbuild.StepError, ".nbdata"))
    for what, declaration, use, error, text in kinds:
        source = unit(declarations=declaration, before_end=f"    if (object == 0) cnt = {use};\n")
        try:
            rig.build(source)
        except error as exc:
            if text not in str(exc):
                return f"{what}: refused for another reason: {exc}"
            continue
        except (D.InputError, matchbuild.StepError) as exc:
            return f"{what}: refused by {type(exc).__name__}, wanted {error.__name__}: {exc}"
        return f"{what} data was accepted"
    return None


def declared_only_in_the_build(rig: Rig) -> list[str]:
    """Functions of protos.h taking no argument that the build declares and the symbol file does not assign."""
    known = D.addresses(rig.cfg)
    assigned = set(re.findall(r"^\s*([A-Za-z_]\w*)\s*=", Path(rig.cfg.symbols_path).read_text(), re.M))
    protos = (HERE.parent / "protos.h").read_text()
    return [n for n in re.findall(r"^void (func_[0-9a-f]{8})\(void\);", protos, re.M)
            if n in known and n not in assigned and n != NAME]


def assigned_in_the_symbol_file(rig: Rig) -> list[str]:
    known = D.addresses(rig.cfg)
    assigned = set(re.findall(r"^\s*([A-Za-z_]\w*)\s*=", Path(rig.cfg.symbols_path).read_text(), re.M))
    protos = (HERE.parent / "protos.h").read_text()
    return [n for n in re.findall(r"^void (func_[0-9a-f]{8})\(void\);", protos, re.M) if n in assigned and n in known and n != NAME]


def case_external_callee(rig: Rig):
    # One callee comes from the tree's symbol file, one only from the build's declarations (others.ld).
    from_file, from_build = assigned_in_the_symbol_file(rig), declared_only_in_the_build(rig)
    if not from_file or not from_build:
        return f"no suitable callee in protos.h: {len(from_file)} from the symbol file, {len(from_build)} from the build"
    callees = [from_file[0], from_build[0]]
    calls = "".join(f"    if (object == 0) {callee}();\n" for callee in callees)
    build = rig.build(unit(before_end=calls))
    if rig.run(build) != rig.run(rig.build(SOURCE)):
        return "the result differs from the unedited function's"
    targets = rig.jals(build)
    known = D.addresses(rig.cfg)
    for callee in callees:
        if known[callee] not in targets:
            return f"{callee} should be at {known[callee]:#x}; jal targets {[hex(t) for t in targets]}"
    return None


def case_helper_named_like_a_declared_function(rig: Rig):
    # A name that only the build's declarations know (no line in the symbol file): the unit's helper must run.
    name = declared_only_in_the_build(rig)[0]
    build = rig.build(unit(declarations=f"void {name}(void) {{ game_state.field_92 ^= 1; }}\n", before_end=f"    {name}();\n"))
    discarded, equal, different = rig.run(build)
    known = D.addresses(rig.cfg)
    if different == 0:
        return f"different 0 ({equal} equal): the original code ran in the helper's place"
    return None if known[name] not in rig.jals(build) else f"the call goes to the original address {known[name]:#x}"


def case_another_run_is_left_alone(rig: Rig):
    """A second run starts and ends while a first one is at work: the first one's folder and files stay."""
    first = make_workspace()
    try:
        (first / "in-use.c").write_text("/* a source of a run that is still at work */\n")
        second = make_workspace()          # what a run does when it starts
        if second == first or second.parent != first.parent:
            return f"the second run got {second}, the first has {first}"
        if not (first / "in-use.c").is_file() or not rig.folder.is_dir():
            return "starting a run removed another run's folder"
        (second / "x.c").write_text("\n")
        remove_workspace(second)           # what a run does when it ends
        if second.exists():
            return "a run left its own folder behind"
        if not (first / "in-use.c").is_file() or not rig.folder.is_dir():
            return "ending a run removed another run's folder"
    finally:
        remove_workspace(first)
    return None if not first.exists() else "the first run's folder could not be removed by its owner"


def case_workspace_is_ignored(rig: Rig):
    """The throwaway sources lie where the repository ignores them, and not under ps1/src."""
    if HERE.parent in rig.folder.parents:
        return f"the workspace {rig.folder} lies in the source tree"
    probe = rig.folder / f"{NAME}.c"
    if not probe.is_file():
        return "the workspace holds no unit to ask about"
    try:
        asked = subprocess.run(["git", "-C", str(HERE), "check-ignore", "-q", str(probe)], timeout=60)
    except (OSError, subprocess.TimeoutExpired) as exc:
        return f"cannot ask git: {exc}"
    if asked.returncode != 0:
        return f"git does not ignore {probe} (status {asked.returncode})"
    listed = subprocess.run(["git", "-C", str(HERE), "status", "--porcelain", "--", str(rig.folder)],
                            capture_output=True, text=True, timeout=60)
    return None if not listed.stdout.strip() else f"git lists the workspace: {listed.stdout.strip()}"


CASES = [
    ("m-unedited-function-passes", case_baseline),
    ("m-helper-named-like-a-linker-symbol-runs", case_colliding_helper_runs),
    ("m-without-the-filter-the-build-is-refused", case_colliding_helper_fail_closed),
    ("m-self-call-goes-to-the-unit", case_self_call),
    ("m-helper-before-the-entry-and-constant-table", case_helper_and_table),
    ("m-three-kinds-of-writable-data-are-refused", case_writable_data),
    ("m-helper-named-like-a-declared-function-runs", case_helper_named_like_a_declared_function),
    ("m-external-callees-keep-their-original-address", case_external_callee),
    ("w-another-run-is-left-alone", case_another_run_is_left_alone),
    ("w-workspace-is-ignored-by-git", case_workspace_is_ignored),
]


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Controls of difftest.py that need the toolchain")
    parser.add_argument("--config", type=Path, required=True, help="the build configuration (build.toml)")
    args = parser.parse_args(argv)

    try:
        cfg = matchbuild.load_config(args.config.resolve())
    except matchbuild.ConfigError as exc:
        print(f"cannot run: configuration problem: {'; '.join(exc.errors)}", file=sys.stderr)
        return 2
    except (D.InputError, OSError) as exc:
        print(f"cannot run: {exc}", file=sys.stderr)
        return 2

    folder = make_workspace()
    signal.signal(signal.SIGTERM, lambda *_: sys.exit(143))  # run the cleanup below
    failed = 0
    try:
        rig = Rig(cfg, folder)
        rig.config_path = args.config.resolve()
        try:
            rig.build(SOURCE)
        except (matchbuild.StepError, matchbuild.EnvironmentFailure, D.InputError, OSError) as exc:
            print(f"cannot run: the toolchain or the private inputs are missing: {exc}", file=sys.stderr)
            return 2
        for name, case in CASES:
            try:
                why = case(rig)
            except Exception as exc:  # a case that raises is a case that fails
                why = f"raised {type(exc).__name__}: {exc}"
            if why:
                failed += 1
                print(f"FAIL {name}: {why}")
            else:
                print(f"ok {name}")
    finally:
        remove_workspace(folder)
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
