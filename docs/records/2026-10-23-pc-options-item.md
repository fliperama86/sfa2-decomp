## PC OPTIONS in the mode select (2026-10-23)

The owner asked what GAME INFORMATION does, then "go ahead, replace it with
PC OPTIONS". A scripted run of the port showed that the item starts the
disc's stand-alone program `BF3.EXE` (its `BF3.STR` movie, about 50 MB); the
program table of the resident executable points the menu's program 2 at it.
The port has no C for it and skipped it.

With `settings_menu` on, `menu_run.c` writes `PC OPTIONS` and its centred x
over the item's string and text record in the attract module's data each
time `modules.c` places that module, after checking the disc's bytes there,
and `overrides.c` runs the settings menu in place of Exec of `bf3.exe`. The
game then starts its session again, as after the program, so the menu's
RETURN TO GAME leads to the title. The menu's own title is PC OPTIONS too.
`port/settings-menu.md` has the addresses.

What ran: the Linux program's build; a run with the user's disc and an input
script that went to the mode select (its last item read PC OPTIONS), chose
it, changed the window size in the menu and returned to the title;
`test_menu.py`, `test_hostkernel_start.py`, `test_settings.py` and
`test_hostlaunch.py --linux` (all but its card-routines case, which fails
on main as well).
