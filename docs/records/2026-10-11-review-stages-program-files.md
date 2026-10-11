## The review command stages the programs' private files (2026-10-11)

What was wrong: a pull request that changed only the contract file of
a function of the program `z2o` got the review command's check
`programs-z2o-diff-1`, a fresh comparison of that function, and the
check failed with the test tool's line `INPUT ERROR: program z2o: the
file Z2O.EXE cannot be read`. The review runs its checks in a frozen
copy of the commit, into which it links the private inputs that the
build configuration names; a program's file is named by the programs'
declaration (`ps1/src/programs/programs.toml`) instead, and nothing
linked it. The comparison of a program's function could not run under
the review command at all; until then every change of a program's
function had come with new fixtures, which need only the replay.

What changed (`tools/ai_workflow/review.py`):
- `program_inputs` lists the files that the programs' declaration
  names. A path that is absolute, has a `~` or leaves the checkout is
  refused, as the build's inputs are; a declaration that cannot be
  parsed, or an entry without a file, is refused by name.
- `stage_inputs` links each of them into the frozen copy when the
  user's tree has it. A missing one is left out and is no error: a
  replay needs no program file, and a comparison that needs it stops
  with the test tool's own line. A tracked file, an existing file or
  a dangling link at that path is never replaced.
- The key of a saved result of a `programs-...` check holds the
  content of each program file that exists and the absence of each
  that does not, so that a result is not reused after the file
  changed, appeared or went. The keys of all other checks are what
  they were.
- The listing of prerequisites (`doctor`) is unchanged: a program's
  file is optional and does not make a tree incomplete for the build.

What ran: `python tools/ai_workflow/tests/test_workflow.py`: OK, 72
tests (59 before). The new tests use made-up trees and made-up
files. One real check in a
private folder: the tool's own `freeze` of this change's head and
`stage_inputs` on it gave a link for each declared program file, and
a comparison of one function of `z2o` run in that copy printed its
line (`cases 20, discarded 0, equal 20, different 0`). Mutants of the
new lines in bounded runs by a private script, by kind of mutation:
condition 14, constant 4, deletion 11, operator 8, 37 in all; 35
failed a test; none hit the time limit; 2 passed, both a change of
the word that stands for an absent file, which is only compared for
equality and cannot be a file's digest (64 hexadecimal digits): an
empty file and a real file were tried and neither tells them apart.
The last test added (an absolute path inside the checkout) was run
against the one mutant it was written for, not in a third full run.

Limits: the key tests call the tool's fingerprint with a scope that
skips the compiler's part, which needs private tools; the programs'
part does not depend on the scope. Which checks a change selects is
unchanged.

Correction, added 2026-10-11 after the review of that change: the
sentence "the keys of all other checks are what they were" holds for
what goes into a key (the programs' files are not added to other
checks), not for the keys' values. The change altered `review.py`,
whose own content is part of every key, so every saved result was
keyed anew once.
