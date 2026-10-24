## Screens: tiles cut from the arcade's frames (2026-10-24)

Toward no disc, by the owner's rule "rebuild arcade-style".

- `ARCADE_SCREENS_WINDOW`: a tile item cut from a frame of the arcade (an art
  record or scroll 2 blocks, drawn by `compose.c`) at a position, with the rows
  kept and the stored flip. `screen_windows.py` finds the windows (`--find`
  makes the 157 frames of its `SOURCES`); `arcade_screens_source_index` gives a
  frame's index to other builders.
- `ARCADE_SCREENS_BY_DESIGN` on tile runs and windows: the select portraits'
  clipped edge (170 tiles) is the arcade's whole tile. The dithered gradient
  pages of the select and win screens are blank by design. Both are rows of
  `DESIGN_ROWS`, counted by the generator.
- A chunk that starts with bytes built by design keeps the archive header's first word
  (`arcade_screens_head_word`): the game's disc stream compares it and re-reads
  the file when it differs.
- The pose library (350 KB in DEMO, SELECT and CONT), the select screen's
  poses, grid and NEXT arrow, the win screens' poses and the continue screens'
  art are now built from the arcade alone.

Checked on 2026-10-24: unit tests (compose 12, screens 47, windows 8, screen
lists 14), `other_files.py --compare-c` (0 problems), `screen_lists.py
--compare-c` unchanged, `--arcade-check` status 0. Screens from the arcade
alone 23.83 to 32.82 MB; pack kept 14.21 to 5.62 MB (the file 18.79 to 10.13
MB). Left: NDEMO's name plate map (164 KB), CONT10 and CONT10X slot 0x13 (a
segment that does not pack into its room), the Japanese glyph tiles, the own
cell lists.

Also run on 2026-10-24: a pack made again (9 MB) and its start, and the
Zangief session from it through the select screen (poses drawn), a fight on
stage 0F, the win screen and the continue screen, with no stop. A blanked
SELECTA page first crashed the select screen because its first word no
longer matched the archive header; the loader's first-word check
(`first_words()`, same day) now guards every chunk.
