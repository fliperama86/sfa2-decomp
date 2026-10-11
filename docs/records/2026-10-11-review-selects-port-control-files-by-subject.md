## The review selects the port's control files by what each is about (2026-10-11)

What was wrong: `workflow.py review` ran every control file of
`port/tools` (25 on this day) and the build of the PC program for any
change under `port/`, whatever the change was: one runtime file, one
override, one control file. `docs/efficiency.md` listed that among its
known costs of 2026-10-10 and says that a review runs the tests of the
piece that changed. On 2026-10-11 a change of one runtime file
(`press.c`, pull request 262) cost ten runs of the whole selection,
none of which got to its end, and the owner said again that what was
tested is not run again.

What changed: `tools/ai_workflow/port_subjects.toml` is a table of what
each control file is about, written by reading each file's own header;
`review.py` (`port_controls`) selects by it.

- A control file is about itself, about the Python files of
  `port/tools` that it imports (every import statement of the file,
  followed through the imported files), and about the paths its line
  lists. A path that ends in `/` names what is below that folder. A
  tool that a control file runs as a separate program, and a file of
  the tree that it reads, are not seen in its imports and stand in its
  line.
- A changed file selects the control files that name it.
- To the wide side: a control file without a line is selected by every
  change of the port; a changed file that no control file names selects
  all of them; with no table in the tree all of them are selected, as
  before.
- A listed path that is not in the tree, a line for a control file that
  is not there, an entry that is not a list of paths, and a control file
  that is not Python that can be read stop the review with a line.
- The build of the PC program is selected for every change of the port
  that is not a control file (before: for every change of the port).
- The table's `[build]` paths are those whose check is that build alone:
  `port/overrides/` (each override has its own checks besides, selected
  as before) and `port/sweep_rows.toml`. No control file reads the
  tree's own copy of them.

What that selects on this tree, printed by the tool's selection for each
change: `press.c` alone 1 control file; `interrupt.c` 1; `kernel.c` 3;
`cd.c` 5; an override 0; the shared rig `test_hostlaunch.py` 6;
`hostbuild.py` 10; `hostcheck.py` 12; `libgap.py` 11; `hostneeds.py` 1;
`psyz.patch` 7; `check_psyz.sh`, which nobody names, all 25.

Found by the owner's review of the first version: a change of
`hostcheck.py` selected neither `test_hostcheck` nor
`test_hostcheck_stop`, and a change of `libgap.py` not `test_libgap`.
Those control files run their tool as a separate program by its path, so
no import names it, and their lines in the table were empty; other
control files import the same tools through `hostbuild.py`, so the file
was named by someone and the wide rule did not apply. Their lines name
the tool now, and so does the line of `test_hostneeds` (it was selected
only because nobody else named `hostneeds.py`). Looking at every control
file for the same thing found one more: `test_psyzbuild.py` reads the
tree's own `port/psyz.patch`. That path stands in its line now, and in
the lines of the six control files that take a build of the library the
patch changes. A test on the tree's own table now asks, for every tool
of `port/tools`, that its change selects the control files named after
it, and for every file that a control file takes by a path built at its
top (`SCRIPT = TOOLS / "hostcheck.py"`), that its change selects that
control file; with the table of the first version it fails.

One judgment, stated in the table's header: the rig of
`test_hostlaunch.py`, which other control files run their own cases on,
is listed for every runtime file that the rig links and for `port.h`.
The control files that use the rig are listed only for the files their
own cases are about. So a change of a runtime file runs the rig's own
launch cases, where a missing stand-in or a broken link shows, and not
every control file that happens to link the file.

A second one: an override's change selected every control file before
(a test of the workflow says "the checks of a change of the build
tool"). With the table it selects the override's own checks and the
build. That test still passes, because its made-up tree has no table.

The controls. `python tools/ai_workflow/tests/test_workflow.py`: 84
tests, OK (73 before; the 11 new ones are the class `PortSubjects`, two
of them on the tree's own table). Changes made on purpose, by a private
script, each on a scratch copy of the tool, each run under a time and a
memory limit: 33 of `port_controls` and its two call sites at first, of
which 31 failed a named test at once and 2 none (a listed file taken as
the beginning of another file's name; the build left out when a control
file is among the changes); a case was written for each, and both then
failed a named test. After the review the reading of imports was
rewritten to read each file once: the 9 changes that touch it and 2 new
ones were run, and all 11 failed a named test; the other 24 were not run
again.

Not done: the review still stops at its first failed check and begins
again from the first; the port's checks are still never reused from an
earlier run; `hostbuild.py` still compiles every unit on each run. The
tuple of control files that get `--cc` is still a list of names in
`review.py`.

No control file of the port was run for this change: it changes no file
of `port/`.
