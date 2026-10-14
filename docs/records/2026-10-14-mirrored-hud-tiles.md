## Mirrored HUD tiles keep the console's sampling (2026-10-14)

The owner reported that with `pixel_perfect_ui` on P2's life bar looked
uneven, and only P2's.

Cause, from the game's tile maps and from pictures of the video memory: P2's
bar is P1's tiles with its two end tiles mirrored. The frame's empty part is a
one-pixel checker with transparent texels. The console's texel at a pixel
counts from the vertex, so a mirrored side with 16 texels shows texels 16 down
to 1; PsyZ's `FixupFlipUV` does the same on the original path. The one-to-one
grid skipped that shift and mirrored exactly (15 down to 0), which turns the
checker one column out of phase on the mirrored tiles: a doubled column where
P2's right end meets the middle of the bar. A mirrored side with 15 texels
already shows 15 down to 0 on the console and is unchanged.

The change: `port_video_pixel_grid` samples a mirrored 16-texel side one texel
on (and leaves a tile on the original path when that would pass the end of the
texture page). `build.sh` now also keys the library on `port/src/video.h`,
which `psyzbuild.py` copies into the library: before, a change of that header
alone left the library as it was.

Evidence, on the branch's tree: `test_video.py`, `test_ui_layout.py` and
`test_build_sh.py` pass; nine one-line changes of the new rule, each run alone
against `test_video.py` under limits, were all noticed, and the new build-script
control fails with the key line taken out. One hidden, silent arcade fight with
the program of this tree, dumped every 5 seconds: P2's interior has no doubled
dither column (none in rows 17 to 22), and P2's right end has its border where
the original sampling has it (game x 361 to 364). P1's bottom-right corner now
sits one pixel right of its top row, as it does with the original sampling; the
one-to-one grid had aligned it. Not run: a window, which is the owner's to see.
