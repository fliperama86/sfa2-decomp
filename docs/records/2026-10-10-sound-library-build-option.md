## The PC build can compile the game's own sound library (2026-10-10)

- What: `port/tools/hostbuild.py` has an option, `--sound-library`, which
  needs `--psyz`. It compiles the units of `ps1/src/sdk/libsnd` and
  `ps1/src/sdk/libspu` and the folder `ps1/src/library_nonmatching` into the
  PC program, with `port/sdkshim/` and PsyZ's include folder on the include
  path of those units only. It adds the line `sound library: N units of
  sdk/libsnd, M of sdk/libspu, K of library_nonmatching` after the `psyz:`
  line. Every runtime file gets `-DPORT_SOUND_LIBRARY_C`, and with it
  `port/src/sound.c` has an empty table. A file `port/prefix/UNIT.h` is
  given to the compiler before the source of UNIT. Without the option the
  tool does what it did.
- Why: the game's functions that are library code read the library's
  state in RAM, and PsyZ's sound library as built has no sequencer. The
  program runs the library's own C instead. The page `port/README.md`
  ("The game's own sound library") has the two facts with their places in
  the source, read at PsyZ's pin. This change is only the build's part.
- No path of `ps1/src/sdk/include` reaches the compiler. The shim is
  four headers written by this project. The one unit whose code differs
  under PsyZ's declaration, `libspu/s_sca.c`, has a control,
  `port/tools/test_sdkshim.py`.
- Ran:
  - `python3 port/tools/test_hostbuild.py`: 398 lines `ok` (360 on main),
    `all cases behaved as required`; with `--cc`, 407 lines `ok` (369 on
    main), the same last line.
  - Each new case of `test_hostbuild.py` fails under at least one change
    made to a copy of the tool (25 changes in all, each run under a time
    limit in a copy of the tree; a one-off run, the table is in the
    working notes and not in the repository).
  - `python3 port/tools/test_sdkshim.py --cc CC --psyz-build DIR`:
    `calls 150000, different 0` and, for the negative control,
    `calls 150000, different 5186`, `all cases behaved as required`.
  - `tools/ai_workflow/tests/test_workflow.py`: 39 tests, `OK` (38 before;
    the added one hands `test_sdkshim.py` its arguments).
  - The build without the option printed the lines of main's build
    (3907 units, 12686 functions with C, `linked: ..., verified`). The
    build with `--sound-library --psyz DIR` printed `sound library: 45
    units of sdk/libsnd, 22 of sdk/libspu, 6 of library_nonmatching`,
    `units: 3980 compiled, 161 of them nonmatching, 0 failed` and
    `linked: ..., verified`.
  - Two starts of built programs with the disc, 20 seconds each, offscreen,
    one at a time (one-off runs with a private input; no command of the
    repository repeats them). Without the option: no line that begins
    `refused` or `stop`, ended by the time limit (status 124). With the
    option: `stop: library function DMACallback (0x8015f020) has no host
    routine yet`, status 4.
- Did not run: the sound chip is not served and no system service for the
  library is written, so nothing sounds and no library function was
  checked against its behaviour. The review command was not run.
- Limits: the comparison that found 66 of the 67 units identical to the
  build with the matching work's headers was made once, privately, and is
  not repeatable here. The counts in the `sound library:` line count units
  and files selected for compiling.
- Later: serving the chip's registers, and the system services such as
  `DMACallback` that the library calls.
