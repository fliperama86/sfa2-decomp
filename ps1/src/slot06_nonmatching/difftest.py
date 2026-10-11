#!/usr/bin/env python3
"""Differential test of a nonmatching C function against the original code.

    difftest.py --config ../build.toml [--folder DIR] [--cases N] [--seed S]
                [--control | --writes | --edges [--jobs N]] [--uncovered] (FUNC... | --all)
    difftest.py --config ../build.toml [--folder DIR] --record [--cases N] [--seed S] [--max M] [--jobs N] FUNC
    difftest.py --config ../build.toml [--folder DIR] --replay [FUNC...]

For each FUNC the tool builds `FUNC.c` of the folder (this one, or DIR) with the pinned
toolchain of the matching build (the preprocessing, compiler, maspsx and
assembler steps of `ps1/tools/matchbuild.py`), links it alone at a test
address outside the console's RAM with the tree's `symbols.ld`, and runs the
original function and the build under the Unicorn emulator on identical
random inputs. The final states are compared: the return
register when the contract says there is one, the callee-saved registers,
and all of RAM and the scratchpad except the stack region.

The contract of FUNC is the entry of `contracts.CONTRACTS`, or the value
`CONTRACT` of the file `FUNC.py` beside `FUNC.c`. A second line says how
many instruction slots of the original function the cases executed, and
`--uncovered` lists the others: cases that never reach a part of the
function say nothing about it.

The option --writes audits where the ORIGINAL writes, which the comparison above does not
say: both runs may end alike and both may have written outside what the contract names.
It runs the original only (nothing is built) on the cases of the seed and prints per function

    FUNC writes: cases N, discarded D, outside K (largest B bytes)

K is the number of cases in which the original changed at least one byte of RAM or of the
scratchpad that is neither made nor in the stack region; B is the largest number of such bytes
in one case. D counts the cases that were discarded (a fault, the instruction budget, or a call
log of the contract that is full: a run is ended at the call whose entry would not fit in the room
the contract gave its log, in every mode, for the original and for the build), as
in the default mode; a discarded case is not counted as outside. With --uncovered and a K above
0 a second line follows, `  first: case C; 0xADDR..0xADDR (NAME+0xOFF); ...`: the first such
case and the address runs of its bytes (bytes less than 17 apart form one run), each run with
the nearest name at or below its first address in the same region (RAM or scratchpad) of the
tree's symbol table, `?` when there is none; at most six runs, then `and M more`. The status is
0 when every K is 0, 1 otherwise, 2 for the errors above, and --writes with --control is one.
The default mode and --control print what they printed before the option existed.

The option --edges looks for the constants whose edge no case tries. It alters the ORIGINAL
code, one constant at a time, and leaves the build alone (--control does the opposite: it alters one
word of the BUILD). Coverage of instruction slots does not show that the edge of a comparison was
tried: random inputs rarely land on the one value where a limit and its neighbour differ. For each FUNC
the build is made once and the default comparison is run first, with the given cases and seed; if it
shows a difference or a discarded case the edges of that function are not swept (one line says so, and
the status is 1). Otherwise the words of the original function are scanned, and a word is a constant of
the sweep when it is one of these (a choice, not a theory: the sweep finds the edges of comparisons and
of added limits, not every constant):

  - `slti` or `sltiu`, with any registers;
  - `addiu rt,rs,imm` with neither rs nor rt `sp` or `gp`, and not the low half of an address or of a
    32-bit constant (it is such a low half when rs == rt and the nearest earlier word of the function
    that writes rt is `lui rt`);
  - `ori rt,zero,imm`. An `ori` whose source is not `zero` is not taken, so `ori rt,zero,imm` cannot be
    the low half of a 32-bit constant, and the `lui` rule is applied to `addiu` only.

Nothing else is a constant of the sweep: no load or store offset, no `andi` or `xori` mask, no shift
amount, no branch, no `lui`. Each constant gets two altered runs, its immediate plus 1 and minus 1,
each taken modulo 0x10000 (0xffff plus 1 is 0, and 0 minus 1 is 0xffff). An altered run is the default
comparison (same cases, same seed, same build) with that one word of the original replaced in the memory
image of every case; the original is put back before the next run. A run ends at its first case that
differs: it is then noticed and it is not run to its end. A run that has found no difference goes on
to the last case, because only then is it unnoticed (or all discarded). An altered original has an
instruction budget of its own: eight times the longest run of the unaltered original over the cases, at
least 20,000 instructions and at most the general budget. A case in which it runs longer is discarded,
like any case whose original does not end; so an altered original that would have ended later than that
counts as one that does not end. The altered runs of a function are made `--jobs N` at a time (default 8),
each in a forked process of its own (the option needs a system that has that start method); 1 makes them
in this process. The lines are printed in the fixed order below whatever N is. An altered original can
jump anywhere, and the emulator itself has ended (with a signal) on what it found there, seen with a
bounds check before a jump table moved by one: the process of that run then ends without a result, the
run is neither noticed nor unnoticed, and it is counted and listed apart (with `--jobs 1` such an end is
the end of the tool). `--jobs` is for `--edges` and `--record` only, and N below 1 is an input error. Per
function it prints

    FUNC edges: constants C, altered runs R, unnoticed U, all discarded A

with `, no result L` at its end when L runs ended without a result, and, in order of the word's index in
the original (the numbering of the coverage line), plus before minus, a line for every altered run that
ends with `different 0`, or without a result:

    `  slot I: MNEMONIC OPERANDS, immediate 0xOLD -> 0xNEW: different 0 of N, discarded D`
    `  slot I: MNEMONIC OPERANDS, immediate 0xOLD -> 0xNEW: no result, the process of this run ended (signal S)`

A constant in a slot that no case of the unaltered run executed counts in C, but its two runs are not
made, they are not in R, and it is not in U (the coverage line says that the slot is not executed); with
--uncovered a line `  slot I: not executed by any case` is printed for it, after the lines above. A run in
which every case is discarded (the altered original faults or does not end) is not unnoticed: it counts
in A. The status is 0 when every U is 0, 1 when any U is above 0 or a function was not swept, 2 and 3 as
above; --edges with --control or with --writes is an input error (2).

What an unnoticed line means: no case of this seed tells the constant from its neighbour. That is a gap
of the setup when the contract's inputs can reach the edge, and it is no gap when they cannot (the edge
lies in inputs the contract excludes, or the constant has no effect on what the test compares). The tool
cannot tell the two apart; the author of the contract does, for each line. A sweep with U at 0 is not
equivalence either: it covers the constants of the list above, by one in each direction, for these cases.

"Made" is memory that the setup of the case wrote through `State.write` (so through `w8`, `w16`,
`w32` and the helpers that call them, the recorders of `contracts.CallLog` among them, which
write code and log through `State`), each block that `State.alloc` handed out, whole, and what
the setup marked with `State.owns(address, size)`. `owns` writes nothing: a setup uses it for
memory whose content from the image it wants to keep and that the function may write, and the
header of that function's contract says why. The padding that `alloc` adds to reach a multiple
of 4 bytes is not made. The function's own memory is the stack region [STACK_LOW, STACK_TOP),
the region the comparison leaves out, and the 16 bytes from the initial stack pointer upward,
[STACK_TOP, STACK_TOP + 16): under the calling convention (o32) they are the callee's argument
home area, where it may spill a0 to a3. The comparison still compares those 16 bytes; only the
audit does not count them. Memory that the setup made stays made wherever it lies, in the home
area too. The scratchpad has no such region. `owns` and `read`
refuse a negative size, and `alloc` a negative size or a block that does not fit the arena,
before they change anything; every method refuses a span that does not lie inside the RAM or
the scratchpad; a span of no bytes is valid and makes nothing. The record
of what was made has one mark per byte of RAM and of scratchpad, and the audit refuses a state
whose record has another size.

What the audit does not see: a store of the value that is already there changes no byte, so
it is not counted; reads are not audited; "made" says that the setup touched or allocated the
memory, not that it filled it with varied content; and only the cases of the given seed are
run. A K above 0 means the function runs past a table the setup built, or writes a table or
a global that the setup did not make, or the setup leaves a table unfilled that the function
writes: the setup is to be changed, or the contract's header is to name the exception.

The size of a resident function comes from `ps1/inventory/game.tsv` or `library.tsv`, whichever lists
its address, and an address listed in neither or in both is an error (status 2).

The option --record writes a few fixtures of one function, and --replay tests the function's C on them
without running the original code. A fixture is one case of the function in terms of what it does:

    args          the argument registers the setup gave
    reads         every byte the original FUNCTION's own instructions read before anything wrote it,
                  as [address, hex bytes] runs; what an original callee (below) reads is not a read of
                  the function;
                  the function's stack region, the 16 bytes above it and the harness's own memory (the
                  call log, the recorders' code and cells) are left out. What a recorder copies into the
                  log is not a read of the function. The one exception is a word that a recorder reads
                  and then writes itself (`counts`): its first value is kept, the replay's recorder
                  needs it
    same          the input value of bytes the original stored and did not read first, when the store
                  changed nothing or a recorder saw the byte (a watched block or pointee): the
                  replay's memory holds them beforehand, or the store would look like a change; and the
                  bytes of the case's input that only an original callee read (format 2)
    recorders     the recorder of each callee that the contract replaced: name (the symbol table's, else the address), address,
                  number of arguments, the options of the recorder that are not return values
                  (pointees, masks, stores, counts, fills, ends_run_at) and, for a recorder in a block of
                  the setup's own, the pointer cell (the aligned word of `reads` that holds its address)
    watch         the watched blocks of the call log
    callees       (format 2, only in a fixture that has an original call) the callees that run as the game's
                  own code in the original run, as {at, callee}: where the replay puts their stand-ins
    calls         the calls made, in order, recorders' and original callees' together (see below): callee, arguments, the value the recorder returned and, for
                  the words behind each pointer argument and for each watched block, the bytes that
                  differ at that call from the same memory in the case's INPUT state (the memory before
                  the function ran), as [offset in the block, hex] spans, empty when nothing differs. Not
                  a chain of changes against the previous call: each call reads alone. In words: "at
                  this call these bytes of the block had been changed to these values"
    writes        every byte of RAM and scratchpad that differs after the run from before it, outside
                  the same regions as `reads`, as runs
    result        v0, when the contract says there is a result; null otherwise
    ends_in_callee  the run ended inside a recorder (a function that never returns)
    registers     only when the original did not restore the saved registers and sp
    note, seed, case  why the case was kept, and where it came from

FILLS. A recorder of the contract can store given bytes through one of its register arguments (`fills` of
`CallLog.replace`, [call, argument, offset, hex of the data] in the file; the format number is the same). At the
call it stores the data at the argument's value plus the offset, after its entry is logged and after its `stores`
and `counts`. The address is the argument's value at the call, which is how it stands for a callee that fills a
local of the function (the two codes place a local differently; give the argument the mask 0). Limits: register
arguments only, and the data is chosen by the contract, not computed from the call. In a record the bytes a fill
wrote are, for what the function reads afterwards, memory that something wrote earlier: not in `reads`, not memory
the setup did not make. Like a `stores` word they are the stand-in's, not the function's, found by the same hook:
they are in `writes` when they lie outside the stack region and change a byte (the replay's recorder makes them
again), and the comparison and the write audit count them as they count any byte of memory. The reader refuses a
`fills` that is not of the form or is out of range for its recorder (status 2). The native replay does not replay
a function whose recorders have fills.

MASKED POINTEES. A recorder argument with a mask of 0 and a pointee is the address of a local of the function (the log
holds 0 for it). At the stand-in's entry the record reads the argument's real value (a register, or the caller's
stack for the fifth on). When the pointee's whole block lies in the stack region of a case (STACK_LOW to the top
and the home area: not recorded, zero at the start, not input) the call holds the bytes as changes against zeros;
otherwise the function is refused (`the callee F, argument I (from 0): it is masked to 0 and has a pointee ...`,
status 1, no file). The replay compares against zeros and fails a build whose real pointer is not wholly in that
region. The native replay does not replay such a function.

ORIGINAL CALLEES. A callee that the contract did not replace (`CallLog.replace`) runs as the game's own code
in the original's run: an ORIGINAL CALLEE. Its call begins at the first instruction that lies outside the
function, outside every recorder (its code and its stub at the callee's address) and is not the stop address,
when the instruction before it was inside the function; it ends when an instruction inside the function is about
to run again, or with the run. Everything in between belongs to the call, the calls it makes itself included;
the entries that recorders make in the log during a call are not in `calls` (the stand-in replaces the whole
call). The tool tells the begin and the end by the instruction hook alone (the same one that finds the reads).
Each call is an entry of `calls`, at its place among the recorders' calls, with

    callee, original   the symbol's name, else the address as text; `original` is true
    uses               the registers of the caller that the callee read while they were UNTOUCHED (see below),
                       as {name: value at the begin of the call}: at, v0, v1, a0 to a3, t0 to t9, gp, hi, lo.
                       (A register counts as read when an executed instruction of the call has it as a source:
                       `registers_of` is the table of the MIPS I instructions of the game, hi and lo included;
                       an instruction it does not know is an error naming the word, status 2, not a guess.)
                       At replay the stand-in requires each named register to hold its value (`call N to F:
                       register NAME is X, the original gave Y`; for a0 to a3 `argument NAME`). The whole register
                       is compared: a callee that reads only part of an argument register (a byte or a halfword)
                       leaves the other bits free and two codes may differ there, but the stand-in compares all 32
                       bits, so such a function fails the check of its own record and is not recorded (a stated limit)
    stack              the words at or above the call's stack pointer that the callee read through sp, as
                       [offset from that stack pointer, hex] (its fifth and later arguments)
    reads              the bytes outside the stack region that the callee read before writing them and that
                       the setup made or the run had written before the read, as [address, hex] runs. A byte
                       that the setup did not make and nothing wrote (content of the game's image) is stored
                       nowhere: not here, not in the fixture's `reads`, and it is not counted as a read of the
                       function (no warning). A byte of these that the function did not read goes to `same`
    effects            the bytes outside the stack region that differ at the end of the call from the begin,
                       as [address, hex] runs (what the recorders called inside did to memory included)
    leaves             {name: value at the end of the call} for exactly the registers that are WRITTEN then, among
                       at, v0, v1, a0 to a3, t0 to t9, gp, s0 to s7, fp, hi and lo. At replay the stand-in sets
                       exactly these and no other register: an untouched register keeps the build's own value
                       (the stand-in never injects the recorded caller's state). ra, k0, k1 and sp are never in it

REGISTERS OF A CALL. For each register (1 to 31, hi and lo) the trace keeps one of two states during a call: UNTOUCHED
(it holds the value it had at the begin) or WRITTEN. The callee's own memory is its frame (the stack below the call's
stack pointer) and its home area. A `sw R` of an UNTOUCHED R into the callee's own memory is a SAVE: it is no use of R,
and each of the word's four bytes is from then on a COPY of R (of that byte of the value R had at the begin). A
register may have any number of copies at the same time. A byte stops being a copy when a store writes that byte, and
only then: a store of one byte into a saved word leaves the other three copies. A `lw R` over four bytes that are R's
own copy, in order, is a RESTORE: R is UNTOUCHED again. Any other load of a byte that is still a copy, whatever the
register it loads into and however many bytes it loads, is a use of the register the byte is a copy of. Any other read
of an UNTOUCHED register by an executed instruction is a use of it, except `jr ra` reading ra and register 0. A use of
s0 to s7, fp, k0, k1, or of ra other than by `jr ra`, refuses the case: `the original callee F reads register NAME of its
caller, which is no argument`. A call that ends with another stack pointer than it began with refuses the case: `the
original callee F leaves the stack pointer changed`.
The stack pointer may be read as the base of a load or a store and by the instruction that moves it (`addiu sp, sp,
N`). Read in any other way (copied, added to, stored as a value) its value goes somewhere else, and what the callee
then makes of it is an address of its stack, which is another one under a build and which nothing here could check: the
case is refused, `the original callee F takes the address of its stack`. A callee that loads a byte of its own frame
(below the call's stack pointer) that it has not written during this call reads what earlier code left on the stack,
which is other code under a build: the case is refused, `the original callee F reads its own frame at ADDRESS before
writing it`. (`--control` alters the build and `--edges` alters the original; neither takes part here.)

The 16 bytes from the call's stack pointer upward are the callee's own argument home area (o32, as for the function
itself): its writes there are neither effects nor refusals, and a read there of a byte it wrote itself is not a read
of the call; a read there before any write of its own is a `stack` word (offset below 16). A case is refused (one line
saying which, status 1, no file written, an existing file left alone) when an original callee writes the stack region
at or above the call's stack pointer plus 16, or reads it at or above the call's stack pointer other than by reads
through sp (`stack`; a pointer to a local of the function is such a read the build cannot repeat, the frames
differ), when it reads a register of its caller that is no argument, when it leaves sp changed, when it is
entered again (a `jal` or `jalr` to its first instruction) before it ended, or when it does not end when the run
does while the setup says the function returns. These are limits of this piece. A function whose callee is a
`tail` recorder still cannot be described (status 2).

The file is `FUNC.fixtures.json` beside `FUNC.c`: the format (1; 2 when a fixture has an original call), the
function, a `recorded` object (date, cases, seed and the line of the default comparison), the fixtures, and
`uncovered` (the slots of the original that no kept case executed, and the constants of `--edges` that no kept case
could cover). Keys are sorted, one fixture to a line, hex in lower case; the same input writes the same bytes. A
file whose fixtures call no original callee is format 1 and is what this tool wrote before format 2 existed.

--record runs the default comparison of the function first (it must show `different 0` and no discarded
case; otherwise nothing is written and the status is 1) and then chooses cases, in this order, until M
(default 12) are kept: (a) each case, in order, that executes a slot of the original that no kept case
executed (note `slots: N new`); (b) for each constant of the --edges sweep and each direction, the first
case that notices the alteration (note `edge: slot I, immediate 0xOLD against 0xNEW`; a case already kept
gets the note as a second reason); (c) when the function has a result and fewer than M are kept, the first
case for each result value that no kept case has (note `result 0xV`). It prints

    FUNC fixtures: kept K of N cases (slots A, edges B, results C)

A and B and C count the cases kept for that reason first. A line `FUNC fixtures: warning, N bytes read were
not made by the setup: ...` comes before it when kept cases read memory that the setup did not make, or
memory that it marked with `State.owns` (made for the audit of the writes, with the image's content left in
it): such bytes are the game's. The reads of an original callee count like the function's own: the bytes
themselves are not kept as reads of the call, but what the callee makes of them is in the file (the bytes it
stores, the registers it leaves). The file is written all the same, and it is not for publication. The edge runs of (b) are made as --edges makes
them (altered ORIGINAL code, `--jobs N` at a time, the altered original's own budget); the build is not
altered. An altered run whose process ended without a result gives no case, and its line is among the
uncovered edges of the file. A file that exists is replaced, and the tool says so. A setup with more than one CallLog, or a recorder with a `tail`, cannot be described:
status 2. The first three cases are looked at for what refuses a function (the refusals of the original callees
above) before the altered runs are made, which are the long part; the chosen cases are looked at again when they
are written. After choosing and before writing, each kept fixture is replayed on the build the tool already has
(the same replay as --replay, below), for every function with or without original callees; a fixture that fails is
a refusal of the function: `FUNC fixtures: not recorded, the replay of case N fails: FIRST LINE`, status 1, no file.
No fixture has a `needs_image` field (an earlier draft had one; no published file has it), and --replay refuses a
file that has it (status 2).

--replay builds FUNC.c, and for each fixture puts poison bytes (0xA5, the stack zero as in a case) in memory,
puts `reads` and `same` in place (a byte of `writes` whose input value the fixture does not give starts as its
expected value with every bit flipped, never as the filler, so that a missing store shows whatever its value), puts a recorder at each callee that returns the recorded values in order, puts a STAND-IN at each address of
`callees`, runs the BUILD, and requires
the same calls in the same order with the same logged arguments and returned values, the same changes
to each watched block and pointee as the fixture says (the replay's own input state, poison where the fixture
says nothing, with the changes over it: a build that changes a watched byte the original had not changed by
that call, or does not change one it had, fails; the replay's recorders and their log are put above every
address that the fixture names or that a recorder copies from, the end of a watched block and of the words
behind a pointer argument included, so that they never lie inside what is compared), the result, the writes exactly (each byte of
`writes` has that value afterwards, and no other byte outside the stack and the harness changed) and the saved
registers and sp as the original left them. It prints `FUNC replay: fixtures K, passed P, failed F`
and, per failed fixture, a line naming it and at
most three lines saying what differed first. The status is 0, 1 for a failed fixture, 2 for a missing or
malformed file (format 1 or 2 is read; a format 1 file with an original call is malformed), 3 when the build fails. The original code is never run in this mode and the contract is not
read. Every address executed during a replay must lie in the build, in a recorder, in a stand-in or be the stop address (a
build that calls an address without a recorder or a stand-in fails). The replay never loads the game's image and uses no byte of
the game, but it starts like every mode: it loads the configuration, which reads the baseline executable to check its hash, so it needs the private
inputs of the configuration, and it builds the C with the private PS1 compiler. A replay that needs nothing
private is a later piece (the C as compiled for a PC).

A stand-in is two words at the callee's address, `jr ra; nop` (harness memory: not compared, may run), and an
instruction hook at that address, placed like the one that guards the log. When the build reaches it, the next call
of the fixture must be this callee's (the position of the call is the number of calls that recorders have made, which
the log's cursor tells, plus the stand-ins' calls so far; otherwise a line `call N: missing, the original called F`,
`call N: extra, the build called F` or `call N: the build called F, the original called G`). Then each register of
`uses` must hold its value (`call N to F: argument aK is X, the original gave Y`), each word of `stack` must be at
its offset from the build's stack pointer, and each byte of `reads` must be in memory (`call N to F: memory at A is
X there at this call, the original had Y`); then the `effects` are written and the registers of `leaves` are set,
and the two words return. The recorders' own calls keep going through the call log. The original callee's code is
never run in this mode, and neither are the recorders that it called inside: the stand-in replaces the whole call.

What replay shows: for the recorded inputs the C reads, calls and writes what the original did. It is evidence
for those inputs, not the wide comparison and not equivalence. A fixture goes stale when the C changes what it
reads (it then reads poison), calls or writes; nothing regenerates a fixture except a new --record.

The memory hooks of the emulator are not used to find the reads and writes: with one installed, Unicorn 2.1.4
ends a run with an exception when a load or store lies in the delay slot of a conditional branch that is not
taken and a jump follows it. The tool looks at each instruction before it runs (an instruction hook) and
decodes the loads and stores.

The private inputs (the baseline executable and the module archive that
the build configuration names) are read through the configuration; the
tool stops with a message when they are absent.

Exit status: 0 when every function passes, 1 for a difference (or, with
--control, when the control does not trip), 2 for a configuration or input
problem, 3 when the build fails.
"""

from __future__ import annotations

import argparse
import bisect
import concurrent.futures
import dataclasses
import datetime
import importlib.util
import json
import multiprocessing
import multiprocessing.connection
import random
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parents[1] / "tools"))
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(HERE.parent / "programs"))

import matchbuild  # noqa: E402
from elftools.elf.elffile import ELFFile  # noqa: E402
import contracts  # noqa: E402
import programs  # noqa: E402
from unicorn import UC_ARCH_MIPS, UC_HOOK_BLOCK, UC_HOOK_CODE, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32, Uc, UcError  # noqa: E402
from unicorn import mips_const as reg  # noqa: E402

RAM_BASE = 0x80000000
RAM_SIZE = 0x200000
SCRATCH_BASE = 0x1F800000
SCRATCH_SIZE = 0x1000  # one page; the console's scratchpad is the first 0x400 bytes
TEST_ADDRESS = 0x80400000  # the build is linked here; physical address 0x400000, outside RAM
TEST_SIZE = 0x10000
STOP_ADDRESS = TEST_ADDRESS + 0xF000  # ra of the calls; never executed
ARENA_BASE = 0x80040000  # free RAM below the resident executable; blocks of the setup
ARENA_END = 0x80100000
STACK_TOP = 0x801FF000  # initial sp
STACK_LOW = 0x801FE000  # the stack region [STACK_LOW, STACK_TOP) is not compared
BUDGET = 2_000_000
HOME_AREA = 16  # the callee's argument home area, sp to sp + 15 on entry (o32): the --writes audit counts it as the function's own
PAGE = 0x1000  # the unit of the first pass of the memory comparison
SAVED = (reg.UC_MIPS_REG_S0, reg.UC_MIPS_REG_S1, reg.UC_MIPS_REG_S2, reg.UC_MIPS_REG_S3,
         reg.UC_MIPS_REG_S4, reg.UC_MIPS_REG_S5, reg.UC_MIPS_REG_S6, reg.UC_MIPS_REG_S7, reg.UC_MIPS_REG_FP)
SAVED_NAMES = ("s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "fp")
PROGRAMS_TOML = HERE.parent / "programs" / "programs.toml"  # the declaration of the disc's stand-alone programs
ARGS = (reg.UC_MIPS_REG_A0, reg.UC_MIPS_REG_A1, reg.UC_MIPS_REG_A2, reg.UC_MIPS_REG_A3)


class InputError(Exception):
    """A configuration or private input problem."""


class State:
    """RAM and scratchpad of one case, addressed by console addresses."""

    def __init__(self, ram: bytes, scratch: bytes, gp: int = 0):
        self.ram = bytearray(ram)
        self.scratch = bytearray(scratch)
        self.gp = gp  # the global pointer register at the begin of a run: a program's own, 0 for the game
        self.arena = ARENA_BASE
        self.stop = STOP_ADDRESS  # where a run ends; a recorder that ends the run jumps there
        self.made_ram = bytearray(RAM_SIZE)  # 1 for each byte that the setup made (see `owns`)
        self.made_scratch = bytearray(SCRATCH_SIZE)
        self.call_logs: list = []  # the `contracts.CallLog`s of the setup, for `--record`
        self.kept: list[tuple[int, int]] = []  # what the setup marked with `owns`: made, with the image's content

    def _locate(self, address: int, size: int) -> tuple[bytearray, int]:
        if size < 0:
            raise ValueError(f"size {size} is negative")
        if RAM_BASE <= address and address + size <= RAM_BASE + RAM_SIZE:
            return self.ram, address - RAM_BASE
        if SCRATCH_BASE <= address and address + size <= SCRATCH_BASE + SCRATCH_SIZE:
            return self.scratch, address - SCRATCH_BASE
        raise ValueError(f"address {address:#x} is outside RAM and the scratchpad")

    def write(self, address: int, data: bytes) -> None:
        block, offset = self._locate(address, len(data))
        block[offset : offset + len(data)] = data
        self._mark(block, offset, len(data))

    def _mark(self, block: bytearray, offset: int, size: int) -> None:
        # `_locate` has refused a negative size and a span outside the memory: a slice with a negative end
        # would shrink or shift the record instead of marking it.
        made = self.made_ram if block is self.ram else self.made_scratch
        made[offset : offset + size] = b"\1" * size

    def owns(self, address: int, size: int) -> None:
        """Mark [address, address + size) as made without writing it (see `--writes`)."""
        block, offset = self._locate(address, size)
        self._mark(block, offset, size)
        self.kept.append((address, size))

    def read(self, address: int, size: int) -> bytes:
        block, offset = self._locate(address, size)
        return bytes(block[offset : offset + size])

    def w8(self, address: int, value: int) -> None:
        self.write(address, struct.pack("<B", value & 0xFF))

    def w16(self, address: int, value: int) -> None:
        self.write(address, struct.pack("<H", value & 0xFFFF))

    def w32(self, address: int, value: int) -> None:
        self.write(address, struct.pack("<I", value & 0xFFFFFFFF))

    def alloc(self, size: int) -> int:
        """A zero-initialised block of free RAM, word aligned. The block is made; the padding after it is not."""
        if size < 0:
            raise ValueError(f"size {size} is negative")
        address = self.arena
        end = (address + size + 3) & ~3
        if end > ARENA_END:
            raise ValueError("the setup needs more free RAM than the arena holds")
        self.arena = end
        # Made like what `owns` marks, but not kept content of the image: the arena is free RAM.
        self._mark(*self._locate(address, size), size)
        return address


# ---------------------------------------------------------------------------
# Build


@dataclasses.dataclass(frozen=True)
class Build:
    """A linked nonmatching unit."""

    code: bytes  # what is loaded at TEST_ADDRESS: the unit's code, then its read-only data
    size: int  # bytes of code
    entry: int  # offset of the function in the code; a unit may define helpers before it


WRITABLE = (".data", ".sdata", ".bss", ".sbss")
# A line of a linker symbol file that gives a name a value, plain or inside PROVIDE().
ASSIGNMENT = re.compile(r"\s*(?:PROVIDE\s*\(\s*)?([A-Za-z_.$][\w.$]*)\s*=[^=]")


def build_function(cfg, name: str, directory: Path, folder: Path = HERE) -> Build:
    """Compile `name`.c of `folder` and link it at TEST_ADDRESS.

    A unit may hold helper functions and read-only data (a jump table, a
    constant table). It may not define writable data: the game's data lies
    in the image, and data of the unit's own would keep its values from one
    case to the next.
    """
    source = folder / f"{name}.c"
    if not source.is_file():
        raise InputError(f"no source {source.name} in {folder.name}")
    report = {"inputs": {}, "cache": {"units": {}}}
    pipeline, failures = matchbuild.prepare_pipeline(cfg, "difftest", directory, None, report)
    if failures:
        raise matchbuild.StepError("; ".join(failures))
    report["inputs"]["preprocessed"] = {}
    unit = matchbuild.UnitDecl(
        name=name,
        source=str(source),
        flags=("-O2", "-G0"),
        functions=(matchbuild.FunctionDecl(name, TEST_ADDRESS, 0),),
    )
    errors, _ = matchbuild.unit_object(pipeline, unit, directory, report)
    # The object is checked here as a unit of the build that declares no size and no data. Those
    # findings are expected: the code's size is free, and what data a unit may hold is decided below.
    errors = [e for e in errors if "text size mismatch" not in e and "must be declared" not in e]
    if errors:
        raise matchbuild.StepError("; ".join(errors))
    symbols = matchbuild.defined_symbols(directory / f"unit-{name}.o")
    defined = {s.name for s in symbols}
    entry = next((s.offset for s in symbols if s.name == name and s.kind == "text"), None)
    if entry is None:
        raise InputError(f"{source.name} does not define the function {name}")
    sizes = section_sizes(directory / f"unit-{name}.o")
    written = [f"{section} ({sizes[section]} bytes)" for section in WRITABLE if sizes.get(section)]
    if written or any(s.section == "SHN_COMMON" for s in symbols) or sizes.get("COMMON"):
        raise InputError(f"{source.name} defines writable data: {', '.join(written) or 'a common symbol'}")
    # Declared functions of the build that the code may call resolve to their original addresses.
    # A program's own names (its data, its functions) hold for that program's functions only and
    # win over the tree's symbol file and over the build's declared functions there.
    extra = program_symbols(program_for(cfg, image_of(name)))
    known = set(cfg.symbol_values) | set(extra)
    lines = [f"{fn.name} = {fn.address:#x};\n" for u in cfg.units for fn in u.functions
             if fn.name not in defined and fn.name not in known]
    (directory / "others.ld").write_text("".join(lines))
    # The tree's symbol file places names at their original addresses, the parked function's
    # own name among them. An assignment there would win over a definition of the unit, and a
    # call by that name would run the ORIGINAL code: every name the unit defines is taken out.
    (directory / "symbols.ld").write_text(link_symbols(Path(cfg.symbols_path).read_text(), defined, extra))
    discard = " ".join(f"*({s})" for s in matchbuild.DISCARDED_SECTIONS)
    (directory / "link.ld").write_text(
        "OUTPUT_ARCH(mips)\n"
        f'INCLUDE "{(directory / "symbols.ld").resolve()}"\n'
        f'INCLUDE "{(directory / "others.ld").resolve()}"\n'
        "SECTIONS {\n"
        f" .text {TEST_ADDRESS:#x} : SUBALIGN(1) {{ unit-{name}.o(.text) }}\n"
        f" .rodata : {{ unit-{name}.o(.rodata*) }}\n"
        f" /DISCARD/ : {{ {discard} *(.note*) }}\n"
        "}\n"
    )
    prefix = cfg.binutils_prefix
    matchbuild.run([prefix + "ld", "-EL", "-T", "link.ld", "-e", f"{TEST_ADDRESS:#x}", "-o", "image.elf", f"unit-{name}.o"],
                   cwd=directory, step="link")
    matchbuild.run([prefix + "objcopy", "-O", "binary", "image.elf", "image.bin"], cwd=directory, step="objcopy")
    code = (directory / "image.bin").read_bytes()
    if len(code) > STOP_ADDRESS - TEST_ADDRESS:
        raise InputError(f"{source.name} builds {len(code)} bytes, more than the test area holds")
    # Fail closed: whatever the link did, a name the unit defines must lie in the unit.
    astray = misplaced(linked_addresses(directory / "image.elf"), defined, TEST_ADDRESS, TEST_ADDRESS + len(code))
    if astray:
        raise InputError(f"{source.name}: the link placed outside the unit what the unit defines: {', '.join(astray)}")
    # And a name of the program must lie where the program says, not where the tree's file puts it.
    wrong = misvalued(linked_addresses(directory / "image.elf"), {n: a for n, a in extra.items() if n not in defined})
    if wrong:
        raise InputError(f"{source.name}: the link did not place the program's names as declared: {', '.join(wrong)}")
    return Build(code, sizes.get(".text", 0), entry)


def without_assignments(text: str, names: set[str]) -> str:
    """A linker symbol file without the lines that assign one of `names`."""
    kept = []
    for line in text.splitlines(keepends=True):
        match = ASSIGNMENT.match(line)
        if match is None or match.group(1) not in names:
            kept.append(line)
    return "".join(kept)


def link_symbols(tree_text: str, defined: set[str], extra: dict[str, int]) -> str:
    """The symbol file of one link: the tree's file without the lines that assign a name the unit
    defines or a name of the program, then the program's names at their addresses (a name the unit
    defines is not among them: the unit's own definition stands)."""
    kept = without_assignments(tree_text, defined | set(extra))
    return kept + "".join(f"{name} = {address:#x};\n" for name, address in extra.items() if name not in defined)


def misvalued(linked: dict[str, int], wanted: dict[str, int]) -> list[str]:
    """The names of `wanted` that the link placed, but not at the address wanted. A name the link does
    not carry is not placed: a unit that uses it would not have linked."""
    return [f"{name} at {linked[name]:#x}, declared {address:#x}" for name, address in sorted(wanted.items())
            if name in linked and linked[name] != address]


def linked_addresses(elf_path: Path) -> dict[str, int]:
    """The address of every named symbol of a linked file."""
    with open(elf_path, "rb") as handle:
        symtab = ELFFile(handle).get_section_by_name(".symtab")
        return {sym.name: sym["st_value"] for sym in symtab.iter_symbols() if sym.name} if symtab else {}


def misplaced(linked: dict[str, int], names: set[str], low: int, high: int) -> list[str]:
    """The names of `names` that the link did not place in [low, high), with where it put them."""
    return [f"{name} at {linked[name]:#x}" if name in linked else f"{name} nowhere"
            for name in sorted(names) if not low <= linked.get(name, -1) < high]


def section_sizes(obj_path: Path) -> dict[str, int]:
    """The size of each section of an object, and of its common symbols under the name COMMON."""
    with open(obj_path, "rb") as handle:
        elf = ELFFile(handle)
        sizes = {section.name: section["sh_size"] for section in elf.iter_sections()}
        symtab = elf.get_section_by_name(".symtab")
        common = sum(sym["st_size"] for sym in symtab.iter_symbols() if sym["st_shndx"] == "SHN_COMMON") if symtab else 0
    if common:
        sizes["COMMON"] = common
    return sizes


# ---------------------------------------------------------------------------
# Memory image and emulation


def initial_memory(cfg, image: str | None) -> bytes:
    """RAM with the resident executable and the module image `image` (None: none) at their load addresses.

    Several module images share one address, so exactly the image of the function is loaded.
    The body of a stand-alone program (`image` names one of `programs.toml`) is loaded over the resident.
    """
    ram = bytearray(RAM_SIZE)
    images = [(cfg.load, cfg.payload)] + [(i.address, i.payload) for i in cfg.images if i.name == image]
    program = program_for(cfg, image)
    if program is not None:
        # A program replaces what the resident executable holds at its addresses, as `Exec` does; the
        # resident stays where the body does not reach. The file's hash and header are checked first.
        try:
            images.append((program.address, programs.read_body(program)))
        except programs.ProgramError as exc:
            raise InputError(str(exc)) from exc
    for address, data in images:
        offset = address - RAM_BASE
        if not data or offset < 0 or offset + len(data) > RAM_SIZE:
            raise InputError(f"an image of {len(data)} bytes does not fit RAM at {address:#x}")
        ram[offset : offset + len(data)] = data
    return bytes(ram)


def machine(code: bytes) -> Uc:
    uc = Uc(UC_ARCH_MIPS, UC_MODE_MIPS32 | UC_MODE_LITTLE_ENDIAN)
    # Unicorn translates the console's kseg0 addresses to physical ones.
    uc.mem_map(0, RAM_SIZE)
    uc.mem_map(SCRATCH_BASE, SCRATCH_SIZE)
    uc.mem_map(TEST_ADDRESS & 0x1FFFFFFF, TEST_SIZE)
    # The host side of Unicorn takes physical addresses.
    uc.mem_write(TEST_ADDRESS & 0x1FFFFFFF, code)
    uc.mem_write(STOP_ADDRESS & 0x1FFFFFFF, b"\0\0\0\0")
    return uc


def run_once(uc: Uc, state: State, entry: int, setup: contracts.Setup, blocks: set | None = None,
             watchers: tuple = (), budget: int | None = None, steps: list | None = None,
             standins: tuple = ()) -> dict | str:
    """Run one call on `state`. Returns the final state, or a text naming the failure.

    With `blocks`, every block of code that the run enters is added to it as (address, size).
    `watchers` are (hook type, callback) pairs that are put on the emulator for this run only
    (`--record` uses them for the memory reads and writes). `budget` is the instruction budget of
    this run when it is not the general one. With `blocks` and `steps`, the instructions of the blocks
    the run entered are appended to `steps` as one number (how long the run was). `standins` are
    (address, callback) pairs: an instruction hook limited to that one address, for this run only (`--replay`
    puts the stand-in of an original callee there).
    """
    uc.mem_write(0, bytes(state.ram))
    uc.mem_write(SCRATCH_BASE, bytes(state.scratch))
    # A setup may write code (a recorder in a callee's place) that differs from case to case.
    # Unicorn keeps translated code across a write to memory: without this the first case's code would run again.
    uc.ctl_flush_tb()
    for number in range(reg.UC_MIPS_REG_0, reg.UC_MIPS_REG_31 + 1):
        uc.reg_write(number, 0)
    for index, number in enumerate(SAVED):
        uc.reg_write(number, 0x5A5A0000 + index)
    for number, value in zip(ARGS, setup.args):
        uc.reg_write(number, value)
    uc.reg_write(reg.UC_MIPS_REG_RA, STOP_ADDRESS)
    uc.reg_write(reg.UC_MIPS_REG_SP, STACK_TOP)
    uc.reg_write(reg.UC_MIPS_REG_GP, state.gp)
    hooks = []
    entered = [0]
    if blocks is not None:
        def enter(_uc, address, size, _data):
            blocks.add((address & 0xFFFFFFFF, size))
            entered[0] += size // 4

        hooks.append(uc.hook_add(UC_HOOK_BLOCK, enter))
    for kind, callback in watchers:
        hooks.append(uc.hook_add(kind, callback))
    for address, callback in standins:
        hooks.append(uc.hook_add(UC_HOOK_CODE, callback, begin=address, end=address))
    # A recorder writes its entry without looking at the room that is left, so a run that calls without end
    # (an original with an altered constant, say) would write past the log, over the recorders' own code and
    # whatever follows. The run is ended at the call whose entry would not fit, before the recorder runs.
    # Each call of a recorder adds an entry of a known size and a run starts with an empty log, so the room
    # is counted down here and the emulator's memory is not read at each call (a function may call a
    # recorder some thousand times in one case).
    full: list[int] = []

    def guard(left, size, address):
        def at_call(emulator, _address, _size, _data):
            left[0] -= size
            if left[0] < 0:
                full.append(address)
                emulator.emu_stop()
        return at_call

    for log in state.call_logs:
        left = [log.end - log.entries]
        for address in log.recorders:
            hooks.append(uc.hook_add(UC_HOOK_CODE, guard(left, 4 * log.entry_words(address), address), begin=address, end=address))
    try:
        uc.emu_start(entry, STOP_ADDRESS, count=BUDGET if budget is None else budget)
    except UcError as exc:
        return f"fault ({exc})"
    finally:
        for hook in hooks:
            uc.hook_del(hook)
    if blocks is not None and steps is not None:
        steps.append(entered[0])
    if full:
        return f"the call log is full at a call of {full[0]:#x} (the run makes more calls than the log has room for)"
    if uc.reg_read(reg.UC_MIPS_REG_PC) != STOP_ADDRESS:
        return "instruction budget exceeded"
    return {
        "ram": uc.mem_read(0, RAM_SIZE),
        "scratch": uc.mem_read(SCRATCH_BASE, SCRATCH_SIZE),
        "v0": uc.reg_read(reg.UC_MIPS_REG_V0),
        "saved": [uc.reg_read(n) for n in SAVED],
        "sp": uc.reg_read(reg.UC_MIPS_REG_SP),
    }


def differences(a: dict, b: dict, returns_value: bool, returns: bool = True) -> list[str]:
    """What differs between two final states, as text lines (empty when equal).

    With `returns` false the runs were ended inside the function, and no register is compared.
    """
    found = []
    if returns and returns_value and a["v0"] != b["v0"]:
        found.append(f"v0: original {a['v0']:#x}, build {b['v0']:#x}")
    for name, x, y in zip(SAVED_NAMES, a["saved"], b["saved"]):
        if returns and x != y:
            found.append(f"{name}: original {x:#x}, build {y:#x}")
    if returns and a["sp"] != b["sp"]:
        found.append(f"sp: original {a['sp']:#x}, build {b['sp']:#x}")
    low, high = STACK_LOW - RAM_BASE, STACK_TOP - RAM_BASE
    for label, base, x, y, skip in (("ram", RAM_BASE, a["ram"], b["ram"], (low, high)),
                                    ("scratchpad", SCRATCH_BASE, a["scratch"], b["scratch"], (0, 0))):
        # Whole pages are compared first: a byte loop over all of RAM costs seconds a case.
        for start in range(0, len(x), PAGE):
            if x[start : start + PAGE] == y[start : start + PAGE]:
                continue
            for i in range(start, min(start + PAGE, len(x))):
                if x[i] != y[i] and not skip[0] <= i < skip[1]:
                    found.append(f"{label} {base + i:#x}: original {x[i]:#04x}, build {y[i]:#04x}")
    return found


def image_of(name: str) -> str | None:
    """The module image in the name `func_<address>_<image>`, None for the resident executable."""
    return name.split("_", 2)[2] if name.count("_") >= 2 else None


def original_function(cfg, name: str) -> tuple[int, int]:
    """Address and size of the original function that `name` stands for.

    The name is `func_<address>` for the resident executable or
    `func_<address>_<image>` for a module image. The address is in the name;
    the size comes from the published function inventory (`ps1/inventory/`),
    because a nonmatching function is not declared in the build. A resident
    name is looked up in `game.tsv` and in `library.tsv` and must be in exactly one. When the image part is a
    program of `programs.toml` the function must be listed there, and the toml gives its size.
    """
    match = re.fullmatch(r"func_([0-9a-f]{8})(?:_(\w+))?", name)
    if match is None:
        raise InputError(f"{name} is not named func_<address>[_<image>]")
    address = int(match.group(1), 16)
    inventory = HERE.parents[1] / "inventory"
    if match.group(2) is None:
        sizes = []
        for table in ("game.tsv", "library.tsv"):
            rows = [line.split("\t") for line in (inventory / table).read_text().splitlines()]
            sizes += [int(r[1]) for r in rows if int(r[0], 16) == address]
    elif program_for(cfg, match.group(2)) is not None:
        program = program_for(cfg, match.group(2))
        function = next((f for f in program.functions if f.name == name), None)
        if function is None:
            raise InputError(f"{name}: programs.toml lists no such function of the program {program.name!r}")
        return function.address, function.size
    else:
        image = next((i for i in cfg.images if i.name == match.group(2)), None)
        if image is None:
            raise InputError(f"{name}: the build declares no module image or program {match.group(2)!r}")
        rows = [line.split("\t") for line in (inventory / "modules.tsv").read_text().splitlines()]
        sizes = [int(r[3]) for r in rows if r[0] == image.archive.name and int(r[1], 16) == image.slot and int(r[2], 16) == address]
    if len(sizes) != 1:
        raise InputError(f"{name}: the inventory lists {len(sizes)} functions at {address:#x}")
    return address, sizes[0]


def slots_in(blocks: set, start: int, size: int) -> set[int]:
    """The instruction slots of [start, start + size) that lie in one of the entered blocks, as offsets."""
    slots: set[int] = set()
    for address, length in blocks:
        for at in range(max(address, start), min(address + length, start + size), 4):
            slots.add(at - start)
    return slots


def ranges_text(offsets: list[int]) -> str:
    """Sorted instruction offsets as text, runs joined: `+0x40..+0x4c, +0x88`."""
    parts = []
    for offset in offsets:
        if parts and parts[-1][1] + 4 == offset:
            parts[-1][1] = offset
        else:
            parts.append([offset, offset])
    return ", ".join(f"+{a:#x}" if a == b else f"+{a:#x}..+{b:#x}" for a, b in parts)


def declared_programs() -> tuple:
    """The programs of `programs.toml`; whatever the reader refuses is an input error."""
    try:
        return programs.load(PROGRAMS_TOML)
    except programs.ProgramError as exc:
        raise InputError(f"{PROGRAMS_TOML.name}: {exc}") from exc


def program_for(cfg, image: str | None):
    """The program that the image part of a function's name stands for, None for the resident executable
    and for a module image. A name that is both a module image and a program is ambiguous and refused."""
    if image is None:
        return None
    program = next((p for p in declared_programs() if p.name == image), None)
    if program is not None and any(i.name == image for i in getattr(cfg, "images", ())):
        raise InputError(f"{image!r} is both a module image of the build and a program of {PROGRAMS_TOML.name}")
    return program


def global_pointer(cfg, name: str) -> int:
    """The global pointer register at the begin of a run of the function `name`: the `gp` that the declaration
    gives the function's program (its entry code sets it before the program's main), and 0 for a program without
    one and for every function of the game, whose code does not use the register."""
    program = program_for(cfg, image_of(name))
    return program.gp if program is not None and program.gp is not None else 0


def program_symbols(program) -> dict[str, int]:
    """The names that hold inside a program: its symbols and its listed functions, each with its address."""
    if program is None:
        return {}
    table = {f.name: f.address for f in program.functions}
    table.update(program.symbols)
    return table


def addresses(cfg, name: str | None = None) -> dict[str, int]:
    """What a contract's setup is given as `sym`: the names of the tree's linker symbols
    and of the functions that the build declares, each with its address. For a function of a
    program (`name` given) the program's own names are added and win."""
    table = {fn.name: fn.address for unit in getattr(cfg, "units", ()) for fn in unit.functions}
    table.update(cfg.symbol_values)
    if name is not None:
        table.update(program_symbols(program_for(cfg, image_of(name))))
    return table


def test_function(cfg, name: str, code: bytes, cases: int, seed: int, ram: bytes, scratch: bytes, entry: int = 0,
                  stop_at_first_difference: bool = False, on_case=None, original_budget: int | None = None,
                  longest: list | None = None) -> tuple[int, int, int, list, set]:
    """Run the cases. `entry` is the offset of the function in `code`.

    Returns (discarded, equal, different, first difference report, executed): `executed` holds
    the offsets of the original function's instruction slots that the runs of the original
    which reached their end have executed. With `stop_at_first_difference` the loop ends after the
    first case that differs (only `--edges` asks for it; the counts are then those of the cases run).
    `on_case`, when given, is called as `on_case(case, setup, reference, slots, differs)` for every case
    that was not discarded: `slots` are the offsets of the original's instruction slots the case executed
    (`--record` uses it; the counts and the return value do not depend on it).
    `original_budget` is the instruction budget of the original's runs when it is not the general one
    (the altered runs of `--edges` give one). `longest`, when given, gets one number appended: the
    instructions of the longest run of the original that reached its end (0 when none did).
    """
    contract = contracts.CONTRACTS[name]
    original, size = original_function(cfg, name)
    uc = machine(code)
    sym = addresses(cfg, name)
    discarded = equal = different = 0
    first: list[str] = []
    executed: set[int] = set()
    most = 0
    gp = global_pointer(cfg, name)  # the declaration is read once for the run, not once per case
    for case in range(cases):
        rng = random.Random(f"{seed}:{name}:{case}")
        state = State(ram, scratch, gp)
        setup = contract.setup(state, rng, sym)
        blocks: set = set()
        steps: list = []
        # The two options are passed only when a caller of this function asked for one of them.
        extra = {"budget": original_budget, "steps": steps} if original_budget is not None or longest is not None else {}
        reference = run_once(uc, state, original, setup, blocks, **extra)
        if isinstance(reference, str):
            discarded += 1
            continue
        most = max([most, *steps])
        case_slots = slots_in(blocks, original, size)
        executed |= case_slots
        built = run_once(uc, state, TEST_ADDRESS + entry, setup)
        if isinstance(built, str):
            lines = [f"build: {built}"]
        else:
            lines = differences(reference, built, setup.returns_value, getattr(setup, "returns", True))
        if on_case is not None:
            on_case(case, setup, reference, case_slots, bool(lines))
        if lines:
            different += 1
            if not first:
                first = [f"first difference: case {case}, seed {seed}", *lines[:24]]
                if len(lines) > 24:
                    first.append(f"... and {len(lines) - 24} more")
            if stop_at_first_difference:
                break
        else:
            equal += 1
    if longest is not None:
        longest.append(most)
    return discarded, equal, different, first, executed


def outside_addresses(state: State, final: dict) -> list[int]:
    """The addresses of the bytes that the run changed and that are neither made nor in the stack region."""
    low, high = STACK_LOW - RAM_BASE, STACK_TOP + HOME_AREA - RAM_BASE
    if len(state.made_ram) != RAM_SIZE or len(state.made_scratch) != SCRATCH_SIZE:
        raise InputError("the record of what the setup made has another size than the memory it describes")
    found = []
    for base, before, after, made, skip in ((RAM_BASE, state.ram, final["ram"], state.made_ram, (low, high)),
                                            (SCRATCH_BASE, state.scratch, final["scratch"], state.made_scratch, (0, 0))):
        for start in range(0, len(before), PAGE):
            if before[start : start + PAGE] == after[start : start + PAGE]:
                continue
            for i in range(start, min(start + PAGE, len(before))):
                if before[i] != after[i] and not made[i] and not skip[0] <= i < skip[1]:
                    found.append(base + i)
    return found


def runs_text(found: list[int], sym: dict) -> str:
    """Addresses (RAM ones, then scratchpad ones, each ascending) as `0xA..0xB (NAME+0xOFF); ...`:
    runs of bytes less than 17 apart, six at most, then `and M more`."""
    names = sorted((v, k) for k, v in sym.items() if isinstance(v, int))
    runs = []
    for address in found:
        if runs and 0 <= address - runs[-1][1] <= 16:
            runs[-1][1] = address
        else:
            runs.append([address, address])
    parts = []
    for first, last in runs[:6]:
        region = RAM_BASE if first >= RAM_BASE else SCRATCH_BASE
        size = RAM_SIZE if first >= RAM_BASE else SCRATCH_SIZE
        i = bisect.bisect_right(names, (first, "~")) - 1
        near = f"{names[i][1]}+{first - names[i][0]:#x}" if i >= 0 and region <= names[i][0] < region + size else "?"
        parts.append(f"{first:#x}..{last:#x} ({near})")
    if len(runs) > 6:
        parts.append(f"and {len(runs) - 6} more")
    return "; ".join(parts)


def audit_writes(cfg, name: str, cases: int, seed: int, ram: bytes, scratch: bytes) -> tuple[int, int, int, str]:
    """Run the original alone on the cases and count those that change a byte outside what the setup made.

    Returns (discarded, outside, largest, first): `largest` is the most such bytes in one case;
    `first` is the text `case C; <runs>` of the first such case, empty when there is none.
    """
    contract = contracts.CONTRACTS[name]
    original, _size = original_function(cfg, name)
    uc = machine(b"\0" * 16)
    sym = addresses(cfg, name)
    discarded = outside = largest = 0
    first = ""
    gp = global_pointer(cfg, name)
    for case in range(cases):
        rng = random.Random(f"{seed}:{name}:{case}")
        state = State(ram, scratch, gp)
        setup = contract.setup(state, rng, sym)
        final = run_once(uc, state, original, setup)
        if isinstance(final, str):
            discarded += 1
            continue
        found = outside_addresses(state, final)
        if found:
            outside += 1
            largest = max(largest, len(found))
            if not first:
                first = f"case {case}; {runs_text(found, sym)}"
    return discarded, outside, largest, first


# ---------------------------------------------------------------------------
# Edges of the constants (--edges)

REGISTER_NAMES = ("zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
                  "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra")
SP, GP, RA_NUMBER = 29, 28, 31
# opcode -> mnemonic of the immediate instructions that the sweep may take
OP_ADDIU, OP_SLTI, OP_SLTIU, OP_ORI, OP_LUI = 0x09, 0x0A, 0x0B, 0x0D, 0x0F
EDGE_MNEMONICS = {OP_SLTI: "slti", OP_SLTIU: "sltiu", OP_ADDIU: "addiu", OP_ORI: "ori"}
# SPECIAL functions that write no register (jr, mult, div, mthi, mtlo, syscall, break)
SPECIAL_NO_WRITE = frozenset({0x08, 0x11, 0x13, 0x18, 0x19, 0x1A, 0x1B, 0x0C, 0x0D})
# SPECIAL functions that write rd
SPECIAL_WRITES_RD = frozenset({0x00, 0x02, 0x03, 0x04, 0x06, 0x07, 0x09, 0x10, 0x12, 0x20, 0x21, 0x22, 0x23,
                               0x24, 0x25, 0x26, 0x27, 0x2A, 0x2B})


def register_written(word: int) -> int | None:
    """The register that the instruction `word` writes, None when it writes none.

    Raises InputError for a word whose opcode or function this table does not know: the rule for the
    low half of a constant needs to know what a word writes, and it is not guessed.
    """
    op, rs, rt, rd, funct = word >> 26, (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31, word & 63
    if op == 0:
        if funct in SPECIAL_WRITES_RD:
            return rd
        if funct in SPECIAL_NO_WRITE:
            return None
    elif op == 1:  # REGIMM: bltzal and bgezal write ra
        return RA_NUMBER if rt in (0x10, 0x11) else None
    elif op == 3:  # jal
        return RA_NUMBER
    elif op in (2, 4, 5, 6, 7):  # j and the branches
        return None
    elif 0x08 <= op <= 0x0F or 0x20 <= op <= 0x26:  # arithmetic immediates, lui; loads
        return rt
    elif 0x28 <= op <= 0x2E or op in (0x32, 0x3A):  # stores; lwc2, swc2
        return None
    elif op == 0x10:  # COP0: mfc0 writes rt
        return rt if rs == 0 else None
    elif op == 0x12:  # COP2 (the GTE): mfc2 and cfc2 write rt
        return rt if rs in (0, 2) else None
    raise InputError(f"word {word:#010x}: the tool does not know which register it writes")


def is_low_half(words: list[int], index: int, reg_number: int) -> bool:
    """True when the nearest earlier word of `words` that writes `reg_number` is `lui reg_number`."""
    for earlier in range(index - 1, -1, -1):
        written = register_written(words[earlier])
        if written == reg_number:
            return words[earlier] >> 26 == OP_LUI
    return False


def edge_constant(words: list[int], index: int) -> tuple[str, int, int, int] | None:
    """(mnemonic, rs, rt, immediate) when word `index` of `words` is a constant of the sweep, else None."""
    word = words[index]
    op, rs, rt, imm = word >> 26, (word >> 21) & 31, (word >> 16) & 31, word & 0xFFFF
    if op in (OP_SLTI, OP_SLTIU):
        return EDGE_MNEMONICS[op], rs, rt, imm
    if op == OP_ADDIU:
        if SP in (rs, rt) or GP in (rs, rt):
            return None
        if rs == rt and is_low_half(words, index, rt):
            return None
        return "addiu", rs, rt, imm
    if op == OP_ORI and rs == 0:
        return "ori", rs, rt, imm
    return None


def edge_constants(words: list[int]) -> list[tuple[int, str, int, int, int]]:
    """The constants of the sweep in a function's words, as (index, mnemonic, rs, rt, immediate)."""
    found = []
    for index in range(len(words)):
        constant = edge_constant(words, index)
        if constant is not None:
            found.append((index, *constant))
    return found


def edge_neighbours(immediate: int) -> tuple[int, int]:
    """The immediate plus one and minus one, each modulo 0x10000."""
    return (immediate + 1) & 0xFFFF, (immediate - 1) & 0xFFFF


def operands_text(mnemonic: str, rs: int, rt: int, immediate: int) -> str:
    return f"{REGISTER_NAMES[rt]},{REGISTER_NAMES[rs]},{immediate:#x}"


def edge_line(index: int, mnemonic: str, rs: int, rt: int, old: int, new: int, different_of: int, discarded: int) -> str:
    return (f"  slot {index}: {mnemonic} {operands_text(mnemonic, rs, rt, old)}, immediate {old:#x} -> {new:#x}: "
            f"different 0 of {different_of}, discarded {discarded}")


def lost_line(index: int, mnemonic: str, rs: int, rt: int, old: int, new: int, status: int) -> str:
    how = f"signal {-status}" if status < 0 else f"status {status}"
    return (f"  slot {index}: {mnemonic} {operands_text(mnemonic, rs, rt, old)}, immediate {old:#x} -> {new:#x}: "
            f"no result, the process of this run ended ({how})")


# What the processes of the altered runs (or the one process, with `--jobs 1`) share: set by `edge_init`.
EDGE_CONTEXT: dict = {}
# The instruction budget of an altered original: this many times the longest run of the unaltered original,
# and at least the floor (never above the general budget).
EDGE_BUDGET_FACTOR = 8
EDGE_BUDGET_FLOOR = 20_000


def edge_budget(longest: int | None) -> int | None:
    """The budget of an altered original's run; None (the general budget) when the longest run is not known."""
    if longest is None:
        return None
    return min(BUDGET, max(EDGE_BUDGET_FLOOR, EDGE_BUDGET_FACTOR * longest))


def edge_init(context: dict) -> None:
    EDGE_CONTEXT.clear()
    EDGE_CONTEXT.update(context)


def edge_run(task: tuple[int, int, int]) -> tuple[int, int, int, int]:
    """One altered run: the default comparison with word `index` of the original replaced by one with
    the immediate `new`, ended at its first differing case. Returns (index, which, discarded, different).

    The image is altered in place and put back, whatever happens, before the next run of this process.
    """
    return edge_run_case(task)[:4]


def edge_run_case(task: tuple[int, int, int]) -> tuple[int, int, int, int, int | None]:
    """`edge_run` with a fifth value: the number of the first case that differs, None when none does."""
    index, which, new = task
    c = EDGE_CONTEXT
    at = c["base"] + 4 * index
    word = c["words"][index]
    image = c["image"]
    image[at : at + 4] = struct.pack("<I", (word & 0xFFFF0000) | new)
    try:
        noticed: list[int] = []
        gone, _equal, changed, _first, _executed = test_function(
            c["cfg"], c["name"], c["code"], c["cases"], c["seed"], bytes(image), c["scratch"], c["entry"],
            stop_at_first_difference=True, original_budget=c.get("budget"),
            on_case=lambda case, _setup, _reference, _slots, differs: noticed.append(case) if differs and not noticed else None)
    finally:
        image[at : at + 4] = struct.pack("<I", word)
    return index, which, gone, changed, noticed[0] if noticed else None


def edge_child(worker, task, send) -> None:
    """The body of the process of one altered run: sends ("result", value) or ("raise", exception)."""
    try:
        message = ("result", worker(task))
    except Exception as exc:  # the parent raises it again
        message = ("raise", exc)
    send.send(message)
    send.close()


def edge_runs(context: dict, tasks: list[tuple[int, int, int]], jobs: int, worker=edge_run) -> list[tuple]:
    """The results of `worker` (`edge_run`, or `edge_run_case` for `--record`) for the tasks, in the order
    they finish (the caller sorts them).

    With `jobs` above 1 every altered run is made in a forked process of its own, `jobs` of them at a time.
    An altered original can jump anywhere, and the emulator itself has ended (a signal) on what it found
    there: the process of that run then ends without a result, and the run's result here is
    (index, which, None, exit status of the process), with a fifth None for `edge_run_case`. With `jobs` 1
    the runs are made in this process, and such an end is the end of the tool.
    """
    if not tasks:
        return []
    if jobs == 1:
        edge_init(context)
        try:
            return [worker(task) for task in tasks]
        finally:
            EDGE_CONTEXT.clear()
    try:
        fork = multiprocessing.get_context("fork")
    except ValueError as exc:
        raise InputError("--jobs above 1 needs the fork start method of this system; use --jobs 1") from exc
    edge_init(context)  # the forked processes find it as it is here
    waiting, running, results = list(reversed(tasks)), {}, []
    try:
        while waiting or running:
            while waiting and len(running) < jobs:
                task = waiting.pop()
                receive, send = fork.Pipe(duplex=False)
                child = fork.Process(target=edge_child, args=(worker, task, send))
                child.start()
                send.close()
                running[receive] = (child, task)
            for receive in multiprocessing.connection.wait(list(running)):
                child, task = running.pop(receive)
                try:
                    kind, value = receive.recv()
                except EOFError:
                    kind, value = "lost", None
                receive.close()
                child.join()
                if kind == "raise":
                    raise value
                if kind == "lost":
                    value = (task[0], task[1], None, child.exitcode) + ((None,) if worker is edge_run_case else ())
                results.append(value)
    finally:
        for child, _task in running.values():
            child.terminate()
            child.join()
        EDGE_CONTEXT.clear()
    return results


def edge_report(name: str, cases: int, constants: list, executed: set, results: list, uncovered: bool) -> tuple[list[str], int]:
    """The lines of one function and the status, from the results of its altered runs in any order.

    `constants` are (index, mnemonic, rs, rt, immediate); a result is (index, which, discarded, different),
    `which` 0 for the immediate plus one and 1 for minus one, or (index, which, None, exit status) for a run
    whose process ended without a result.
    """
    by_constant = {c[0]: c for c in constants}
    runs = unnoticed = all_discarded = lost = 0
    lines: list[str] = []
    for index, which, gone, changed in sorted(results, key=lambda r: r[:2]):
        _i, mnemonic, rs, rt, immediate = by_constant[index]
        runs += 1
        if gone is None:
            lost += 1
            lines.append(lost_line(index, mnemonic, rs, rt, immediate, edge_neighbours(immediate)[which], changed))
            continue
        if changed:
            continue
        if gone == cases:
            all_discarded += 1
        else:
            unnoticed += 1
            lines.append(edge_line(index, mnemonic, rs, rt, immediate, edge_neighbours(immediate)[which], cases, gone))
    skipped = [f"  slot {c[0]}: not executed by any case" for c in constants if 4 * c[0] not in executed]
    head = (f"{name} edges: constants {len(constants)}, altered runs {runs}, unnoticed {unnoticed}, "
            f"all discarded {all_discarded}" + (f", no result {lost}" if lost else ""))
    return [head, *lines, *(skipped if uncovered else [])], 1 if unnoticed else 0


def edge_sweep(cfg, name: str, build: Build, cases: int, seed: int, ram: bytes, scratch: bytes,
               uncovered: bool, jobs: int = 1) -> tuple[list[str], int]:
    """The edges sweep of one function: (lines to print, status 0 or 1).

    The default comparison runs first. The words of the original are scanned in `ram`; the altered runs
    are made by `jobs` processes (in this one with 1) and printed in a fixed order.
    """
    longest: list = []
    discarded, _equal, different, _first, executed = test_function(cfg, name, build.code, cases, seed, ram, scratch, build.entry,
                                                                   longest=longest)
    if different or discarded:
        return [f"{name} edges: not swept, the comparison without alteration shows different {different}, "
                f"discarded {discarded}"], 1
    constants, tasks, context = edge_plan(cfg, name, build, cases, seed, ram, scratch, executed, longest[0])
    return edge_report(name, cases, constants, executed, edge_runs(context, tasks, jobs), uncovered)


def edge_plan(cfg, name: str, build: Build, cases: int, seed: int, ram: bytes, scratch: bytes, executed: set,
              longest: int | None = None) -> tuple[list, list, dict]:
    """(constants, altered runs, context for the processes) of a function whose comparison without alteration
    has passed and has executed the slots `executed`. `longest` is the longest run of the unaltered
    original in instructions: the altered originals get their budget from it (`edge_budget`)."""
    address, size = original_function(cfg, name)
    base = address - RAM_BASE
    image = bytearray(ram)
    words = list(struct.unpack(f"<{size // 4}I", bytes(image[base : base + size - size % 4])))
    constants = edge_constants(words)
    tasks = [(index, which, new) for index, _m, _rs, _rt, immediate in constants if 4 * index in executed
             for which, new in enumerate(edge_neighbours(immediate))]
    context = {"cfg": cfg, "name": name, "code": build.code, "entry": build.entry, "cases": cases, "seed": seed,
               "scratch": scratch, "base": base, "words": words, "image": image, "budget": edge_budget(longest)}
    return constants, tasks, context


# ---------------------------------------------------------------------------
# Fixtures (--record, --replay)

FIXTURE_FORMATS = (1, 2)  # 2: a fixture may call an original callee (`callees`, and calls with `original`)
FIXTURE_CAP = 12  # fixtures kept by --record unless --max says otherwise
RECORD_PROBES = 3  # cases that --record looks at for a refusal before it makes the altered runs
POISON = 0xA5  # what the memory of a replay holds where the fixture says nothing
FIXTURE_FIELDS = ("args", "calls", "case", "ends_in_callee", "note", "reads", "recorders", "result",
                  "same", "seed", "watch", "writes")
CALL_FIELDS = ("args", "callee", "pointees", "returned", "watched")
ORIGINAL_CALL_FIELDS = ("callee", "effects", "leaves", "original", "reads", "stack", "uses")
INITIAL_SAVED = tuple(0x5A5A0000 + index for index in range(len(SAVED)))


class FixtureError(Exception):
    """A fixture file that cannot be read, or a setup that fixtures cannot describe."""


def fixtures_path(folder: Path, name: str) -> Path:
    return folder / f"{name}.fixtures.json"


def today() -> str:
    return datetime.date.today().isoformat()


def hex8(value: int) -> str:
    return f"{value & 0xFFFFFFFF:#010x}"


def console_address(physical: int) -> int:
    """The console address of a physical one as the emulator's hooks give it (RAM or scratchpad)."""
    return physical if physical >= SCRATCH_BASE else RAM_BASE + physical


def names_of(cfg, name: str | None = None) -> dict[int, str]:
    """address -> name, for the names of the tree's symbols and declared functions (the first name in order)."""
    table: dict[int, str] = {}
    for symbol, value in sorted(addresses(cfg, name).items()):
        if isinstance(value, int):
            table.setdefault(value, symbol)
    return table


def runs_of(found: dict[int, int]) -> list[list[str]]:
    """{console address: byte} as [address, hex bytes] with adjacent bytes joined, ascending."""
    runs: list[list] = []
    for address in sorted(found):
        if runs and runs[-1][0] + len(runs[-1][1]) == address:
            runs[-1][1].append(found[address])
        else:
            runs.append([address, bytearray([found[address]])])
    return [[hex8(start), bytes(data).hex()] for start, data in runs]


def bytes_of(runs: list) -> dict[int, int]:
    """The inverse of `runs_of`."""
    found: dict[int, int] = {}
    for start, text in runs:
        for offset, byte in enumerate(bytes.fromhex(text)):
            found[int(start, 16) + offset] = byte
    return found


def skip_map(harness: list[tuple[int, int]]) -> bytearray:
    """One byte per byte of RAM: 1 where memory is the function's own stack or the harness's."""
    skip = bytearray(RAM_SIZE)
    low, high = STACK_LOW - RAM_BASE, STACK_TOP + HOME_AREA - RAM_BASE
    skip[low:high] = b"\1" * (high - low)
    for address, size in harness:
        start = address - RAM_BASE
        skip[start : start + size] = b"\1" * size
    return skip


# The instructions that touch memory: opcode -> (loads?, which bytes of the word). The R3000 is little
# endian: lwl and swl touch the bytes from the start of the word up to the address, lwr and swr the bytes
# from the address to the end of the word.
MEMORY_OPS = {0x20: (True, "b"), 0x21: (True, "h"), 0x22: (True, "l"), 0x23: (True, "w"), 0x24: (True, "b"),
              0x25: (True, "h"), 0x26: (True, "r"), 0x32: (True, "w"),
              0x28: (False, "b"), 0x29: (False, "h"), 0x2A: (False, "l"), 0x2B: (False, "w"), 0x2E: (False, "r"),
              0x3A: (False, "w")}


def touched(kind: str, address: int) -> range:
    """The addresses that one access of this kind at `address` reaches."""
    if kind == "b":
        return range(address, address + 1)
    if kind == "h":
        return range(address, address + 2)
    if kind == "l":
        return range(address & ~3, address + 1)
    if kind == "r":
        return range(address, (address | 3) + 1)
    return range(address, address + 4)


# The registers that an instruction reads and writes, for the instructions of the MIPS I that the game uses
# (the R3000 with its coprocessor 2, the GTE). Register numbers 0 to 31; HI and LO are 32 and 33. `--record` looks
# at them only in the code of an original callee, to find the argument registers it read before it wrote them.
# A word that this table does not know is an error that names it.
HI, LO = 32, 33
# SPECIAL, by function: (sources, destinations) as letters: s = rs, t = rt, d = rd, R = ra, H = hi, L = lo
SPECIAL_REGISTERS = {
    0x00: ("t", "d"), 0x02: ("t", "d"), 0x03: ("t", "d"),  # sll, srl, sra
    0x04: ("st", "d"), 0x06: ("st", "d"), 0x07: ("st", "d"),  # sllv, srlv, srav
    0x08: ("s", ""), 0x09: ("s", "d"),  # jr, jalr
    0x10: ("H", "d"), 0x11: ("s", "H"), 0x12: ("L", "d"), 0x13: ("s", "L"),  # mfhi, mthi, mflo, mtlo
    0x18: ("st", "HL"), 0x19: ("st", "HL"), 0x1A: ("st", "HL"), 0x1B: ("st", "HL"),  # mult, multu, div, divu
    0x20: ("st", "d"), 0x21: ("st", "d"), 0x22: ("st", "d"), 0x23: ("st", "d"),  # add, addu, sub, subu
    0x24: ("st", "d"), 0x25: ("st", "d"), 0x26: ("st", "d"), 0x27: ("st", "d"),  # and, or, xor, nor
    0x2A: ("st", "d"), 0x2B: ("st", "d"),  # slt, sltu
}
# the other opcodes: (sources, destinations)
OP_REGISTERS = {
    0x02: ("", ""), 0x03: ("", "R"),  # j, jal
    0x04: ("st", ""), 0x05: ("st", ""), 0x06: ("s", ""), 0x07: ("s", ""),  # beq, bne, blez, bgtz
    0x08: ("s", "t"), 0x09: ("s", "t"), 0x0A: ("s", "t"), 0x0B: ("s", "t"),  # addi, addiu, slti, sltiu
    0x0C: ("s", "t"), 0x0D: ("s", "t"), 0x0E: ("s", "t"), 0x0F: ("", "t"),  # andi, ori, xori, lui
    0x20: ("s", "t"), 0x21: ("s", "t"), 0x23: ("s", "t"), 0x24: ("s", "t"), 0x25: ("s", "t"),  # lb, lh, lw, lbu, lhu
    0x22: ("st", "t"), 0x26: ("st", "t"),  # lwl, lwr: the old value of rt is merged
    0x28: ("st", ""), 0x29: ("st", ""), 0x2B: ("st", ""), 0x2A: ("st", ""), 0x2E: ("st", ""),  # sb, sh, sw, swl, swr
    0x32: ("s", ""), 0x3A: ("s", ""),  # lwc2, swc2
}


def registers_of(word: int) -> tuple[tuple[int, ...], tuple[int, ...]]:
    """(registers read, registers written) by the instruction `word`. Raises FixtureError for a word not in the table."""
    op, rs, rt, rd, funct = word >> 26, (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31, word & 63
    fields = {"s": rs, "t": rt, "d": rd, "R": RA_NUMBER, "H": HI, "L": LO}
    if op == 0:
        spec = SPECIAL_REGISTERS.get(funct)
    elif op == 1:  # REGIMM: bltz, bgez; bltzal, bgezal write ra
        spec = {0: ("s", ""), 1: ("s", ""), 0x10: ("s", "R"), 0x11: ("s", "R")}.get(rt)
    elif op == 0x10:  # COP0: mfc0, mtc0, rfe
        if word & (1 << 25):
            spec = ("", "") if funct == 0x10 else None
        else:
            spec = {0: ("", "t"), 4: ("t", "")}.get(rs)
    elif op == 0x12:  # COP2, the GTE: mfc2, cfc2, mtc2, ctc2, and the commands (bit 25), which touch no register of the CPU
        spec = ("", "") if word & (1 << 25) else {0: ("", "t"), 2: ("", "t"), 4: ("t", ""), 6: ("t", "")}.get(rs)
    else:
        spec = OP_REGISTERS.get(op)
    if spec is None:
        raise FixtureError(f"word {word:#010x} of an original callee: the tool does not know which registers it reads "
                           f"and writes")
    # each letter of a specification names one register ("HL": both hi and lo)
    return tuple(fields[c] for c in spec[0]), tuple(fields[c] for c in spec[1])


# The registers that the stand-in of an original callee sets after the call (`leaves`): the ones a callee may
# leave anything in, by the calling convention (the saved ones, sp and ra are the callee's to restore).
LEAVES = (("at", 1), ("v0", 2), ("v1", 3), ("a0", 4), ("a1", 5), ("a2", 6), ("a3", 7), ("t0", 8), ("t1", 9), ("t2", 10),
          ("t3", 11), ("t4", 12), ("t5", 13), ("t6", 14), ("t7", 15), ("s0", 16), ("s1", 17), ("s2", 18), ("s3", 19),
          ("s4", 20), ("s5", 21), ("s6", 22), ("s7", 23), ("t8", 24), ("t9", 25), ("gp", 28), ("fp", 30), ("hi", HI),
          ("lo", LO))
# the registers whose value at the begin of a call may be a use of the call (`uses`); a use of any other register of
# the caller (the saved ones, k0, k1, and ra other than by `jr ra`) is a refusal. sp and register 0 are no use.
USABLE = frozenset(number for name, number in LEAVES if not name.startswith("s") and name != "fp")
REGISTER_LABELS = {**{number: name for number, name in enumerate(REGISTER_NAMES)}, HI: "hi", LO: "lo"}
REGISTER_NUMBERS = {name: number for number, name in REGISTER_LABELS.items()}
STACK_HIGH = STACK_TOP + HOME_AREA  # the end of the function's own stack region, the home area included
PLACE_FUNCTION, PLACE_HARNESS, PLACE_STOP, PLACE_OTHER = range(4)


def register_unicorn(number: int) -> int:
    """The Unicorn constant of register `number` (0 to 31, HI or LO)."""
    return {HI: reg.UC_MIPS_REG_HI, LO: reg.UC_MIPS_REG_LO}.get(number, reg.UC_MIPS_REG_0 + number)


def register_value(uc, number: int) -> int:
    return uc.reg_read(register_unicorn(number))


def in_stack(address: int, size: int) -> bool:
    """True when the `size` bytes at the console address `address` lie wholly in the stack region of a case (from its
    low end to the top and the callee's home area above it): memory that the tool does not record and that starts as zeros."""
    start = address & 0x1FFFFFFF
    return STACK_LOW & 0x1FFFFFFF <= start and start + size <= STACK_HIGH & 0x1FFFFFFF


def masked_pointees(pointees, masks) -> list[int]:
    """The arguments of a recorder that have a pointee and a mask of 0: the log holds 0 for their value."""
    return sorted(int(i) for i in pointees if masks.get(i) == 0)  # both dicts have int keys (a recorder) or text keys (a file)


def argument_at_entry(uc, index: int) -> int:
    """The value of argument `index` of a call as the callee is entered: a register, or the caller's stack for the fifth on."""
    if index < 4:
        return uc.reg_read(reg.UC_MIPS_REG_A0 + index)
    sp = uc.reg_read(reg.UC_MIPS_REG_SP)
    return struct.unpack("<I", uc.mem_read((sp + 16 + 4 * (index - 4)) & 0x1FFFFFFF, 4))[0]


class Refusal(FixtureError):
    """A case that this piece cannot describe (status 1 for the function, no file)."""


class OriginalCall:
    """One call of an original callee, from its first instruction to the function's next one (or the run's end)."""

    def __init__(self, entry: int, begin: dict[int, int]):
        self.entry = entry
        self.begin = begin  # register number (0 to 31, HI, LO) -> its value when the call began
        self.sp = begin[SP]  # the stack pointer of the call
        self.args = [begin[number] for number in range(4, 8)]  # a0 to a3 at the begin
        self.uses: dict[int, int] = {}  # register -> value at the begin, for each register used before it was written
        self.written_regs: set[int] = set()  # the registers in the state WRITTEN (the others are UNTOUCHED)
        # Every byte of the callee's own memory that holds a saved copy of a register that was UNTOUCHED when it was
        # stored: byte (physical address) -> (register, which of its four bytes). A register may have several
        # live copies; a store ends only the bytes it writes.
        self.saved: dict[int, tuple[int, int]] = {}
        self.frame_written: set[int] = set()  # bytes of its frame (below the call's sp) that the callee wrote
        self.reads: dict[int, tuple[int, bool]] = {}  # byte -> (value, written earlier in the run); read before written here
        self.written: dict[int, int] = {}  # byte -> value before the call's first write
        self.stack_bytes: set[int] = set()  # bytes at or above sp read through sp
        self.home_written: set[int] = set()  # bytes of [sp, sp + 16) that the callee wrote itself
        self.leaves: dict[str, int] = {}
        self.effects: dict[int, int] = {}
        self.stack: list[tuple[int, int]] = []  # (offset from sp, word)
        self.kept_reads: dict[int, int] = {}  # byte -> value, the ones the fixture keeps
        self.input_reads: dict[int, int] = {}  # of those, the ones nothing had written before: bytes of the case's input
        self.unmade = 0  # bytes it read that the setup did not make and nothing had written: the game's image


class Trace:
    """The memory accesses of one run of the original, found by looking at each instruction before it runs.

    `reads` holds the bytes the FUNCTION's own instructions read before anything wrote them (their value when
    read); `written` holds the bytes the run wrote (recorders and original callees included), with the value each
    held before its first write. What a recorder copies into the log is not a read of the function and is not in
    `reads`; a byte a recorder reads and then writes itself (a word that `counts` adds to) is kept apart in
    `recorder_reads`, because the replay's recorder needs its first value.
    The function's own stack region, the home area above it and the harness's memory (`skip`) are left out.

    The code of an ORIGINAL CALLEE (any code outside the function and outside the harness) is told from the
    function's by the address of each instruction: a call begins at the first instruction outside the function, the
    harness and the stop address when the instruction before it was inside the function, and it ends when an
    instruction inside the function is about to run again, or with the run (`finish`). Everything between belongs to
    the call, the recorders it runs included: the accesses of the call are kept in its `OriginalCall`, and they are
    not reads of the function. `sequence` lists, in the order they happened, the calls of recorders made outside any
    original call (("log", number of the entry in the log)) and the original calls (("call", OriginalCall)).

    The emulator's memory hooks are not used: with one installed, Unicorn 2.1.4 ends a run with an
    exception when a load or store lies in the delay slot of a conditional branch that is not taken and
    a jump follows it (made-up code that shows it is in `test_difftest.py`). An instruction hook does not.
    A refusal or an error found by a hook is kept in `error` and ends the run; the caller raises it.
    """

    def __init__(self, skip: bytearray, original: int = 0, size: int = 0, recorders: frozenset = frozenset(),
                 names: dict | None = None, made=None, pointers: dict | None = None):
        self.skip = skip
        self.original, self.size = original, size
        self.recorders = recorders
        self.names = names or {}
        self.made = made or (lambda at: True)  # at (physical) -> did the setup make this byte
        self.reads: dict[int, int] = {}
        self.written: dict[int, int] = {}
        self.recorder_reads: dict[int, int] = {}
        self.recorder_written: set[int] = set()
        self.sequence: list[tuple[str, object]] = []
        self.entries = 0  # instructions run at a recorder's address: one entry of the log each
        self.pointers = pointers or {}  # recorder address -> its arguments with a mask of 0 and a pointee
        self.entry_pointers: list[dict[int, int]] = []  # for each entry: those arguments' real values at the callee's entry
        self.call: OriginalCall | None = None
        self.open_at_end = False
        self.previous_in_function = False
        self.error: Exception | None = None

    def inputs(self) -> dict[int, int]:
        """`reads` and the first values of the bytes a recorder read and wrote itself."""
        found = {a: v for a, v in self.recorder_reads.items() if a in self.recorder_written}
        found.update(self.reads)
        return found

    def ignored(self, address: int) -> bool:
        if address < RAM_SIZE:
            return bool(self.skip[address])
        return not SCRATCH_BASE <= address < SCRATCH_BASE + SCRATCH_SIZE

    def place(self, address: int) -> int:
        if self.original <= address < self.original + self.size:
            return PLACE_FUNCTION
        if address == STOP_ADDRESS:
            return PLACE_STOP
        physical = address & 0x1FFFFFFF
        return PLACE_HARNESS if physical < RAM_SIZE and self.skip[physical] else PLACE_OTHER

    def label(self, address: int) -> str:
        return self.names.get(address, hex8(address))

    def step(self, uc, address, size, data) -> None:
        try:
            self._step(uc, address)
        except FixtureError as exc:
            self.error = self.error or exc
            uc.emu_stop()

    def _step(self, uc, address) -> None:
        place = self.place(address)
        if place == PLACE_STOP:
            self.previous_in_function = False
            return
        if self.call is None:
            if place == PLACE_OTHER and self.previous_in_function:
                begin = {n: uc.reg_read(reg.UC_MIPS_REG_0 + n) for n in range(32)}
                begin[HI], begin[LO] = uc.reg_read(reg.UC_MIPS_REG_HI), uc.reg_read(reg.UC_MIPS_REG_LO)
                self.call = OriginalCall(address, begin)
                self.sequence.append(("call", self.call))
        elif place == PLACE_FUNCTION:
            self.close(uc)
        self.previous_in_function = place == PLACE_FUNCTION
        if address in self.recorders:
            if self.call is None:
                self.sequence.append(("log", self.entries))
            self.entry_pointers.append({i: argument_at_entry(uc, i) for i in self.pointers.get(address, ())})
            self.entries += 1
        word = struct.unpack("<I", uc.mem_read(address & 0x1FFFFFFF, 4))[0]
        if self.call is not None:
            self.inside(uc, address, word)
            return
        op = MEMORY_OPS.get(word >> 26)
        if op is None:
            return
        loads, kind = op
        in_recorder = place == PLACE_HARNESS
        for at in touched(kind, self.target(uc, word)):
            if self.ignored(at) or at in self.written or (loads and at in self.reads):
                continue
            byte = uc.mem_read(at, 1)[0]
            if loads and in_recorder:
                self.recorder_reads.setdefault(at, byte)
            elif loads:
                self.reads[at] = byte
            else:
                self.written[at] = byte
                if in_recorder:
                    self.recorder_written.add(at)

    @staticmethod
    def target(uc, word: int) -> int:
        offset = word & 0xFFFF
        return (uc.reg_read(reg.UC_MIPS_REG_0 + ((word >> 21) & 31)) + offset - (0x10000 if offset & 0x8000 else 0)) & 0x1FFFFFFF

    def inside(self, uc, address: int, word: int) -> None:
        """One instruction that belongs to the open call."""
        call = self.call
        sources, dests = registers_of(word)
        op, rt = word >> 26, (word >> 16) & 31
        memory = MEMORY_OPS.get(op)
        span = touched(memory[1], self.target(uc, word)) if memory else None
        # The stack pointer may be read as the base of a load or a store and by the instruction that moves it.
        # Read in any other way its value goes somewhere else, and what the callee then makes of it is an address of
        # its stack, which is another one under a build: nothing here could check that, so it is refused.
        if SP in sources:
            as_base = memory is not None and (word >> 21) & 31 == SP and (memory[0] or rt != SP)
            moved = op in (8, 9) and (word >> 21) & 31 == SP and rt == SP
            if not (as_base or moved):
                raise Refusal(f"the original callee {self.label(call.entry)} takes the address of its stack (it reads sp "
                              f"other than as the base of a load or a store, or to move it)")
        # The callee's own memory is its frame and its home area. `sw R` of an UNTOUCHED R there is a SAVE (no use of
        # R): its four bytes are then copies of R's value of the call's begin, and they stay so, each one, until a
        # store writes that byte. `lw R` over four bytes that are R's own copy, in order, is a RESTORE (R is UNTOUCHED
        # again). Any other load of a byte that is still a copy is a use of the register it is a copy of.
        own_low, own_high = STACK_LOW & 0x1FFFFFFF, (call.sp & 0x1FFFFFFF) + HOME_AREA
        save = (memory is not None and op == 0x2B and rt not in (0, SP) and rt not in call.written_regs
                and span.start >= own_low and span.stop <= own_high)
        restore = memory is not None and op == 0x23 and all(call.saved.get(span.start + k) == (rt, k) for k in range(4))
        if memory is not None and memory[0] and not restore:
            for number in sorted({call.saved[at][0] for at in span if at in call.saved}):
                self.use(call, number)
        if memory is not None and not memory[0]:
            for at in span:
                call.saved.pop(at, None)
            if save:
                for k in range(4):
                    call.saved[span.start + k] = (rt, k)
        for number in sources:
            if number == 0 or number == SP or (save and number == rt):
                continue
            if number == RA_NUMBER and op == 0 and word & 63 == 8:  # jr ra
                continue
            if number not in call.written_regs:
                self.use(call, number)
        if restore:
            call.written_regs.discard(rt)
        else:
            call.written_regs.update(number for number in dests if number != 0)
        entered = None
        if op == 3:
            entered = (address & 0xF0000000) | ((word & 0x03FFFFFF) << 2)
        elif op == 0 and word & 63 == 9:  # jalr
            entered = uc.reg_read(reg.UC_MIPS_REG_0 + ((word >> 21) & 31))
        if entered == call.entry:
            raise Refusal(f"the original callee {self.label(call.entry)} is entered again before it ended")
        memory = MEMORY_OPS.get(op)
        if memory is None:
            return
        loads, kind = memory
        low, high = call.sp & 0x1FFFFFFF, STACK_HIGH & 0x1FFFFFFF
        for at in touched(kind, self.target(uc, word)):
            if low <= at < high:
                if low <= at < low + HOME_AREA:
                    # The callee's own argument home area: its writes are its own memory (no effect, no refusal), and a
                    # read of a byte it wrote there is not a read of the call.
                    if not loads:
                        call.home_written.add(at)
                        continue
                    if at in call.home_written:
                        continue
                if not loads:
                    raise Refusal(f"the original callee {self.label(call.entry)} writes the stack at {console_address(at):#x}, "
                                  f"at or above its stack pointer plus {HOME_AREA}")
                if (word >> 21) & 31 != SP:
                    raise Refusal(f"the original callee {self.label(call.entry)} reads the stack at {console_address(at):#x}, "
                                  f"at or above its stack pointer, through a register other than sp (a pointer to a local "
                                  f"of the function)")
                call.stack_bytes.add(at)
                continue
            if (STACK_LOW & 0x1FFFFFFF) <= at < low:
                # Its own frame. What it holds before the callee writes it is what earlier code left on the stack, which
                # is other code under a build: a callee that reads it cannot be stood in for.
                if not loads:
                    call.frame_written.add(at)
                elif at not in call.frame_written:
                    raise Refusal(f"the original callee {self.label(call.entry)} reads its own frame at "
                                  f"{console_address(at):#x} before writing it")
                continue
            if self.ignored(at):
                continue
            byte = uc.mem_read(at, 1)[0]
            if loads:
                if at not in call.written and at not in call.reads:
                    call.reads[at] = (byte, at in self.written)
            else:
                call.written.setdefault(at, byte)
                self.written.setdefault(at, byte)

    def use(self, call: OriginalCall, number: int) -> None:
        """The call reads the register `number` while it is UNTOUCHED: a use, or a refusal for a register that is no argument."""
        if number in call.uses:
            return
        if number not in USABLE:
            raise Refusal(f"the original callee {self.label(call.entry)} reads register {REGISTER_LABELS[number]} of its "
                          f"caller, which is no argument")
        call.uses[number] = call.begin[number]

    def close(self, uc) -> None:
        """The open call ends here: take its registers, its effects and its stack words."""
        call, self.call = self.call, None
        if uc.reg_read(reg.UC_MIPS_REG_SP) != call.sp:
            raise Refusal(f"the original callee {self.label(call.entry)} leaves the stack pointer changed")
        # exactly the registers in the state WRITTEN at the end; an UNTOUCHED one (never written, or restored) is not there
        for name, number in LEAVES:
            if number in call.written_regs:
                call.leaves[name] = register_value(uc, number)
        for at, before in call.written.items():
            now = uc.mem_read(at, 1)[0]
            if now != before:
                call.effects[at] = now
        base = call.sp & 0x1FFFFFFF
        for word in sorted({at & ~3 for at in call.stack_bytes}):
            call.stack.append((word - base, struct.unpack("<I", uc.mem_read(word, 4))[0]))
        for at, (value, earlier) in call.reads.items():
            # Bytes the setup did not make and nothing had written are the game's image: not kept as reads, and
            # counted, because what the callee makes of them (its effects, the registers it leaves) is in the file.
            if earlier or self.made(at):
                call.kept_reads[at] = value
                if not earlier:
                    call.input_reads[at] = value
            else:
                call.unmade += 1

    def finish(self, uc) -> None:
        """The run is over: a call that is still open ends with it."""
        if self.call is not None:
            self.open_at_end = True
            self.close(uc)


def changed_bytes(before_ram: bytes, before_scratch: bytes, final: dict, skip: bytearray) -> dict[int, int]:
    """{console address: byte after} for the bytes of RAM and scratchpad that differ from `before_*`,
    outside what `skip` marks."""
    found: dict[int, int] = {}
    for base, before, after, marks in ((RAM_BASE, before_ram, final["ram"], skip),
                                       (SCRATCH_BASE, before_scratch, final["scratch"], None)):
        for start in range(0, len(before), PAGE):
            if before[start : start + PAGE] == after[start : start + PAGE]:
                continue
            for i in range(start, min(start + PAGE, len(before))):
                if before[i] != after[i] and not (marks is not None and marks[i]):
                    found[base + i] = after[i]
    return found


def input_bytes(before_ram: bytes, before_scratch: bytes, address: int, size: int) -> bytes:
    """`size` bytes at the console address `address` of the input state (the memory before the run)."""
    if address < RAM_BASE:
        if not (SCRATCH_BASE <= address and address + size <= SCRATCH_BASE + SCRATCH_SIZE):
            raise ValueError(f"{address:#x} is outside RAM and the scratchpad")
        return bytes(before_scratch[address - SCRATCH_BASE : address - SCRATCH_BASE + size])
    if address + size > RAM_BASE + RAM_SIZE:
        raise ValueError(f"{address:#x} is outside RAM and the scratchpad")
    return bytes(before_ram[address - RAM_BASE : address - RAM_BASE + size])


def span_changes(seen: bytes, base: bytes) -> list[list]:
    """The bytes of `seen` that differ from `base`, as [offset, hex] spans with adjacent bytes joined."""
    spans: list[list] = []
    for offset, (x, y) in enumerate(zip(seen, base)):
        if x == y:
            continue
        if spans and spans[-1][0] + len(spans[-1][1]) == offset:
            spans[-1][1].append(x)
        else:
            spans.append([offset, bytearray([x])])
    return [[offset, bytes(data).hex()] for offset, data in spans]


def apply_changes(base: bytes, spans: list) -> bytes:
    """`base` with the spans put over it: what a block held at a call, from the input state and its changes."""
    block = bytearray(base)
    for offset, text in spans:
        data = bytes.fromhex(text)
        block[offset : offset + len(data)] = data
    return bytes(block)


CALL_SCALARS = ("args", "callee", "returned")


def call_text(call: contracts.Call, label: dict[int, str], log: contracts.CallLog, before_ram: bytes, before_scratch: bytes,
              real: dict[int, int] | None = None) -> dict:
    """A decoded call as the fixture holds it. The words behind a pointer argument and the watched blocks are
    stored as the bytes that differ, at this call, from the same memory in the input state. `real` holds the real
    values, at the callee's entry, of the arguments that the log holds as 0 (mask 0) and that have a pointee: such a
    pointee must lie wholly in the stack region of the case, which starts as zeros and is not input, and is
    described against zeros; otherwise the case is refused (Refusal)."""
    real = real or {}
    pointees = {}
    for index, data in sorted(call.pointees.items()):
        if index in real:
            if not in_stack(real[index], len(data)):
                raise Refusal(f"the callee {label[call.address]}, argument {index} (from 0): it is masked to 0 and has a pointee, and its "
                              f"real value {hex8(real[index])} leads to {len(data)} bytes that are not wholly inside the stack region "
                              f"({hex8(STACK_LOW)} to {hex8(STACK_HIGH)}): their bytes cannot be told from the case's input, and the "
                              f"argument's value is not in the log")
            base = bytes(len(data))
        else:
            base = input_bytes(before_ram, before_scratch, call.args[index], len(data))
        pointees[str(index)] = span_changes(data, base)
    watched = [span_changes(data, input_bytes(before_ram, before_scratch, block, 4 * count))
               for (block, count), data in zip(log.watch, call.watched)]
    return {"args": [hex8(a) for a in call.args], "callee": label[call.address], "pointees": pointees,
            "returned": hex8(call.returned), "watched": watched}


def recorder_text(recorder: contracts.Recorder, label: str, cell: int | None) -> dict:
    """The recorder as the fixture holds it: the callee's stand-in, apart from the values it returns."""
    found: dict = {"arguments": recorder.arguments, "at": hex8(recorder.address), "callee": label}
    if cell is not None:
        found["cell"] = hex8(cell)
    if recorder.counts:
        found["counts"] = [hex8(a) for a in recorder.counts]
    if recorder.fills:
        found["fills"] = [[call, argument, offset, data.hex()] for call, argument, offset, data in recorder.fills]
    if recorder.ends_run_at:
        found["ends_run_at"] = recorder.ends_run_at
    if recorder.masks:
        found["masks"] = {str(i): m for i, m in sorted(recorder.masks.items())}
    if recorder.pointees:
        found["pointees"] = {str(i): n for i, n in sorted(recorder.pointees.items())}
    if recorder.stores:
        found["stores"] = [[n, hex8(a), hex8(v)] for n, a, v in recorder.stores]
    return found


def pointer_cell(reads: dict[int, int], target: int) -> int | None:
    """The first aligned word of `reads` that holds `target`, as its address; None when there is none."""
    wanted = struct.pack("<I", target)
    for address in sorted(reads):
        if address % 4 == 0 and all(reads.get(address + i) == wanted[i] for i in range(4)):
            return address
    return None


def made_by_setup(state: State, at: int) -> bool:
    """True when the setup made the byte at the physical address `at`. What the setup marked with `owns` is not
    made in this sense: it is made for the audit of the writes, and its content is the image's all the same."""
    address = at + RAM_BASE if at < RAM_SIZE else at
    if any(start <= address < start + size for start, size in state.kept):
        return False
    return bool(state.made_ram[at] if at < RAM_SIZE else state.made_scratch[at - SCRATCH_BASE])


def unmade_reads(state: State, reads: dict[int, int]) -> int:
    """How many of the bytes read are memory that the setup did not make: they come from the game's image."""
    return sum(1 for at in reads if not made_by_setup(state, at))


def original_call_text(call: OriginalCall, label: str) -> dict:
    """A call of an original callee as the fixture holds it (format 2)."""
    return {"callee": label,
            "effects": runs_of({console_address(a): b for a, b in call.effects.items()}),
            "leaves": {name: hex8(value) for name, value in sorted(call.leaves.items())},
            "original": True,
            "reads": runs_of({console_address(a): b for a, b in call.kept_reads.items()}),
            "stack": [[offset, hex8(word)] for offset, word in call.stack],
            "uses": {REGISTER_LABELS[number]: hex8(value) for number, value in sorted(call.uses.items())}}


def record_case(uc: Uc, state: State, original: int, size: int, setup: contracts.Setup, names: dict[int, str]) -> tuple[dict, int]:
    """Run the original once on `state` with the hooks and describe the case as a fixture (without note, seed
    and case). Returns it with the number of bytes it read that the setup did not make."""
    if len(state.call_logs) > 1:
        raise FixtureError("the setup makes more than one CallLog; fixtures describe one")
    log = state.call_logs[0] if state.call_logs else None
    if log is not None and any(r.tail for r in log.recorders.values()):
        raise FixtureError("a recorder with a tail stands for the callee in machine code of the contract's own; "
                           "fixtures cannot describe what it returns")
    harness = list(log.owned) if log else []
    skip = skip_map(harness)
    pointers = {r.address: tuple(masked_pointees(r.pointees, r.masks)) for r in log.recorders.values()} if log else {}
    trace = Trace(skip, original, size, frozenset(log.recorders) if log else frozenset(), names,
                  lambda at: made_by_setup(state, at), pointers)
    final = run_once(uc, state, original, setup, None, ((UC_HOOK_CODE, trace.step),))
    if trace.error is not None:
        raise trace.error
    if isinstance(final, str):
        raise FixtureError(f"the original does not finish on a case that the comparison accepted: {final}")
    trace.finish(uc)
    if trace.open_at_end and setup.returns:
        raise Refusal(f"the original callee {trace.label(trace.sequence[-1][1].entry)} does not end when the run does, "
                      f"and the setup says the function returns")
    writes = changed_bytes(state.ram, state.scratch, final, skip)
    reads = trace.inputs()
    decoded = log.calls(final["ram"]) if log is not None else []
    if trace.entries != len(decoded):
        raise FixtureError(f"the run made {trace.entries} calls of recorders and the log holds {len(decoded)}")
    originals = [item for kind, item in trace.sequence if kind == "call"]
    # The bytes of the memory that a recorder saw at a call (pointees and watched blocks): the calls hold them as
    # changes against the input state, so a byte of them that the function stored needs its input value stated.
    # The entries that a recorder made inside the call of an original callee are not in the file.
    kept_entries = [(number, decoded[number]) for kind, number in trace.sequence if kind == "log"]
    seen_by_recorders: set[int] = set()
    for _number, call in kept_entries:
        for index, data in call.pointees.items():
            seen_by_recorders.update(range(call.args[index] & 0x1FFFFFFF, (call.args[index] & 0x1FFFFFFF) + len(data)))
        for block, count in log.watch:
            seen_by_recorders.update(range(block & 0x1FFFFFFF, (block & 0x1FFFFFFF) + 4 * count))
    # A byte the run stored without changing it, or that a recorder saw, and did not read first: the replay's
    # memory holds its input value beforehand, or the store would look like a change (or a call would look changed).
    same = {at: before for at, before in trace.written.items()
            if at not in reads and (at in seen_by_recorders or
                                    before == (final["ram"][at] if at < RAM_SIZE else final["scratch"][at - SCRATCH_BASE]))}
    # A byte that only an original callee read and that nothing had written: an input of the case. The replay's
    # memory must hold it when the stand-in checks the call.
    for call in originals:
        for at, value in call.input_reads.items():
            if at not in reads:
                same[at] = value
    recorders, calls = [], []
    label = {}
    if log is not None:
        label = {address: names.get(address, hex8(address)) for address in log.recorders}
        for recorder in sorted(log.recorders.values(), key=lambda r: r.address):
            cell = None if recorder.address in names else pointer_cell(reads, recorder.address)
            recorders.append(recorder_text(recorder, label[recorder.address], None if cell is None else console_address(cell)))
    for kind, item in trace.sequence:
        if kind == "log":
            calls.append(call_text(decoded[item], label, log, state.ram, state.scratch, trace.entry_pointers[item]))
        else:
            calls.append(original_call_text(item, trace.label(item.entry)))
    result = hex8(final["v0"]) if setup.returns_value and setup.returns else None
    fixture = {"args": [hex8(a) for a in setup.args], "calls": calls, "ends_in_callee": not setup.returns,
               "reads": runs_of({console_address(a): b for a, b in reads.items()}),
               "recorders": recorders, "result": result,
               "same": runs_of({console_address(a): b for a, b in same.items()}),
               "watch": [[hex8(a), n] for a, n in (log.watch if log else ())], "writes": runs_of(writes)}
    if originals:
        stands = {call.entry: trace.label(call.entry) for call in originals}
        fixture["callees"] = [{"at": hex8(at), "callee": name} for at, name in sorted(stands.items())]
    if setup.returns and (tuple(final["saved"]) != INITIAL_SAVED or final["sp"] != STACK_TOP):
        fixture["registers"] = {"saved": [hex8(v) for v in final["saved"]], "sp": hex8(final["sp"])}
    return fixture, unmade_reads(state, reads) + sum(call.unmade for call in originals)


class Choice:
    """The cases kept for a fixture file, with the reasons each was kept for."""

    def __init__(self, cap: int):
        self.cap = cap
        self.reasons: dict[int, list[str]] = {}
        self.by = {"slots": 0, "edges": 0, "results": 0}

    def add(self, case: int, reason: str, kind: str) -> bool:
        """Keep `case` for `reason`. A case already kept only gets the reason. False when the cap leaves it out."""
        if case in self.reasons:
            self.reasons[case].append(reason)
            return True
        if len(self.reasons) >= self.cap:
            return False
        self.reasons[case] = [reason]
        self.by[kind] += 1
        return True


def record_function(cfg, name: str, build: Build, cases: int, seed: int, cap: int, ram: bytes, scratch: bytes,
                    folder: Path, jobs: int) -> tuple[list[str], int]:
    """The fixtures of one function: (lines to print, status). Writes `NAME.fixtures.json` in `folder`."""
    info: dict[int, tuple[frozenset, int | None]] = {}

    def collect(case, setup, reference, slots, _differs):
        info[case] = (frozenset(slots), reference["v0"] if setup.returns_value and setup.returns else None)

    longest: list = []
    discarded, equal, different, first, executed = test_function(cfg, name, build.code, cases, seed, ram, scratch,
                                                                  build.entry, on_case=collect, longest=longest)
    original, size = original_function(cfg, name)
    comparison = (f"{name}: built {build.size} bytes, original {size} bytes; "
                  f"cases {cases}, discarded {discarded}, equal {equal}, different {different}")
    if different or discarded or not equal:
        return [f"{name} fixtures: not recorded, the comparison shows different {different}, discarded {discarded}",
                *(f"  {line}" for line in first)], 1

    contract = contracts.CONTRACTS[name]
    sym, names = addresses(cfg, name), names_of(cfg, name)
    uc = machine(b"\0" * 16)
    gp = global_pointer(cfg, name)

    def recorded(case: int) -> tuple[dict | None, int, tuple[list[str], int] | None]:
        """(fixture, bytes not made, None) of a case, or (None, 0, the lines and status that refuse the function)."""
        rng = random.Random(f"{seed}:{name}:{case}")
        state = State(ram, scratch, gp)
        setup = contract.setup(state, rng, sym)
        try:
            fixture, not_made = record_case(uc, state, original, size, setup, names)
        except Refusal as exc:
            return None, 0, ([f"{name} fixtures: not recorded, case {case}: {exc}"], 1)
        except (FixtureError, ValueError) as exc:
            return None, 0, ([f"{name} fixtures: not recorded, case {case}: {exc}"], 2)
        return fixture, not_made, None

    # What refuses a function is looked for in its first cases before the altered runs are made: they are
    # the long part, and a refusal after them would have been known without them.
    for case in sorted(info)[:RECORD_PROBES]:
        if (refusal := recorded(case)[2]) is not None:
            return refusal

    choice = Choice(cap)
    # a. the cases that execute a slot of the original that no kept case executed
    covered: set[int] = set()
    for case in sorted(info):
        new = info[case][0] - covered
        if new and choice.add(case, f"slots: {len(new)} new", "slots"):
            covered |= info[case][0]
    # b. the first case that notices each alteration of a constant of the original (as `--edges` makes them)
    constants, tasks, context = edge_plan(cfg, name, build, cases, seed, ram, scratch, executed, longest[0])
    noticed = {(r[0], r[1]): r for r in edge_runs(context, tasks, jobs, edge_run_case)}
    by_constant = {c[0]: c for c in constants}
    left_out: list[str] = []
    for index, mnemonic, rs, rt, immediate in constants:
        if 4 * index not in executed:
            left_out.append(f"slot {index}: not executed by any case")
            continue
        for which, new in enumerate(edge_neighbours(immediate)):
            _i, _w, gone, changed, case = noticed[(index, which)]
            if gone is None:
                left_out.append(lost_line(index, mnemonic, rs, rt, immediate, new, changed).strip())
            elif changed:
                text = f"slot {index}, immediate {immediate:#x} against {new:#x}"
                if not choice.add(case, f"edge: {text}", "edges"):
                    left_out.append(f"slot {index}: {mnemonic} {operands_text(mnemonic, rs, rt, immediate)}, immediate "
                                    f"{immediate:#x} -> {new:#x}: first noticed by case {case}, which the cap of {cap} left out")
            else:
                left_out.append(edge_line(index, mnemonic, rs, rt, immediate, new, cases, gone).strip())
    # c. one case for each result value that no kept case has
    if any(result is not None for _slots, result in info.values()):
        seen = {info[case][1] for case in choice.reasons}
        for case in sorted(info):
            result = info[case][1]
            if result not in seen and len(choice.reasons) < cap:
                seen.add(result)
                choice.add(case, f"result {result:#x}", "results")

    fixtures, unmade = [], 0
    for case in sorted(choice.reasons):
        fixture, not_made, refusal = recorded(case)
        if refusal is not None:
            return refusal
        fixture.update({"case": case, "note": "; ".join(choice.reasons[case]), "seed": seed})
        unmade += not_made
        fixtures.append(fixture)
    kept_slots = set().union(*(info[c][0] for c in choice.reasons)) if choice.reasons else set()
    missing = [o for o in range(0, size - size % 4, 4) if o not in kept_slots]
    # A file whose fixtures call no original callee stays format 1, byte for byte what it was before format 2.
    doc = {"format": 2 if any("callees" in f for f in fixtures) else 1, "function": name, "fixtures": fixtures,
           "recorded": {"cases": cases, "comparison": comparison, "date": today(), "seed": seed},
           "uncovered": {"edges": left_out, "slots": ranges_text(missing).split(", ") if missing else []}}
    text = fixtures_text(doc)
    # The kept fixtures are replayed on the build before anything is written: a file that the build does not pass
    # is not recorded (its first line says why).
    checked = machine(build.code)
    for fixture in json.loads(text)["fixtures"]:
        try:
            failed = replay_fixture(checked, build, fixture, gp)
        except (KeyError, ValueError, TypeError, AttributeError, FixtureError) as exc:
            failed = [f"cannot be read: {type(exc).__name__}: {exc}"]
        if failed:
            return [f"{name} fixtures: not recorded, the replay of case {fixture['case']} fails: {failed[0]}"], 1
    path = fixtures_path(folder, name)
    lines = []
    if path.exists():
        lines.append(f"{name} fixtures: replaced {path.name}")
    path.write_text(text)
    if unmade:
        lines.append(f"{name} fixtures: warning, {unmade} bytes read were not made by the setup: they come from the "
                     f"game's memory image, which the file would publish")
    lines.append(f"{name} fixtures: kept {len(fixtures)} of {cases} cases (slots {choice.by['slots']}, "
                 f"edges {choice.by['edges']}, results {choice.by['results']})")
    return lines, 0


def fixtures_text(doc: dict) -> str:
    """The file's text: sorted keys, one fixture to a line, the same bytes for the same document."""
    rows = [json.dumps(fixture, sort_keys=True, separators=(", ", ": ")) for fixture in doc["fixtures"]]
    out = ["{", ' "fixtures": ['] + [f"  {row}{',' if i < len(rows) - 1 else ''}" for i, row in enumerate(rows)] + [" ],"]
    rest = sorted((k, v) for k, v in doc.items() if k != "fixtures")
    for i, (key, value) in enumerate(rest):
        out.append(f" {json.dumps(key)}: {json.dumps(value, sort_keys=True)}{',' if i < len(rest) - 1 else ''}")
    out.append("}")
    return "\n".join(out) + "\n"


def load_fixtures(path: Path, name: str) -> dict:
    """The document of a fixtures file, checked for its shape. Raises FixtureError."""
    try:
        doc = json.loads(path.read_text())
    except OSError as exc:
        raise FixtureError(f"cannot read {path.name}: {exc.strerror or exc}") from exc
    except ValueError as exc:
        raise FixtureError(f"{path.name} is not JSON: {exc}") from exc
    if not isinstance(doc, dict):
        raise FixtureError(f"{path.name}: the file is not an object")
    if doc.get("format") not in FIXTURE_FORMATS:
        raise FixtureError(f"{path.name}: format {doc.get('format')!r}, this tool reads formats 1 and 2")
    if doc.get("function") != name:
        raise FixtureError(f"{path.name}: the file is for {doc.get('function')!r}, not {name}")
    if not isinstance(doc.get("fixtures"), list) or not doc["fixtures"]:
        raise FixtureError(f"{path.name}: no fixtures")
    for number, fixture in enumerate(doc["fixtures"]):
        if not isinstance(fixture, dict):
            raise FixtureError(f"{path.name}: fixture {number} is not an object")
        if "needs_image" in fixture:
            raise FixtureError(f"{path.name}: fixture {number} has the field needs_image; fixtures do not stand in for "
                               f"callees that run as original code, and this tool reads no such file")
        missing = [f for f in FIXTURE_FIELDS if f not in fixture]
        if missing:
            raise FixtureError(f"{path.name}: fixture {number} lacks {', '.join(missing)}")
        if doc["format"] == 1 and "callees" in fixture:
            raise FixtureError(f"{path.name}: fixture {number} has callees, which only format 2 has")
        for recorder in fixture["recorders"] if isinstance(fixture["recorders"], list) else []:
            if isinstance(recorder, dict) and "fills" in recorder:
                try:
                    fills_of(recorder)
                except ValueError as exc:
                    raise FixtureError(f"{path.name}: fixture {number}: a recorder's fills are malformed: {exc}") from exc
        for call in fixture["calls"] if isinstance(fixture["calls"], list) else []:
            original = isinstance(call, dict) and call.get("original") is True
            if original and doc["format"] == 1:
                raise FixtureError(f"{path.name}: a call of fixture {number} is of an original callee, which only format 2 has")
            lacking = [f for f in (ORIGINAL_CALL_FIELDS if original else CALL_FIELDS) if not isinstance(call, dict) or f not in call]
            if lacking:
                raise FixtureError(f"{path.name}: a call of fixture {number} lacks {', '.join(lacking)}")
    return doc


def fills_of(recorder: dict) -> tuple:
    """The fills of a recorder of a fixture as `CallLog.replace` takes them: from [call, argument, offset, hex].
    ValueError says what is wrong."""
    found = []
    listed = recorder["fills"]
    if not isinstance(listed, list):
        raise ValueError(f"fills is not a list: {listed!r}")
    for fill in listed:
        if not isinstance(fill, list) or len(fill) != 4 or not isinstance(fill[3], str) \
                or not re.fullmatch(r"(?:[0-9a-fA-F]{2})+", fill[3]):
            raise ValueError(f"a fill is [call, argument, offset, hex of the data], not {fill!r}")
        found.append((fill[0], fill[1], fill[2], bytes.fromhex(fill[3])))
    arguments = recorder.get("arguments")
    if not isinstance(arguments, int) or isinstance(arguments, bool):
        raise ValueError(f"the recorder's arguments is not a number: {arguments!r}")
    return contracts.CallLog._checked_fills(found, arguments)


def replay_memory(fixture: dict, gp: int = 0) -> tuple[State, dict[int, int]]:
    """The state a replay starts from: poison, then what the fixture says was read or stored without a change.
    A byte of `writes` whose input value the fixture does not give starts as its expected value with every bit
    flipped, never as the filler, so that a store that is missing shows whatever the value. Returns the state
    with the bytes the fixture's writes name."""
    ram = bytearray([POISON]) * RAM_SIZE
    scratch = bytearray([POISON]) * SCRATCH_SIZE
    # The stack is not recorded (its bytes are not compared), and a case starts with it zero.
    low, high = STACK_LOW - RAM_BASE, STACK_TOP + HOME_AREA - RAM_BASE
    ram[low:high] = bytes(high - low)
    for key in ("reads", "same"):
        for address, byte in bytes_of(fixture[key]).items():
            if address < RAM_BASE:
                scratch[address - SCRATCH_BASE] = byte
            else:
                ram[address - RAM_BASE] = byte
    stated = set(bytes_of(fixture["reads"])) | set(bytes_of(fixture["same"]))
    writes = bytes_of(fixture["writes"])
    for address, byte in writes.items():
        if address not in stated:
            if address < RAM_BASE:
                scratch[address - SCRATCH_BASE] = byte ^ 0xFF
            else:
                ram[address - RAM_BASE] = byte ^ 0xFF
    return State(bytes(ram), bytes(scratch), gp), writes


def replay_harness(state: State, fixture: dict, writes: dict[int, int]) -> contracts.CallLog | None:
    """Put the recorders of the fixture in the state, above every address the fixture uses in the arena."""
    top = ARENA_BASE
    used = [*bytes_of(fixture["reads"]), *bytes_of(fixture["same"]), *writes, *(int(r["at"], 16) for r in fixture["recorders"]),
            *(int(c["at"], 16) for c in fixture.get("callees", ()))]
    recorded_calls = [c for c in fixture["calls"] if not c.get("original")]
    # (What a stand-in writes, its `effects`, is in `writes` or in `same`: a byte that the original changed by the end of the
    # call is in `writes`, and one that it changed and restored is in `same`.)
    # What a recorder copies at a call is memory of the case too, to its last byte, whether or not the function
    # reads or writes it: a watched block, and the words behind a pointer argument. The recorders and their log
    # go above all of it, or they would lie inside what is compared. (A word that a recorder stores or adds to
    # is in `reads`, `same` or `writes` already.)
    used += [int(a, 16) + 4 * n - 1 for a, n in fixture["watch"]]
    behind = {r["callee"]: r.get("pointees", {}) for r in fixture["recorders"]}
    for call in recorded_calls:
        used += [int(call["args"][int(i)], 16) + 4 * n - 1 for i, n in behind[call["callee"]].items()]
    for address in used:
        if ARENA_BASE <= address < ARENA_END:
            top = max(top, address + 1)
    state.arena = (top + 0x100 + 15) & ~15
    # `alloc` hands out zeroed blocks because the arena of a real case is zero: it is poison here.
    state.ram[state.arena - RAM_BASE : ARENA_END - RAM_BASE] = bytes(ARENA_END - state.arena)
    if not fixture["recorders"] and not fixture["calls"]:
        return None
    watched_words = sum(n for _a, n in fixture["watch"])
    entry = {r["callee"]: 1 + r["arguments"] + sum(r.get("pointees", {}).values()) + watched_words for r in fixture["recorders"]}
    words = sum(entry[c["callee"]] for c in recorded_calls) + 8 * max(entry.values(), default=0)
    log = contracts.CallLog(state, words=words + 64, watch=tuple((int(a, 16), n) for a, n in fixture["watch"]))
    returned: dict[str, list[int]] = {}
    for call in recorded_calls:
        returned.setdefault(call["callee"], []).append(int(call["returned"], 16))
    for recorder in fixture["recorders"]:
        results = tuple(returned.get(recorder["callee"], ()))
        log.replace(int(recorder["at"], 16), recorder["arguments"], 0,
                    pointees={int(i): n for i, n in recorder.get("pointees", {}).items()} or None,
                    masks={int(i): m for i, m in recorder.get("masks", {}).items()} or None,
                    results=results, ends_run_at=recorder.get("ends_run_at", 0),
                    stores=tuple((n, int(a, 16), int(v, 16)) for n, a, v in recorder.get("stores", ())),
                    counts=tuple(int(a, 16) for a in recorder.get("counts", ())),
                    fills=fills_of(recorder) if "fills" in recorder else ())
    return log


def call_difference(number: int, want: dict | None, got: dict | None) -> str:
    """One line for the first way in which the scalars of call `number` (from 1) differ."""
    if got is None:
        return f"call {number}: missing, the original called {want['callee']}"
    if want is None:
        return f"call {number}: extra, the build called {got['callee']}"
    if want.get("original") or got.get("original"):  # a call of an original callee stands at this place in one of them
        return f"call {number}: the build called {got['callee']}, the original called {want['callee']}"
    for field in CALL_SCALARS:
        if want[field] != got[field]:
            if field == "args":
                for index, (x, y) in enumerate(zip(want["args"], got["args"])):
                    if x != y:
                        return f"call {number} to {want['callee']}: argument {index + 1} is {y}, the original gave {x}"
            return f"call {number} to {want['callee']}: {field} differ: the build {got[field]}, the original {want[field]}"
    return f"call {number}: differs"


def block_difference(number: int, callee: str, place: str, expected: bytes, seen: bytes) -> str | None:
    """A line when a block at a call is not what the fixture says, else None."""
    at = next((i for i in range(len(expected)) if expected[i] != seen[i]), None)
    if at is None:
        return None
    return (f"call {number} to {callee}: {place} differs from byte {at} (the build has {seen[at]:#04x} there at this call, "
            f"the original had {expected[at]:#04x})")


def calls_in_log(uc: Uc, log: contracts.CallLog | None) -> int:
    """How many calls of recorders the run has made so far: the entries between the start of the log and its cursor."""
    if log is None:
        return 0
    word = lambda address: struct.unpack("<I", uc.mem_read(address & 0x1FFFFFFF, 4))[0]  # noqa: E731
    end, position, count = word(log.cursor), log.entries, 0
    while position < end:
        position += 4 * log.entry_words(word(position))
        count += 1
    return count


class StandIns:
    """The stand-ins of the original callees of one fixture, for one replay of the build.

    The stub at a callee's address is `jr ra; nop`; this hook runs before its first instruction. The calls of
    the fixture are in the order they were made, recorders' and original callees' together, and the position of
    a call in them is known here from the log: its cursor says how many calls of recorders the build has made, and
    `made` holds the positions of the stand-ins' calls so far, so the call that is made now is number
    (calls of recorders so far) + (stand-in calls so far) + 1. The first thing that does not agree stops the
    run, and `failure` holds its line.
    """

    def __init__(self, fixture: dict, log: contracts.CallLog | None):
        self.calls = fixture["calls"]
        self.log = log
        self.made: list[int] = []
        self.failure: str | None = None

    def hooks(self, fixture: dict) -> tuple:
        return tuple((int(c["at"], 16), self.at_call(c["callee"])) for c in fixture.get("callees", ()))

    def at_call(self, name: str):
        def hook(uc, _address, _size, _data) -> None:
            if self.failure is None:
                self.failure = self.check(uc, name)
                if self.failure is not None:
                    uc.emu_stop()

        return hook

    def check(self, uc: Uc, name: str) -> str | None:
        position = calls_in_log(uc, self.log) + len(self.made)
        number = position + 1
        want = self.calls[position] if position < len(self.calls) else None
        if want is None or not want.get("original") or want["callee"] != name:
            return call_difference(number, want, {"callee": name, "original": True})
        self.made.append(position)
        for register, text in want["uses"].items():
            have = register_value(uc, REGISTER_NUMBERS[register])
            if have != int(text, 16):
                what = "argument" if register in ("a0", "a1", "a2", "a3") else "register"
                return f"call {number} to {name}: {what} {register} is {hex8(have)}, the original gave {text}"
        sp = uc.reg_read(reg.UC_MIPS_REG_SP)
        for offset, text in want["stack"]:
            have = struct.unpack("<I", uc.mem_read((sp + offset) & 0x1FFFFFFF, 4))[0]
            if have != int(text, 16):
                return (f"call {number} to {name}: the stack word at sp+{offset:#x} is {hex8(have)} there at this call, "
                        f"the original had {text}")
        for address, byte in sorted(bytes_of(want["reads"]).items()):
            have = uc.mem_read(address & 0x1FFFFFFF, 1)[0]
            if have != byte:
                return (f"call {number} to {name}: memory at {address:#x} is {have:#04x} there at this call, "
                        f"the original had {byte:#04x}")
        for address, byte in bytes_of(want["effects"]).items():
            uc.mem_write(address & 0x1FFFFFFF, bytes([byte]))
        for register, text in want["leaves"].items():  # exactly the registers the callee wrote, and no other
            uc.reg_write(register_unicorn(REGISTER_NUMBERS[register]), int(text, 16))
        return None


def replay_fixture(uc: Uc, build: Build, fixture: dict, gp: int = 0) -> list[str]:
    """Run the build on one fixture, the global pointer register `gp` at its begin. Returns the lines that say how it
    differs (empty: it passes)."""
    state, writes = replay_memory(fixture, gp)
    log = replay_harness(state, fixture, writes)
    # The stand-in of an original callee: two words, `jr ra; nop`, and a hook (the harness's own memory).
    stubs = [(int(c["at"], 16), 8) for c in fixture.get("callees", ())]
    for at, _length in stubs:
        state.write(at, struct.pack("<II", contracts.JR_RA, 0))
    standins = StandIns(fixture, log)
    # The real pointer of an argument masked to 0 that has a pointee, at the stand-in's entry (the log holds 0).
    pointers: list[tuple[int, str, int, int]] = []  # (call number, callee, argument, real value)
    pointer_hooks = []
    for recorder in fixture["recorders"]:
        for index in masked_pointees(recorder.get("pointees", {}), recorder.get("masks", {})):
            def at_entry(uc, _address, _size, _data, callee=recorder["callee"], index=index):
                pointers.append((calls_in_log(uc, log) + len(standins.made) + 1, callee, index, argument_at_entry(uc, index)))
            pointer_hooks.append((int(recorder["at"], 16), at_entry))
    hooks = standins.hooks(fixture) + tuple(pointer_hooks)
    setup = contracts.Setup(args=tuple(int(a, 16) for a in fixture["args"]), returns_value=fixture["result"] is not None,
                            returns=not fixture["ends_in_callee"])
    before_ram, before_scratch = bytes(state.ram), bytes(state.scratch)
    executed: set = set()
    final = run_once(uc, state, TEST_ADDRESS + build.entry, setup, executed, **({"standins": hooks} if hooks else {}))
    if standins.failure is not None:
        return [standins.failure]
    # Only the build, the recorders and the stop address may run: code at any other address is the original's
    # (or whatever the memory holds), and a replay must not run it.
    own = [(TEST_ADDRESS, len(build.code)), (STOP_ADDRESS, 16), *(log.owned if log is not None else []), *stubs]
    outside = sorted(a for a, n in executed if not any(lo <= a and a + n <= lo + length for lo, length in own))
    if outside:
        return [f"build: executed code at {outside[0]:#x}, which is neither the build nor a recorder"]
    if isinstance(final, str):
        return [f"build: {final}"]
    lines: list[str] = []
    sizes = {r["callee"]: r["pointees"] for r in fixture["recorders"] if "pointees" in r}
    for number, callee, index, value in pointers:
        size = 4 * sizes[callee][str(index)]
        if not in_stack(value, size):
            lines.append(f"call {number} to {callee}: argument {index + 1} points to {hex8(value)}, and its {size} bytes are not wholly "
                         f"inside the stack region; the original handed the callee a local of the function, the build must too")
    label = {int(r["at"], 16): r["callee"] for r in fixture["recorders"]}
    decoded = log.calls(final["ram"]) if log is not None else []
    got_recorders = [{"args": [hex8(x) for x in c.args], "callee": label[c.address], "returned": hex8(c.returned)} for c in decoded]
    # The calls in the order the build made them: a stand-in's call stands at the position its hook counted.
    standing = {position: c for position, c in zip(standins.made, (c for c in fixture["calls"] if c.get("original")))}
    got, rest = [], iter(got_recorders)
    for position in range(len(got_recorders) + len(standins.made)):
        got.append({"callee": standing[position]["callee"], "original": True} if position in standing else next(rest))
    want = [{"callee": c["callee"], "original": True} if c.get("original") else {k: c[k] for k in CALL_SCALARS}
            for c in fixture["calls"]]
    for number in range(1, max(len(got), len(want)) + 1):
        a = want[number - 1] if number <= len(want) else None
        b = got[number - 1] if number <= len(got) else None
        if a != b:
            lines.append(call_difference(number, a, b))
            break
    else:
        # The memory behind the pointer arguments and the watched blocks, at each call: the replay's own input
        # state with the recorded changes over it.
        masks_of = {r["callee"]: (r.get("pointees", {}), r.get("masks", {})) for r in fixture["recorders"]}
        recorder_calls = [(n, c) for n, c in enumerate(fixture["calls"], 1) if not c.get("original")]
        for (number, recorded), call in zip(recorder_calls, decoded):
            zero = masked_pointees(masks_of[recorded["callee"]][0], masks_of[recorded["callee"]][1])
            places = [(f"argument {int(i) + 1}", call.pointees[int(i)], None if int(i) in zero else call.args[int(i)], spans)
                      for i, spans in recorded["pointees"].items()]
            places += [(f"watched block {k + 1}", data, block, spans)
                       for k, ((block, _n), data, spans) in enumerate(zip(log.watch, call.watched, recorded["watched"]))]
            for place, data, address, spans in places:
                base = bytes(len(data)) if address is None else input_bytes(before_ram, before_scratch, address, len(data))
                expected = apply_changes(base, spans)
                if (line := block_difference(number, recorded["callee"], place, expected, data)):
                    lines.append(line)
                    break
            if lines:
                break
    if fixture["result"] is not None and setup.returns and hex8(final["v0"]) != fixture["result"]:
        lines.append(f"result: the build gives {hex8(final['v0'])}, the original gave {fixture['result']}")
    if setup.returns:
        registers = fixture.get("registers")
        saved = [int(v, 16) for v in registers["saved"]] if registers else list(INITIAL_SAVED)
        sp = int(registers["sp"], 16) if registers else STACK_TOP
        for register, x, y in zip(SAVED_NAMES, saved, final["saved"]):
            if x != y:
                lines.append(f"{register}: the build leaves {y:#x}, the original left {x:#x}")
        if sp != final["sp"]:
            lines.append(f"sp: the build leaves {final['sp']:#x}, the original left {sp:#x}")
    harness = [*(log.owned if log is not None else []), *stubs]
    skip = skip_map(harness)
    changed = changed_bytes(before_ram, before_scratch, final, skip)
    for address in sorted(set(writes) | set(changed)):
        now = (final["scratch"][address - SCRATCH_BASE] if address < RAM_BASE else final["ram"][address - RAM_BASE])
        if address in writes and now != writes[address]:
            kind = "missing write" if address not in changed else "wrong value"
            lines.append(f"{kind} at {address:#x}: the build leaves {now:#04x}, the original left {writes[address]:#04x}")
        elif address not in writes:
            lines.append(f"extra write at {address:#x}: {now:#04x}, the original wrote nothing there")
    return lines


def replay_function(cfg, name: str, build: Build, doc: dict) -> tuple[list[str], int]:
    """Replay every fixture of `doc` on `build`. Returns (lines to print, status)."""
    uc = machine(build.code)
    gp = global_pointer(cfg, name)
    passed = failed = 0
    report: list[str] = []
    for number, fixture in enumerate(doc["fixtures"]):
        try:
            lines = replay_fixture(uc, build, fixture, gp)
        except (KeyError, ValueError, TypeError, AttributeError) as exc:
            raise FixtureError(f"{name}.fixtures.json: fixture {number} cannot be read: {type(exc).__name__}: {exc}") from exc
        if lines:
            failed += 1
            note = fixture["note"] if len(fixture["note"]) <= 70 else fixture["note"][:67] + "..."
            report.append(f"  fixture {number} (case {fixture['case']}, {note}):")
            report.extend(f"    {line}" for line in lines[:3])
        else:
            passed += 1
    head = f"{name} replay: fixtures {len(doc['fixtures'])}, passed {passed}, failed {failed}"
    return [head, *report], 1 if failed else 0


def load_contracts(folder: Path, names: list[str]) -> None:
    """Take the contract of each name that has a file `NAME.py` in `folder` into `contracts.CONTRACTS`."""
    for name in names:
        path = folder / f"{name}.py"
        if name in contracts.CONTRACTS or not path.is_file():
            continue
        spec = importlib.util.spec_from_file_location(f"contract_{name}", path)
        module = importlib.util.module_from_spec(spec)
        try:
            spec.loader.exec_module(module)
        except Exception as exc:  # a contract file is input: whatever it raises is reported, not a traceback
            raise InputError(f"{path.name} cannot be loaded: {type(exc).__name__}: {exc}") from exc
        if not isinstance(getattr(module, "CONTRACT", None), contracts.Contract):
            raise InputError(f"{path.name} does not define CONTRACT, a contracts.Contract")
        contracts.CONTRACTS[name] = module.CONTRACT


def replay_main(cfg, folder: Path, names: list[str]) -> int:
    """`--replay`: the status of the replay of each function (all that have a fixtures file when none is named)."""
    if not names:
        names = sorted(path.name[: -len(".fixtures.json")] for path in folder.glob("*.fixtures.json"))
        if not names:
            print(f"INPUT ERROR: no fixtures file in {folder.name}", file=sys.stderr)
            return 2
    status = 0
    for name in names:
        try:
            doc = load_fixtures(fixtures_path(folder, name), name)
        except FixtureError as exc:
            print(f"INPUT ERROR: {exc}", file=sys.stderr)
            return 2
        with tempfile.TemporaryDirectory(prefix="difftest-") as directory:
            try:
                build = build_function(cfg, name, Path(directory), folder)
            except (matchbuild.StepError, matchbuild.EnvironmentFailure, InputError) as exc:
                print(f"BUILD ERROR: {exc}", file=sys.stderr)
                return 3
        try:
            lines, replayed = replay_function(cfg, name, build, doc)
        except (FixtureError, InputError, OSError) as exc:
            print(f"INPUT ERROR: {exc}", file=sys.stderr)
            return 2
        for line in lines:
            print(line)
        status = max(status, replayed)
    return status


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Differential test of nonmatching C against the original code")
    parser.add_argument("--config", type=Path, required=True, help="the build configuration (build.toml)")
    parser.add_argument("--folder", type=Path, default=HERE,
                        help="the folder of the sources and of their contract files (default: the tool's own)")
    parser.add_argument("--cases", type=int, default=1000)
    parser.add_argument("--seed", type=int, default=1)
    parser.add_argument("--control", action="store_true",
                        help="negative control: alter one instruction of the build and require differences")
    parser.add_argument("--uncovered", action="store_true",
                        help="list the instruction slots of the original that no case executed "
                             "(with --writes: where the first case outside wrote)")
    parser.add_argument("--writes", action="store_true",
                        help="audit where the original writes, without building the C: cases that change "
                             "bytes the setup did not make")
    parser.add_argument("--edges", action="store_true",
                        help="alter one constant of the original at a time (plus and minus one) and list the "
                             "alterations that no case notices")
    parser.add_argument("--jobs", type=int, default=None,
                        help="with --edges or --record: processes that make the altered runs (default 8; 1 runs them here)")
    parser.add_argument("--record", action="store_true",
                        help="run the original on the cases of one function and write its fixtures, NAME.fixtures.json "
                             "beside NAME.c")
    parser.add_argument("--replay", action="store_true",
                        help="test the C of each function on its fixtures, without running the original "
                             "(no function named: every function of the folder that has a fixtures file)")
    parser.add_argument("--max", type=int, default=None, dest="cap",
                        help=f"with --record: at most this many fixtures (default {FIXTURE_CAP})")
    parser.add_argument("--all", action="store_true", help="every function that has a source in the folder")
    parser.add_argument("functions", nargs="*", metavar="FUNC")
    args = parser.parse_args(argv)
    folder = args.folder.resolve()
    if args.writes and args.control:
        print("INPUT ERROR: --writes and --control do not combine", file=sys.stderr)
        return 2
    if args.edges and args.control:
        print("INPUT ERROR: --edges and --control do not combine", file=sys.stderr)
        return 2
    if args.edges and args.writes:
        print("INPUT ERROR: --edges and --writes do not combine", file=sys.stderr)
        return 2
    if args.record and args.replay:
        print("INPUT ERROR: --record and --replay do not combine", file=sys.stderr)
        return 2
    if (args.record or args.replay) and (args.control or args.writes or args.edges):
        print("INPUT ERROR: --record and --replay do not combine with --control, --writes or --edges", file=sys.stderr)
        return 2
    if args.cap is not None and not args.record:
        print("INPUT ERROR: --max applies to --record only", file=sys.stderr)
        return 2
    if args.cap is not None and args.cap < 1:
        print("INPUT ERROR: --max needs 1 or more", file=sys.stderr)
        return 2
    if args.jobs is not None and not (args.edges or args.record):
        print("INPUT ERROR: --jobs applies to --edges and --record only", file=sys.stderr)
        return 2
    if args.record and (args.all or len(args.functions) != 1):
        print("INPUT ERROR: --record takes one function", file=sys.stderr)
        return 2
    if args.jobs is not None and args.jobs < 1:
        print("INPUT ERROR: --jobs needs 1 or more", file=sys.stderr)
        return 2
    if args.replay and args.all:
        print("INPUT ERROR: --replay takes the functions named, or none for every fixtures file of the folder", file=sys.stderr)
        return 2
    if not args.replay and args.all == bool(args.functions):
        print("INPUT ERROR: name the functions, or give --all and none", file=sys.stderr)
        return 2
    if args.all:
        args.functions = sorted(path.stem for path in folder.glob("func_*.c"))
        if not args.functions:
            print(f"INPUT ERROR: no func_*.c in {folder.name}", file=sys.stderr)
            return 2

    try:
        cfg = matchbuild.load_config(args.config.resolve())
    except matchbuild.ConfigError as exc:
        for error in exc.errors:
            print(f"CONFIG ERROR: {error}", file=sys.stderr)
        return 2
    except (InputError, OSError) as exc:
        print(f"INPUT ERROR: {exc}", file=sys.stderr)
        return 2
    scratch = bytes(SCRATCH_SIZE)
    if args.replay:
        return replay_main(cfg, folder, args.functions)
    try:
        load_contracts(folder, args.functions)
    except (InputError, OSError) as exc:
        print(f"INPUT ERROR: {exc}", file=sys.stderr)
        return 2

    status = 0
    for name in args.functions:
        if name not in contracts.CONTRACTS:
            print(f"INPUT ERROR: no contract for {name}", file=sys.stderr)
            return 2
        try:
            original_size = original_function(cfg, name)[1]
            ram = initial_memory(cfg, image_of(name))
        except (InputError, OSError) as exc:
            print(f"INPUT ERROR: {exc}", file=sys.stderr)
            return 2
        if args.writes:
            discarded, outside, largest, first = audit_writes(cfg, name, args.cases, args.seed, ram, scratch)
            print(f"{name} writes: cases {args.cases}, discarded {discarded}, outside {outside} (largest {largest} bytes)")
            if outside and args.uncovered:
                print(f"  first: {first}")
            if outside:
                status = 1
            continue
        with tempfile.TemporaryDirectory(prefix="difftest-") as directory:
            try:
                build = build_function(cfg, name, Path(directory), folder)
            except (matchbuild.StepError, matchbuild.EnvironmentFailure, InputError) as exc:
                print(f"BUILD ERROR: {exc}", file=sys.stderr)
                return 3
        if args.record:
            try:
                lines, recorded = record_function(cfg, name, build, args.cases, args.seed,
                                                  FIXTURE_CAP if args.cap is None else args.cap, ram, scratch, folder,
                                                  8 if args.jobs is None else args.jobs)
            except (InputError, OSError) as exc:
                print(f"INPUT ERROR: {exc}", file=sys.stderr)
                return 2
            for line in lines:
                print(line)
            status = max(status, recorded)
            continue
        if args.edges:
            try:
                lines, swept = edge_sweep(cfg, name, build, args.cases, args.seed, ram, scratch, args.uncovered,
                                         8 if args.jobs is None else args.jobs)
            except (InputError, OSError) as exc:
                print(f"INPUT ERROR: {exc}", file=sys.stderr)
                return 2
            for line in lines:
                print(line)
            status = max(status, swept)
            continue
        code = build.code
        if args.control:
            # The control sees the unit's code, not its read-only data, and may alter one word of it.
            words = list(struct.unpack(f"<{build.size // 4}I", code[: build.size]))
            index, word, what = contracts.CONTRACTS[name].control(words)
            if not 0 <= index < len(words):
                print(f"INPUT ERROR: the control of {name} names word {index} of {len(words)}", file=sys.stderr)
                return 2
            code = code[: 4 * index] + struct.pack("<I", word) + code[4 * index + 4 :]
        discarded, equal, different, first, executed = test_function(cfg, name, code, args.cases, args.seed, ram, scratch, build.entry)
        if args.control:
            print(f"{name} control: different {different} of {args.cases} (expected more than 0)")
            print(f"  altered: {what}, instruction slot {index}")
            if different == 0:
                status = 1
            continue
        print(f"{name}: built {build.size} bytes, original {original_size} bytes; "
              f"cases {args.cases}, discarded {discarded}, equal {equal}, different {different}")
        for line in first:
            print(f"  {line}")
        slots = original_size // 4
        print(f"{name} coverage: {len(executed)} of {slots} instruction slots of the original executed")
        if args.uncovered and len(executed) < slots:
            print(f"  not executed: {ranges_text([o for o in range(0, original_size, 4) if o not in executed])}")
        if different or not equal:
            status = 1
    return status


if __name__ == "__main__":
    sys.exit(main())
