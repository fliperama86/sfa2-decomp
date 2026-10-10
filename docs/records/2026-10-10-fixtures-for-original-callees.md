## Fixtures for functions whose callees run as original code (2026-10-10)

What changed: `difftest.py --record` no longer refuses a function
because a callee of it is not replaced by a recorder and so runs as the
game's own code. It records each call of such a callee: the argument
registers the callee read before writing them, the stack words it read
through the stack pointer, the bytes it read that the setup made or the
run had written, the bytes it changed and the registers it leaves.
`--replay` puts a stand-in at the callee that checks the first three
and does the last two; the callee's code is never run in a replay. A
file with such a call is format 2; the files without one are format 1
as before, byte for byte (one published file was recorded again into a
copy of its folder and compared).

`--record` also replays every kept fixture on the build before it
writes the file. A fixture that fails is a refusal of the function, so
a file whose own replay fails cannot be written any more (the first
wide record, in pull request 180, had found 24 such files, a fault of
the replay then).

The first use, on the 53 functions that pull request 180 listed under
"the contract lets a callee run as original code". Figures of
2026-10-10 from the outputs of the records and from a private script,
one-off:

- 33 have a file now.
- 15 read bytes of the game's image and have none: for 10 the record
  warned at once; 5 more were found when the count was corrected (see
  below).
- 2 are over 100,000 bytes also with six cases (one with 500 cases
  instead of 2,000, because its record with 2,000 did not end in ten
  minutes).
- 2 are refused: a callee takes a pointer to a local of the function,
  which a build places elsewhere. A limit of the tool.
- 1 fails its record's own check: its callee reads the low byte of an
  argument register, the two codes differ in the bits above, and the
  stand-in compares the whole register. A limit of the tool.

With the 101 of pull request 180, the six replay commands printed 134
lines at that point, one per function with a file, every one `failed 0`
(about 18 seconds for the six started together, one-off timing). The
lines of the 101 are the lines they printed before. (After the review,
below, it is 133.)

A fault of this change's first design, found at the top level before
anything was pushed: the record left out the bytes that an original
callee reads and that the setup did not make (content of the game's
image), and did not count them. But what the callee makes of such bytes
is in the file: the bytes it stores and the registers it leaves. The
record now counts them like a read of the function's own and warns. A
private script ran the kept cases of the 38 files recorded until then
through the corrected record: 33 read no such byte, 5 do, and those 5
are not in the tree and not in the branch's history (the branch was
built again on main without them).

A finding of the owner's review, corrected in this change: the stand-in
set every recorded volatile register, also ones the callee never wrote,
and `uses` checked only the argument registers. His case, all made up:
the caller sets `a0 = 7`, calls a callee that is `jr ra; nop`, and
returns `a0`; a build that sets 8 really returns 8, and the replay
passed, because the stand-in put the recorded 7 back. Now the trace
keeps every register of a call as untouched or written. A register the
callee saves into its own memory and loads back is untouched again.
`leaves` holds exactly the registers that are written when the call
ends, and the stand-in sets those and no other. `uses` holds every
caller register the callee read while it was untouched (the argument
registers, the temporaries, `at`, `v0`, `v1`, `gp`, `hi`, `lo`), and the
stand-in checks each. A callee that reads a register its caller must
keep (`s0` to `s7`, `fp`, `k0`, `k1`, or `ra` other than to return) in
any other way than to save it is refused, and so is one that leaves the
stack pointer changed. The 33 format 2 files were recorded again under
these rules: none warned, none was refused, the replay lines are the
same. The one function that fails its record's own check
(`func_8011df3c`) fails it as before.

Two more findings of the owner's review on the same rule, corrected in
this change. The trace kept one save slot for each register, so a
second save of the same register hid the first copy, and a load of the
first into another register was no use; and a store of one byte ended
the whole slot, so the three bytes that still held the register's value
were no copies any more. His cases: `sw a0,0(sp); sw a0,4(sp); lw
v0,0(sp)`, and `sw a0,0(sp); sb zero,0(sp); lbu v0,1(sp)`; in both a
build with another `a0` really returns another value, and the replay
passed. Now every byte of a saved word is a copy of its register until
a store writes that very byte, a register may have any number of
copies, a restore is a load of the whole word into the same register
while all four bytes are still its copies, and any other load of a byte
that is still a copy is a use. Two patterns that nothing could check
are refused from now on: a callee that reads the stack pointer in any
way but as the base of a load or a store or to move it (it takes the
address of its stack), and a callee that loads a byte of its own frame
that it has not written during the call (what earlier code left on the
stack). The 33 format 2 files were recorded again: 32 came out byte for
byte as they were; one function is refused by the first of the two new
refusals (`func_801b1b40_slot04_0e`: a callee hands a local of its own
to a function it calls) and has no file any more. So 133 functions have
a file, and the six replay commands print 133 lines, every one
`failed 0`.

Tests: `test_difftest.py`, 420 cases, "all cases behaved as required"
(347 before this change); `test_difftest_build.py --config
../build.toml`, the same line; 94 mutants of this change's code as it
was before the last two findings, in bounded runs, each failing a named
case then; 17 more of the copy tracking and of the two new refusals,
each failing a named case (an eighteenth changes nothing that runs and
was dropped);
`python tools/ai_workflow/tests/test_workflow.py`, OK.

Not done and not shown: no contract and no C file was changed; the
functions still without a file stay with the wide comparison (their
pages say which and why); a replay is evidence for the recorded inputs,
not the wide comparison and not equivalence; the stand-in sets the
registers a callee leaves, which only matters to code that uses such a
leftover; no replay that needs nothing private yet.
