# SFA2 decompilation

Game-focused Street Fighter Alpha 2 / Street Fighter Zero 2 reconstruction.
The current working target is the Japanese PlayStation release, `SLPS_004.15`.
Windows research is retained as a comparison and semantic reference, not a
second simultaneous delivery target.

See the [completion map](docs/completion-map.md) for source coverage, retained
bytes and the remaining route to delivery.

[![Coverage map: one square per function of the static sweep, green where the build owns it](https://fliperama86.github.io/sfa2-decomp/completion-map.svg)](https://fliperama86.github.io/sfa2-decomp/)

The picture is drawn on every push to `main` from the published
[function inventory](ps1/inventory/README.md) and the build configuration.

## Current evidence

The [PS1 pilot](ps1/docs/matching-pilot.md) rebuilds three connected functions
from readable C, matching all 592 original bytes at their original addresses.
A clean rebuild, independent byte comparison, bounded execution tests, and
mutation controls passed.

The [matching build](ps1/docs/matching-build.md) now rebuilds the complete
resident executable byte-identically from declared owners: 1,754 functions at
their original addresses, of the 1,852 that a sweep of its code counts.
1,701 are compiled from C (288,624 bytes) and 53 are
assembled from assembly source (1,040 bytes): BIOS and system call stubs, a
few host-debugging stubs and the program entry routine. Units also own 2,352
bytes of read-only data and 5,900 bytes of initialised data. The
remaining 316,484 payload bytes are retained from the baseline and counted as
raw. Retained bytes are scaffolding, not recovered source. This is not a
full-game decompilation.
The same build has fifty-two overlay modules as images of their own. In `slot2a`
all 28 functions are exact from C, 6,052 bytes, and 15,112 bytes are
retained raw.
In `slot0b` 56 C functions, 5,652 bytes, are exact and 25,784 bytes are
retained raw.
In `slot12` 178 C functions, 26,224 bytes, are exact and 93,944 bytes are
retained raw.
In `slot16` 11 C functions, 984 bytes, are exact and 10,406 bytes are
retained raw. In `slot17` the 11 functions of `slot16` are linked a second
time, at another address and from the same objects, and are exact against
its own chunk; 10,406 bytes are retained raw there as well.
In `slot00` 109 C functions, 12,240 bytes, are exact and 4,964 bytes are
retained raw, and in `slot08` the same 109 functions are linked a second
time and are exact against its own chunk.
In `slot2b` 102 C functions, 9,052 bytes, are exact and 23,516 bytes are
retained raw, and in `slot2c` the same 102 functions are linked a second
time and are exact against its own chunk.
In `slot0f` 215 C functions, 32,712 bytes, are exact and 71,136 bytes are
retained raw.
In the 20 stage modules, `slot06_00` to `slot06_13`, 756 C functions,
75,748 bytes, are exact and 1,284,940 bytes are retained raw.
In `slot27` 208 C functions, 30,184 bytes, are exact and 97,140 bytes are
retained raw.
In `slot28` 677 C functions, 83,960 bytes, are exact and 186,056 bytes are
retained raw.
In `slot01` 116 C functions, 16,100 bytes, are exact and 270,440 bytes are
retained raw.
In the 11 first-side character blocks of slot `0x4`, `slot04_0c` to
`slot04_17` (the files `PL0C.PAC` to `PL17.PAC`), 2,307 C functions,
276,772 bytes, are exact and 733,568 bytes are retained raw. In the eight
second-side images `slot05_0c` to `slot05_14` the functions of the first
sides are linked a second time, at another address and from the same
objects, and are exact against their own chunks; two units of one
character stay raw there. The other modules are not in the build.
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
- [`port/`](port/README.md): groundwork for a port to macOS, Windows and
  Linux. Nothing of the game compiles or runs there.
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
