## Widescreen, step 1: the wide canvas (2026-10-23)

The owner: "widescreen, that is the hard one", then the two decisions of
`port/widescreen.md` (the gameplay and the HUD stay the original's). Research
found no widescreen of the PS1 version; psxrecomp's notes and CPS-2 Recomp's
arcade Alpha 2 (its "Invisible walls" option is this design) were read for
ideas, no code taken.

- `port/psyz.patch` (port_video_api 5): every flush whose drawing area is a
  384 x 224 frame area is drawn a second time on a canvas of 512 x 224, 64
  columns over (the draw offset of the shader moved, not the viewport, which
  would clip the left columns); clears follow, images into the area mark the
  canvas stale; the canvas is shown at 16:9 when it is good, else the frame
  as before. `Psyz_Sfa2ReadWide` reads it back.
- The setting `[experimental] widescreen` (default off), `--widescreen`, a
  WIDESCREEN row in the settings menu and 16:9 window sizes there, and
  `PREFIX_wide_NN.ppm` pictures with `--dump-vram`.

What ran: the PsyZ build; `test_video.py` (with the wide fit), the generated
-pixel GPU fixture with its new wide case (a short window), `test_psyzbuild.py`,
`test_settings.py`, `test_menu.py`, `test_input_keys.py`,
`test_hostkernel_start.py`, `test_hostlaunch.py --linux` (all but its
card-routines case, which fails on main as well); the Linux program with the
user's disc and an input script into a fight, whose wide pictures show the
select screen centred with black sides, the world map wider, and the fight at
512 columns with the HUD centred and black where the game draws nothing.
Not run: the Windows and Mac programs, the window's picture itself (the
pictures are the canvas read back).
