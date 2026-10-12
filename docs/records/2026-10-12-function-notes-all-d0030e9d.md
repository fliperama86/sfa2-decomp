## Notes on 8836 functions and 2517 new names, made by the naming pipeline (2026-10-12)

`ps1/docs/function-notes.tsv` now has 8836 lines (`tail -n +2 ps1/docs/function-notes.tsv | wc -l`), 8436 of them with a name
(`tail -n +2 ps1/docs/function-notes.tsv | awk -F'\t' '$3 != ""' | wc -l`). 2517 functions were renamed in this change, on main at `d0030e9d`;
the rename tool's last line: `2517 renames, 6554 places rewritten in 2008 of 4361 files`.

The sentences and the names were written by a model that read each function with the sentences of its
callees, callees first; `ps1/docs/names.md` says how a name was accepted. They are inferred. Nobody read
every one of them afterwards.

What ran: the matching build of every image, each built alone, all ending `RESULT: PASS`; the inventory
sweep, which agrees with `ps1/inventory/` on the line of every renamed function. By the owner's word of 2026-10-11 naming goes to main without a pull
request, and a rename is checked by that build and comparison only.
