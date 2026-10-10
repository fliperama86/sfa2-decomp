## Fixtures for the published nonmatching functions (2026-10-10)

What was done: every nonmatching function of the five nonmatching folders
and every override of the port got its fixtures file from `--record
--cases 2000 --jobs 4 FUNC` where the tool accepts it (up to six records at
the same time). Each of the six folders' pages has a section on how to
replay, how to record a new or changed function, what a pass is (evidence
for the recorded inputs, not the wide comparison, not equivalence) and
which functions have no file and why. The replay command of each folder
printed `failed 0` on every line; the lines are on the pages.

Figures of 2026-10-10, counted by a private script over the tree (one-off, not
printed by a command of a page): functions, with fixtures file, fixtures,
file bytes, uncovered slot ranges, uncovered edges.

| folder | functions | files | fixtures | bytes | slots | edges |
| --- | --- | --- | --- | --- | --- | --- |
| ps1/src/slot06_nonmatching | 29 | 21 | 116 | 317168 | 48 | 108 |
| ps1/src/slot04b_nonmatching | 21 | 7 | 19 | 35494 | 0 | 4 |
| ps1/src/resident_nonmatching | 58 | 33 | 193 | 617305 | 19 | 163 |
| ps1/src/modules_nonmatching | 47 | 31 | 84 | 349802 | 3 | 163 |
| ps1/src/library_nonmatching | 6 | 2 | 24 | 40692 | 0 | 9 |
| port/overrides | 12 | 7 | 36 | 166483 | 0 | 2 |

That is 173 functions, 101 with a file and 72 without. (The first script
that made the table skipped `func_801e9080_slot06_00`, whose contract is in
`contracts.py` and not in a `.py` file; it has a file, and the figures above
include it.) The four files of the resident folder that existed before are
counted.

The four faults of the tool, found by recording all of them (from the commit
that repaired them, none is in a function's C or contract):
- Replay put its recorders and log right above the highest address a fixture
  names, so a watched block or the words behind a pointer argument that the
  function does not touch reached further and the log lay inside what the
  replay compares. Found because 24 functions failed their replay right after
  their record.
- A recorder wrote its entry without looking at the room left in the log; an
  original with an altered loop constant called without end and the emulator
  ended with a signal.
- An altered original can jump anywhere and the emulator itself ended on what
  it found. The records of 11 functions crashed (a broken process pool, and
  a signal with `--jobs 1`); the commit does not say which of these two
  faults each crash was.
- An altered original that does not end cost the general budget in every
  case. Found by the time of the records (22 s to 7 s and 256 s to 90 s after
  the repair, one-off timings with six cases instead of twelve in the second
  run of each).
Tests after the repair: `test_difftest.py`, 345 cases, "all cases behaved as
required"; `test_difftest_build.py --config ../build.toml`, the same line; 26
mutants of the new code, each failing a named case.

Not recorded, 72 functions, by reason:
- 53 whose contract lets a callee run as original code (`--record` refuses
  them; they stay with the wide comparison until a contract stands a recorder
  in the callee's place);
- 9 whose cases read bytes of the game image (the files would publish them);
- 5 over 100,000 bytes also with `--max 6` or with one case;
- 2 with a recorder with a tail;
- 3 that read address 0.
These are the five reasons of the table of the day; the names are on each
folder's page. The reason figures were counted by the same private script.

Not done: no replay that needs nothing private (no native replay of the C as
compiled for a PC); the review tool does not select replay yet; no contract,
C file or function was changed to make a function pass.
