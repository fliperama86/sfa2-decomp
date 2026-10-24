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

## The owner's decisions (2026-10-23)

- **Gameplay stays the original's.** The fighters' walls, the camera and the
  spacing are the PS1's (and the arcade's): the play area stays 384 pixels
  wide and the wider picture only shows more of the stage at its sides. The
  fighters stop 64 pixels short of a screen edge.
- **The HUD stays centred**, where it is now. Spreading it to the edges may
  come later as an option.

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

Not in this increment: the other layer forms (`draw_skewed_tile_layer_quads`,
`draw_tile_layer_sprites_with_partial_row`, the one-stage forms), objects in
the sides (step 4), banners and other screen-space objects at the stage ends
(they move with the world there; they are told apart later).

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
