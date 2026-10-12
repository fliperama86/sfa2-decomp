## Batch 3: four functions exact, three with labelled stand-ins and one without, in one pull request (2026-10-11)

- Four functions are published together: `func_8011cf98`,
  `func_80141cec`, `func_80022944_slot28`, `func_800e3478_slot0f`. Each
  was prepared on a branch of its own (unit, build entry, removal of its
  readable C, its page lines, its record) and the four branches are
  merged here onto main at `af9123a`, which holds batch 2 (pull request
  264).
- Three have stand-ins of the guide, each under its fixed comment with
  the measured figure: a copy local in `func_8011cf98` (27 slots without
  it) and in `func_80141cec` (2); in `func_80022944_slot28` a local for
  a constant (2), one do-while wrapper of the second kind (3) and a
  named constant (4). `func_80022944_slot28` also gives its callee
  `func_80022bf0_slot28` an unused parameter (5 slots with the call
  written without the argument); the callee's unit `slot28_12944_r3`
  stays exact.
- `func_800e3478_slot0f` has no stand-in. Its branch first had one (the
  address of a table written as another name's address minus 0x24). At
  the top level the saved block at 0x8016e664 was declared as one
  object under a second name, and the plain member reads are exact:
  the compiler takes the rows' address from the register that holds the
  flag's address by itself. With the rows under their separate name: 13
  slots. The guide gets one entry for this cause in its list of
  differences and what resolved them.
- Each figure above was measured again on this merged tree, by
  rebuilding the unit with that one thing taken out and putting it back
  (`fndiff.py --rebuild`): 27, 2, 2, 3, 4, 5 and 13 differing
  instruction slots, as the comments say.
- The conflicts between the branches, and with main once batch 2 was
  in it, were of the kind of batches 1 and 2 (two branches each took its own lines out of the same place of
  `modules_nonmatching/README.md`, of `resident_nonmatching/README.md`
  and of `symbols.ld`) and are resolved
  by taking both removals. One unit was renamed after its address
  before the merge (`d41cec_r1`).
- What ran on the merged tree: `matchbuild.py --image` for `resident`,
  `slot28`, `slot0f`: `RESULT: PASS` each. `fndiff.py --rebuild`:
  - `d1cf98_r1`: `baseline 1568 bytes at 0x8011cf98, built 1568 bytes at 0x8011cf98, 0 differing instruction slots: IDENTICAL`
  - `d41cec_r1`: `baseline 328 bytes at 0x80141cec, built 328 bytes at 0x80141cec, 0 differing instruction slots: IDENTICAL`
  - `slot28_2944x_r1`: `baseline 440 bytes at 0x80022944, built 440 bytes at 0x80022944, 0 differing instruction slots: IDENTICAL`
  - `slot28_12944_r3`: `baseline 76 bytes at 0x80022bf0, built 76 bytes at 0x80022bf0, 0 differing instruction slots: IDENTICAL`
  - `slot0f_3478x_r1`: `baseline 796 bytes at 0x800e3478, built 796 bytes at 0x800e3478, 0 differing instruction slots: IDENTICAL`
  The private check of resident table entries: `548 functions in a
  declared table compared, 0 differ`. The byte-offset scan on the four
  units: none. The map's render:
  `overall: 5499/5600 distinct functions exact (98.2%); 12935/13072 placements (99.0%); 1740580/1809836 code bytes (96.2%)`.
  The figures in the four functions' own records are those of each
  branch alone. No whole build, no PC build, no differential test.
