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
- Disc baseline manifest: all 260 files pinned by hash, tool to pin and verify.
- [Matching build](../ps1/docs/matching-build.md): range ownership, whole-image
  link at original addresses, fail-closed comparison with negative controls.
  The rebuilt executable is byte-identical to the baseline.
- Five more functions matched from C: four setup helpers and the smaller of
  the family's two callers. Eight functions, 1,228 bytes in total.
- Native local compiler, checked against saved reference outputs. The pinned
  reference compiler rebuilds the same image.

## Current checkpoint

This independent repository is the active workspace. Documents, goals, lessons,
and safe tools are publishable; private working artifacts remain local and
ignored. The 68k/CPS2 implementation stays behind. The
[migration record](migration.md) tracks the completed checks and exclusions.

## Next implementation package

1. Reconstruct the large object reset routine that calls the whole matched
   group, with its remaining small callees. It is the other caller of the
   matched family and ties the group together.
2. Add data and read-only data ownership to the matching build when a unit
   first needs it. Until then such units are rejected, not misplaced.
3. Resolve the overlay question below far enough to name what the resident
   code calls.

## Open overlay question

Resident code calls three addresses above the end of the resident image. Two
of them are the same routine at two bases 0x18000 apart, chosen by player side.
The per-character `PL##.PAC` and `PL##X.PAC` archives contain MIPS code, which
fits one code copy per side. Archive format, load addresses and which archive
entry holds the code are not yet established. Nothing here was observed in a
running game.

Do not make game booting, runtime injection, rebuilt-disc packaging, or a new
emulator integration a prerequisite for accepting exact code. Use runtime
observation only when it answers a specific remaining question.

## Not yet accomplished

Whole executable/overlay source inventory, a whole image built from source
rather than mostly retained bytes, data ownership, assembly owners, overlay
images, complete gameplay reconstruction, SDK exception accounting, final
reproducible delivery, and source-publication policy review. Existing private source is not
implicitly cleared for Git publication by the repository migration.

Windows VC5 matching remains optional comparison work, not a blocker for PS1.
The generic 68000 backend and CPS2 reconstruction remain in the original repo,
outside this project's current execution plan.
