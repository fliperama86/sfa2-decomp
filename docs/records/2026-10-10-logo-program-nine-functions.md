## Nine functions of the logo's program as C, with contracts and fixtures (2026-10-10)

What was added: C for nine of the logo program's own functions in
`ps1/src/programs/logo/`, each with its contract, its five test runs
on the folder's page and its fixtures; the header `logo.h` with the
program's data by address and the prototypes of the library routines
the functions call that the game's headers do not have; their rows and
names in `ps1/src/programs/programs.toml`. With the first function the
folder holds ten of the program's sixteen own functions. Nothing of
the program is in the PC program yet.

The nine (names and roles inferred from the code, not original
names): an empty function (`func_80011838_logo`); the wait on a flag
word (`func_80010f20_logo`); the flip between two display environments
(`func_80010a38_logo`); the movie decoder's output routine, which
copies a strip to the picture and asks for the next
(`func_80010f38_logo`); the expansion of run-length data
(`func_800110f4_logo`); the disc volume's fade up and fade down
(`func_80010fc4_logo`, `func_80011060_logo`); the drawing of a still
picture (`func_800111d0_logo`); the set-up of the graphics and the two
display environments (`func_80010928_logo`).

How they are tested: a callee is never run; a recorder stands at its
address, so no library code and no hardware access runs in a case.
Each contract makes every byte its function reads, so no case reads a
byte of the program's data and no record warned. These functions reach
their data through the global pointer in the original; the tool begins
their runs with the declared value (the change before this one). A
control of that, run once by hand with the declared value taken out of
the declaration, 200 cases each: `func_80010a38_logo` and
`func_80010f38_logo` differ in 200 of 200, `func_800110f4_logo` in 185
of 200; the waiting function and the first function do not show it
(the first does not use the register, and the waiting function reads a
zero word either way).

Names: the program's data is `data_ADDRESS_logo`. Two library routines
are called by the names the game's own headers have for them
(`func_80157fc4` and `func_80158470`), bound in the declaration to the
program's own copies; the others by Sony's documented names
(`DecDCTout`, `StCdInterrupt`, `CdMix`, `SetVideoMode`, the graphics
calls). One name of the private table of the program's functions was
wrong and is corrected here: the routine the graphics set-up calls
with the video mode word is `SetVideoMode`, not a callback setter (it
stores its argument in the word that the routine next to it, named
`GetVideoMode` in the game's symbols, returns).

What ran, each function with 2,000 cases, on the folder's page: the
comparison with seeds 1 and 7 (`different 0`, `discarded 0`, every
instruction slot of the original executed), the control (`different`
above 0 for each: 2000 of 2000 for six, 991, 1001 and 1463 of 2000
for the two fades and the expansion), the write audit with both seeds
(`outside 0`), the record with no warning, and the replay (`failed 0`;
17 fixtures in the nine files). All of these lines were printed again
on the final source by a private script that runs every command of
the page and compares each line with the page: 10 functions, no line
missing. `python -B ../programs/test_programs.py`: `all cases behaved
as required`.

Limits, stated: a pass is evidence for the tested inputs of a
contract, not equivalence; no tool compares these functions' bytes
with the original's (seven of the nine build to another size than the
original, as C that names its data by address does). The waiting
function is tested with the flag at 0 only: with another value it ends
only through an interrupt, which the tool does not model. The graphics
set-up hands a callee the address of a local, and the test compares
that address, which holds only while the C's frame places the local
where the original's does. The fades' controls alter one of the two
loops the compiler emits; which one was not looked at. One function of
this package is not here: the start of the disc
(`func_80011794_logo`) hands a callee a local that the callee fills,
which the fixtures cannot describe yet.
