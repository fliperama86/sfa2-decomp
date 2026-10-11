## A named local for a constant; func_80120cf0 exact with it (2026-10-11)

- The owner's decision of 2026-10-11, in his words: "name local is
  fine". Asked of him: three functions are byte-identical only when a
  constant sits in a named local, helpers and wrappers do not replace
  it; allow it with a fixed comment and the measurement without it? The
  matching guide gets the rule with this pull request ("What is
  allowed", one paragraph with the fixed comment; "What this compiler
  does", one entry with the measured case), and the sentence that had
  forbidden such a local points to it. The rule as written is narrower
  than the three words: only where the original's instructions show the
  constant in a register of its own that is then used; a local for a
  constant the original keeps in no register, a local that holds an
  address to move a load, and an assignment that nothing reads stay not
  allowed.
- `func_80120cf0` (592 bytes, resident; inferred: it starts a round:
  resets fields of the game state, loads others from the options
  tables, prepares both players): unit `d20cf0_r1`, `baseline 592 bytes
  at 0x80120cf0, built 592 bytes at 0x80120cf0, 0 differing instruction
  slots: IDENTICAL`. No wrapper, no helper. "Lines" are instruction
  lines of 148 by the private check on the published unit, each variant
  built and taken out again.
- The two named constants, each read from the original's listing:
  - `zero`: `move s2,zero` at 80120d70, read by eight byte stores
    (`sb s2`: the left player's kind, two fields of each player,
    `field_2e`, `data_801a6938`, `data_801a6985`). With 0 written at
    those eight places: 25 lines, 2 instructions fewer. The other zeros
    of the function are literals, as the original stores `$zero` there.
  - `none`: `li v1,-1` at 80120df4, read by three halfword stores
    (`field_110`, `head_a`, `head_b`). With -1 written at them: 12
    lines, 1 instruction more.
  - The original also keeps 1 in a register for three stores; the
    literal builds that, so no local stands for it.
- A second name. `externs.h` declares the byte at 0x80198067 as the
  scalar `u8 data_801a8067` (four other units use it; the name is older
  than its address). Read through the scalar: 7 lines. The unit reads it
  as `table_801a8067[0]`, an array name of its own for that address, and
  `symbols.ld` gets the line (the tree has such second names:
  `count_80190100` beside `data_80190100`, `table_80188ed0` beside
  `data_80188ed0`). No shared declaration changes.
- Measured besides and commented at their places: `data_801a6966`
  declared in the unit as an array (as the scalar that most units
  declare: 5 lines; one other unit declares the array too); the byte
  `data_8016e688` read signed through a pointer cast, as the original
  loads it with `lb` (the plain test of the header's `u8`: 1 line; a
  value cast `(s8)`: 1 line); the masked random byte through a byte
  local before its store (in one statement: 2 lines).
- The definition agrees with `protos.h` and its one caller
  (`s11ffdc_r5.c`). The console-only behaviours were looked for and not
  found (no shift, argument counts agree, `void`, no local without a
  value): no override.
- It leaves `ps1/src/resident_nonmatching/` (two files, its five README
  lines and its mention in the list of functions without a fixtures
  file), its line leaves `symbols.ld`, and its row of
  `ps1/inventory/game.tsv` gets its name (the sweep).
- What ran: `fndiff.py --rebuild` on the unit (the line above);
  `matchbuild.py --image resident`: `RESULT: PASS`; the private check
  of resident table entries: `548 functions in a declared table
  compared, 0 differ`; the unit compiled with `-Wreturn-type` and
  `-Wuninitialized`: 0 warnings (it is `void`); the map's render on
  this branch, which does not hold pull request 256:
  `overall: 5479/5600 distinct functions exact (97.8%); 12913/13072 placements (98.8%); 1727324/1809836 code bytes (95.4%)`. No whole build, no PC build, no differential test.
