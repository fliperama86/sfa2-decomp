## Batch 10: two more functions of the stage modules exact (2026-10-11)

- `func_801e9798_slot06_06` (972 bytes) and `func_801ea3b4_slot06_08`
  (180 bytes), both from `slot06_nonmatching`, one placement each. The
  tree is made from main at `b1b6a44`.
- `func_801e9798_slot06_06` is the seventeenth function of the tile-map
  routine and has the form of batch 8, no stand-in: the remainder in an
  `int`, the quotient written in the statement that uses it and
  assigned second (`nib2 = lo - (lo / 16) * 16; nib = lo / 16;`), the
  two texture coordinates in locals, one `else if` per tile number with
  the page statement in its arm (12 numbers), and the command word
  written in its store. With the quotient assigned first the unit
  builds to 964 bytes and 124 differing instruction slots. An unused
  local that a worker's form still declared was taken out at the top
  level, and the unit measured again.
- `func_801ea3b4_slot06_08` has two things, both with a comment and a
  figure in the unit:
  - The call of `func_80131094` passes a second argument although the
    callee takes one: the original has the local `a` (the object's
    `step_flags`, loaded once) in the second argument register at both
    calls. It is written through a cast of the callee with the guide's
    comment for that case. With one argument at both calls: 172 bytes,
    12 differing instruction slots.
  - The call stands in two arms, once for each path that leads to it
    (the picture of batch 9: on the path that falls through from the
    function's entry the original has no argument copy before the call,
    on the path behind a label it has one). With one call that both
    paths reach: 172 bytes, 9 slots. Ordinary code: a statement written
    in each arm of an `if`.
  The form was written from the reading of the compiler's jump pass
  that batch 9 records, and was exact at the first build of that form.
- The units' comments of what the function does are the nonmatching
  files'. `slot06_06` uses none of the renamed fields or functions;
  `slot06_08` is written with main's names (`vel_x`, `step_flags`,
  `accel_x`, `state_2`). The word that this function keeps another
  object's `state_2` in has the table's name `accel_x`; the names page
  leaves such uses open.
- Taken out for each function: its three files of `slot06_nonmatching`,
  its lines of that folder's page (12 and 7), its alias in `symbols.ld`.
  A private check compared the page with main's minus the two
  functions' lines: no other difference.
- What ran, on the tree made from main at `b1b6a44`: `matchbuild.py
  --image slot06_06` and `--image slot06_08`: `RESULT: PASS` each.
  `fndiff.py --rebuild`:
  - `slot06_06_9798x_r1`: `baseline 972 bytes at 0x801e9798, built 972 bytes at 0x801e9798, 0 differing instruction slots: IDENTICAL`
  - `slot06_08_a3b4x_r1`: `baseline 180 bytes at 0x801ea3b4, built 180 bytes at 0x801ea3b4, 0 differing instruction slots: IDENTICAL`
  The three figures of the variants are from the same command on the
  unit's text with that one change, the unit put back. The byte-offset
  scan on the two units: none. The map's render:
  `overall: 5533/5600 distinct functions exact (98.8%); 12985/13072 placements (99.3%); 1765572/1809836 code bytes (97.6%)` (with main at `0111554`, which has batch 9, merged in).
  No whole build, no PC build, no differential test.
