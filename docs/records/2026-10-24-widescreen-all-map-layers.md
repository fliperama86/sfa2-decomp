## Widescreen: every stage's map layers (2026-10-24)

The fourth increment of the side tiles (`port/widescreen.md`): the twelve map
layer functions of one stage each, so that every stage's map layers are drawn
over the wide view (72 copies in `wide_stage.c`'s table). Birdie's and Chun-Li's
other's `draw_scrolling_tile_map_layer` turned out to be two different
functions; most others are known shapes with other counts or flags.

What ran: `test_wide_stage.py` (94 cases: the real C of one copy of each of the
twelve against the maker on 64 fixtures each, side cells, negative controls,
the override's queue, the table against `ps1/src`, about 20 hand-made breaks of
the maker each caught); the Linux program with the user's disc and input
scripts of training mode, the fifteen stages of the select screen's rows, two
at a time, no sound, Ryu walking to the left wall and pushing the dummy to
the right wall (the owner's advice: hold left or right, the corners matter).
At the right end all fifteen fill the picture but for objects (Sagat's
pillars, Dhalsim's and Ryu's upper left, a pole of Guy's) and Birdie's
perspective edges; the left end was not reached (the dummy in the middle
holds the camera).
