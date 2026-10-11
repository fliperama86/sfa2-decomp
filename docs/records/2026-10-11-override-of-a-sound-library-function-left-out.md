## An override of a sound-library function is left out when the library is not compiled (2026-10-11)

- Port tool only (`port/tools/hostbuild.py` and its test file, two
  sentences in `port/README.md` and `port/overrides/README.md`).
  Nothing under `ps1/` changes, and no override is added here.
- Why: the units of the game's own sound library (`.c` units under
  `ps1/src/sdk/libsnd/` and `sdk/libspu/`) are compiled into the PC
  program only with `--psyz` (and not with `--no-sound-library`).
  `hostbuild.py` refused an override whose function belongs to a unit
  that the run does not select, so the first override of a
  sound-library function would have made every build without the
  library fail. One such function is waiting for this: its exact C
  does not mean on a PC what the bytes do.
- The rule now: such an override, in a run that does not compile the
  sound library, is left out: not compiled, not linked, not counted in
  `functions overridden in C: N`, no `override:` line with `--list`.
  One line per file follows the count line, in the order of the file
  names: `override left out: NAME at PATH, its unit UNIT is of the
  sound library, which this run does not compile`. The check for
  `NAME.py` comes first, also for a file that would be left out. In a
  run that compiles the library the override is used as any other.
  Every other refusal is as before, with its message: a function of an
  assembly unit (also one under `sdk/libsnd/`), of a `.c` unit under
  another folder of `sdk/`, a nonmatching function, an unknown name. A
  run that leaves nothing out prints what it printed before.
- Inside: `select_units` records the names of the sound library's `.c`
  units when the library is not compiled (`Selection.sound_skipped`);
  `read_overrides` returns the overrides and, as a second list, the
  ones left out.
- Tests: 31 new cases in `test_hostbuild.py`, all named
  `override-sound-...`: left out without `--psyz` and with `--psyz
  --no-sound-library` (status, the count line, the exact new line, no
  object, the program linked); used with the library; one ordinary
  override beside one left out; two left out in file-name order; the
  refusals above with and without the library; a missing `NAME.py`
  with and without it; `read_overrides` called directly; and a control
  that a run without such a file still has 11 lines of output.
- What ran: `test_hostbuild.py` without `--cc`: 552 cases, `all cases
  behaved as required` (521 before the change); with `--cc`: 566 cases,
  the same last line (535 before). The PC program built with the
  changed tool, once without `--psyz` (`units: 3907 compiled, 114 of
  them nonmatching, 0 failed`, `functions overridden in C: 15`,
  `linked: ..., verified`) and once with it (`units: 3980 compiled, 119
  of them nonmatching, 0 failed`, `functions overridden in C: 15`,
  `linked: ..., verified`); neither prints a "left out" line, as no
  override of this kind exists yet.
