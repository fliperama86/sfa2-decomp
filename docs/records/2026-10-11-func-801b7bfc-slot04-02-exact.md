## func_801b7bfc_slot04_02 exact, plain (2026-10-11)

- `func_801b7bfc_slot04_02` (348 bytes, modules `slot04_02` and its
  second placement `slot05_02`; inferred: a state of a helper object
  that follows another one): unit `slot04_02_7bfcx_r1`, `baseline 348
  bytes at 0x801b7bfc, built 348 bytes at 0x801b7bfc, 0 differing
  instruction slots: IDENTICAL`. No wrapper, no helper, no stand-in, no
  `goto`. "Lines" are instruction lines by the private check on the
  published unit.
- What made it exact against the published readable C, each measured on
  the published unit: `obj->field_09 = 6;` is written twice, around the
  copy of `field_1c`, because the original stores the value to that
  byte twice (the readable version's header already said so; with one
  store: 3 of 87 lines); the other object's `field_5c` is compared
  through a cast to a signed 16-bit value (without it: 1 line; the
  original loads it signed); and the statement that sets `pos_y` stands
  directly after the one that sets `pos_x` (where the readable version
  had it, after `field_a0`: 13 lines). The first two have a comment
  with their figure.
- The second store is the original's own behaviour, two store
  instructions to the same byte, and not an assignment made up for the
  match.
- It leaves `ps1/src/modules_nonmatching/` (two files, its five README
  lines and its name in the README's list of functions without a
  fixtures file). `symbols.ld` had no line for it.
- What ran: `fndiff.py --rebuild` on the unit (the line above);
  `matchbuild.py --image slot04_02` and `--image slot05_02`: `RESULT:
  PASS` each; `module_protos.py`: 0 table entries with another
  parameter list; the unit compiled with `-Wreturn-type` and
  `-Wuninitialized` (it is `void`, and no local is used without a
  value); the inventory sweep (no row changes) and the map's render on
  this branch, which holds pull request 235 and neither 236 nor 237:
  `overall: 5459/5600 distinct functions exact (97.5%); 12890/13072
  placements (98.6%); 1715072/1809836 code bytes (94.8%)`. No whole
  build, no differential test.
