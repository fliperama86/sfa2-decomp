## Three functions exact: func_8013fc18, func_800793d4_slot2b, func_801b2560_slot04_00 (2026-10-11)

Each was found by one worker on a bounded try with the two ranking rules
of the same day's record, made ready against the shared headers by a
second worker, and checked at top level. The counts of "instruction
lines" below are those of the private check on the linked function; the
three IDENTICAL lines are the build's.

- `func_8013fc18` (384 bytes, resident): unit `d40b5c_r2`, `baseline 384
  bytes at 0x8013fc18, built 384 bytes at 0x8013fc18, 0 differing
  instruction slots: IDENTICAL`. One do-while wrapper, around the five
  statements that compute the horizontal offset between the two objects.
  Without it 10 of 96 instruction lines differ (two values in each
  other's registers). Ten other placements were tried (the first three
  statements, the last four, the five with the subtraction that follows,
  and so on): 10 to 41 lines, none exact. The function copies the global
  reference into a local struct (`other_ref = ref_other;`) and reads the
  other object through it; the forms a reader might expect in its place
  were measured with the same wrapper: a pointer local set from
  `object->other` 17 lines, set from `ref_other.p` 19, `object->other`
  read again 21, `ref_other.p` read again 15. One halfword is read
  through a cast at a field that the struct has as a byte (the plain
  spelling: 1 line); the comment at it says so.
- `func_800793d4_slot2b` (204 bytes, the module of slot `0x2b`): unit
  `slot2b_93d4x_r1`, `baseline 204 bytes at 0x800793d4, built 204 bytes
  at 0x800793d4, 0 differing instruction slots: IDENTICAL`; its second
  placement in the image of slot `0x2c` passes too. One do-while
  wrapper, around the eight statements that set the object up. Without
  it 15 of 51 lines differ. Other placements: the first seven of the
  eight 11 lines, the last seven 6, the eight with the two calls after
  them 12, with the increment before them 5, the whole if/else 20. The
  candidate's two workarounds are out (a pointer to a table held in a
  local, a copy of the object pointer).
- `func_801b2560_slot04_00` (144 bytes, the module of the first
  character file): unit `slot04_00_2560x_r1`, `baseline 144 bytes at
  0x801b2560, built 144 bytes at 0x801b2560, 0 differing instruction
  slots: IDENTICAL`; its second placement in `slot05_00` passes too. No
  wrapper. The candidate's `goto` is gone (an early return gives the
  same bytes). Two locals of 8 and 16 bits hold values that the function
  computes before a store: without the first 4 of 36 lines differ,
  without the second 5, and with `int` for either the same. The word at
  `field_14`, which the struct has as a halfword, is read and written
  through a cast (the plain member: 3 lines); the comment at it says so.
- All three leave their `*_nonmatching` folders (three files each, and
  their lines of the two READMEs). The line of `func_8013fc18` and of
  `func_800793d4_slot2b` leave `symbols.ld`; the third had none.
- What ran: `fndiff.py --rebuild` on the three units (the lines above);
  `matchbuild.py --image` for `resident`, `slot2b`, `slot2c`,
  `slot04_00` and `slot05_00`: `RESULT: PASS` each; the private
  declaration check of module units: `data: 3161 symbols declared in
  module units, 0 in more than one way`; the inventory sweep (one
  resident row gets its name) and the map's render: `overall: 5439/5600
  distinct functions exact (97.1%); 12868/13072 placements (98.4%);
  1703372/1809836 code bytes (94.1%)`.
- Not claimed: that the original source had a macro where a wrapper
  stands. An exact build with a wrapper shows that the source is
  compatible with the original's bytes.
