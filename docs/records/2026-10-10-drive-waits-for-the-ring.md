## The port's drive waits for the game's ring (2026-10-10)

- What: `port/src/cd.c` does not deliver a sector of a streamed load of
  the game while the game's ring of seven sectors holds four or more that
  the game has not read. It applies when a read is on, the mode's XA bit
  is clear and the ready handler in the library's variable is the game's
  stream handler, `func_8014f918` at `0x8014f918`. The fill is
  `(w + 7 - r) % 7`, with `w` and `r` bytes `0x16` and `0x17` of the
  game's stream object `ctl_80190948` at `0x80190948`, both 1 to 7
  (otherwise no guard). Before each sector of a vblank's row with a fill
  of 4 or more, the sector waits and the row ends, the carry cut to its
  remainder. After 30 vblanks in a row held, the next vblank delivers its
  row regardless, with one line on the output and a count; the count
  begins again. `CdReady` with mode 0 is not guarded and neither counts
  as a held vblank nor ends a run. The counts go to the last line of a
  trace, and a trace gets a line each time a run of held vblanks ends.
- Why: the known fault of the port's pages. When a pass of the game's
  main loop takes several frames, the ring is lapped; if the lost sectors
  were a sound bank's, the game's sound library never recovers and the
  game restarts its load for ever. The owner decided on 2026-10-10 to
  publish the guard as a listed, game-specific exception: asked "Shall I
  publish the guard?" after the two ways were put to him, he answered
  "yes". It is listed in `port/README.md`; "Not decided" gains the port's
  clock following the game's progress, the general answer, which is not
  designed.
- What the game's source shows of the two bytes: byte `0x16` is where the
  stream's work writes a sector (`func_80150100`, at 0x801e0000 plus the
  place times 2048) and is stepped from 1 to 7 and back to 1; byte `0x17`
  is the place the main loop's `func_80150294` reads next, stepped the
  same way; both are set to 0 when a stream begins (`func_8014f690`,
  `func_8014f76c`). That they make a ring whose fill is `(w + 7 - r) % 7`
  is inferred, from that source and from the trial's traces. The handler
  is stored in the library's variable by `func_8014f690` (`v10_r1.c`)
  and the other stores of this variable in the game's C (`y14e890_r2.c`, `s14f9f0_r3.c`) store 0.
- Ran (all on 2026-10-10):
  - `python3 port/tools/test_hostcd.py`: 129 lines `ok` on main, 162 now,
    last line `all cases behaved as required`, 1.4 seconds before. Cases
    added: the fill of all 49 pairs as a table written out by hand; places
    outside 1 to 7; held at fills 4, 5 and 6 at both speeds with the
    carry as the rule gives it; a fill that reaches 4 within a row; the
    30 held vblanks, the forced row, its line and the count beginning
    again; a vblank not held beginning the count again; another handler,
    no handler, the XA bit; `CdReady(0)` and a run it does not touch; the
    trace lines. The seam of the rig: the variable can hold the stream
    handler's value, and the made-up handler steps the writer's place.
  - Negative controls: 26 changes to a copy of `cd.c`, one or more per
    rule, each run under `timeout 120`; each failed at least one case (one-off;
    the table is in the working notes, not in the repository).
  - `test_hostlaunch.py --cc CC`: 68 `ok`, `all cases behaved as required`.
    `test_hostaudio.py --cc CC --psyz-build DIR`: 20 `ok`, `all 20 cases
    that ran behaved as required`. `test_hostxa.py`: 54 `ok`.
  - With the user's disc (one-off, private, offscreen, `--audio-dump`):
    the stressed run (110 seconds, full `--trace`, four busy processes)
    twice with main's build and three times with this branch's. Main's
    first run went silent from the 60th second to the end; its second did
    not. This branch's three did not hang and reached the loads of
    sectors 0x37dff and 0x1ad1f. No run showed the guard holding a
    vblank. The light run (60 seconds) gave 4 attract cycles on each
    build. The page has the figures.
- Limits: the guard is knowledge of this game's memory in the disc layer.
  That the two bytes are what the source and the traces say is not
  tested against the real game by any command of the repository. The
  runs with the disc show that the build did not hang, not that the
  guard does anything in a run.
