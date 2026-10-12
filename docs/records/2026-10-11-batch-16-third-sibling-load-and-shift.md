## Batch 16: the third sibling of batch 15 exact (2026-10-11)

- `func_801b4a3c_slot04_0e` (260 bytes), from `slot04b_nonmatching`;
  two placements (`slot05_0e` is `like` `slot04_0e`). The tree is made
  from main at `72e1009`, which has batch 15.
- The residue and what closed it are batch 15's: the original loads a
  record's whole word and shifts it in a later instruction, the build
  made one halfword load, and the read through a cast to `volatile` at
  its one use keeps the two apart. The label and its comment are the
  ones of the two siblings. Without the cast, on the final text: 5
  differing instruction slots.
- Besides the cast, ordinary C, found by a worker of this lane with two
  sweeps of statement order and measured again at the top level on the
  final text: the store of the slot's `pos_y` stands before the store
  of its `field_48` (behind it: 26 slots); the sum has the object's
  position first (the other way: 2 slots); the first five stores stand
  in the order `field_00`, `field_02`, `field_03`, `field_09`,
  `field_02` (with `field_03` before the first `field_02`: 5 slots).
  The slot's `field_02` is stored twice, 0x3b and then 0x14, as the
  original's instructions do and as `func_801b420c_slot04_0e` has it.
- The unit's comment of what the function does is the nonmatching
  file's, with "every eighth value of the frame counter" replaced by
  the test the code makes (the low three bits of `game_state.field_1d`
  are 0) and "the record's x offset" by the high half of its
  `field_10`. The record table's declaration and the definition's type
  agree with the image's other units.
- Taken out: the function's three files of `slot04b_nonmatching`, its 6
  lines of that folder's page, its alias in `symbols.ld`. A private
  check compared the page with main's minus the function's lines: no
  other difference.
- What ran: `matchbuild.py --image slot04_0e` and `--image slot05_0e`:
  `RESULT: PASS` each. `fndiff.py --rebuild`:
  - `slot04_0e_4a3cx_r1`: `baseline 260 bytes at 0x801b4a3c, built 260 bytes at 0x801b4a3c, 0 differing instruction slots: IDENTICAL`
  The figures of the variants: the same command on the unit with that
  change, the unit put back. The byte-offset scan: none. The compiler's
  warnings on the unit: none. The map's render:
  `overall: 5553/5600 distinct functions exact (99.2%); 13008/13072 placements (99.5%); 1775008/1809836 code bytes (98.1%)`.
  No whole build, no PC build, no differential test.
