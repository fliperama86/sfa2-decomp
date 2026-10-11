## The logo's program is listed: its movie plays in the PC program (2026-10-11)

What changed: `port/programs.toml` lists the logo's program
(`PAC/LOGO.EXE`). The PC build compiles its sixteen functions from
`ps1/src/programs/logo/` into the PC program, and when the game calls
`Exec` for it the runtime checks the file, the header and the body
against the user's disc and runs the program's C. No original code of
the program runs; nothing under `ps1/` changed.

The entry: main is `func_800106fc_logo`. Setup gives the stream calls
the address of the program's word `data_8002edb8_logo`, which the
program's own C reads and clears when a sector was put aside while
the decoder was busy; teardown takes the address away. Eighteen
library names are bound to routines of the runtime: the movie
decoder's five (`press.c`), the stream's seven (`stream.c`), the
plain read's two, the three simple pad calls and `SetVideoMode`
(`progcalls.c`). The program's other library names are names of the
game: a call goes to the game's own address for the name, where the
runtime's routine stands or its stop names the call.

What ran, on this change's source:
- The PC program built with the picture library and without it:
  `linked: ..., verified` both times, with `programs built in: 1` and
  `program logo: 16 functions`.
- One start of the build with the library, without a window, for 60
  seconds, with the user's disc: no stop line; the disc layer's line
  for the movie's sound (`audio: a stream starts at sector 179376:
  file 1 channel 1, stereo, 37800 Hz`); the game then went on through
  its own start and three rounds of its attract loop, each with the
  `skipped:` line of the opening movie's program, which has no C; the
  run was ended by the time limit. Video memory was dumped every 3
  seconds; the dump at 3 seconds, rebuilt as a 24-bit picture by a
  private script, shows the logo's letters forming.
- One start of the build without the library, for 20 seconds: it
  stops as it did before, at the game's first graphics call
  (`library function ResetGraph ... has no host routine yet`), before
  any program is started.
- In a private trial of the same sources before this change, the
  program's run was looked at more closely: dumps once a second held
  movie frames for about 9 seconds, which fits what the program's C
  says (it ends at frame 280 and waits two vblanks a frame).

Not right yet, stated:
- The movie has no sound. The disc layer finds and decodes its audio,
  but the sound chip's model mixes disc audio only with its control
  register's bit 0 set and scales it by its CD volume and main
  volume, and all three are 0 when the PC program starts. Before the
  logo the game makes no sound call, and the logo's program only
  fades the drive's own mix. So on the console that state comes from
  the boot code (inferred; no source for the values was found). An
  audio dump of the trial was silent for its first 17 seconds. The
  sound model belongs to the sound work; it is told.
- The window's own picture was not seen: every run here had no
  screen. What was looked at is video memory.
- Nothing was compared with a console.
- The opening movie's program and the third program of the disc have
  no C and are skipped as before.

Found by the tool's own test once a program was listed: two of its
invocations started the build on a made-up tree without naming a
list (one case with relative paths, and the rig of the cases that use
the real compiler), so they read the tree's list against the made-up
tree's declaration and stopped. Both name their own empty list now,
as the test's other invocations did, and the tool's header says that
a build of another tree names its list. `python
port/tools/test_hostbuild.py --cc CC`: `all cases behaved as
required`, 535 `ok` lines.

Also here, two wordings that the review of pull request 204 named:
the page said that the tick "returns at once but for the last
millisecond", the reverse of what it does (it sleeps while the next
vblank is more than a millisecond away and returns at once inside
that millisecond); and a comment in the test of the plain read
described a time-out where the case checks a read that completes.
And two sentences of the page that said no program is listed, or
that nothing reads the programs' table.
