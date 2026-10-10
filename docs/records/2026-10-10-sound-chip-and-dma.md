## The I/O page serves the sound chip and DMA channel 4 (2026-10-10)

- What: in a build with PsyZ, `port/src/iopage.c` serves the sound chip
  (`0x1f801c00` to `0x1f801dff`, 16 bits, through `Psyz_SpuRead` and
  `Psyz_SpuWrite` under PsyZ's audio lock) and DMA channel 4 (`0x1f8010c0`,
  `0x1f8010c4`, `0x1f8010c8`, 32 bits). A write to the control with bit 24
  set is a transfer, done at once between RAM and the chip's RAM. The
  model refuses with a stop line: an address whose top byte is not `0x00`
  or `0x80`, a span outside the 2 MB of RAM (64-bit sum), a block control
  with a zero half, a span over the chip's 512 KB, and a start while the
  last completion is pending. Without PsyZ the chip and DMA addresses stop
  as before.
- The completion of a transfer is delivered to the routine registered with
  `DMACallback(4, routine)`, on the game's thread, under the vblank's
  rules, and taken exactly once by the first poll (no delay), by the timer
  thread (not before the start plus 13 ticks of the console's clock per
  byte; the figure is not verified) or by the vblank's delivery before its
  handlers. The routine's address is checked at each call. `interrupt.c`,
  its entry, `cd.c` and PsyZ are unchanged: the timer's existing call
  `port_interrupt_take` also claims the completion, and `port_interrupt_work`
  delivers it.
- Services: `DMACallback`, `DeliverEvent`, `WaitEvent` in the kernel's
  table (`kernel.c`, `iopage.c`); the page says which calls of the
  library reach each.
- With `--trace` the counts of the chip, DMA and the completions are
  written at exit, and again every 60 vblanks when they changed, because a
  run ended by a time limit does not reach its exit.
- Changed outside the files of the page: `hostbuild.py` compiles
  `iopage.c` with PsyZ's flags (and its test has a case for it);
  `test_hostlaunch.py`'s rig takes a list of libraries for the link line;
  the review command hands `test_hostiopage.py` the PsyZ build folder
  (and `test_workflow.py` has a case). Nothing under `ps1/` and nothing of
  PsyZ changed.
- Ran (all on 2026-10-10, with the compiler of the port and PsyZ as built):
  - `test_hostiopage.py --cc CC --psyz-build DIR`: 110 lines `ok`, `all
    110 cases that ran behaved as required`, 55 seconds; four runs in a
    row gave 110 each time (55, 56, 66, 68 seconds). Without
    `--psyz-build`: 68 lines `ok`, 46 lines `NOT RUN NAME: it needs
    --psyz-build`, `46 case(s) were not run`, status 3. The file had 68
    lines `ok` before.
  - `test_hostlaunch.py` 68, `test_hostmirror.py` 103, `test_hostmodules.py`
    55, `test_hostpads.py --psyz-build` 96, `test_hostbuild.py` 407 before
    and 408 after (the one added is the case for `iopage.c`); all `all cases
    behaved as required`.
  - `tools/ai_workflow/tests/test_workflow.py`: 40 tests, `OK` (39 before).
  - 35 changes made to copies of the runtime's folder, each run under a
    time limit of 240 seconds against the cases that name its rule, each
    failing at least one case (a one-off run; the table is in the working
    notes, not in the repository), and one change each to a copy of
    `hostbuild.py` and of the review command, each failing its case. One
    rule has no case that could fail: the audio lock around the model's
    accesses. It costs nothing while no audio layer runs, so no run shows
    whether it is there.
  - Starts with the disc, one at a time, offscreen (one-off runs with a
    private input): the build with `--psyz DIR` for 25 seconds, no line
    began `refused` or `stop`, status 124; the build with `--sound-library
    --psyz DIR` for 40 seconds with `--trace`, no stop line, status 124,
    440 transfers and 896192 bytes to the chip, 440 completions all taken
    by a poll (the page has the lines).
- Judgment calls: the vblank's delivery takes a pending completion with no
  due time (the private trial required the due time there); the model
  refuses a span over the chip's 512 KB, which PsyZ's bulk routines would
  write past; the DMA registers exist only with PsyZ, like the chip;
  `WaitEvent` is not bounded.
- Did not do: any audio output, XA, the disc layer, the trial's
  histograms. The disc's pace is a known fault, stated in the page.
- After main's change of the disc's rate (pull request 179) was merged
  in: `python3 port/tools/test_hostiopage.py --cc CC --psyz-build DIR`
  and `tools/ai_workflow/tests/test_workflow.py` were run again (their
  lines are in the pull request), and each build was started once more
  with the disc, 40 seconds, not traced (one-off): no line beginning
  `refused` or `stop`, 2 lines `skipped: Exec of z2o.exe` each. The
  page's paragraph on the known fault was reworded for the rate main
  now has.
