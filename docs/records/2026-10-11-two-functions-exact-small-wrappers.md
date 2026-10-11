## Two functions exact with a small wrapper: func_8013b558 and func_8011f06c (2026-10-11)

Both carry a do-while wrapper under the rule of the guide, and in both
the wrapper is small: three statements in the first, one in the second.
The record says where else it was tried, so that the choice can be
judged. "Lines" are instruction lines by the private check on the
linked function.

- `func_8013b558` (608 bytes, resident; inferred: it walks the pairs of
  objects of the object table and calls a test on each pair that can
  collide): unit `d20cf0_r2`, `baseline 608 bytes at
  0x8013b558, built 608 bytes at 0x8013b558, 0 differing instruction
  slots: IDENTICAL`. The wrapper stands around the three statements that
  look the first object's box up: the index read from its frame, the
  load of the box table, and the store of the box's address. Without it
  8 of 152 lines differ (the table pointer and the index in each
  other's registers). The index is held in a local (`index_a`): with
  the index written inside the sum, no placement of a wrapper is exact
  (8 to 16 lines). With the local, numbered 1 the table's load, 2 the
  store of it, 3 the index, 4 the load through the table, 5 the store:
  exact are 3 alone, 2 to 3, and 3 to 5 (published, the whole lookup);
  not exact: 1 (8), 1 to 2 (8), 1 to 3 (8), 1 to 4 (4), 1 to 5 (8), 2
  (8), 2 to 4 (4), 2 to 5 (8), 3 to 4 (4), 4 (8), 4 to 5 (8), 5 (4);
  around both objects' lookups 27. The second object's lookup needs no
  wrapper. The two pointer locals each hold first a table and then a
  box, as the candidate had it: with two locals on the second object's
  side 17 to 20 lines differ.
- `func_8011f06c` (48 bytes, resident: clears 128 bytes of the game
  state and one byte more): unit `d1f06c_r1`, `baseline 48 bytes at
  0x8011f06c, built 48 bytes at 0x8011f06c, 0 differing instruction
  slots: IDENTICAL`. The wrapper stands around the one statement of the
  loop, `*p++ = 0;`. Without it 10 of 12 lines differ (the pointer and
  the counter in each other's registers, and the order of their
  setup). This is the weakest kind of wrapper, around a single
  statement, and nothing larger is exact: around the loop 10 lines,
  around the pointer's setup and the loop 10, around everything with
  the last store 10. Also exact, and no better: around the pointer's
  setup alone; around `*p = 0;` with the increment outside; around the
  increment alone. Without any wrapper the closest forms differ in 2
  lines (the counter declared before the pointer; both set before the
  loop; both in the `for`'s head; a `while` loop); a loop counting
  down 3; an index into the bytes 4; halfwords and 0x40 rounds 7. A
  worker had run 117 combinations of types and loop shapes without a
  wrapper before that, none exact. The cast to a byte pointer is needed:
  the field at that place is a halfword.
- Both leave `ps1/src/resident_nonmatching/` (three files each and their
  lines of the README, the control's second line included), and their
  lines leave `symbols.ld`.
- What ran: `fndiff.py --rebuild` on the two units (the lines above);
  `matchbuild.py --image resident`: `RESULT: PASS`; the inventory sweep
  (two rows get their names) and the map's render: `overall: 5442/5600
  distinct functions exact (97.2%); 12871/13072 placements (98.5%);
  1705096/1809836 code bytes (94.2%)`.
- Not claimed: that the original source had a macro where a wrapper
  stands. An exact build with a wrapper shows that the source is
  compatible with the original's bytes.
