
## Batch 7: eight tile-map functions of the stage modules exact (2026-10-11)

- Eight functions from `slot06_nonmatching`, one routine in eight stage
  modules (it draws a tile map as sprite records):
  `func_801e9f90_slot06_01` (648 bytes), `func_801e9d04_slot06_03` (672),
  `func_801e9970_slot06_07` (768), `func_801e9b54_slot06_08` (744),
  `func_801e9840_slot06_09` (728), `func_801e8df0_slot06_0a` (768),
  `func_801ea640_slot06_0c` (780), `func_801e8fec_slot06_11` (728). Each
  has one placement. The branch was made on main at `f9c69b1`; main at
  `50ac55b` is merged in.
- One residue held all eight, 13 differing instruction slots each: the
  two instructions of the constant in the record's command word
  (`0xe100001c`, `0xe100001b` in `slot06_0c`). In
  `func_801e9d04_slot06_03` the build has `lui v1` and `ori v1` before
  the first of the block's five stores and the sum in `v1`; the original
  has `lui v0`, `ori v0`, `addu v0,t0,v0` in a row after the fifth
  store. Read in the compiler's source (inferred): the first scheduling
  pass splits the constant's load in two before it orders the block, and
  a register set twice does not get the place next to its first use.
  The guide's list of differences has an entry for it.
- The stand-in, the same in the eight units: the guide's do-while
  wrapper of the second kind (order of instructions) around the one
  statement before the sum, `prim->sprt.v = tv;`, with the guide's
  comment and, in a comment of its own, the figure. It carries the
  second comment and not the first because the difference without it is
  the place of two instructions and, through that, the register of the
  sum; no reference whose register matters is inside it. The wrapper
  around the sum itself was measured on `slot06_03_9d04x_r1`: 7 slots.
- `func_801e9f90_slot06_01`: its cell value is a 16-bit local
  (`s16 cell`); with `int cell` and the wrapper it differs in 3 slots.
- `func_801e9f90_slot06_01` and `func_801e9d04_slot06_03` have no case
  for other values of `field_0e` and then read `x` and `y` without a
  first value. Each unit's comment says so and claims nothing about
  whether such a call happens.
- The units' text: the comment of what the function does is the one of
  the nonmatching file (its contract goes with that file); the body is
  the form that this lane's workers reached on 2026-10-11 from the
  nonmatching C, plus the wrapper. None of the twelve renamed fields of
  `Object` is used (`renamefields.py apply` with the table of `7a1d6ab`
  as `--old`: 0 uses rewritten in the eight files).
- Taken out for each function: its three files of `slot06_nonmatching`,
  its lines of that folder's page (90 lines in all), its alias in
  `symbols.ld`. A private check compared the page with main's minus the
  eight functions' lines: no other difference.
- Not in this batch: `func_801e9080_slot06_00`, the same routine, is
  exact with the same wrapper in a private trial (the scratch tool's
  byte comparison, not this build). Its nonmatching file is the base of
  that folder's build controls (`test_difftest_build.py`) and its
  contract is in `contracts.py`, so taking it out is a change to those
  tools and gets a pull request of its own. Other relatives of the
  routine in that folder are still open with other differences.
- What ran, on the tree with main at `50ac55b` merged in:
  `matchbuild.py --image` for `slot06_01`, `slot06_03`, `slot06_07`,
  `slot06_08`, `slot06_09`, `slot06_0a`, `slot06_0c`, `slot06_11`:
  `RESULT: PASS` each. `fndiff.py --rebuild`:
  - `slot06_01_9f90x_r1`: `baseline 648 bytes at 0x801e9f90, built 648 bytes at 0x801e9f90, 0 differing instruction slots: IDENTICAL`
  - `slot06_03_9d04x_r1`: `baseline 672 bytes at 0x801e9d04, built 672 bytes at 0x801e9d04, 0 differing instruction slots: IDENTICAL`
  - `slot06_07_9970x_r1`: `baseline 768 bytes at 0x801e9970, built 768 bytes at 0x801e9970, 0 differing instruction slots: IDENTICAL`
  - `slot06_08_9b54x_r1`: `baseline 744 bytes at 0x801e9b54, built 744 bytes at 0x801e9b54, 0 differing instruction slots: IDENTICAL`
  - `slot06_09_9840x_r1`: `baseline 728 bytes at 0x801e9840, built 728 bytes at 0x801e9840, 0 differing instruction slots: IDENTICAL`
  - `slot06_0a_8df0x_r1`: `baseline 768 bytes at 0x801e8df0, built 768 bytes at 0x801e8df0, 0 differing instruction slots: IDENTICAL`
  - `slot06_0c_a640x_r1`: `baseline 780 bytes at 0x801ea640, built 780 bytes at 0x801ea640, 0 differing instruction slots: IDENTICAL`
  - `slot06_11_8fecx_r1`: `baseline 728 bytes at 0x801e8fec, built 728 bytes at 0x801e8fec, 0 differing instruction slots: IDENTICAL`
  The figures without the wrapper (13 in each of the eight), with the
  wrapper around the sum (7) and with `int cell` (3) are from the same
  command on the unit's text with that one change, the unit put back
  after it. The byte-offset scan on the eight units: none. The map's
  render:
  `overall: 5518/5600 distinct functions exact (98.5%); 12964/13072 placements (99.2%); 1756152/1809836 code bytes (97.0%)`.
  No whole build, no PC build, no differential test.
