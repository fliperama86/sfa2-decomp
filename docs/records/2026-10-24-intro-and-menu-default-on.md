## The arcade intro and the settings menu on by default (2026-10-24)

The owner: "turn the arcade intro and pc settings default on".

- `settings.c`: `arcade_intro` and `settings_menu` default to true in a build with
  PsyZ (as the graphics settings; false in a build without it, which cannot show
  them). `port/settings.ini` the same.
- `main.c`: the intro's pack is the player's to make. With the setting on and no
  `--intro-data`, a pack that is not at the default path gives one line and the
  PS1 opening; a pack named with `--intro-data` that cannot be read still
  refuses the start, as does a pack that is there but broken.
- `build.sh -r` gives the program `--intro-data` with
  `port/local/intro-asset-probe/intro-full.dat` (the importer's path,
  `port/intro/README.md`) or `SFA2_INTRO_DATA` when the file exists.

What ran: `test_settings.py`, `test_build_sh.py` (22 cases, a new one for the
pack), `test_hostlaunch.py --linux` (all but its card-routines case, which fails
on main as well), the Linux program's build, and two runs of it with the user's
disc and no settings file: with the importer's pack, GAME INFORMATION read PC
OPTIONS and the arcade intro played to its end ("complete after 2948
updates"); without a pack, the line and the PS1 opening.
