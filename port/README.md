# Port

Groundwork for a port of the game to macOS on Apple Silicon, Windows and
Linux. **Nothing of the game links or runs on any of them.** Its C units
compile on one of them, Linux, in a trial that is described below. This folder
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

A direction, not a ruling. Nothing is built on it:

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
port/tools/check_psyz.sh [--headless] [BUILD_DIR]
```

It fetches the submodule and the SDL source that PsyZ builds against, builds
PsyZ's own test suite and runs it one area at a time. It needs git, a C and
a C++ compiler, CMake 3.21 or later and Ninja; the last two can come from
`pip install cmake ninja`. `--headless` is for a Linux machine
without the development files for windows, graphics and sound. `BUILD_DIR`
is taken from the folder the script is called from; without it the build
goes to `port/build/psyz-tests`, which Git ignores.

What ran, on 2026-10-07, on one Linux machine of that kind, x86-64, with
`--headless`: the library and its tests build, and the script printed

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

and ended with status 1, as it does when any area fails. The three areas
that compare drawn pictures mostly fail there: the pictures that were
looked at came out black, on a machine that has nothing to draw with. Ten
tests of timer events fail too, and why was not looked into.

What this shows: PsyZ builds from the pin on one Linux machine, and its
tests of the disc, the sound chip, the memory card and the arithmetic pass
there. What it does not show: a window, a drawn picture, sound from a
speaker, macOS, Windows, or anything about the game. With the real suite
the script has not been seen to end with status 0 anywhere.

```sh
port/tools/check_psyz_controls.sh
```

runs the script against stand-ins for git, CMake and the test program, so
nothing is fetched or compiled: a fresh checkout with the default folder, a
relative and an absolute folder, `--headless`, and each of the statuses 0,
1 and 2. Its last line on 2026-10-07 was `controls: 15 of 15 as expected`.
It says nothing about PsyZ.

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
python3 port/tools/hostcheck.py [--cc CC] [--std STD] [--build DIR] [--jobs N] [--timeout SECONDS]
python3 port/tools/hostneeds.py [--build DIR] [--out FILE]
```

The first compiles every C unit of the build configuration that is not
Sony's library with the C compiler of the machine it runs on, each unit
alone, into `port/build/`. It also checks the layout of every shared struct
with that compiler and counts the literals at PS1 addresses in the
sources. The second reads the objects that the first left and sorts every
name that some object needs and none defines: what a linker would ask for.

The counts move with every unit that the matching work adds, so the ones
here are for one tree: `ps1/` as it is in commit `a4b6481`. For today's, run
the scripts as they are. For these, give them that tree:

```sh
mkdir /tmp/tree && git archive a4b6481 ps1/src ps1/inventory | tar -x -C /tmp/tree
python3 port/tools/hostcheck.py --config /tmp/tree/ps1/src/build.toml
python3 port/tools/hostneeds.py --config /tmp/tree/ps1/src/build.toml \
    --symbols /tmp/tree/ps1/src/symbols.ld --inventory /tmp/tree/ps1/inventory
```

All of it ran on 2026-10-09 on one Linux machine, x86-64.

### What compiles

```
compiler: cc (GCC) 16.2.1 20260810
language level: gnu89
pointer size: 8 bytes
units: 2088 compiled, 129 under sdk/ and 8 not C left out
passed: 2088
failed: 0
warning -Wpointer-to-int-cast: 213 in 121 units
warning -Wincompatible-pointer-types: 182 in 89 units
warning -Wint-to-pointer-cast: 170 in 95 units
warning -Wint-conversion: 57 in 27 units
warning (no option): 13 in 11 units
structs: 158 of 225 keep their layout
structs with a pointer: 67, of which 67 lose their layout
structs without a pointer: 158, of which 0 lose their layout
fixed addresses: 304 literals in 160 units
main memory: 246 literals in 133 units
main memory, uncached: 0 literals in 0 units
scratchpad: 58 literals in 30 units
ports: 0 literals in 0 units
BIOS: 0 literals in 0 units
```

and with `--cc clang`, without its last line, which names the 112 units:

```
compiler: clang version 23.1.1
language level: gnu89
pointer size: 8 bytes
units: 2088 compiled, 129 under sdk/ and 8 not C left out
passed: 1976
failed: 112
warning -Wdeprecated-non-prototype: 612 in 353 units
warning -Wpointer-to-int-cast: 211 in 120 units
error -Wincompatible-pointer-types: 182 in 89 units
warning -Wint-to-pointer-cast: 168 in 94 units
error -Wint-conversion: 57 in 27 units
warning -Wreturn-type: 9 in 9 units
error -Wreturn-mismatch: 8 in 5 units
warning -Wunsequenced: 4 in 4 units
warning -Wparentheses: 3 in 3 units
warning -Wpointer-sign: 3 in 2 units
warning -Wint-to-void-pointer-cast: 2 in 1 units
warning -Wvoid-pointer-to-int-cast: 2 in 2 units
warning -Warray-bounds: 1 in 1 units
structs: 158 of 225 keep their layout
structs with a pointer: 67, of which 67 lose their layout
structs without a pointer: 158, of which 0 lose their layout
fixed addresses: 304 literals in 160 units
main memory: 246 literals in 133 units
main memory, uncached: 0 literals in 0 units
scratchpad: 58 literals in 30 units
ports: 0 literals in 0 units
BIOS: 0 literals in 0 units
```

- The source is C that a compiler of today reads: with GCC and the old
  language level, every one of the 2,088 units compiles for a machine with
  eight-byte pointers.
- clang refuses 112 of them, for three things: a pointer of one type
  handed over where another is declared and an integer and a pointer mixed
  without a cast, which GCC warns about there, and a `return` that does
  not fit the function's type, which GCC lets pass at this language level.
  This is clang as Linux has it, not Apple's build of it.
- 67 of the 225 shared structs lose their layout, and they are exactly
  the 67 that hold a pointer. No struct loses it for another reason. Code
  and data that count on these offsets cannot use these structs as they
  are.
- 213 casts turn a pointer into an integer of another size and 170 turn
  such an integer into a pointer. An address does not survive the first
  kind on this host.
- 304 literals are PS1 addresses by their value: 246 in main memory and
  58 in the scratchpad.

### What the objects need

From the objects of the GCC run:

```
objects: 2088 of 2088 units
defined: 6180 names
needed: 3334 names
library by name: 10 names, needed by 11 units
assembly: 9 names, needed by 6 units
unit not compiled: 0 names, needed by 0 units
unknown: 2 names, needed by 150 units
C under another name: 2 names, needed by 2 units
library by address: 83 names, needed by 134 units
game function: 53 names, needed by 122 units
module function: 121 names, needed by 79 units
function elsewhere: 38 names, needed by 10 units
scratchpad data: 71 names, needed by 45 units
module data: 2172 names, needed by 956 units
resident data: 773 names, needed by 1359 units
library by address: 76 named, 7 unnamed
unknown: _GLOBAL_OFFSET_TABLE_ __stack_chk_fail
```

- Data is nearly all of it: 2,172 names in the modules, 773 in the
  resident executable and 71 in the scratchpad that only the symbol file
  places. No C defines them. On the PS1 they are addresses that the
  linker is told; on another machine each needs something behind it.
- The library is asked for by address far more often than by name: 83
  names against 10. For 76 of the 83 the project knows the library's
  own name, which is the list a port needs to call PsyZ; 7 have none
  yet. `--out` writes the pairs. 9 more names are functions that the
  build has as assembly, stubs for calls into the BIOS among them.
- 53 functions of the resident executable and 121 of the modules are
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
- The needs of the units that clang refuses: from clang's objects the
  list is another, with a class of its own for what those units would
  have defined.
- Anything about Microsoft's compiler, which has no such language level,
  or about macOS and Windows as hosts. One machine ran this.
- That a literal in a range is an address: `0x80000000` is also the sign
  bit of a word.
- The same counts from another version of a compiler: the kinds are the
  names that this version gives its diagnostics.

`python3 port/tools/test_hostcheck.py` and
`python3 port/tools/test_hostneeds.py` run the two scripts on small
made-up trees. The first needs `cc` and a host with eight-byte pointers
and says so when it has neither; the second uses a stand-in for `nm` and
needs neither. On 2026-10-09 they printed 90 and 142 lines that begin
`ok` and each ended with `all cases behaved as required`.

## Not decided

- How game units reach the library. They call it by address names, such as
  `func_80157fc4`, and PsyZ has the library's own names. The list of
  needs has the pairs for the names the project knows.
- What stands behind the data names that only the symbol file places.
  They are most of what a link would ask for, and it is the question of
  stored addresses and of game data as C from the other side.
- What supplies the five functions that PsyZ has as a stub, for one
  compiler only, or not at all: written into PsyZ and offered to its
  authors, or kept beside it here.
- How the 23 unnamed functions get names, and whether the modules call
  library functions that the resident code does not.
- The build of the game side for a host, and where it is checked on all
  three systems.
- What the port's build does about the 112 units that clang refuses:
  compiler options that turn those errors back into warnings, or casts in
  the source, if the matching work finds that they leave the bytes alone.
- How the 67 structs with a pointer keep the layout that the game's data
  has. It is the question of stored addresses above, now with a count.
