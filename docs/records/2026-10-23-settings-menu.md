## PC settings menu (2026-10-23)

The owner asked for "a PC settings menu using the same structure and fonts,
but in a way that it won't be lost after the assets retarget", then "go
ahead, build the settings menu", with this first content: window or
fullscreen, resolutions, scaling, "all saved automatically in the settings
file". The contract is `port/settings-menu.md`. It is behind
`[experimental] settings_menu`, default off.

- Survives the arcade assets: the menu reads the game's font page and
  palettes from the video memory, where the game's text routine reads them,
  so it follows whatever loaded them. The palettes and layout were matched
  exactly on a picture of the game's mode select screen.
- `settings.c`: a `[display]` section (`screen`, `window_size`,
  `fullscreen_size`, `filter`); `filter` replaces `[experimental]
  sharp_bilinear`, which is still read. `port_settings_write` is the one
  writer of the file's form, for saves and `--show-settings`.
- `menu.c` (pure: state, value lists, picture) and `menu_run.c` (the pause,
  keys, display and saves), called at the end of each vblank by `kernel.c`.
  The clock leaves the menu's time out. The menu shows a 384 x 240 display
  of its own: in the first live run the menu, drawn in the game's own
  display, could not open on the title screen, which is 320 wide.
- `port/psyz.patch` (port_video_api 4): the filter (nearest, bilinear,
  sharp bilinear), the screen (window size, exclusive fullscreen at a mode,
  borderless) and the host keys (F1, Escape, F4, a gamepad's guide button).
- `input.c`: the input script presses the host keys (`f1 escape f4 guide`)
  and its clock counts the menu's frames, so a script drives the menu. An
  earlier test sent keys to the desktop with `wtype`; they reached another
  window, and that way is not used.
- `build.sh -r` uses `local/settings.ini`, copied from `port/settings.ini`,
  so that the saves never change a tracked file.

What ran, on Linux, in the worktree of this change: `test_settings.py`,
`test_menu.py`, `test_input_keys.py`, `test_video.py`, `test_psyzbuild.py`,
`test_build_sh.py`, `test_hostkernel_start.py`, `test_hostiopage.py
--linux`, `test_hostlaunch.py --linux` (all but its card-routines case,
which fails on main as well), the PsyZ build and the Linux program's build.
A run of the program with the user's disc and an input script opened the
menu on the title, changed the window to 1440x1080 and the filter to
nearest, saved both, closed it, opened it again and quit from it; its
video memory pictures show the menu. Not run: exclusive fullscreen and
borderless (no check opens a fullscreen window), a gamepad's guide button,
the Windows and Mac programs, and `test_hostpads.py`, which needs the
Windows tool chain (its list of buttons is updated).
