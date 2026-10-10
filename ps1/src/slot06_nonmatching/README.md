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
func_801e84cc_slot06_0e edges: constants 13, altered runs 24, unnoticed 6, all discarded 2
  slot 74: addiu a1,a3,0x4, immediate 0x4 -> 0x5: different 0 of 2000, discarded 1968
  slot 74: addiu a1,a3,0x4, immediate 0x4 -> 0x3: different 0 of 2000, discarded 1968
  slot 103: addiu a1,a1,0x1c, immediate 0x1c -> 0x1d: different 0 of 2000, discarded 1963
  slot 103: addiu a1,a1,0x1c, immediate 0x1c -> 0x1b: different 0 of 2000, discarded 1963
  slot 112: addiu a3,a3,0x1c, immediate 0x1c -> 0x1d: different 0 of 2000, discarded 1965
  slot 112: addiu a3,a3,0x1c, immediate 0x1c -> 0x1b: different 0 of 2000, discarded 1965
  slot 36: not executed by any case
func_801e8bd8_slot06_08 edges: constants 71, altered runs 142, unnoticed 43, all discarded 1
  slot 38: addiu v0,v0,0xffff, immediate 0xffff -> 0xfffe: different 0 of 2000, discarded 0
  slot 50: addiu a1,a1,0xffb1, immediate 0xffb1 -> 0xffb2: different 0 of 2000, discarded 0
  slot 50: addiu a1,a1,0xffb1, immediate 0xffb1 -> 0xffb0: different 0 of 2000, discarded 0
  slot 155: slti v0,t2,0x1b, immediate 0x1b -> 0x1c: different 0 of 2000, discarded 0
  slot 155: slti v0,t2,0x1b, immediate 0x1b -> 0x1a: different 0 of 2000, discarded 0
  slot 159: addiu t1,t3,0x1d, immediate 0x1d -> 0x1e: different 0 of 2000, discarded 925
  slot 159: addiu t1,t3,0x1d, immediate 0x1d -> 0x1c: different 0 of 2000, discarded 925
  slot 207: addiu t1,t1,0x28, immediate 0x28 -> 0x29: different 0 of 2000, discarded 911
  slot 207: addiu t1,t1,0x28, immediate 0x28 -> 0x27: different 0 of 2000, discarded 911
  slot 217: addiu t3,t3,0x28, immediate 0x28 -> 0x29: different 0 of 2000, discarded 923
  slot 217: addiu t3,t3,0x28, immediate 0x28 -> 0x27: different 0 of 2000, discarded 923
  slot 223: addiu t2,t2,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 981
  slot 236: addiu v0,v0,0xf, immediate 0xf -> 0x10: different 0 of 2000, discarded 0
  slot 272: addiu v1,v1,0xf, immediate 0xf -> 0x10: different 0 of 2000, discarded 0
  slot 308: addiu at,zero,0xffff, immediate 0xffff -> 0x0: different 0 of 2000, discarded 0
  slot 308: addiu at,zero,0xffff, immediate 0xffff -> 0xfffe: different 0 of 2000, discarded 0
  slot 324: addiu at,zero,0xffff, immediate 0xffff -> 0x0: different 0 of 2000, discarded 0
  slot 324: addiu at,zero,0xffff, immediate 0xffff -> 0xfffe: different 0 of 2000, discarded 0
  slot 333: slti v0,t2,0x1b, immediate 0x1b -> 0x1c: different 0 of 2000, discarded 0
  slot 333: slti v0,t2,0x1b, immediate 0x1b -> 0x1a: different 0 of 2000, discarded 0
  slot 337: addiu t1,t3,0x1d, immediate 0x1d -> 0x1e: different 0 of 2000, discarded 1417
  slot 337: addiu t1,t3,0x1d, immediate 0x1d -> 0x1c: different 0 of 2000, discarded 1417
  slot 385: addiu t1,t1,0x28, immediate 0x28 -> 0x29: different 0 of 2000, discarded 1385
  slot 385: addiu t1,t1,0x28, immediate 0x28 -> 0x27: different 0 of 2000, discarded 1385
  slot 394: addiu t3,t3,0x28, immediate 0x28 -> 0x29: different 0 of 2000, discarded 1413
  slot 394: addiu t3,t3,0x28, immediate 0x28 -> 0x27: different 0 of 2000, discarded 1413
  slot 400: addiu t2,t2,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1524
  slot 406: ori fp,zero,0x88, immediate 0x88 -> 0x89: different 0 of 2000, discarded 0
  slot 406: ori fp,zero,0x88, immediate 0x88 -> 0x87: different 0 of 2000, discarded 0
  slot 419: addiu at,zero,0xffff, immediate 0xffff -> 0x0: different 0 of 2000, discarded 0
  slot 419: addiu at,zero,0xffff, immediate 0xffff -> 0xfffe: different 0 of 2000, discarded 0
  slot 435: addiu at,zero,0xffff, immediate 0xffff -> 0x0: different 0 of 2000, discarded 0
  slot 435: addiu at,zero,0xffff, immediate 0xffff -> 0xfffe: different 0 of 2000, discarded 0
  slot 444: slti v0,t2,0x1b, immediate 0x1b -> 0x1c: different 0 of 2000, discarded 0
  slot 444: slti v0,t2,0x1b, immediate 0x1b -> 0x1a: different 0 of 2000, discarded 0
  slot 448: addiu t1,t3,0x1d, immediate 0x1d -> 0x1e: different 0 of 2000, discarded 1260
  slot 448: addiu t1,t3,0x1d, immediate 0x1d -> 0x1c: different 0 of 2000, discarded 1260
  slot 496: addiu t1,t1,0x28, immediate 0x28 -> 0x29: different 0 of 2000, discarded 1242
  slot 496: addiu t1,t1,0x28, immediate 0x28 -> 0x27: different 0 of 2000, discarded 1242
  slot 505: addiu t3,t3,0x28, immediate 0x28 -> 0x29: different 0 of 2000, discarded 1249
  slot 505: addiu t3,t3,0x28, immediate 0x28 -> 0x27: different 0 of 2000, discarded 1249
  slot 511: addiu t2,t2,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1334
  slot 516: addiu s2,s2,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1524
func_801e8dc4_slot06_00 edges: constants 9, altered runs 18, unnoticed 0, all discarded 0
func_801e8dc8_slot06_05 edges: constants 20, altered runs 34, unnoticed 2, all discarded 0
  slot 189: addiu t2,t2,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1707
  slot 194: addiu t3,t3,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1738
  slot 103: not executed by any case
  slot 110: not executed by any case
  slot 170: not executed by any case
func_801e8df0_slot06_0a edges: constants 14, altered runs 24, unnoticed 2, all discarded 0
  slot 170: addiu a3,a3,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1726
  slot 175: addiu t7,t7,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1755
  slot 123: not executed by any case
  slot 131: not executed by any case
func_801e8fec_slot06_11 edges: constants 14, altered runs 24, unnoticed 3, all discarded 0
  slot 28: addiu v1,v1,0x7f, immediate 0x7f -> 0x7e: different 0 of 2000, discarded 0
  slot 162: addiu a3,a3,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1682
  slot 167: addiu t7,t7,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1716
  slot 115: not executed by any case
  slot 123: not executed by any case
func_801e9080_slot06_00 edges: constants 12, altered runs 20, unnoticed 2, all discarded 0
  slot 154: addiu a3,a3,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1721
  slot 159: addiu t7,t7,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1751
  slot 107: not executed by any case
  slot 115: not executed by any case
func_801e90a8_slot06_10 edges: constants 2, altered runs 2, unnoticed 0, all discarded 0
  slot 17: not executed by any case
func_801e9114_slot06_12 edges: constants 18, altered runs 30, unnoticed 2, all discarded 0
  slot 186: addiu a2,a2,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1748
  slot 191: addiu t3,t3,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1776
  slot 117: not executed by any case
  slot 124: not executed by any case
  slot 167: not executed by any case
func_801e96fc_slot06_0b edges: constants 35, altered runs 64, unnoticed 2, all discarded 0
  slot 231: addiu t2,t2,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1724
  slot 236: addiu t3,t3,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1762
  slot 108: not executed by any case
  slot 115: not executed by any case
  slot 211: not executed by any case
func_801e9738_slot06_0e edges: constants 17, altered runs 32, unnoticed 6, all discarded 4
  slot 96: addiu a1,t2,0x4, immediate 0x4 -> 0x5: different 0 of 2000, discarded 1883
  slot 96: addiu a1,t2,0x4, immediate 0x4 -> 0x3: different 0 of 2000, discarded 1883
  slot 125: addiu a1,a1,0x20, immediate 0x20 -> 0x21: different 0 of 2000, discarded 1820
  slot 125: addiu a1,a1,0x20, immediate 0x20 -> 0x1f: different 0 of 2000, discarded 1820
  slot 134: addiu t2,t2,0x20, immediate 0x20 -> 0x21: different 0 of 2000, discarded 1878
  slot 134: addiu t2,t2,0x20, immediate 0x20 -> 0x1f: different 0 of 2000, discarded 1878
  slot 37: not executed by any case
func_801e9798_slot06_06 edges: constants 26, altered runs 46, unnoticed 2, all discarded 0
  slot 223: addiu t2,t2,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1712
  slot 228: addiu t4,t4,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1746
  slot 116: not executed by any case
  slot 123: not executed by any case
  slot 204: not executed by any case
func_801e9840_slot06_09 edges: constants 14, altered runs 24, unnoticed 3, all discarded 0
  slot 28: addiu v1,v1,0x7f, immediate 0x7f -> 0x7e: different 0 of 2000, discarded 0
  slot 162: addiu a3,a3,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1710
  slot 167: addiu t7,t7,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1749
  slot 115: not executed by any case
  slot 123: not executed by any case
func_801e98c8_slot06_0c edges: constants 2, altered runs 2, unnoticed 0, all discarded 0
  slot 17: not executed by any case
func_801e98dc_slot06_07 edges: constants 2, altered runs 2, unnoticed 0, all discarded 0
  slot 17: not executed by any case
func_801e9970_slot06_07 edges: constants 13, altered runs 22, unnoticed 2, all discarded 0
  slot 171: addiu a3,a3,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1709
  slot 176: addiu t7,t7,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1747
  slot 124: not executed by any case
  slot 132: not executed by any case
func_801e998c_slot06_0d edges: constants 2, altered runs 2, unnoticed 0, all discarded 0
  slot 17: not executed by any case
func_801e99b4_slot06_04 edges: constants 21, altered runs 36, unnoticed 4, all discarded 0
  slot 135: ori v0,zero,0x6, immediate 0x6 -> 0x7: different 0 of 2000, discarded 0
  slot 135: ori v0,zero,0x6, immediate 0x6 -> 0x5: different 0 of 2000, discarded 0
  slot 182: addiu a2,a2,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1697
  slot 187: addiu t4,t4,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1736
  slot 119: not executed by any case
  slot 126: not executed by any case
  slot 163: not executed by any case
func_801e99c8_slot06_02 edges: constants 16, altered runs 26, unnoticed 2, all discarded 0
  slot 193: addiu a2,a2,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1740
  slot 198: addiu t5,t5,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1782
  slot 127: not executed by any case
  slot 134: not executed by any case
  slot 173: not executed by any case
func_801e9b54_slot06_08 edges: constants 13, altered runs 22, unnoticed 2, all discarded 0
  slot 166: addiu a3,a3,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1700
  slot 171: addiu t7,t7,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1730
  slot 119: not executed by any case
  slot 127: not executed by any case
func_801e9bb0_slot06_0f edges: constants 15, altered runs 24, unnoticed 2, all discarded 0
  slot 177: addiu a2,a2,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1746
  slot 182: addiu t8,t8,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1773
  slot 124: not executed by any case
  slot 131: not executed by any case
  slot 157: not executed by any case
func_801e9c80_slot06_0e edges: constants 2, altered runs 2, unnoticed 0, all discarded 0
  slot 17: not executed by any case
func_801e9d04_slot06_03 edges: constants 12, altered runs 20, unnoticed 2, all discarded 0
  slot 146: addiu a3,a3,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1714
  slot 151: addiu t7,t7,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1750
  slot 102: not executed by any case
  slot 110: not executed by any case
func_801e9d14_slot06_0e edges: constants 38, altered runs 70, unnoticed 2, all discarded 0
  slot 245: addiu t1,t1,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1715
  slot 250: addiu t3,t3,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1752
  slot 116: not executed by any case
  slot 123: not executed by any case
  slot 226: not executed by any case
func_801e9ef4_slot06_10 edges: constants 14, altered runs 22, unnoticed 2, all discarded 0
  slot 158: addiu a2,a2,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1711
  slot 163: addiu t7,t7,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1740
  slot 105: not executed by any case
  slot 112: not executed by any case
  slot 138: not executed by any case
func_801e9f20_slot06_0d edges: constants 16, altered runs 26, unnoticed 3, all discarded 0
  slot 31: addiu v1,v1,0x7f, immediate 0x7f -> 0x7e: different 0 of 2000, discarded 0
  slot 174: addiu a3,a3,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1713
  slot 179: addiu t8,t8,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1743
  slot 115: not executed by any case
  slot 122: not executed by any case
  slot 154: not executed by any case
func_801e9f90_slot06_01 edges: constants 12, altered runs 20, unnoticed 2, all discarded 0
  slot 141: addiu a3,a3,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1711
  slot 146: addiu t7,t7,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1736
  slot 94: not executed by any case
  slot 102: not executed by any case
func_801ea3b4_slot06_08 edges: constants 1, altered runs 2, unnoticed 0, all discarded 0
func_801ea640_slot06_0c edges: constants 13, altered runs 22, unnoticed 2, all discarded 0
  slot 173: addiu a3,a3,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1597
  slot 178: addiu t7,t7,0x1, immediate 0x1 -> 0x0: different 0 of 2000, discarded 1625
  slot 129: not executed by any case
  slot 137: not executed by any case
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
`FUNC fixtures: kept K of N cases (slots A, edges B, results C), needs image: yes|no`.
A file that exists is replaced and the tool says so. A line starting
`warning` says that the cases read bytes the setup did not make, which come
from the game's memory image and would be published with the file.

`--replay` puts the memory of the case in place (the game's image only when
the fixture says `needs_image`, because the function runs code of the game
other than its own and the stand-ins; otherwise poison bytes), puts a stand-in
at each callee that returns the recorded values in order, runs the build, and
requires the same calls in the same order with the same logged values, the
result, the written bytes exactly, and the saved registers and stack pointer as
the original left them. The line is `FUNC replay: fixtures K, passed P, failed F`
(with `(game image used for N)` when the image was read), followed for each
failure by the fixture and up to three lines saying what differed first. The
status is 0 when F is 0, 1 otherwise, 2 for a missing or malformed file. With no
function named, every function of the folder that has a fixtures file is
replayed. The replay does not run the original and uses no byte of the game for
a fixture that does not need the image, but it starts like every mode: it loads
the configuration, which reads the baseline executable to check its hash, so it
needs the private inputs of the configuration, and it builds the C with the
private PS1 compiler. A replay that needs nothing private is a later piece (the
C as compiled for a PC).

What a replay shows is narrow: for the recorded inputs, the C reads, calls and
writes what the original did. It is evidence for those inputs. It is not the
wide comparison (that is made once, by `--record` and the default mode, when
the function is written) and it is not equivalence. A fixture goes stale when
the C changes what it reads (a byte the fixture does not hold is poison), calls
or writes: the replay then fails, and nothing regenerates a file except a new
`--record`.

Choices of the tool, each of them a decision to review:

- `reads` holds what the function's own instructions read (and what its real
  callees read, when the fixture needs the image). What a stand-in copies into
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
  (`stores`, `counts`, `ends_run_at`, `masks`) are part of the fixture; a stand-in
  with a `tail` (machine code of the contract's own) cannot be described and the
  record refuses it with status 2.
- The reads and writes are found by looking at each instruction before it runs,
  not with the emulator's memory hooks: with a memory hook, Unicorn 2.1.4 ends a
  run with an exception when a load or store lies in the delay slot of a
  conditional branch that is not taken and a jump follows (`test_difftest.py`
  has made-up code that shows the difference).

The fixtures of five functions of `../resident_nonmatching` are the first.
Each block is what the commands printed (seed 1, `--record --cases 2000 --jobs 4`):

```
func_8012fd80 fixtures: kept 6 of 2000 cases (slots 2, edges 3, results 1), needs image: no
func_8012fd80 replay: fixtures 6, passed 6, failed 0
func_80119694 fixtures: kept 4 of 2000 cases (slots 1, edges 3, results 0), needs image: no
func_80119694 replay: fixtures 4, passed 4, failed 0
func_8011cf98 fixtures: kept 12 of 2000 cases (slots 6, edges 6, results 0), needs image: yes
func_8011cf98 replay: fixtures 12, passed 12, failed 0 (game image used for 12)
func_8011a880 fixtures: kept 7 of 2000 cases (slots 5, edges 2, results 0), needs image: no
func_8011a880 replay: fixtures 7, passed 7, failed 0
func_801189c4 fixtures: kept 5 of 2000 cases (slots 4, edges 1, results 0), needs image: no
func_801189c4 replay: fixtures 5, passed 5, failed 0
```

The files are 4897, 3614, 22085, 17393 and 70736 bytes. The records took 8, 5,
161, 14 and 43 seconds by the shell's clock; each replay took 1 second by the
same clock, the compile of the C included. What they leave uncovered is in the
files: 6, 6, 48, 2 and 2 lines of `uncovered.edges` (altered runs that no case
notices; each line gives the number of discarded cases of that run), and slot
`+0x48` of `func_8011cf98` and slots `+0x330..+0x348` of `func_801189c4`, which
no case executes. A second record of each function wrote the same bytes.

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
stand-in, `needs_image` set and unset (and the image asked for only then), a
stand-in at an unnamed address with its pointer cell, the options of a
stand-in, that the replay never runs the original and reads no contract, the
input errors of the options, and the decoding of the call log by
`contracts.CallLog`.

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
    python difftest.py --config ../build.toml --cases 2000 --seed 7 func_801e9080_slot06_00
    python difftest.py --config ../build.toml --cases 2000 --control func_801e9080_slot06_00
    python difftest.py --config ../build.toml --cases 2000 --writes func_801e9080_slot06_00
    python difftest.py --config ../build.toml --cases 2000 --writes --seed 7 func_801e9080_slot06_00

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

The functions of this folder.
The three blocks are a record, not an instruction: what the commands of
"Running" printed for each function when it was last run. As it stands that
is one run of the whole folder on 2026-10-09, with `--all` in place of the
function's name. From now on a function's lines change only when that
function is run again.

The test (`--cases 2000 FUNC`, seed 1):

```
func_801e84cc_slot06_0e: built 504 bytes, original 536 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e84cc_slot06_0e coverage: 133 of 134 instruction slots of the original executed
func_801e8bd8_slot06_08: built 1496 bytes, original 2124 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e8bd8_slot06_08 coverage: 523 of 531 instruction slots of the original executed
func_801e8dc4_slot06_00: built 344 bytes, original 432 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e8dc4_slot06_00 coverage: 108 of 108 instruction slots of the original executed
func_801e8dc8_slot06_05: built 756 bytes, original 840 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e8dc8_slot06_05 coverage: 207 of 210 instruction slots of the original executed
func_801e8df0_slot06_0a: built 768 bytes, original 768 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e8df0_slot06_0a coverage: 190 of 192 instruction slots of the original executed
func_801e8fec_slot06_11: built 648 bytes, original 728 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e8fec_slot06_11 coverage: 180 of 182 instruction slots of the original executed
func_801e9080_slot06_00: built 700 bytes, original 700 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e9080_slot06_00 coverage: 173 of 175 instruction slots of the original executed
func_801e90a8_slot06_10: built 136 bytes, original 148 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e90a8_slot06_10 coverage: 36 of 37 instruction slots of the original executed
func_801e9114_slot06_12: built 704 bytes, original 828 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e9114_slot06_12 coverage: 204 of 207 instruction slots of the original executed
func_801e96fc_slot06_0b: built 820 bytes, original 996 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e96fc_slot06_0b coverage: 246 of 249 instruction slots of the original executed
func_801e9738_slot06_0e: built 588 bytes, original 628 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e9738_slot06_0e coverage: 156 of 157 instruction slots of the original executed
func_801e9798_slot06_06: built 888 bytes, original 972 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e9798_slot06_06 coverage: 240 of 243 instruction slots of the original executed
func_801e9840_slot06_09: built 728 bytes, original 728 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e9840_slot06_09 coverage: 180 of 182 instruction slots of the original executed
func_801e98c8_slot06_0c: built 136 bytes, original 148 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e98c8_slot06_0c coverage: 36 of 37 instruction slots of the original executed
func_801e98dc_slot06_07: built 136 bytes, original 148 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e98dc_slot06_07 coverage: 36 of 37 instruction slots of the original executed
func_801e9970_slot06_07: built 768 bytes, original 768 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e9970_slot06_07 coverage: 190 of 192 instruction slots of the original executed
func_801e998c_slot06_0d: built 136 bytes, original 148 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e998c_slot06_0d coverage: 36 of 37 instruction slots of the original executed
func_801e99b4_slot06_04: built 744 bytes, original 812 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e99b4_slot06_04 coverage: 200 of 203 instruction slots of the original executed
func_801e99c8_slot06_02: built 828 bytes, original 860 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e99c8_slot06_02 coverage: 212 of 215 instruction slots of the original executed
func_801e9b54_slot06_08: built 744 bytes, original 744 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e9b54_slot06_08 coverage: 184 of 186 instruction slots of the original executed
func_801e9bb0_slot06_0f: built 784 bytes, original 800 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e9bb0_slot06_0f coverage: 197 of 200 instruction slots of the original executed
func_801e9c80_slot06_0e: built 136 bytes, original 148 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e9c80_slot06_0e coverage: 36 of 37 instruction slots of the original executed
func_801e9d04_slot06_03: built 652 bytes, original 672 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e9d04_slot06_03 coverage: 166 of 168 instruction slots of the original executed
func_801e9d14_slot06_0e: built 952 bytes, original 1060 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e9d14_slot06_0e coverage: 262 of 265 instruction slots of the original executed
func_801e9ef4_slot06_10: built 636 bytes, original 724 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e9ef4_slot06_10 coverage: 178 of 181 instruction slots of the original executed
func_801e9f20_slot06_0d: built 768 bytes, original 788 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e9f20_slot06_0d coverage: 194 of 197 instruction slots of the original executed
func_801e9f90_slot06_01: built 628 bytes, original 648 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e9f90_slot06_01 coverage: 160 of 162 instruction slots of the original executed
func_801ea3b4_slot06_08: built 180 bytes, original 180 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801ea3b4_slot06_08 coverage: 45 of 45 instruction slots of the original executed
func_801ea640_slot06_0c: built 780 bytes, original 780 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801ea640_slot06_0c coverage: 193 of 195 instruction slots of the original executed
```

The control (`--cases 2000 --control FUNC`):

```
func_801e84cc_slot06_0e control: different 1968 of 2000 (expected more than 0)
  altered: v coordinate store moved by one byte, instruction slot 89
func_801e8bd8_slot06_08 control: different 110 of 2000 (expected more than 0)
  altered: row count limit 0x20 changed to 0x10, instruction slot 67
func_801e8dc4_slot06_00 control: different 2000 of 2000 (expected more than 0)
  altered: constant 0x100 of field_78 changed to 0x101, instruction slot 69
func_801e8dc8_slot06_05 control: different 1790 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 178
func_801e8df0_slot06_0a control: different 1811 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 180
func_801e8fec_slot06_11 control: different 1779 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 155
func_801e9080_slot06_00 control: different 1797 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 164
func_801e90a8_slot06_10 control: different 2000 of 2000 (expected more than 0)
  altered: low half stored at field_04, instruction slot 10
func_801e9114_slot06_12 control: different 1820 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 168
func_801e96fc_slot06_0b control: different 1813 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 193
func_801e9738_slot06_0e control: different 1883 of 2000 (expected more than 0)
  altered: v coordinate store moved by one byte, instruction slot 107
func_801e9798_slot06_06 control: different 1799 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 211
func_801e9840_slot06_09 control: different 1800 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 172
func_801e98c8_slot06_0c control: different 2000 of 2000 (expected more than 0)
  altered: low half stored at field_04, instruction slot 10
func_801e98dc_slot06_07 control: different 2000 of 2000 (expected more than 0)
  altered: low half stored at field_04, instruction slot 10
func_801e9970_slot06_07 control: different 1804 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 181
func_801e998c_slot06_0d control: different 2000 of 2000 (expected more than 0)
  altered: low half stored at field_04, instruction slot 10
func_801e99b4_slot06_04 control: different 1788 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 176
func_801e99c8_slot06_02 control: different 1805 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 194
func_801e9b54_slot06_08 control: different 1794 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 176
func_801e9bb0_slot06_0f control: different 1825 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 183
func_801e9c80_slot06_0e control: different 2000 of 2000 (expected more than 0)
  altered: low half stored at field_04, instruction slot 10
func_801e9d04_slot06_03 control: different 1787 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 151
func_801e9d14_slot06_0e control: different 1803 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 226
func_801e9ef4_slot06_10 control: different 1793 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 148
func_801e9f20_slot06_0d control: different 1804 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 179
func_801e9f90_slot06_01 control: different 1788 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 147
func_801ea3b4_slot06_08 control: different 1609 of 2000 (expected more than 0)
  altered: field_54 store moved by four bytes, instruction slot 36
func_801ea640_slot06_0c control: different 1663 of 2000 (expected more than 0)
  altered: record counter store moved by two bytes, instruction slot 183
```

The write audit (`--cases 2000 --writes FUNC`, seed 1):

```
func_801e84cc_slot06_0e writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e8bd8_slot06_08 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e8dc4_slot06_00 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e8dc8_slot06_05 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e8df0_slot06_0a writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e8fec_slot06_11 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e9080_slot06_00 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e90a8_slot06_10 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e9114_slot06_12 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e96fc_slot06_0b writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e9738_slot06_0e writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e9798_slot06_06 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e9840_slot06_09 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e98c8_slot06_0c writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e98dc_slot06_07 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e9970_slot06_07 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e998c_slot06_0d writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e99b4_slot06_04 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e99c8_slot06_02 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e9b54_slot06_08 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e9bb0_slot06_0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e9c80_slot06_0e writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e9d04_slot06_03 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e9d14_slot06_0e writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e9ef4_slot06_10 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e9f20_slot06_0d writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e9f90_slot06_01 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801ea3b4_slot06_08 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801ea640_slot06_0c writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
```

A function with fewer slots executed than it has names the others in its
header comment, with the reason why no input reaches them.
