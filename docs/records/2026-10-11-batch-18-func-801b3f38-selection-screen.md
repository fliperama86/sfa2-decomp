## Batch 18: `func_801b3f38_slot04_sel` exact, the sibling of batch 17 (2026-10-11)

- `func_801b3f38_slot04_sel` (340 bytes, the selection screen's module),
  from `modules_nonmatching`. One placement (no image is `like`
  `slot04_sel`). The tree is made from main at `5fcfeab` and has main
  at `0b2a087`, with batch 17, merged in.
- The residue is batch 17's, on the other argument: before the second
  call through the handler table the original steps the player pointer
  in its saved register and then copies it to the first argument
  register (`addiu s0,s0,916`, then `move a0,s0`); the build made one
  instruction of the two.
- Two stand-ins close it, each with its label and figure in the unit:
  - The term of batch 17 in the first test behind the calls, on the
    stepped pointer, in the form its review asked for (the pointer
    through the widest unsigned integer, then to 32 bits):
    `(game_state.field_07 | (((u32)(unsigned long long)w << 24) & 0x100)) != 0`.
    Without it: 344 bytes, 9 differing instruction slots.
  - The do-while wrapper for order around the first call statement,
    with the guide's fixed comment. With the term in and no wrapper the
    build sets the first argument before the record pointer is loaded
    and puts the pointer's step into the first call's delay slot, where
    the original has the second argument's copy. Without it: 340 bytes,
    3 slots.
  With both taken out: 336 bytes, 2 slots. All three measured at the
  top level on the final text.
- How it went: a worker on this function got the term from the top
  level while it ran and reached 3 lines with it (7 without); it
  searched about 9,600 orders and casts without getting lower. The top
  level then tried eight forms of the wrapper in the head; around the
  first call statement it was exact. Also tried and no better than 3: the
  assignments and the step written inside the calls' arguments (36
  forms), a local for the handler.
- The unit's comment of what the function does and its declarations are
  the nonmatching file's.
- One sentence of comment changed in batch 17's unit too
  (`s124304x_r1.c`), after the reviewer's note on pull request 296: the
  intermediate integer was called "the widest unsigned integer"; it is
  an unsigned integer of 64 bits, wide enough for the pointers of the
  32-bit and 64-bit compilers this project builds with, and both units
  say that now. Comment only; that unit was built again and is
  identical (928 bytes).
- Taken out: the function's three files of `modules_nonmatching` and its
  6 lines of that folder's page (it had no alias in `symbols.ld`). A
  private check compared the page with main's minus the function's
  lines: no other difference.
- What ran: `matchbuild.py --image slot04_sel`: `RESULT: PASS`.
  `fndiff.py --rebuild`:
  - `slot04_sel_3f38x_r1`: `baseline 340 bytes at 0x801b3f38, built 340 bytes at 0x801b3f38, 0 differing instruction slots: IDENTICAL`
  The figures of the variants: the same command on the unit with that
  change, the unit put back. The byte-offset scan: none. The compiler's
  warnings on the unit: none. The map's render:
  `overall: 5555/5600 distinct functions exact (99.2%); 13010/13072 placements (99.5%); 1776276/1809836 code bytes (98.1%)`.
  No whole build, no PC build, no differential test.
