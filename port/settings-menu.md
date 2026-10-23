# PC settings menu

An in-game menu of the PC program's own settings, drawn with the game's font
and in the style of its menus, behind the setting `[experimental]
settings_menu` (default off). Asked for by the owner on 2026-10-23 ("a PC
settings menu using the same structure and fonts, but in a way that it won't
be lost after the assets retarget"), with this first content: window or
fullscreen, resolutions, scaling (sharp bilinear, nearest and the like), "all
saved automatically in the settings file". Widescreen and more gamepads come
after it.

This page is the contract of the first increment. Nothing here changes the
game's C, `ps1/`, or what a run does with the setting off.

## Why it survives the arcade assets

The menu holds no picture of its own. It reads the font and the palettes from
the video memory, at the places where the game's text routine
(`draw_text_items`) reads them: the 4-bit font page at (0x3c0, 0x100) and
palette p at (0x70, 0x1e0 + p). Whatever loads them, the PS1's `ASCII.PAC` or
the arcade loader's rebuild of it, has to put them there for the game's own
text to show, and the menu follows. Its strings are project text in
`menu.c`.

## Settings file

`port/src/settings.h` is the typed model. The file gains a `[display]`
section; `[experimental]` keeps its keys but `sharp_bilinear`, and gains
`settings_menu`.

```ini
[experimental]
pixel_perfect_ui = true
arcade_video = true
center_ui = true
fast_loads = true
memory_card = true
arcade_intro = false
arcade_assets = false
settings_menu = false

[display]
screen = window            # window | fullscreen | borderless
window_size = 1280x960     # WxH, each side 320 to 16384
fullscreen_size = desktop  # desktop | WxH
filter = sharp_bilinear    # nearest | bilinear | sharp_bilinear
```

- Defaults: as today's behaviour. `screen = window`, `window_size =
  1280x960` (PsyZ's own first size), `fullscreen_size = desktop`, `filter =
  sharp_bilinear` with PsyZ and `nearest` without it (as `sharp_bilinear`
  was), `settings_menu = false`.
- The parser stays as strict as it is: an unknown section or key, a key in
  the wrong section, a repeated key, a bad value, a NUL byte or an overlong
  line refuses the whole file. A size is `W` `x` `H` in decimal digits, a
  lowercase `x`, no spaces inside, no leading zero.
- The old key `[experimental] sharp_bilinear = true|false` is still read, as
  `filter = sharp_bilinear|nearest`. Both keys in one file is a refusal
  ("sharp_bilinear and filter both given"). Saving never writes it.
- Saving writes both sections in the order above, every key.
- `port_settings` is what the program uses (the file, then the command
  line). `port_settings_file` is what the file gave (the defaults when it was
  missing). `port_settings_path` is the file read (`settings.ini` in the
  working directory unless `--settings FILE`).

Command line: `--sharp-bilinear` and `--nearest-scaling` stay and set
`filter`; `--settings-menu` and `--no-settings-menu` set `settings_menu`.
`--show-settings` prints both sections in the file's form, then the volume
line.

## Saved automatically

A change made in the menu (or with F4, below) is applied at once and saved at
once: the changed `[display]` keys are copied into `port_settings_file`, and
that is saved to `port_settings_path` with the existing atomic save. So a
command-line override of another key never ends up in the file. When the file
was missing it is created. A failed save prints one line
(`settings: not saved: ...`) and the program goes on. Nothing is saved
while the menu is off, so with the setting off no file is ever written, as
before.

`build.sh -r` now gives the program `local/settings.ini` (ignored by Git)
unless `SFA2_SETTINGS` names another file, and copies `port/settings.ini`
there when it is missing, so that the menu's saves never change a tracked
file.

## Keys

With `settings_menu` on, PsyZ's own Escape (end the program) and F4 (toggle
fullscreen) are taken over by the port (`port_gpu_set_host_keys`):

- F1, Escape or a gamepad's guide button opens the menu, and closes it.
- F4 switches between the window and fullscreen (the last of `fullscreen`
  or `borderless` used, `fullscreen` at first), and saves it.
- Ending the program: the menu's QUIT GAME, or closing the window.

With the setting off every key does what it did.

The input script (`--input FILE`, `port/src/input.c`) presses these keys
too: `f1`, `escape`, `f4` and `guide` are buttons of its lines, and a down
line (or a repeat's press) of one is that key's press. Its clock counts the
menu's frames as well as the vblanks, so a script can open the menu, move
in it and close it, with no key sent to the desktop. The pictures of
`--dump-every` and the watchdog of `--watchdog` count the menu's frames in
the same way.

The menu opens at the end of a vblank when the game has drawn, its display
is on, the game's font page has been loaded and no fast load is running. A
press when that is not so is kept until it is (a second press drops it).

## The menu

While it is open the game is paused: no vblank reaches the game, and the
sound stops with it. The menu shows a display of its own, 384 x 240 (the
size of the game's menus and fights), at the place of the game's display:
the game's last picture is behind it, stretched to that size where the
game's display is another (the title's is 320 x 240), dimmed to an eighth,
and black for a 24-bit display (a movie). On closing, the video memory under
it and the game's display are put back as they were and the clock skips the
time it was open, so the game goes on where it stopped. Found in the first
live run: with the menu drawn in the game's own display, it could not open
on the title screen.
The pad state that the game sees is not touched: the menu waits for every
button to be released (at most half a second) before it returns.

Layout, in the menu's display (taken from the game's mode select screen: its
16 x 16 font, 16 pixels a character, rows 24 pixels apart):

```
                PC SETTINGS               title, palette 0x1e, centred, y 24
  DISPLAY             WINDOW              y 56
  RESOLUTION        1280X960              y 80
  FILTER      SHARP BILINEAR              y 104

               RETURN TO GAME             centred, y 152
                 QUIT GAME                centred, y 176
  UP/DOWN SELECT  LEFT/RIGHT CHANGE  F1 BACK     8 x 8 font, palette 0x1a, centred, y 216
```

Labels start at x 16; values end at x 352. The selected row (label
and value) has palette 0x10; on the other rows the label has 0x1a and the
value 0x16 (the game's blue, so that a long value does not read as part of
its label); an unavailable row has 0x1b.

Values:

- DISPLAY: WINDOW, FULLSCREEN, BORDERLESS.
- RESOLUTION, window: 640X480, 960X720, 1280X960, 1440X1080, 1600X1200,
  1920X1440, 2560X1920, 2880X2160, those that fit the desktop (all when its
  size is unknown, 640X480 when none fits), plus the current size when it is
  not one of them, in order of area.
- RESOLUTION, fullscreen: DESKTOP, then the display's fullscreen sizes,
  largest first, plus the current size when the display does not list it
  (after DESKTOP).
- RESOLUTION, borderless: DESKTOP, unavailable (grey, the cursor skips it).
- FILTER: NEAREST, BILINEAR, SHARP BILINEAR.

Pad (the keyboard through PsyZ's keys: arrows, X cross, D circle, S
triangle, Return start):

- Up/down: the previous/next selectable row, wrapping. Left/right: the
  previous/next value, wrapping. Held, a direction repeats after 18 frames,
  then every 5.
- Cross or start: on a setting, the next value; on RETURN TO GAME, close;
  on QUIT GAME, end the program as a closed window does (status 0, with the
  line `stop: quit from the settings menu`).
- Circle or triangle: close.
- Buttons held when the menu opens do nothing until released.

Drawing (`menu.c`, pure):

- A 16 x 16 glyph of code c (32 < c < 128): v = c - 0x20, its texels from
  u = ((v & 7) << 4) | 0x80, v' = (v & 0x78) << 1 of the font page. An 8 x 8
  glyph of code c: u = (c & 15) << 3, v' = (c & 0xf0) >> 1. (The game's
  `add_glyph_16x16_prim` and `add_glyph_8x8_prim`.) A space draws nothing.
- Texel (u, v') of the page: halfword (0x3c0 + u / 4, 0x100 + v') of the
  video memory, index (halfword >> ((u & 3) * 4)) & 15; its colour is entry
  `index` of the palette, and an entry of 0x0000 is transparent.
- Dimming: each channel shifted right by three, bit 15 cleared. The game's
  picture is stretched by nearest sampling: menu pixel (x, y) is the game's
  display pixel (x * w / 384, y * h / 240).

## Graphics side

`port/src/gpu.h` has the port's calls, `port/src/video.h` PsyZ's
(`Psyz_Sfa2*`, port_video_api 4), `port/psyz.patch` their SDL_GPU code. The
OpenGL backend gets no-ops that link (SetScreen and the modes return -1 or 0,
TakeKeys 0, SetHostKeys nothing).

- SetFilter replaces SetSharpBilinear: nearest is the one nearest blit as
  before, bilinear is one linear blit, sharp bilinear is today's path.
- SetScreen before the window exists keeps the request; the window then opens
  at the window size (in place of PsyZ's 1280 x 960) or goes fullscreen as it
  is shown. PsyZ's resize to 1280 x 960 at a display size change uses the
  kept window size instead. Exclusive fullscreen uses the closest mode of the
  window's display to the size (the desktop's for 0, 0); borderless sets no
  mode. PsyZ's own fullscreen flag follows, so its F4 (when the port does not
  take the keys) still works from any state.
- With host keys on, Escape, F4 and F1 key presses (not repeats) and the
  guide button set bits for TakeKeys and do nothing else.

## Not in this increment

Widescreen, gamepad fixes, PsyZ's internal resolution, the other
experimental settings in the menu, a window size followed from the user's
own resizing. The Mac program compiles the same files (its runtime is
`port/src/*.c`), but the menu was not tried there.
