## func_801e1d6c_slot0b exact, plain (2026-10-11)

- `func_801e1d6c_slot0b` (624 bytes, module `slot0b`; inferred: it
  draws a tile map as sprite records): unit `slot0b_1d6cx_r1`,
  `baseline 624 bytes at 0x801e1d6c, built 624 bytes at 0x801e1d6c, 0
  differing instruction slots: IDENTICAL`. No wrapper, no helper, no
  stand-in, no `goto`. "Lines" are instruction lines by the private
  check on the published unit.
- What made it exact against the private candidate that the search
  started from (not the published readable C): the cell's low byte is
  split as `rem = cell % 256; v = rem % 16; u = rem / 16;` with the
  remainder in a 16-bit local and `v` computed before `u` (the other
  order: 44 of 156 lines; the remainder as `int`: 12); the list link is
  stored through `PrimTag`, as the published readable C already had it
  (the private candidate had the masks written out); the row's y is an `int` of the row loop and the tile's y a
  16-bit local of the column loop (as `int`: 30).
- Two spellings that the bytes need, each commented: the cell is read
  as a sum of bytes (the plain index differs in 1 line, the operand
  order of one addition, as in `func_801347b4`), and `pos_y` is read
  through a cast to unsigned (the plain signed field: 1 line).
- It leaves `ps1/src/modules_nonmatching/` (three files and its six
  README lines), and its line leaves `symbols.ld`. The image has no
  second placement.
- What ran: `fndiff.py --rebuild` on the unit (the line above);
  `matchbuild.py --image slot0b`: `RESULT: PASS`; `module_protos.py`: 0
  table entries with another parameter list; the unit compiled with
  `-Wreturn-type` and `-Wuninitialized` (it is `void`, and no local is
  used without a value); the inventory sweep (no row changes) and the
  map's render: `overall: 5457/5600 distinct functions exact (97.4%);
  12887/13072 placements (98.6%); 1714024/1809836 code bytes (94.7%)`.
  No whole build, no differential test.
