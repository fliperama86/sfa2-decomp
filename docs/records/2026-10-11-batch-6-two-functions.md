## Batch 6: two more functions of the character modules exact (2026-10-11)

- `func_801b248c_slot04_0f` (120 bytes, also placed in `slot05_0f`) and
  `func_801b81f8_slot04_14` (348 bytes, also placed in `slot05_14`), both
  from `slot04b_nonmatching`. Each was prepared on a branch of its own;
  the two are merged here onto main at `7a1d6ab`. After the pull
  request was opened, main at `f4a543b` (the twelve renamed fields of
  `Object`, pull request 273) was merged in: `renamefields.py apply`
  with the table of `7a1d6ab` as `--old` rewrote four uses in the two
  units (`state_3` twice, `state_0`, `accel_x`), and the checks below
  were run again on that tree with the same results.
- `func_801b248c_slot04_0f`: one stand-in under the guide's general
  comment: a copy `t = a` before `a` takes a constant (a local of its own
  for the constant: 3 differing instruction slots). A search found the
  form; it was reduced by hand.
- `func_801b81f8_slot04_14`: no stand-in. `pos_y` is stored right after
  `pos_x` (stored after the next field: 5 slots). The original stores 6
  to `field_09` twice (two `sb` to that offset in its listing); the unit
  has both stores and a comment with the figure (without the second: 3
  slots).
- The three figures were measured again at the top level on each
  branch's final text (`fndiff.py --rebuild`, the unit put back).
- One conflict, with main: batch 5 and the first branch each took lines
  out of the same place of `slot04b_nonmatching/README.md`; the page was
  made again from main's text minus the function's own lines. The second
  branch also takes its function's name out of the page's list of
  functions without a fixtures file. A private check compared the page
  with main's minus the two functions' lines: only that list entry
  differs.
- What ran on the merged tree: `matchbuild.py --image` for `slot04_0f`,
  `slot05_0f`, `slot04_14` and `slot05_14`: `RESULT: PASS` each.
  `fndiff.py --rebuild`:
  - `slot04_0f_248cx_r1`: `baseline 120 bytes at 0x801b248c, built 120 bytes at 0x801b248c, 0 differing instruction slots: IDENTICAL`
  - `slot04_14_81f8x_r1`: `baseline 348 bytes at 0x801b81f8, built 348 bytes at 0x801b81f8, 0 differing instruction slots: IDENTICAL`
  The byte-offset scan on the two units: none. The map's render:
  `overall: 5509/5600 distinct functions exact (98.4%); 12955/13072 placements (99.1%); 1748852/1809836 code bytes (96.6%)`.
  No whole build, no PC build, no differential test.
