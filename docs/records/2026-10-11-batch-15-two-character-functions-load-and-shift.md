
## Batch 15: two functions of the character modules exact, a load kept apart from its shift (2026-10-11)

- `func_801b4100_slot04_0e` (268 bytes) and `func_801b420c_slot04_0e`
  (256 bytes), from `slot04b_nonmatching`. The image `slot04_0e` has a
  second placement, `slot05_0e` (the build's `like` line), so two
  placements each. The tree is made from main at `45435b5`.
- The residue that every earlier pass named: both read the high half of
  a record's word. The original loads the whole word (`lw`) and shifts
  it right by 16 in a later instruction; the build made one halfword
  load (`lh`) of it, and no order of statements or local kept the two
  apart.
- What closed it: the read of that word through a cast to `volatile` at
  its one use, `(*(volatile s32 *)&rec->field_10 >> 16)`. The compiler
  does not join a volatile load with the shift. The guide names
  "`volatile` in a cast at one use" among the stand-ins; batches 11 to
  13 used it in the library, this is its first use in game code. The
  label is the guide's general one (order of instructions), with a
  comment of its own on what the original does there. Without the
  cast, on the final text: 5 differing instruction slots in each
  function.
- Besides the cast, ordinary C. In `func_801b4100_slot04_0e`: the sum is
  written with the object's position first (the other way: 2 slots);
  the store of the slot's `field_48` stands behind the store of its
  `pos_y` (before it: 3 slots, 272 bytes); the object's `field_65` is
  read into a local early and stored late (without the local: 10
  slots, 272 bytes). All four measured at the top level on the final
  text. In `func_801b420c_slot04_0e` the order of seven stores behind
  the position is the one a worker's sweep found with the cast in; no
  figure is given for other orders.
- How it went: six workers were given the nearest open functions to
  write again from the listing alone, without the earlier candidates.
  None came closer than the earlier candidate by its own form (five
  reports: 9, 8, 3, 3 and 2 slots where the earlier ones had 3, 3, 2,
  3 and 2). The sixth, on `func_801b4100_slot04_0e`, reached 5 slots
  and named the joined load; the top level put the cast in and swept
  the positions of two statements (0 slots at the 36th form). A worker
  then carried the form to `func_801b420c_slot04_0e` within a minute.
  A third sibling, `func_801b4a3c_slot04_0e`, is in work.
- The units' comments of what the functions do are the nonmatching
  files', with two meanings replaced by the fields the code names
  ("the partner's height": its `field_70`; "the record's x offset": the
  high half of its `field_10`) and "starts sequence 0x49" replaced by
  the call. Both declare the record table as the image's exact unit
  `slot04_0e_6b64_r1.c` does, and their definitions agree with the
  prototypes in `slot04_0e_3950_r1.c`, which calls them.
- Taken out for each function: its three files of `slot04b_nonmatching`,
  its 6 lines of that folder's page, its alias in `symbols.ld`. A
  private check compared the page with main's minus the two functions'
  lines: no other difference.
- What ran: `matchbuild.py --image slot04_0e` and `--image slot05_0e`:
  `RESULT: PASS` each. `fndiff.py --rebuild`:
  - `slot04_0e_4100x_r1`: `baseline 268 bytes at 0x801b4100, built 268 bytes at 0x801b4100, 0 differing instruction slots: IDENTICAL`
  - `slot04_0e_420cx_r1`: `baseline 256 bytes at 0x801b420c, built 256 bytes at 0x801b420c, 0 differing instruction slots: IDENTICAL`
  The figures of the variants: the same command on the unit with that
  change, the unit put back. The byte-offset scan on the two units:
  none. The compiler's warnings on the two units: none. The map's
  render:
  `overall: 5552/5600 distinct functions exact (99.1%); 13006/13072 placements (99.5%); 1774488/1809836 code bytes (98.0%)`.
  No whole build, no PC build, no differential test.
