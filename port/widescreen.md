# Widescreen

A 16:9 picture of the fights, behind the setting `[experimental] widescreen`
(default off). Asked for by the owner on 2026-10-23 ("widescreen, that is
the hard one"). This page is the plan and the contract of each step; a step's
record goes to `docs/records/`.

**State, 2026-10-24: step 1 and the first increment of steps 2 and 3 are
in.** Fights and menus are drawn on the 512 x 224 canvas and shown at 16:9.
The three layer forms most shared are drawn over the whole width, and the
view is anchored at the stage ends with the HUD centred. On Birdie's stage
(seen in a run) the walls and back of the stage fill the width; its
perspective floor and ceiling, one-stage forms, still end where the game's do.
Fourteen of the fifteen stages of the select screen's rows were seen wide
(scripted training fights, 2026-10-24), most over the whole picture.

**State, later on 2026-10-24: the stage's ends are a setting, `wide_ends`,
narrowed by default** (below, "The stage's ends"): the game's screen is taken
as 512 wide, the camera stops 64 short of each end and the fighters reach the
wide picture's edges, so the walls stay where they are and the stage's own
art fills the picture. The owner on it: "it looks really good in many stages,
in fact they look perfect". Gouki's haze, drawn over the 384 only, and the
black wedges of Birdie's perspective ceiling and floor are fixed (the order
of the side cells in an entry, below; Birdie's form, `PORT_WIDE_PERSP`).
Open: a strip of about 12 pixels at both walls where a back plane's art in
the PS1's data ends short of the wide view (Rolento's sky and poster,
Birdie's back wall).

## The owner's decisions (2026-10-23)

- **Gameplay stays the original's.** The fighters' walls, the camera and the
  spacing are the PS1's (and the arcade's): the play area stays 384 pixels
  wide and the wider picture only shows more of the stage at its sides. The
  fighters stop 64 pixels short of a screen edge.
- **The HUD stays centred**, where it is now. Spreading it to the edges may
  come later as an option.
- **2026-10-24, the walls of the wide view.** After the anchored ends moved
  the slower planes against the rest ("messing with paralax isn't working"),
  the owner asked for the view simply centred ("don't touch any of the
  paralax code and just remove any clipping"), then for the walls limited
  ("can't we limit how far the player can go on each side?"), and then for
  the game's own rules to use the wide screen ("the internal code should also
  consider the new proportion and not 4:3"). That is `wide_ends = narrowed`,
  the default: the first decision's play area of 384 is replaced by the
  wide picture's 512, the stage's walls unchanged. The anchored view stays as
  `wide_ends = anchored`, the board's rules exactly.

## What the game assumes

From a survey of the C on 2026-10-23 (read, not run):

- The display is 384 x 240 and the game draws rows 8 to 231 (draw areas of
  384 x 224 at (0, 8) and (0, 248) of the video memory, displays at (0, 240)
  and (0, 0)); the arcade canvas (`arcade_video`) shows exactly those rows.
  At the same pixel shape a 16:9 picture is 512 x 224: 64 pixels more on
  each side.
- The video memory has no room for a wider frame: the frames take x 0 to 383
  and the textures start at x 384. So the wide picture is drawn on a canvas
  of the PC's own (as psxrecomp does; its notes on Tomba show the flicker of
  widening inside the video memory).
- A stage is a tile map 1024 wide; the main layer's camera (`box_margin[0]`,
  the view's left edge) is clamped to 256..640, so the playfield is 256..1024.
  A 512 view fits in it: left edge 256..512.
- Each stage draws its layers as tiles from the camera: 25 columns of 16
  (400 pixels), 13 of 32, wider skewed and perspective layers, each stage
  with its own copies of the functions and fixed pools of primitives.
- The fighters' walls are `box_margin[0]` and `box_margin[0] + 0x180`
  (`clamp_pos_x_to_bounds_and_push_other.c`); the camera follows the
  players with dead zones of 96 pixels. These stay as they are.
- Objects are taken out at about 64 pixels past the 384 view
  (`clear_or_set_flag_by_pos_x_limit.c`: `pos_x - margin + 64 > 512`, and
  two kin), which is inside the wide picture.
- The HUD is laid out for 384 (life bars at 24 and 216, timer at 168).

## The plan

1. **The wide canvas.** Every primitive the game draws into its frame is also
   drawn on a canvas of 512 x 224, 64 pixels to the right, clipped at 512 in
   place of 384; the canvas is shown in place of the frame. HUD and fighters
   stay where they are (centred), menus look as now with black sides. What
   the game does not draw past 384 stays black. Contract below.
2. **The stages' sides.** The tile layers drawn over 512 pixels and more (the
   column counts and the left start of each stage's layer functions, and
   room for the primitives), with the gameplay unchanged.
3. **The stage ends.** At the ends of the camera's range the 512 view would
   show past the playfield; there the world is drawn shifted toward the
   stage's inside (the view's left edge clamped to 256..512) while the HUD
   stays centred. Needs the HUD's packets told apart from the world's (the
   classification of `ui_layout.h`).
4. **Objects in the sides.** The take-out margins widened by 64, so a
   projectile or a stage object is not lost while it is in the picture.
5. Then: the HUD at the edges as an option, other screens (select, demos)
   wide where they have the art.

## Step 1: the wide canvas

Setting `[experimental] widescreen` (default off), `--widescreen` /
`--no-widescreen`, and a row WIDESCREEN (ON, OFF) in the settings menu,
applied at once. It acts only where the arcade canvas acts (the game's
384 x 240 display in a 15-bit mode, `arcade_video` on); elsewhere nothing
changes. With it on, the window sizes of the settings menu are the 16:9 ones
(1280X720, 1600X900, 1920X1080, 2560X1440, 3840X2160, those that fit the
desktop) in place of the 4:3 ones.

The backend (`port/psyz.patch`, SDL_GPU; port_video_api 5) with
`Psyz_Sfa2SetWide(extra)` (0 off, else the pixels added on each side; the
port gives 64):

- **Frame areas.** A drawing area (the scissor of the draw area) of exactly
  384 x 224 is a frame area. Each frame area seen has a canvas of its own,
  (384 + 2 * extra) x 224 at the internal resolution (two in this game, one
  per display buffer; at most four kept).
- **Primitives.** A flush whose scissor is a frame area is drawn twice: into
  the video memory as now, and into that area's canvas with the same
  vertices, uniforms, pipelines and texture, a viewport moved by
  (extra - area.x, -area.y) and a scissor of the whole canvas. A video memory
  coordinate (x, y) lands at (x - area.x + extra, y - area.y).
- **Clears.** A clear (`Draw_ClearImage`) that meets a frame area clears the
  same rectangle on its canvas, moved the same way; one that spans the area's
  whole width spans the canvas's whole width. A clear of the whole area marks
  the canvas good.
- **Images.** A `LoadImage` or a `MoveImage` whose destination meets a frame
  area marks its canvas stale: what the game copied there is in the video
  memory only (the settings menu, the arcade intro and movies do this).
- **Present.** When the setting is on, the arcade canvas is active and the
  frame area at (display x, display y + 8) has a good canvas, that canvas is
  shown in place of the 384 x 224 rows, at its own aspect (16:9 for 512 x 224:
  `port_video_wide_fit` in `video.h`); otherwise the frame is shown as now
  (4:3, with bars in a wider window). Sharp bilinear and the other filters
  work on the canvas as on the frame.
- **Read back.** `Psyz_Sfa2ReadWide(pixels, w, h)` gives the canvas of the
  frame last shown (RGB 5551 halfwords) for `--dump-vram`, which then also
  writes `PREFIX_wide_NN.ppm`.

Not in step 1: anything past 400 pixels of the stages (black at the far
sides), the stage ends, the objects' margins, the HUD at the edges.

## Steps 2 and 3, first increment: the bridge and the most shared layers

Port work changes nothing under `ps1/`, so the wider stages are drawn by the
port around the game's own C. Steps 2 and 3 go in together: side tiles
without the stage ends would show past the playfield there.

**The bridge (`gpu.c`).**

- `port_gpu_queue_packet(entry, words, len)` (gpu.h) keeps a packet of the
  port's own (Sony's layout without the tag: `len` words, the GP0 commands of
  one packet) for entry `entry` (0..29) of the ordering table that the next
  DrawOTag draws. The walk sends an entry's queued packets when it reaches
  that entry's tag (a packet of length 0 that `port_video_ot_index` names),
  before the packets the game linked there, with the same checks of their
  commands as the game's. The queue empties at the end of that DrawOTag;
  queued packets are kept in host memory (at most 0x10000 words a frame; a
  packet past that is dropped with one line per run).
- `port_gpu_queue_owner(pool_lo, pool_hi)` (2026-10-24): the packets queued
  after it belong to the layer whose own packets lie in that pool, and are
  sent where the game's list would have them had the layer linked them after
  its own: just before the first packet of its pool in the entry (with those
  of layers drawn later in the frame first, the newest owner first, each
  owner's newest first), or at the entry's end. The side cells are owned by
  their layer's pool. Found on Gouki's stage: the haze and the floor's
  skewed quads share an entry, and the floor's side cells, sent after the
  haze's, covered them, so the haze ended at the 384 ("Akuma's stage haze
  only covers the area corresponding to the 4:3 AR"; its map row repeats
  four tiles over the whole width).
- **The stage ends.** During a fight (the main layer's active flag, at
  `main_layer` 0x801aa5d4, is set and its range is valid) with widescreen on,
  the walk moves every packet that is not the HUD's by the anchor `a`:
  `cam` is the s16 at `main_layer + 0x12` (the camera's left edge as the
  game draws it, `box_margin[0]`), `min` and `max` the s16s at +0x44 and
  +0x46; the wide view's left edge is `left = clamp(cam - 64, min, max - 128)`
  (the middle of the two when `max - min < 128`), and `a = cam - 64 - left`,
  from -64 at the left end to +64 at the right end. The HUD's packets are told
  apart as for `center_ui` (`port_ui_packet_kind` in `ui_layout.h`) and keep
  their place (centred). Outside a fight, or with widescreen off, `a` is 0.

**The side tiles (`port/src/wide_stage.c`).**

- For each stage function of the three forms most shared, an override
  (`port_wide_overrides`, always placed; with widescreen off it only runs the
  game's C) runs the game's own C (its implementation, found by name) and
  then, with widescreen on, makes the tiles of the columns that C does not
  draw and queues them for the layer's entry (`Chan.ot_index`):
  `draw_tile_layer_0_16px` (13 stages), `draw_tile_layer_1_16px` (6) and
  `draw_tilemap_layer_32px_9x13` (13).
- The columns: the C draws k = 0..24 (16 px) or 0..12 (32 px) from the
  layer's scroll; the maker adds the columns left of k = 0 and right of the
  last whose screen x falls in -128..512 (what the anchored view can show),
  with the same arithmetic per column as the C (map address, blank tile,
  position, u, v, clut, the texture page word), and the static words of a
  packet (the mode word, the sprite's code and colour) taken from the first
  record of the function's own pool, which the stage's init fills.
- The maker is checked against the game's C: for k = 0..24 (0..12) on a made
  -up layer and map it gives the packets the C writes into its pool.

Second increment (2026-10-24): `draw_skewed_tile_layer_quads` and
`draw_tile_layer_sprites_with_partial_row`, ten stages each, the same way (the
skewed form's quads that reach into -128..512 beyond the C's own -11..32).

Third increment (2026-10-24): `draw_tilemap_layer_32px_7x13` (Ken, Nash,
Rose), `draw_tile_map_layer_16px_half_scroll` (Sakura, Vega) and
`draw_tile_map_layer_32px` (Gouki, Sakura, Vega; Gouki's copy scrolls at
full speed and draws 13 records, a shape of its own). The map layers of
twelve stages are then all done (Adon, Chun-Li, Dan, Dhalsim, Gen, Gen's
other, Guy, Ryu, Sagat, Sakura, Vega, Zangief). `draw_object_tile_grid_sprites`
and its kin draw an object's own grid of cells, not a map: they have no side
cells, and widening them is step 4's (the objects alive in the sides).

Fourth increment (2026-10-24): the map layers of one stage each (Birdie's and
Chun-Li's other's `draw_scrolling_tile_map_layer` are two different
functions; Gouki's, Nash's, Ken's, Rose's, Rolento's and Sodom's), most of
them a known shape with other counts or a flag. Every stage's map layers are
then done (72 copies in the table). What is left of the stages are the
per-object grids (`draw_object_tile_grid_sprites` and kin): an object's own
cells at its position, drawn when the object is alive and in its margin; in
the sides of the wide view they are taken out by the game's margin test
(step 4). Seen at the stage ends: black where such objects stand (Sagat's
pillars, Dhalsim's and Ryu's upper left, a pole of Guy's), and the edges of
Birdie's perspective floor and ceiling.

Step 4 (2026-10-24, `wide_objects.c`): the game's margin test
(`clear_or_set_flag_by_pos_x_limit`) hides an object more than 448 pixels
right of the camera and gives the shown ones of mode 8 a place on the frame's
collision list; drawing reads only the hidden flag, and the object's update
goes on either way. With widescreen on, the objects it hid that the wide view
shows (up to 576) are marked drawn for the draw phase alone
(`select_buffers_and_run_first_setup`) and hidden again after it, so the
collision list and every reader of the flag in the game's logic stay as the
game makes them. The test hides nothing on the left. Projectiles are still
taken out where the game takes them out (state 3 at 448 or 512 right of the
camera): near the left end, where the view reaches 512, one may vanish short
of the picture's edge.

**The art's own ends.** Some slow background layers hold art only for what the
PS1's camera shows of them; at the stage ends the wide view goes past it, and
the picture is black there: Sagat's sky (a block of about 50 x 170 at the far
corner at both ends), a strip of Guy's at the right end, slivers of Ryu's, and
the edges of Birdie's perspective floor and ceiling. Seen in scripted training
fights at both walls of the fifteen stages (2026-10-24). The arcade's layers
may hold more of that art, for the arcade assets.

**Each plane kept inside its painting (step 5, contract).** The arcade's port
(CPS-2 Recomp) draws each background plane moved by as much as its wide view
would reach past its painting, a picture-only change. Measured here on
2026-10-24 (scripted training fights at both walls of the fifteen stages, the
bridge's `wide-video: stage K, camera at its ... end` lines): the main layer
follows the camera; the others move at other speeds (Sagat's far layer 306 to
334 over the camera's 256 to 640), and some move by themselves (Ken's, Chun-Li's,
Adon's and Rose's: their scroll differs two seconds on at the same camera).
The black at Sagat's ends puts its far layer's painting at about 256 to 768,
more than its board view (306 to 718), so a table of board views would clamp
too much; the painting is read from the map instead:

- In each layer override of `wide_stage.c`, after the game's C and the side
  cells: the layer's painted extent around its view, in layer pixels relative
  to its scroll: from the view's left edge leftward the columns that have a
  cell that is not blank in the rows the C draws, down to -256 at most, a gap
  of at most two blank columns not ending it; the same rightward from 384 up
  to 640. Given to the bridge with the layer's pool range and entry:
  `port_gpu_layer_view(pool_lo, pool_hi, entry, art_lo, art_hi)` (gpu.h).
- The bridge (`gpu.c`), each frame: only the plane furthest back (the
  highest entry among the layers given) is corrected, only at the stage ends
  (`a`, the world's anchor, not 0), toward 0 from `a` and only as far as its
  wide view, -64 - shift to 448 - shift, needs to stay inside its painting.
  The game's packets inside that layer's pool and the side cells queued for it
  get that shift; every other packet gets `a` as before (the HUD its own).
- The first form of this (2026-10-24) corrected every plane, in the middle of
  the stage too, and split a short painting between the sides. The owner saw
  Zangief's lamps and pillars move against the rest: a plane in front has
  blank columns where the planes behind show, and gaps between its pieces, so
  its "painting" read short and it was moved where nothing was missing. Only
  the plane furthest back can show black, and only at the ends.

**Each plane anchored at its own speed (step 6, contract).** The owner saw,
on Zangief's stage near the right end, the lamps, the pillars and the big
screen drift right while the floor and the cage held still (2026-10-24, two
screenshots): over the camera's last 64 pixels the view stops, and the one
anchor `a` moved every plane as the main layer, but a plane that scrolls at a
fraction f of the camera's speed moved only f of the camera's step, so it went
the other way by (1 - f) of it. A plane is moved by f * a instead: as if the
camera stood where the view stopped.

- The bridge measures each layer's f during the fight, from frame to frame:
  with the camera moved by dc and the layer's scroll by ds (wrapped into
  -256..255), less the layer's own drift r (its average step while the camera
  stands still: clouds, water), f is the least-squares ratio of (ds - r) to dc
  over the recent frames (a forgetting factor), 1 until enough motion is seen,
  kept inside 0..3. Steps over 64 are left out. A new stage, or the main
  layer off, starts over.
- `port_gpu_layer_view` names the layer's record too (its address, so that
  the bridge reads its scroll at +0x12 and its entry at +0x8a). For a
  registered layer: its pool's packets and its queued side cells get
  round(f * a). For a layer behind the camera's plane (f < 1), every other
  packet of its own entry (its ot_index) gets it too: the objects drawn at its
  depth are its props (Zangief's big screen); a plane in front (f >= 1) shares
  its entry with nothing else for that.
- The plane furthest back is then kept inside its painting as in step 5, now
  from f * a, and only within the span from 0 to a (in the middle of a stage
  nothing moves).

Checking a stage in the game: training mode reaches the fifteen stages of
the select screen's three rows by an input script (down to TRAINING MODE,
square on Ryu, the dummy by the cursor, square, circle at the training menu);
the stage is the dummy's. The hidden characters' stages (Gouki, Dan, Vega)
need their codes.

Not yet: the forms of one stage, objects in
the sides (step 4), banners and other screen-space objects at the stage ends
(they move with the world there; they are told apart later).

## The stage's ends (`wide_ends`, contract)

`[experimental] wide_ends = centred | anchored | narrowed` (default
`narrowed`; `--wide-ends MODE`), read only with widescreen on.

- **centred**: the wide view is the game's view with 64 columns at each side,
  everywhere; no plane is moved (the anchor is 0), so each scrolls as the game
  scrolls it. At the camera's ends the outer columns show past the stage's art
  (black, or the map's wrapped cells).
- **anchored**: steps 3, 5 and 6 above (the view shifted toward the stage's
  inside at its ends, each plane by its own speed).
- **narrowed** (`port/src/wide_walls.c`): the view is centred as above, and
  the game's rules take the screen as the wide view's 512 columns:
  - every stage's camera range is 256..640 (the 384 view's left edge,
    `init_sprite_position_wide`); main_layer's is cut by 64 at each end,
    320..576, so the wide view ends where the stage does, and the walls of the
    world stay at 256 and 1024;
  - the fighters are held inside the wide view (its left edge, 512 wide) in
    place of the 384 (`clamp_pos_x_to_bounds_and_push_other`,
    `push_both_objects_inside_bounds`), so they reach the picture's edges and
    can stand up to 512 apart;
  - the camera follows them with the game's margins of 96 from the wide
    view's edges, and centres them from a span of 320 (512 less both margins)
    in place of 192 (`update_camera_x_from_player_span`).

  The four are overrides of the game's functions, the game's C with those
  numbers; with any other setting they run the game's C.
  `port/tools/test_wide_walls.py` compares each with the game's C at an
  extra of 0 over 4000 fixtures, checks the contract at 64, and checks that
  every other setting calls through. This is the design of CPS-2 Recomp's
  widescreen (its default; its "Invisible walls" is `anchored`).

Not yet in narrowed: moves and objects that test the screen's edge in the
characters' own code (Vega's wall climb, the wall jumps, objects placed at
a margin) still use the 384; the sound's stereo place too.

## References

- psxrecomp's widescreen notes (`docs/WIDESCREEN.md`, `NATIVE_WIDE_PLAN.md` of
  github.com/mstan/psxrecomp): a host-side wide canvas with the draw offset
  moved, a view anchored toward a room's inside at its ends, and Capcom's
  16-pixel tile layers drawn over the wider view (Mega Man X6). Read for
  ideas; no code of it is used (its licence is not stated).
- CPS-2 Recomp (github.com/tmcleroy/cps2recomp, GPL-3.0-only, the arcade
  set) has widescreen for Alpha 2 at 512 x 224. Its "Invisible walls" option
  is this page's design: the wide picture, the board's rules, and near a
  stage's end the view kept inside the stage. Its notes say that the arcade's
  scroll layers hold the stage's art for the camera's whole range on all
  eighteen stages, so the wide picture's sides are the game's own art. Read
  for ideas only: its licence does not allow copying it here.
- Street Fighter Alpha 3 Recompiled (psxrecomp) has no widescreen yet; its
  `analysis/widescreen_sites.toml` holds scanner candidates only.
