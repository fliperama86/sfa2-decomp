# Nonmatching C

C for functions of the PS1 game that compiles with the pinned toolchain but
does not reproduce the original's bytes. Each file here is separate from the
exact owner of those bytes: the PS1 build does not use any file of this
folder, and nothing in `build.toml`, `symbols.ld` or `types.fields` refers
to it. The raw bytes stay what the matching build owns.

A function of this folder that is later rebuilt exactly moves to the folder
of its image, enters the build there, and leaves this one.

## What "nonmatching" means here

A function here is NOT byte-identical to the original: its build differs
from the original's bytes, and mostly in size too (the lines under
"Functions" give both sizes). What these files claim is narrower: on the
inputs of the function's contract, the build and the original code leave
the same final state.

## The test

`difftest.py` builds the function with the matching build's steps (the same
preprocessing, compiler, assembler wrapper and flags, `-O2 -G0`), links the
object alone at a test address outside the console's RAM with the tree's
`symbols.ld`, and runs it in the Unicorn emulator (MIPS32, little-endian)
next to the original code at its original address. The memory holds the
resident executable and the function's module image, read through the build
configuration from the private inputs it names (they are not in Git; the tool
stops with a message when they are absent).

For each case the function's contract writes a seeded random input state
that it allows. A contract is code: an entry of `contracts.py`, or the file
`FUNC.py` beside `FUNC.c`. Both codes run from identical copies of it with
the same argument registers. After both stop it compares:

- the return register, when the contract says the function returns a value;
- the callee-saved registers and the stack pointer, set to known values
  before the call;
- all of RAM and the scratchpad, byte for byte, except the stack region (the
  frames differ). The order of stores inside a run is not compared.

A case is discarded, not failed, when the ORIGINAL code faults or exceeds its
instruction budget: the input was not one the original accepts, so it says
nothing about the build. A case where the original completes and the build
faults, exceeds the budget or ends in a different state counts as different.
Hardware addresses are not mapped, so a run that touches them faults.

A second line says how many instruction slots of the original function the
cases executed, and `--uncovered` lists the others by their offset. Cases
that never reach a part of the function say nothing about that part. The
header comment of a `.c` file names the slots that no input reaches.

A function may call one that cannot run in the test, because it reaches
Sony's library and through it the hardware, or one whose own state the
contract does not build. The contract then puts a recorder in the callee's
place (`CallLog` in `contracts.py`), in both runs alike: it appends the
callee's address and its declared arguments to a log in RAM and returns a
value that the contract chose. The log is part of the compared RAM, so a
call that is missing, out of order or made with another argument is a
difference. What the callee itself would have done is then outside the
test, and the header comment of the `.c` file says which callees are
replaced.

A recorder returns at once, so by itself it shows only that a call was made
and with which arguments. Two things it then misses, and each let wrong C
pass here before it was closed. One is memory behind a pointer argument
that the function fills for that call: a recorder can copy those words into
its entry (`pointees`). The other is the order of a store against a call: C
that stores a field after a call which the original stores before it ends
in the same state. So every recorder also copies the blocks that the log
watches (`watch`), which is what the function has written there by the time
of the call. A header says what is watched. What a real callee would have
changed in memory, and what the function does with that, stays outside the
test unless the header says that a recorder stands for it.

For a function that waits or never returns, a recorder can give its results
in turn (`results`), store a word at its N-th call or count a word up at
every call (`stores`, `counts`: what an interrupt does to memory while the
function runs), and end the run at its N-th call (`ends_run_at`). The number
of a call that a contract names is at most 32767 (the stand-in loads it as a
16-bit number; a larger one is refused, it would be loaded as another). A run
that was ended is cut inside the function: its registers are then not
compared, its log and memory are.

Two more things a contract can say about a callee. An argument can be
logged under a mask (`masks`), for a callee that reads only a byte or a
halfword of it while the two codes extend it differently; a mask of 0 logs
nothing of its value, for the address of a local, which the two codes place
differently. And a contract can give a recorder a few instructions of its
own to run in place of its return (`tail`), for a callee that fills a
buffer the caller hands it or returns that buffer's address: that is a
model written by the contract's author, and the function's header says
what it models and from what that is known.

A callee that only stores given bytes through one of its pointer arguments
needs no machine code: `fills` says it as data, `(call, argument, offset,
data)` for each fill. At the call (0: every call; N: the N-th only, N up to
32767) the
recorder stores `data` (1 to 256 bytes) at the value of that argument plus
`offset` (0 to 0x7fff), after the entry is logged and after `stores` and
`counts`, before the run is ended or the result returned. The address is the
argument's value at the call, so it works where the argument is the address of
a local, which the two codes place differently and which no contract can name
(give the argument the mask 0). Its limits: the argument is one of the four
register arguments; the data is chosen by the contract and is not computed from
the call (a callee that copies from one argument to another, or returns an
argument, still needs a `tail`); and `fills` and `tail` cannot be used together.
A fixture holds the fills, so a function with them has a record and a replay.

A pointee behind an argument masked to 0 (the address of a local of the function, not in the log) is recorded
too, when the whole block lies in the stack region of a case. The record reads the argument's real value at the
stand-in's entry; the block is described against zeros (the region starts as zeros and is not input). If any
byte of the block lies outside the region (a global, memory of the game, a block that straddles an end), the
function is refused with a line naming the callee, the argument and the real address, and no file is written.
The replay compares the block against zeros and also looks at the build's real pointer at the entry: a build
that hands the callee a pointer outside the stack region fails (`argument K points to ADDR, and its N bytes are
not wholly inside the stack region`). The file needs no new field: `masks` {i: 0} with `pointees` {i: n} says
it. The native replay does not replay such a function.

## What a pass does not show

A pass is evidence for the tested inputs only. It is not a proof of
equivalence. Inputs outside the contract, rare combinations that random
generation does not reach, timing, and the order of memory stores are not
covered. The contract is in the header comment of each `.c` file and in
`contracts.py`.

A pass also says nothing about where the two codes wrote: it compares
them with each other. If a setup lets the original run past the end of
a table, both codes run past it alike and the test says "equal"; if a
setup leaves a table at the image's zeros, a C that clears a field
which it must keep passes. `--writes` looks at that. It runs the
original alone on the cases of the seed and counts the cases in which
the original changed a byte of RAM or of the scratchpad that the setup
did not make: a byte that the setup neither wrote, nor got as part of a
block it allocated, nor marked as the function's to write. The
function's own memory is not counted: the stack, and the 16 bytes from
the initial stack pointer upward, where the calling convention lets a
function keep its arguments. A count above 0 means that the function
runs past a table, or writes a table or a global that the setup did not
make. What this audit cannot see: a store of the value that is already
there; reads; and whether memory that the setup made holds varied
content or was only touched. It runs the cases of the given seed only.

## Negative control

`--control` alters one instruction of the build that the contract's inputs
make the function execute, then requires the test to report differences. A
control that reports none exits with status 1. A control by hand: change one
constant in a copy of the `.c` and the test reports differences.

## Edges of the constants

Coverage of instruction slots does not show that the edge of a comparison was
tried: a deliberate error in the C of one function (`0x59` in place of `0x58`
in a reach) passed 2,000 cases although every slot of the original was
executed, because random inputs rarely land on the one value where the two
differ. `--edges` looks for such constants. It alters the ORIGINAL code, one
constant at a time, and leaves the build alone (`--control` is the other way
round: it alters one word of the build).

    python difftest.py --config ../build.toml --cases 2000 --edges --uncovered [--jobs N] FUNC

It is a tool for the day a function is written or its setup changes: run it
for that one function, and keep the cases that notice an edge. It is not a
check to repeat on other changes (see
[efficient checks](../../../docs/efficiency.md)).

For each function the default comparison runs first. If it shows a difference
or a discarded case, the function is not swept: one line says so and the
status is 1. Otherwise each constant of the original gets two altered runs,
its immediate plus 1 and minus 1 (taken modulo 0x10000), each the default
comparison with the same cases, the same seed and the same build, with that
one word of the original replaced in the memory image of every case. A word is
a constant of the sweep when it is:

- `slti` or `sltiu`;
- `addiu rt,rs,imm` with neither register `sp` or `gp`, and not the low half of
  an address or of a 32-bit constant (`rs == rt` and the nearest earlier word of
  the function that writes `rt` is `lui rt`);
- `ori rt,zero,imm`.

Nothing else is taken: no load or store offset, no `andi` or `xori` mask, no
shift amount, no branch, no `lui`. This list is a choice. The sweep finds the
edges of comparisons and of added limits, not every constant.

The first line per function is
`FUNC edges: constants C, altered runs R, unnoticed U, all discarded A`. A line
follows for every altered run that no case notices (`different 0`), in the
order of the slot, plus before minus; with `--uncovered`, a line follows for
each constant in a slot that no case executed (such a constant counts in C and
gets no run). A run ends at its first differing case, which means it is
noticed. A run in which every case is discarded (the altered original faults or
does not end) counts in A, not in U. The status is 0 when every U is 0 and 1
otherwise. `--jobs N` (default 8, only with `--edges`) sets how many processes
make the altered runs of a function; the output is the same for every N.

What an unnoticed line means: no case of this seed tells the constant from its
neighbour. That is a gap of the setup when the contract's inputs can reach the
edge, and it is no gap when they cannot (the edge lies in inputs the contract
excludes, or the constant has no effect on what the test compares). The tool
cannot tell the two apart; the author of the contract does, for each line. A
sweep with U at 0 is not equivalence either.

The block below is a record, not an instruction: what the mode printed for
this folder's functions on 2026-10-10 (seed 1, 2,000 cases, `--jobs 8`), in
one run of the whole folder with `--all` in place of FUNC, made once when
the mode was new. That run took 1079 seconds by the shell's clock, 352 of
them for func_801e8bd8_slot06_08, on a machine that other work was also
using. The lines are open: they are not yet read one by one, and no contract
was changed for them. A function's lines are replaced when the mode is run
for that function again.

```
```

## Fixtures

A fixture is one case of one function, in terms of what the function does: the
arguments, every byte the original read, the calls it made to the stand-ins of
its callees with what they were given and what they returned, every byte it
changed, and the result. `--record` runs the original once and writes a few of
them to `FUNC.fixtures.json` beside `FUNC.c`; `--replay` runs the BUILD of
`FUNC.c` on each of them and never runs the original.

    python difftest.py --config ../build.toml [--folder DIR] --record --cases 2000 [--seed S] [--max M] [--jobs N] FUNC
    python difftest.py --config ../build.toml [--folder DIR] --replay [FUNC]

`--record` first runs the default comparison (it must show `different 0` and
no discarded case; otherwise nothing is written and the status is 1). It then
keeps, until M (default 12) are kept: every case that executes a slot of the
original that no kept case executed; for each constant of the `--edges` sweep
and each direction, the first case that notices the alteration (the sweep
alters the ORIGINAL, as `--edges` does; `--control` is the other way round and
plays no part here); and, when the function has a result, the first case for
each result value no kept case has. Each fixture says why it was kept in its
`note`. The file also lists, under `uncovered`, the slots of the original that
no kept case executed and the constants whose edge no kept case could cover.
The line printed is
`FUNC fixtures: kept K of N cases (slots A, edges B, results C)`.
A file that exists is replaced and the tool says so. A line starting
`warning` says that the cases read bytes the setup did not make, which come
from the game's memory image and would be published with the file.

A callee that the contract does not replace with a recorder runs as the game's
own code when the original is recorded. `--record` records each call of such a
callee as an entry of `calls` (with `original` true): the registers of the
caller that it used while they still held the caller's value (`uses`: at, v0,
v1, a0 to a3, t0 to t9, gp, hi, lo; a save of a register into the callee's
own memory is no use, and each byte of the saved word is then a copy of the
register until a store writes that very byte; a load of the whole word back
into the same register while all four bytes are still its copies is a restore
and no use, and any other load of a byte that is still a copy is a use of the
register it is a copy of), the stack words it read through the
stack pointer (`stack`), the bytes it read that the setup made or the run had
written (`reads`), the bytes it changed (`effects`) and the registers it
wrote and did not restore (`leaves`). A fixture with such a call also has `callees`, the
addresses where replay puts the stand-ins, and the file is format 2; a file
without such a call stays format 1. At replay a stand-in is two words,
`jr ra; nop`, and a hook at that address: it checks the call's place among the
calls, the registers of `uses` (the whole register is compared), the stack
words and the bytes read, then writes the effects and sets exactly the registers
of `leaves`, and no other, so a register the callee never wrote keeps the
build's own value; so
the callee's code, and the recorders it called, are never run. The 16 bytes
from the call's stack pointer upward are the callee's own argument home area.
The tool refuses a case, with one line and status 1, when the callee writes
the stack at or above that area, reads it through a register other than sp (a
pointer to a local of the function, which the build cannot repeat), is entered
again before it ended, reads a register of its caller that is no argument (s0
to s7, fp, k0, k1, or ra other than by `jr ra`), leaves the stack pointer
changed, reads the stack pointer in any way but as the base of a load or a
store or to move it (it takes the address of its stack), loads a byte of its
own frame that it has not written during the call, or does not end when the
run does while the setup says the function returns; these are limits of the
tool. After choosing and before
writing, `--record` replays every kept fixture on the build, and a fixture that
fails is a refusal (`the replay of case N fails`). A byte that such a callee
reads and that the setup did not make is content of the game's image: it is
not kept as a read, and it is counted in the record's warning like a read of
the function's own, because what the callee makes of it (the bytes it stores,
the registers it leaves) is in the file. A file with that warning is not
published. `--edges` alters the original, as the altered runs of a record do;
`--control` alters the build and is no part of a record. For
the earlier refusal, a fixture has no `needs_image` field (an earlier draft
had one; no published file has it), and `--replay` refuses a file that has it,
with status 2.

`--replay` puts the memory of the case in place (poison bytes; a byte of
`writes` whose input value the fixture does not give starts as its expected
value with every bit flipped, never as the filler, so that a missing store shows
whatever its value), puts a stand-in at each callee that returns the recorded
values in order, runs the build, and requires the same calls in the same order
with the same logged values, the result, the written bytes exactly, and the
saved registers and stack pointer as the original left them. Every address
executed must lie in the build, in a stand-in, or be the stop address; a build
that calls an address without a stand-in fails. The line is
`FUNC replay: fixtures K, passed P, failed F`, followed for each failure by the
fixture and up to three lines saying what differed first. The status is 0 when
F is 0, 1 otherwise, 2 for a missing or malformed file. With no function named,
every function of the folder that has a fixtures file is replayed. The replay
does not run the original, never loads the game's image and uses no byte of the
game, but it starts like every mode: it loads the configuration, which reads the
baseline executable to check its hash, so it needs the private inputs of the
configuration, and it builds the C with the private PS1 compiler. A replay that
needs nothing private is a later piece (the C as compiled for a PC).

What a replay shows is narrow: for the recorded inputs, the C reads, calls and
writes what the original did. It is evidence for those inputs. It is not the
wide comparison (that is made once, by `--record` and the default mode, when
the function is written) and it is not equivalence. A fixture goes stale when
the C changes what it reads (a byte the fixture does not hold is poison), calls
or writes: the replay then fails, and nothing regenerates a file except a new
`--record`.

Choices of the tool, each of them a decision to review:

- `reads` holds what the function's own instructions read. What a stand-in copies into
  the log is not a read of the function: the words behind a pointer argument and
  the watched blocks are stored in each call as the bytes that differ, at that
  call, from the same memory in the case's input state (the memory before the
  function ran), as `[offset, hex]` spans, empty when nothing differs. In words:
  at this call these bytes of the block had been changed to these values. Each
  call reads alone; it is not a chain against the previous call. The replay
  rebuilds the block from its own input state (poison where the fixture says
  nothing) and the changes, and requires it to equal what the build's call
  shows, so a build that changes a watched byte the original had not changed by
  that call, or does not change one it had, fails. The stack region, the 16
  bytes above it and the harness's own memory are left out of reads and writes.
- `same` holds bytes the original stored without changing them and did not
  read; without them such a store would look like an extra write in the replay.
- The stand-ins' own options that stand for what a callee does to memory
  (`stores`, `counts`, `ends_run_at`, `masks`, `fills`) are part of the fixture; a
  stand-in with a `tail` (machine code of the contract's own) cannot be described
  and the record refuses it with status 2. A recorder's `fills` is a list of
  `[call, argument, offset, hex of the data]`; the file's format number does not
  change, and the reader refuses a `fills` that is not of that form, or out of
  range for the recorder, naming the fixture. The bytes a fill wrote are, for
  what the function reads afterwards, memory that something wrote earlier: they
  are not in `reads` and not "memory the setup did not make". Like the bytes a
  `stores` writes, they are written by the stand-in and not by the function, and
  are found by the same hook: they are in `writes` when they lie outside the
  function's stack region and change a byte, in the comparison and in the write
  audit as for any other byte of memory, and the replay's recorder makes them
  again. The native replay (`port/tools/hostfixtures.py`) does not replay a
  function whose fixtures have `fills` (the native harness's stand-ins cannot
  write through an argument yet) and says so.
- The reads and writes are found by looking at each instruction before it runs,
  not with the emulator's memory hooks: with a memory hook, Unicorn 2.1.4 ends a
  run with an exception when a load or store lies in the delay slot of a
  conditional branch that is not taken and a jump follows (`test_difftest.py`
  has made-up code that shows the difference).

The fixtures of four functions of `../resident_nonmatching` are the first.
Each block is what the commands printed (seed 1, `--record --cases 2000 --jobs 4`):

```
func_8012fd80 fixtures: kept 6 of 2000 cases (slots 2, edges 3, results 1)
func_8012fd80 replay: fixtures 6, passed 6, failed 0
func_80119694 fixtures: kept 4 of 2000 cases (slots 1, edges 3, results 0)
func_80119694 replay: fixtures 4, passed 4, failed 0
func_8011a880 fixtures: kept 7 of 2000 cases (slots 5, edges 2, results 0)
func_8011a880 replay: fixtures 7, passed 7, failed 0
func_801189c4 fixtures: kept 5 of 2000 cases (slots 4, edges 1, results 0)
func_801189c4 replay: fixtures 5, passed 5, failed 0
```

The files are 4765, 3526, 18237 and 71582 bytes. The records took 9, 6, 15 and
52 seconds by the shell's clock; each replay took 1 second by the same clock,
the compile of the C included. What they leave uncovered is in the files:
`uncovered.edges` holds altered runs that no case notices (each line gives the
number of discarded cases of that run), and `uncovered.slots` the slots
`+0x330..+0x348` of `func_801189c4`, which no case executes. A second record of
each function wrote the same bytes.

## Fixtures of this folder and the tool after recording them

A function that has fixtures has a file `FUNC.fixtures.json` beside its
`FUNC.c`. Replay runs the C of the function on the recorded inputs and never
runs the original code. It still needs the private inputs of the build
configuration and the PS1 compiler. From the tool's folder
(`ps1/src/slot06_nonmatching/`), for this folder and for one function:

    python -B difftest.py --config ../build.toml --replay
    python -B difftest.py --config ../build.toml --replay FUNC

For a new or changed function, record once (the wide comparison, then the
fixtures it keeps), then replay:

    python -B difftest.py --config ../build.toml --cases 2000 --jobs 4 --record FUNC

A pass is evidence for the recorded inputs. It is not the wide comparison and
it is not equivalence.

Recording every published function found four faults of the tool, which are
repaired. Replay keeps its call log clear of the watched blocks and of the
words behind pointer arguments that it compares. A run, in every mode and for
the original and the build alike, is ended at the call whose entry would not
fit in the room of the contract's call log, and a case that ends so is
discarded. Each altered run of `--edges` and of `--record` is made in a process
of its own, so that an altered original that ends the emulator is counted and
listed apart. An altered original has an instruction budget of its own, eight
times the longest run of the unaltered original (at least 20,000
instructions). `--edges` alters the original; `--control` alters the build.
`--record` also looks at the first three cases for what refuses a function
before it makes the altered runs, and it counts memory that a setup marked
with `State.owns` (made, with the image's content) among the bytes that a
file would publish.

What the replay command of the folder printed on 2026-10-10, one line per function:

```
```

## Controls of the tool

    python test_difftest.py

The controls of `difftest.py` need no private input, no toolchain and no
network: only Python, this repository and Unicorn. They print one line per
case, `ok NAME` or `FAIL NAME: why`, and end with the line
`all cases behaved as required` (status 0), or with a count of the cases that
behaved wrongly (status 1). Group A feeds the comparator made-up final states
(borders of the stack region, ends of RAM and scratchpad, each register). Group
B runs the emulator path on short made-up MIPS functions, written as
instruction words, and checks the equal, different and discarded decisions and
the seed. Group C runs `main` with stand-ins for the build and the run and
checks the exit status, the printed lines and the negative control's altered
word. Group D checks that every name that `difftest.py` takes from
`matchbuild.py` exists and that its declarations can be made as the tool makes
them. Group E checks the coverage figure (which instruction slots of the
original the completed runs executed, with branch arms, callees, discarded runs
and the helper functions that cut and print the offsets). Group F checks the
coverage line, `--uncovered`, `--folder` and contract files through `main`. Group G
checks `contracts.CallLog`: what a recorder logs for zero to six arguments,
the value it returns, that the callee's own code does not run, and that calls
missing, out of order or with another value are differences. Group N checks
pointees: the words behind a pointer argument (in a register or on the stack)
are logged after the arguments, so that a call made with other contents behind
the pointer is a difference. Group O checks watched blocks: they are copied
into every entry, so that a store made after a call instead of before it is a
difference. Group P checks the recorder's own footprint (which registers it
leaves alone; not a calling convention). Group Q checks masks: an argument is
logged under its mask. Group R checks results in turn: each call gets the next
result and the last one repeats. Group S checks a run that a recorder ends at
its N-th call: the call is logged, the store after it is not made, the run
counts as completed, and with `returns=False` registers are not compared while
the log and memory still are. Group T checks `differences` with `returns`
false. Group U checks `stores` and `counts`, which stand for what an interrupt
does: a word stored at the N-th call or counted up at every call, after the
entry is written. Group V checks a tail: it runs after the entry is written,
`result` and `results` have no effect with it, `stores`, `counts` and
`ends_run_at` act before it, an argument masked to 0 with its contents as a
pointee makes two builds equal that keep a local at different places of
their frames, and the encoders that the module exports match the test's own.
Group W checks that a mask of 0 logs 0 for the argument while other bad
masks are still refused. Group H checks that
code a setup writes into RAM runs as written in every case, not as the first
case wrote it (recorder results that change from case to case, and both arms of
a branch on one). Group I checks the build result: its size and entry reach the
printed line and the run, and the control sees only the code words and refuses
a word outside them; it also reads the section sizes of an object built in the
test, but not the toolchain's refusal of writable data or the unit's code size
from a real link, which need the toolchain. Group J checks the table of
addresses that a setup is given (linker symbols win over declared functions).
Group K checks `--all` (each `func_*.c` once in sorted order, and status 2 for
names with it, for neither, or for no source).
Group X checks the write audit: what counts as made by a setup (its
writes, its blocks, memory it marks, and the padding behind a block,
which is not made), that a negative size or a block that does not fit
is refused before anything changes and leaves the record of made
memory as it was, the stack and the argument area at their borders,
the scratchpad, cases that are discarded, the recorders' own log and
code, the address runs and names of the second line, the lines, status
and errors of `--writes` through `main` (no build is attempted), and
that the default mode prints what it printed before.
Group Y checks the edges sweep: which words are constants (each class, and each
exclusion: `sp`, `gp`, the `lui` pair, loads, stores, masks, shifts), the wrap
at both ends, the lines, a made-up function run under the emulator with a
noticed, an unnoticed, a never-executed and an all-discarded constant (same
lines for `--jobs 1`, 3 and 8), that a noticed run stops at its first
difference and a run without one does not, that the original is put back
between runs, the status, and the input errors.
Group L checks the symbol file of the standalone link: the lines that assign a
name the unit defines are removed (plain, spaced and inside `PROVIDE`), names
that only begin alike stay, and a link that puts a defined name outside the
unit is reported with the name and the address.
Group Z checks the fixtures on made-up functions: the reads found (not the
stack's, not the log's, and what a stand-in copies into the log), the
choice of cases (slots, edges, results, the cap, a case kept for two reasons,
`uncovered`, nothing written when the comparison fails), the file (the same
bytes for the same input, the keys, the line printed, malformed files), a
record followed by a replay that passes, each kind of failure of a replay (a
wrong result, a missing, extra or reordered call, a wrong argument or memory
behind a pointer, a missing, extra or wrong write, a clobbered saved register
or stack pointer, a read the fixture does not hold), a run that ends inside a
stand-in, the calls of callees that run as original code (what a call holds, the
stand-in and its checks, the refusals, format 2, the check of a record by its own
replay) and that a file with `needs_image` is not read, that a replay executes only the build and the
stand-ins (and fails for a build that calls an address without one), stores of
the filler value that cannot be missed, a
stand-in at an unnamed address with its pointer cell, the options of a
stand-in, that the replay never runs the original and reads no contract, the
input errors of the options, and the decoding of the call log by
`contracts.CallLog`. Group FL checks `fills`: a local at two places of two
frames is filled in both and a build that reads the neighbouring byte differs;
a fill at every call against one at the N-th; a pointee that shows the bytes
from before the fill; the later of two overlapping fills; an unaligned offset,
an odd length, 256 bytes and the largest offset; the order against `stores`,
`counts` and `ends_run_at`; the recorder's footprint; that a recorder without
fills has the words it had before fills existed (listed in the test); each
refusal by name; a fill through an argument that is not an address (a fault,
as any wild store: the original's case is discarded, the build's is `build:
fault (Invalid memory write (UC_ERR_WRITE_UNMAPPED))`); and the record (the
fills in the file, no warning, the filled bytes not in `reads`), the replay of
the right build and of builds that fail it, a file whose fill was changed by
hand, and the reader's refusals. Group SP checks a pointee behind an argument masked to 0: a local filled by the
function, recorded and replayed in two frames, a build that puts another value there, a pointer to a global
(refused with its line), a build whose pointer leaves the stack (fails), the edges of the stack region, a fill
through the same argument, and a fifth argument on the caller's stack. One case of group P shows that the
declaration of the programs is read as often for five cases as for one.

## Controls that need the toolchain

    python test_difftest_build.py --config ../build.toml

These controls build small units made from `control_base.c` by text
edits (the nonmatching C of `func_801e9080_slot06_00` as it stood before that
function became exact; the file is kept as their base and is no function of
this folder), in a folder of the run's own under `ps1/build/`, which the repository
ignores. A run removes its own folder and no other, so two runs side by side
do not disturb each other; two cases check that, and that Git ignores the
folder. They need
the pinned toolchain and the private inputs of the matching build; with either
missing they print one line and exit with status 2. They check that a helper
named like a linker symbol of the tree runs (and is not replaced by the original
code), that the build is refused if the filter of the symbol file is bypassed,
that a call of the function to its own name stays in the unit, that a helper
before the entry and a constant table build with the right entry and size, that
initialized, uninitialized and own-section data are each refused, and that a
callee the unit does not define keeps its original address. Last line printed:
`all cases behaved as required`.

## Running

From this folder, with a Python that has the packages of the repository's
`requirements.txt` (Unicorn is one of them) and with the private inputs of
the matching build in place:

    python difftest.py --config ../build.toml --cases 2000 FUNC
    python difftest.py --config ../build.toml --cases 2000 --seed 7 FUNC
    python difftest.py --config ../build.toml --cases 2000 --control FUNC
    python difftest.py --config ../build.toml --cases 2000 --writes FUNC
    python difftest.py --config ../build.toml --cases 2000 --writes --seed 7 FUNC

A function is tested alone: these five runs for the function that was added
or changed, at the same time. No other function is run again for it, and a
folder is not run as a whole (see
[efficient checks](../../../docs/efficiency.md)).

`--seed S` changes the random inputs (default 1); a run is reproducible for a
given seed. `--folder DIR` reads the sources and contract files of another
folder; `--uncovered` lists the instruction slots that no case executed,
and with `--writes` it adds where the first case outside wrote. Exit
status 1 means a difference, or no equal case, or a control that did not
trip, or with `--writes` a case in which the original wrote outside what
the setup made.

## Functions

The functions of this folder. At present there is none: the last three became
exact on 2026-10-12 and stand in `../slot06_08/slot06_08_8bd8x_r1.c`,
`../slot06_0e/slot06_0e_9738x_r1.c` and `../slot06_0e/slot06_0e_84ccx_r1.c`; the
page and the tool stay for the next one.
The three blocks are a record, not an instruction: what the commands of
"Running" printed for each function when it was last run. As it stands that
is one run of the whole folder on 2026-10-09, with `--all` in place of the
function's name. From now on a function's lines change only when that
function is run again.

The test (`--cases 2000 FUNC`, seed 1):

```
```

The control (`--cases 2000 --control FUNC`):

```
```

The write audit (`--cases 2000 --writes FUNC`, seed 1):

```
```

A function with fewer slots executed than it has names the others in its
header comment, with the reason why no input reaches them.
