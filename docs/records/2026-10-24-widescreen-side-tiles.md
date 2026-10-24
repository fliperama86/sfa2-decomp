## Widescreen, side tiles and stage ends: first increment (2026-10-24)

Steps 2 and 3 of `port/widescreen.md`, together: side tiles without the
anchored view would show past the playfield at the stage ends.

- `gpu.c`: `port_gpu_queue_packet` keeps the port's own packets for an entry
  of the ordering table, sent when the walk reaches that entry (before the
  game's there) and dropped at the end of the walk. During a fight with
  widescreen on, every packet but the HUD's is moved by the anchor, from -64
  at the camera's left end to +64 at its right end.
- `library.c`: `port_function_impl` finds a function's C by name, for an
  override that runs it.
- `wide_stage.c`: overrides of the 32 stage copies of
  `draw_tile_layer_0_16px`, `draw_tile_layer_1_16px` and
  `draw_tilemap_layer_32px_9x13` run the game's C and then queue the columns
  it does not draw, for screen x -128 to 512, with the C's own arithmetic and
  the static words of the function's own pool. The copies were checked to
  hold identical code.

What ran: `test_wide_stage.py` (the maker against the real C of each form on
64 fixtures each, the side columns, negative controls, the table against
`ps1/src`), `test_ui_layout.py` (the queue and the anchor through the real
walker), `test_video.py`, `test_settings.py`, `test_menu.py`,
`test_hostkernel_start.py`, `test_hostlaunch.py --linux` (all but its
card-routines case, which fails on main as well); the Linux program with the
user's disc and an input script into a fight on Birdie's stage, whose wide
pictures show the stage's walls and back over the 512 columns, the view
anchored near the left end with the HUD centred. Not covered yet: the
perspective, skewed and one-stage layer forms, objects in the sides, banners
at the stage ends; the Windows and Mac programs were not run.
