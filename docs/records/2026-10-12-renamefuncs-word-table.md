## The function renamer looks each word up in a table (2026-10-12)

`ps1/tools/renamefuncs.py` found the names with one pattern that listed
every name as an alternative. The time of that grows with the count of
renames, and the naming work now asks for thousands in one run. The tool
now takes each whole word of a file (the longest run of letters, digits and
underscores) and looks it up in a table of the names. What it finds is the
same: a name stood between two characters that are not of that kind before,
and it is the whole of such a run now.

Measured on main at `cee262bc`, a dry run with `--fresh` that renames every
placeholder function that has no file of its name (8,758 renames) over
`ps1/src`, `ps1/inventory` and `port`: 1.2 seconds, ending
`8758 renames, 28719 places found in 3930 of 4365 files`. The tool as it
was did not end the same run in 60 seconds (stopped by `timeout 60`,
exit 124).

What ran: `python3 ps1/tools/test_renamefuncs.py`, all 15 cases as
required, unchanged; the two runs above.
