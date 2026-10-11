## func_800e68e8_slot0f exact, plain (2026-10-11)

- `func_800e68e8_slot0f` (352 bytes, module `slot0f`; inferred: it
  fills in two adjacent sprite records for an object): unit
  `slot0f_68e8x_r1`, `baseline 352 bytes at 0x800e68e8, built 352 bytes
  at 0x800e68e8, 0 differing instruction slots: IDENTICAL`. No wrapper,
  no helper, no stand-in, no `goto`. "Lines" are instruction lines by
  the private check on the published unit.
- It is the twin of `func_80016af0_slot12` (pull request 235) with this
  module's three tables, and the same form is exact here: the list of
  halfwords is walked instead of indexed. Measured on this unit: without
  the step before the call's second value, with index 1 in the call: 26
  of 88 lines; with a local of its own for the first value: 8; the
  second texture coordinate as `list[1]` with one step of 6 below: 7;
  the plain test of the index: 4. Both places have a comment with their
  figures.
- Why the step matters is inferred from the compiler's source and not
  shown by a dump here: the compiler joins a load and its sign
  extension into one signed load unless something stands between the
  load and its use, and the step of the pointer that the load goes
  through is such a thing.
- It leaves `ps1/src/modules_nonmatching/` (three files and its six
  README lines), and its line leaves `symbols.ld`. The image has no
  second placement.
- What ran: `fndiff.py --rebuild` on the unit (the line above);
  `matchbuild.py --image slot0f`: `RESULT: PASS`; `module_protos.py`: 0
  table entries with another parameter list; the unit compiled with
  `-Wreturn-type` and `-Wuninitialized` (it is `void`, and no local is
  used without a value); the inventory sweep (no row changes) and the
  map's render on this branch, which does not hold pull request 235:
  `overall: 5458/5600 distinct functions exact (97.5%); 12888/13072
  placements (98.6%); 1714376/1809836 code bytes (94.7%)`. No whole
  build, no differential test.
