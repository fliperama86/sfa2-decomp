## Widescreen, side tiles: the skewed and partial-row forms (2026-10-24)

The second increment of the side tiles of `port/widescreen.md`: overrides of
the ten stage copies each of `draw_skewed_tile_layer_quads` and
`draw_tile_layer_sprites_with_partial_row` (identical code in every copy),
which run the game's C and queue the cells it leaves out for screen x -128
to 512. The skewed form's offsets depend on the row only, so its side cells
are well defined; the C already draws k = -11..32 and the maker adds the
quads past those that reach into the range.

What ran: `test_wide_stage.py` (30 cases: the real C of Chun-Li's copies
against the maker on 64 fixtures each, independent expectations of the side
cells, negative controls, the 52-entry table against `ps1/src`); the Linux
program with the user's disc, idle at the title so that its demo fights ran
(no input): Adon's stage wide over the whole picture, Rolento's and Rose's
wide but for parts of layers drawn by forms not yet done.
