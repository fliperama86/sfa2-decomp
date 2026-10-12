## Fast loads, a default-off setting (2026-10-12)

The owner asked to take the loading waits out of the port. The first try, letting the disc layer
deliver up to 256 sectors a vblank, broke the game: its sound-bank loader handles one sector per
frame and must see the main loop between sectors, so a long row ran past the end of the data and
the game restarted the same load for ever. The disc was never the bottleneck; the frame was. That
try is dropped.

New setting `fast_loads` under `[experimental]`, default off (`--fast-loads`, `--ps1-loading`,
`--show-settings`). On, the frame stops being the clock of a load wait, and everything the game sees
stays counted by frames:
- Two host overrides (`port/src/loadwait.c`), installed only while the setting is on, replace the
  game's two load-waiting routines. Each starts the load as the game's C does, then runs the game's
  own load step and the disc's vblank step (`port_cd_tick`) in the frame's order, with no wait for the
  host's clock, until the load reports done. Every 256th pass one real clock step runs, so events and
  audio go on. Not run during the wait: the game's vblank handlers and the main thread's frame
  (drawing, pads, voice threads).
- In the disc layer, a blocking wait of the game (CdReady or CdSync with mode 0, which the game's file
  readers use sector by sector) steps the host's clock at every 256th poll only; the sectors and
  responses are the same. A poll in mode 1 keeps its step.

Overrides gain an optional `enabled` pointer to a setting (library.c installs a row only while it is not 0),
so with the setting off nothing is installed and the game's C runs as before.

Evidence: `port/tools/test_hostloadwait.py` (the loop with made-up hooks, the table, the gate),
`port/tools/test_hostcd.py` (new `fast_wait_cases`; a mutant that steps at every poll fails three of
them), `port/tools/test_settings.py`. Measured once on the Mac build with the user's disc: the first
module after the logo at 13.0 s instead of 16.4 s (the rest is the logo movies). Not measured: fights,
character select, the other loads. Not run: `test_hostlaunch.py`, `test_hostmodules.py` and
`test_hostprograms.py` (they need a Windows cross compiler). Unknown: whether skipping the main
thread's frames and the vblank handlers during a load changes anything the game later depends on.
