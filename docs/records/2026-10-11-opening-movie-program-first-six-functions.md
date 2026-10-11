## The opening movie's program in the test tool, with its first six functions as C (2026-10-11)

What was added: the disc's second stand-alone program, `PAC/Z2O.EXE`
(inferred: the player of the opening movie), is declared in
`ps1/src/programs/programs.toml` under the name `z2o`, with its folder
`ps1/src/programs/z2o/` (a page and a header in the form of the
logo's) and C for six of its own functions, each with its contract,
its five test runs on the page and its fixtures. Nothing of the
program is in the PC program yet.

The declaration: the file's hash as `discfile.py` prints it for the
user's copy (131,072 bytes), the load address from the file's header,
and no global pointer. The entry code first clears the words from
0x8002f7f0 up to 0x8008a400 and only then sets the register to
0x8002f7f0 before it jumps to the main; the reader checks a declared
value against the entry code's first two instructions only, so none
is declared and the test tool begins each run with the register at 0.
That is right only for a function that does not use the register:
the listing of each of the six was read for it, and each header says
so. A later function of this program that uses the register needs
the reader extended first.

The six (names and roles inferred from the code): two that store a
value (`func_800116c8_z2o`, a byte; `func_800119b8_z2o`, a word); the
stream's frame routine, which adds 1 to a counter
(`func_80011810_z2o`); the wait until a flag word is 0, then a copy of
one word to another (`func_80011428_z2o`); a look-up in a table of
12-byte entries that returns an entry's first word and stores its
second through a pointer (`func_800119e8_z2o`); and the flip of the
two display buffers, which hands the current display environment to
the program's copy of a library routine and inverts the index
(`func_80010be4_z2o`).

How they are tested: as the logo's. The one callee is a recorder that
logs the pointer it gets and the five words behind it, with the index
word watched at the call. The waiting function is tested with the
flag at 0 only: with another value it ends only through an interrupt,
which the tool does not model, so the test shows the exit, the read
of the flag and the copy, not the waiting.

What ran, each function with 2,000 cases, on the folder's page: the
comparison with seeds 1 and 7 (`different 0`, `discarded 0`), the
control (`different 2000` of 2000 for each), the write audit with both
seeds (`outside 0`), the record with no warning, and the replay
(`failed 0`; 17 fixtures in the six files). Every instruction slot of
the original was executed for each. All of these lines were printed
again on the final source by a private script that runs every command
of the page with its status and compares each line with the page: 6
functions, no line missing, no run failed, 1 minute 17 seconds. The
replay of the logo's sixteen functions still gives `failed 0`.
`python -B ../programs/test_programs.py`: `all cases behaved as
required`. `python tools/ai_workflow/tests/test_workflow.py`: OK. The
names that the six setups take were linted against each function's C
(none missing; a private script).

Publication: no record warned. Of the 96 bytes the six fixtures files
hold at addresses inside the program's body, one that is not zero
equals the program's byte there (0x02 at 0x8001b3aa, in a case of the
table look-up whose entry the setup filled with random bytes; the
seven bytes beside it differ). A private check; one byte in 256 does
that by chance.

Limits, stated: a pass is evidence for the tested inputs of a
contract, not equivalence; no tool compares these functions' bytes
with the original's. The third program of the disc (`PAC/BF3.EXE`)
is the same file but for 8 bytes (a private comparison of the two
files) and is not declared here.
