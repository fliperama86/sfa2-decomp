# Repository migration record

Date: 2026-10-03. Source: `~/Projects/68k-decomp`. Active destination:
`~/Projects/sfa2-decomp`, origin `https://github.com/fliperama86/sfa2-decomp.git`.

This was a copy-only migration, not a move or an import of old Git history.
The destination's existing empty Git repository, `main` branch, and origin were
preserved. Old repository files and unrelated uncommitted work were not edited,
staged, reset, or deleted.

## Durable knowledge

The new repository carries its own [agent instructions](../AGENTS.md),
[delivery plan](../PLAN.md), [requirements](requirements.md),
[goals/resumption point](goals.md), and [project memory](project-memory.md).
These retain relevant decisions, user preferences, corrections, tool setup,
results, limits, and next steps without depending on old-directory assistant
memory or conversation history.

The existing PS1 and Windows pilot reports and target-selection investigation
were migrated and linked from the new layout. Requirements from the earlier
CPS2 project were adapted to the selected PS1 target, not copied as misleading
CPS2 encryption, 68000 backend, or runtime-adapter requirements. The correction
that exact bytes do not need a mandatory gameplay test is recorded explicitly.

## File mapping

Paths in the left column are relative to the old repository. Target-specific
content now stays within its platform domain.

| Previous location | New location |
| --- | --- |
| `local/ps1-matching-pilot/` | `ps1/local/matching-pilot/` |
| `local/target-audit-20261002/ps1/` | `ps1/local/audit/` |
| `local/target-audit-20261002/ghidra/ps1.gpr`, `ps1.rep/` | `ps1/local/ghidra/` |
| `local/windows-gameplay-pilot/` | `windows/local/gameplay-pilot/` |
| `local/target-audit-20261002/windows/` | `windows/local/audit/` |
| `local/target-audit-20261002/windows-cd/` | `windows/local/cd-audit/` |
| `local/target-audit-20261002/ghidra/windows.gpr`, `windows.rep/` | `windows/local/ghidra/` |
| `sfa2/docs/ps1-matching-pilot.md` | `ps1/docs/matching-pilot.md` |
| `sfa2/docs/windows-gameplay-pilot.md` | `windows/docs/gameplay-pilot.md` |
| `sfa2/docs/alternate-target-investigation.md` | `research/target-investigation.md` |
| `local/target-audit-20261002/scripts/` | `tools/ghidra/` (two project-authored Java scripts) |
| Shared audit/extraction records and Saturn inventory metadata | `research/local/audit-20261002/` |

The copy manifest records 1,603 files, 131,451,803 bytes, with per-file SHA-256
checks. Its private location is `local/migration/copy-manifest.json`.
New documentation and the fresh Python environment are additional files, not
part of that copied-file count.

## Deliberate exclusions

- Generic 68000/CPS2 backend, tests, routines, ROMs, keys, captures, and Git history.
- Saturn game binaries, assets, and Ghidra database. Only comparison metadata
  needed to retain the target investigation was copied.
- Old Python virtual environments, caches, and transient SSH control sockets.
- Full disc images, which remain unchanged in the user's game library. Relevant
  extracted PS1/Windows inputs were copied privately.

Existing external dependencies remain where documented: reference-tool
checkouts, Ghidra, native tool installations, and isolated compiler directories
on Supernova. The migrated pilot scripts no longer load files from the old repo.
This does not mean the toolchain is entirely vendored or works offline on a
fresh machine.

## Relocation adjustments

- Updated the PS1 byte-comparison baseline and Windows test input/inventory
  paths to their new platform directories.
- Updated the Windows mutation runner to use its invoking Python interpreter
  instead of a launcher inside the old virtual environment.
- Created a fresh Python 3.12 `.venv` using pinned dependencies;
  `pip check` passed.
- Updated report links and current commands, and marked superseded target
  recommendations as historical.

The manifest separately records before/after hashes for all six changed copied
files. Original experiment outputs and provenance records retain their original
paths and measurements. Those historical paths are not current instructions.
After fresh builds and Ghidra checks, all 1,603 copied files still matched either
their copy hash or the recorded intentional rewrite hash.

## Verification performed from the new repository

| Check | Result |
| --- | --- |
| Copied inputs, source, tools, and evidence | Per-file SHA-256 verification passed |
| Existing PS1 family comparison | All three functions match at original addresses, 592 bytes |
| Independent full-family `.text` comparison | Exact equality with the pinned baseline range |
| Fresh GCC 2.6.3 PS1 build through existing Windows/WSL setup | All three functions match |
| Existing and freshly rebuilt PS1 execution checks | 5,120 cases per build passed |
| Fresh local Windows Clang O0/O2 builds | 5,120 cases per build passed, not byte-matching claims |
| Python compilation and environment | Passed |
| Migrated PS1 and Windows Ghidra projects | Reopened read-only and selected evidence exported successfully |
| Active pilot/tool script path scan | No references to the old repository directory |

New logs and structured reports live in `local/migration/verification/`.
Run `.venv/bin/python tools/verify_workspace.py` for the repeatable offline
comparison/execution checks. Fresh compilation instructions live in the
[PS1](../ps1/README.md) and [Windows](../windows/README.md) guides.
The temporary SSH control connection used for the PS1 rebuild was closed.

No game UI, whole-game build, new runtime integration, or new gameplay
reconstruction was performed during migration. These checks establish that
the existing pilots remain usable after relocation, not full-game completion.

## Git boundary

Publishable files are project-authored documentation, dependency pins, and
generic verification/Ghidra scripts. All platform `local/` directories, root
`local/`, virtual environments, build products, binaries, and databases are
ignored. Reconstructed pilot C and detailed game-derived analysis remain private
under the current publication policy. Credentials are not stored.

A clone therefore receives the project context, not the proprietary inputs or
complete private workspace. A fresh-clone input import/build pipeline remains
planned work, rather than an implied capability of this migration.
