# Optional graphics and settings

The normal port has one build and one settings model. Both experimental
graphics options default off, preserving the existing PS1 canvas and sampling.
No arcade ROM or replacement artwork is required for these options.

```ini
[experimental]
pixel_perfect_ui = false
arcade_video = false
```

`pixel_perfect_ui` corrects **super-meter and life-frame tile sampling**. It maps
each 16x16 source tile one-to-one onto the native pixel grid, including mirrored
tiles, mixed UV spans and the last tile of a texture page. The life-frame artwork
has a six-row interior; selected yellow and delayed-damage fills render at that
height, keeping their original X, widths, colors and health logic. Empty fills
remain empty. Original game C, packet geometry and disc assets stay unchanged:
the bridge adds markers only to known HUD records/lists, and the backend adjusts
decoded render geometry. This is not a global correction for all UI or scaled/
rotated graphics, and it does not import assets or recenter the HUD.
Unmarked packets retain the original conversion path. The backend additionally
gates both corrections on the setting, so stale markers alone cannot activate
them when it is disabled.

`arcade_video` independently exposes the **384x224 native canvas**, displayed at
4:3, instead of the original 368x240 presentation. It applies only to the game's
384x240 non-RGB24 display mode. Movies and other modes retain their original
presentation. It does not recenter the HUD or provide true 16:9. Fractional
window scaling can still give uneven physical pixel sizes even with correct
native texture sampling. Changing the canvas does not turn on pixel-perfect UI.

## Configuration

At startup the port reads `settings.ini` in the working directory. A missing
default file uses disabled defaults. `--settings FILE` explicitly selects a
file; missing or invalid explicit files refuse startup. No settings are written
automatically. Copy/edit the supplied `port/settings.ini`, or give its path.

CLI overrides address these same settings, with the last override winning:

| Setting | Enable | Restore original |
| --- | --- | --- |
| UI sampling | `--pixel-perfect-ui` | `--ps1-rendering` |
| Native canvas | `--arcade-video` | `--ps1-video` |

Overrides apply after loading, regardless of where `--settings` appears.
The original-sampling option selects the prior port path, not a claim of
pixel-identical PS1 GPU output.
`--show-settings` reports resolved values and exits before any disc, game,
GPU/window or audio initialization. Options are startup settings; a live UI
has not been implemented. The future UI uses the same typed C load/save API.

The INI subset accepts LF/CRLF, a UTF-8 BOM, whitespace and `#`/`;` comments.
Values are exactly `true` or `false`. Unknown/duplicate keys, unknown sections,
NUL bytes and overlong lines are rejected. Loading is transactional. Saving
validates the model and atomically replaces the file through an exclusive
same-directory temporary file, keeping the prior file on failure.

## Build and validation

Rebuild PsyZ with the existing `psyzbuild.py` and updated `port/psyz.patch` before
linking. Its metadata advertises `port_video_api = 2`; the host build refuses
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
```

`video_gpu_test.c` is a separate generated-pixel fixture for an already built
SDK. It opens a short-lived window, never initializes audio or reads a disc,
checks mirrors/transparent 4bpp/direct textures, mixed-span life tiles, full/
partial/damage/empty fills and unmarked/flag-off negative controls,
and checks original canvas sizes before/after opting in. Its expectations do
not come from game screenshots. Tests do not prove full-game equivalence.

## Next reviewed increments

Super/life sampling and the independent canvas are integrated. Next is UI
centering/anchoring for the wider canvas, kept small, reviewed and opt-in behind
a settings flag. True16:9 is not implemented.
