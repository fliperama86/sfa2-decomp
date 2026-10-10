# Current state and entry points

The selected PS1 program has an original-address matching build for the resident
image and declared overlay images. It still retains raw ranges. This is not a
whole-game build from recovered source, a completed decompilation, or proof of
live gameplay. The PC port starts the game from the user's disc on Windows and
played into a second fight in one scripted run; it has no sound, skips the disc's
stand-alone programs, stops by name where C is missing, and is not shown to play
to the end.

## Source of truth

- Scope and acceptance: [requirements](requirements.md), [plan](../PLAN.md).
- Active work and blockers: [goals](goals.md), [port decisions](../port/README.md).
- Matching owners and flags: `ps1/src/build.toml`.
- Metadata-only static boundaries: `ps1/inventory/` (estimates, not bytes).
- Matching evidence and limitations: [build guide](../ps1/docs/matching-build.md).
- Nonmatching contracts/evidence: `ps1/src/*_nonmatching/` and their READMEs.
- Historical decisions: [indexed memory](project-memory-index.json) and
  [preserved goals](goals-history.md). Historical counts are dated observations.

Generate the current linked ledger without private game inputs:

```sh
python tools/ai_workflow/workflow.py ledger
```

It distinguishes declared exact ownership, assembly, partial ownership,
unvalidated candidates and missing C. A verified local review report can add
scoped tested-nonmatching status; no gameplay or exact-byte credit is inferred.
Aggregate counts reuse the existing coverage map. Excluded second-link owners
are flagged separately, not silently credited as applicable source.

## Agent entry point

Read short active memory first, retrieve history by topic only when needed,
and pin a task contract before implementation. Use the
[AI workflow](ai-workflow.md) for prerequisite checks, frozen-head reviews and
cache integrity. Generated evidence stays under ignored `local/`.
