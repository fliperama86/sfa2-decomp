## One more tile-map function exact: `func_801e9f20_slot06_0d` (2026-10-11)

- `func_801e9f20_slot06_0d` (788 bytes), from `slot06_nonmatching`. The
  image `slot06_0d` is no other image's `like`, so there is one
  placement. The tree is made from main at `965702c`.
- The form has no stand-in. It is the form of batch 8 (the remainder
  written first with the quotient inside it, the quotient assigned
  second: `nib2 = lo - (lo / 16) * 16; nib = lo / 16;`), and besides
  that, against the nonmatching text: the page statement stands inside
  the loop, before the position; the test of the mirror flag is
  `field_0b == 0` with the plain arm first, and the mirrored arm is
  `x - col * 16 + ox`; the two texture coordinates go through locals;
  the command word is written in its store with no local for the high
  byte; x is a 16-bit unsigned local, y and the cell 16-bit signed.
  A worker of this lane found it. No figure is given for any of these
  alone: none was measured on the final text.
- The record table has one declaration in the image now. The unit of
  the image's initialisation loop (`slot06_0d_85ecx_r1.c`) declared it
  without dimensions, and the new unit indexes it by counter and
  buffer; both declare `Slot06Tile data_801f685c_slot06_0d[0x78][2]`
  (0x78 rows: the count that loop walks), as the three sibling images
  with an interleaved table do. That unit's one use changed from
  `->sprt` to `[0][0].sprt`; it was rebuilt and is identical.
- The unit's comment of what the function does is the nonmatching
  file's, with the contract's "excluded input" replaced by the wording
  of the earlier tile units: a value of `field_0e` other than 8, 4 and
  0xc has no case, x and y are then read without a first value, and
  nothing is claimed about whether such a call happens. The private
  unit check prints both locals; the port needs its own answer for
  them, as for the siblings.
- The unit uses none of the renamed fields or functions (the rename
  tool: 0 uses rewritten).
- Taken out: the function's three files of `slot06_nonmatching`, its
  13 lines of that folder's page, its alias in `symbols.ld`. A private
  check compared the page with main's minus the function's lines: no
  other difference.
- What ran: `matchbuild.py --image slot06_0d`: `RESULT: PASS`.
  `fndiff.py --rebuild`:
  - `slot06_0d_9f20x_r1`: `baseline 788 bytes at 0x801e9f20, built 788 bytes at 0x801e9f20, 0 differing instruction slots: IDENTICAL`
  - `slot06_0d_85ecx_r1`: `baseline 280 bytes at 0x801e85ec, built 280 bytes at 0x801e85ec, 0 differing instruction slots: IDENTICAL`
  The byte-offset scan on the new unit: none. The map's render:
  `overall: 5539/5600 distinct functions exact (98.9%); 12991/13072 placements (99.4%); 1767120/1809836 code bytes (97.6%)` (with main at `4e4900e`, which has the five library functions of
  pull request 289, merged in; before that merge the tree gave
  5534/5600).
  No whole build, no PC build, no differential test.
