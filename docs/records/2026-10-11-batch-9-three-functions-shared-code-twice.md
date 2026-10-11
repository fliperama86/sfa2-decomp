
## Batch 9: three functions exact, the shared code written once per path (2026-10-11)

- `func_80078628_slot00` (132 bytes, second placement in `slot08` at
  `0x8008e728`), from `modules_nonmatching`; `func_801b0904_slot04_04` (168 bytes, second
  placement in `slot05_04` at `0x801c8904`), from `modules_nonmatching`;
  its twin `func_801b0904_slot04_12` (168 bytes, second placement in
  `slot05_12` at `0x801c8904`), from `slot04b_nonmatching`. No stand-in.
- One picture in the three. Two tests guard one block and everything
  else goes to other code. The original has the argument copy
  (`move a0,s0`) before the block's first call, where the build had
  none; its other code uses the object through `a0` without a copy of
  its own; and the copy sits in the delay slot of the second test's
  branch.
- Read in the compiler's source, as an inference: after register
  allocation the jump pass deletes a copy `a0 = s0` when it finds the
  entry copy `s0 = a0` behind it with no label, no set of either
  register and no call between (jump.c; reload.c, `find_equiv_reg`).
  That predicted the source: the other code stood twice, once for each
  test, `if (!a) { other } else if (b) { block } else { other }`. The
  first copy of the other code is the fall-through of the first test
  and loses its argument copy; the block stands behind a label and
  keeps its own; the two copies of the other code are merged later, and
  the pass that fills delay slots puts the remaining copy into the
  second branch's slot. The form was written from that reading and was
  exact at the first build.
- Measured with `fndiff.py --rebuild` on each unit's final text with one
  arm for both tests (`if (a && b) { block } else { other }`), the unit
  put back: `slot00_8628x_r1` 136 bytes and 5 differing instruction
  slots; `slot04_04_0904x_r1` and `slot04_12_0904x_r1` 164 bytes and 6
  slots each. Each unit's comment has its figure.
- The code that stands twice is two statements in `slot00` (a count and
  a call) and one call in the twins. It is ordinary code: the guide
  lists a statement written in each arm of an `if` among the ordinary
  forms.
- The units are the nonmatching files' C with that change; their
  comments of what the function does are those files'. They are written
  with main's names (`state_0` to `state_3`).
- Taken out for each function: its files of the nonmatching folder (3,
  2 and 2), its lines of that folder's page (6, 5 and 5), its alias in
  `symbols.ld`. By hand: the twins' names leave the list of functions
  without a fixtures file on the two pages. A private check compared
  each page with main's minus the functions' lines: only those two
  list lines differ.
- The guide's list of differences gets one entry, an inferred compiler
  example with these measurements. No rule changes.
- What ran, on the tree with main at `b1b6a44` merged in: `fndiff.py
  --rebuild`:
  - `slot00_8628x_r1`: `baseline 132 bytes at 0x80078628, built 132 bytes at 0x80078628, 0 differing instruction slots: IDENTICAL`
  - `slot04_04_0904x_r1`: `baseline 168 bytes at 0x801b0904, built 168 bytes at 0x801b0904, 0 differing instruction slots: IDENTICAL`
  - `slot04_12_0904x_r1`: `baseline 168 bytes at 0x801b0904, built 168 bytes at 0x801b0904, 0 differing instruction slots: IDENTICAL`
  `matchbuild.py --image` for `slot00`, `slot08` (run after the review named that placement, on
  the final tree), `slot04_04`, `slot05_04`,
  `slot04_12`, `slot05_12`: `RESULT: PASS` each (on the tree before
  main's `b1b6a44` was merged in; that merge adds one library unit and
  a tool option and changes none of these images' inputs). The
  byte-offset scan on the three units: none. The map's render:
  `overall: 5531/5600 distinct functions exact (98.8%); 12983/13072 placements (99.3%); 1764420/1809836 code bytes (97.5%)`.
  No whole build, no PC build, no differential test.
