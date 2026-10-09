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
python3 port/tools/hostbuild.py [--config ps1/src/build.toml] [--cc CC] [--build DIR] [--jobs N] [--list]
```

CC is a C compiler for 32-bit Windows, by default `i686-w64-mingw32-gcc`.
The header of the tool is its contract. It compiles the C units of the
build configuration that are not Sony's library and every function of the
folders `ps1/src/*_nonmatching/`, and links them with the runtime of
`port/src/`. It reads no game file. Its output on 2026-10-09, for `ps1/` as
it is in commit `bfaf349`:

```
compiler: i686-w64-mingw32-gcc (GCC) 16.2.0
units: 3731 compiled, 1 of them nonmatching, 0 failed
like images built: 22
functions with C: 12460
functions without C: 622, library 382, game and modules 240
names at PS1 addresses: 45853
data defined in C, at host addresses: 0
linked: port/build/host/sfa2.exe, verified
```

The functions and names of the second placements are in these counts,
each under its own name.

For another tree, give the tool that tree's `build.toml` with `--config`,
as the scripts above take it.

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
jumps: 1404 written for functions with C, 448 for functions without
library: 73 host routines, 309 left that stop
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
library: 73 host routines, 309 left that stop
```

of which the listing gives 30 as routines that do something and 43 as
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
  a critical section. `--no-interrupt` turns the timer off. The cost,
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

Not in this piece: the disc's library, the graphics, the pads, the
modules. Their functions are among the 309 that stop.

### What this does not show

- That the game runs, draws or sounds. No function of the game has been
  executed on a PC by the published tree: the program stops before the
  first one.
- That C which compiles and links behaves on a PC as it does on the PS1.
- Anything about the modules: their jumps are not written. The 22
  images that are a second placement of another image's units are built
  and linked, and nothing of them has run.
- Anything about the library on the real game: its host routines have
  run only on the made-up game code of the controls, because the
  published tree stops at `main` before any of them. PsyZ is not linked.
  The build tool takes a build of PsyZ (`--psyz`) and writes each
  image's archive names into its tables, and its header names `gpu.c`,
  `modules.c` and `psyzbuild.py` for them: those files are not in this
  tree yet.
- Linux and macOS: the memory mapping is written for Windows only.

### Controls

`python3 port/tools/test_hostbuild.py` checks the tool on made-up
assembly, tables and symbol lists, without a compiler: the renaming of
definitions, the names, the tables, the choice of units, the second
placements, the markers, and each miss of the link check. `python3 port/tools/test_hostrun.py` builds the disc
reading and program loading of the runtime with the host's own `cc` and
runs them on disc images that the test makes from invented bytes, with
the hash of the program and the gate at the entry among them.
`python3 port/tools/test_hostlaunch.py --cc CC` needs the cross compiler
and a way to start a Windows program: it builds the runtime with small
made-up tables and runs the real start of the program on invented disc
images. Its cases: the right image stops at a function without C; an
entry with C runs that C; an image that differs in one byte from the
pinned one is refused and nothing of it runs; an entry at an address
that no table holds, inside a function, just before one, or in a module
is refused. Its cases for the library, all on made-up game code: a
library function with a host routine is reached and one without still
stops; an override replaces a function's C; a table that names a
function twice, an unknown one or an unknown address is refused, and so
is an override of a function without C; the trace has each library
call; a read of the PS1's low copy of RAM ends the program with a line
that says so; a thread entry, an event handler, an interrupt callback
and a vertical-blank callback at an address that the program did not
install are each refused before the call, and the byte there does not
run, while the same four paths with a function without C as the target
end with its named stop and with a handler inside the game's own code
run it; a handler runs once per vertical
blank, is held back inside a critical section and delivered once after
it; an event that was closed is not called; three tasks run in the
order the game switches them, and a task function that returns ends
the program; the card routines answer with the time-out event. Its
cases for the interruption: made-up game code that spins on a counter
which only its handler raises ends by itself, and the handler ran on the
game's thread; the same code with `--no-interrupt` is ended by the
watchdog; a loop inside a host routine is not interrupted; a handler is
not interrupted by a second vertical blank; values kept in every
register, in the flags and in the floating-point registers survive many
interruptions; and with all eight floating-point registers of the
interrupted code occupied, a changed rounding mode in both control
words and the direction flag set, the handler finds the default state,
computes rightly with both floating-point units, and the interrupted
code gets its eight values, its control words and its flag back. On
2026-10-09 each of the three ended with
`all cases behaved as required`.

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
- How the modules' jumps are written when the game loads a module.
- What the port's build does about the units that a clang refuses, 35 to 115 by its version:
  compiler options that turn those errors back into warnings, or casts in
  the source, if the matching work finds that they leave the bytes alone.
- How the 75 structs with a pointer keep the layout that the game's data
  has. It is the question of stored addresses above, now with a count.
