## Notes on 1523 functions and 30 new names, made by the naming pipeline (2026-10-12)

`ps1/docs/function-notes.tsv` now has 1523 lines (`tail -n +2 ps1/docs/function-notes.tsv | wc -l`), 80 of them with a name
(`tail -n +2 ps1/docs/function-notes.tsv | awk -F'\t' '$3 != ""' | wc -l`). 30 functions were renamed in this change, on main at `90f252a4`;
the rename tool's last line: `30 renames, 787 places rewritten in 425 of 4373 files`.

The sentences and the names were written by a model that read each function with the sentences of its
callees, callees first; `ps1/docs/names.md` says how a name was accepted. They are inferred. Nobody read
every one of them afterwards.

What ran: the matching build of every image, each built alone, all ending `RESULT: PASS`; the inventory
sweep, equal to `ps1/inventory/`. By the owner's word of 2026-10-11 naming goes to main without a pull
request, and a rename is checked by that build and comparison only.
