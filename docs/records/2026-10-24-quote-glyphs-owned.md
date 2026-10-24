## Win quotes: the glyph segments built whole from the arcade (2026-10-24)

Toward no disc. The winner's glyph segment (in each `CDEMOnn.PAC`'s sprite
blob) held the PS1's Japanese quote glyphs; the English quotes were written
over its start, served to the game's reads only, so a pack still kept the
whole segment from the disc. The loader now builds each segment whole before
the pack's check (`quote_segments()` in `port/src/arcade.c`): the English
glyphs from the arcade's font, the rest of its room zero. It is served to
every read and a pack keeps none of it. The module's tables are still
written by `win_quotes()`.

Checked on 2026-10-24: `--arcade-check` status 0, a pack made again (312 KB
less kept), its start, and a fight from it whose win screen showed
Dhalsim's "I'm your master. Pain is a state of mind and I don't mind your
pain."
