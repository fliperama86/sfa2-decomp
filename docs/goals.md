# Goals and resumption point

Read with [the plan](../PLAN.md), [requirements](requirements.md), and
[project memory](project-memory.md). These documents carry project state across
sessions; there is no separate active goal-tool state that needs to be recreated.

## Long-term outcome

Understandable SFA2 source for the selected PS1 revision, with readable gameplay
C, explicit assembly exceptions, reproducible exact builds of scoped executable
payloads, accurate source coverage, and retained human/AI refinements.

## Completed evidence

- PS1/Windows baseline audits and target comparison.
- Windows three-function behavioral pilot, without exact-byte success.
- Compatible PS1 compiler discovery and exact three-function C reconstruction.
- Rebuild, byte-diff, layout, bounded execution, and mutation evidence for that
  PS1 family. See the detailed reports rather than extrapolating completion.
- Copy-only migration, fresh environment, relocated rebuild/test checks, and
  Ghidra project reopening. Project knowledge now lives in this repository.

## Current checkpoint

This independent repository is the active workspace. Documents, goals, lessons,
and safe tools are publishable; private working artifacts remain local and
ignored. The 68k/CPS2 implementation stays behind. The
[migration record](migration.md) tracks the completed checks and exclusions.

## Next implementation package, after migration

1. Establish a pinned PS1 baseline manifest and a practical range-ownership/build
   layout around the already matched resident family.
2. Inspect its callers and map the next small connected group of game functions.
3. Extend readable C and exact byte comparisons with preserved provenance and
   explicit assembly/raw accounting. Record unresolved overlay/load questions.

Do not make game booting, runtime injection, rebuilt-disc packaging, or a new
emulator integration a prerequisite for accepting exact code. Use runtime
observation only when it answers a specific remaining question.

## Not yet accomplished

Whole executable/overlay source inventory, whole-image matching-source build,
complete gameplay reconstruction, SDK exception accounting, final reproducible
delivery, and source-publication policy review. Existing private source is not
implicitly cleared for Git publication by the repository migration.

Windows VC5 matching remains optional comparison work, not a blocker for PS1.
The generic 68000 backend and CPS2 reconstruction remain in the original repo,
outside this project's current execution plan.
