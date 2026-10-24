## Widescreen: Birdie's perspective ceiling and floor drawn in the sides (2026-10-24)

The owner saw black edges on Birdie's stage as the camera moved ("Bird's
stage is the worse"). In a scripted training fight against Birdie (one
picture a second) they were wedges at the sides: the perspective ceiling and
floor, `build_perspective_layer_tile_quads_slot06_08`, a form of one stage
that had no side maker, ended where the C's 33 columns end, and each row's
quads lean by their own offset.

The new kind `PORT_WIDE_PERSP` of `port/src/wide_stage.c` makes that form's
side quads with the C's arithmetic (both passes of rows, the C's types and
signed divisions), the columns left of the C's and right of them whose quad
reaches into the view, up to 64 on each side. `port/tools/test_wide_stage.py`
(101 cases) compares it with the game's C over the C's own columns, over the
four bands of the first pass's row count, both passes, wrapped maps and
wrapped products; a change of the ramp's divisor fails it, and some
twenty-five other mutations too. Written by a helper against a pinned
contract, reviewed and tried in the game: the wedges are gone in every
picture of the fight. Left on that stage: the back wall's art ends about 12
pixels short of the wide view at both walls (as Rolento's sky does).
