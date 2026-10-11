## The runtime starts a built-in program at the game's Exec (2026-10-10)

What was added: `port/src/programs.c` with `programs.h`. When the game
calls `Exec` for a program of the disc that is built in (a row of the
generated table `port_programs[]`), the runtime verifies the file and
the loaded image and runs the program's C: its setup routine, its
main, its teardown routine; then `Exec` returns 1. A program that is
not built in is handled as before: the stop with status 13, or with
`--skip-programs` a line and the result 1. No program is listed in
the tree, so the PC program of the tree behaves as before.

How it works:
- `port_h_Exec` finds the file the game loaded as before (from the
  disc sector of the header's first word) and offers it to
  `port_program_exec`. The row is found by the file's base name,
  without regard to case.
- Three checks come before any C of the program runs; one that fails
  ends the program with a line (status 13): the SHA-256 of the whole
  file on the disc against the row's (the C was written from one
  file; the user's disc may hold another); the header's four words at
  `Exec`'s argument against the file's; and the body, `t_size` bytes
  from file offset 0x800, against the RAM at `t_addr`, where the
  program's C reads its data.
- Then the row's setup, main as `void (*)(void)`, the row's teardown,
  with a trace line before and after. Nothing else is cleaned up
  after main: the game redoes its callbacks, graphics, disc mode and
  pads itself after `Exec`.
- A built-in program runs whether or not `--skip-programs` is given;
  the option now skips the programs that have no C. Its texts say so.
- The build writes `port_programs[]` always, empty when nothing is
  listed, because the runtime reads it; the two test rigs that write
  a tables file of their own got an empty table.
- The build refuses two listed programs whose files have one base
  name: the runtime finds a program by that name.

What ran: `python port/tools/test_hostprograms.py --cc CC`, `all 70
cases behaved as required`, about 4 seconds: a driver with
`overrides.c`, `programs.c`, the disc reader and the hash, a made-up
table of 17 rows and a made-up disc image of made-up program files,
everything through `port_h_Exec` (the order of setup, main and
teardown; the right one of two programs; a row without setup and
teardown; the name's forms; the option; a file not in the table; one
byte of the file changed in the header's sector, in the body and at
the end; each of the header's four words changed in RAM; the body's
first and last byte changed in RAM; a `t_size` beyond the file; a
body that leaves the RAM; a header outside the RAM). `python
port/tools/test_hostbuild.py --cc CC`, `all cases behaved as
required`, 529 `ok` lines (527 before). The control files of the two
rigs that changed and of the three that use the first's rig passed
(`test_hostlaunch.py`, `test_hostmodules.py`, `test_hostmirror.py`,
`test_hostpads.py`, `test_hostiopage.py`). The PC program built with
the picture library and without it (`linked: ..., verified`,
`programs built in: 0`), and one start without a window for 60
seconds printed the lines it printed before, the five `skipped:`
lines included, and was ended by the time limit.

Mutants of `programs.c` and of `Exec` in bounded runs by a private
script, on the final test file, by kind: 87; 86 failed a named case;
1 passed; none ended the tester without a name; none hit the time
limit. The one that passed takes the last row of a name where the
code takes the first. No table the build writes can tell them apart:
the build refuses a list that would give two rows one file name (a
case of this change), and the table's header states it.

With the user's disc, once, in a private trial build that is not in
the tree (the logo listed): `Exec` found the logo's row, the three
checks passed, and the program's C ran to its end and returned to
the game.

Flagged, as the worker reported them: with `t_size` 0 no body is
needed in the file (a file with no body is not refused for being
shorter than 0x800); the test's driver stands in for the routine
that says which disc sector a word of memory came from, so that
record is not exercised here (the disc layer's own test does that);
the name's forms with a folder, a suffix and mixed case are tested
through the listing and by direct calls of `port_program_exec`,
because the listing itself gives lower-case names without them.

Limits: no program is listed, so nothing of the tree starts one.
What a program may leave behind (a handler, a callback) is put back
only by its row's teardown routine; none is written yet.
