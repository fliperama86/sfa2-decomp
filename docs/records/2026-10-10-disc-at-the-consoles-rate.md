## The port's disc delivers at the console's rate (2026-10-10)

- What: `port/src/cd.c` delivers the sectors of a read at the rate of
  the console's drive, counted by the frame. Before, it delivered three
  sectors at every vblank, 180 a second whatever the drive's mode. Now a
  carry grows by 5 at every vblank's delivery. With the double-speed bit
  (0x80) of the mode a sector costs 2 (2, 3, 2, 3 sectors a vblank, 150
  a second); without it a sector costs 4 (1, 1, 1, 2: 75 a second). The
  carry is cleared when a read starts and when the speed bit changes. When
  a vblank cannot hand over a sector (the reader is not taking, the read
  stopped, the handler paused it), the carry is cut to its remainder
  (carry modulo the cost): what the drive could not hand over is not
  handed over later, so no vblank brings more than 3 sectors at double
  speed or 2 at single speed. The
  delivery is still in `port_cd_tick`, in a row. `PORT_CD_SECTORS_PER_TICK`
  is gone and `cd.h` states the rule. The blocking wait of the game
  (`CdReady` with mode 0) is as it was: it calls `port_cd_tick` when no
  sector is ready, so a wait can bring more sectors in a second than
  the rate gives (the game waiting on the drive, not the drive running
  ahead).
- Why: the game reads a stream into a ring of seven places that its main
  loop empties once per pass, and runs it near its limit. A disc a fifth
  too fast leaves it less room. This was measured in a private trial of
  the port's sound, which is not published.
- The limit: this is the console's rate and nothing more. When a pass of
  the game's loop takes several frames, the ring can still be lapped,
  and for a sound bank the game's library then never recovers. A later
  change.
- Ran:
  - `python3 port/tools/test_hostcd.py`: 60 lines `ok` on main, 88 now,
    last line `all cases behaved as required`, 0.5 seconds. Cases
    corrected by hand: the ones that counted three sectors a tick (the
    ticks of a read, a read after Pause, the sector under the buffer
    cases, the audio cases). Cases added: the counts per vblank in each
    speed, the totals of 60 vblanks (150 and 75), the carry cleared by a
    read's start, by ReadN after Pause, and by a change of the speed bit,
    a Setmode that leaves the bit alone, a read paused by the handler in
    the middle of a vblank's delivery, a blocked reader at both speeds (after 3 and 4 blocked vblanks, and the bound after 50), and ticks with no
    read on.
  - The cost of a sector is taken from the carry before the game's ready
    handler runs, and a read start or a speed change made by the handler
    ends the row. Found in review: with the cost taken after the call, a
    handler that issued ReadN, ReadS or a Setmode that changes the speed
    bit made the unsigned carry wrap and delivered the rest of a
    32-sector image in one tick (with the old code the controls printed
    counts of 13 and 32 sectors in the first tick, and a handler that
    issued ReadN at every sector never ended). Nine cases now cover the
    handler's actions.
  - Each rule fails under a change made to a copy of `cd.c`, run under a
    time limit: 12 changes (one per rule, among them the remainder kept
    whole, the carry cleared to 0 in its place, and the cost taken after
    the handler's call), each failing at least one case (one-off run;
    the table is in the working notes and not in the repository). A
    counter that let the row see a handler's reset was tried and taken
    out again: with the cost taken first a reset leaves the carry at 0
    and the row ends by itself, and no case could tell the two apart.
  - `python3 port/tools/test_hostmodules.py --cc CC`: 55 lines `ok`,
    `all cases behaved as required`; `python3 port/tools/test_hostlaunch.py
    --cc CC`: 68 lines `ok`, the same last line. Their expected values
    did not change; both use the blocking wait.
  - The default build with `--psyz DIR` of this tree and of main printed
    the same lines (3907 units, 12686 functions with C, `linked: ...,
    verified`).
  - Two starts with the disc, 60 seconds each, offscreen, no input, one
    at a time (one-off runs; no command of the repository repeats
    them): the build of this tree and the build of main each printed 4
    lines `skipped: Exec of z2o.exe`, no line that begins `refused` or
    `stop`, and were ended by the time limit (status 124). In this
    comparison the slower disc cost no cycle of the game's attract
    sequence.
- Did not run: no check plays the game; the comparison of the two starts
  is one start each, not a measurement of load times.
- Later: the ring lapped by a long pass of the game's loop.
