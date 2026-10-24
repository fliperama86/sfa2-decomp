## Stages: our own packing of the first layer, from the arcade (2026-10-24)

Toward no disc, by the owner's rule "rebuild arcade-style". The PS1 packed
each stage's layer 0 (the arcade's 8 x 8 scroll pieces, four to a 16 x 16
cell with one palette row a cell) its own way. The loader now packs it
itself from the arcade (`build_l0_own` in `stage.c`, flag
`ARCADE_STAGE_L0_OWN`): a cell whose pieces share an arcade row keeps that
row (row numbers stay the arcade's); otherwise its pixels are re-indexed
into a row holding all its colours, else into a merged row on a free row,
else to the nearest colour; where the tiles exceed the PS1's room, the
least-used tile folds into its nearest. Counts per stage are in the
generated table. Ours is closer to the arcade's colours than the PS1's in
every stage (renders in the private folder). Stage 06's extra sets are its
layer 0's extra map; stages 05 and 12 were exact and are unchanged; stage
0D's slots named by its object lists stay the disc's until those lists are
built. No palette animation writes a layer 0 row (checked from the stage
modules' records).

Map cells in the module are served to the game's reads only (the module's
data pin), pages and palette rows owned. The first word of every built
chunk stays as its header records (the disc stream checks it).

Checked on 2026-10-24: `stages.py --compare-c` (20 of 20 stages, the C
equals the model), the unit tests (stage 29, stages 48, sprite tiles 25),
`--arcade-check` status 0, a start with no first-word difference, and
fights on stages 0E (Gouki against Rolento) and 0F (Zangief against
Dhalsim).
