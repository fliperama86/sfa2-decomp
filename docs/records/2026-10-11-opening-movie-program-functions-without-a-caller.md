## Eight functions of the opening movie's program that nothing calls, as C (2026-10-11)

What was added: C for eight own functions of the opening movie's
program (`PAC/Z2O.EXE`) that have no caller in the program, in
`ps1/src/programs/z2o/`, each with its contract, its five test runs
on the folder's page and its fixtures. Twenty-seven functions of the
program have C. The PC program builds them in with the rest of the
program and never calls them.

"No caller" is inferred from a private scan of the program's whole
file for each address: jumps and calls to it, data words equal to
it, and pairs of instructions that build it; every count was 0. Each
header says so.

The eight (names and roles inferred from the code): three that return
a word of the program's data (`func_800116d8_z2o`,
`func_800116e8_z2o`, `func_800116f8_z2o`; what the third's word means
is not settled); two that store their argument (`func_800119c8_z2o`,
`func_800119d8_z2o`); a test whether the frame number has reached the
movie's count, an unsigned comparison (`func_80011458_z2o`); a pause
or resume of the disc: with the argument 1 the sound down and the
drive paused, otherwise the stream's read started again in the mode
the open chose (`func_80011530_z2o`); and a routine that draws a
picture in bands of 16 or 24 pixels with one picture transfer and a
wait a band (`func_80011478_z2o`).

How they are tested: as the folder's other functions. Every register
is 0 at the start of a run but the arguments; the result register
(where there is a result) and memory are compared after the call;
callees are recorders with scripted results, never run. The band
routine's count of bands is kept at 8 or fewer by the setup (a large
count would mean millions of calls), so its loop's unsigned
comparison cannot be told from a signed one; the header, the contract
and the page say so.

What ran, each function with 2,000 cases, on the folder's page: the
comparison with seeds 1 and 7 (`different 0`, `discarded 0`, every
instruction slot of the original executed), the control (`different`
above 0 for each: 2000 of 2000 for five, and 1895, 676 and 614 of
2000 for the others), the write audit with both seeds (`outside 0`),
the record with no warning, and the replay (`failed 0`; 45 fixtures
in the eight files). These lines were printed again on the final
source by a private script that compares each line with the page: 8
functions, no line missing, no run failed, 1 minute 44 seconds. The
folder's replay: `failed 0` for all twenty-seven. `python -B
../programs/test_programs.py`: `all cases behaved as required`.
`python tools/ai_workflow/tests/test_workflow.py`: OK. The names
that the setups take were linted against each function's C (none
missing; a private script). After each function the PC program was
built without the picture library and linked and verified (`program
z2o: 27 functions` after the last).

Publication: no record warned. The eight new fixtures files hold 8
bytes at addresses inside the program's body, and none that is not
zero equals the program's byte there (a private check).

Not here: the ninth function without a caller, the open of a movie by
its file's name (0x80010f40). Its C and its test exist privately and
pass, but it calls a library routine, `CdSearchFile`, that the port's
list does not bind, and with it the PC build stops (`these symbols
... are not bound ...: z2o: CdSearchFile`). It was taken out of this
change so that the PC build keeps linking; it comes with a routine of
the runtime for that call.

Limits: a pass is evidence for the tested inputs of a contract, not
equivalence; no tool compares these functions' bytes with the
original's.
