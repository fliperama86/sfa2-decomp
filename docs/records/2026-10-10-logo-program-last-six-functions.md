## The last six functions of the logo's program as C: all sixteen have C now (2026-10-10)

What was added: C for the six remaining own functions of the logo's
program in `ps1/src/programs/logo/`, each with its contract, its five
test runs on the folder's page and its fixtures; their data names and
the library routines they call in `logo.h` and in
`ps1/src/programs/programs.toml`. With them every one of the
program's sixteen own functions has C. Nothing of the program is in
the PC program yet.

The six (names and roles inferred from the code): the start of the
disc (`func_80011794_logo`: the init up to four times, a mode, a
seek); the end of the movie (`func_80010e28_logo`: the sound down, the
stream off, the disc paused, the picture cleared twice); the start of
the stream at a sector and the first frame (`func_80010a80_logo`); the
set-up (`func_800107e8_logo`: the buffers from a base, the read of the
stills' file with its 32 pointers made absolute, graphics, stream,
sound up); one frame (`func_80010c20_logo`: the next frame from the
ring with a count of polls, a still for some frame numbers, else the
decoder fed, the next frame's bitstream decoded, the wait, the flip);
and the main (`func_800106fc_logo`: the frame loop until a frame says
so or Start or circle of either pad is newly pressed).

How they are tested: every callee, library routine or function of the
program, is a recorder with scripted results, so each function is
tested alone and no hardware access runs. Where a callee writes
through a pointer what the function then reads (a position, a status
byte, the two pointers a frame is handed back through, the stills'
file), the recorder has fills; where the function hands a callee the
address of a local, the argument is masked and its pointee compared
(both from the change of the test tool before this one). The setups
follow each function's flow to script the results: retries, a status
bit that comes late, frame numbers on and around every limit, pad
words with held and newly pressed bits.

What ran, each function with 2,000 cases, on the folder's page: the
comparison with seeds 1 and 7 (`different 0`, `discarded 0`), the
control (`different` above 0 for each: 2000 of 2000 for two, 1544,
745, 149 and 59 of 2000 for the others), the write audit with both
seeds (`outside 0`), the record with no warning, and the replay
(`failed 0`; 26 fixtures in the six files). Every instruction slot of
the original was executed for five; for `func_80010c20_logo` 128 of
130. All of these lines were printed again on the final source by a
private script that runs every command of the page with its status
and compares each line with the page: 16 functions, no line missing,
no run failed. `python -B ../programs/test_programs.py`: `all cases
behaved as required`. Of the bytes the sixteen fixtures files hold at
addresses inside the program's body (1,545), none that is not zero
equals the program's byte there (a private check).

Also here: one sentence of the page that the owner's review of the
functions before had named: the graphics set-up sets the two
`isrgb24` bytes to 1 and only the index word to 0.

Limits, stated: a pass is evidence for the tested inputs of a
contract, not equivalence; no tool compares these functions' bytes
with the original's. In `func_80010c20_logo` the count of 0x100000
polls cannot run out within the tool's budget of instructions, so the
two slots of that exit are not executed and the count's start and the
exit's value 1 are not tested; its header names them. The test of
`func_80010a80_logo` logs two pointers into its own frame unmasked
(fills and a pointer five bytes into a local need them), so it holds
only while the C's frame has the original's size, as the graphics
set-up's does. In the main, two increments could be 2 instead of 1
without a difference the test compares (the loop only asks whether
the sum is 0). Two fixtures files are large (62 and 38 kilobytes) for
the long scripted flows.
