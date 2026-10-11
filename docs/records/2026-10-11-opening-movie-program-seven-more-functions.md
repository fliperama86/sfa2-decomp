## Seven more functions of the opening movie's program as C (2026-10-11)

What was added: C for seven more own functions of the opening
movie's program (`PAC/Z2O.EXE`) in `ps1/src/programs/z2o/`, each with
its contract, its five test runs on the folder's page and its
fixtures; their data names and the library routines they call in
`z2o.h` and in the program's entry of `programs.toml`. Thirteen of
the program's functions have C now. Nothing of the program is in the
PC program yet.

The seven (names and roles inferred from the code): two setters that
store a volume or a stereo flag, make the four mix bytes from both
and hand them to the drive's mix call (`func_800115a8_z2o`,
`func_80011630_z2o`); the disc volume's fade up and fade down in
steps of 2, one mix call a step (`func_80011830_z2o`,
`func_800118f0_z2o`); the start of the disc, the init up to four
times, a mode, a seek (`func_80011a24_z2o`); the end of the movie,
the fade down, the stream off, a wait for the drive, a sector taken,
the mode set again, the disc paused, the decoder's callback off
(`func_8001139c_z2o`);
and the graphics set-up with the two display environments, both in
the 24-bit mode (`func_80010ae8_z2o`).

How they are tested: every callee, library routine or function of
the program, is a recorder, so each function is tested alone and no
hardware access runs. The results of the routines that are called
until they succeed are scripted case by case (failures, then a
success; for the init also four failures), so that every loop is
entered, left and run more than once. The mix call's recorder logs
the four bytes behind its pointer. Where a function hands a callee
the address of a local (the mode byte and the position of the disc's
start, the rectangle of the graphics set-up), the argument is masked
and its pointee compared; the position that a library routine would
write is a fill of the recorder. The fades' volume word is kept small
by the setups (up to 300, and negative values for the fade down): a
large value would loop for a very long time, and the headers say so.
None of the seven uses the global pointer register (each listing was
read for it; each header says so).

What ran, each function with 2,000 cases, on the folder's page: the
comparison with seeds 1 and 7 (`different 0`, `discarded 0`), the
control (`different` above 0 for each: 2000 of 2000 for two, and
1039, 992, 952, 858 and 827 of 2000 for the others), the write audit
with both seeds (`outside 0`), the record with no warning, and the
replay (`failed 0`; 14 fixtures in the seven files). Every
instruction slot of the original was executed for each. All of these
lines were printed again on the final source by a private script that
runs every command of the page with its status and compares each line
with the page: 13 functions, no line missing, no run failed, 3
minutes 39 seconds. `python -B ../programs/test_programs.py`: `all
cases behaved as required`. `python
tools/ai_workflow/tests/test_workflow.py`: OK. The names that the
setups take were linted against each function's C (none missing; a
private script).

Publication: no record warned. The seven new fixtures files hold 93
bytes at addresses inside the program's body, and none that is not
zero equals the program's byte there (a private check; the folder's
one such byte is the one named with the first six functions).

Limits, stated: a pass is evidence for the tested inputs of a
contract, not equivalence; no tool compares these functions' bytes
with the original's. The fades are tested for small volumes only.
`CdReadCallback` is declared in the documented form of Sony's
library, which the game's headers do not have. Five functions of the
program that have a caller, and nine that have none, have no C yet
(a private count from the program's listing).
