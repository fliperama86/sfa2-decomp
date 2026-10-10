## The decoder's test names the case when its driver ends (2026-10-10)

A correction that the owner's review of pull request 189 asked for,
after approving it. The record of the movie decoder's core said that 82
of 83 mutants failed a named case. The saved log shows 81; one mutant,
the table setter without its check of the size, read past a short table
and ended the test's driver at a guard page: the tester then stopped
with a nonzero status and a Python error, and named no case. The
reviewer checked that this mutant is caught (the correct setter refuses
63 bytes, the changed one ends at the guard) and that the difference is
one of reporting.

What changed: `port/tools/test_hostmdec.py` notes which case's commands
the driver is running; the case of the setters' refusals checks the
driver's status and the number of its lines before it reads them, and a
driver that ended is that case's failure, by name. Any other stop of
the driver in the middle of a case is now reported as a failure of the
case in work (or of the one that follows the last finished case), with
the count of cases before it, instead of a Python error. The sentence
in the record of the core is corrected and says that it was.

What ran: `python port/tools/test_hostmdec.py --cc i686-w64-mingw32-gcc`,
"96 cases", "all 96 cases behaved as required"; the one mutant again,
in a bounded run with the private runner: status 1 and the line
`FAIL set_quant refuses a command that is not 2, 64 bytes short, 128
short; set_scale a wrong command and 127 bytes: each E_COMMAND, tables
unchanged: the driver ended with status 5 after 2 of 7 lines`.

Not done: the other 82 mutants were not run again; `mdec.c` is
unchanged.
