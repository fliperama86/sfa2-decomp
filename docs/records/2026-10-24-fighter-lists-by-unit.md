## Fighters: lists and position runs served one by one (2026-10-24)

Toward no disc. The loader kept a fighter file's whole list block from the
disc when any list in it differed. It now serves each list and position
run on its own (`fighters()` in `port/src/arcade.c`): equal to the disc as
before; built the arcade's way and owned where the PS1 changed a palette or
added a cell (the table's owned lists); the disc's otherwise (the lists
whose tile numbering the PS1 changed, PL08 and PL0E, and one PL14 list whose
palette row is the PS1's own colours). `fighter.c` pads a list the arcade
makes shorter so later lists keep their offsets; PL04's one list with no
arcade record of the same length is the arcade's 15-cell record plus a
blank 16th cell, so PL04 now builds whole.

Visible change, by the owner's rule "rebuild arcade-style": Gouki's and
Shin Gouki's fireball effect lists take the arcade's colours (orange and
yellow instead of the PS1's purple and white).

Checked on 2026-10-24: `fighters.py --compare-c` and `sequences.py
--compare-c` (the C equals the model in every file), the unit tests
(fighter, sequence, module data, sprite tiles), `--arcade-check` status 0
(13,396 lists equal, 48 the arcade's way, 206 kept), a pack made again and
its start (350 KB less of the fighter files kept). Renders of the changed
lists against the arcade's records are in the private folder; no fight
was played for this change.
