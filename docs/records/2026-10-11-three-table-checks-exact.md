## Three sibling functions exact: func_8013eb28, func_8013ec50, func_8013ed90 (2026-10-11)

The three check an object's flag halfword against an entry of the same
table and call one function for a pass and one for a failure. No
wrapper and no `goto` in the first two; the third keeps two labels.
"Lines" are instruction lines by the private check on the published
units; "slots" are `fndiff`'s.

- `func_8013eb28` (204 bytes, resident): unit `d3eb28_r1`, `baseline
  204 bytes at 0x8013eb28, built 204 bytes at 0x8013eb28, 0 differing
  instruction slots: IDENTICAL`.
- `func_8013ec50` (204 bytes, resident): unit `d3ec50_r1`, `baseline
  204 bytes at 0x8013ec50, built 204 bytes at 0x8013ec50, 0 differing
  instruction slots: IDENTICAL`. It is the first with another callee
  for the pass, `func_8013ed1c`, which it calls with the two arguments
  of that function's definition.
- `func_8013ed90` (240 bytes, resident): unit `d3ed90_r1`, `baseline
  240 bytes at 0x8013ed90, built 240 bytes at 0x8013ed90, 0 differing
  instruction slots: IDENTICAL`.
- What the first two needed, measured on `func_8013eb28`: the table
  entry is read into one 16-bit local, copied into a second, and the
  first then holds its low byte (the entry read straight into the
  second: 24 of 51 lines); the flag bits are taken through an `int`
  copy of the flags (straight from the flags: 22); the index is
  narrowed to a byte in `slot` and held once more in `i`, and the calls
  of the first arm and the failures pass `i`, the other passes `slot`
  (with `slot` alone 20 lines, with `i` alone 16). That second holder
  of the index is the one form here that a reader may stop at; the
  unit's comment gives the two numbers. The last two arms test for the
  failure first (`if (t != flags && f != 2)` failure, else pass): a
  worker's earlier exact form had the pass first, three labels and a
  do-while wrapper; with the test turned round neither is needed.
- `func_8013ed90` reads the entry the same way (a separate local for
  the entry: 21 of 60 lines; the bits straight from the flags: 26). Its
  pass increments the slot's counter as a halfword through a cast of
  the byte field's address, as the readable version had it and with its
  note. It calls `func_8013f2c8` with three arguments, although that
  function is defined with one and its 19 other callers pass one: the
  call is written through a cast of the callee with the guide's comment
  above the function; with one argument 22 slots differ. Two labels
  stay (`fail`, `ok`): every structured form measured differs in 22 to
  63 lines.
- All three leave `ps1/src/resident_nonmatching/` (three files each and
  their README lines), and their lines leave `symbols.ld`.
- What ran: `fndiff.py --rebuild` on the three units (the lines above);
  `matchbuild.py --image resident`: `RESULT: PASS`; every unit compiled
  with `-Wreturn-type` (all three are `void`); the inventory sweep
  (three rows get their names) and the map's render: `overall:
  5453/5600 distinct functions exact (97.4%); 12883/13072 placements
  (98.6%); 1710816/1809836 code bytes (94.5%)`. No whole build, no
  differential test.
