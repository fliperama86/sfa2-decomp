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
- The object reset routine and three adjacent helpers matched from C. It is
  the second caller of the first family and calls most of the earlier group.
  Twelve functions, 2,324 bytes in total.
- Static [overlay map](../ps1/docs/overlays.md): archive format, every
  code-bearing chunk on the disc, estimated link addresses.
- First parallel round: five units matched side by side and merged
  mechanically. 33 more functions. 45 functions, 5,548 bytes in total.
- Second parallel round: five more units, 25 more functions. 70 functions,
  9,836 bytes in total. Six functions are parked and stay raw.
- Third parallel round: 21 more functions. 91 functions, 13,316 bytes in
  total. Fifteen functions are parked and stay raw.
- Fourth round, a sweep over small functions: 99 more. 190 functions, 22,572
  bytes in total. 28 functions are parked and stay raw.
- Fifth round, second sweep: 132 more. 322 functions, 35,268 bytes in total.
  46 functions are parked and stay raw.
- Sixth round, third sweep: 179 more. 501 functions, 49,596 bytes in total.
  55 functions are parked and stay raw.
- Seventh round, fourth sweep: 138 more. 639 functions, 63,900 bytes in
  total. 79 functions are parked and stay raw.
- Eighth round, fifth sweep: 130 more. 769 functions, 74,216 bytes in total.
  151 functions are parked and stay raw.
- Ninth round, sixth sweep, plus a retry of every parked function: 152 more.
  921 functions, 87,852 bytes in total. 162 functions are parked and
  stay raw.
- Tenth round, last sweep of small functions: 106 more. 1,027 functions,
  96,944 bytes in total, close to 30 percent of the resident code bytes.
  173 functions are parked and stay raw.
- Eleventh round, first pass over functions of 200 to 300 bytes: 39 of 70
  matched, plus three recovered from the parked pool. 1,069 functions,
  106,776 bytes in total. 201 functions are parked and stay raw.
- Twelfth round, second pass over functions of 200 to 300 bytes: 42 of 70
  matched. 1,111 functions, 117,020 bytes in total. 229 functions are
  parked and stay raw.
- Thirteenth round: the last functions of 200 to 300 bytes and a first 20 of
  300 to 500 bytes. 39 of 63 matched, and five came back from the parked
  pool. 1,155 functions, 128,896 bytes in total, plus the first jump table.
  259 functions are parked and stay raw.
- Fourteenth round, functions of 300 to 500 bytes in game code: 31 of 54
  matched, and one more came back from the parked pool. 1,187 functions,
  141,124 bytes in total, about 43 percent of the resident code bytes.
- Rounds fifteen and sixteen, the rest of the game functions under 1,000
  bytes: 50 of 78 matched. First pass over the functions of 1,000 bytes and
  more, one per agent: 3 of 16. The permutation search and retries of the
  parked pool brought back 18. 22 library functions came in from a public
  reconstruction of the Sony SDK, built unchanged. 1,274 functions, 176,328
  bytes in total, about 54 percent of the inventoried resident code bytes.
- Shared struct layouts now come from a field table, and unit work
  directories merge with a tool. See the
  [matching build](../ps1/docs/matching-build.md) and the
  [matching guide](../ps1/docs/matching-guide.md).

## Current checkpoint

This independent repository is the active workspace. Documents, goals, lessons,
and safe tools are publishable; private working artifacts remain local and
ignored. The 68k/CPS2 implementation stays behind. The
[migration record](migration.md) tracks the completed checks and exclusions.

## Next implementation package

The sweep of small functions is done. What remains in the resident image:
See the project memory for the current counts of unattempted and parked
functions; they change with every round.

1. Land the build-tool change that is ready on its branch: units that own
   initialised data, declare their bss, or are written in assembly. Then
   import the remaining identified SDK files: those with data of their own,
   and the BIOS call stubs as assembly units.
2. Library files that were identified only in part: adapt the reference
   source to this SDK version, one file per agent, as whole-object units.
3. Continue the permutation search over the parked pool. About 210 parked
   functions have not been tried yet. Then a second, different attempt on what remains.
4. Six game functions of 1,700 to 2,400 bytes have not been attempted.
5. Replace `func_<address>` names where several units agree on a role, turn
   the recorded casts and wrapper structs into proper members, and move
   repeated prototypes into shared headers. Every such change must rebuild
   exact.
6. Read-only data is owned per unit. Game data tables still have no owner.

## Overlays

The [overlay map](../ps1/docs/overlays.md) records the archive format, every
code-bearing chunk on the disc and its estimated link address, and what the
three out-of-image calls in the matched code resolve to. It is static analysis.
Still open: confirming the map from the resident loader code, and how the
matching build should own overlay blocks, including one source linked at two
addresses for the two player sides.

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
