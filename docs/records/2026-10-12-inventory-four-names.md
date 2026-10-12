## The function inventory gets the names of four functions that became exact (2026-10-12)

Four functions of the resident program became exact in recent batches
(`func_8011acbc`, `func_80131ab4`, `func_801397d0`, `func_8013f474`) and
the inventory was not regenerated with them: their lines in
`ps1/inventory/game.tsv` still had `-` in the name column. The naming
pipeline's comparison of the inventory with a fresh sweep found it.

Regenerated on main at `90f252a4` with
`python ps1/tools/coveragemap.py sweep audit/files/SLPS_004.15 extract/PAC`
(run from `ps1/`): four lines of `game.tsv` change, each only in its name
column, and no other table changes (`git diff --stat`: 1 file, 4 insertions,
4 deletions). No address, size or boundary moved.
