# Optional graphics and settings

The normal port has one build and one settings model. Since 2026-10-23, by the
owner's decision, every experimental setting is on by default but the arcade
intro, which needs an intro pack. A build without PsyZ (the controls, which open
no window) keeps the graphics settings off by default, since it cannot draw
them; the build tools compile `settings.c` with PsyZ's define for that. Each
setting can still be turned off in the settings file or on the command line.
No arcade ROM or replacement artwork is required for these options.

```ini
[experimental]
pixel_perfect_ui = true
arcade_video = true
center_ui = true

[display]
filter = sharp_bilinear
```

The file has two sections: `[experimental]` (the switches of the changes below, and
`settings_menu`, `widescreen`) and `[display]` (`screen`, `window_size`, `fullscreen_size` and
`filter`). [settings-menu.md](settings-menu.md) is the contract of the whole file.

`pixel_perfect_ui` corrects **super-meter and life-frame tile sampling**. It maps
each 16x16 source tile one-to-one onto the native pixel grid, including mirrored
tiles, mixed UV spans and the last tile of a texture page. Mirrored tiles are
exact mirrors, which keeps every frame border where its artwork puts it. P2's
two life-frame caps are P1's mirrored, and a mirrored 16-pixel tile turns the
bar's one-pixel checker out of phase; the bridge draws each of them as two
halves, the cap's outer half mirrored and its inner half (the bar's repeating
body) unmirrored, so the checker runs on unbroken. The life-frame artwork
has a six-row interior; selected yellow and delayed-damage fills render at that
height, keeping their original X, widths, colors and health logic. Empty fills
remain empty. The super-meter frame's interior is one row higher on that grid
and three, four and five rows tall by level, so the meter's fills are drawn from
Y 212 at those heights in place of Y 213 and three, four and four rows; this one
is done in the bridge's copy of the packet, with no marker in the record.
Original game C, packet geometry and disc assets stay unchanged:
the bridge adds markers only to known HUD records/lists, and the backend adjusts
decoded render geometry. This is not a global correction for all UI or scaled/
rotated graphics, and it does not import assets or recenter the HUD.
Unmarked packets retain the original conversion path. The backend additionally
gates both corrections on the setting, so stale markers alone cannot activate
them when it is disabled. Frame-list selection uses the two initialized tables,
not the address storing their pointer. The earlier selector mistake and its
correction are preserved in the [selection correction record](../docs/records/2026-10-12-hud-selection-correction.md).

`arcade_video` independently exposes the **384x224 native canvas**, displayed at
4:3, instead of the original 368x240 presentation. It applies only to the game's
384x240 non-RGB24 display mode. Movies and other modes retain their original
presentation. It does not recenter the HUD or provide true 16:9. Fractional
window scaling can still give uneven physical pixel sizes even with correct
native texture sampling. Changing the canvas does not turn on pixel-perfect UI.

`center_ui` is a **WIP common-UI layout** option. When the wider game canvas is
active, it centers the original 368-wide UI coordinates in 384 pixels by moving
selected copies eight pixels right. It does not stretch or move Y, the fight
scene or camera. It covers resident HUD tiles/fills/strips and common text,
including shared text used by menus. It has no effect without the wider canvas
or in movies/other display modes, and does not turn on sampling corrections.
Screen-specific menu/overlay artwork is not covered yet, so this is not complete
all-UI or arcade-placement equivalence. Further small reviewed increments use
the same default-off flag. True16:9 and edge anchoring for that width are future
work. Original game RAM/geometry stays unchanged; translation is in copied SDK
packets. Both sampling and layout compose without changing health-bar widths.

`filter = sharp_bilinear` (a value of `[display] filter`, which is `nearest`,
`bilinear` or `sharp_bilinear`; before the settings menu it was the key
`[experimental] sharp_bilinear`) changes only how the finished picture is **stretched to the
window**. The picture is first enlarged by the largest whole factor of each side
that fits its place in the window, with nearest sampling, and the result is then
stretched to that place with linear sampling. Pixels keep hard edges, and where
the window is not a whole multiple of the picture the uneven pixel sizes of
nearest sampling become a thin blend at the pixel borders. When the place is a
whole multiple on both sides, and with `filter = nearest`, the picture is drawn
with nearest sampling alone, as before; `bilinear` stretches it with linear
sampling alone. It applies to every display mode,
movies included, and works with or without the other options; nothing of the
game's drawing changes. How it looks is the owner's to judge: no check here
reads the window's pixels.

## Configuration

At startup the port reads `settings.ini` in the working directory. A missing
default file uses the defaults above. `--settings FILE` explicitly selects a
file; missing or invalid explicit files refuse startup. The program writes the
file only with `settings_menu` on, where the settings menu saves its changes to
the file that was read ([settings-menu.md](settings-menu.md)); with it off no
file is ever written. Copy/edit the supplied `port/settings.ini`, or give its
path. `./build.sh -r` gives the program `local/settings.ini` (ignored by Git,
copied from `port/settings.ini` when it is missing) unless `SFA2_SETTINGS`
names another file.

The old key `[experimental] sharp_bilinear = true|false` is still read, as
`filter = sharp_bilinear|nearest`; a file with both keys is refused, and saving
never writes the old key. A size (`window_size`, `fullscreen_size`) is `WxH` in
decimal digits, a lowercase `x`, no spaces, no leading zero, each side 320 to
16384; `fullscreen_size` may also be `desktop`.

CLI overrides address these same settings, with the last override winning:

| Setting | Enable | Restore original |
| --- | --- | --- |
| UI sampling | `--pixel-perfect-ui` | `--ps1-rendering` |
| Native canvas | `--arcade-video` | `--ps1-video` |
| Common UI position | `--center-ui` | `--ps1-ui-layout` |
| Picture filter (`[display] filter`) | `--sharp-bilinear` (filter = sharp_bilinear) | `--nearest-scaling` (filter = nearest) |
| Settings menu (`[experimental] settings_menu`) | `--settings-menu` | `--no-settings-menu` |
| Widescreen (`[experimental] widescreen`, default off; [widescreen.md](widescreen.md)) | `--widescreen` | `--no-widescreen` |
| Loading waits (not graphics) | `--fast-loads` | `--ps1-loading` |
| Memory cards (not graphics) | `--memory-card` | `--no-memory-card` |

`--volume PERCENT` (0 to 100, default 100) sets the gain of the sound output. It is a command-line option only (the settings file takes
true or false values), it is not a graphics setting, and `--show-settings` reports it on a line after the settings. It does not change a
file written by `--audio-dump`. It needs the patched PsyZ (`port/psyz.patch`).

Overrides apply after loading, regardless of where `--settings` appears. They
change what the program uses, never what the menu saves: the menu saves the
file's own values with its change.
The original-sampling option selects the prior port path, not a claim of
pixel-identical PS1 GPU output.
`--show-settings` reports resolved values, both sections in the file's own form
(what a save would write), then the `volume = N` line, and exits before any
disc, game, GPU/window or audio initialization. The settings menu uses the
same typed C load/save API.

The INI subset accepts LF/CRLF, a UTF-8 BOM, whitespace and `#`/`;` comments.
Values are exactly `true` or `false`. Unknown/duplicate keys, unknown sections,
NUL bytes and overlong lines are rejected. Loading is transactional. Saving
validates the model and atomically replaces the file through an exclusive
same-directory temporary file, keeping the prior file on failure.

## Build and validation

Rebuild PsyZ with the existing `psyzbuild.py` and updated `port/psyz.patch` before
linking. Its metadata advertises `port_video_api = 5`; the host build refuses
stale SDK caches before compiling any game units. The tool copies the
project-owned `video.h` policy into the dependency
build copy, never modifying the source submodule. Patch context and changes
remain under MPL 2.0 as stated in the patch. No dependency snapshot is published.
The experimental paths currently require the patched SDL_GPU backend. The
optional GL backend keeps disabled behavior and explicitly rejects enabling
these unsupported options.

Fast tests, no private input or running game:

```sh
python port/tools/test_settings.py
python port/tools/test_video.py
python port/tools/test_psyzbuild.py
python port/tools/test_ui_layout.py
```

`video_gpu_test.c` is a separate generated-pixel fixture for an already built
SDK. It opens a short-lived window, never initializes audio or reads a disc,
checks mirrors/transparent 4bpp/direct textures, mixed-span life tiles, full/
partial/damage/empty fills and unmarked/flag-off negative controls,
and checks original canvas sizes before/after opting in. Its wide case
(port_video_api 5, [widescreen.md](widescreen.md)) draws rectangles left of and
inside the draw area and reads the wide canvas back, also at internal resolution 2;
it checks clears, the stale canvas after a LoadImage or MoveImage, and the off state.
It is built with the compiler flags of the runtime files, `-D__psyz`, PsyZ's
include folder and the libraries of `psyz.json`, and runs in under a second
(the case sets PsyZ's frame limiter off). Its expectations do
not come from game screenshots. `video_bridge_test.c` links the real GPU bridge
and an already built SDK to check selection against generated mapped RAM. It
exits without video/audio initialization and needs no game or disc. Tests do not
prove full-game equivalence.

`test_ui_layout.py` checks synthetic packet geometry, the actual copied-packet
seam with a mock SDK sink, and disabled/unsupported behavior without an SDK.
`ui_layout_gpu_test.c` separately links the whole GPU bridge and a built SDK:
generated HUD/text/world pixels, both frame tables and all eight flag states,
unchanged source RAM and an other-mode gate. Its readback checks placement and
color classes, not exact texture RGB quantization. It is windowed and silent,
requires no game/disc, and does not repeat the earlier backend fixtures.

## Next reviewed increments

Super/life sampling, the independent canvas and common-UI centering are
integrated. Extend the WIP layout flag to separately identified screen-specific
artwork in small reviewed increments. True16:9 is not implemented.
