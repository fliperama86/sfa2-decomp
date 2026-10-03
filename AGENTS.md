# SFA2 project instructions

Read `PLAN.md`, `docs/requirements.md`, and `docs/project-memory.md` before work.
Record corrections and relevant preferences in the project memory. The active
working repository is this directory, not `../68k-decomp`.

## Scope and evidence

- This is an SFA2 game project, currently targeting PS1 Japan `SLPS_004.15`, not
  a general 68000 framework. Windows is retained reference research.
- Preserve readable gameplay C and explicit assembly exceptions. Neither an
  all-assembly dump nor opaque retained executable bytes complete the project.
- Distinguish exact C, tested nonmatching C, assembly, raw retention, inferred
  facts, original symbols, and unvalidated candidates.
- Exact bytes at the correct addresses/mapping establish instruction identity.
  Do not invent a mandatory game boot, runtime injection, new emulator adapter,
  rebuilt disc, or gameplay replay gate for already byte-identical code.
- Runtime observation is optional for semantic/loading questions. Nonmatching
  code requires scoped behavioral evidence, not validation by nearby exact code.
- No full-game build or live-game validation has been performed. See the pilot
  reports rather than extrapolating whole-game completion from three functions.

## Safety and workflow

- Never commit game binaries, extracted assets, game-derived analysis/source,
  compiler binaries, analysis databases, or credentials. Private artifacts live
  under `ps1/local/`, `windows/local/`, `research/local/`, or root `local/`.
  Any future publication of reconstructed source needs an explicit review of
  the current publication policy and provenance.
- Inspect license, provenance, and user intent before reusing reference code.
  `~/Projects/references` contains external tools/reference repositories.
- Preserve original inputs and historical evidence; write new reports instead
  of silently changing old hashes, paths, results, or conclusions.
- Organize target-specific work inside its root-level platform directory.
  Shared tooling and documentation belong in root `tools/` and `docs/`.
- Work on `main`. Do not create branches or PRs unless asked. Do not include
  agent attribution in branch names or PR titles. Do not push unless requested.
- Leave the old 68k repository and unrelated concurrent work untouched.
- Always run MAME windowed if it is used at all. Do not launch full-screen
  software unnecessarily.

## Communication

- Lead with the answer, keep routine replies short, and handle one subject at a
  time. Expand when requested; avoid unnecessary lists of technical identifiers.
- Do not use the em dash character. Do not suggest stopping or session breaks.
- State what actually ran and what remains planned. Report failed experiments
  honestly and never substitute original bytes while claiming a source match.
