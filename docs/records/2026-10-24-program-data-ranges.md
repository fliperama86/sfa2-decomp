## The program's built data served as ranges; its unread data zeroed by the pin (2026-10-24)

Package A. The loader built 45,858 bytes of the PS1 program's data (72 arcade pieces, 2,125 pointer words, the effects' cell lists) and
wrote them into memory after the program loaded, so a pack still kept the disc's equal bytes under them. `program_ranges` now serves the
same bytes as ranges of `SLPS_004.15` where they equal the disc's (`serve_units`, so only equal bytes), and a pack keeps none of them.
The file's identity check (`disc.c`, the data pin of the row `SLPS_004.15`) hashes the file as read, which now lacks those bytes; the pin
hook (`data_pin`) puts the built bytes back into its copy before it zeroes the ranges and hashes. Nothing differing from the disc is served
to any read.

Package B. `port/arcade/program_unread.py` finds the symbol ranges of the program's data block that nothing in the build reads: no object of the
build refers to the name (`nm -u`, `ps1_NAME`), no constant of `port/src/*.c` names the address, and no pointer word of a read range points
into it. 102 symbol ranges, 7,419 bytes in 10 runs, 2,435 of them not zero on the disc (the jump tables of the game's switches, strings and
tables of Sony's library functions that have no C in this build, and tables the compiled library does not name). The runs are the constant
`RESIDENT_UNREAD` of `code_pins.py`; the pin of the resident program zeroes them as it zeroes the functions (`code_pins_table.c` is
regenerated), so a pack keeps none of their bytes and its file passes the data pin. `program_unread.py --check` fails when the build refers to
a symbol inside one of the runs.

Checked on 2026-10-24: `--arcade-check` status 0; the program's pack share went from 82,221 to 35,330 bytes (A 44,136, B 2,755); a pack made
again and its start (identity by the data pin, "the pack's files and the loader's build agree"); the scripted Zangief fight from that pack
through the select screen, the fight and the win screen with no "refused:" and no stop. The pack run is also the zero-and-run evidence for
the unread data: its memory holds zeros there.

Not claimed: the 10,396 zero bytes of the program's data are small runs inside kept spans (a run of 16 or more is already dropped), so they
are not a saving of their own; and the jump tables the scan lists (136 bytes) are in the unread runs, not a separate class.
