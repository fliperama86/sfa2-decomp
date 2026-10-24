## Widescreen, step 4: objects in the sides (2026-10-24)

`wide_objects.c` overrides the game's margin test and its draw phase (both
resident functions, always placed, the game's C run by name): with
widescreen on, objects that the test hid only because they are past 448 right
of the camera, but that the wide view shows, are marked drawn for the draw
phase and hidden again after it. A read of the C (an agent's survey of
2026-10-24) showed that drawing reads only that flag, that the hidden objects
are still updated, and that the frame's collision list is filled by the test's
other branch only, which is left alone.

What ran: `test_wide_objects.py` (the two overrides with stand-ins of the
game's functions on memory at the game's addresses: drawn in the sides, hidden
again for the logic, not past the wide view, off with widescreen off, never on
the collision list, a negative control); the Linux program with the user's
disc and training-mode input scripts at both walls of the fifteen stages (no
sound): the run's line that objects of the sides were drawn came on five
stages (Ryu, Adon, Ken, Zangief, Nash). The black that is left at some stage
ends is the art's own end (`port/widescreen.md`), not objects.
