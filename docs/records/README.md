# Records

One file per change, so that open pull requests never conflict over a
shared history file. `docs/project-memory-history.md` is the archive up
to the change that added this folder and is not appended to any more.

- A record is a new file here, named `YYYY-MM-DD-short-name.md` (lower
  case letters, digits and hyphens; the date is the day it is written,
  from `date +%F`).
- Its first line is one heading, `## Title (YYYY-MM-DD)`, followed by the
  record's text.
- Nothing is regenerated: `python tools/ai_workflow/workflow.py context
  --query "topic"` reads the history and then these files, the newest
  first (by file name). This README is not a record.
- `python tools/ai_workflow/tests/test_workflow.py` refuses a file whose
  name or first line has not this form.
