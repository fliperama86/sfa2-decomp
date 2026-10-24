## Fighter sprite chunks built whole from the arcade (2026-10-24)

Toward no disc. A fighter file's sprite chunk was rebuilt only for the
tiles fighter.c numbers; tile 0, the tiles after them and ten files came
from the disc, so a pack kept every chunk whole. `port/arcade/sprite_tiles.c`
now builds every tile of every chunk from the arcade graphics by a table of
runs (arcade tile, step, flip; `sprite_tiles.py --c-table`), the runtime
packs each within the disc's size and serves it as a new kind of range
("owned": built from the arcade alone, served to every read; a pack keeps
none of it and its digest covers it).

Checked on 2026-10-24: `sprite_tiles.py --compare-c` (every tile of 45
chunks equals the disc's but 728 tiles of the PS1's own art, all packing in
their room), the unit test (19 cases), `--arcade-check` status 0 (it also
compares every built tile with the disc's), a start from a pack made again
(73 MB, from 86 MB), and a fight with Zangief and Dhalsim drawn whole. The
six chunks with the PS1's own art (Zangief's PL10 and PL10X, Gen's PL11,
PL13 and their X files: 240 and 62 tiles each) stay the pack's.
