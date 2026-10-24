## Widescreen: the shared map layers done, and a correction (2026-10-24)

Correction of `2026-10-24-widescreen-skewed-layers.md`: its run "idle at the
title so that its demo fights ran (no input)" was not idle. The owner was
playing Sakura in the test window ("no, I played as sakura, that was me
playing"), and the PS1 version has no fights of its own at the title. What
that run saw of the stages stands; that the game played them by itself does
not.

The third increment of the side tiles (`port/widescreen.md`): the forms of
two or three stages, `draw_tilemap_layer_32px_7x13`,
`draw_tile_map_layer_16px_half_scroll` and `draw_tile_map_layer_32px`, with
their copies in the table (60 entries).

What ran: `test_wide_stage.py` (46 cases: the real C of one copy per form
against the maker, side cells, negative controls, mutation checks, the table
against `ps1/src`); the Linux program with the user's disc and input scripts
of training mode (Ryu against each dummy of the select screen's three rows,
no sound), fifteen runs six at a time: fourteen stages' wide pictures, most
wide over the whole picture (Ryu, Adon, Chun-Li, Guy, Ken, Dhalsim, Gen,
Sakura, Rolento, Nash, Birdie, Rose, Sodom, Sagat); Zangief's run had a
window below 512 x 288 (six windows tiled), where the picture is the 384
frame by design, so it gave no wide picture.
