## The global pointer of a stand-alone program in the test tool (2026-10-10)

What was added: `ps1/src/programs/programs.toml` can state a program's
global pointer (`gp`), the reader checks the stated value against the
program's own entry code, and the differential test tool begins every
run of a function of that program with the register at that value.
The logo's program gets its value, 0x8002eafc.

Why: the code of the logo's program reaches its small data through
the global pointer register. The test tool began every run with the
register at 0, which is right for the game, whose code does not use
it; an original function of the program that reads through the
register would have read from the wrong place. The program's first
function in the tree does not use the register, so this did not show
yet.

Where the value comes from: the header of the program's file has a
field for the register, and in the logo's file it is 0. The register
is set by the two instructions at the header's entry address, before
the program's main. The reader (`programs.py`, `read_body`) takes the
entry address from the header and accepts a declared `gp` only when
those two instructions are `lui gp,H` and `addiu gp,gp,L` and give
that value; another value, another form of the entry code, or an
entry address outside the body is refused. A program without a
declared `gp` is not looked at there and runs with 0.

In the test tool (`ps1/src/slot06_nonmatching/difftest.py`): a case's
state carries the register's value for the begin of a run; the
comparison, the write audit, the record with its own replay, and the
replay take it from the declaration for a function of a program and
use 0 for a function of the game. The C of a program's function names
its data by address and does not use the register, so a replay's
result does not depend on it today; it is set there all the same, so
that a run begins the same way in every mode.

What ran: `python -B test_difftest.py`, "all cases behaved as
required", 446 cases (4 new); `python -B test_programs.py`, the same
line, 33 cases (4 new); `python -B test_difftest_build.py --config
../build.toml`, the same line. With the real file of the logo's
program the reader accepted the declared value. The first function of
the logo's program: `different 0` in 2,000 cases, replay `fixtures 12,
passed 12, failed 0`; a function of the game with fixtures,
`func_8012fd80`: replay `fixtures 6, passed 6, failed 0`.

Mutants in bounded runs by a private script, by kind: of the reader's
new lines 13, each failing a named case; of the test tool's new lines
11, each failing a named case; one change of nothing in each group
passed, as it must. None ended a tester without a result and none ran
into a time limit. A case of the new tests found a place the change
had missed (the record's own replay began with 0) before any of this
was committed.

Limits, stated: the reader knows one form of entry code; a program
that sets the register in another way cannot state a `gp` until the
reader learns that form. Nothing here shows that a function of the
program that uses the register is tested rightly: no such function is
in the tree yet. A fixtures file does not hold the register's value;
it comes from the declaration at replay.
