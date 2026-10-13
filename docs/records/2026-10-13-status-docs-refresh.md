## Status documents refreshed after every function became exact (2026-10-13)

At the owner's request the status documents were brought in line with the
build and the published counts of 2026-10-13: 5,600 distinct functions, 13,072
placements, all exact (resident 1,852, of which 1,797 from C and 55 from
assembly; overlays 11,220 placements from C), taken from the linked ledger
(`workflow.py ledger`) and the published `completion-map.json`.

- `README.md` was rewritten for a reader new to the project: what it is,
  where it stands in plain words, how the source is laid out, and where to
  go next. The per-image byte counts that it held moved to the completion map.
- `docs/completion-map.md`, `docs/current-state.md`, `docs/goals.md`,
  `PLAN.md`, `docs/requirements.md`, `ps1/README.md` and the opening of
  `port/README.md` state that code is complete and data is not, and that the
  port runs on Windows and on macOS with Apple Silicon.
- The resident byte ownership is derived from `ps1/src/build.toml`: 347,024
  bytes of C, 1,140 of assembly, 2,616 of read-only and 5,900 of initialised
  data in a payload of 614,400, so about 257,700 bytes (42%) are retained raw.
  The overlay payloads were not totalled (that needs the private extracts).
- Not changed: historical records, `docs/goals-history.md`,
  `docs/project-memory-history.md` and the long port page below its opening.
