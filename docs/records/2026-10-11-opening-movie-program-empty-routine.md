## The opening movie program's empty library routine as a function with C (2026-10-11)

What was added: the routine at 0x80011afc of the opening movie's
program (`PAC/Z2O.EXE`), which the program's main calls first, is one
of the program's functions now, `func_80011afc_z2o`, with C, a
contract, its five test runs on the folder's page and its fixtures.
Its listing is a return and its delay slot (read privately); the C is
an empty function. It was a symbol of the program's declaration
before; nineteen functions of the program have C.

Why: the PC build takes a name that a program's C uses as a function
of the program with C, a data name, a name of the game, or a library
routine bound to a routine of the runtime. The game's headers have no
name for this routine, and it does nothing, so C for it is the plain
way and the port's list needs no entry for it.

How it is tested: the four argument registers are random and the
result register is compared, so that a routine that used an argument
or changed the result would show; the bytes around one word of the
program's data are random, so that a store would show. The control
puts an instruction that changes the result register into the
return's delay slot.

What ran, 2,000 cases, on the folder's page: the comparison with
seeds 1 and 7 (`different 0`, `discarded 0`, 2 of 2 instruction slots
executed), the control (`different 2000` of 2000), the write audit
with both seeds (`outside 0`), the record with no warning and the
replay (`failed 0`, 1 fixture). The main's five runs and replay print
the lines the page has for it (its callee is declared a function
now). The page's commands for the new function, the main and the
open were run again by a private script that compares each line with
the page: 3 functions, no line missing, no run failed, 3 minutes 23
seconds. The folder's replay: `failed 0` for
all nineteen. `python -B ../programs/test_programs.py`: `all cases
behaved as required`. `python
tools/ai_workflow/tests/test_workflow.py`: OK. The new fixtures file
holds no byte at an address inside the program's body (a private
check: the folder's count stays 229).

Also here, one of the two comments that the review of pull request
212 named, text only: the header of the movie's open says what its
contract does with the pointer of the drive's mode command (a local's
address compared as it is, which holds only while the C's frame has
the original's layout), where it had said the pointer was masked and
its pointee logged.

Not here, and why: the other comment is in the main's contract file
(its model of the flip equals the real flip for the indices 0 and 1
only). The review command asks for a fresh comparison of a function
whose contract file changed, and it cannot run one for a function of
a program: its frozen copy gets the private inputs that the build
configuration names, and a program's file is named by the programs'
declaration instead (`INPUT ERROR: program z2o: the file Z2O.EXE
cannot be read`). The correction waits for that gap of the tool to be
closed in a change of its own; the wording is ready.

Limits: a pass is evidence for the tested inputs, not equivalence; no
tool compares the function's bytes with the original's.
