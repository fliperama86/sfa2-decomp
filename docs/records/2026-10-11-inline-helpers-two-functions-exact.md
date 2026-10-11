## Inline helpers: func_801b0ea0_slot04_sel and func_80138d70 exact (2026-10-11)

A new kind of form, put to the owner with this change: a `static
inline` helper whose parameter carries a constant. Both functions were
parked because they were exact only with a local that holds a constant,
which the guide does not allow. The guide gets the rule ("What is
allowed") and the compiler's side of it ("What this compiler does").
"Lines" are instruction lines by the private check on the linked
function, measured on the published units.

- What the compiler does, read in its source: an argument of an inlined
  call that is not a register is copied into a new pseudo-register
  (`integrate.c`, `expand_inline_function`). So a constant passed to the
  helper is in a variable, where the same constant written in place is
  folded.
- `func_801b0ea0_slot04_sel` (552 bytes, module `slot04_sel`; inferred:
  the per-frame update of the two-player selection screen): unit
  `slot04_sel_0ea0x_r1`, `baseline 552 bytes at 0x801b0ea0, built 552
  bytes at 0x801b0ea0, 0 differing instruction slots: IDENTICAL`. The
  helper is `static inline Object *player_at(int n) { return
  &player_left + n; }`, used at all six places that name a player (four
  reads of the side, two handler calls). With the plain names
  `player_left` and `player_right` 53 of 138 lines differ; with the
  second player formed from the first in place (`&player_left + 1`) 19;
  with a macro of the helper's text 19; with the helper at the four
  reads only 39, at the two calls only 66, at the first read only 17.
  The parked form had five artifacts (a local for the constant 1, a
  local for 0 and a copy of it, a `void *` that held two things, a read
  of the left player written `(&player_right)[-1]`); none is left.
- The first handler of the table that this function calls through,
  `func_801b10c8_slot04_sel` (an empty function), gets the second
  parameter that the table's other handlers have and that the call
  passes; its unit `slot04_sel_0000_r4` is IDENTICAL as before.
- `func_80138d70` (340 bytes, resident; inferred: it builds a row of 32
  bytes from two counts of a table and gives each active player one
  byte of it, chosen at random): unit `d38d70_r1`, `baseline 340 bytes
  at 0x80138d70, built 340 bytes at 0x80138d70, 0 differing instruction
  slots: IDENTICAL`. The helper is `static inline void row_fill(u8
  *row, u8 value)`, a loop over the 32 bytes, called once with 0x40.
  With the same loop written in place 2 of 85 lines differ; with a
  macro of the helper's text 11. The function's two other fills are
  loops up to a count read from the table, not fills of a row, and do
  not use it. This unit also keeps one sum of an integer and a pointer,
  `((u8 *)(i + (int)pattern))[j]`, with its comment: `pattern[i + j]`
  differs in 10 lines, `(pattern + i)[j]` in 1 (the order of the
  operands of one addition).
- Both leave their `*_nonmatching` folders (files and README lines, the
  control's second line included; the module function also leaves the
  README's list of functions without a fixtures file).
- What ran: `fndiff.py --rebuild` on the three units (the lines above);
  `matchbuild.py --image slot04_sel` and `--image resident`: `RESULT:
  PASS` each; `module_protos.py`: 0 table entries with another
  parameter list; the inventory sweep and the map's render: `overall:
  5445/5600 distinct functions exact (97.2%); 12874/13072 placements
  (98.5%); 1706484/1809836 code bytes (94.3%)`. No whole build, no
  differential test.
- Not claimed: that the original source had an inline function at
  either place. An exact build with the helper shows that the source is
  compatible with the original's bytes.
