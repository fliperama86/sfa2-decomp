#!/usr/bin/env python3
"""Controls for build.sh (the repository's one build command), with stand-ins for everything it runs.

    python3 test_build_sh.py

No compiler, no network, no disc, no game program: a few seconds. Each case copies build.sh into a made-up tree whose
`port/tools/psyzbuild.py`, `hostbuild.py` and `macbuild.py` are stand-ins that write their name and arguments to a log, make the
file the real tool would leave (the library's `psyz.json`, the program) and end with status 3 when the case tells them to. The
"program" they leave writes its path (from the folder it was started in) and its arguments to the same log and ends with status 7. PATH holds only stand-ins for the compilers,
for `wslpath` (it prints `W:` and its argument) and for `git submodule update` (logged, and it makes the folder), and the few
system programs the script uses; the made-up PsyZ folder is a real git work tree of its own, as the submodule is.

The cases show: a wrong command line, a program option without -r and -r without a disc end with status 2 before any tool
runs; the tools run in the documented order with the documented arguments, for both systems; a tool that fails ends the script
with its status and nothing after it runs, the program included; the library step is skipped on a second run and is run again
when the PsyZ commit, the patch, the tool or the compiler's version line changed, when the PsyZ tree has a change, when
`psyz.json` is gone, and after a failed run; a PsyZ folder that is no git work tree of its own is built every time; the
program step is skipped on a second run in a git work tree with no change under `ps1/` and `port/`, and is run again when a
file there changed or is new (at every run until it is committed, and once after the commit), when the script, the library's
inputs or the compiler changed, when the program is gone, and after a failed run, and always where the tree is no git work
tree; a change elsewhere in the tree does not run it; the submodule is fetched only when the folder is empty; and the program
gets the documented command line, with paths in Windows
form only for the Windows program, only for a file that exists and only where `wslpath` is. Every case prints `ok NAME` or
`FAIL NAME: why`.
"""

from __future__ import annotations

import json
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
SCRIPT = REPO / "build.sh"
WIN_LIB = "port/build/psyz-i686-w64-mingw32"
MAC_LIB = "port/build/psyz-mac"
WIN = [["psyzbuild", "--cc", "i686-w64-mingw32-gcc", "--build", WIN_LIB], ["hostbuild", "--psyz", WIN_LIB]]
MAC = [["psyzbuild", "--cc", "clang", "--build", MAC_LIB], ["hostbuild", "--guest", "--psyz", MAC_LIB], ["macbuild"]]
DISC = "E:\\a folder\\disc.cue"

TOOL = """import json, os, sys
from pathlib import Path
name, argv = Path(sys.argv[0]).stem, sys.argv[1:]
with open(os.environ["STUB_LOG"], "a") as log:
    log.write(json.dumps([name, *argv]) + "\\n")
if os.environ.get("STUB_FAIL") == name:
    sys.exit(3)
def program(path):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("#!" + sys.executable + "\\nimport json, os, sys\\nopen(os.environ['STUB_LOG'], 'a').write(json.dumps(['program', os.path.relpath(sys.argv[0]), *sys.argv[1:]]) + '\\\\n')\\nsys.exit(7)\\n")
    path.chmod(0o755)
if name == "psyzbuild":
    build = Path(argv[argv.index("--build") + 1])
    build.mkdir(parents=True, exist_ok=True)
    (build / "psyz.json").write_text("{}")
elif name == "hostbuild" and "--guest" not in argv:
    program("port/build/host/sfa2.exe")
elif name == "macbuild":
    program("port/build/mac/sfa2-mac")
"""
GIT = """#!{python}
import json, os, sys
from pathlib import Path
argv = sys.argv[1:]
if "submodule" in argv and "update" in argv:
    with open(os.environ["STUB_LOG"], "a") as log:
        log.write(json.dumps(["git", *argv]) + "\\n")
    if argv[0] != "-C":
        Path(argv[-1], "psyz").mkdir(parents=True)
    sys.exit(0)
os.execv({git!r}, [{git!r}, *argv])
"""
REAL_GIT = shutil.which("git")
IDENTITY = ["-c", "user.name=test", "-c", "user.email=test@example.invalid"]


def script(path: Path, text: str) -> None:
    path.write_text(text)
    path.chmod(0o755)


class Tree:
    """One made-up checkout. `run` gives (status, the log's lines as lists, stderr). With `repo` the tree is a git work tree
    with one commit that holds everything but what the real repository ignores, and PsyZ's folder as the submodule's entry."""

    def __init__(self, root: Path, psyz: str = "git", wslpath: bool = True, repo: bool = False):
        self.root = root
        root.mkdir()
        shutil.copy(SCRIPT, root / "build.sh")
        for name in ("psyzbuild", "hostbuild", "macbuild"):
            self.write(f"port/tools/{name}.py", TOOL)
        self.write("port/psyz.patch", "a patch\n")
        self.write("port/src/video.h", "/* a policy */\n")
        self.write("port/settings.ini", "[experimental]\n")
        self.psyz = root / "port/external/psyz"
        if psyz != "missing":
            self.write("port/external/psyz/psyz/a.c", "int a;\n")
        if psyz == "git":
            self.git("init", "-q")
            self.commit("first")
        bin_dir = root / "bin"
        bin_dir.mkdir()
        for name in ("i686-w64-mingw32-gcc", "clang"):
            script(bin_dir / name, '#!/bin/sh\necho "$STUB_CC"\necho more\n')
        if wslpath:
            script(bin_dir / "wslpath", '#!/bin/sh\n[ "$1" = -w ] && echo "W:$2"\n')
        script(bin_dir / "git", GIT.format(python=sys.executable, git=REAL_GIT))
        for name in ("dirname", "head", "cat", "rm", "grep"):
            (bin_dir / name).symlink_to(shutil.which(name))
        self.log = root / "calls.log"
        self.env = {"PATH": str(bin_dir), "HOME": str(root), "GIT_CONFIG_NOSYSTEM": "1", "STUB_LOG": str(self.log), "STUB_CC": "cc 1.0",
                    "SFA2_PYTHON": sys.executable}
        if repo:
            self.write("ps1/src/a.c", "int a;\n")
            self.write(".gitignore", "build/\n/local/\n/bin/\n/calls.log\n")
            self.git("init", "-q", at=root)
            self.commit("first", at=root)

    def write(self, rel: str, text: str) -> Path:
        path = self.root / rel
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text)
        return path

    def git(self, *argv: str, at: Path | None = None) -> None:
        subprocess.run([REAL_GIT, "-C", str(at or self.psyz), *IDENTITY, *argv], check=True, capture_output=True)

    def commit(self, message: str, at: Path | None = None) -> None:
        self.git("add", "-A", at=at)
        self.git("commit", "-q", "-m", message, at=at)

    def run(self, *argv: str, cwd: Path | None = None, **env: str):
        self.log.unlink(missing_ok=True)
        proc = subprocess.run(["/bin/sh", str(self.root / "build.sh"), *argv], cwd=cwd or self.root, env={**self.env, **env}, capture_output=True,
                              text=True, timeout=60)
        calls = [json.loads(line) for line in self.log.read_text().splitlines()] if self.log.exists() else []
        return proc.returncode, calls, proc.stderr


def same(got, want, what: str = "") -> None:
    assert got == want, f"{what + ': ' if what else ''}got {got!r}, want {want!r}"


def case_a_wrong_command_line_runs_nothing(root: Path):
    tree = Tree(root)
    for argv in ([], ["linux"], ["-x", "win"], ["-r"], ["win", "--no-audio"], ["mac", "x"]):
        status, calls, err = tree.run(*argv, SFA2_DISC=DISC)
        same((status, calls), (2, []), " ".join(argv) or "no argument")
        assert "usage: ./build.sh [-r] win|mac" in err, err
    status, calls, err = tree.run("win", "--no-audio")
    assert "--no-audio: a program option needs -r" in err, err


def case_r_without_a_disc_builds_nothing(root: Path):
    tree = Tree(root)
    status, calls, err = tree.run("-r", "win")
    same((status, calls), (2, []))
    assert "-r needs SFA2_DISC" in err, err
    same(tree.run("-r", "mac", SFA2_DISC="")[:2], (2, []), "an empty SFA2_DISC")


def case_the_windows_steps_in_order(root: Path):
    tree = Tree(root)
    same(tree.run("win")[:2], (0, WIN))
    assert (tree.root / (WIN_LIB + ".key")).is_file(), "no key file beside the library's folder"


def case_the_mac_steps_in_order(root: Path):
    tree = Tree(root)
    same(tree.run("mac")[:2], (0, MAC))
    same(tree.run("mac")[:2], (0, MAC[1:]), "second run")


def case_from_another_folder(root: Path):
    tree = Tree(root)
    other = root / "elsewhere"
    other.mkdir()
    same(tree.run("win", cwd=other)[:2], (0, WIN))
    assert not (other / "port").exists(), "something was written in the folder the script was started from"


def case_a_failed_tool_ends_the_script(root: Path):
    tree = Tree(root)
    same(tree.run("-r", "win", SFA2_DISC=DISC, STUB_FAIL="psyzbuild")[:2], (3, WIN[:1]), "the library fails")
    assert not (tree.root / (WIN_LIB + ".key")).exists(), "a key file after a failed library step"
    same(tree.run("-r", "win", SFA2_DISC=DISC, STUB_FAIL="hostbuild")[:2], (3, WIN), "the program's build fails")
    same(tree.run("-r", "mac", SFA2_DISC=DISC, STUB_FAIL="hostbuild")[:2], (3, MAC[:2]), "the guest image fails")
    same(tree.run("-r", "mac", SFA2_DISC=DISC, STUB_FAIL="macbuild")[:2], (3, MAC[1:]), "the Mac program's build fails")


def case_the_library_is_skipped_when_nothing_changed(root: Path):
    tree = Tree(root)
    same(tree.run("win")[:2], (0, WIN), "first run")
    same(tree.run("win")[:2], (0, WIN[1:]), "second run")
    same(tree.run("win")[:2], (0, WIN[1:]), "third run")
    same(tree.run("mac")[:2], (0, MAC), "the other system has a library of its own")
    same(tree.run("win")[:2], (0, WIN[1:]), "and leaves this one's alone")


def case_the_library_is_built_again_when_an_input_changed(root: Path):
    tree = Tree(root)
    tree.run("win")
    tree.write("port/psyz.patch", "another patch\n")
    same(tree.run("win")[:2], (0, WIN), "the patch changed")
    same(tree.run("win")[:2], (0, WIN[1:]), "after the patch")
    tree.write("port/tools/psyzbuild.py", TOOL + "\n")
    same(tree.run("win")[:2], (0, WIN), "the tool changed")
    tree.write("port/src/video.h", "/* another policy */\n")
    same(tree.run("win")[:2], (0, WIN), "the policy header that the library holds a copy of changed")
    same(tree.run("win")[:2], (0, WIN[1:]), "after the policy header")
    same(tree.run("win", STUB_CC="cc 2.0")[:2], (0, WIN), "the compiler's version line changed")
    same(tree.run("win", STUB_CC="cc 2.0")[:2], (0, WIN[1:]), "after the compiler")
    tree.write("port/external/psyz/psyz/a.c", "int b;\n")
    same(tree.run("win", STUB_CC="cc 2.0")[:2], (0, WIN), "the PsyZ tree has a change")
    same(tree.run("win", STUB_CC="cc 2.0")[:2], (0, WIN), "it still has")
    tree.commit("second")
    same(tree.run("win", STUB_CC="cc 2.0")[:2], (0, WIN), "another PsyZ commit")
    same(tree.run("win", STUB_CC="cc 2.0")[:2], (0, WIN[1:]), "after the commit")
    tree.write("port/external/psyz/new.txt", "x\n")
    same(tree.run("win", STUB_CC="cc 2.0")[:2], (0, WIN), "a file git does not know in the PsyZ tree")
    (tree.psyz / "new.txt").unlink()
    (tree.root / WIN_LIB / "psyz.json").unlink()
    same(tree.run("win", STUB_CC="cc 2.0")[:2], (0, WIN), "psyz.json is gone")


def case_a_failed_library_step_is_not_remembered(root: Path):
    tree = Tree(root)
    tree.run("win")
    tree.write("port/psyz.patch", "another patch\n")
    same(tree.run("win", STUB_FAIL="psyzbuild")[:2], (3, WIN[:1]), "the changed patch fails")
    tree.write("port/psyz.patch", "a patch\n")
    same(tree.run("win")[:2], (0, WIN), "the first patch again: the folder may hold half of the failed build")


def case_a_folder_that_is_no_git_tree_is_built_every_time(root: Path):
    tree = Tree(root, psyz="plain")
    for n in (1, 2):
        same(tree.run("win")[:2], (0, WIN), f"run {n}")
    assert not (tree.root / (WIN_LIB + ".key")).exists(), "a key file for a source whose state cannot be told"


def case_the_program_step_is_skipped_when_nothing_changed(root: Path):
    tree = Tree(root, repo=True)
    same(tree.run("win")[:2], (0, WIN), "first run")
    assert (tree.root / "port/build/host.key").is_file(), "no key file beside the program's folder"
    same(tree.run("win")[:2], (0, []), "second run")
    program = ["program", "port/build/host/sfa2.exe", "--skip-programs", "--settings", "W:port/settings.ini", DISC]
    same(tree.run("-r", "win", SFA2_DISC=DISC)[:2], (7, [program]), "the program is started after a skipped build")
    tree.write("docs/a.md", "text\n")
    tree.write("local/build.env", "")
    same(tree.run("win")[:2], (0, []), "a new file outside ps1/ and port/")
    same(tree.run("mac")[:2], (0, MAC), "the other system has a program of its own")
    same(tree.run("mac")[:2], (0, []), "the other system, second run")
    same(tree.run("win")[:2], (0, []), "and leaves this one's alone")


def case_the_program_step_runs_again_when_an_input_changed(root: Path):
    tree = Tree(root, repo=True)
    tree.run("win")
    for rel in ("ps1/src/a.c", "port/tools/hostbuild.py", "port/settings.ini"):
        tree.write(rel, (tree.root / rel).read_text() + "\n")
        same(tree.run("win")[:2], (0, WIN[1:]), f"{rel} changed")
        same(tree.run("win")[:2], (0, WIN[1:]), f"{rel} is still changed")
        tree.commit(rel, at=tree.root)
        same(tree.run("win")[:2], (0, WIN[1:]), f"{rel} committed: another tree")
        same(tree.run("win")[:2], (0, []), f"after {rel}")
    for rel in ("ps1/src/new.c", "port/src/new.c"):
        new = tree.write(rel, "int n;\n")
        same(tree.run("win")[:2], (0, WIN[1:]), f"{rel} is new")
        new.unlink()
        same(tree.run("win")[:2], (0, WIN[1:]), f"{rel} is gone again: the program on disk was built with it")
        same(tree.run("win")[:2], (0, []), f"after {rel}")
    script = tree.root / "build.sh"
    script.write_text(script.read_text() + "\n")
    same(tree.run("win")[:2], (0, WIN[1:]), "the script changed")
    same(tree.run("win")[:2], (0, []), "after the script")
    same(tree.run("win", STUB_CC="cc 2.0")[:2], (0, WIN), "the compiler's version line changed: the library and the program")
    same(tree.run("win", STUB_CC="cc 2.0")[:2], (0, []), "after the compiler")
    tree.write("port/external/psyz/psyz/a.c", "int b;\n")
    same(tree.run("win", STUB_CC="cc 2.0")[:2], (0, WIN), "the PsyZ tree has a change")
    tree.commit("second")
    same(tree.run("win", STUB_CC="cc 2.0")[:2], (0, WIN), "PsyZ is at a commit that the tree does not pin")
    same(tree.run("win", STUB_CC="cc 2.0")[:2], (0, WIN[1:]), "and stays there: the library is built, the program is not trusted")
    tree.commit("the pin moves", at=tree.root)
    same(tree.run("win", STUB_CC="cc 2.0")[:2], (0, WIN[1:]), "the pin is committed: another tree")
    same(tree.run("win", STUB_CC="cc 2.0")[:2], (0, []), "after the pin")
    (tree.root / "port/build/host/sfa2.exe").unlink()
    same(tree.run("win", STUB_CC="cc 2.0")[:2], (0, WIN[1:]), "the program is gone")


def case_a_failed_program_step_is_not_remembered(root: Path):
    tree = Tree(root, repo=True)
    tree.run("win")
    tree.write("ps1/src/a.c", "int b;\n")
    tree.commit("b", at=tree.root)
    same(tree.run("win", STUB_FAIL="hostbuild")[:2], (3, WIN[1:]), "the changed tree fails")
    assert not (tree.root / "port/build/host.key").exists(), "a key file after a failed program step"
    tree.git("reset", "-q", "--hard", "HEAD~1", at=tree.root)
    same(tree.run("win")[:2], (0, WIN[1:]), "the first tree again: the folder may hold half of the failed build")
    tree.run("mac")
    tree.write("ps1/src/a.c", "int c;\n")
    tree.commit("c", at=tree.root)
    same(tree.run("mac", STUB_FAIL="macbuild")[:2], (3, MAC[1:]), "the Mac program's build fails after the guest image")
    same(tree.run("mac")[:2], (0, MAC[1:]), "both again")
    same(tree.run("mac")[:2], (0, []), "then neither")


def case_a_tree_that_is_no_git_tree_builds_the_program_every_time(root: Path):
    tree = Tree(root)
    for n in (1, 2, 3):
        same(tree.run("win")[:2], (0, WIN if n == 1 else WIN[1:]), f"run {n}")
    assert not (tree.root / "port/build/host.key").exists(), "a key file for a tree whose state cannot be told"


def case_the_submodule_is_fetched_only_when_missing(root: Path):
    tree = Tree(root, psyz="missing")
    fetch = [["git", "submodule", "update", "--init", "port/external/psyz"], ["git", "-C", "port/external/psyz", "submodule", "update", "--init", "external/SDL"]]
    same(tree.run("win")[:2], (0, fetch + WIN), "first run")
    same(tree.run("win")[:2], (0, WIN), "second run: the folder is there")


def case_the_program_s_command_line(root: Path):
    tree = Tree(root)
    exe, mac = "port/build/host/sfa2.exe", "port/build/mac/sfa2-mac"
    same(tree.run("-r", "win", SFA2_DISC=DISC)[:2], (7, WIN + [["program", exe, "--skip-programs", "--settings", "W:port/settings.ini", DISC]]), "no option")
    same(tree.run("-r", "win", "--no-audio", "--volume", "50", SFA2_DISC=DISC)[1][-1],
         ["program", exe, "--skip-programs", "--settings", "W:port/settings.ini", "--no-audio", "--volume", "50", DISC], "options")
    same(tree.run("win", "-r", SFA2_DISC=DISC)[:2], (2, []), "-r after the system is a program option without -r")
    same(tree.run("-r", "win", "--settings", "my file.ini", SFA2_DISC=DISC)[1][-1], ["program", exe, "--skip-programs", "--settings", "my file.ini", DISC],
         "a --settings of the caller's")
    mine = tree.write("local/my settings.ini", "")
    disc = tree.write("local/a disc.cue", "")
    same(tree.run("-r", "win", SFA2_DISC=str(disc), SFA2_SETTINGS="local/my settings.ini")[1][-1],
         ["program", exe, "--skip-programs", "--settings", "W:local/my settings.ini", f"W:{disc}"], "files that exist")
    same(tree.run("-r", "win", SFA2_DISC=DISC, SFA2_SETTINGS="no such.ini")[1][-1], ["program", exe, "--skip-programs", "--settings", "no such.ini", DISC],
         "a settings file that does not exist is handed on as it is")
    same(tree.run("-r", "mac", "--no-audio", SFA2_DISC=str(disc), SFA2_SETTINGS=str(mine))[1][-1],
         ["program", mac, "--skip-programs", "--settings", str(mine), "--no-audio", str(disc)], "the Mac program takes paths as they are")


def case_without_wslpath_paths_stay(root: Path):
    tree = Tree(root, wslpath=False)
    disc = tree.write("local/a disc.cue", "")
    same(tree.run("-r", "win", SFA2_DISC=str(disc))[1][-1], ["program", "port/build/host/sfa2.exe", "--skip-programs", "--settings", "port/settings.ini", str(disc)])


def case_the_local_file_is_read(root: Path):
    tree = Tree(root)
    tree.write("local/build.env", "SFA2_DISC='E:\\a folder\\disc.cue'\nSTUB_CC='cc from the file'\nexport STUB_CC\n")
    same(tree.run("-r", "win")[1][-1][-1], DISC, "the disc of the file")
    same((tree.root / (WIN_LIB + ".key")).read_text().splitlines()[-1], "compiler cc from the file", "the file's environment reaches the tools")


CASES = [
    case_a_wrong_command_line_runs_nothing, case_r_without_a_disc_builds_nothing, case_the_windows_steps_in_order, case_the_mac_steps_in_order,
    case_from_another_folder, case_a_failed_tool_ends_the_script, case_the_library_is_skipped_when_nothing_changed,
    case_the_library_is_built_again_when_an_input_changed, case_a_failed_library_step_is_not_remembered,
    case_a_folder_that_is_no_git_tree_is_built_every_time, case_the_program_step_is_skipped_when_nothing_changed,
    case_the_program_step_runs_again_when_an_input_changed, case_a_failed_program_step_is_not_remembered,
    case_a_tree_that_is_no_git_tree_builds_the_program_every_time, case_the_submodule_is_fetched_only_when_missing, case_the_program_s_command_line,
    case_without_wslpath_paths_stay, case_the_local_file_is_read,
]


def main() -> int:
    failed = 0
    with tempfile.TemporaryDirectory() as tmp:
        for fn in CASES:
            name = fn.__name__.replace("case_", "").replace("_", "-")
            try:
                fn(Path(tmp) / name)
            except Exception as err:
                failed += 1
                print(f"FAIL {name}: {type(err).__name__}: {err}")
            else:
                print(f"ok   {name}")
    ran = len(CASES)
    print(f"{failed} case(s) behaved wrongly" if failed else f"all {ran} cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
