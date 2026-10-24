## Stages: the effect sprites from the arcade (2026-10-24)

Toward no disc. Each stage file's slot 3 (the effect sprites: hit sparks,
fireballs and the like, the same chunk in all 20 files, 2,336 tiles) is now
a row of the sprite table (`port/arcade/sprite_tiles.py`), built whole from
the arcade graphics like the fighters' chunks and served to every read; a
pack keeps none of it. Most tiles are arcade 16 x 16 tiles; 558 are
quarters of the arcade's 32 x 32 scroll 3 tiles, a new run kind
(`ARCADE_SPRITE_SCROLL3` and the quarter in the run's flip byte). The 23
tiles with no arcade source are blank by the owner's rule "rebuild
arcade-style".

Checked on 2026-10-24: `sprite_tiles.py --compare-c` (65 of 65 rows build
equal to the disc's decoded tiles, the 23 blank ones by design; every row
packs), the unit tests (25), `--arcade-check` status 0, a pack made again
(20 MB, from 22 MB), its start, and a fight from it (hit sparks drawn).
