# Nonmatching C

C for functions of the PS1 game that compiles with the pinned toolchain but
does not reproduce the original's bytes. Each file here is separate from the
exact owner of those bytes: the PS1 build does not use any file of this
folder, and nothing in `build.toml`, `symbols.ld` or `types.fields` refers
to it. The raw bytes stay what the matching build owns.

A function of this folder that is later rebuilt exactly moves to the folder
of its image, enters the build there, and leaves this one.

## What "nonmatching" means here

The function is NOT byte-identical to the original: its build has the same
size (see the table) but differs from the original's bytes. What these files
claim is narrower: on the inputs of the function's contract, the build and
the original code leave the same final state.

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
function runs), and end the run at its N-th call (`ends_run_at`). A run
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
which is not made), the stack and the argument area at their borders,
the scratchpad, cases that are discarded, the recorders' own log and
code, the address runs and names of the second line, the lines, status
and errors of `--writes` through `main` (no build is attempted), and
that the default mode prints what it printed before.
Group L checks the symbol file of the standalone link: the lines that assign a
name the unit defines are removed (plain, spaced and inside `PROVIDE`), names
that only begin alike stay, and a link that puts a defined name outside the
unit is reported with the name and the address.

## Controls that need the toolchain

    python test_difftest_build.py --config ../build.toml

These controls build small units made from `func_801e9080_slot06_00.c` by text
edits, in a folder of the run's own under `ps1/build/`, which the repository
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

    python difftest.py --config ../build.toml --cases 2000 func_801e9080_slot06_00
    python difftest.py --config ../build.toml --cases 2000 --control func_801e9080_slot06_00
    python difftest.py --config ../build.toml --cases 2000 --writes func_801e9080_slot06_00

`--seed S` changes the random inputs (default 1); a run is reproducible for a
given seed. `--folder DIR` reads the sources and contract files of another
folder; `--uncovered` lists the instruction slots that no case executed,
and with `--writes` it adds where the first case outside wrote. Exit
status 1 means a difference, or no equal case, or a control that did not
trip, or with `--writes` a case in which the original wrote outside what
the setup made.

## Functions

The functions of this folder.
What these commands printed on 2026-10-09:

    python difftest.py --config ../build.toml --cases 2000 --all

```
func_801e9080_slot06_00: built 700 bytes, original 700 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e9080_slot06_00 coverage: 173 of 175 instruction slots of the original executed
```

    python difftest.py --config ../build.toml --cases 2000 --control --all

```
func_801e9080_slot06_00 control: different 1797 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 164
```

    python difftest.py --config ../build.toml --cases 2000 --writes --all

```
func_801e9080_slot06_00 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
```

A function with fewer slots executed than it has names the others in its
header comment, with the reason why no input reaches them.
