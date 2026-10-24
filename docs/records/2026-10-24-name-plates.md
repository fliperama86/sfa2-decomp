## The name plates' maps from the arcade (2026-10-24)

Toward no disc. Between fights the PS1 shows a name plate (each `NDEMOnn`
file): a 64 x 32 cell map over a page of the arcade's scroll 1 pieces. The
map is the select backdrop's strip of the arcade (8 rows: columns 0 to 6 of
set 0, 7 to 31 of set 64 plus the stage's band, 32 to 63 of set 84; the band
is the file's fighter, NDEMO14 shows band 2), now built by
`port/arcade/plates.c` (table from `plates.py`: numbers only). The map is no
module chunk, so it is served owned. Of 43,008 cells, 42,978 equal the
disc's; the other 30 (NDEMO00, 08, 0E, 10) are pieces the page lacks, now
written into free units of the page and showing the arcade's piece.
`compose.c` gained the scroll 1 cell and piece readers.

Checked on 2026-10-24 (on the previous commit plus these files): the unit
tests (plates 6, compose 12, screens 47), `plates.py --compare-c` (21 files,
the C equals the model), `--arcade-check` status 0, a pack made and its
start, and the Zangief session through select, fight, win and continue. The
plate itself was checked as a render (the scripted fight loses before one
shows).
