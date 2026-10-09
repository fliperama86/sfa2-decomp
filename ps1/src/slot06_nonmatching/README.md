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

## What a pass does not show

A pass is evidence for the tested inputs only. It is not a proof of
equivalence. Inputs outside the contract, rare combinations that random
generation does not reach, timing, and the order of memory stores are not
covered. The contract is in the header comment of each `.c` file and in
`contracts.py`.

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
missing, out of order or with another value are differences. Group H checks that
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
Group L checks the symbol file of the standalone link: the lines that assign a
name the unit defines are removed (plain, spaced and inside `PROVIDE`), names
that only begin alike stay, and a link that puts a defined name outside the
unit is reported with the name and the address.

## Controls that need the toolchain

    python test_difftest_build.py --config ../build.toml

These controls build small units made from `func_801e9080_slot06_00.c` by text
edits, in a temporary folder beside this one that is removed afterwards. They need
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

`--seed S` changes the random inputs (default 1); a run is reproducible for a
given seed. `--folder DIR` reads the sources and contract files of another
folder; `--uncovered` lists the instruction slots that no case executed. Exit status 1 means a difference, or no equal case, or a control
that did not trip.

## Functions

The figures are the two lines that `difftest.py` prints for the command above.

| Function | Built bytes | Original bytes | Cases | Discarded | Equal | Different | Slots executed |
| --- | --- | --- | --- | --- | --- | --- | --- |
| `func_801e9080_slot06_00` | 700 | 700 | 2000 | 0 | 2000 | 0 | 173 of 175 |

With `--control`, the same function prints
`func_801e9080_slot06_00 control: different 1797 of 2000` (more than 0, as
required).
