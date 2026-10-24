## The disc's layout as numbers and names; a layout backend in the disc layer (2026-10-24)

Package C. `port/arcade/layout.py` reads the disc once and writes `port/arcade/layout_table.c`: the 260 files (name, first sector, length), the
8 music streams (sector count and, for each of the 8 channels, the last sector that holds its sound) and the two runs of filler sectors that are not
data sectors, with the disc's length in sectors. The streams' subheaders follow a model (channel = sector mod 8, sound until the channel's end, the
end marked, padding after, a last sector of its own) that the tool verifies on every one of the 145,096 sectors before it writes the table.

`port_disc_use_layout` (`disc.c`) presents the table with a pack, as a wrapper, not a replacement: the pack still gives the files' bytes (and the
loader's builds go over them as before); the layout gives the structure. `port_disc_find` and `port_disc_list` answer from the table, so no
volume descriptor and no directory sector is read; a sector outside the files is made from the layout (a form 1 data sector of zeros, or the
filler run's own submode) and the pack's raw extents are not read; a silent stream sector has the model's subheader and the pack's is not read.
A sector of a file the pack leaves out is still refused. The pack must agree with the table (length, every file's first sector and size, every
stream), or the start is refused with a line. `main.c` presents it whenever a pack is the disc; a real disc is read as before.

`--arcade-check` compares the table with the disc (files, stream subheaders, filler headers): 0 differ. The pack format and `--arcade-import` are
unchanged, so the pack still carries the 332 raw sectors (780,864 bytes) and the streams' flags and subheaders (1.15 MB) that a run no longer
reads; dropping them from the format is a later step. Checked on 2026-10-24: `test_arcade_layout.py` (the model, the tool's refusals, and the
backend on a made-up disc and pack with the real `disc.c`), `--arcade-check` status 0, a pack made and its Zangief session through the win screen
(399 requests as the arcade's effects, 6 silent, the same as before), and the same session from a copy of the pack whose raw extents and silent
subheaders were overwritten with junk: unchanged to the select screen and 100 s into the fight.
