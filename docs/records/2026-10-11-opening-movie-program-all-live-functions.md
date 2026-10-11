## The opening movie's program: its last five functions with a caller as C (2026-10-11)

What was added: C for five more own functions of the opening movie's
program (`PAC/Z2O.EXE`) in `ps1/src/programs/z2o/`, each with its
contract, its five test runs on the folder's page and its fixtures.
With them every own function of the program that has a caller,
eighteen, has C. Nine that have none (a private count from the
program's listing) do not. Nothing of the program is in the PC
program yet.

The five (names and roles inferred from the code):
- `func_80011708_z2o`, the decoder's completion routine: counts a
  strip; when a flag word says so, calls the stream's sector routine
  and clears the word; draws the strip unless a word forbids it; and
  while strips remain, steps the rectangle and starts the next one.
- `func_80011018_z2o`: opens a movie by its index in the file table
  and hands its nine other arguments on.
- `func_80010c38_z2o`, the open: stores its arguments, resets the
  decoder, sets the completion routine, the ring and the stream,
  fades the sound down, sets the filter and the position, the mode,
  waits 51 vblanks, starts the stream's read (with a retry), waits
  until the drive says it reads, fades the sound up, takes the first
  frame's header and decodes its bitstream.
- `func_800110d0_z2o`, one frame: takes the next frame from the ring
  (a count of 0x100000 polls), sets the rectangle and the sizes from
  the header kept before, feeds the decoder and starts its output,
  flips the buffer index, keeps the new header and decodes its
  bitstream; returns 1 when the frame number has reached the movie's
  count, or when the polls ran out.
- `func_800108e8_z2o`, the main: the set-up (callbacks, graphics, the
  disc, the pads, a library set-up call of the geometry routines, the
  program's own setters, the open of file 0xff with 0x334 frames and
  a ring of 0x1e slots); the loop of a frame, the wait, a vblank, the
  flip and the pads, left on Start or circle of either pad (a word of
  the scratchpad is then set to 1) or when the frame routine says so;
  the movie's end; the picture cleared twice.

How they are tested: as the functions before them. Every callee is a
recorder with scripted results; the six arguments on the caller's
stack are written by the setups; where a callee writes through a
pointer what the function then reads (the two pointers a frame is
handed back through, the drive's result bytes), the recorder has
fills; recorders also store into words that an interrupt could change
between two reads of the function, so that the re-reads show.

The two branches of this work were written side by side and merged at
the top level, which gave the open and its two callers one set of
parameter types (the open-by-index and the main had passed plain
words where the open takes pointers) and typed the two bitstream
buffer pointers as a table of two, as the logo's are: the movie's
end, merged before, reads the table's first entry now. All eighteen
replays pass on the result, and every command of the page was run
again on it (below).

What ran, each function with 2,000 cases, on the folder's page: the
comparison with seeds 1 and 7 (`different 0`, `discarded 0`), the
control (`different` above 0 for each: 2000 of 2000 for two, and 955,
384 and 181 of 2000 for the others), the write audit with both seeds
(`outside 0`), the record with no warning, and the replay (`failed
0`; 29 fixtures in the five files). Every instruction slot of the
original was executed for three; for the open 192 of 194 and for the
frame 172 of 179, the others named in their headers with the reason.
All of these lines were printed again on the merged source by a
private script that runs every command of the page with its status
and compares each line with the page: 18 functions (the shared header
changed for all), no line missing, no run failed, 14 minutes 47
seconds. `python -B
../programs/test_programs.py`: `all cases behaved as required`.
`python tools/ai_workflow/tests/test_workflow.py`: OK. The names that
the setups take were linted against each function's C (none missing;
a private script).

Publication: no record warned. The five new fixtures files hold 40
bytes at addresses inside the program's body, and none that is not
zero equals the program's byte there (a private check).

Limits, stated: a pass is evidence for the tested inputs of a
contract, not equivalence; no tool compares these functions' bytes
with the original's. In the frame function the count of 0x100000
polls cannot run out within the tool's budget of instructions, so the
slots of that exit are not executed, as in the logo's; the other
unexecuted slots of the frame and of the open are corrections for a
negative value that an unsigned halfword cannot give. The main's
control differs in 181 of 2000 cases only: it narrows the buttons'
mask, which shows on few pad words. The main's contract models the
flip by a recorder that stores the other value into the index word
(a model of the setup's, said in its header). The main stores 0 into
the word it later takes the newly pressed buttons against and never
writes it again, so that word stays 0 unless a callee changes it
(read from the code; what the original intended is not known).
