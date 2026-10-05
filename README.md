# SFA2 decompilation

Game-focused Street Fighter Alpha 2 / Street Fighter Zero 2 reconstruction.
The current working target is the Japanese PlayStation release, `SLPS_004.15`.
Windows research is retained as a comparison and semantic reference, not a
second simultaneous delivery target.

## Current evidence

The [PS1 pilot](ps1/docs/matching-pilot.md) rebuilds three connected functions
from readable C, matching all 592 original bytes at their original addresses.
A clean rebuild, independent byte comparison, bounded execution tests, and
mutation controls passed.

The [matching build](ps1/docs/matching-build.md) now rebuilds the complete
resident executable byte-identically from declared owners: 1,616 functions at
their original addresses. 1,563 are compiled from C (271,324 bytes) and 53 are
assembled from assembly source (1,040 bytes): BIOS and system call stubs, a
few host-debugging stubs and the program entry routine. Units also own 2,332
bytes of read-only data and 5,900 bytes of initialised data. The
remaining 333,804 payload bytes are retained from the baseline and counted as
raw. Retained bytes are scaffolding, not recovered source. This is not a
full-game decompilation.
The [Windows pilot](windows/docs/gameplay-pilot.md) is behaviorally tested but
not byte-matching. VC5 tools are available; game matching with VC5 is untested.

**Exact bytes at the correct addresses/mapping establish code identity.**
Running a game is not another mandatory correctness gate for byte-identical
output. Runtime observation is optional when useful for understanding behavior
or resolving loading questions. Nonmatching code needs its own validation.

## Layout

- [`ps1/`](ps1/README.md): primary target, matching build, overlay map,
  baseline audit and Ghidra project in an ignored local workspace.
- [`windows/`](windows/README.md): comparison research and private working files.
- [`research/target-investigation.md`](research/target-investigation.md): original
  target-selection evidence, including a historical Saturn comparison.
- [`tools/ghidra/`](tools/ghidra/): shared analysis scripts written for the project.
- [`PLAN.md`](PLAN.md), [`docs/requirements.md`](docs/requirements.md): scope and
  delivery process, distinguishing implemented results from planned work.
- [`docs/project-memory.md`](docs/project-memory.md): decisions and lessons.
- [`docs/goals.md`](docs/goals.md): completed milestones and next work package.
- [`docs/migration.md`](docs/migration.md): copied/excluded files and verification.

The reconstructed PS1 source is in [`ps1/src/`](ps1/src/). ROM/disc extracts,
compiler binaries, analysis databases, third-party SDK headers and detailed
experiment outputs remain under ignored directories. The repository does not
yet provide a fresh-clone import/build pipeline: building needs local inputs
that it does not contain. See the [PS1 instructions](ps1/README.md).

## Existing local workspace

The existing offline checks use Python 3.12, the dependencies in
`requirements.txt`, and `mipsel-linux-gnu-objcopy` from the documented MIPS
binutils installation:

```sh
python3.12 -m venv .venv
.venv/bin/python -m pip install -r requirements.txt
.venv/bin/python tools/verify_workspace.py
```

Rebuilding PS1 C uses a private native build of the documented historical
compiler and local MIPS tools. The pinned reference compiler on the remote
Windows/WSL machine is optional and reproduces the same image. See the
[PS1 instructions](ps1/README.md). Do not copy an old virtual environment;
its launchers can retain paths to another repository.

The generic 68000/CPS2 workbench, its source/tests/history, and its ROM/capture
workspaces were deliberately not migrated. They remain in `../68k-decomp`.
