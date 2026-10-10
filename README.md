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
resident executable byte-identically from declared owners: 1,759 functions at
their original addresses, of the 1,852 that a sweep of its code counts.
1,706 are compiled from C (289,388 bytes) and 53 are
assembled from assembly source (1,040 bytes): BIOS and system call stubs, a
few host-debugging stubs and the program entry routine. Units also own 2,352
bytes of read-only data and 5,900 bytes of initialised data. The
remaining 315,720 payload bytes are retained from the baseline and counted as
raw. Retained bytes are scaffolding, not recovered source. This is not a
full-game decompilation.
The same build has seventy-seven overlay modules as images of their own. In `slot2a`
all 28 functions are exact from C, 6,052 bytes, and 15,112 bytes are
retained raw.
In `slot0b` 57 C functions, 5,892 bytes, are exact and 25,544 bytes are
retained raw.
In `slot12` 181 C functions, 27,872 bytes, are exact and 92,296 bytes are
retained raw.
In `slot16` 11 C functions, 984 bytes, are exact and 10,406 bytes are
retained raw. In `slot17` the 11 functions of `slot16` are linked a second
time, at another address and from the same objects, and are exact against
its own chunk; 10,406 bytes are retained raw there as well.
In `slot00` 109 C functions, 12,240 bytes, are exact and 4,964 bytes are
retained raw, and in `slot08` the same 109 functions are linked a second
time and are exact against its own chunk.
In `slot2b` 103 C functions, 9,396 bytes, are exact and 23,172 bytes are
retained raw, and in `slot2c` the same 103 functions are linked a second
time and are exact against its own chunk.
In `slot0f` 218 C functions, 33,288 bytes, are exact and 70,560 bytes are
retained raw.
In the 20 stage modules, `slot06_00` to `slot06_13`, 845 C functions,
139,908 bytes, are exact and 1,220,780 bytes are retained raw.
In `slot27` 208 C functions, 30,184 bytes, are exact and 97,140 bytes are
retained raw.
In `slot28` 679 C functions, 84,216 bytes, are exact and 185,800 bytes are
retained raw.
In `slot01` 117 C functions, 16,372 bytes, are exact and 270,168 bytes are
retained raw.
In `slot04_00` to `slot04_0b`, the first-side blocks of the twelve character
files `PL00.PAC` to `PL0B.PAC`, 2,100 C functions, 253,332 bytes, are exact
and 684,880 bytes are retained raw; in `slot05_00` to `slot05_0b` without
`slot05_06` the functions of eleven of them are linked a second time, at a
second address, and are exact against the chunks of the `X` files.
The second side of the twelfth, `slot05_06` from `PL06X.PAC`, is four bytes
shorter than its first side, so it is an image with units of its own, copied
from those of `slot04_06`: 228 C functions, 25,276 bytes, are exact and
62,624 bytes are retained raw.
In the 11 first-side character blocks of slot `0x4`, `slot04_0c` to
`slot04_17` (the files `PL0C.PAC` to `PL17.PAC`), 2,311 C functions,
277,928 bytes, are exact and 732,412 bytes are retained raw. In the eight
second-side images `slot05_0c` to `slot05_14` the functions of the first
sides are linked a second time, at another address and from the same
objects, and are exact against their own chunks; two units of one
character stay raw there. In `slot04_sel`, the content of slot `0x4` of `SELECT.PAC`, 89 C functions,
27,092 bytes, are exact and 13,288 bytes are retained raw. The other modules
are not in the build.
The [Windows pilot](windows/docs/gameplay-pilot.md) is behaviorally tested but
not byte-matching. VC5 tools are available; game matching with VC5 is untested.

**Exact bytes at the correct addresses/mapping establish code identity.**
Running a game is not another mandatory correctness gate for byte-identical
output. Runtime observation is optional when useful for understanding behavior
or resolving loading questions. Nonmatching code needs its own validation.
Such code is in the folders `ps1/src/*_nonmatching/`, the first of them
[`ps1/src/slot06_nonmatching/`](ps1/src/slot06_nonmatching/README.md) with
the test: functions that the build keeps as original bytes, outside the
build and outside every count here, each with a contract and a
differential test against the original code.

## Layout

- [`ps1/`](ps1/README.md): primary target, matching build, overlay map,
  baseline audit and Ghidra project in an ignored local workspace.
- [`windows/`](windows/README.md): comparison research and private working files.
- [`port/`](port/README.md): groundwork for a port to macOS, Windows and
  Linux. The game does not run on macOS or Linux. On Windows a program
  linked from its unchanged C starts the game from the user's disc image
  and, in one scripted run, played into a second fight; it has no sound
  and is not shown to play to the end.
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

## AI-friendly workflow

Read [short current state](docs/current-state.md) and
[active memory](docs/project-memory.md), then retrieve historical context by
subject. The [AI workflow](docs/ai-workflow.md) provides one-command frozen-head
validation, a linked function ledger, integrity-checked result reuse and bounded
task contracts. Reports and private dependencies stay under ignored `local/`.

Before any build or test run, read [efficient checks](docs/efficiency.md): an
exact match is shown by its image's bytes alone, a nonmatching function is
tested alone, and no run is repeated or made for hours.
