# SFA2 delivery plan

Date: 2026-10-03, state refreshed 2026-10-13. This is the game-focused continuation after migration from
the separate 68000 workbench. It does not import that project's CPS2 backend,
milestones, encryption requirements, or completion claims.

## Outcome

An understandable, reproducible reconstruction of the selected PS1 SFA2 program,
including the main executable and relevant executable overlays discovered on
the way. Prefer readable, byte-matching gameplay C, with reviewed assembly
exceptions. Original non-code assets remain external user-supplied inputs.

Working baseline: Japanese `SLPS_004.15`, pinned in the
[PS1 pilot](ps1/docs/matching-pilot.md). Windows is a reference, not evidence that
the PS1 or arcade versions behave identically. No further region/platform work
is required to finish this selected target unless explicitly added.

## Current state

- Static baseline audit and resident-code load mapping exist. All 260 disc
  files are pinned by hash in a private manifest derived from the disc image.
- Every function that the static sweep finds is exact: 5,600 distinct
  functions, 13,072 placements counting code linked a second time. The
  resident executable has 1,852 (1,797 from C, 55 from assembly: stubs and the
  program entry routine, code that was assembly in the original) and 77
  overlay images own the rest from C. Of the resident C functions, many are Sony
  SDK library functions built from a public reconstruction of the SDK, part of
  them adapted to this SDK version; a number have a form found by an automatic
  permutation search and say so in their source.
- The [matching build](ps1/docs/matching-build.md) rebuilds the resident
  executable and the overlay images byte-identically from declared owners. C
  units link at original addresses. Data is owned only in part: units declare
  44 read-only and 8 initialised data ranges, and the rest of each payload is
  retained raw and counted separately. Checks fail closed and carry their own
  negative controls.
- The build runs locally with a native GCC 2.6.3. The pinned reference
  compiler reproduces the same image.
- The source is sorted by subsystem, character, stage and screen, one file per
  unit named after its first function ([layout](ps1/src/README.md)).
- Names are inferred; 98% of functions, 68% of fields and 8% of data symbols
  have one, and none is an original symbol.
- No whole-program source from which a disc is rebuilt, ownership of all game
  data, or live-gameplay observations of the matched code are claimed.
- The repository's public-facing files are documentation, tooling and the
  reconstructed PS1 source in `ps1/src/`. Building it needs local inputs that
  the repository does not contain.

## Validation principle

For an exact reconstruction, verify the compiler inputs, full claimed output
ranges, layout, call targets, and addresses against the pinned baseline.
**Matching bytes with the correct mapping do not need a gameplay test as
additional proof that the instructions match.**

Runtime observation remains useful when it answers a specific unresolved
question: which state a routine operates on, how an overlay is loaded, or what
an inferred field represents. It is not an obligatory next stage. Nonmatching
C or intentional changes require separate behavioral tests and clear labels.

## Planned sequence

| Stage | Deliverable | State |
| --- | --- | --- |
| Feasibility | Connected matching-C pilot with actual build/diff evidence | Achieved at three-function scope |
| Baseline and inventory | Pinned code-bearing files, load/overlay maps, original-vs-inferred symbols, function/data boundaries | Disc files pinned; [overlay map](ps1/docs/overlays.md) with link addresses confirmed against the loader's tables, and an inventory of the functions inside the modules that is an estimate from a static sweep; [library families](ps1/docs/library-families.md) of the game functions in the resident image and the modules, an estimate over the same sweep |
| Reproducible matching build | Range ownership, original-address linking, fallback accounting, build manifests, fail-closed byte checks | Working for the resident image and all 77 overlay images, each linked and compared alone; second links of shared units are compared with their own chunks |
| Gameplay reconstruction | Expand meaningful connected routines/subsystems with readable types and named data | Code: every function of the sweep is exact (5,600 distinct, 13,072 placements; resident 1,852, overlays 11,220 placements). Data and names remain |
| Coverage and exceptions | All scoped executable code accounted for; reviewed C/assembly, SDK handling, explained exceptions | Code is accounted for; exceptions and data ownership are under review |
| Reproduction and delivery | Clean rebuild, complete code-payload comparisons, source/provenance review, operating instructions and remaining limits | Planned |

Continue with game data ownership, names and the delivery steps. Add data
ownership to the build when a unit first needs it. Do not start a new generic
architecture project or require a full-disc builder before extending useful
source coverage.

Assembly/raw retention can bootstrap a whole-image build, but its bytes must be
reported separately from recovered C. A byte-identical copy-through build is
not evidence that those retained instructions are understood.

## Per-routine process

1. Pin the source file, load address, byte range, references, and known ABI.
2. Recover types and control/data flow; mark names and roles as inferred unless
   original symbols actually exist. Resolve material ambiguities first.
3. Write readable C, compile with pinned tools, link at original addresses,
   and compare every claimed byte with no masks or baseline substitution.
4. Keep any nonmatching candidate separate from the exact owner. Validate it
   with an explicit input/output contract if retaining it as behavioral C.
   The folders `ps1/src/*_nonmatching/` hold them; the test is in
   `ps1/src/slot06_nonmatching/`.
5. Preserve source refinements, failed hypotheses, current ownership, tool
   provenance, and verification results independently of generated analysis.
6. Expand the connected caller/callee context and aggregate coverage honestly.

## Meaning of complete

All scoped code-bearing payloads must be represented by reviewed source or an
explicitly documented assembly/SDK exception, with correct layout and exact
baseline comparison for the authoritative build. Unexplained executable blobs
prevent an unqualified completeness claim. Human-readable gameplay logic,
data types, subsystem documentation, dependency manifests, and clean rebuild
instructions are part of delivery, not optional polish.

No mandatory game-boot gate is added when full relevant output identity is
established. Optional disc packaging, ports, modifications, other revisions,
or arcade-equivalence research are separate scope decisions. See the
[requirements](docs/requirements.md) for evidence and publication rules.

One such decision was taken on 2026-10-06: the owner started a port to
macOS on Apple Silicon, Windows and Linux. It runs beside this plan and is
no part of what complete means here. Its state is on the
[port page](port/README.md).
