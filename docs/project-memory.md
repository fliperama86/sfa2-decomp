# Active project memory

This is the short startup context, not a replacement for preserved evidence.
Read [current state](current-state.md), [requirements](requirements.md) and
[the plan](../PLAN.md). Retrieve only relevant historical sections:

```sh
python tools/ai_workflow/workflow.py context --query "compiler" --limit 80
```

The previous memory is byte-preserved in
[project-memory-history.md](project-memory-history.md). Its heading/line index
and whole-file hash are in [project-memory-index.json](project-memory-index.json).
The former goals log is preserved in [goals-history.md](goals-history.md).
Old figures describe their own date, not today's completion.

## Scope and owner decisions

- Active repository: SFA2, PS1 Japan `SLPS_004.15`, not the old 68k project.
  Windows is reference research. Keep unrelated work untouched.
- Readable gameplay C is required, with explicit assembly/SDK exceptions.
  Raw retained bytes and decompiler output are not recovered source.
- Exact bytes at the correct mapping establish instruction identity. Do not
  add game boot, disc rebuilding or emulator injection as an acceptance gate.
  Nonmatching C needs its own scoped contract and behavioral evidence.
- The port stays beside the PS1 work, with no PS1 changes made for the port
  until the port runs. It runs C only, never an interpreter of missing original
  code. User-supplied disc data stays private. Read `port/README.md` first.
- Source written here is published by the owner's decisions of 2026-10-05
  and 2026-10-06. First publication still needs provenance/privacy review.
  Third-party source retains its license. Private headers, binaries, game
  assets, analysis and credentials never enter Git.
- Use a branch and PR for changes. Poll owner comments every 60 seconds while
  active. Only the owner's account can approve; a plain comment is approval.
  A reviewer posts comments, not formal reviews. Check actual child exits.
  Never put agent attribution in branch names or PR titles.
- After an approved merge, proceed to the next goal without asking. Decisions
  belong to the owner; no implied approval from passing automated checks.
- Checks are isolated and fast, by the owner's rule of 2026-10-10. An exact
  match needs only its image's byte comparison. A nonmatching function is
  tested alone. No run is repeated. A run of hours is redesigned, not made.
  Read [efficiency.md](efficiency.md) before any run.

## Working style

Answer first, briefly, one subject at a time. No em dash, unnecessary lists of
identifiers, recaps or suggestions about stopping. Distinguish what ran from
what is planned. Preserve failed experiments and their original evidence.
For mechanically specified grind tasks, use small bounded packages with actual
acceptance commands. Stop and report options on a decision-bearing fork.

## Lessons to apply before a run

- Stage private inputs from the actual build configuration, not a guessed
  `ps1/local/` layout. Baseline, archives, compiler and SDK are distinct inputs.
- Matching C needs preprocessing with the pinned settings; original-address
  linking, symbol ownership and all claimed bytes matter. Never patch output
  bytes or substitute a baseline while calling it a source match.
- Shared-header, binding, compiler and contract changes invalidate dependent
  checks. Existing contracts alone do not establish tested nonmatching C.
- Snapshot the exact head before testing. Keep head, command, real exit status,
  scope and evidence hashes together. Failed or incomplete jobs are not passes.
- Do not truncate measurement lines before parsing their numbers. Print binding
  deltas, not entire large maps. Compare effective local/second-link bindings.
- Negative controls must actually trip; mutation text must be unique. Use time
  limits and terminate the whole subprocess group on timeout. Fixtures sharing
  a seed build cannot be assumed independent just because names differ.
- A regenerated inventory can contain zero data-only rows, even though earlier
  inventories contained them. Test data exclusion with a deliberate fixture,
  not a hard-coded assumption about today's inventory.
- The canonical coverage tool currently counts some excluded second-link
  owners. The linked ledger flags rows without an applicable owner as raw;
  it preserves canonical aggregate totals rather than silently rewriting them.

## Keeping this file small

Keep active memory within 120 lines. Put detailed new evidence in a record
file in `docs/records/` (one per change) or a dated/domain report. The
history is the archive: preserve its bytes and do not append to it.
Current state belongs in [current-state.md](current-state.md), next packages in
[goals.md](goals.md), and acceptance specifications in `docs/tasks/`.

## Workflow implementation lessons

A Git submodule entry is not a missing regular file. Record its pinned commit
and available content separately. Optional Python modules can have absent system
libraries even while required checks run; track that availability rather than
claiming that all optional modules were executed. Initial failed smoke reports
remain in private evidence and are not relabeled as successful runs.
