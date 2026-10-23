## Experimental settings on by default (2026-10-23)

The owner, after the Linux program ran: "make is all on by default", then, asked
whether for their runs only or for the project's defaults: "Project defaults".
The option chosen said the arcade intro stays off, since a player without an
intro pack would be refused at start.

- `settings.c`: the defaults are on for `pixel_perfect_ui`, `arcade_video`,
  `center_ui`, `fast_loads`, `memory_card` and `sharp_bilinear`; `arcade_intro`
  stays off. The four graphics settings need PsyZ's patched backend, and a build
  without PsyZ refuses them at start; such a build (the controls) has no window,
  so its graphics defaults stay off. `hostbuild.py` and `macbuild.py` compile
  `settings.c` with `PORT_HAVE_PSYZ` when they link PsyZ.
- `port/settings.ini`: the same values. `port/graphics.md` and `port/README.md`
  say so; the project memory has the decision.
- Controls: `settings_test.c` takes the defaults as a macro and
  `test_settings.py` runs it as both kinds of build, and checks the start-up
  parser of a build with PsyZ. The case of `test_hostlaunch.py` about empty
  card slots asks for `--no-memory-card` now. Its crash case reads the last
  page on Linux (`0xfffff000`): the 32-bit program's stack lies at a random
  place near the top of the address space and covered `0xffff1000` in one run
  of 20.

What ran, on Linux: `test_settings.py`, `test_hostbuild.py`, `test_macbuild.py`
(5 cases that run off a Mac), `test_hostmemcard.py`, `test_hostlaunch.py
--linux` (all but the stale card-routines case), and the Linux program's
`--show-settings` with no settings file. Not run: the Windows and Mac programs.
