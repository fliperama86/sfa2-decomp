## Memory cards that persist, a default-off setting (2026-10-12)

The owner asked to look into disk persistence, in port code only. The port answered "no card" to every memory card call, so nothing the game
saves (its options and records, kept by the backup menu of the demo module as the file BISLPS-00415-SFZ2SYS) survived a restart.

New setting `memory_card` under `[experimental]`, default off (`--memory-card`, `--no-memory-card`, `--show-settings`), and the options `--card1 FILE`
(default `card1.mcr` in the working directory) and `--card2 FILE` (no second card unless given). On, the two slots are 128 KB card files in the standard
card format (`port/src/memcard.c`: header, directory frames with checksums, 15 blocks of 8 KB, the layout of the console's documentation), which other programs that
use that format open too. Off, the answers are the ones the port always gave.

`port/src/card.c` implements the calls the game makes (read from its C in `ps1/src/overlays/attract_demo/`, not changed): `_card_info`, `_card_load`
and `_card_clear` answer with the software and hardware events of the card classes the game opens (info: TIMOUT with no card, NEW until the card is cleared, then
IOE; load: TIMOUT, NEW for a card that is not formatted, IOE; clear: the hardware IOE); `format`, `firstfile`, `nextfile`, `open` (FREAD, FWRITE, FCREAT with the
blocks in the high bits), `read`, `write` and `close` work on the card image and complete at once. A card file that is not there is a card that was never formatted
(the game offers to format it); the file is written at format and at close. A card file of another size is refused at start.

Evidence: `port/tools/test_hostmemcard.py` (isolated: the card image byte by byte against the documented layout, the file system, and the BIOS calls and events on a made-up
game; three mutants each fail their cases), `test_settings.py`, `test_hostlaunch.py`'s source and `port_subjects.toml` updated, MinGW compiles of `card.c` and `memcard.c`
under `-Werror`. Not tested: the game's own card screens end to end (the owner's play), other programs reading the file (the format is the console's, from its documentation),
and a Windows run. Unknown: whether the BIOS's real answers differ from this reading in a case the game does not reach; deleting and renaming files and a card changed while the game runs
are not made.
