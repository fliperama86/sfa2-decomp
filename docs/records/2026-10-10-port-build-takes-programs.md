## The PC build takes the C of a stand-alone program (2026-10-10)

What was added: `port/tools/hostbuild.py` can build the C of a
stand-alone program of the disc (a folder `ps1/src/programs/NAME/`,
declared in `ps1/src/programs/programs.toml`) into the PC program, for
the programs that a new list of the port names, `port/programs.toml`,
and it writes a table of those programs, `port_programs[]`, for the
runtime. The list in the tree names no program: the logo is listed when
all its functions have C and every library routine it calls exists.
Nothing of the runtime reads the table yet; starting a program from the
game's `Exec` is the next change. Nothing under `ps1/` changed.

The design (the top level's; the reasons are inferred from the code,
not from a run of a program):
- A program's own functions are not placed at their PS1 addresses and
  get no jump there. Each is the alias `ps1_func_ADDR_NAME =
  impl_func_ADDR_NAME;`, so a program's C calls its own C directly.
  `Exec` is the only way into a program, and a study of the logo found
  no pointer to one of its own functions in its data. This also needs
  no inventory of a program's functions, which would be a second
  published table of the analysis.
- The names a program adds are of four kinds: a function (the alias
  above); a symbol of the declaration that begins `data_` (its
  address: the game loads the program's data there from the user's
  disc); a symbol that the game's names already hold (skipped: the
  game's own address stands for it, where the runtime's routine is);
  any other symbol, which the port's list must bind to a routine of
  the runtime (`ps1_NAME = ROUTINE;`), else the build stops naming it.
- After the renaming `nm` is run on every object of a program, and an
  undefined name that is not a `ps1_` name ends the build naming the
  object and the name. Without that a plain name would link against
  whatever has that name; the picture library now has routines under
  Sony's names.
- The list gives each program its main function and, optionally,
  `setup` and `teardown` calls: routines of the runtime called before
  main and after it with the address of one of the program's symbols,
  or 0. The build generates the two routines; the table's row holds
  the program's name, the base name of its file on the disc, the
  file's SHA-256 from the declaration, main's C and the two routines.

What ran: `python port/tools/test_hostbuild.py --cc CC`, `all cases
behaved as required`, 518 `ok` lines (408 before the change), about 40
seconds; its new cases use made-up trees, a made-up declaration and a
made-up list, and one of them builds a made-up program with the real
compiler and runs its setup, main, a bound routine and teardown. The
PC program of the tree, built with the picture library and without it:
`linked: ..., verified` both times, with the lines it printed before
plus `programs built in: 0`. `python
tools/ai_workflow/tests/test_workflow.py`: OK.

One trial that is not in the tree: with a list that named the logo (a
stand-in for its main, which had no C yet, and three library names
bound to invented routines), the logo's ten functions of that day
compiled, their names resolved and no plain undefined name was left;
the build then ended at the link, naming the first invented routine.
With a scratch copy of the runtime that defined the three routines it
linked and verified. Nothing was started.

Mutants of the new code in bounded runs by a private script (a time
limit and a memory limit per run), without `--cc`, so the cases that
need the real compiler did not run under them: 87 on the test file as
it was before its last case; 86 failed a named case; 1 passed (a
two-field test of an `nm` line that a defined symbol's three-field
line never meets: it changes nothing an input can show); none ended
the tester without a name; none hit the time limit; a change of
nothing passed. For the refusal added last (below), 2 more on the
final test file: one failed the new case by name, the other ended the
tester's case list with the tester's own line.

Found in the top level's reading and corrected before publishing: the
tool's header said that a program's `data_` name which the game holds
at another address is the error of two addresses, and nothing refused
it (the program's address would have replaced the game's in the names
file without a line). It is refused now, with a case.

Flagged, as the worker reported them:
- The table's struct is `struct port_builtin_program`: the runtime
  has a `struct port_program` for the main executable already.
- A program's units are kept apart from the game's: they are not
  counted in the `units:` line, a unit that does not compile ends the
  build naming it, and a unit may define only its function.
- The generated file declares each routine that `setup` or `teardown`
  calls as `void ROUTINE(unsigned)`. The runtime's routines for that
  use must have this form.
- The tool's line for a failed link now also shows the linker's
  "undefined symbol ... referenced in expression", which is what a
  bound routine that the runtime lacks produces.
- A list file that does not exist lists nothing, as the tool's table
  of sweep rows does.

Limits: no program is listed and nothing reads the table, so nothing
here shows a program running. The old sample of the build's output in
the page's first part does not have the new line; the new part names
it.
