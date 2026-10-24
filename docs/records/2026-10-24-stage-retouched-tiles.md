## Stages: the PS1's retouched layer tiles drawn as the arcade draws them (2026-10-24)

Toward no disc, by the owner's rule "rebuild arcade-style". Where the
PS1 changed the arcade's layer 1 and 2 art, the arcade's version is now
built, from an explicit counted table (`DESIGN_BY_DESIGN` in `stages.py`):

- stage 03 layer 1: 192 cells the arcade leaves blank (rows 28 to 31),
  which the PS1 filled with copies of the rows above, are blank;
- stage 03 layer 2: six 32 x 32 tiles the PS1 retouched are the arcade's;
- stage 10 layer 1: 17 retouched tiles are the arcade's, and 3 cells take
  the arcade's palette row.

These bytes differ from the disc's on purpose and are served owned (each
layer gains own cell and tile spans; `struct arcade_stage_span`). 29 more
page slots of stage 03 were found to be arcade tiles. Module zeros and the
stage modules' small integer words stay with the object lists (a later
step).

Checked on 2026-10-24: `stages.py --compare-c` (the C equals the model for
all 20 stages), the unit tests (stage 25, stages 47, sprite tiles 25),
`--arcade-check` status 0, a pack made again and its start. Stages 03 and
10 were not shown in a fight.
