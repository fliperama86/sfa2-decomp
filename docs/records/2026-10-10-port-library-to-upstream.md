## The port's PsyZ moves to upstream's head of 2026-10-10 (2026-10-10)

What changed: the submodule `port/external/psyz` is pinned to PsyZ
commit `6d01339aadf16c5c9d6e44f44c0d66514752e472` (2026-10-10) in place
of `4e4b3e8dc7ae740c085fd190d635f54142d2d552` (2026-10-05). The port's
patch (`port/psyz.patch`) names the new commit and its hunk's place
(line 165, four lines further down; the three lines it expects are
unchanged). The pages name the new commit and the tool's output for
it. No line of the port's code or tools had to change.

Why: the owner's word of 2026-10-10 on the logo and the attract movie:
borrow from PsyZ and its sister project, do not write our own from
scratch. Between the two commits (15 of upstream's) PsyZ got a core of
the movie decoder chip with tests against captures from a console, the
24-bit display mode in both of its picture backends, a notification
for the end of a DMA transfer in its kernel, and decompiled C of the
movie routines of Sony's library. This change only moves the pin; the
port uses none of that yet. The port's own decoder core
(`port/src/mdec.c`, added earlier the same day) stays for now; what
becomes of it is a later change.

What of the 15 commits touches what the port builds on: `libgpu.c`
(the queue's flush on 32-bit targets, see below, and a mask of the
display area), `kernel.c` (`DMACallback` and the DMA notification),
`libetc.c`, `libcd.c` and `libsnd.c` (stand-ins removed where
decompiled C took over), `libpress.c`, `libcd.h` (a type in the
stream's header). The port defines none of the names that were added
or removed, and the PC program linked without a change.

The patch: upstream's change "gpu: draw, not drop, 32-bit targets'
pending primitives on exeque" makes the library send its batch to the
picture when the queue is executed, on 32-bit targets too. With it the
port's picture test passes without the port's patch as well (shown
once, with a second library build whose patch changed a comment only).
So that test no longer shows the patch to be needed. The patch is kept
in this change all the same: with it the batch is sent after every
drawing list, as before, and no run of the game without it was made.
Whether it can go is a question for a later change.

What ran, all on 2026-10-10 by a private script with the review tool's
options (`--cc`, `--psyz-build`), before (main with the library of the
old pin) and after (this branch with a new library build):
- `python3 port/tools/psyzbuild.py`: `psyz: 6d01339aadf16c5c9d6e44f44c0d66514752e472`,
  `patch: psyz.patch applied at 1 place`; the first build took about
  35 seconds (one-off timing).
- All 20 files `port/tools/test_*.py` (among them the picture test, the
  sound chip's page, the audio test, the pads, the modules, the native
  replay of the fixtures): each ended with status 0 before and after,
  with the same count of cases and the same outcome in its last line;
  the two files whose last line gives the time they took differ in the
  seconds.
- `python3 port/tools/hostbuild.py --sound-library`: `units: 3980
  compiled, 161 of them nonmatching, 0 failed`, linked and verified.
- Three runs of the PC program without a window, the same input
  script, 150 seconds each, ended by the time limit: the old program
  twice and the new one once, with 29 picture dumps and the sound
  written to a file. All three reached the same place (the same last
  line of the log). The run is not the same every time: the old
  program's two runs differ from each other in 26 of 29 pictures and
  in the length of the sound file. The new program's run differs from
  each old run by about as much as they differ from each other, and is
  nearer to one of them (12 pictures identical) than they are to each
  other (3).

Limits, stated: the three runs show no difference between the old and
the new library beyond what two runs of the same program already
differ by; they show nothing finer. PsyZ's own test suite was not run
again at the new commit: the page's table of that suite is marked as
the previous pin's. Nothing was heard or seen by a person.

Not done: any use of the new decoder core, display mode, notification
or movie routines; the removal of the port's patch or of its own
decoder core.
