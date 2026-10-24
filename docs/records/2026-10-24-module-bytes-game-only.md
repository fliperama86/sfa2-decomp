## Built bytes inside a pinned module go to the game's reads only (2026-10-24)

A fix to two changes of the same day. `modules.c` checks each module
chunk's data pin (the SHA-256 of the disc's chunk with its code zeroed) on
the runtime's own read of the chunk, and refuses the module when a byte
differs. The fighter change (lists served one by one) served the lists it
builds the arcade's way as owned ranges inside the fighter modules, and the
stage change (retouched tiles) did the same for a few map cells in the
stage modules of stages 03 and 10: in a run, Gouki's module was refused
("not the pinned content"). `--arcade-check` and a pack's start do not load
modules, so they did not show it.

Such bytes are now served to the game's reads only (`add_game_range`), as
the win quotes and the full animation sets already were: the module check
reads the disc's bytes, the game reads the arcade's. A pack keeps the
disc's bytes there until the data pins are made from the loader's build.

Checked on 2026-10-24: `--arcade-check` status 0, a pack made again (17 MB)
and its start, and a run as Gouki from the disc: his module loads and his
ground fireball is drawn purple, as in the arcade.
