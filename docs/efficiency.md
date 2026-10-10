# Efficient checks

The owner's rule of 2026-10-10, for every agent and every session that
works here: think about cost and scale before a run. A check is an
isolated unit test that runs fast. A run of hours for a small change is
a design fault, not diligence.

The reason he gave: agents had been rebuilding and retesting the whole
game for each function they matched exactly, although an exact match
needs no testing at all. Every process is to be made so that an agent
can run isolated unit tests that finish very quickly, without spending
time on running the program. His words are in the project's history
(`python tools/ai_workflow/workflow.py context --query "Efficient checks: the owner's rule"`).

## The rules

1. Before a run, know what it costs and what it can tell you that you
   do not know yet. When a run for a small change would take more than
   a few minutes, do not start it: narrow it, run its parts at the
   same time, reuse a result, or fix the tool. If none of these is
   possible, say the cost and ask the owner.
2. An exact match needs no testing. Its evidence is the rebuilt bytes
   at the right address. Rebuild the unit's own image and compare. Do
   not rebuild the whole game for it, do not rerun other functions'
   tests, and do not run a differential test of it.
3. A nonmatching function is tested alone, with its own contract. Test
   the functions that the change touches. One added function is no
   reason to run its folder again, and none to run every folder.
4. Do not repeat a run whose inputs did not change. A worker's run, the
   same run by whoever directs the worker and the same run by a review
   are one run made three times. Reuse the result and name the commit
   it was made on.
5. Runs that do not depend on each other go at the same time. The
   machine is shared: a handful at once, not every core.
6. No check boots or plays the game. A running game answers questions
   that nothing else can; it is never a gate (see `AGENTS.md`).
7. A new tool or check is designed for this from its first version. It
   works on one function, one unit or one file. Its header says what
   it reads and what a run costs. Its own tests need no private input
   where that is possible.
8. A work package for another agent states the machine time its
   acceptance commands are expected to take.

## What to run for which change

| The change | Evidence that is enough | Commands | Not needed |
| --- | --- | --- | --- |
| A new or changed exact unit | Its image builds and its bytes are the original's | `python ps1/tools/matchbuild.py --image IMAGE` (`resident` or the module's name); `python ps1/tools/fndiff.py --rebuild UNIT` for the listing of one unit (it needs one earlier build with the same `--tag`) | The whole build, any differential test, any other image |
| A new or changed nonmatching function | Its differential test and its write audit on two seeds, and its control | From `ps1/src/slot06_nonmatching/`: `python difftest.py --config ../build.toml --folder ../FOLDER --cases 2000 FUNC`, again with `--seed 7`, each also with `--writes`, and once with `--control`: five runs, at the same time | `--all`, other folders, the matching build |
| A shared header, `types.fields`, `symbols.ld` or the shape of `build.toml` | What the change can reach still builds to the same bytes and still passes | The whole matching build once (its object cache compiles only what changed), and the nonmatching folders' tests once, the folders at the same time | A second run of either on the same tree |
| A piece of the port (`port/src`, `port/tools`) | The control files of that piece pass | The `port/tools/test_*.py` files that cover the piece; `python3 port/tools/hostbuild.py` once when the build or its tables changed | Every control file for a change of one piece |
| An override of the port (`port/overrides/`) | As for a nonmatching function | The same five runs with `--folder ../../../port/overrides` | The other overrides |
| Text only | Nothing; `python tools/ai_workflow/tests/test_workflow.py` when the history or its index changed | | |

A boundary that moves still needs the inventory regenerated, as
`AGENTS.md` says. A pull request says which of these ran, on which
commit, and what did not run.

From now on a folder of nonmatching functions is kept so: its page
shows, for each function, what the function's commands printed when it
was added or last changed. Adding a function adds its lines; it does
not call for the others to be run again. The pages that exist today
were each written from one run of all their functions and say so; they
are reworded when they next change.

## For a review

By the owner's word of 2026-10-10 a reviewer does not spend hours
retesting. A review is: run the isolated tests of what changed, read
the code, and that is it.

- For an exact unit: build its image and see the bytes match. Read the
  C.
- For a nonmatching function: run that function's own test. Read the C
  and its contract against each other. A case the reviewer invents for
  that one function is welcome; a rerun of other functions is not.
- For a tool or a piece of the port: run the tests of that piece. Read
  the code.
- For text: read it.

Not part of a review: the whole matching build, every folder's tests,
every control file of the port, or any run that the change cannot
affect. When a reviewer thinks a wider run is needed, the reason is a
gap in the isolated tests, and the finding is that gap.

## Fixtures for nonmatching functions

The owner's decision of 2026-10-10: a nonmatching function comes with
a few fixtures for its unit test, and that is all its test needs. A
fixture is one input (the arguments and the memory the function reads)
with the result the original code gives for it (the return value, the
memory it leaves, the calls it makes). The test runs the function's C
on each fixture and compares. It does not run the original code, it
does not depend on any other function's C, and nothing has to be run
again when another function changes: each function cares about what it
does, not about what depends on it or what it depends on.

- The wide search with many random inputs against the original code is
  made once, when the function is written or changed. What it found
  worth keeping becomes fixtures.
- Fixtures are chosen on purpose: every arm of the function, and the
  edges of its comparisons, not only a random sample.
- Fixtures are published with the function. The owner has said that
  values the original code computes for inputs the project makes up
  may be published.
- The same fixtures can test the function as compiled for the PC,
  which the comparison with the original's machine code cannot.

This is the direction; as of 2026-10-10 the tools do not have it yet.
Today's test (`difftest.py`) runs the original code and random cases
on every run. Until the fixture form exists, the table above is what
to run.

## Known costs, as of 2026-10-10

These are the places where today's tools do not yet follow the rules.
Each is work to be done, and until it is done the narrow commands of
the table are the way around it.

- The whole matching build links every image, one after the other,
  also when one unit changed. `--image` is the narrow form.
- `python tools/ai_workflow/workflow.py review` runs every nonmatching
  folder's tests again, one check after the other, as soon as a shared
  file changes (`symbols.ld`, `types.fields`, a header), and all the
  port's control files for any change under `port/`. Its results are
  keyed by the whole tree under `ps1/`, so one changed file ends every
  reuse.
- A case of `difftest.py` costs about the same whatever the size of
  the function: the tool handles the whole of the console's memory for
  every case. (Measured once, on 2026-10-10; not yet profiled in
  detail.)
- `port/tools/hostbuild.py` compiles every unit again on each run.

- Nonmatching functions have no fixtures yet (see above).

When one of these is repaired, its line leaves this list in the same
pull request.
