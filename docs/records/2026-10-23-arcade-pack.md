## Playing without the disc: the pack (2026-10-23)

Asked where the PS1's own data should come from for a start without the
disc, the owner chose a one-time import ("One-time import (Recommended)"),
and for the sound "Just use the arcade. It should sound as good as the
arcade version".

`--arcade-import PACK DISC` writes a private pack: the disc's layout as
extents of sectors (the files' data bytes that the loader does not build
equal to the disc's; the music streams, the logo and opening movies and the
sectors between files as raw sectors), with, per file, the ranges the loader
served and their SHA-256. `--arcade-pack PACK` opens it as the disc
(`disc.c`: `port_disc_open_pack`, `port_disc_raw`, which the CD layer and
the intro's soundtrack now read through); the loader serves the recorded
ranges without a disc to compare with and refuses a pack whose digests its
build does not reproduce. The fighters' palettes are served palette by
palette now, so the PS1's own palettes stay in the pack.

What ran: the import (3 s, 437 MB, in `local/`), a start from the pack with
no disc into a fight (120 s, with sound) and with the arcade intro;
`--arcade-check` with the disc unchanged (status 0); `test_intro_audio_host.py`,
`test_settings.py`, `test_hostiopage.py --linux` (112), `test_hostlaunch.py
--linux` (all but the stale card case). The first start from the pack was
refused by the digests (the screens' check against the disc had nothing to
compare with in a pack); fixed.

Not done: the sound effects by the arcade's player (an agent maps the PS1's
requests), and shrinking the pack (the music streams are most of it).
