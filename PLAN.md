# SFA2 delivery plan

Date: 2026-10-03. This is the game-focused continuation after migration from
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
- 1,754 functions match exactly: 1,701 from C (288,624 bytes) and 53 from
  assembly (1,040 bytes): stubs and the program entry routine, code that was
  assembly in the original. Units also own 2,352 bytes of
  read-only data in 20 ranges and 5,900 bytes of initialised data in eight.
  302 of the C functions are Sony SDK library functions built from a public
  reconstruction of the SDK, part of them adapted to this SDK version (seven
  that the reconstruction lacks were written here, and two that it has only
  in a much smaller later form), and
  42 of the stubs are generated from its macros. 84 functions have a form
  found by an automatic permutation search and say so in their source, six
  of them cleaned by hand afterwards; 25 more say that they were finished by
  hand from a candidate the search had reshaped, 4 that they were written
  by hand in the form of such a function, and 1 that its form came from a
  scripted enumeration of variants.
- The [matching build](ps1/docs/matching-build.md) rebuilds the whole resident
  executable byte-identically from declared owners. C units link at original
  addresses; the other 316,484 payload bytes are retained raw and counted
  separately. Checks fail closed and carry their own negative controls.
- The build runs locally with a native GCC 2.6.3. The pinned reference
  compiler reproduces the same image.
- Ten overlay modules are in the build as images of their own. `slot2a`:
  all 28 functions are exact from C, 6,052 bytes, and 15,112 bytes are
  retained raw. `slot0b`: 56 C functions, 5,652 bytes, are exact and 25,784 bytes
  are retained raw. `slot12`: 178 C functions, 26,224 bytes, are exact and
  93,944 bytes are retained raw. `slot16`: 11 C functions, 984 bytes, are exact and 10,406 bytes are
  retained raw. In `slot17` the 11 functions of `slot16` are linked a
  second time, at another address and from the same objects, and are exact
  against its own chunk; 10,406 bytes are retained raw there as well.
  `slot00`: 109 C functions, 12,240 bytes, are exact and 4,964 bytes are
  retained raw; `slot08` is the same 109 functions linked a second time,
  exact against its own chunk. `slot2b`: 102 C functions, 9,052 bytes,
  are exact and 23,516 bytes are retained raw; `slot2c` is the same 102
  functions linked a second time, exact against its own chunk. `slot0f`:
  215 C functions, 32,712 bytes, are exact and 71,136 bytes are retained
  raw. The other modules are not in the build.
- No complete overlay source coverage, whole-program source inventory,
  ownership of game data, rebuilt disc, or live-gameplay observations are
  claimed.
- The repository's public-facing files are documentation, tooling and, since
  2026-10-05, the reconstructed PS1 source in `ps1/src/`. Building it needs
  local inputs that the repository does not contain.

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
| Reproducible matching build | Range ownership, original-address linking, fallback accounting, build manifests, fail-closed byte checks | Working for the resident image: code, read-only data, initialised data, bss and assembly owners. Module images are built and compared the same way, each linked alone; four are declared, one of them as a second link of another: the same units at the other address, compared with its own chunk |
| Gameplay reconstruction | Expand meaningful connected routines/subsystems with readable types and named data | Resident image: 1,754 functions (1,367 game, 387 library) of the 1,852 that a sweep of its code counts (1,436 game, 416 library). Module images: `slot2a` 28 functions, `slot0b` 56 functions, `slot12` 178 functions, `slot16` 11 functions, and the same 11 linked again in `slot17`, `slot00` 109 functions, and the same 109 linked again in `slot08`, `slot2b` 102 functions, and the same 102 linked again in `slot2c`, `slot0f` 215 functions. Expanding |
| Coverage and exceptions | All scoped executable code accounted for; reviewed C/assembly, SDK handling, explained exceptions | Planned |
| Reproduction and delivery | Clean rebuild, complete code-payload comparisons, source/provenance review, operating instructions and remaining limits | Planned |

Continue by adding units to the matching build, following callers and callees
of the matched group. Add data ownership to the build when a unit first needs
it. Do not start a new generic architecture project or require a full-disc
builder before extending useful source coverage.

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
