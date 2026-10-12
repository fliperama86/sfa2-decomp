## Batch 21: `func_80012dec_slot01` and its copy `func_801e0ea8_slot0b` exact (2026-10-11)

- `func_80012dec_slot01` (372 bytes, image `slot01`) and
  `func_801e0ea8_slot0b` (372 bytes, image `slot0b`): one function in
  two images, the same instructions with another table (one distinct
  function for the map, two placements). Both from
  `modules_nonmatching`. Neither image is another's `like`.
- The residue of the earlier candidate (5 lines, one instruction short):
  the original masks the high nibble's byte once more (`andi
  a1,a1,0xff` behind `andi a1,a1,0xf0`) and has two neighbouring
  operations in the other order.
- What closed it: the types of five locals, nothing else. The texture
  byte is a `u8`, the two sizes and the two nibble values are `u16`,
  where the candidate had `int` for all. No stand-in, no label.
  Measured at the top level on the final text of the `slot01` unit,
  each changed back to `int` alone: the byte 368 bytes and 43 differing
  instruction slots; the first size 15 slots; the second size 2 slots;
  the low nibble's value 33 slots; the high nibble's value 368 bytes
  and 43 slots.
- How it went: a worker of this lane, briefed with this evening's two
  levers, saw that neither applied (the build was one instruction
  short because the compiler knew the value's range, not because of a
  joined copy) and searched the locals' types instead: 756 forms of
  three types and seven spellings, then the eight combinations of the
  two sizes' types. The form is in the `slot01` unit as found; the
  `slot0b` unit is the same text with that image's table, built and
  compared on its own.
- The signature: both definitions keep the second parameter as the
  callers declare it (`u8 *`, in `slot01_2714_r4.c`, `slot0b_0ea8_r4.c`
  and `slot0b_00c4_r2.c`) and take it as a pointer to the record type
  through a local. The candidate had the record type in the parameter;
  with the callers' type the units are identical too.
- The units' comments of what the function does are the nonmatching
  files' (the two files describe it in different words; each unit has
  its own file's).
- Taken out for each: its three files of `modules_nonmatching`, its 6
  lines of that folder's page, its alias in `symbols.ld`. A private
  check compared the page with main's minus the two functions' lines:
  no other difference.
- What ran: `matchbuild.py --image slot01` and `--image slot0b`:
  `RESULT: PASS` each. `fndiff.py --rebuild`:
  - `slot01_2decx_r1`: `baseline 372 bytes at 0x80012dec, built 372 bytes at 0x80012dec, 0 differing instruction slots: IDENTICAL`
  - `slot0b_0ea8x_r1`: `baseline 372 bytes at 0x801e0ea8, built 372 bytes at 0x801e0ea8, 0 differing instruction slots: IDENTICAL`
  The figures of the variants: the same command on the unit with that
  change, the unit put back. The byte-offset scan on the two units:
  none. The compiler's warnings on the two units: none. The map's
  render: `overall: 5560/5600 distinct functions exact (99.3%); 13016/13072 placements (99.6%); 1777324/1809836 code bytes (98.2%)`. The tree is made from
  main at `d55ff8a` and has main at `9c5851e`, with batch 20, merged
  in; both images were built again after the merge.
  No whole build, no PC build, no differential test.
