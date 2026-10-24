## Stages: the object grids from the arcade (2026-10-24)

Toward no disc. A stage module's objects (the people and things that move
in the background) are drawn from grids: a header (columns, rows, palette
row, the offset of the top left) and a cell a 16 x 16 place, each a tile of
the stage's object chunk. Each grid is an arcade art record laid on the
16-pixel grid: `port/arcade/stage_objects.c` rebuilds it from its record
(a generated table of offsets, record addresses and tile number ranges,
`stage_objects.py`; no byte of either game), and the stage build serves it
where it equals the disc. 604 of the 789 grids are built (20 KB); the rest
stay the disc's (no matching record, no arcade tiles, or stage 0C's 32 that
come out differently). The steps that point at the grids come next.

Checked on 2026-10-24: the unit tests (stage objects 10, stage 29, stages
48; the new C with sanitizers and 32-bit), `stages.py --compare-c` (20 of
20 stages), `--arcade-check` status 0, a start, and fights on stages 0E,
0F and 05.
