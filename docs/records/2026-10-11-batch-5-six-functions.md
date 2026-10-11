
## Batch 5: six functions of the character modules exact, the first of the folder `slot04b_nonmatching` (2026-10-11)

- By the owner's word of today ("yes") the 50 functions of
  `ps1/src/slot04b_nonmatching` and `ps1/src/slot06_nonmatching` are
  matched in this lane too. These are the first six: three pairs, each
  pair the same routine in the modules `slot04_17` and `slot04_11`.
  `func_801b24e0_slot04_17` and `func_801b24f0_slot04_11` (592 bytes
  each), `func_801b27a0_slot04_17` and `func_801b27b0_slot04_11` (528
  each), `func_801b34b8_slot04_17` (556) and `func_801b34e4_slot04_11`
  (628; these two are alike, not the same code). Each was prepared on a
  branch of its own and the six branches are merged here onto main at
  `ae125bc`.
- First pair: the candidate that the other lane had left for
  `func_801b24e0_slot04_17` built the original's bytes as it stood with
  today's headers. It was reduced (a cast, a mask and nested labels
  gone) and carried to the twin with that module's names. Two stand-ins
  under the guide's general comment: an assignment that nothing reads
  (`t = 0;`: 8 differing instruction slots without it) and the local `i`
  used again for the fourth table value (a local of its own: 17).
- Second pair: one stand-in of two locals under the general comment: a
  local that holds the constant 0x2000 for one test, and a local that
  keeps the masked value apart from its 16-bit copy (either taken out
  alone, or both: 3 slots).
- Third two: the guide's do-while wrapper of the first kind around the
  eight statements of the hit arm (plain braces: 32 slots in
  `slot04_11`, 34 in `slot04_17`), and the local `i` of that arm used in
  the other arm for a table halfword (a local of its own: 13 in each).
  `func_801b34b8_slot04_17` reads `obj->other` once before the test and
  uses that value after calls in one branch, as the original's
  instructions do; the readable C it replaces read the field again.
- Every figure above was measured again at the top level on each
  branch's final text (`fndiff.py --rebuild`, the unit put back).
- In the second pair a local is read without a first value on one way:
  when `field_cd` is not 0 at the first test and 0 at the second, after
  the call of `func_8014a170` between them. The original's instructions
  do the same with a register. `func_8014a170` itself does not write
  `field_cd`; what it calls was not followed.
- The conflicts between the branches were of the known kind (each took
  its own lines out of the same place of `slot04b_nonmatching/README.md`
  and of `symbols.ld`), resolved by taking both removals.
- What ran on the merged tree: `matchbuild.py --image` for `slot04_17`,
  `slot04_11` and `slot05_11` (the second placement of `slot04_11`):
  `RESULT: PASS` each. `fndiff.py --rebuild`:
  - `slot04_17_24e0x_r1`: `baseline 592 bytes at 0x801b24e0, built 592 bytes at 0x801b24e0, 0 differing instruction slots: IDENTICAL`
  - `slot04_11_24f0x_r1`: `baseline 592 bytes at 0x801b24f0, built 592 bytes at 0x801b24f0, 0 differing instruction slots: IDENTICAL`
  - `slot04_17_27a0x_r1`: `baseline 528 bytes at 0x801b27a0, built 528 bytes at 0x801b27a0, 0 differing instruction slots: IDENTICAL`
  - `slot04_11_27b0x_r1`: `baseline 528 bytes at 0x801b27b0, built 528 bytes at 0x801b27b0, 0 differing instruction slots: IDENTICAL`
  - `slot04_11_34e4x_r1`: `baseline 628 bytes at 0x801b34e4, built 628 bytes at 0x801b34e4, 0 differing instruction slots: IDENTICAL`
  - `slot04_17_34b8x_r1`: `baseline 556 bytes at 0x801b34b8, built 556 bytes at 0x801b34b8, 0 differing instruction slots: IDENTICAL`
  The byte-offset scan on the six units: none. The map's render:
  `overall: 5508/5600 distinct functions exact (98.4%); 12951/13072 placements (99.1%); 1747916/1809836 code bytes (96.6%)`.
  The figures in the six functions' own records are those of each
  branch alone. No whole build, no PC build, no differential test.
