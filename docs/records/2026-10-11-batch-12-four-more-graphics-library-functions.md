## Batch 12: four more functions of the graphics library exact (2026-10-11)

- `SetDrawEnv` (`0x80158a84`, 676 bytes), `_getctl` (`0x801598e8`, 24),
  `_exeque` (`0x80159cec`, 788) and `get_alarm` (`0x8015a2b8`, 364):
  functions of the library's `libgpu/sys.c` that had no unit. The names
  are the reference's, by the functions' shape and place. The tree is
  made from main at `ee1556f`, which has pull requests 289 and 290.
- How they were found: the list of open functions was made again by
  image and address, from the coverage tool's own classification, not
  by name. It showed nine functions of this one file that nobody had
  worked on by hand. A private check that builds the reference's file
  and compares each function with the image gave: `_getctl` exact with
  the reference's text, the other eight between 4 and 80 differing
  slots.
- `_getctl`: the reference's text, unchanged.
- `get_alarm`: the reference's text with one read through a cast to
  `volatile`, of the first of the three words it prints. The original
  forms that word's address in a register and loads through it; a plain
  read builds one load with the offset in it. Without the cast: 360
  bytes, 5 differing instruction slots.
- `SetDrawEnv`: against the reference, the width and the height of the
  area are limited from 1, not from 0 (the original branches on "not
  above 0" and loads 1); that is a difference of this game's library
  version, ordinary code. Each upper limit is read once into a local
  inside the conditional, as in `get_cs` of batch 11, and through the
  same cast: one stand-in at two places. With both casts taken out:
  652 bytes, 13 slots. With one taken out, either one: 664 bytes, 8
  slots.
- `_exeque`: the reference's text with the cast at six places: the
  first read of the callback, the three stores of the last call's
  words, the store that clears the pending flag, and the read of the
  callback at its call. One stand-in at six places; with all six taken
  out (the reference's text): 744 bytes, 26 slots. Each was taken out
  alone: 7, 7, 7, 8, 7 and 8 slots in that order. A seventh cast, on
  the read of the pending flag, was in the first exact form and was
  taken out: the unit is identical without it.
- The casts are confined to single reads and stores; no shared
  declaration changes. This is the form the review of pull request 285
  asked for and pull request 289 used.
- The units: `sdk/libgpu/sys_p10.c` to `sys_p13.c`, one function each,
  made by hand in the form of the importer's parts from `sys_p3.c`, by
  a private generator: the part's header line, `get_tw` as a prototype,
  the function's prototype replaced by its text. `sys_p13` owns 76
  bytes of read-only data at `0x8016dca0`, the two format strings of
  `get_alarm`.
- Where the text comes from, corrected after the review of the first
  head: the first form of this record and of the library page said the
  text is the reference's with the changes listed here. It is not. The
  parts are cut from this project's adapted copy of the reference's
  `libgpu/sys.c`, a private working file in which earlier work of this
  project changed bodies to this game's library version, from the
  original's instructions. A comparison of the four functions with the
  pinned commit's file (`git show` of that commit's file in the
  reference's checkout, the two texts side by side) shows what that
  copy had changed before this batch: `_getctl` returns the table's
  entry alone (the pinned text also puts the index into the high
  byte); `get_alarm` has another limit (0xF0000 for 0x780000), a comma
  for the line end of its first message, and a second `printf`;
  `_exeque` is another body altogether; `SetDrawEnv` takes its upper
  limits from the library's state where the pinned text has constants.
  This batch's own changes are the casts, the lower limit 1 and the
  limit locals named above. The reference stays MIT with its notice;
  the differences are this project's. Nothing else was read for these
  four; the forms of this batch were written at the top level.
- The same holds for the word "reference's text" in the lines above on
  `_getctl`, `get_alarm` and `_exeque`: read it as the adapted copy's
  text.
- Other files: `symbols.ld` loses the three names that the units now
  define (`_getctl` had none) and gets two data names of the reference
  that the parts refer to, `D_last` (`0x80181a88`) and `D_intr_exeque`
  (`0x80181aa4`), their addresses read from the original's
  instructions. The inventory carries the four names. The library
  page: its counts (94 C files, 40 parts, seven made by hand), a
  paragraph on the four parts, and at four places "the 87 files counted
  above" became "the files counted above": the number had not followed
  the count above it since it was 87.
- What ran: `matchbuild.py --image resident`: `RESULT: PASS`.
  `fndiff.py --rebuild`:
  - `sdk_libgpu_sys_p10`: `baseline 676 bytes at 0x80158a84, built 676 bytes at 0x80158a84, 0 differing instruction slots: IDENTICAL`
  - `sdk_libgpu_sys_p11`: `baseline 24 bytes at 0x801598e8, built 24 bytes at 0x801598e8, 0 differing instruction slots: IDENTICAL`
  - `sdk_libgpu_sys_p12`: `baseline 788 bytes at 0x80159cec, built 788 bytes at 0x80159cec, 0 differing instruction slots: IDENTICAL`
  - `sdk_libgpu_sys_p13`: `baseline 364 bytes at 0x8015a2b8, built 364 bytes at 0x8015a2b8, 0 differing instruction slots: IDENTICAL`
  The figures of the variants: the same command on the part with that
  change, the part put back and built again. The map's render:
  `overall: 5543/5600 distinct functions exact (99.0%); 12995/13072 placements (99.4%); 1768972/1809836 code bytes (97.7%)`.
  No whole build, no PC build, no differential test.
- Still open in this file: `PutDispEnv`, `_clr`, `_dws`, `_drs` and
  `_addque2`. In `_addque2` the same casts bring the size to the
  original's 764 bytes with 16 slots left: the store of the interrupt
  mask comes after the store of the flag in the original and before it
  in the build.
