# Port

Groundwork for a port of the game to macOS on Apple Silicon, Windows and
Linux. **Nothing of the game links or runs on any of them.** Its C units
compile on one of them, Linux, in a trial that is described below. This folder
holds the decisions taken so far, one pinned dependency, one check of that
dependency, one comparison of it with what the game calls, and one trial
that compiles the game's C with a PC compiler. It changes nothing under
`ps1/`.

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

```sh
python3 port/tools/hostcheck.py [--cc CC] [--std STD] [--build DIR] [--jobs N] [--timeout SECONDS]
```

It compiles every C unit of the build configuration that is not Sony's
library with the C compiler of the machine it runs on, each unit alone,
into `port/build/`. Nothing is linked, nothing is run and no game file is
read. It also checks the layout of every shared struct with that compiler
and counts the literals at PS1 addresses in the sources. The header of the
script says how. The counts move with every unit that the matching work
adds: run it for today's. On 2026-10-08, on one Linux machine, x86-64:

```
compiler: cc (GCC) 16.2.1 20260810
language level: gnu89
pointer size: 8 bytes
units: 2002 compiled, 129 under sdk/ and 8 not C left out
passed: 2002
failed: 0
warning -Wpointer-to-int-cast: 203 in 114 units
warning -Wincompatible-pointer-types: 182 in 89 units
warning -Wint-to-pointer-cast: 158 in 85 units
warning -Wint-conversion: 57 in 27 units
warning (no option): 12 in 10 units
structs: 155 of 219 keep their layout
structs with a pointer: 64, of which 64 lose their layout
structs without a pointer: 155, of which 0 lose their layout
fixed addresses: 280 literals in 150 units
main memory: 222 literals in 123 units
main memory, uncached: 0 literals in 0 units
scratchpad: 58 literals in 30 units
ports: 0 literals in 0 units
BIOS: 0 literals in 0 units
```

and with `--cc clang`, without its last line, which names the 111 units:

```
compiler: clang version 23.1.1
language level: gnu89
pointer size: 8 bytes
units: 2002 compiled, 129 under sdk/ and 8 not C left out
passed: 1891
failed: 111
warning -Wdeprecated-non-prototype: 591 in 334 units
warning -Wpointer-to-int-cast: 202 in 114 units
error -Wincompatible-pointer-types: 182 in 89 units
warning -Wint-to-pointer-cast: 156 in 84 units
error -Wint-conversion: 57 in 27 units
warning -Wreturn-type: 9 in 9 units
error -Wreturn-mismatch: 7 in 4 units
warning -Wunsequenced: 4 in 4 units
warning -Wparentheses: 3 in 3 units
warning -Wpointer-sign: 3 in 2 units
warning -Wint-to-void-pointer-cast: 2 in 1 units
warning -Warray-bounds: 1 in 1 units
warning -Wvoid-pointer-to-int-cast: 1 in 1 units
structs: 155 of 219 keep their layout
structs with a pointer: 64, of which 64 lose their layout
structs without a pointer: 155, of which 0 lose their layout
fixed addresses: 280 literals in 150 units
main memory: 222 literals in 123 units
main memory, uncached: 0 literals in 0 units
scratchpad: 58 literals in 30 units
ports: 0 literals in 0 units
BIOS: 0 literals in 0 units
```

What that says:

- The source is C that a compiler of today reads: with GCC and the old
  language level, every one of the 2,002 units compiles for a machine with
  eight-byte pointers.
- clang refuses 111 of them, for three things: a pointer of one type
  handed over where another is declared and an integer and a pointer mixed
  without a cast, which GCC warns about there, and a `return` that does
  not fit the function's type, which GCC lets pass at this language level.
  This is clang as Linux has it, not Apple's build of it.
- 64 of the 219 shared structs lose their layout, and they are exactly
  the 64 that hold a pointer. No struct loses it for another reason. Code
  and data that count on these offsets cannot use these structs as they
  are.
- 203 casts turn a pointer into an integer of another size and 158 turn
  such an integer into a pointer. An address does not survive the first
  kind on this host.
- 280 literals are PS1 addresses by their value: 222 in main memory and
  58 in the scratchpad.

What it does not say:

- That code which compiles would work. A unit without a diagnostic can
  still count on the PS1 in ways no compiler sees.
- What the units need from each other, from the library and from data
  that only the symbol file places. Nothing is linked. That is the next
  measurement.
- Anything about Microsoft's compiler, which has no such language level,
  or about macOS and Windows as hosts. One machine ran this.
- That a literal in a range is an address: `0x80000000` is also the sign
  bit of a word.
- The same counts from another version of a compiler: the kinds are the
  names that this version gives its diagnostics.

`python3 port/tools/test_hostcheck.py` runs the tool on small made-up
trees, part of them with a stand-in for the compiler and part with the
real one. It needs `cc` and a host with eight-byte pointers and says so
when it has neither. On 2026-10-08 it printed 89 lines that begin
`ok` and ended with `all cases behaved as required`.

## Not decided

- How game units reach the library. They call it by address names, such as
  `func_80157fc4`, and PsyZ has the library's own names.
- What supplies the five functions that PsyZ has as a stub, for one
  compiler only, or not at all: written into PsyZ and offered to its
  authors, or kept beside it here.
- How the 23 unnamed functions get names, and whether the modules call
  library functions that the resident code does not.
- The build of the game side for a host, and where it is checked on all
  three systems.
- What the port's build does about the 111 units that clang refuses:
  compiler options that turn those errors back into warnings, or casts in
  the source, if the matching work finds that they leave the bytes alone.
- How the 64 structs with a pointer keep the layout that the game's data
  has. It is the question of stored addresses above, now with a count.
