## Widescreen: the stage's ends, and the walls of the wide view (2026-10-24)

Steps 5 and 6 of `port/widescreen.md` (each plane kept inside its painting,
each plane anchored at its own speed) went in, and the owner, testing on
Zangief's stage, still saw the pillars and the big screen move against the
rest near the walls: "still a bit off", then "messing with paralax isn't
working". At the owner's word the stage's ends became a setting,
`[experimental] wide_ends`:

- `centred`: no plane moved at all; parallax is the game's, and the outer
  columns show past the stage's art at the walls. Tried first ("this is
  already better"; black past the right end of Rolento's stage).
- `anchored`: steps 3, 5 and 6, the board's rules exactly.
- `narrowed`, the default: the game's screen taken as 512 wide
  (`port/src/wide_walls.c`). The camera's range is cut by 64 at each end, the
  fighters are held inside the wide view in place of the 384, and the camera
  follows with the wide view's margins. The walls of the world stay where
  they were and the picture ends where the stage does. The owner asked for
  the walls limited and then for "the internal code" to "consider the new
  proportion and not 4:3"; on trying it: "it looks really good in many
  stages, in fact they look perfect".

The four overrides are the game's C with the wide numbers.
`port/tools/test_wide_walls.py` compares them with the game's C at an extra
of 0 (4 functions x 4000 fixtures, byte for byte), checks the contract at 64
and that every other setting calls through; a mutation of the follower's
margin fails it. Seen in play (the owner, 2026-10-24): five stages with the
camera at both ends of its new range, 320..576.

Also in this change: the module loader no longer refuses when two images
share a page and the other's code there is stale (Zangief's mirror match;
`check_shared` gives that entry its own jump, with one line); the wide
canvas is read back regardless of the window's size; a once-per-stage line
of each layer's speed and the camera's ends.

Open, seen by the owner: black edges on Birdie's stage as the camera moves;
no art at the right end of Rolento's outdoor stage in the PS1's data;
Gouki's haze drawn over the 384 only. Moves that test the screen's edge in
the characters' own code (Vega's wall climb, wall jumps) still use the 384.
