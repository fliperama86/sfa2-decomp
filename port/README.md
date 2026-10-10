# Port

Groundwork for a port of the game to macOS on Apple Silicon, Windows and
Linux. **The game does not run on any of them.** One thing runs since
2026-10-09, on Windows: a program linked from the game's unchanged C that
loads the game's program from a disc image and stops at the first function
that has no C, which today is the game's `main`. It is described under
"The host program" below. The library
that is to replace Sony's passes its own tests on a runner of each. With GCC every
one of its C units compiles on Linux and on Windows, and Apple's compiler
on macOS refuses a few, in a trial that is described below. This folder
holds the decisions taken so far, one pinned dependency, one check of that
dependency, one comparison of it with what the game calls, and one trial
that compiles the game's C with a PC compiler and lists what the objects
need. It changes nothing under `ps1/`.

The rule for a port is in the [requirements](../docs/requirements.md): the
game code stays as close to the original as possible, only Sony's library
code is swapped, and what game code cannot keep on another machine goes
behind build switches and is listed. No build switch exists yet.

## Decisions

By the owner, on 2026-10-06:

- The targets are macOS on Apple Silicon, Windows and Linux, from one source.
- The port has a worktree and a lane of its own, apart from the matching work.
- The replacement of Sony's library is the first piece.
- That replacement is [PsyZ](https://github.com/Xeeynamo/psyz), used as a
  dependency. It is not written here and not copied here.

By the owner, on 2026-10-09, when the measurements below were in and the
choice between the two ways that follow was put:

- "Until we proved it runs, we are not changing anything and we need to
  keep it as close to PS1's original as possible." Nothing of the game's
  source is changed for the port before the port is shown running.
- "Our port can just require the path to the ISO (provided by the user)."
  The game's data comes from a disc image that the user points to. The
  port holds and ships none.
- The proof comes on Linux or on Windows first, and macOS on Apple Silicon
  after it runs. This was put to the owner as what the first decision
  costs, and accepted: with no change to the source, the game's C holds
  only where a pointer has four bytes, as on the PS1, and macOS runs no
  native 32-bit application since version 10.15, by
  [Apple's note](https://support.apple.com/en-us/103076); the macOS of an
  Apple Silicon Mac is later than that. Whether such a program could be
  run there through emulation was not looked into, and the order of the
  proof does not rest on it. None of this was tried here. "With four-byte
  pointers" below has the count behind the first half.

What follows from these: the port's memory at the PS1's own addresses,
filled from the disc, in a 32-bit program, so that the addresses in the
game's data and source mean what they meant. "The host program" below is
the first piece of that, built and run.

By the owner, on 2026-10-09, later that day:

- The session that had done the stage and character modules took the port
  over: "I need you to take over."
- No original code is run by the port. Asked whether the PC program may
  run the functions that are not C yet from the original code on the
  disc, through a small interpreter, until their C exists, the owner
  answered: "No interpreter needed with the unmatched code, right?" So
  every function that is not C gets C first, nonmatching where it cannot
  be exact, with a contract and a differential test (see
  [`ps1/src/slot06_nonmatching/`](../ps1/src/slot06_nonmatching/README.md)),
  and the PC program stops with the function's name where C is missing.
  That the port is to consist of the project's C only is this page's
  reading of the answer, not the owner's words.
- Asked "shall I start?" on writing the PC program itself, after the
  first piece had been described: "sure go ahead".

Before that rule, a direction that was not a ruling. Nothing was built on
it, and the rule above replaces it for the time until the port runs:

- The game's tables hold PS1 addresses, four bytes each, and all three
  targets have eight-byte addresses. The sotn-decomp project, which uses
  PsyZ, has no stand-in for PS1 memory: its tables are C that its matching
  build checks byte for byte, so a host compiler gives them host addresses,
  and pictures and the like are read from files of the player's own disc
  or made into source from it when building. The recommendation put to the
  owner was to go the same way when the tables of this game are C, which
  nearly all are not yet (the plan lists ownership of game data as not
  accomplished), and not to decide before the library work needs it. The
  owner went on to the library.
- The other way that was discussed: one block of memory that stands in for
  the PS1's, loaded from the disc, with every stored address translated
  where game code follows it. It needs no table in C, and it touches game
  source at each such place.

## The dependency

PsyZ replaces the PSY-Q library, so that source written against it compiles
for other machines. It is a submodule at `external/psyz`, pinned to commit
`4e4b3e8dc7ae740c085fd190d635f54142d2d552`. A submodule records an address
and a commit: no file of PsyZ is in this repository.

Its `LICENSE` file at that commit gives three terms: MIT for its samples
and its test framework, among others; MPL 2.0 for its own two source
folders; and "Unlicensed" for its header folder and for a source folder
that the tree at that commit does not have. Its folder of decompiled
library code carries an MIT file of its own. The owner was told on 2026-10-06 that the headers carry no license and
decided to use PsyZ as a dependency.

Its porting guide asks of game source, among other things: its types for
ordering tables and for the first word of a primitive, no fixed addresses,
no symbols that overlap, and no address kept in a 32-bit integer. Each is a
build switch in the sense of the requirements when it is made.

### Building it for the port

```sh
python3 port/tools/psyzbuild.py [--psyz DIR] [--patch FILE] [--cc CC] [--build DIR]
```

The tool builds PsyZ and the SDL it carries as static libraries for the
machine the port runs on, by default with `i686-w64-mingw32-gcc` for
32-bit Windows, and prints what a program that links them needs. It
needs CMake and Ninja. It never writes into the submodule: it copies the
folders that the build reads and builds the copy. Its header is its
contract. On 2026-10-09, with PsyZ at its pin, it printed, with the
build folder written here as BUILD and the system libraries of the last
line left out:

```
psyz: 4e4b3e8dc7ae740c085fd190d635f54142d2d552
patch: psyz.patch applied at 1 place
library: BUILD/obj/psyz/libpsyz.a
include: BUILD/src/psyz/include
link: BUILD/obj/psyz/libpsyz.a BUILD/obj/psyz/sdl/libSDL3.a -lm ...
```

The copy is patched first, with `port/psyz.patch`: two lines removed.
PsyZ has one path for machines whose `unsigned long` has 4 bytes, and on
that path it draws into its batch of vertices and sends the batch to the
picture only when built for the browser. A 32-bit native program then
sees nothing of what it draws. The patch makes that path send the batch
always. The file says what was observed with and without it. The three
lines of PsyZ that the patch quotes are PsyZ's, under the Mozilla Public
License 2.0, and the patch says so and is offered under the same
license. Offering the fix to PsyZ's authors has not been done: that is
the owner's to decide.

The tool reads the patch with a reader of its own and refuses rather
than guesses: the lines a hunk expects must be in the file at exactly
one place, a hunk's body must have exactly the line counts of its
header, and any other line between hunks is refused. So a PsyZ whose
source differs at the patched place is not built. Every file the patch
names must lie inside the copy: an absolute path, a path with `..`, a
path that a symbolic link leads out of the copy, and a file named twice
are refused, and nothing is written until every hunk of every file has
been found.

The tool writes only under its build folder and only reads the PsyZ
source. Before it creates or deletes anything it resolves the source,
the build folder and the patch file, symbolic links followed, and
refuses a run in which the build folder is the source or lies inside
it, the source lies inside the build folder, or the patch file lies
where the tool deletes or writes. Symbolic links inside the folders it
copies are refused too. And because the tool builds again in an object
tree that it finds, it walks that tree first, without following links,
and refuses any symbolic link in it, any file with a second name, and
anything that is neither a file nor a folder: a link left in the object
tree could lead the build's own writes back into the source. The first
version checked none of this and deleted the source when given a build
folder that contained it; the owner's review found that, and then the
link in the object tree. What the tool does not guard is stated in its
header: a program that changes the tree while it runs, the contents of
the files it finds in the object tree, and the compiler, CMake and
Ninja it is given.

`python3 port/tools/test_psyzbuild.py` checks the reader, the paths,
the output lines and the exit statuses on invented trees and patch
texts, and the real patch on a file made of the lines it expects; every
case of a refusal also checks that the whole input is unchanged (a
difference is reported by the entries that were added, removed or changed;
the fixture's git commands are run with git's background work switched off,
so that nothing but the tool can write into a case's folder). It
needs no compiler. On 2026-10-09 it ended with
`all cases behaved as required`.

The graphics layer of the host program links the result when the host
build is given `--psyz` (below).

## The check

```sh
port/tools/check_psyz.sh [--headless] [--pictures DIR] [BUILD_DIR]
```

It fetches the submodule and the SDL source that PsyZ builds against, builds
PsyZ's own test suite and runs it one area at a time. It needs git, a C and
a C++ compiler, CMake 3.21 or later and Ninja; the last two can come from
`pip install cmake ninja`. `--headless` is for a Linux machine
without the development files for windows, graphics and sound.
`--pictures DIR` keeps the pictures that the suite writes when a
comparison fails. `BUILD_DIR` and `DIR` are taken from the folder the
script is called from; without `BUILD_DIR` the build goes to
`port/build/psyz-tests`, which Git ignores. It ends with status 0 when
every area passes, 1 when one does not and 2 when the suite cannot be
built or listed.

### On three systems

The workflow `Port PsyZ tests` runs the script on GitHub's runners for
Linux, macOS and Windows and puts the output on the page of the run. It
runs when the pin, the script or the workflow changes, and on request. It
is a measurement and no gate: a job fails only on status 2.

[Run 37946944473](https://github.com/fliperama86/sfa2-decomp/actions/runs/37946944473) of 2026-10-09, PsyZ at the pin:

| system | compiler, as CMake names it | the whole suite | status |
| --- | --- | --- | --- |
| Linux, x86-64 | GNU 13.3.0 | ztest: 342 passed, 0 failed, 1 skipped | 0 |
| macOS, Apple Silicon | AppleClang 21.0.0.21000101 | ztest: 342 passed, 0 failed, 1 skipped | 0 |
| Windows, x86-64 | GNU 15.2.0 | ztest: 342 passed, 0 failed, 1 skipped | 0 |

The three jobs printed the same lines:

```
bu: ztest: 7 passed, 0 failed, 0 skipped
dither: ztest: 10 passed, 0 failed, 0 skipped
events: ztest: 76 passed, 0 failed, 1 skipped
gpu: ztest: 37 passed, 0 failed, 0 skipped
gte: ztest: 133 passed, 0 failed, 0 skipped
horizontal_grid: ztest: 4 passed, 0 failed, 0 skipped
libcd: ztest: 14 passed, 0 failed, 0 skipped
libcd_playback: ztest: 3 passed, 0 failed, 0 skipped
path_adjustment: ztest: 7 passed, 0 failed, 0 skipped
spu: ztest: 45 passed, 0 failed, 0 skipped
spu_malloc: ztest: 3 passed, 0 failed, 0 skipped
truncation: ztest: 3 passed, 0 failed, 0 skipped
all: ztest: 342 passed, 0 failed, 1 skipped
```

- The whole suite passes on all three, the areas that compare drawn
  pictures among them: `dither`, `gpu` and `horizontal_grid`. The runners
  have no screen. On Linux the workflow installs the software drawing
  driver that PsyZ's own workflow installs; on Windows it tells SDL to
  draw off screen, as PsyZ's workflow does.
- On Windows the compiler was GCC, as on the Linux runner. PsyZ's own
  workflow builds with Microsoft's compilers there; this is the kind of
  build that a port compiled with GCC would link.
- One test is skipped by the suite itself, on all three as on the machine
  below.

What this does not show: a window on a screen, sound from a speaker, a
game. A test of the suite passing says that PsyZ does what its authors
check, not that it does what this game needs.

### On a machine that cannot draw

On 2026-10-07, on the Linux machine of these sessions, x86-64, which has
no development files for windows, graphics and sound, with `--headless`:
the library and its tests build, and the script printed

```
bu: ztest: 7 passed, 0 failed, 0 skipped
dither: ztest: 0 passed, 10 failed, 0 skipped
events: ztest: 66 passed, 10 failed, 1 skipped
gpu: ztest: 5 passed, 32 failed, 0 skipped
gte: ztest: 133 passed, 0 failed, 0 skipped
horizontal_grid: ztest: 1 passed, 3 failed, 0 skipped
libcd: ztest: 14 passed, 0 failed, 0 skipped
libcd_playback: ztest: 3 passed, 0 failed, 0 skipped
path_adjustment: ztest: 7 passed, 0 failed, 0 skipped
spu: ztest: 45 passed, 0 failed, 0 skipped
spu_malloc: ztest: 3 passed, 0 failed, 0 skipped
truncation: ztest: 3 passed, 0 failed, 0 skipped
all: ztest: 287 passed, 55 failed, 1 skipped
```

and ended with status 1. The three areas that compare drawn pictures
mostly fail there: the pictures that were looked at came out black. Ten
tests of timer events fail too.

Neither failure was reproduced on the three runners above, which pass the
picture tests and the timer tests. That is all the runs say. The black
pictures are consistent with that machine having nothing to draw with.
Why the timer tests fail there was not looked into: it may be the
machine, and a fault of the library that shows only in some surroundings
is not ruled out.

### The script's own controls

```sh
port/tools/check_psyz_controls.sh
```

runs the script against stand-ins for git, CMake and the test program, so
nothing is fetched or compiled: a fresh checkout with the default folder, a
relative and an absolute folder, `--headless`, `--pictures`, and each of
the statuses 0, 1 and 2. Its last line on 2026-10-09 was
`controls: 20 of 20 as expected`, here and in each of the three jobs. It
says nothing about PsyZ.

## What the game calls and what PsyZ has

```sh
python3 port/tools/libgap.py [--all] [--out FILE] [--archive FILE]
```

It reads the published [library inventory](../ps1/inventory/README.md) and
the source of PsyZ at the pin, which
`git submodule update --init port/external/psyz` fetches; without it the
script ends with status 2 and says which file it misses. No game file is
read and nothing is built. The header of the script says how each status
is decided. Its output on
2026-10-07:

```
compared: 101 of 416 library functions (those with a caller)
built: 73
stub: 1
some-targets: 1
assembly: 0
not-built: 3
absent: 0
unnamed: 23
disc: built 11, stub 0, some-targets 0, assembly 0, not-built 0, absent 0, unnamed 1
graphics: built 23, stub 0, some-targets 0, assembly 0, not-built 0, absent 0, unnamed 2
memory card: built 2, stub 0, some-targets 1, assembly 0, not-built 0, absent 0, unnamed 0
pads: built 1, stub 0, some-targets 0, assembly 0, not-built 0, absent 0, unnamed 2
sound: built 23, stub 1, some-targets 0, assembly 0, not-built 0, absent 0, unnamed 0
system: built 13, stub 0, some-targets 0, assembly 0, not-built 3, absent 0, unnamed 0
threads: built 0, stub 0, some-targets 0, assembly 0, not-built 0, absent 0, unnamed 3
unidentified: built 0, stub 0, some-targets 0, assembly 0, not-built 0, absent 0, unnamed 15
stub: SsSeqOpen
some-targets: _bu_init
not-built: Exec FlushCache InitHeap
unnamed: 80157090 80157174 8015760c 8015762c 801577bc 801577dc 801577ec 8015789c 80158374 80158470 8015a560 8015a570 8015c814 8015f734 8015fd24 8015fde4 8015fe04 8015fe28 80164ef0 801656ec 80166144 8016904c 8016a7e4
renamed by a header: EnterCriticalSection to PS1_EnterCriticalSection (psyz/include/kernel.h:171)
renamed by a header: ExitCriticalSection to PS1_ExitCriticalSection (psyz/include/kernel.h:172)
```

What that says of the 101 library functions that a game function of the
resident executable calls:

- 73 have a definition that PsyZ builds for every target. Two of them, the
  pair that enters and leaves a critical section, have it under another
  name that a header gives them.
- None is missing from PsyZ's tree altogether.
- `SsSeqOpen`, which opens a music sequence, is a stub: its body carries
  PsyZ's own mark for not implemented.
- `_bu_init`, which sets up the memory card, carries that mark under one
  compiler only.
- `Exec`, `FlushCache` and `InitHeap` are in PsyZ only as assembly for the
  PS1, in files that it does not build. Its headers declare them and the
  built library does not define them.
- 23 have no library name in this project yet, 15 of them in no family.
  They cannot be compared before the matching work names them.

What it does not say:

- Only names are compared. A function without the mark may do less than
  Sony's did, and PsyZ's version of a function may take other arguments
  than the version this game was linked with.
- The count of callers is the inventory's: the resident executable only,
  and only direct calls. Calls from the overlay modules are in no
  published table, so a library function that only a module calls is not
  among the 101.
- With `--all` every library function of the image is compared, called by
  the game or not, and the first lines are

```
compared: 416 of 416 library functions (all)
built: 235
stub: 22
some-targets: 5
assembly: 2
not-built: 51
absent: 49
unnamed: 52
```

  Many of the 49 that are absent are sound functions whose names begin
  with `SpuVm`, where PsyZ has names that begin with `_SsVm`. No resident
  game function calls one of them. It suggests, and does not show, that
  the library of this game is another version than the one PsyZ follows.

The scan was checked against the library that `check_psyz.sh --headless`
built on Linux from the same commit. With `--archive` and that file the
last line is `archive: 77 names checked, 0 disagree`, and with `--all` as well it is
`archive: 359 names checked, 0 disagree`: every name that the scan calls
built or stub is a symbol of that library, and none of the names that it
calls assembly, not built or absent is.

`python3 port/tools/test_libgap.py` runs the tool on small made-up trees.
On 2026-10-07 it printed 108 lines that begin `ok` and ended with
`all cases behaved as required`.

## The game's C on a PC compiler

Two scripts. Neither links or runs anything, and neither reads a game
file. The header of each says how it works.

```sh
python3 port/tools/hostcheck.py [--cc CC] [--std STD] [--flag=FLAG ...] [--build DIR] [--jobs N] [--timeout SECONDS]
python3 port/tools/hostneeds.py [--build DIR] [--out FILE]
```

The first compiles every C unit of the build configuration that is not
Sony's library with the C compiler of the machine it runs on, each unit
alone, into `port/build/`. It also checks the layout of every shared struct
with that compiler and counts the literals at PS1 addresses in the
sources. The second reads the objects that the first left and sorts every
name that some object needs and none defines: what a linker would ask for.

The counts move with every unit that the matching work adds, so the ones
here are for one tree: `ps1/` as it is in commit `8d0bb0d`. For today's, run
the scripts as they are. For these, give them that tree:

```sh
mkdir /tmp/tree && git archive 8d0bb0d ps1/src ps1/inventory | tar -x -C /tmp/tree
python3 port/tools/hostcheck.py --config /tmp/tree/ps1/src/build.toml
python3 port/tools/hostneeds.py --config /tmp/tree/ps1/src/build.toml \
    --symbols /tmp/tree/ps1/src/symbols.ld --inventory /tmp/tree/ps1/inventory
```

### What compiles, on one Linux machine

On 2026-10-09, x86-64:

```
compiler: cc (GCC) 16.2.1 20260810
language level: gnu89
pointer size: 8 bytes
units: 3727 compiled, 129 under sdk/ and 8 not C left out
passed: 3727
failed: 0
warning -Wpointer-to-int-cast: 363 in 242 units
warning -Wincompatible-pointer-types: 184 in 89 units
warning -Wint-to-pointer-cast: 177 in 102 units
warning -Wint-conversion: 57 in 27 units
warning (no option): 20 in 18 units
structs: 174 of 249 keep their layout
structs with a pointer: 75, of which 75 lose their layout
structs without a pointer: 174, of which 0 lose their layout
fixed addresses: 349 literals in 184 units
main memory: 253 literals in 138 units
main memory, uncached: 0 literals in 0 units
scratchpad: 96 literals in 49 units
ports: 0 literals in 0 units
BIOS: 0 literals in 0 units
```

and with `--cc clang`, without its last line, which names the 115 units:

```
compiler: clang version 23.1.1
language level: gnu89
pointer size: 8 bytes
units: 3727 compiled, 129 under sdk/ and 8 not C left out
passed: 3612
failed: 115
warning -Wpointer-to-int-cast: 360 in 240 units
error -Wincompatible-pointer-types: 184 in 89 units
warning -Wint-to-pointer-cast: 175 in 101 units
error -Wint-conversion: 57 in 27 units
warning -Wdeprecated-non-prototype: 18 in 4 units
warning -Wreturn-type: 18 in 15 units
error -Wreturn-mismatch: 12 in 8 units
warning -Wunsequenced: 4 in 4 units
warning -Wparentheses: 3 in 3 units
warning -Wpointer-sign: 3 in 2 units
warning -Wvoid-pointer-to-int-cast: 3 in 3 units
warning -Wint-to-void-pointer-cast: 2 in 1 units
warning -Warray-bounds: 1 in 1 units
structs: 174 of 249 keep their layout
structs with a pointer: 75, of which 75 lose their layout
structs without a pointer: 174, of which 0 lose their layout
fixed addresses: 349 literals in 184 units
main memory: 253 literals in 138 units
main memory, uncached: 0 literals in 0 units
scratchpad: 96 literals in 49 units
ports: 0 literals in 0 units
BIOS: 0 literals in 0 units
```

- The source is C that a compiler of today reads: with GCC and the old
  language level, every one of the 3,727 units compiles for a machine with
  eight-byte pointers.
- This clang refuses 115 of them, for three things: a pointer of one type
  handed over where another is declared and an integer and a pointer mixed
  without a cast, which GCC warns about there, and a `return` that does
  not fit the function's type, which GCC lets pass at this language level.
- 75 of the 249 shared structs lose their layout, and they are exactly
  the 75 that hold a pointer. No struct loses it for another reason. Code
  and data that count on these offsets cannot use these structs as they
  are.
- 363 casts turn a pointer into an integer of another size and 177 turn
  such an integer into a pointer. An address does not survive the first
  kind on this host.
- 349 literals are PS1 addresses by their value: 253 in main memory and
  96 in the scratchpad.

### What compiles, on three systems

The workflow `Port compile trial` runs the first script on GitHub's
runners for Linux, macOS and Windows, with the compilers that those have,
and puts each output on the page of the run. It runs when something under
`port/` changes, once a week, and on request. It is a measurement and no
gate: a job fails only when the script cannot run, and it checks nothing
of the matching build.

[Run 37943006240](https://github.com/fliperama86/sfa2-decomp/actions/runs/37943006240) of 2026-10-09, the same tree:

| system | compiler | passed | failed | errors, by the compiler's name for them |
| --- | --- | --- | --- | --- |
| Linux, x86-64 | gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0 | 3,727 | 0 | none |
| Linux, x86-64 | Ubuntu clang version 18.1.3 (1ubuntu1) | 3,692 | 35 | `-Wint-conversion` 57, `-Wreturn-type` 12 |
| macOS, Apple Silicon | Apple clang version 21.0.0 (clang-2100.1.1.101) | 3,688 | 39 | `-Wint-conversion` 57, `-Wreturn-mismatch` 12, `-Werror,-Wimplicit-function-declaration` 7 |
| Windows, x86-64 | gcc (x86_64-posix-seh-rev1, Built by MinGW-Builds project) 15.2.0 | 3,727 | 0 | none |
| Windows, x86-64 | clang version 20.1.8 | 3,692 | 35 | `-Wint-conversion` 57, `-Wreturn-mismatch` 12 |

- GCC compiles every unit on Linux and on Windows.
- The clang of each system refuses 35 to 39 units. All three refuse an
  integer and a pointer mixed without a cast and a `return` that does not
  fit. None of the three refuses a pointer of the wrong type, which they
  warn about: that error is the newer clang's of the machine above.
- Apple's clang, on Apple Silicon, refuses 4 units more than the other
  two, for calls of functions that nothing declares.
- The structs come out the same in all five jobs, and so do the pointer
  size and the count of literals: 75 of 249 lose their layout, on Windows
  too, where a `long` has four bytes.

### With four-byte pointers

`--flag=-m32` makes GCC compile for a machine whose pointers have four
bytes, as the PS1's have. The same tree, the Linux machine, 2026-10-09:

```
compiler: cc (GCC) 16.2.1 20260810
language level: gnu89
flags: -m32
pointer size: 4 bytes
units: 3727 compiled, 129 under sdk/ and 8 not C left out
passed: 3727
failed: 0
warning -Wincompatible-pointer-types: 184 in 89 units
warning -Wint-conversion: 57 in 27 units
warning (no option): 20 in 18 units
structs: 249 of 249 keep their layout
structs with a pointer: 75, of which 0 lose their layout
structs without a pointer: 174, of which 0 lose their layout
fixed addresses: 349 literals in 184 units
main memory: 253 literals in 138 units
main memory, uncached: 0 literals in 0 units
scratchpad: 96 literals in 49 units
ports: 0 literals in 0 units
BIOS: 0 literals in 0 units
```

- All 249 shared structs keep their layout, the ones with a pointer
  too, and every one of the 3,727 units compiles.
- The two kinds of cast between a pointer and an integer of another size
  are gone: no such warning is left.
- The literals at PS1 addresses stay. In such a program they can be the
  addresses they were, if its memory is put there.
- That machine compiles such a unit and cannot link such a program: it
  lacks the 32-bit libraries. Nothing was linked.

### What the objects need

From the objects of the GCC run on the Linux machine:

```
objects: 3727 of 3727 units
defined: 8685 names
needed: 4193 names
library by name: 10 names, needed by 11 units
assembly: 10 names, needed by 7 units
unit not compiled: 0 names, needed by 0 units
unknown: 2 names, needed by 174 units
C under another name: 2 names, needed by 4 units
library by address: 82 names, needed by 142 units
game function: 51 names, needed by 190 units
module function: 47 names, needed by 51 units
function elsewhere: 38 names, needed by 10 units
scratchpad data: 71 names, needed by 72 units
module data: 3094 names, needed by 1638 units
resident data: 786 names, needed by 1768 units
library by address: 76 named, 6 unnamed
unknown: _GLOBAL_OFFSET_TABLE_ __stack_chk_fail
```

- Data is nearly all of it: 3,094 names in the modules, 786 in the
  resident executable and 71 in the scratchpad that only the symbol file
  places. No C defines them. On the PS1 they are addresses that the
  linker is told; on another machine each needs something behind it.
- The library is asked for by address far more often than by name: 82
  names against 10. For 76 of the 82 the project knows the library's
  own name, which is the list a port needs to call PsyZ; 6 have none
  yet. `--out` writes the pairs. 10 more names are functions that the
  build has as assembly, stubs for calls into the BIOS among them.
- 51 functions of the resident executable and 47 of the modules are
  needed by the C and are not C yet. 38 more names are functions at
  addresses where the resident executable has none: another block that
  is loaded at that moment.
- 2 names are functions that a unit has under another name.
- The two unknown names are the host compiler's own, for its stack check
  and its table of addresses. Nothing of the game is unknown.

### What these do not say

- That code which compiles would work. A unit without a diagnostic can
  still count on the PS1 in ways no compiler sees.
- What a need is. A count of data names says nothing of how many bytes
  stand behind them or what they hold.
- That a need which has a name is met: the comparison above says what
  PsyZ has for a library name, not this list.
- The needs of the units that a clang refuses: from clang's objects the
  list is another, with a class of its own for what those units would
  have defined. The list was made on Linux only.
- Anything about Microsoft's own compiler, which has no such language
  level and was not tried. On Windows the compilers were GCC and clang.
- That a literal in a range is an address: `0x80000000` is also the sign
  bit of a word.
- The same counts from another version of a compiler: the kinds are the
  names that a version gives its diagnostics, and what is a warning in
  one is an error in the next, as the table shows.

`python3 port/tools/test_hostcheck.py` and
`python3 port/tools/test_hostneeds.py` run the two scripts on small
made-up trees. The first needs `cc` and a host with eight-byte pointers
and says so when it has neither; the second uses a stand-in for `nm` and
needs neither. On 2026-10-09 they printed 95 and 142 lines that begin
`ok` and each ended with `all cases behaved as required`. The workflow
runs the first of them on Linux and on macOS.

`python3 port/tools/test_hostcheck_stop.py` is one more control, for every
system: a stand-in compiler that does not end must be stopped, with what
it started. The others build their stand-ins in a way that Windows cannot
start, so the workflow runs this one in all five jobs.

## The host program

The first piece of the port itself. It is for Windows as a 32-bit program
and was built and run on one machine: Linux under WSL 2, with a MinGW-w64
cross compiler, the program started from that shell.

### How the game's C is made to run at PS1 addresses

- The program maps 2 MB at `0x80000000` and the scratchpad at
  `0x1f800000`, and loads the game's program there from the disc image.
- Every name of the game, data or function, is linked as its PS1 address.
  C then reads its data where the disc put it, and a call by name and a
  call through a function address that the disc's data holds both arrive
  at the PS1 address.
- Each unit is compiled to assembly. In the assembly every global symbol
  that the unit defines is renamed, on its definition only, to
  `impl_NAME`; in the object every reference to a game name becomes
  `ps1_NAME`, and the link places the `ps1_` names at the PS1 addresses.
  No game source is changed for it, and no game name can meet a name of
  the host's C library: the game has functions called `memcpy`, `printf`
  and `rand`, and the first program linked without that prefix ended
  before its first line.
- At start the program writes a 5-byte jump to `impl_NAME` at each
  resident function's PS1 address. A function without C gets a 5-byte
  call to one routine that says which function was reached and ends the
  program. Nothing is interpreted.
- The link is checked afterwards and the build fails otherwise: every
  `ps1_` name has its address, every implementation lies outside the PS1
  ranges, and no plain game name sits at a PS1 address.
- An image that the build configuration marks as `like` another is that
  image's units linked a second time at another address: the second side
  of each character is one. The tool makes a second object from the same
  assembly, with the names that move with the image renamed
  (`impl_NAME__X`, `ps1_NAME__X` for image X) and placed at the moved
  addresses, and every other name where it was. Which names move is taken
  from the matching build's rule; the tool's header gives it, with the one
  bound that is looser here because it would need the game's archive.
- Which functions have no C is read from the function inventory, a
  table that a static sweep of the original code made. A few of its
  rows are not functions, and the runtime writes a stop at every
  function without C: a stop written at such a row would land in the
  game's data. Two kinds are left out, and nothing else is.
  A row that begins with data in front of a function with C is left
  out only if `port/sweep_rows.toml` lists it. Each entry of that table
  names the row, the function behind the data and the number of data
  bytes, and says in words what was read in the original's listing that
  shows the bytes are data. The tool verifies every entry against the
  tree (the row exists, the function has C at that address and lies
  inside the row at the stated distance, a named data symbol stands at
  the row's address) and refuses to build with an entry it cannot
  verify. A row in front of a function that the table does not list
  keeps its stop, whatever lies next to it: nothing is inferred from
  what stands near a row.
  A row that begins inside the address range of a unit that is built,
  without being one of that unit's functions, is the tail of a function
  that the sweep split. That range is the unit's own code because the
  matching build requires the functions of a unit to follow one another
  without a gap; the tool checks that itself, and a unit for which it
  does not hold gets no such treatment.
  The tool counts the rows left out and `--list` names each with its
  reason.
- The program's image holds the console's copy of RAM: image base
  `0x10000`, no relocations, a filler section from `0x11000`, the first
  real section at `0x200000`; the check reads the linked file's header
  (see the header of the tool).
- The link places one marker before and one after the code of all game
  objects and checks that every implementation lies between them and that
  nothing of the runtime does. A later piece of the runtime uses them to
  tell the game's code from its own.
- The program runs nothing that the disc holds. The build carries the
  SHA-256 that the build configuration pins for the game's executable,
  and the runtime refuses a disc whose program has another hash before a
  byte of it is copied to a PS1 address. The only addresses it calls are
  ones where it wrote a jump itself: an entry code that points anywhere
  else is refused. The first version lacked both, and a made-up disc
  could make it call a byte of the disc; the owner's review found that.

### Building it

```sh
python3 port/tools/hostbuild.py [--config ps1/src/build.toml] [--cc CC] [--build DIR] [--overrides DIR] [--jobs N] [--list]
```

CC is a C compiler for 32-bit Windows, by default `i686-w64-mingw32-gcc`.
The header of the tool is its contract. It compiles the C units of the
build configuration that are not Sony's library and every function of the
folders `ps1/src/*_nonmatching/`, and links them with the runtime of
`port/src/`. It reads no game file. Its output on 2026-10-10, for main as
of commit `ca997f6` with the two functions and the three overrides that
the same change added:

```
compiler: i686-w64-mingw32-gcc (GCC) 16.2.0
units: 3907 compiled, 155 of them nonmatching, 0 failed
like images built: 22
functions with C: 12686
functions overridden in C: 13
functions without C: 385, library 384, game and modules 1
sweep rows that are not functions: 11
names at PS1 addresses: 46284
data defined in C, at host addresses: 0
linked: port/build/host/sfa2.exe, verified
```

The functions and names of the second placements are in these counts,
each under its own name. The thirteen of the line
`functions overridden in C` are the twelve functions of the folder
[`overrides/`](overrides/README.md) (see "Overrides in C" below); the
function of the character module `slot04_0f` is placed a second time in its
`like` image `slot05_0f` and counted again. The one
function of the game and its modules that the build still counts as
without C is the entry code (`--list` names it: `func_80118908`), which is
hand-written assembly that the program does not run. The
program's start line `overrides in C: M` is not in the sample of
"Running it", which was printed before that line existed and when the
game's `main` had no C.

For another tree, give the tool that tree's `build.toml` with `--config`,
as the scripts above take it, and with `--overrides` that tree's
overrides, or an empty folder for none: the default is this
repository's folder, whose files name functions of this game. A path
that is not a folder is refused.

### Running it

```sh
port/build/host/sfa2.exe DISC
```

DISC is the `.cue` of the user's disc image, or its `.bin`; from a Linux
shell under WSL the path is given in Windows form. With the disc of
`SLPS_004.15`, the program built above printed, with the image's path
written here as FILE:

```
memory: RAM at 0x80000000 (2 MB), scratchpad at 0x1f800000
disc: FILE, 2352-byte sectors
program: SLPS_004.15 at sector 243219, 614400 bytes to 0x80118900, entry 0x80118908
identity: SHA-256 matches the build's baseline
jumps: 1408 written for functions with C, 444 for functions without
library: 84 host routines, 300 left that stop
overrides: 1
start: 0x801189c4
stop: no C yet for func_801189c4 (0x801189c4)
```

and ended with status 3. The game's `main` is the first function it
calls, and `main` has no C in that tree. The original's entry code is
hand-written assembly and is not run: the program finds `main` as the
target of the last call before the entry code's halt.

### What stands in for the library

The game calls Sony's library and the console's BIOS at fixed addresses
of its own program. The runtime has a table of host routines for them,
by name or by address, and writes a jump to each over the stop call at
the routine's PS1 address. A library function without a host routine
still ends the program with its name. `sfa2.exe --list-library` prints
the table; for the program built from this tree it ends with

```
library: 84 host routines, 300 left that stop
```

of which the listing gives 39 as routines that do something and 45 as
routines that do nothing on purpose. What is in this piece:

- Events, critical sections, root counters and the callbacks of the
  BIOS and the interrupt library, with a frame clock: every wait or poll
  of the game that reaches one of these routines lets one vertical blank
  happen when 1/60 s has passed since the last, and the game's own
  handlers run then, on the game's thread. `--watchdog S` ends a run
  after S seconds without a vertical blank and says where it was.
- The vertical blank as an interruption. The game's `main` has a wait
  that calls nothing and loops until a counter moves which only its
  vertical-blank handler raises; with the frame clock alone the program
  would wait there for ever. So a second thread, the timer, interrupts
  the game's thread when a vertical blank is due and no library routine
  has taken it: it suspends the thread and points it at a small routine
  that saves the flags, every register and the floating-point state,
  gives the handler the state that compiled C expects at a call (an
  empty floating-point stack, the default control words of both
  floating-point units, a clear direction flag, an aligned stack), runs
  the vertical-blank work on the game's own thread, puts the saved
  state back and returns to the interrupted instruction. It does not
  save the upper halves of the AVX registers: only the game's own code
  is interrupted, and the build compiles it without AVX. Its bounds: the timer never calls game
  code or any library; a vertical blank is taken exactly once, by the
  timer or by the clock; the thread is interrupted only while it is in
  the game's own code, between the build's two markers, or at a jump in
  the PS1's RAM, never while a handler of the game runs and never inside
  a critical section. `--no-interrupt` turns the timer off. Every
  suspension of the game's thread, the timer's and the watchdog's, goes
  through one gate that is closed for good before the process ends: it is
  registered with `atexit` (every `exit()` and a return from `main`) and
  called before each direct `ExitProcess` on the game's thread (the crash
  routine of `main.c` and the stops of `mirror.c`), so the timer cannot
  be ended between a suspension and its resumption and leave the game's
  thread suspended inside the exit. `--timer-burst` makes the timer
  attempt its suspension without waiting between attempts and stay 1 ms in
  each round, and makes the stop routine check its own contract: if a
  round of the timer was still in flight when it returned, the program
  prints `stop: a suspension round was in flight when the stop returned`
  and ends with status 10 (a round that begins after the stop ends it with a line too; a direct `ExitProcess` that was not preceded by the stop ends with a line
  of its own, status 10; without the option those checks are not made).
  The hang itself was shown with the gate and the `atexit` call removed;
  with only the flag, no run hung. The gate is kept for the interval that
  no run reaches, and its contract has a case. The cost,
  stated: the game's code can be interrupted between any two
  instructions, as on the console, C that a PC compiler orders
  differently may be interrupted in a state the console never showed,
  and a run is not repeatable to the instruction.
- The BIOS's threads as fibers. The game uses them as tasks that switch
  only where the game says so.
- The few functions of the C library that the game takes from the BIOS.
- The memory card answers as if no card were inserted. The sound
  library's functions are accepted and do nothing: the port is silent.
  Both are stand-ins and listed as such.
- Overrides: host routines that run in place of a game function's C,
  for what cannot run on a PC as written. There is one, with its reason
  in the listing: it sets up the game's task slots as the function's C
  does, without the stores into the BIOS's thread table, which is at an
  address the program does not have. An override is accepted only for a
  function that has C: it is not a way to supply a function. A second
  one is needed and is not in this piece: one function walks an
  ordering table through the copy of RAM that the PS1 shows from
  address 0, a range that a Windows program cannot have. Its host
  routine waits for differential evidence against the original code;
  until then that function is one that stops.
- Overrides in C: a second C file of the port, kept as
  `port/overrides/NAME.c` with `NAME.py` beside it, that runs in the PC
  program in place of the C of the game function NAME. The reason for
  such a file is exact C that cannot run on a PC as written: for
  example a call through a pointer declared without parameters, to a
  function that reads its parameter, which on the console finds the
  value still in the argument register. The game's source is not
  changed for the port, so the port carries its own C for that function.
  `hostbuild.py` accepts one only for a function that a unit of the
  configuration declares and that `hostcheck` selects: a nonmatching
  function, an unknown name and a function of an unselected unit are each
  refused with their own message and the file's name, before any
  compilation, and so is a file without `NAME.py`, which is the contract
  of the override's differential test (the tool requires the file and does
  not read it). The override is compiled alone with the flags of a unit,
  includes the game's headers by a path relative to its own folder, and may
  define exactly one global symbol, NAME; static functions and data are
  fine. In the unit that defines NAME the one definition is renamed
  `replaced_NAME` and the unit's other functions stay as they are; every
  call to NAME, from other units and from the unit itself, goes by the PS1
  address and so reaches the override. A `like` image gets the override a
  second time for its second placement (`impl_NAME__X`), and none where
  the unit is left out. The row of `port_functions` has a field
  `overridden`, 1 for such a row. The build prints `functions overridden
  in C: N` directly after `functions with C:`, and `--list` names each
  override with its file and its unit. The program prints `overrides in
  C: M` directly after the `overrides:` line, M being the number of rows
  with that field at 1; a function that has such a row and is also in the
  table of host overrides above is refused at start, by name, with the
  words "has two overrides".
  The evidence for an override comes from the differential test of its
  contract, run with the tool of `ps1/src/slot06_nonmatching/` and
  `--folder`; a pass is evidence for the tested inputs, not equivalence.
  The build above counts those of [`overrides/`](overrides/README.md):
  functions of the resident program and of modules whose exact C calls
  a function without the argument that the callee reads: by a plain
  call, through a pointer of the scratchpad, or after another call,
  where the callee gets what that earlier call left in the register;
  and functions whose exact C is declared `void` although their
  callers use what they leave in the result register (the override
  returns it). That page has each one's contract and
  what its test printed. Each has a control that alters the build of
  the override, and the test must then differ from the original code:
  for an override that passes an argument, the altered build hands the
  callee another value; for one that returns a result, the altered
  build returns its result plus 1, after its library call and with the
  arguments unchanged.
  What the build does not check: that an override's C has the
  parameters and the result type of the unit's C, and that the test of
  its contract passes or was run. The build reads no C and has none of
  the game's files; both belong to the differential test.
  What no command of this repository shows: that the PC program with
  them plays on where it stopped without them. That was seen only in
  two private runs with the user's disc on 2026-10-10, the same tree
  built with the folder and with an empty one: without the overrides
  the program stopped in a fight, inside the call that `func_8014dcc0`
  makes through the pointer; with them it played through that fight
  and on for as long as the run lasted.
- The runtime calls only what it installed. The game hands the library
  addresses to call later: a thread's entry, an event's handler, the
  interrupt and vertical-blank callbacks. Each is checked at the moment
  of the call: it must be an address where the runtime itself wrote a
  jump or a stop, or lie inside the game's own compiled code, between
  the build's two markers. Anything else ends the program with a line
  that names the path and the address, before the call. A function
  without C as a target still ends with its named stop. The limit of
  the second test: any address inside the game's compiled code passes,
  not only a function's first instruction, because the build does not
  list the game's static functions.
- `--trace` and `--trace-file FILE` write one line per library call.
- The disc. The game reads its files through the CD library: it sets a
  position, starts a read, and a handler of the game's takes each
  sector as it arrives. The host routines serve that from the user's
  image: `CdInit`, `CdSync`, `CdReady`, `CdControl` and its two
  variants, `CdMix`, `CdGetSector` and the two position conversions,
  with these commands of the drive: no operation, set position, read
  (both kinds), pause, set filter, set mode, get position, seek.
  Sectors are delivered three per frame, from the vertical blank or
  from the game's own poll, and the game's handler is called once per
  sector; no vertical blank is delivered inside it. Sectors of
  compressed audio are taken and dropped: nothing sounds. Any other
  command, and the mode that asks for whole raw sectors, ends the
  program with a line that names it. The handler's address is checked
  at each call like the other addresses the game hands over.
- What the disc layer does not trust. `CdGetSector` copies a sector to
  an address and a length that the game gives: both must lie inside the
  PS1's RAM, or the program ends with a line before a byte is copied.
  The file table of the image is read record by record inside the
  bytes that each directory declares as its own: a record that crosses
  the end of its sector or of its directory is refused, and what stands
  in the sector behind the declared end is not read. Every file that
  the reader hands out lies inside the image with its last byte, by
  arithmetic that cannot wrap; a file that does not is refused with its
  name and with the sector where the image ends. The first version
  bounded the directories and not the files in them; the owner's review
  found that.
- Programs of the disc. The game starts other programs from the disc
  with `Exec`. No C exists for any of them, so the port ends there with
  `stop: no C yet for the program NAME (disc sector N)`, as it does for
  a function without C. `--skip-programs` goes on instead as if the
  program had returned at once, and prints a line at each skip; that is
  not what the game does, and it is off unless asked for.

- The modules. The game loads its modules from the disc into RAM and
  calls into them. The runtime cannot know at start which module will
  lie where, so it writes a module's jumps when the game first runs it.
  The disc layer remembers for every word of RAM which sector it came
  from, and a page that it wrote loses the right to execute. The game's
  first call into such a page faults. The fault handler looks at the
  word at the called address: its sector names a file of the disc and,
  with the archive's header, a chunk; the image is the one that the
  build's tables give for that archive, slot and address, and the
  address must be the start of one of its functions. Before a jump is
  written, the chunk is read again from the user's image and its
  SHA-256 is compared with the hash that the build configuration pins
  for the image, and the bytes in RAM at each function's start must
  still be the chunk's. Then the jumps to the C, and the stops for the
  functions without C, are written over the functions that the chunk
  wrote, the pages may execute again, the program prints
  `module: NAME at ADDRESS, N C jumps, M without C`, and the call goes
  on. A later write from the disc takes the right away again.
  What ends the program instead, each with its line: a chunk that is
  not the pinned content, an image without a pin, memory that changed
  after the disc wrote it, an archive header whose counts, offsets or
  lengths do not fit the file or the RAM, a call that no image fits, a
  call at bytes that did not come from the disc, a copy from the disc
  that reaches the jump of a resident function, and a page that would
  become executable while it holds the start of a function of another
  image that is not yet placed. By the build's tables no two images
  that can be in memory together share a page with their code, so that
  last refusal is not expected on the real game; a module's data can
  share a page with anything, and a page that holds resident code never
  loses the right to execute.
  Installation is tracked per entry, not per page. For each function of
  the tables the layer keeps whether the five bytes of its jump (to the C)
  or of its stop call (without C) were written. A function counts as
  installed, for the checks of addresses that the game hands over (a
  thread entry, an event handler, a callback), only if that is so, the
  pages under its five bytes still belong to the module, and the five
  bytes are at this moment exactly the jump or call that was written. A
  later write from the disc to such a page takes the module away from it,
  and every entry on it is then not installed. Before anything is
  written, every declared entry of the image that lies on a page about to
  become executable must have its five bytes inside the chunk and all
  written from it; otherwise the placement is refused with the lowest such
  entry named, and no jump of the image is written, so a page never
  becomes executable first. This answers two cases of the review: the
  first sector of a module loaded and its first function called, then a
  later entry of the same page handed over as a callback; and a copy of
  only four bytes of an entry. Both are refused (status 12 for the
  callback, the placement refusal for the copy), also for an entry
  without C, and a game write over an installed entry is refused with
  `jump is no longer there`.
  A resident function's entry is checked by content as well: its five
  bytes must still be a jump or call to host code. Stated limits: a game
  write that leaves another jump to host code is not seen; a direct call
  (not a handed-over address) into overwritten bytes is not seen in either
  layer; a second placement gets the jumps of its own table, and the
  units left out for it are functions without C there.
  Two handlers of access faults exist in the program. This layer's
  handles execute faults only, at an address in the PS1's RAM whose page
  it made non-executable; the mirror's (see "The console's copy of RAM at
  address 0") handles read and write faults only, below 2 MB, from the
  game's code on the game's thread. They are installed in this order: the
  mirror's at start, this layer's after the program is loaded; the
  later one is asked first, declines every fault that is not its own
  kind and place, and the other then gets its turn. An execute fault at a
  low address is neither's: it ends with the mirror's crash line. The
  program ends only by `exit()` (registered with `atexit` to close the
  timer's gate first, see the timer above) or by the crash routines,
  which close the gate themselves; with `--timer-burst` the program says
  so if a way of ending skipped the stop. The controls run each program
  that this layer ends with and without that option and compare.

- The graphics, through PsyZ. This part exists only in a program built
  with `hostbuild.py --psyz DIR`, DIR being a build folder of
  `psyzbuild.py`; without the option the graphics functions stay among
  those that stop and the program links nothing of PsyZ. With it the
  program built from this tree prints
  `library: 111 host routines, 274 left that stop`. The layer serves 27
  functions of the graphics library. The drawing goes through PsyZ:
  every list that the game hands to `DrawOTag`, and the packet of
  `PutDrawEnv`, is walked by the port and each packet is handed on
  behind the two-word tag that PsyZ uses, because the game's lists are
  linked by 24-bit addresses of the PS1's RAM. `ClearImage`,
  `LoadImage`, `StoreImage`, `MoveImage`, `DrawSync`, `SetDispMask`,
  `PutDispEnv`, `ResetGraph` and the window are PsyZ's as well. The
  port itself does what only touches the game's own structures: the
  ordering-table setters, `AddPrim` and its relatives, the setters of
  primitives, `GetTPage`, `GetClut`, `SetDrawMode`, the default
  environments, and the words of a drawing environment, after the
  library's own code. A picture is presented once per vertical blank.
  The window opens windowed; closing it ends the program.
- What the graphics layer does not trust. The lists are the game's data:
  every link must point into the RAM and a list that does not end is cut
  off by a count. A packet is a stream of commands: before any word of
  it reaches PsyZ, the port steps through the whole packet with the
  number of words that each kind of command takes, and the stream must
  end exactly at the packet's end; a command that would need words
  beyond it ends the program with a line that names the kind and the
  word. PsyZ is given the packet's words and nothing else. A kind that
  the port does not decode (polylines, the copies that carry their data,
  and a few more) ends the program too, wherever it stands in a packet:
  nothing is skipped. So does a kind whose length PsyZ reads differently
  from the console's: the lines with bit 2 set (`0x44` to `0x47` and
  `0x54` to `0x57`), for which PsyZ takes one word more, the next
  command's first word. Such a kind is refused, never rewritten into
  another form. The walker's table and PsyZ's decoder were compared
  for all 256 kinds (the comment above `command_words` in `gpu.c` has the
  table with file and line), and `test_hostgpu.py` sends every kind,
  followed by a complete fill, as a control. A rectangle of an image routine must lie inside
  the frame buffer of 1024 by 512; a width or height of zero or less
  becomes 1 and one above 1023 or 511 becomes that, as the library's
  code does, before the test. The console's hardware wraps a rectangle
  that leaves the frame buffer; the port does not do that yet and ends
  with a line instead. A drawing or image routine before
  `ResetGraph(0)` ends the program: PsyZ would hang. `SetDispMask` and
  `DrawSync` are served before it, as the console's library serves them
  and as the game's start-up calls them; `ResetGraph(0)` then leaves
  the display off, as the reset of the hardware does.
- Which memory is the game's. A structure that the game's C keeps in a
  local lies on the console's stack inside the RAM; on a PC it lies on
  the host's stack. So a structure or buffer that the game hands over by
  pointer is accepted when the whole of it lies in the RAM, in the
  scratchpad, or in the live part of the stack that the calling task is
  running on, and refused otherwise: not another task's stack, not the
  heap, not the program's own code or data. One routine of the runtime
  decides this for the graphics layer. The nodes of a list must be in
  the RAM, since their links are RAM addresses. The first version
  accepted the RAM only and stopped the real game at its first
  `ClearImage`, whose rectangle is a local.
- Where the picture differs from the console's, known so far: a drawn
  pixel reads back with its top bit set, where PsyZ keeps opacity; PsyZ
  rounds a flat colour's 8 bits to 5 in its own way (248 gives 30, the
  console's shift gives 31); `ClearOTag` ends a table with the
  library's end mark itself; a present waits for the display's refresh
  on a window that cannot tear. None of this has been compared with the
  console's picture: the list is what the worker saw in PsyZ's code and
  in the controls.
- `--dump-vram PREFIX` writes the frame buffer as a picture file when
  the program ends, and `--dump-every N` every N seconds.

- The pads. `InitPAD` gives the BIOS two buffers and `StartPAD` starts
  the reading; from then on, at every vertical blank, port 1's buffer
  receives the BIOS's frame of a digital pad (status byte 0, kind 0x41,
  two bytes of buttons in which a 0 bit is a pressed button) and port
  2's receives 0xff, the BIOS's "no controller". Nothing is written
  before `StartPAD`, and a buffer is written up to the 34 bytes of a
  controller's frame and no further. The buffers are the game's, so each
  is checked when `InitPAD` gives it: a null buffer and a length of 0
  are served without a write, a negative length is refused, and any
  other buffer must lie whole in the game's memory by the rule given
  above for the graphics (the RAM, the scratchpad, the live part of the
  running stack); otherwise the run ends with `stop: InitPAD: ...`, status
  9, before any byte is written. Reading the devices is
  PsyZ's: with PsyZ linked (`hostbuild.py --psyz`) `InitPAD` calls PsyZ's
  `PadInit(0)`, which starts SDL's game controller handling and marks
  PsyZ's pads initialised, and every vertical blank calls
  `Psyz_PadsPoll()` and then `Psyz_PadsGet(0, ...)`. The poll is made
  here because `Psyz_PadsGet` polls the devices only once by itself and
  then hands out the same frame until PsyZ's own `VSync` clears its flag,
  and this program never calls that `VSync`. Without PsyZ port 1 is a
  digital pad with no button pressed. With PsyZ linked the start prints
  one line after the `overrides:` line that names the keys of PsyZ's table
  for port 1 (`pad: keys: W = L2, ...`; a game controller works as well);
  the control reads PsyZ's table and button definitions from its source
  and compares the line with them. A known limit, not worked around: PsyZ
  reads the keyboard for port 1 only when SDL reports that a keyboard
  exists, and in a Windows Remote Desktop session SDL reported none
  (a probe of 2026-10-10, not a published command); key events still
  arrive there (Escape ends the program) but no key is a button, so
  without a game controller PsyZ's frame says "no controller" and the
  game sees none. The keyboard is not read beside PsyZ.
- The input script. `--input FILE` presses and releases buttons at
  frames of the port's own clock, on top of what PsyZ gave (a pressed
  bit is 0 in the buffer), so it does not depend on the window's focus
  or the host's speed. A line is `SECONDS BUTTON down`, `SECONDS BUTTON
  up` or `repeat SECONDS EVERY BUTTON`; blank lines and lines that start
  with `#` are skipped. SECONDS and EVERY are decimal numbers (digits and
  one point); a time is the frame `round(SECONDS * 60)`, the frame being
  the number of the vertical blank since the program started (the first
  is 1). The times do not go backwards (compared as frames, so two times
  that round to one frame are in order). BUTTON is one of `start select
  up down left right cross circle square triangle l1 r1 l2 r2 l3 r3`. A
  button is held from its `down` line to its `up` line; a `repeat` line
  presses BUTTON for 4 frames every EVERY seconds (at least 0.1) from its
  time up to, not including, the time of the next line that is not a
  `repeat` line, or for an hour if there is none, and a press that would
  run past that time ends at it. The holds of the `down` and `up` lines
  and the presses of the `repeat` lines are kept apart: at a vertical
  blank a button is pressed when a `down` line holds it or a press of a
  `repeat` is running, so the end of a repeat's press never releases a
  button that a `down` line holds, and two repeats on one button do not
  cut each other short. At one frame the lines apply in their order. A script that
  is empty or holds only comments, a file over 1 MB, a line over 200
  characters, a NUL byte, a script of over 1,000,000 steps, and every
  line that is not of these forms refuse the start (status 2) with one
  line that names the file and the line number, before anything else is
  printed. When PsyZ's frame says "no controller" the script presses
  nothing, and one line says so the first time. With `--trace`, a line
  `pad frame N: 0x.... -> 0x....` goes to the trace file when the word
  that the game builds from port 1's buffer (`~(byte3 | byte2 << 8)`, a 1
  bit for a pressed button) changes; a frame that says "no controller"
  gives the word 0.

### The console's copy of RAM at address 0

The console shows its RAM a second time from address 0, and the game's
C reaches that copy in two ways. By accident: a function adds an offset
to a null pointer and reads there. A private run of the real game met
one such read, at address `0xd`, while its first fight was loading; on
the console that is a byte of low RAM and nothing happens. On purpose:
one function follows links of 24 bits, whose top byte is cut off,
through an ordering table.

A Windows process cannot have memory at address 0, and from `0x10000`
up to the end of the 2 MB it would get memory of the system's own, where
an access does not fault. So the program takes the range first, with its
own image. It is linked with its image base at `0x10000`, not
relocatable, and with its first real section at `0x200000`. Windows
refuses an image whose sections leave a gap, so a filler section,
`.hole`, uninitialized, covers `0x11000` to `0x1fffff`. The system maps
the filler readable and writable; at start the runtime makes it
inaccessible. Nobody else can allocate there, so every access of the game
to the range faults, each time. The settings and the check of the linked
file's header are in `hostbuild.py`. The program also checks, at start,
that every page of `0` to `0x1fffff` is inaccessible, and refuses to
start otherwise and names the page; a program linked the old way
refuses.

A handler for access faults takes a read or a write that the game's
thread made, by an instruction inside the game's own compiled code, at
an address that lies wholly in `0` to `0x1fffff` and does not touch the
image's header page. It decodes that one instruction, carries it out on
the mapped RAM at `0x80000000` plus the address, sets the registers and
the flags as the processor would, and resumes behind it. Nothing of the
game is replaced, and no original code is run: the instruction is the PC
compiler's translation of the game's C. An access that begins inside
the range and ends at or above `0x200000`, one made by the runtime's or
a library's code, and an execute fault each end with the crash line. The
forms served are loads, stores and plain arithmetic and logic (`mov`,
`movzx`, `movsx`, `add`, `sub`, `and`, `or`, `xor`, `cmp`, `test`); any
other form ends the program with a line that gives the instruction's
bytes, and is built when a run of the real game stops on it. The first
time a function uses the copy, the program prints one line with the
function's name; with `--trace` every access is a line of the trace file.

What the copy holds. On the console the first 64 KB of RAM belong to
the BIOS. The port has no BIOS: that part of the RAM holds zeros until
the game or the port writes there. An accidental read therefore reads 0
where the console read a byte of the BIOS's.

The header page. The first page of the image, `0x10000` to `0x10fff`,
holds the executable's header and stays readable: the C library reads
the header when the program exits, and with the page closed the program
ended with an access violation inside the library (the record has the
evidence). So an access of the game to that page is not served: a read
returns the header's bytes and is not seen, and a write is the crash line.

What else is not served. `0x200000` and above is the program's own first
section: a read there returns the program's bytes and is not seen, a
write is the crash line.

Cost. Every served access is a fault, which costs far more than a memory
access. The controls print the cost of 10000 served reads and the time of
a walk of 2,000 nodes of an ordering table through links of 24 bits, once
through the low view and once through real addresses (the last runs are
in the record). A function that walks the low view in a loop is slow,
not wrong. The game's function that follows such links needs no host
routine: its C runs through this layer (the controls show a made-up walk,
not that function).

### What this does not show

- That the game runs, draws or sounds. No function of the game has been
  executed on a PC by the published tree: the program stops before the
  first one.
- That C which compiles and links behaves on a PC as it does on the PS1.
- Anything about the modules on the real game: the published tree
  stops at `main` before one is loaded. The placing of a module has run
  only on the made-up archives and game code of the controls.
- Anything about the library on the real game: its host routines have
  run only on the made-up game code of the controls, because the
  published tree stops at `main` before any of them. That holds for the
  graphics too: nothing of the real game has been drawn by a published
  commit. The build tool also writes each image's archive names into
  its tables, and its header names `modules.c` for them: that file is
  not in this tree yet.
- Anything about the pads on the real game or a real device: the controls
  run the pad routines on made-up game code with a stand-in of this
  file's own for PsyZ's three routines, so no key, controller or window
  was involved, and nothing here shows that PsyZ's real routines give
  the frames the stand-in gives. The line of keys is compared with
  PsyZ's source, not with a keypress. The script has been run against
  that stand-in and the build without PsyZ only.
- Linux and macOS: the memory mapping is written for Windows only.
- Anything about the real game's accesses to the copy: the controls use
  made-up game code.
- An access of the game to the header page, `0x10000` to `0x10fff`: a
  read returns the header's bytes without a fault.

### Controls

`python3 port/tools/test_hostbuild.py` checks the tool on made-up
assembly, tables and symbol lists, without a compiler: the renaming of
definitions, the names, the tables, the choice of units, the second
placements, the markers, and each miss of the link check. Its cases for
overrides in C: the rename to `replaced_NAME`, the unit's other
definitions, the flagged rows, the object between the markers, the
second placement and the left-out unit, each refusal (an unknown name,
a nonmatching function, a function of an unselected unit, a missing
`NAME.py`, an option that names no folder), a build that stops (an override that does not compile,
defines another global symbol or not NAME, or whose unit does not define
NAME), the link check of a `replaced_` symbol, and the two output lines
with 0 and with more. With `--cc CC` (and `--run PREFIX` as for
`test_hostlaunch.py`) it also builds a made-up game of four units with
overrides and starts the Windows program: the override runs for a call
from another unit and for a call from the unit that defines the
function, the unit's other functions keep their own C, a call through a
pointer declared without parameters reaches a callee that reads its
parameter (the case does not state what the unit's own C prints there,
which is not the same on every run), and both placements of a `like`
image run the override, each with the moved name of a data symbol; the
same game without overrides runs the units' C and says 0. `python3 port/tools/test_hostrun.py` builds the disc
reading and program loading of the runtime with the host's own `cc` and
runs them on disc images that the test makes from invented bytes, with
the hash of the program and the gate at the entry among them, and the
refusal of a function that has a host override and a flagged row.
`python3 port/tools/test_hostlaunch.py --cc CC` needs the cross compiler
and a way to start a Windows program: it builds the runtime with small
made-up tables and runs the real start of the program on invented disc
images; a failing case prints the status and the whole output (every
line of stdout and of stderr) of its last program run, also when that run
did not end in time, and the graphics and mirror controls do the same. Its cases: the right image stops at a function without C; an
entry with C runs that C; an image that differs in one byte from the
pinned one is refused and nothing of it runs; an entry at an address
that no table holds, inside a function, just before one, or in a module
is refused. Its cases for the library, all on made-up game code: a
library function with a host routine is reached and one without still
stops; an override replaces a function's C; a flagged row is counted on
the line `overrides in C` and not on `overrides`, and a function with a
host override and a flagged row is refused by name; a table that names a
function twice, an unknown one or an unknown address is refused, and so
is an override of a function without C; the trace has each library
call; a read of an address that no memory has ends the program with
the crash line; a thread entry, an event handler, an interrupt callback
and a vertical-blank callback at an address that the program did not
install are each refused before the call, and the byte there does not
run, while the same four paths with a function without C as the target
end with its named stop and with a handler inside the game's own code
run it; a handler runs once per vertical
blank, is held back inside a critical section and delivered once after
it; an event that was closed is not called; three tasks run in the
order the game switches them, and a task function that returns ends
the program; the card routines answer with the time-out event. Its
cases for the end of the program: a program that ends by `exit()` and one that ends through the crash routine, each run many times with `--timer-burst`, each of which must end by itself in time (a run that does not fails its case and is ended by its process id). Its cases for the interruption: made-up game code that spins on a counter
which only its handler raises ends by itself, and the handler ran on the
game's thread; the same code with `--no-interrupt` is ended by the
watchdog; a loop inside a host routine is not interrupted; a handler is
not interrupted by a second vertical blank; values kept in every
register, in the flags and in the floating-point registers survive many
interruptions; and with all eight floating-point registers of the
interrupted code occupied, a changed rounding mode in both control
words and the direction flag set, the handler finds the default state,
computes rightly with both floating-point units, and the interrupted
code gets its eight values, its control words and its flag back. Its cases for the disc layer in the linked program: the ready handler
at an address that the program did not install is refused, one that is
a function without C ends with the named stop, a valid one runs and no
vertical blank arrives inside it; `Exec` ends with the named stop, and
with `--skip-programs` prints its line and goes on; a sector copy to an
address outside the RAM ends the run. `python3 port/tools/test_hostcd.py`
builds the disc layer with the host's own `cc` and drives it by a
script on images of invented bytes: sectors in order and a fixed
number per frame, a sector handed out in pieces, pause and a new
position in the middle of a read, the end of the image, the position
conversions at their borders, the poll without a handler, audio
sectors dropped, the stops for a command that is not served, and each
way of giving `CdGetSector` a buffer that is not inside the RAM.
`python3 port/tools/test_hostmodules.py --cc CC` builds the runtime
with made-up tables and runs it on invented discs whose archives hold
made-up modules: a module is placed at its first call and its C runs; a
second module loaded over it is placed in its turn; a chunk whose
content is not the pinned one, an image without a pin and memory
changed after the load are refused; archives with too many chunks, a
length that wraps, a chunk past the file, no chunk, or less than a
header end with their line; a module that begins in the middle of a
page is placed; of two modules that share a page the second one's call
is refused; a call at bytes that the disc did not write ends with the
crash line; the disc's data in the page where resident code begins
leaves that code running, and a copy over a resident function's start
ends the program; a module function handed over as a handler is placed
and runs, and an address inside a module that is no function start is
refused; a second placement runs its own C; a module whose first sector
only is loaded refuses a later entry of its page as a callback, and so
does a copy of four bytes of an entry, each also for an entry without
C; a game or disc write over an installed entry or its page is
refused when the entry is next used; a module function whose C reads and
writes the low view below and above 64 KB runs after its placement, a
call at a non-entry while the low view is in use is refused as before,
an execute fault at a low address stays the mirror's crash line, a read
outside the mirror's range is swallowed by neither handler, and two
thousand loads and first calls in a row, alone and mixed with accesses
to the low view, with the timer running, end without a hang; every
program that the layer ends is run with and without `--timer-burst` and
must end the same. A failing case prints the whole output of its last
program run. No case runs a program twice to get a pass.
`python3 port/tools/test_hostgpu.py --cc CC --psyz-build DIR` needs a
built PsyZ and a way to open a window: it links the graphics layer
with a small program of its own and reads the frame buffer back. Its
cases: each routine's value; a picture checked pixel by pixel; the
first list of a program drawn right; every primitive kind the walker
knows at the length it needs, at 255 words and one word short; lists
that leave the RAM, loop or run past its end; ordering tables from 30
entries to all of RAM; rectangles at the frame buffer's edges and one
pixel past each; pixel buffers at the end of the RAM; a call before
`ResetGraph(0)`, and `SetDispMask` before it; packets with an incomplete
command in each position, after a command without effect and after each
setting word; a kind that is not decoded; structures on the stack of
the first task and of another one, on a dead part of the stack, on the
heap and in the program's own data; a display that cannot be opened;
the dump onto a folder and onto a path that cannot be made; a program
that outlasts its time, which is gone when the run goes on, while the
same program started as another run would start it lives on. The cases
run on SDL's offscreen video driver: PsyZ and its device work, the
picture is read back, and no window is made. Nothing is stopped by
name: two runs of this file on one machine do not touch each other's
programs.
`python3 port/tools/test_hostpads.py --cc CC --psyz-build DIR` builds the
runtime twice, without PsyZ and with `input.c` compiled for it and a
stand-in object that defines `PadInit`, `Psyz_PadsPoll` and
`Psyz_PadsGet` (it counts the calls, makes the frame depend on the number
of polls, and like PsyZ's polls once by itself when no poll came before);
no case starts PsyZ's window or reads a device. Made-up game code records
port 1's buffer in a vertical-blank handler and prints it. Its cases:
nothing is written before `StartPAD` and the buffer is written at every
vertical blank after it, with the poll once per blank and before the
fetch, the fetch for port 1 only, and port 2 filled with 0xff; every
length from 0 to beyond the frame's size, for both buffers, in both
builds; a buffer in the low mirror, above the RAM, ending past the RAM
or the scratchpad, a negative length and the smallest one, each for
port 1 and port 2, ending the run before a write, and a buffer ending at
the last byte, a length of 0 with a wild address and null buffers served;
the trace line, also to 0 for "no controller"; a script pressing and
releasing at the exact frames on top of the stand-in's buttons and in
the build without PsyZ, with a `repeat` line and its end, with the steps
of one frame in order; the script on a frame that says "no controller";
each kind of malformed line, an empty file, a file of comments, a file
that is too large, one that is missing and a folder; the options that go
with `--input`; and the line of keys, printed only with PsyZ and equal to
the line made from PsyZ's source. The game cases run with the timer on,
with `--no-interrupt` and with `--timer-burst`, which must print none of
its lines. Without `--psyz-build` the case for the keys line is skipped
with a line that says so.
`test_hostrun.py` also reads file tables made to break the reader: a
record that runs past its sector, a name past its record, a directory
extent beyond the image, a folder too large; a file that starts beyond
the image, one of 4,294,967,295 bytes, one whose sectors wrap 32 bits,
one that is a byte longer than the image holds and one that ends with
the image's last byte, an image cut inside the last file and one cut
right behind its last byte; a directory that declares only its own two
records, and a record that crosses or only begins inside the declared
end, for the reader that lists and for the one that looks a file up. On
2026-10-09 each of the five ended with
`all cases behaved as required`.
`python3 port/tools/test_hostmirror.py --cc CC` has the cases for the
copy of RAM at address 0, in two parts. The first needs no Windows
program: it builds the layer's decisions with the host's own `cc` and
tests them alone: whether a fault is served, at the borders `0xffff`,
`0x10000`, `0x10fff`, `0x11000`, `0x1fffff` and `0x200000` and across
them; the start check on invented answers of the system, over the whole
range and with the header page let through; the image's filler read
from invented headers; the decoder on every served form in every
addressing shape, and on encodings it must refuse; and each served
operation against the processor itself, register by register and flag by
flag. The second part runs the linked program on made-up game code: a
served read and write of each size; at `0x11000`, `0x3e0cc` and the last
word `0x1ffffc`, and the sum of two upper-view addresses that wraps; a
destination that is also the address's register; the read at a null
pointer plus `0xd`; an access across `0x200000`, a write at it, one from
a host routine and an execute fault at a low address, each the crash
line; a read of the header page and a write to it; a form that is not
served, the line with its bytes; a second fault inside the handler; the
first-use line and the trace lines; the start check, and a program
linked the old way, which refuses; a walk of 2,000 nodes through
24-bit links equal to the same walk on real addresses; and a loop of
served accesses with the timer running, in which the timer sometimes
aims the thread between a fault and its handler. It prints the cost of a
served access and the time of the walk. `test_hostbuild.py` also checks
the linked file's header against the link settings, on invented headers
and through each setting a linker might not honour. On 2026-10-10 the
file ended with `all cases behaved as required` and had printed
`timing: 10000 served reads took 75730 us (7.57 us each)` and
`timing walk: real addresses 3 us, low view 60587 us`.

## Not decided

- How game units reach the library. They call it by address names, such as
  `func_80157fc4`, and PsyZ has the library's own names. The list of
  needs has the pairs for the names the project knows. The host program
  stops at the first such call.
- What stands behind the data names that only the symbol file places.
  They are most of what a link would ask for, and it is the question of
  stored addresses and of game data as C from the other side.
- What supplies the five functions that PsyZ has as a stub, for one
  compiler only, or not at all: written into PsyZ and offered to its
  authors, or kept beside it here.
- How the 23 unnamed functions get names, and whether the modules call
  library functions that the resident code does not.
- Where the build of the host program is checked on a runner, and the
  same build for Linux and for macOS.
- What the port's build does about the units that a clang refuses, 35 to 115 by its version:
  compiler options that turn those errors back into warnings, or casts in
  the source, if the matching work finds that they leave the bytes alone.
- How the 75 structs with a pointer keep the layout that the game's data
  has. It is the question of stored addresses above, now with a count.
