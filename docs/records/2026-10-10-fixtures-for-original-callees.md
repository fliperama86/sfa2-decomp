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

With the 101 of pull request 180, the six replay commands print 134
lines, one per function with a file, every one `failed 0` (about 18
seconds for the six started together, one-off timing). The lines of the
101 are the lines they printed before.

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

Tests: `test_difftest.py`, 396 cases, "all cases behaved as required"
(347 before); `test_difftest_build.py --config ../build.toml`, the same
line; 68 mutants of the new code in bounded runs, each failing a named
case; `python tools/ai_workflow/tests/test_workflow.py`, OK.

Not done and not shown: no contract and no C file was changed; the
functions still without a file stay with the wide comparison (their
pages say which and why); a replay is evidence for the recorded inputs,
not the wide comparison and not equivalence; the stand-in sets the
registers a callee leaves, which only matters to code that uses such a
leftover; no replay that needs nothing private yet.
