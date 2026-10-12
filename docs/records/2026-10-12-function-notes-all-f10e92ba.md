## Notes on 8831 functions and 5839 new names, made by the naming pipeline (2026-10-12)

`ps1/docs/function-notes.tsv` now has 8831 lines (`tail -n +2 ps1/docs/function-notes.tsv | wc -l`), 5919 of them with a name
(`tail -n +2 ps1/docs/function-notes.tsv | awk -F'\t' '$3 != ""' | wc -l`). 5839 functions were renamed in this change, on main at `f10e92ba`;
the rename tool's last line: `5839 renames, 20948 places rewritten in 3554 of 4361 files`.

The sentences and the names were written by a model that read each function with the sentences of its
callees, callees first; `ps1/docs/names.md` says how a name was accepted. They are inferred. Nobody read
every one of them afterwards.

What ran: the matching build of every image, each built alone, all ending `RESULT: PASS`; the inventory
sweep, which agrees with `ps1/inventory/` on every line that was not out of date on main before this change; 10 lines that had no name in the table on main took the name the sweep gives. By the owner's word of 2026-10-11 naming goes to main without a pull
request, and a rename is checked by that build and comparison only.
