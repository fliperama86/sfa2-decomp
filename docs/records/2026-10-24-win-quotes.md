## Win quotes in the arcade's English (2026-10-24)

The owner asked for the English text. The PS1's win screen draws a
Japanese quote as a sprite of 16 x 16 glyph tiles (the winner's
`CDEMOnn.PAC`: a tile segment of the sprite blob, cell lists and position
streams in the shared module, a quote offset table a fighter). With
`arcade_assets` the loader now writes the US arcade's four quotes of each
fighter there, with data only (`port/arcade/quotes.c`, table from
`quotes.py --c-table`): the lines in the arcade's own 8 x 16 font, two
characters a tile, and every offset table entry pointing at one of the
four. The design is in `port/arcade/README.md`, "Win quotes".

Checked on 2026-10-24: `quotes.py --compare-c` (all 21 files draw the
arcade's quotes, glyph by glyph), the unit test (31 cases),
`--arcade-check` status 0, and a fight from the pack whose win screen
showed Dhalsim's "Meditate on your loss. / Someday our paths will cross
again." Not shown on screen yet: the other fighters' quotes (the previews
in the private folder show them).
