
## Batch 8: seven more tile-map functions of the stage modules exact, no stand-in (2026-10-11)

- Seven functions from `slot06_nonmatching`, relatives of the routine of
  batch 7 with their own differences (a parallax term, a mirrored draw,
  another texture page for some cells): `func_801e99c8_slot06_02` (860
  bytes), `func_801e99b4_slot06_04` (812), `func_801e8dc8_slot06_05`
  (840), `func_801e9d14_slot06_0e` (1060), `func_801e9bb0_slot06_0f`
  (800), `func_801e9ef4_slot06_10` (724), `func_801e9114_slot06_12`
  (828). One placement each. The tree is made from main at `c774d77`.
- What held them: the same two things in all seven, in the block that
  splits the cell value into its two nibbles. The original has the
  remainder in `v1`, the temporary of the division by 16 in `v0` and the
  quotient in `a0`, copied to another register afterwards; earlier forms
  had three other registers there, and a copy of the target's first
  instruction in a delay slot where the original has a `nop`.
  - The `nop` follows from the registers (read in reorg.c, an
    inference): the instruction that the pass moved into the slot of the
    next branch sets `v0` in the original, and the pass then treats `v0`
    as live on the first branch's fall-through, so the `andi` at the
    target, which sets `v0`, is not copied.
  - The registers come from the order of two statements. The quotient
    is a temporary in the original, and the variable gets a copy of it:
    `nib2 = lo - (lo / 16) * 16;` first, then `nib = lo / 16;`. The
    compiler finds the second division again and makes the assignment a
    copy. With the quotient assigned first (`nib = lo / 16; nib2 = lo -
    nib * 16;`) the seven units differ from the original in 83, 36, 91,
    31, 22, 22 and 83 instruction slots (in the order above) and in size
    by 12, 4, 16, 4, 4, 4 and 12 bytes.
- No stand-in in any of the seven, and no wrapper: the constant of the
  command word needs none in these forms.
- Ordinary forms that the units have and the batch 7 units do not:
  - `func_801e8dc8_slot06_05` and `func_801e9d14_slot06_0e` test the
    cell against each tile number in an `else if` of its own, with the
    page statement in each arm. The original has one compare and branch
    per number. The guide lists a statement written in each arm of an
    `if` among the ordinary forms. No figure was measured for the same
    tests written as one condition.
  - `func_801e99b4_slot06_04`, `func_801e9ef4_slot06_10` and
    `func_801e9114_slot06_12` keep the cell in two locals assigned
    together (`cell = hi = ...`), one for the tests and the remainder,
    one for the command word.
- `func_801e99b4_slot06_04`, `func_801e9bb0_slot06_0f` and
  `func_801e9ef4_slot06_10` have no case for other values of `field_0e`
  than 8, 4 and 0xc and then read `x` and `y` without a first value.
  Each unit's comment says so and claims nothing about whether such a
  call happens.
- The units' text: the comment of what the function does is the one of
  the nonmatching file, reflowed; the bodies are the forms that this
  lane's workers reached on 2026-10-11, two of them changed at the top
  level before publishing (`slot06_04`: `tu` from `nib` directly, without
  an 8-bit copy that was not needed; `slot06_0f`: the body written again
  in the layout of its relatives). None of the renamed fields or
  functions is used (`renamefields.py` and `renamefuncs.py` against the
  tables of `7a1d6ab`: nothing rewritten).
- Taken out for each function: its three files of `slot06_nonmatching`,
  its lines of that folder's page (86 lines in all), its alias in
  `symbols.ld`. A private check compared the page with main's minus the
  seven functions' lines: no other difference.
- The guide's list of differences gets two entries, both inferred
  compiler examples with these measurements: the quotient as a
  temporary, and the `nop` that follows from a register.
- What ran, on the tree made from main at `c774d77`: `matchbuild.py
  --image` for `slot06_02`, `slot06_04`, `slot06_05`, `slot06_0e`,
  `slot06_0f`, `slot06_10`, `slot06_12`: `RESULT: PASS` each.
  `fndiff.py --rebuild`:
  - `slot06_02_99c8x_r1`: `baseline 860 bytes at 0x801e99c8, built 860 bytes at 0x801e99c8, 0 differing instruction slots: IDENTICAL`
  - `slot06_04_99b4x_r1`: `baseline 812 bytes at 0x801e99b4, built 812 bytes at 0x801e99b4, 0 differing instruction slots: IDENTICAL`
  - `slot06_05_8dc8x_r1`: `baseline 840 bytes at 0x801e8dc8, built 840 bytes at 0x801e8dc8, 0 differing instruction slots: IDENTICAL`
  - `slot06_0e_9d14x_r1`: `baseline 1060 bytes at 0x801e9d14, built 1060 bytes at 0x801e9d14, 0 differing instruction slots: IDENTICAL`
  - `slot06_0f_9bb0x_r1`: `baseline 800 bytes at 0x801e9bb0, built 800 bytes at 0x801e9bb0, 0 differing instruction slots: IDENTICAL`
  - `slot06_10_9ef4x_r1`: `baseline 724 bytes at 0x801e9ef4, built 724 bytes at 0x801e9ef4, 0 differing instruction slots: IDENTICAL`
  - `slot06_12_9114x_r1`: `baseline 828 bytes at 0x801e9114, built 828 bytes at 0x801e9114, 0 differing instruction slots: IDENTICAL`
  The seven figures of the other order are from the same command on
  each unit's text with those two statements changed, the unit put
  back. The byte-offset scan on the seven units: none. The map's render:
  `overall: 5527/5600 distinct functions exact (98.7%); 12974/13072 placements (99.3%); 1763184/1809836 code bytes (97.4%)`.
  No whole build, no PC build, no differential test.
