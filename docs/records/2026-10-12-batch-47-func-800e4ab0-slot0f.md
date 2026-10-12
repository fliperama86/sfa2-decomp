## Batch 47: `func_800e4ab0_slot0f` exact (2026-10-12)

- Function: `func_800e4ab0_slot0f`, 608 bytes (152 instructions), image slot0f, unit `slot0f_4ab0x_r1` (ps1/src/slot0f/slot0f_4ab0x_r1.c). It comes from `modules_nonmatching`.
- Residue (from the matching workers): register allocation and instruction order of a tile-map draw loop. Closed by plain C forms: the 16-bit counter increment, a signed division by 16 written as two arms, a negated sum for the vertical position, a copy of the count read by the loop test, and a per-row y local.
- One stand-in, with the guide's fixed label: the local `n`, which is only a copy of `count` and is what the loop test reads (choice of registers; 11 slots with `count` in the test). Four plain C choices carry one-line ordinary comments with their figures, measured on the final text: `count` as an int parameter: 11 slots with s16; `fy` as the negation of field_14 minus 0x100000: 28; the two-arm division for `ty`: 67 (with `>> 4`); the local `yy`: 8.
- Taken out because not needed (each rebuilt alone, then all together, still IDENTICAL): the dead tests on key, ty and h (`(u8)x & 0x100`), the term ORed to wmask, the dead assignment `count = 1;`, and the early counter read `c1 = counter` (now `counter = counter + 1`).
- Taken out of the tree: the three nonmatching files (.c, .fixtures.json, .py), 6 README lines of modules_nonmatching, 1 alias line of symbols.ld. The list sentence needed no edit.
- The parameter `count` is an `int` in the definition (with `s16` the unit is 604 bytes and differs in 11 slots). The one declaration elsewhere, in the caller unit `slot0f_4800_r3.c`, said `s16` and is changed to `int` with it; both calls pass the constant 0x18, and that unit rebuilds IDENTICAL (296 bytes).
- What ran:
  - `matchbuild --image slot0f`: `RESULT: PASS`
  - `fndiff --rebuild slot0f_4ab0x_r1`: `baseline 608 bytes at 0x800e4ab0, built 608 bytes at 0x800e4ab0, 0 differing instruction slots: IDENTICAL`
  - `fndiff --rebuild slot0f_4800_r3`: `baseline 296 bytes at 0x800e4988, built 296 bytes at 0x800e4988, 0 differing instruction slots: IDENTICAL`
