## The port's disc delivers at the console's rate (2026-10-10)

- What: `port/src/cd.c` delivers the sectors of a read at the rate of
  the console's drive, counted by the frame. Before, it delivered three
  sectors at every vblank, 180 a second whatever the drive's mode. Now a
  carry grows by 5 at every vblank's delivery. With the double-speed bit
  (0x80) of the mode a sector costs 2 (2, 3, 2, 3 sectors a vblank, 150
  a second); without it a sector costs 4 (1, 1, 1, 2: 75 a second). The
  carry is cleared when a read starts and when the speed bit changes. A
  sector that could not be delivered keeps its cost in the carry. The
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
  - `python3 port/tools/test_hostcd.py`: 60 lines `ok` on main, 74 now,
    last line `all cases behaved as required`, 0.5 seconds. Cases
    corrected by hand: the ones that counted three sectors a tick (the
    ticks of a read, a read after Pause, the sector under the buffer
    cases, the audio cases). Cases added: the counts per vblank in each
    speed, the totals of 60 vblanks (150 and 75), the carry cleared by a
    read's start, by ReadN after Pause, and by a change of the speed bit,
    a Setmode that leaves the bit alone, a read paused by the handler in
    the middle of a vblank's delivery, a blocked reader, and ticks with no
    read on.
  - Each rule fails under a change made to a copy of `cd.c`, run under a
    time limit: 11 changes, each failing at least one case (one-off run;
    the table is in the working notes and not in the repository).
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
