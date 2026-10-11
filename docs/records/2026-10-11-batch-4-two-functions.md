
## Batch 4: two functions exact with labelled stand-ins, in one pull request (2026-10-11)

- `func_8014c4a8` (128 bytes, resident) and `func_800145f4_slot28` (132
  bytes, module slot28) are published together. Each was prepared on a
  branch of its own (unit, build entry, removal of its readable C, its
  page lines, its record) and the two branches are merged here onto main
  at `a18d46d`, without a conflict.
- `func_8014c4a8`: two stand-ins under the guide's general comment.
  Three locals are used again for later values that the original keeps
  in the same registers in turn (each later value in a local of its own,
  the copy kept: 12 differing instruction slots), and one copy of the
  difference into such a local (without it: 8). Ordinary spellings with
  their figures in ordinary comments: a 16-bit local for the stored
  value (as int: 3) and the index read into a local first (the box in
  one expression: 15). The unit was renamed after its address
  (`d4c4a8_r1`).
- `func_800145f4_slot28`: one stand-in under the guide's general
  comment, three nested do-while wrappers around the statement that
  loads the HUD pointer (no wrapper: 11 slots; one or two: 6).
- Every figure above was measured again on the final text of each
  branch at the top level, by rebuilding the unit with that one thing
  changed and putting it back (`fndiff.py --rebuild`). One figure of the
  branch was corrected by it (13 to 12, with the variant now stated in
  the comment).
- What ran on the merged tree: `matchbuild.py --image` for `resident`
  and `slot28`: `RESULT: PASS` each. `fndiff.py --rebuild`:
  - `d4c4a8_r1`: `baseline 128 bytes at 0x8014c4a8, built 128 bytes at 0x8014c4a8, 0 differing instruction slots: IDENTICAL`
  - `slot28_45f4x_r1`: `baseline 132 bytes at 0x800145f4, built 132 bytes at 0x800145f4, 0 differing instruction slots: IDENTICAL`
  The private check of resident table entries: `548 functions in a
  declared table compared, 0 differ`. The byte-offset scan on the two
  units: none. The map's render:
  `overall: 5503/5600 distinct functions exact (98.3%); 12941/13072 placements (99.0%); 1741784/1809836 code bytes (96.2%)`.
  The figures in the two functions' own records are those of each
  branch alone. No whole build, no PC build, no differential test.
