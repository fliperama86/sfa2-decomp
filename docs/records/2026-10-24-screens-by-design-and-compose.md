## Screens: blank by design, and one parser of the arcade's frames (2026-10-24)

Toward no disc, by the owner's rule "rebuild arcade-style".

- By design. A screens item or picture op can now mark units that differ
  from the disc on purpose (`ARCADE_SCREENS_BY_DESIGN` on a ZERO item,
  `ARCADE_SCREENS_OP_BLANK`); verify skips them, the build counts them and
  `other_files.py --compare-c` fails when the count differs from the
  generator's explicit table (`DESIGN_ROWS`). Blank now: the ranking
  screen's backdrop page (CONT slot 0x15, no arcade source), the PS1's own
  logo tiles of the final screen (END slot 0x2d; the arcade's CAPCOM tiles
  stay) and the PS1's own tiles of the ending art (END slot 0x29; the
  arcade's stay). These segments are then built from the arcade alone.
- `port/arcade/compose.c` is the one parser of the arcade's sprite records
  and scroll 2 blocks, with a canvas and a 16 x 16 cut; `screen_lists.c`
  now uses it. It is the base of the next step, the screens' tiles cut from
  the arcade's frames.

Checked on 2026-10-24: the unit tests (compose 12, screens 36, screen
lists 14), `other_files.py --compare-c` (0 problems; 46,261 units by
design), `screen_lists.py --compare-c` unchanged against the previous
commit, `--arcade-check` status 0, a pack made again and its start (see the
stage record of the same day).
