## The port's extra memory is 2 MB (2026-10-24)

The full animation sets live in the port's extra memory after the RAM
(Linux and Windows; the Mac has none and keeps the PS1's reduced sets).
Gen's full set needs about 1.33 MB with Zangief's, so the region is now
2 MB (0x80200000 to 0x80400000, `PORT_EXTRA_SIZE`). `port_game_span`
counts the region as the game's memory once it is mapped (it is read-only
after `port_seal_extra`, so a game write there still stops the run), and
the Linux program check refuses a program segment inside it.

Checked on 2026-10-24: `test_hostbuild.py`, a build, `--arcade-check`
status 0, a pack made and started, and a Zangief fight from it with his full
set in the extra memory.
