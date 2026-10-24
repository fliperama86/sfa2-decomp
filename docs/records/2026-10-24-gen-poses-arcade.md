## Gen's PS1-only poses drawn as the arcade draws them (2026-10-24)

Toward no disc, by the owner's rule "rebuild arcade-style". Gen's sprite
chunk (PL11, PL11X, PL13, PL13X) held 62 tiles of the PS1's own art in
four poses (cell lists 186, 284, 285 and 287). The arcade draws the same
poses with its own cells, and each of its cells lies at a cell position of
the PS1's list with the same palette. So each such tile number is now made
from the arcade tile the arcade draws in that cell (four are blank), and
the PS1's cell lists stay byte for byte. Gen's chunk now builds whole from
the arcade graphics and the pack keeps none of it.

- `port/arcade/sprite_tiles.py`: the `REDRAWN` table (tile number, arcade
  code, flip; numbers only) and its evidence; the redrawn tiles are runs
  of their own marked by bit 2 of the flip (`ARCADE_SPRITE_REDRAWN`).
- `port/arcade/sprite_tiles.c` accepts the bit; the loader's check skips
  those tiles, since they differ from the disc's by design.

Checked on 2026-10-24: `sprite_tiles.py --compare-c` (45 of 45 rows; the
table equals the tool's), the unit tests (22), `--arcade-check` status 0,
a pack made again (23 MB, from 24 MB), its start, and a fight as Gen from
it.
