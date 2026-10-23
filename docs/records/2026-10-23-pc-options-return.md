## PC OPTIONS goes back to the mode select (2026-10-23)

The owner, after PC OPTIONS replaced GAME INFORMATION: "start the option to
return back to the mode menu". The first version ran the settings menu in
place of Exec of `BF3.EXE`; the game then restarted its session, at the title,
as the item's handler had already stopped the music, marked the restart and
loaded the program's 128 KB over the top of RAM.

- `menu_run.c`: an override of that handler,
  `request_sound_then_exec_program_2_slot0f` (setting `settings_menu`), does
  what BACKUP's handler does around its screen: the mode select's phase one
  back, the menu, a return. The Exec hook of the first version is gone.
- `modules.c` and `library.c`: an override may name a module function. Its
  jump is written at each placement of the module in place of the C (a host
  call on the Mac); `library.c` writes only the resident ones at start.

What ran: the Linux program's build; a run with the user's disc and an input
script: the mode select, PC OPTIONS, a change in the menu, RETURN TO GAME, the
mode select again with PC OPTIONS selected, and its cursor moved;
`test_menu.py`, `test_input_keys.py`, `test_settings.py`,
`test_hostkernel_start.py` and `test_hostlaunch.py --linux` (all but its
card-routines case, which fails on main as well). Not run: the Mac program,
whose placement of an override is written but not built here.
