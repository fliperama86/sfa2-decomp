## The arcade asset loader begins (2026-10-23)

The owner asked what it would take to use only CPS2 assets, without the PS1
disc. A first answer from assumptions was wrong (it overstated the sprite
and sound work and proposed porting the arcade's hand-written 68000 code);
the owner asked for the facts. Measured then with the user's disc and
arcade set: every sprite tile of two characters on the disc is an arcade
tile (color index one higher), a character's attack boxes are the arcade's
records in the other byte order, one of twelve voice samples is the
arcade's resampled, and the port has no arcade sound driver (the arcade
intro plays the PS1 disc's recording). The owner's direction followed: the
PS1 code stays, the port maps and uses the CPS2 assets and writes anew what
is missing, starting with "the loader targeting the arcade rom", working
through "the files it expects".

- `port/arcade/`: `zip.c` (the archive with its own inflate), `rom.c` (the
  four views of the set, each SHA-256 pinned to the examined `sfa2u`),
  `program.c` (48 pieces of the PS1 program's data built from the arcade
  program: addresses, sizes and layouts only) and the design page.
- `port/src/arcade.c`: the setting `arcade_assets` (default off),
  `--arcade-rom FILE`, `--arcade-check`. With a disc the pieces are written
  over the loaded program and the first read of each disc file prints that
  it still comes from the disc; without one the program stops with what it
  cannot build. `cd.c` has a read hook for that, null otherwise.
- Settings: the key, its overrides and the tests of both.

Facts, method and limits are on [the arcade page](../../port/arcade/README.md).

What ran, on Linux: `test_arcade_zip.py` (8 tests, 2.1 s, synthetic archives
with every deflate kind and the malformed ones), `test_arcade_rom.py` (3, 0.6
s), `test_settings.py`, `test_hostbuild.py`, `test_intro_audio_host.py`,
`test_macbuild.py` (its cases that run off a Mac), and with `--linux`
`test_hostmirror.py`, `test_hostmodules.py`, `test_hostiopage.py` (112) and
`test_hostlaunch.py` (all but the stale card case of the Linux record). The
program built (`./build.sh linux`). With the user's disc and set:
`--arcade-check` found the 48 pieces equal to the disc's (and named the one
piece whose layout was changed by hand for a negative control); a start
without the disc stopped with the line of what is missing; a run of 150 s
with the pieces in place went from the logo through the title, the menus, a
fight and the continue screen to the opening, and printed the files the game
read. `test_workflow.py` fails as before on other files (records of
2026-10-12, control files of the intro and the settings without the pass
line, a missing library); the two new control files print the pass line.

Not run: the Windows and Mac programs, `test_hostaudio.py` (a rig for the
Windows program).
