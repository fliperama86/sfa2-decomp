## func_801347b4 exact; a port override gives its unset local a value (2026-10-11)

- `func_801347b4` (1760 bytes, resident; inferred: it draws a tile map
  as sprite records): unit `d347b4_r1`, `baseline 1760 bytes at
  0x801347b4, built 1760 bytes at 0x801347b4, 0 differing instruction
  slots: IDENTICAL`. No wrapper, no helper, no stand-in. "Lines" are
  instruction lines by the private check on the published unit.
- The exact C leaves one local without a value on one path, and that is
  the original: when the map has no rows and the kind is 2 or 3, the
  function calls `func_801350c0(kind, col)` with `col` never set. The
  original passes the low byte of `s7` there (`andi a1,s7,0xff` at
  0x80134e5c), a saved register that it writes first inside the loop
  over the rows (`move s7,zero` at 0x8013483c): what the caller left in
  it. With `col` set to 0 at its declaration 58 lines differ (one
  instruction more).
- So that the PC program has a defined value there, the port gets an
  override, `port/overrides/func_801347b4.c`: the unit's C with `col`
  set to 0. It is a third kind of override ("the local without a
  value"), described on the folder's page. The readable C that stood
  for this function before had passed 0 on that path too; its contract
  (`func_801347b4.py`) moves to the override unchanged but for its
  first line, and excludes that path as before, because the original's
  value there depends on the caller.
- The override's five runs (`difftest.py`, 2000 cases each, the folder
  `port/overrides`): `built 1764 bytes, original 1760 bytes; cases
  2000, discarded 0, equal 2000, different 0`; `coverage: 436 of 440
  instruction slots of the original executed` (the four others are the
  ones the contract lists as unreachable); seed 7 the same; `control:
  different 711 of 2000`, `altered: page word constant 0x7807 changed
  to 0x7809, instruction slot 124`; `writes: cases 2000, discarded 0,
  outside 0` for both seeds.
- The path itself, run natively (`port/tools/hostreplay.py`, built by
  the port's compiler; a map header of zeros, so no rows; a recorder at
  `func_801350c0` that logs two bytes): the override calls it with (2,
  0), (3, 0) and (2, 0) for the kinds 2, 3 and 0x102, and not at all
  for the kind 0; the unit's C, built the same way, calls it with (2,
  0x57), (3, 0xc1) and (2, 0x2d), which is the value without a meaning
  that the override is for.
- The PC program built from this change (`hostbuild.py` with the
  picture library): `units: 3980 compiled, 138 of them nonmatching, 0
  failed`, `functions overridden in C: 14`, `linked ... verified`.
  Started once, hidden and without sound from the speakers, for 35
  seconds: it ran until the time limit, printed `overrides in C: 14`
  and no line that begins with `refused` or `stop`, and its sound file
  held sound.
- What makes the unit exact, measured: the parameters keep the `int`s
  of the shared prototype and are narrowed in locals (the list index to
  16 bits and the page base to 8: without these two copies 90 of 440
  lines differ); the first read of a cell is written as a sum of
  bytes (the plain index differs in 1 line, the operand order of one
  addition); the 32 corner stores address their record as
  `(&table[i].polys[n])->field` (plain: 212 lines); three tests of the
  kind stay in byte forms (written plainly: 21, 118 and 2 lines).
- The function leaves `ps1/src/resident_nonmatching/` (its C and its
  five README lines; its name leaves the list of functions without a
  fixtures file), and its line leaves `symbols.ld`.
- What ran besides: `fndiff.py --rebuild` on the unit; `matchbuild.py
  --image resident`: `RESULT: PASS`; the unit compiled with
  `-Wreturn-type` (it is `void`); the inventory sweep (one row gets its
  name) and the map's render: `overall: 5456/5600 distinct functions
  exact (97.4%); 12886/13072 placements (98.6%); 1713400/1809836 code
  bytes (94.7%)`. No whole PS1 build, no other function tested again.
