## Fighter blocks built from the arcade set (2026-10-23)

The loader builds the cell lists and position runs of the fighter files from
the arcade's art records at the start (`port/arcade/fighter.c`, the rules of
`fighters.py` in C, and `fighters_table.c`, generated: addresses, counts and
offsets only) and serves them in place of the disc's bytes where they are
equal to them. One table of byte ranges in `port/src/arcade.c` is applied to
the game's sector reads (the read hook of `cd.c` now gets the raw sector)
and to the runtime's own (`disc.c`, a patch hook), so the module layer's
pinned digests still hold.

What ran: `fighters.py --compare-c` (the helper agent: C equal to the model
for all 37 files); `test_arcade_fighter.py` (15), `test_arcade_fighters.py`
(27), `test_arcade_rom.py`, `test_intro_audio_host.py`, `test_hostiopage.py
--linux` (112) and `test_hostlaunch.py --linux` (all but its stale card
case). With the user's inputs: `--arcade-check`, 27 of 37 files equal and
the 10 others the PS1's known changes, status 0; with one segment's count
changed by hand, status 1 naming that file; a run of 120 s into a fight with
Ryu's and Dhalsim's blocks served from the arcade's.

Not covered yet: PL04 (a pose of the PS1's own), PL10, PL11 and PL13 (art of
the PS1's own), the sprite tiles, the animation steps and everything else
of the files.
