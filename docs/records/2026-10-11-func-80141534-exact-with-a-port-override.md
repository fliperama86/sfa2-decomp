## func_80141534 exact; a port override masks its shift count (2026-10-11)

- `func_80141534` (140 bytes, resident; inferred: it tests one bit of a
  16-bit mask chosen by the object's kind and a pair of bytes): unit
  `e41534_r1`, `baseline 140 bytes at 0x80141534, built 140 bytes at
  0x80141534, 0 differing instruction slots: IDENTICAL`. No wrapper, no
  helper, no stand-in, no `goto`. "Lines" are instruction lines by the
  private check on the published unit.
- What made it exact against the published readable C, each measured on
  the published unit: the table entry is read into a signed 16-bit
  local directly after the first part of the count is computed, before
  the count is complete (read in the return statement: 17 of 35 lines);
  and the shift count is not masked (with `(shift & 31)` written: 2
  lines, one instruction more). Both places have a comment with their
  figure.
- The unmasked count is the original's behaviour: the count can reach
  134 (the first byte of the pair halved, plus 6, plus 1) and the
  original shifts with a variable shift instruction (`srav` at
  801415ac), which on the console takes the count modulo 32 (read from
  the listing, not tested). In C a shift by 32 or more has no defined
  result, so the PC program gets its own C:
  `port/overrides/func_80141534.c` is the unit's C with the count masked
  with 31, as the readable C had it. The overrides page has a fourth
  kind of change for it ("the shift count"), its row and its recorded
  lines.
- Why the private search found the form (inferred from the compiler's
  source and from a debugger on the compiler during the search, not
  shown by a dump here): the original fills the delay slot of one
  branch with the first instruction of the branch's target; the
  compiler refuses that when the register the instruction sets counts
  as live on the other path. The published readable C already had the
  early `return 1`; the private candidate the search started from did
  not.
- The override's own runs, from `ps1/src/slot06_nonmatching/` with
  `--folder ../../../port/overrides`: `func_80141534: built 144 bytes,
  original 140 bytes; cases 2000, discarded 0, equal 2000, different
  0`, `coverage: 35 of 35 instruction slots of the original executed`
  (seed 1 and seed 7 printed the same two lines); `control: different
  1514 of 2000 (expected more than 0)`, altered: `final inversion xori 1
  replaced by xori 0, instruction slot 32`; `writes: cases 2000,
  discarded 0, outside 0 (largest 0 bytes)` on both seeds; `replay:
  fixtures 4, passed 4, failed 0` (the fixtures file moved with the
  contract; it was not recorded again). The setup draws the first byte
  of the pair from 0 to 255, so the cases include counts of 32 and
  more.
- It leaves `ps1/src/resident_nonmatching/` (its C is deleted, its
  contract and fixtures move to `port/overrides/`, its six README lines
  go), its line leaves `symbols.ld`, and its row of
  `ps1/inventory/game.tsv` gets its name (the sweep).
- What ran: `fndiff.py --rebuild` on the unit (the line above);
  `matchbuild.py --image resident`: `RESULT: PASS`; the private check of
  resident table entries: `546 functions in a declared table compared,
  0 differ`; the unit compiled with `-Wreturn-type` and
  `-Wuninitialized`: 0 warnings (both paths return a value); the five
  runs and the replay above; one build of the PC program from this
  branch: `units: 3980 compiled, 132 of them nonmatching, 0 failed`,
  `functions overridden in C: 15`, linked and verified; one start of
  that program without a window and with `--no-audio`, 35 seconds: no
  `stop` or `refused` line, `overrides in C: 15`; the map's render:
  `overall: 5461/5600 distinct functions exact (97.5%); 12893/13072
  placements (98.6%); 1715900/1809836 code bytes (94.8%)`. No whole PS1
  build, no other function tested again.
