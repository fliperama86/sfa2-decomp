## Port build: a unit of kind asm is not C, whatever its file is named (2026-10-13)

The layout change of 2026-10-12 named every file of the resident executable
after its first function, with the ending `.c`, eight assembly units among
them (`kind = "asm"`; the entry code, six kernel calls, one of the disc's
routines). The port's tools took a unit for C by its file's ending alone, so
`hostbuild.py` handed those eight files to the C compiler, listed them as
failed and ended with status 1. The program it linked was the one of before:
the eight units' functions had no C before the rename either.

- `hostcheck.py` leaves out a unit of `kind = "asm"` as not C, whatever its
  source ends in. `hostbuild.py` takes its units from there, and applies the
  same rule to the units of the sound library. `hostneeds.py` classes such a
  unit as assembly. The contract text of each says so.
- New controls: `select-kind-asm-in-a-c-file-is-not-c` (`test_hostcheck.py`),
  `select-kind-asm-in-a-c-file-is-no-job` and `...-keeps-its-names`
  (`test_hostbuild.py`), `decl: AsmInC is assembly` (`test_hostneeds.py`).
  Each fails with the rule taken out.

What ran, on main `5475ce61` plus this change. `test_hostcheck.py` and
`test_hostbuild.py`: all cases pass. `test_hostneeds.py`: the new case
passes; four other cases fail, and fail the same way on the unchanged commit
(not looked into here). One build of the Windows program with PsyZ at its
pin: `units: 3982 compiled, 0 of them nonmatching, 0 failed`, status 0,
`functions with C: 12852`, `functions without C: 219, library 218, game and
modules 1`, linked and verified. The same commit without the change:
`3990 compiled`, `8 failed`, status 1, the same two function counts.

Not done: the eight files keep their `.c` names. Nothing under `ps1/` is
changed.
