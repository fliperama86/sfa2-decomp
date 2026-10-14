## Super-meter fills follow the one-to-one frame (2026-10-13)

The owner reported, with a picture of a fight, that the super bar was filled
one pixel too low. His settings have `pixel_perfect_ui`, `arcade_video` and
`center_ui` on.

Cause, inferred from the game's C and from the frame's tile rows read in a
private picture of the video memory of an earlier run: `init_hud_primitives`
gives the six fill records Y 213 and the heights 3, 4 and 4 (first, second and
third level). The frame's tiles are 16 pixels with 15 texels on the original
path, which shows the rows of the lower half one pixel lower and never shows a
tile's last row; the fills fit that picture. The artwork itself has the
interior at rows 11 to 13, 11 to 14 and 11 to 15 of the tiles, whose top is
Y 201. With one-to-one tiles that is Y 212, and 3, 4 and 5 rows. PR 342 made the
same correction for the life bars and left the meter's fills where they were.

The change: with `pixel_perfect_ui` on, the bridge's list walker recognises the
twelve records (six fills, two display buffers) by address and list, and writes
Y 212 and the heights 3, 4 and 5 into its copy of the command. The game's
record, X, width and colour stay; no marker is put into the record and the
PsyZ patch is not changed, so the library needs no rebuild. With the setting
off the packets are what they were.

What ran, on the branch's tree: `python3 port/tools/test_ui_layout.py` and
`python3 port/tools/test_video.py` pass (3 tests each, under a second each);
18 one-line changes of the new rule and of its two uses in `gpu.c`, each run
alone against `test_ui_layout.py` under limits, were all noticed; `gpu.c`
compiles for 32-bit Windows with the build's flags and no warning. Not run:
the PC program's build, the two fixtures that need a built library and a
window, and the game itself, which the owner had open.
