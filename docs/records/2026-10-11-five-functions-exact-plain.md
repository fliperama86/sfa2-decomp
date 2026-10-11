## Five functions exact in plain C (2026-10-11)

No wrapper, no helper, no stand-in in any of them. "Lines" are
instruction lines by the private check on the linked function, measured
on the published units.

- `func_80134234` (1008 bytes, resident; inferred: it queues the pieces
  of a screen for drawing): unit `d34234_r1`, `baseline 1008 bytes at
  0x80134234, built 1008 bytes at 0x80134234, 0 differing instruction
  slots: IDENTICAL`. The parked form held index arithmetic in three
  locals; written into the arguments of the calls, and then as typed
  indexing (`strips[sel]`, `&strips2[sel][1]`,
  `data_80188d6c[sel].unknown_1c`), it is exact. The buffer number is
  read once into a 16-bit local at its declaration: read after the four
  early returns, 14 of 252 lines differ. `i = 0` stands before the
  calls: as an initializer 32 lines differ. The two tail loops are
  `while` loops: the first as a `for` loop, 11.
- `func_8011a880` (868 bytes, resident; inferred: it decides which of
  the two fighters is drawn in front and passes their parts on): unit
  `d189c4_r2`, `baseline 868 bytes at 0x8011a880, built 868 bytes at
  0x8011a880, 0 differing instruction slots: IDENTICAL`. The choice
  between the two is written with the store for the right fighter at
  two places; as one condition with `||` 7 of 217 lines differ, as the
  `>=` form with one store per side 13. The second fighter's pointer is
  formed from the first (`left + 1`); with `&player_right` 36 lines
  differ. The two players are read as `Fighter`, the type that has the
  part pointers, through one cast at the top; it is the first exact
  unit that uses that type.
- `func_801364a0` (456 bytes, resident; inferred: one step of a
  horizontal scroll follower between the two players): unit `v3_r5`,
  `baseline 456 bytes at 0x801364a0, built 456 bytes at 0x801364a0, 0
  differing instruction slots: IDENTICAL`. Three sums are written as an
  assignment and a compound operation on a 16-bit local (`amount = hi;
  amount -= lo;`): written in one expression each, 15 of 114 lines
  differ (alone 10, 11 and 1). Inferred: the original copies the value
  before each use, and a 16-bit local that is assigned at several
  places keeps the copy. The second player's pointer is formed from the
  first; with `&player_right` 29 lines differ.
- `func_80119694` (132 bytes, resident; inferred: it walks back over at
  most 29 words and lets the top of each run of links skip the run):
  unit `v7_r3`, `baseline 132 bytes at 0x80119694, built 132 bytes at
  0x80119694, 0 differing instruction slots: IDENTICAL`. The two inner
  loops test first and loop back from inside the branch: the first
  written as `while ((link = *cell) != ...)` differs in 16 of 33 lines,
  the second in 17, both in 32. Without the local for the loaded word,
  4.
- `func_801b03e4_slot04_0a` (188 bytes, module `slot04_0a`; inferred: a
  state that picks a random variant): unit `slot04_0a_03e4x_r1`,
  `baseline 188 bytes at 0x801b03e4, built 188 bytes at 0x801b03e4, 0
  differing instruction slots: IDENTICAL`. One unsigned local holds the
  random number and then the callee's result: with a local for each, 14
  of 47 lines differ; as `int`, 8. Each of its three casts is needed (5
  lines without any one).
- All five leave their `*_nonmatching` folders (files and README lines,
  the control's second line included; the module function also leaves
  the README's list of functions without a fixtures file), and the four
  resident lines leave `symbols.ld`.
- What ran: `fndiff.py --rebuild` on the five units (the lines above);
  `matchbuild.py --image resident` and `--image slot04_0a`: `RESULT:
  PASS` each; `module_protos.py`: 0 table entries with another
  parameter list; every unit compiled with `-Wreturn-type` (all five
  are `void`); the inventory sweep (four rows get their names) and the
  map's render: `overall: 5449/5600 distinct functions exact (97.3%);
  12879/13072 placements (98.5%); 1709228/1809836 code bytes (94.4%)`.
  No whole build, no differential test.
