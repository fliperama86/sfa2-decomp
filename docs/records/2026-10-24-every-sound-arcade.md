## Every sound request is the arcade's (2026-10-24)

Toward no disc, and by the owner's word ("Just use the arcade"). The
sound table (`port/arcade/sounds.py --c-table`) no longer leaves a request
to the PS1's sound banks: 419 of its 466 sounds are arcade effects and 47
play nothing (`ARCADE_SOUND_SILENT`, which the runtime counts and does not
send). The changes: a sample that starts up to 64 bytes before an arcade
instrument record counts as in it (four more sounds mapped); "stop" rows
are silent; the menu, HUD and results sounds take the effect the arcade's
own routine for the same moment plays (a hand table in `sounds.py`, each
row with the decrypted arcade program's routine as evidence, found on a
private decrypted copy); a few by their place in a run of mapped
neighbours; the rest silent, as the arcade has no sound at that moment.
The fighters' name voices of the PS1's select screen are silent: the
arcade's select screen plays only the cursor and the pick.

Checked on 2026-10-24: the unit test (19 cases), `--arcade-check` status
0, and a fight from the pack: 393 requests played as the arcade's
effects, 6 silent, none as the PS1's sounds.
