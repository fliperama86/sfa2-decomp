# Current state and entry points

All functions that the static sweep finds are exact: 5,600 distinct
functions, 13,072 placements counting code linked a second time, 1.8 MB of
code. The resident executable has 1,852 (1,797 from C, 55 from assembly)
and 77 overlay images own the rest, every one from C. The build still
takes data (tables, text, similar) from the original files: units declare 44
read-only and 8 initialised data ranges, so about two fifths of the
resident payload is raw. Names are inferred, none is an original symbol (98%
of functions, 68% of fields and 8% of data symbols have one). The
sweep's boundaries are estimates, and no whole-game build or disc rebuild
from source alone has been shown. There is no fresh-clone import and build
yet.

The PC port runs the game from the user's disc on Windows and, since
2026-10-12, natively on macOS with Apple Silicon (in a small virtual
machine): title, menus, a fight and sound. Since 2026-10-23 it also runs
natively on Linux, as a 32-bit program of the Windows design
([Linux page](../port/linux/README.md)). It skips the disc's stand-alone
programs, stops by name where C is missing, and is not shown to play to the
end.

The port-only experimental arcade intro now completes all eight native phases
in the normal Windows executable, with an adapted PS1-disc soundtrack, then
returns to the game. Since 2026-10-24 it is on by default (the PS1 opening plays
when there is no intro pack). See the
[runtime evidence and fidelity limits](records/2026-10-14-intro-full-runtime.md).

By the owner's direction of 2026-10-23 the port is to need no PS1 disc: the
game's C stays, and what it reads is built from the user's arcade set where
the arcade has it, or written anew. The loader has begun behind the default-off
setting `arcade_assets`: it builds 39% of the disc's files (other than the
program and the music and movie streams) and half of the program's data from
the arcade set, and plays the arcade's songs for the music tracks it knows;
the disc gives the rest ([arcade assets](../port/arcade/README.md)).

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
