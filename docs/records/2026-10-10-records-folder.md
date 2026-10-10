## Records are files of a folder, one per change (2026-10-10)

Added the folder `docs/records/`: a record of a change is a new file
`YYYY-MM-DD-short-name.md` whose first line is `## Title (date)`. The
workflow tool's `context --query` now searches the history's sections as
before and then the records, in order of file name, and prints each with
its source (`[records/FILE]`). A check in `tools/ai_workflow/tests/test_workflow.py`
refuses a record whose name or first line has not that form, naming the file.
`context --reindex` still regenerates only the history's index; records have none.

Why: every pull request appended its record to the history file and
regenerated its index, so each merge made every other open pull request
conflict in those two files. On 2026-10-10 that happened after each of ten
merges. Separate files cannot conflict.

From now on `docs/project-memory-history.md` and `docs/project-memory-index.json`
are the archive up to this change and are not appended to or regenerated.

What ran: `python tools/ai_workflow/tests/test_workflow.py` only; nothing was built.
