## The review command selects the replay of fixtures (2026-10-10)

What changed: `tools/ai_workflow/review.py` chooses the checks of a
nonmatching function or an override of the port by whether it has a
fixtures file (`FUNC.fixtures.json`, in the tree since pull request 180
for the functions the tool accepts).

- A changed function with a file: one replay check for its folder
  (`difftest.py --folder DIR --replay FUNC...`). No original code runs.
  The wide comparison that made the file was its author's step.
- Its contract changed and its file did not: the wide runs too (the
  comparison and the write audit on two seeds, the control), because
  nothing recorded stands for the new contract.
- A changed function without a file: the wide runs, as before.
- A header, the configuration or a build tool changed: the matching
  build as before, then the replay of every function with a file and
  one comparison, on the first seed, of every function without. Before,
  such a change selected the five wide runs of every function of every
  folder, one check after the other.
- The test tool of nonmatching C changed (`difftest.py`, `contracts.py`
  or their two test files): its two test files and the same replay and
  comparison. It no longer selects the matching build or the build of
  the PC program, which it cannot affect. (The message of commit
  `1b0540a` names the tests of `ps1/tools` there; the old selection did
  not have them for this change. What it had was printed from main's
  `review.py` by a private script: the matching build, the five wide
  runs of every folder, the tool's two test files and the PC build.)
- A fixtures file is a file of its function. Before, it counted as an
  unknown file of its folder, and that selected everything.
- The evidence of a replay check: one line for each function, at least
  one fixture, all passed, none failed. A line that is missing, twice,
  or goes on after `failed 0` is refused.

Why: the owner's rule of 2026-10-10 (`docs/efficiency.md`): a review
runs the isolated tests of what changed and does not retest the tree.

What ran: `python tools/ai_workflow/tests/test_workflow.py`, 43 tests,
OK (39 before). 18 mutants of the new code in bounded runs, each
failing a named test (a private script). On the tree of this change,
the selection for five kinds of change was printed by a private script
and read: a function with a file gets the replay and the PC build; its
contract alone gets the replay and the five wide runs; a function
without a file gets the five wide runs and the PC build; `difftest.py`
gets, for the six folders, the replay of 101 functions and one
comparison of 72, and its two test files; a header gets the same with
the matching build and the PC build.

One private trial of the command end to end: a commit that is not
published added a line end to one fixtures file
(`ps1/src/resident_nonmatching/func_8012fd80.fixtures.json`), and
`workflow.py review --base HEAD~1 --head HEAD --data-root ROOT` selected
one check, `resident_nonmatching-replay`, ran it on the frozen head and
ended with `checks_passed`; the check's evidence is the line
`func_8012fd80 replay: fixtures 6, passed 6, failed 0`. About 5 seconds
(one-off timing).

Not done and not shown: no review with the wide runs or with a broad
change was run with the new selection (the commands it selects ran for
pull request 180). The ledger is unchanged: its tested status asks for
the five wide runs in the report it is given, and a report with replays
only leaves a function's status. Results are still keyed by the whole
tree under `ps1/`, and checks still run one after the other.
