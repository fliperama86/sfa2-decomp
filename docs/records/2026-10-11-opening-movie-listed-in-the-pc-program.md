## The opening movie's program is listed: it plays in the PC program (2026-10-11)

What changed: `port/programs.toml` lists the opening movie's program
(`PAC/Z2O.EXE`, `z2o`). The PC build compiles its nineteen functions
into the PC program, and when the game calls `Exec` for it the
runtime checks the file, the header and the body against the user's
disc and runs the program's C. Two pieces of the runtime came with
it, because the program does not run without them. Nothing under
`ps1/` changed.

The entry: main is `func_800108e8_z2o`; setup gives the stream calls
the address of the program's word `data_80031e68_z2o` (read and
cleared by the program's own completion routine when a sector was put
aside), teardown takes it away; sixteen library names are bound to
routines of the runtime (the decoder's five, the stream's seven, the
three pad calls and `InitGeom`).

The two pieces, each found by starting the program in a private
trial build (not in the tree):
- A started program begins with the callback layer not started
  (`port_callbacks_program_start` in `kernel.c`, called by
  `port_program_exec` after its checks and before the row's setup).
  A stand-alone program carries its own copy of the library, so on
  the console its interrupt handling starts fresh at each start and
  its first `ResetCallback` hooks the interrupts and turns them on
  (inferred from the programs' own copies; nothing was compared with
  a console). The runtime serves a program's `ResetCallback` with the
  game's routine, which does nothing while the game's callbacks are
  running; and the game calls `Exec` with interrupts off. In the
  trial, before this piece, no completion of the decoder's output was
  delivered to the program (temporary counters at the stop: one frame
  fed to the decoder since the logo, its first output transfer pending
  for 116 milliseconds, the last completion 22 seconds before),
  sectors were put aside, and the stream stopped the program with
  `the queue of 16 sectors put aside is full`. The logo's program had run without it because the game's
  callbacks were stopped when it was started.
- `port_InitGeom` (`progcalls.c`) does nothing. Sony's routine sets
  up the console's geometry coprocessor; the PC program has none, the
  port's picture calls use none, and none of the program's functions
  with a caller holds a coprocessor instruction (a private scan
  counted 0).

What ran, on this source:
- `python port/tools/test_hostprograms.py --cc CC`: `all 71 cases
  behaved as required` (70 before): the fresh state comes once, after
  the checks and before setup, and not for a file that is not built
  in or fails a check. `python port/tools/test_hostkernel_start.py
  --cc CC` (new; it includes the real `kernel.c`): `all 1 cases
  behaved as required`: with the callbacks started and interrupts
  off, `ResetCallback` alone leaves interrupts off and the vblank
  count where it was; after the new routine the same call turns them
  on and the count starts at 0; the routine alone changes neither.
  `python port/tools/test_hostprogcalls.py --cc CC`: `all 49 cases
  behaved as required` (48 before). `python
  port/tools/test_hostbuild.py --cc CC`: `all cases behaved as
  required`. `python tools/ai_workflow/tests/test_workflow.py`: OK
  (the new control is in the review tool's list of controls that take
  the compiler).
- The PC program built with the picture library and without it:
  `linked: ..., verified` both times, with `programs built in: 2`,
  `program logo: 16 functions` and `program z2o: 19 functions`.
- One start of the build with the library, without a window, 150
  seconds, with the user's disc: no `stop:`, `refused` or `skipped:`
  line; the logo's sound line, then twice the opening movie's
  (`audio: a stream starts at sector 235526: file 1 channel 1,
  stereo, 37800 Hz`): the game starts the movie again in its attract
  loop. Video memory was dumped every 5 seconds; three of the dumps,
  rebuilt as 24-bit pictures by a private script, show frames of the
  movie.
- In the private trial of the same runtime changes, temporary
  counters at the program's return read 820 frames fed to the decoder
  and 16,391 output transfers, each of them completed and none cut
  off by a reset (20 a frame would be 16,400).
- Mutants of the new lines in bounded runs by a private script, on
  the final tests, by kind: 20; 18 failed a named case; 2 passed;
  none ended a tester without a name; none hit the time limit. The
  two that passed make `InitGeom` write the flag that `PadInit` and
  `PadStop` keep; nothing in the file reads that flag (the three pad
  calls were tried in every order), so no input can show them.

A fault of mine in these runs, said here because it reached the
owner's desk: the start above, and eight hidden starts of my private
trials this night, opened the machine's sound output. Sound is on by
default in a build with the picture library; a scripted start must
take `--no-audio` or `--audio-dump FILE`, and I had not passed either
(my brief for the start said "no `--audio`", which is not the same).

Not right yet, stated:
- In the logo's program (listed before this change) a temporary
  counter of the trial read, in two runs, 48 of 191 decoded frames
  whose last output transfers were cut off by the next frame's reset
  of the decoder. On a PC the bitstream decode returns at once, so
  the program's wait finds its flag still clear and does not wait.
  Letting the first completion through during the decode gave whole
  frames in the trial but fewer of them (135), because completions
  are delivered one per round of the timer. Not changed here; it
  needs a decision about how completions are timed.
- The logo's movie has no sound (the record of the logo's listing
  says why). The opening movie's sound was audible in these runs.
- The window's own picture was not seen: every run here had no
  screen. Nothing was compared with a console.
- The third program of the disc has no C and is skipped as before.
