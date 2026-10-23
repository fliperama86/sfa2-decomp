## The arcade loader builds 39% of the disc's files (2026-10-23)

The owner asked to continue until no more progress could be made. Helper
agents worked in parallel on one kind of data each, each leaving a Python
model, a generated table of addresses and counts (no game bytes), C, a
`--compare-c` of the C against the model and the disc, and a sanitized test;
the lead reviewed each, wired it into `port/src/arcade.c` and committed it.
In order: the fighters' palettes and portrait pages (`fighter_parts.c`), the
animation steps (`sequence.c`), the stages (`stage.c`), half of the
program's data (`program_more.c`: pointer words and the effects' lists), the
other screens' tiles, palettes and pictures (`screens.c`), the arcade's music
(`music.md`, `music.c`: a player of the port's own; the fighters' themes and
16 more tracks play as the arcade's songs), the rest of the fighter modules'
data (`module_data.c`) and the screen modules' cell lists
(`screen_lists.c`). The loader now serves only bytes equal to the disc's,
or packed tiles that decode to the disc's, and the music in place of the
recorded tracks it knows.

`--arcade-check` on the user's disc and set: every table equal or the PS1's
known changes, status 0, 3 seconds; its coverage line: 39% of the bytes of
the disc's files other than the program and the streams are built from the
arcade alone (fighter files 46%, stages 56%, the HUD file 90%, endings 3%);
45,858 of the program's 90,088 data bytes. Each wiring was followed by a
play run with sound into a fight. Found on the way and fixed: object files of
two folders with the same name collided (the build names them after their
folder now), unfinished files of an agent broke the build (the build takes
the loader files that `port/arcade/build.txt` lists now), a use of a buffer
after it passed to the overlay (a crash at start), and a fixed table of
ranges that the module data overflowed (it grows now).

Not done: the parts of the PS1's own (its tables, text and menus, its added
animation steps and art, the ending illustrations, the movies, the sound
banks' recipe), the sound banks (the encoder exists, `vag.c`; not wired), and
a start without the disc. The loader adds about 2.3 seconds at the start.
