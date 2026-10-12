## Batch 36: `func_801e98dc_slot06_07` exact (2026-10-11)

- Function: func_801e98dc_slot06_07, 148 bytes, image slot06_07, from ps1/src/slot06_nonmatching/. Unit slot06_07_98dcx_r1.
- The same text stands in four other images, each its own unit and nonmatching files: func_801e98c8_slot06_0c (slot06_0c_98c8x_r1), func_801e998c_slot06_0d (slot06_0d_998cx_r1), func_801e9c80_slot06_0e (slot06_0e_9c80x_r1), func_801e90a8_slot06_10 (slot06_10_90a8x_r1); 148 bytes each.
- The four nonmatching files have the same body up to the function name (read and compared); the units are the first unit with the name changed. All figures below were measured on the first unit only; the four others are IDENTICAL with the same text.
- Residue: registers and the stack frame (inferred). Closed by three zero terms ORed into the field_08 expression, a copy of the header through two locals, a cast-pointer store of field_04, an unused 8-byte local, and a base local set directly before the offset statement.
- Stand-in 1: one stand-in at 3 places, the term ORed to the field_08 bits (0 for every value of v), choice of registers. Without it (all three out): 10 slots. With fewer than three: 13 slots (matching worker's figure, not re-measured).
- Stand-in 2: `u8 unused[8]`, stack frame. Without it: 4 slots.
- Plain C, ordinary comments: header copied through t and v (one local instead: 18 slots, 144 bytes); field_04 stored through a cast pointer (plain store: 3 slots); base kept as a local (inline in the call: 9 slots).
- Taken out because not needed: the local p (the 0x1f0 term is written inline in the offset statement). The statement order of base after q was also identical, so base stands right before the offset statement.
- Taken out of the tree for each of the five functions: its 3 nonmatching files (.c, .fixtures.json, .py), 8 README lines, 1 alias line of symbols.ld. No list sentence edited. The units' entries in ps1/src/build.toml are the added units.
- What ran (last lines), all five images and units:
  slot06_07: RESULT: PASS; baseline 148 bytes at 0x801e98dc, built 148 bytes at 0x801e98dc, 0 differing instruction slots: IDENTICAL
  slot06_0c: RESULT: PASS; baseline 148 bytes at 0x801e98c8, built 148 bytes at 0x801e98c8, 0 differing instruction slots: IDENTICAL
  slot06_0d: RESULT: PASS; baseline 148 bytes at 0x801e998c, built 148 bytes at 0x801e998c, 0 differing instruction slots: IDENTICAL
  slot06_0e: RESULT: PASS; baseline 148 bytes at 0x801e9c80, built 148 bytes at 0x801e9c80, 0 differing instruction slots: IDENTICAL
  slot06_10: RESULT: PASS; baseline 148 bytes at 0x801e90a8, built 148 bytes at 0x801e90a8, 0 differing instruction slots: IDENTICAL
