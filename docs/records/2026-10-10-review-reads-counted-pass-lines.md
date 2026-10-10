## The review command reads the pass lines that the port's tests print (2026-10-10)

What was wrong: the review command (`tools/ai_workflow/review.py`,
`check_output`) took a control file for passed only when its output
held the exact words `all cases behaved as required`. Six of the
port's twenty control files end with a line that carries a number:
`all N cases behaved as required` (`test_hostmdec.py`,
`test_hostxa.py`), `all N cases that ran behaved as required`
(`test_hostaudio.py`, `test_hostiopage.py`) and `all N tests passed in
S seconds` (`test_hostreplay.py`, `test_hostfixtures.py`). The command
refused each of them with status 0 and a clean pass, so a review of
any change of the port ended at the first of them, `test_hostaudio`,
with "control suite lacks executed cases and a completion marker". It
showed when the review of a change of the port was run on 2026-10-10;
with main's rule the six lines are refused and the fourteen others
accepted (tried on the rule directly). The pull requests that added
those files were not run through the command by their authors; for
three of the six (the native replay harness, its fixtures, the decoder
core) that is this session's omission.

What it does now: `control_passed` accepts the four forms and reads
them strictly. The passing line must be the last line, whole, and the
only line of its kind; at least one case must show as `ok`; no line
may say that a case or the file failed (`FAIL ...`, `N case(s) behaved
wrongly`, `N of M tests FAILED`), wherever it stands; and where the
line gives a number, exactly that many cases must show as `ok`. So a
file that says 20 passed and shows 19, or 21, is refused. The old rule
looked for the words anywhere in the output and for one `ok`.

What ran: `python tools/ai_workflow/tests/test_workflow.py`, 54 tests,
OK (3 new: each form accepted with lines of detail between the cases;
15 outputs that contradict their last line, each refused; and for every
control file of `port/tools`, `ps1/tools` and `ps1/src/programs`, 31
files, that its text holds exactly one passing line of a known form
and that the rule accepts that form). The saved outputs of all twenty
port control files from a run of the same day were each accepted by
the new rule (a private check). Mutants of the rule in bounded runs by
a private script: 16, each failing a named test; a change of nothing
passed. Two more were left out as changing nothing that an input can
show: the count's first digit (a count of 0 is refused by the
comparison with the cases shown) is one; the other was removed by
writing the rule with no check twice.

Limits, stated: the rule knows the four forms that the files of the
tree print today; a new control file with another last line is
refused, and the new test over the files says so at once. The words
"that ran" in two files' lines mean that those files may leave cases
out when an option is missing; the rule cannot see a case that was
left out, only that the number said equals the number shown. The
checks named `difftest-tools` and `difftest-build-tools` are judged by
their status alone, as before.
