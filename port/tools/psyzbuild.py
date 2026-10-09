#!/usr/bin/env python3
"""Build PsyZ and its SDL for the port's target, from a patched copy.

PsyZ (port/external/psyz, a submodule at a pinned commit) is the library that
draws the picture for the port (`port/src/gpu.c`). This tool builds it as a
static library for the machine the port runs on, and prints what a program
that links it needs. It never writes into the submodule's folder: it copies
the three folders that the build reads (`psyz`, `decomp`, `external/SDL`) to
`BUILD/src`, applies `port/psyz.patch` to the copy, configures with CMake and
builds with Ninja in `BUILD/obj`. Python 3.11 or later, standard library
only; CMake (3.21 or later) and Ninja are found on PATH, else in
`.venv/bin` of this checkout, or are given with `--cmake` and `--ninja`.

The patch
---------

`port/psyz.patch` is a unified diff of a few lines written by this project;
the text before its first `---` line says what it fixes and for which PsyZ
commit. This tool applies it with its own reader (no `patch` program is
needed) under one rule: for every hunk, the lines the hunk expects to find
(its context and its removed lines, exactly, blanks included) must occur in
the file at exactly ONE place. No place, or more than one, is a refusal that
names the file and the hunk; the copy is then left unpatched and nothing is
built. So a PsyZ whose source differs at the patched place is refused, and a
patch can never land somewhere other than where it was written for. A file
that the patch names and the copy lacks is a refusal too. A hunk's body has
exactly the old and new line counts of its header: after the counts are met
only blank lines, the next hunk header, the next file header or the end of
the text may follow, and a body that ends early or a line beyond the counts is
a refusal that names the hunk.

Every file the patch names must lie inside the copy: a name that is absolute,
that has a `..` component, or that resolves (symbolic links followed, the
file itself too) to a place outside the copy is a refusal that names it.
All names are checked and all hunks are matched before any file is written, so
a refused patch changes nothing, in the first file as in the last. A file
named twice is a refusal.

Paths
-----

The tool deletes and writes only these, all under the build folder BUILD:
`BUILD/src` (deleted and made again at each run), `BUILD/obj` (CMake and Ninja
write it), `BUILD/toolchain.cmake`, `BUILD/psyz.json` and `BUILD/build.log`.
Nothing outside BUILD is ever written, and the PsyZ source is only read. Before
anything is created, deleted or written, the PsyZ source, the build folder and
the patch file are resolved (symbolic links followed; for a path that does not
exist yet, its nearest existing parent is resolved and the rest appended), and
the run is refused (status 2, a message naming both paths) when, for the
resolved paths, the build folder is the source or lies inside it, or the source
lies inside the build folder, or the patch file is one of the places listed
above or lies inside `BUILD/src` or `BUILD/obj`, or one of the listed places
already exists as a symbolic link. A link that aliases one folder to another is
therefore caught as the overlap it is. Symbolic links inside the three copied
folders of the source are refused (a link in the copy could lead a write back
into the source); a source tree that has links needs them replaced by files
first. The copy is made only after all of this, so a refused run leaves the
source and the build folder as they were.

The build
---------

The target is named by the compiler: `i686-w64-mingw32-gcc` builds for
Windows 32-bit with a generated CMake toolchain file (`BUILD/toolchain.cmake`:
the compiler, its C++ driver, `ar`, `ranlib` and `windres` next to it, found
by name; the search paths of the compiler's own tree). Any other compiler
builds for the machine it runs on. The default build folder is
`port/build/psyz-TRIPLET` (TRIPLET is the compiler's name without `-gcc`).
Release, `GTE_USE_HW_SQRT=ON`, renderer `sdl3_gpu` (SDL_GPU: Direct3D 12 on
Windows, Vulkan on Linux, Metal on macOS). SDL is built from the copy as a
static library, PsyZ's own CMake does that. A wrapper project in
`BUILD/src/CMakeLists.txt` adds PsyZ and one probe program that links it;
the libraries the probe's link line names (found in `build.ninja`) are what a
consumer needs, so the list comes from CMake and is not written by hand.
Copies keep the source files' times, so a second run rebuilds only what the
patch or the source changed.

Output
------

Standard output, in this order, nothing else:

    psyz: COMMIT              the commit of the PsyZ source (`unknown` without git, and when
                              the folder is not the top of a git work tree of its own)
    patch: NAME applied at N place(s)
    library: PATH             libpsyz.a
    include: DIR              the copy's psyz/include (use with -D__psyz)
    link: PATH PATH -lNAME ...  the library files and system libraries, in link order

and `BUILD/psyz.json`, which `hostbuild.py --psyz BUILD` reads:
`{"include": DIR, "define": ["__psyz"], "link": [...], "commit": ...}`.
The line `note: the patch was written for COMMIT` precedes `library:` when git
can tell that the source is at another commit and the patch still applied.

Exit status: 0 built; 1 the build (configure, compile or link) failed, with
the last lines of its log on standard error and the whole log in
`BUILD/build.log`; 2 an input is missing, the patch is refused, a tool
(compiler, CMake, Ninja) is missing; one line on standard error says which.

usage:
  psyzbuild.py [--psyz DIR] [--patch FILE] [--cc CC] [--build DIR]
               [--cmake CMAKE] [--ninja NINJA] [--timeout SECONDS]

The defaults: `port/external/psyz`, `port/psyz.patch`,
`i686-w64-mingw32-gcc`, `port/build/psyz-TRIPLET`, 1800 seconds for each of
the configure and build steps.
"""

from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path

PORT = Path(__file__).resolve().parents[1]
REPO = PORT.parent
COPIED = ("psyz", "decomp", "external/SDL")
PROBE = "int main(void) { return 0; }\n"
WRAPPER = """cmake_minimum_required(VERSION 3.21)
project(portpsyz C CXX)
add_subdirectory(psyz)
add_executable(probe probe.c)
target_link_libraries(probe PRIVATE psyz)
"""


class Problem(Exception):
    """An input is wrong or a tool is missing (exit status 2)."""


class Failure(Exception):
    """The build itself failed (exit status 1)."""


# The patch.


@dataclass
class Hunk:
    old: list[str]
    new: list[str]
    header: str


@dataclass
class FilePatch:
    path: str
    hunks: list[Hunk]


HUNK = re.compile(r"^@@ -(\d+)(?:,(\d+))? \+(\d+)(?:,(\d+))? @@")


def parse_patch(text: str) -> list[FilePatch]:
    """The files and hunks of a unified diff; text before the first `---` line is ignored.
    A hunk body has exactly the counts of its header; anything else after it is refused."""
    lines = text.split("\n")
    files: list[FilePatch] = []
    i = 0
    while i < len(lines) and not lines[i].startswith("--- "):
        i += 1
    while i < len(lines):
        if lines[i].startswith("--- "):
            if i + 1 >= len(lines) or not lines[i + 1].startswith("+++ "):
                raise Problem("the patch has a `---` line without a `+++` line after it")
            name = lines[i + 1][4:].split("\t")[0].strip()
            if name.startswith("b/"):
                name = name[2:]
            files.append(FilePatch(name, []))
            i += 2
            continue
        match = HUNK.match(lines[i])
        if match and files:
            old_n = int(match.group(2)) if match.group(2) is not None else 1
            new_n = int(match.group(4)) if match.group(4) is not None else 1
            header = lines[i]
            old: list[str] = []
            new: list[str] = []
            i += 1
            while (len(old) < old_n or len(new) < new_n) and i < len(lines):
                line = lines[i]
                if line.startswith("\\"):
                    i += 1
                    continue
                if line == "" and i == len(lines) - 1:
                    break  # the terminating empty string of the text, not a context line
                if line.startswith("--- ") and i + 2 < len(lines) and lines[i + 1].startswith("+++ ") and HUNK.match(lines[i + 2]):
                    break  # the next file's header arrives before the counts are met
                tag, body = line[:1], line[1:]
                if tag == " " or (tag == "" and line == ""):
                    old.append(body)
                    new.append(body)
                elif tag == "-":
                    old.append(body)
                elif tag == "+":
                    new.append(body)
                else:
                    break
                i += 1
            if len(old) != old_n or len(new) != new_n:
                raise Problem(f"the patch hunk {header} has {len(old)} old and {len(new)} new lines, not {old_n} and {new_n}")
            files[-1].hunks.append(Hunk(old, new, header))
            continue
        if lines[i] == "" or lines[i].startswith("\\"):
            i += 1
            continue
        raise Problem(f"the patch has a surplus line after the hunk {files[-1].hunks[-1].header if files[-1].hunks else files[-1].path}: {lines[i]!r}")
    if not files or not all(f.hunks for f in files):
        raise Problem("the patch has no hunk")
    return files


def find_places(lines: list[str], block: list[str]) -> list[int]:
    n = len(block)
    return [k for k in range(len(lines) - n + 1) if lines[k:k + n] == block]


def apply_patch(text: str, root: Path) -> list[str]:
    """Apply the patch under `root`. Every hunk must match at exactly one place of its file,
    or nothing is written and a Problem names the file and the hunk. Returns one
    description per hunk applied."""
    done: list[str] = []
    staged: dict[Path, str] = {}
    patches = parse_patch(text)
    root_real = real_path(root)
    seen: set[Path] = set()
    for fp in patches:
        parts = re.split(r"[/\\]", fp.path)
        if fp.path.startswith(("/", "\\")) or re.match(r"^[A-Za-z]:", fp.path):
            raise Problem(f"the patch names {fp.path}, an absolute path; names must lie inside the copy")
        if ".." in parts:
            raise Problem(f"the patch names {fp.path}, which has a `..` component; names must lie inside the copy")
        target = real_path(root / fp.path)
        if not inside(target, root_real):
            raise Problem(f"the patch names {fp.path}, which resolves outside the copy (to {target})")
        if target in seen:
            raise Problem(f"the patch names {fp.path} twice (or two names for one file)")
        seen.add(target)
    for fp in patches:
        path = root / fp.path
        if not path.is_file():
            raise Problem(f"the patch names {fp.path} and the PsyZ source has no such file")
        body = path.read_text()
        lines = body.split("\n")
        edits: list[tuple[int, int, list[str]]] = []
        for hunk in fp.hunks:
            places = find_places(lines, hunk.old)
            if len(places) != 1:
                what = "no place" if not places else f"{len(places)} places"
                raise Problem(f"the patch hunk {hunk.header} of {fp.path} fits {what} in the PsyZ source, and must fit exactly one")
            edits.append((places[0], len(hunk.old), hunk.new))
            done.append(f"{fp.path} line {places[0] + 1}")
        edits.sort()
        for (a, la, _), (b, _, _) in zip(edits, edits[1:]):
            if a + la > b:
                raise Problem(f"two hunks of the patch overlap in {fp.path}")
        for start, length, new in reversed(edits):
            lines[start:start + length] = new
        staged[path] = "\n".join(lines)
    for path, content in staged.items():
        path.write_text(content)
    return done


def patch_commit(text: str) -> str | None:
    """The commit the patch's header says it was written for (the first 40-digit hex number before the diff)."""
    head = text.split("\n--- ", 1)[0]
    match = re.search(r"\b([0-9a-f]{40})\b", head)
    return match.group(1) if match else None


# The build.


def triplet_of(cc: str) -> str:
    name = Path(cc).name
    return name[:-4] if name.endswith("-gcc") else name


def which_tool(name: str, given: str | None) -> str:
    if given:
        found = shutil.which(given)
        if not found:
            raise Problem(f"{given} was named for {name} and is not an executable")
        return found
    found = shutil.which(name) or (str(REPO / ".venv" / "bin" / name) if (REPO / ".venv" / "bin" / name).is_file() else None)
    if not found:
        raise Problem(f"{name} is not on PATH and not in .venv/bin of this checkout; install it or name it with --{name}")
    return found


def toolchain_text(cc: str) -> str:
    """CMake toolchain file for a MinGW compiler `cc` (an absolute path)."""
    path = Path(cc)
    prefix = path.name[:-4]
    root = path.parent.parent
    folder = path.parent
    tool = lambda suffix: folder / (prefix + suffix)  # noqa: E731
    arch = prefix.split("-")[0]
    return (
        "set(CMAKE_SYSTEM_NAME Windows)\n"
        f"set(CMAKE_SYSTEM_PROCESSOR {arch})\n"
        f"set(CMAKE_C_COMPILER {path})\n"
        f"set(CMAKE_CXX_COMPILER {tool('-g++')})\n"
        f"set(CMAKE_RC_COMPILER {tool('-windres')})\n"
        f"set(CMAKE_AR {tool('-ar')})\n"
        f"set(CMAKE_RANLIB {tool('-ranlib')})\n"
        f"set(CMAKE_FIND_ROOT_PATH {root}/{prefix})\n"
        "set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)\n"
        "set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)\n"
        "set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)\n"
        'set(CMAKE_EXE_LINKER_FLAGS_INIT "-static")\n'
    )


def probe_link_items(ninja_text: str, build: Path) -> list[str]:
    """The library items on the link line of the probe program, in order, files made absolute."""
    lines = ninja_text.split("\n")
    for k, line in enumerate(lines):
        if re.match(r"build probe(\.exe)?:", line):
            for follow in lines[k + 1:]:
                if not follow.startswith((" ", "\t")):
                    break
                key, _, value = follow.strip().partition(" = ")
                if key == "LINK_LIBRARIES":
                    items = []
                    for item in value.split():
                        if item.startswith("-") or os.path.isabs(item):
                            items.append(item)
                        else:
                            items.append(str((build / item).resolve()))
                    return items
            break
    raise Failure("build.ninja has no link line for the probe program")


def run_logged(argv: list[str], log: Path, env: dict[str, str], timeout: int, what: str) -> None:
    try:
        proc = subprocess.run(argv, capture_output=True, text=True, env=env, timeout=timeout)
    except subprocess.TimeoutExpired:
        raise Failure(f"{what} did not end after {timeout} seconds")
    except OSError as err:
        raise Problem(f"cannot run {argv[0]}: {err}")
    with log.open("a") as f:
        f.write(f"$ {' '.join(argv)}\n{proc.stdout}{proc.stderr}\n")
    if proc.returncode != 0:
        tail = [x for x in (proc.stdout + proc.stderr).strip().splitlines() if x][-12:]
        raise Failure(f"{what} failed (status {proc.returncode}); last lines:\n" + "\n".join(tail))


def commit_of(source: Path) -> str:
    """The commit of the PsyZ source, when the folder is the top of a git work tree of its own.

    A folder that lies inside another repository (a plain copy, a submodule that was never fetched) is
    `unknown`: git would answer there with the commit of the repository around it."""
    try:
        top = subprocess.run(["git", "-C", str(source), "rev-parse", "--show-toplevel"], capture_output=True, text=True, timeout=30)
        if top.returncode != 0 or not top.stdout.strip() or Path(top.stdout.strip()).resolve() != source.resolve():
            return "unknown"
        proc = subprocess.run(["git", "-C", str(source), "rev-parse", "HEAD"], capture_output=True, text=True, timeout=30)
    except (OSError, subprocess.TimeoutExpired):
        return "unknown"
    out = proc.stdout.strip()
    return out if proc.returncode == 0 and re.fullmatch(r"[0-9a-f]{40}", out) else "unknown"


def real_path(path: Path) -> Path:
    """The path with symbolic links followed; the part that does not exist yet is appended to the nearest existing parent."""
    return Path(os.path.realpath(path))


def inside(path: Path, folder: Path) -> bool:
    return path == folder or folder in path.parents


def written_places(build: Path) -> list[Path]:
    return [build / "src", build / "obj", build / "toolchain.cmake", build / "psyz.json", build / "build.log"]


def check_paths(source: Path, build: Path, patch_file: Path) -> Path:
    """Refuse a layout in which the run could delete, copy into or write inside the PsyZ source or the
    patch. Reads nothing but the paths. Returns the build folder, resolved."""
    src, bld, pat = real_path(source), real_path(build), real_path(patch_file)
    if inside(bld, src):
        raise Problem(f"the build folder {build} (resolved: {bld}) is the PsyZ source {source} (resolved: {src}) or lies inside it; the source is never written")
    if inside(src, bld):
        raise Problem(f"the PsyZ source {source} (resolved: {src}) lies inside the build folder {build} (resolved: {bld}), where the tool deletes and writes")
    for place in written_places(bld):
        if inside(pat, place):
            raise Problem(f"the patch file {patch_file} (resolved: {pat}) is {place} or lies inside it, where the tool deletes or writes")
        if place.is_symlink():
            raise Problem(f"{place} is a symbolic link, and the tool deletes and writes there; remove it or use another build folder")
    return bld


def refuse_links(source: Path) -> None:
    for rel in COPIED:
        for here, dirs, files in os.walk(source / rel):
            dirs[:] = [d for d in dirs if d not in (".git", ".github")]
            for name in dirs + files:
                if os.path.islink(os.path.join(here, name)):
                    raise Problem(f"{os.path.join(here, name)} is a symbolic link; the copy of the PsyZ source holds none, replace it by the file or folder it names")


def check_source(source: Path) -> None:
    """Refuse a source that lacks what the build reads, or holds a symbolic link. Reads only."""
    for rel in COPIED:
        if not (source / rel).is_dir():
            raise Problem(f"{source / rel} is missing; the PsyZ source needs {', '.join(COPIED)} (git submodule update --init, for SDL also inside psyz)")
    if not (source / "psyz" / "CMakeLists.txt").is_file():
        raise Problem(f"{source / 'psyz' / 'CMakeLists.txt'} is missing")
    if not (source / "external" / "SDL" / "CMakeLists.txt").is_file():
        raise Problem(f"{source / 'external' / 'SDL' / 'CMakeLists.txt'} is missing; the SDL submodule of PsyZ is not checked out")
    refuse_links(source)


def copy_source(source: Path, dest: Path) -> None:
    if dest.exists():
        shutil.rmtree(dest)
    for rel in COPIED:
        shutil.copytree(source / rel, dest / rel, symlinks=True, ignore=shutil.ignore_patterns(".git", ".github"))


def build(args: argparse.Namespace, out: list[str]) -> None:
    source: Path = args.psyz
    patch_path: Path = args.patch
    if not patch_path.is_file():
        raise Problem(f"{patch_path} does not exist")
    patch_text = patch_path.read_text()
    parse_patch(patch_text)
    cc = shutil.which(args.cc) or (args.cc if Path(args.cc).is_file() else None)
    if not cc:
        raise Problem(f"the compiler {args.cc} is not an executable")
    cc = os.path.abspath(cc)
    cmake = which_tool("cmake", args.cmake)
    ninja = which_tool("ninja", args.ninja)
    build_dir: Path = args.build or PORT / "build" / f"psyz-{triplet_of(args.cc)}"
    build_dir = check_paths(source, build_dir, patch_path)
    check_source(source)
    build_dir.mkdir(parents=True, exist_ok=True)
    work = build_dir / "src"
    log = build_dir / "build.log"
    log.write_text("")

    copy_source(source, work)
    done = apply_patch(patch_text, work)
    commit = commit_of(source)
    out.append(f"psyz: {commit}")
    out.append(f"patch: {patch_path.name} applied at {len(done)} place{'' if len(done) == 1 else 's'}")
    wanted = patch_commit(patch_text)
    if wanted and commit != "unknown" and commit != wanted:
        out.append(f"note: the patch was written for {wanted}")

    (work / "CMakeLists.txt").write_text(WRAPPER)
    (work / "probe.c").write_text(PROBE)
    mingw = "mingw" in Path(cc).name
    env = dict(os.environ)
    env["PATH"] = os.pathsep.join([str(Path(ninja).parent), str(Path(cmake).parent), str(Path(cc).parent), env.get("PATH", "")])
    obj = build_dir / "obj"
    configure = [cmake, "-GNinja", f"-DCMAKE_MAKE_PROGRAM={ninja}", "-DCMAKE_BUILD_TYPE=Release", "-DGTE_USE_HW_SQRT=ON", "-DPSYZ_RENDERER=sdl3_gpu"]
    if mingw:
        toolchain = build_dir / "toolchain.cmake"
        toolchain.write_text(toolchain_text(cc))
        configure.append(f"-DCMAKE_TOOLCHAIN_FILE={toolchain}")
    else:
        configure.append(f"-DCMAKE_C_COMPILER={cc}")
    configure += ["-S", str(work), "-B", str(obj)]
    run_logged(configure, log, env, args.timeout, "cmake configure")
    run_logged([cmake, "--build", str(obj)], log, env, args.timeout, "cmake build")

    items = probe_link_items((obj / "build.ninja").read_text(), obj)
    libs = [x for x in items if x.endswith("libpsyz.a")]
    if len(libs) != 1 or not Path(libs[0]).is_file():
        raise Failure(f"the probe's link line has {len(libs)} libpsyz.a, and it must have exactly one that exists")
    include = work / "psyz" / "include"
    (build_dir / "psyz.json").write_text(json.dumps({"include": str(include), "define": ["__psyz"], "link": items, "commit": commit}, indent=2) + "\n")
    out.append(f"library: {libs[0]}")
    out.append(f"include: {include}")
    out.append("link: " + " ".join(items))


def main() -> int:
    parser = argparse.ArgumentParser(description="Build PsyZ and SDL for the port's target from a patched copy.")
    parser.add_argument("--psyz", type=Path, default=PORT / "external" / "psyz")
    parser.add_argument("--patch", type=Path, default=PORT / "psyz.patch")
    parser.add_argument("--cc", default="i686-w64-mingw32-gcc")
    parser.add_argument("--build", type=Path, default=None)
    parser.add_argument("--cmake", default=None)
    parser.add_argument("--ninja", default=None)
    parser.add_argument("--timeout", type=int, default=1800)
    args = parser.parse_args()
    out: list[str] = []
    try:
        build(args, out)
    except Problem as err:
        print(f"psyzbuild.py: {' '.join(str(err).split())}", file=sys.stderr)
        return 2
    except Failure as err:
        sys.stdout.write("".join(x + "\n" for x in out))
        print(f"psyzbuild.py: {err}", file=sys.stderr)
        return 1
    sys.stdout.write("".join(x + "\n" for x in out))
    return 0


if __name__ == "__main__":
    sys.exit(main())
