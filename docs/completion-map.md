# SFA2 completion map

The Japanese PS1 reconstruction is **not complete**. The resident image has
high function coverage, but most overlay contents have no declared source
owners yet. Matching whole images still depends on retained original bytes.

Snapshot: 2026-10-06, after [PR 70](https://github.com/fliperama86/sfa2-decomp/pull/70),
at [revision 331e382](https://github.com/fliperama86/sfa2-decomp/commit/331e382f0b32fb22ffa8ca79920928fa1421f051).
Target and completion contract: [plan](../PLAN.md), [requirements](requirements.md).

## Map

[![Coverage map: one square per function of the static sweep, green where the build owns it](https://fliperama86.github.io/sfa2-decomp/completion-map.svg)](https://fliperama86.github.io/sfa2-decomp/)

One square per function that the static sweep lists, in address order: green
where a C unit of the matching build owns its bytes, blue where an assembly
unit does, grey where no unit does. The left panel is the resident
executable, the game code in one block and the Sony library by
[family](../ps1/docs/library-families.md). The right panel is the overlay
modules, one block per distinct content of a slot, labelled by slot where the
slot has one content and by archive where it has several. Its unit is the
function placement: the contents linked a second time, `0x8` and `0x17`,
count their functions again and are drawn in the darker green. The
repository's workflow draws the picture from the
[inventory](../ps1/inventory/README.md) and the build configuration on every
push to `main`, so it shows the current state, while the counts below are
the pinned snapshot. [Its page](https://fliperama86.github.io/sfa2-decomp/)
lists every block and links the counts as JSON; opening the picture itself
and hovering a block shows its name and counts.

A square is exact when units own all of its bytes, functions and data
together, or own it without a gap from one of its ends with a function among
them: the sweep's boundaries are estimates, and `ps1/tools/coveragemap.py`
states the rule. Two places differ from the sweep. The first function of
slots `0x0` and `0x8` the sweep begins 16 bytes early, at a table that a unit
owns as data. The program entry routine the sweep makes 16 bytes longer than
the assembly unit, because the four table words after it, retained raw,
follow without a return. Both count as exact; the 16 raw bytes do not.

## At a glance

```text
PS1 Japan SLPS_004.15
  +-- Baseline and matching tools: established
  +-- Resident executable: 1,754 / 1,852 estimated functions exact
  |     +-- Game: 1,367 / 1,436, 69 candidates still parked
  |     +-- Library: 387 / 416, 29 functions still unowned
  |     +-- Data: partial ownership, not complete
  +-- Overlays: 7 / 77 analyzed slot/content pairs in the build
  |     +-- 5 source groups, 2 additional links of shared objects
  |     +-- 501 / 11,220 estimated function placements exact
  |     +-- Most character and stage contents: not in the build
  +-- Delivery: source published, fresh-clone build pipeline unfinished

Windows research: tested nonmatching reference, outside PS1 completion
Ports, other revisions and rebuilt-disc packaging: separate scope
```

These are separate measures, not one whole-game completion percentage.
Function boundaries and the game/library split are static estimates. Overlay
counts include placements of shared code at different addresses, not that
many unique source functions. See the [inventory method](../ps1/docs/overlays.md)
and [family analysis](../ps1/docs/library-families.md).

## What counts as progress

| State | Meaning |
| --- | --- |
| Exact C | Readable source compiles and links to all claimed original bytes at the correct mapping. |
| Exact assembly | Reviewed assembly owner, counted separately from C. |
| Owned data | A source unit reconstructs and compares its declared data range. |
| Parked candidate | Source exists but is not exact; it is not a build owner. |
| Retained raw | Original bytes copied through, zero recovered-source progress. |
| Tested nonmatching | Scoped behavioral evidence, not an exact match or PS1 source-coverage credit. |

Names, types and roles are inferred unless explicitly established otherwise.
An exact match does not prove a unique historical source form or compiler.

## Resident executable

| Measure | Exact | Estimated total | Remaining |
| --- | ---: | ---: | ---: |
| Game functions | 1,367 | 1,436 | 69 |
| Library functions | 387 | 416 | 29 |
| All functions | 1,754 | 1,852 | 98 |
| Function code bytes, C plus assembly | 289,664 | 348,000 | 58,336 |

The exact functions comprise **1,701 C functions** and **53 assembly
functions**. Code-byte coverage against the sweep is **83.2%**, while function
coverage is **94.7%**. Neither measures the completeness of game data or
overlays. The library count includes 302 C functions reconstructed from the
SDK reference; it is not all project-authored gameplay C.

The 69 remaining game functions have parked candidates. Library exceptions,
unidentified routines and consistent ABI/type documentation still need their
own accounting. [Current work and unresolved questions](goals.md).

## Overlay map

Each content is counted once per slot, regardless of how many archives carry
it. Exact counts below mean declared build owners, not matching bytes found
incidentally elsewhere. Totals come from the static sweep.

| Slot | Contents | Exact function placements / estimated total | State |
| --- | ---: | ---: | --- |
| `0x00` | 1 | 109 / 112 | In build; 3 parked |
| `0x01` | 1 | 0 / 124 | Not in build |
| `0x04` | 24 | 0 / 4,540 | Character contents not in build |
| `0x05` | 20 | 0 / 3,823 | Character contents not in build |
| `0x06` | 20 | 0 / 874 | Stage contents not in build |
| `0x08` | 1 | 109 / 112 | Second link of `0x00`; same 3 parked |
| `0x0b` | 1 | 56 / 61 | In build; 5 parked |
| `0x0f` | 1 | 0 / 228 | Not in build |
| `0x12` | 1 | 178 / 187 | In build; 9 parked |
| `0x16` | 1 | 11 / 11 | All swept functions exact; data still retained |
| `0x17` | 1 | 11 / 11 | Second link of `0x16`; data still retained |
| `0x27` | 1 | 0 / 212 | Not in build |
| `0x28` | 1 | 0 / 683 | Not in build |
| `0x2a` | 1 | 27 / 28 | In build; 1 parked |
| `0x2b` | 1 | 0 / 107 | Not in build |
| `0x2c` | 1 | 0 / 107 | Not in build |
| **Total** | **77** | **501 / 11,220** | **7 contents built, 70 not built** |

The five source groups own **381 C functions**. Second links add 120 verified
placements, not 120 newly recovered functions. There are 18 parked functions
across those five source groups; their second-address copies are not extra
candidates. The first 16 bytes of slots `0x00` and `0x08` are a table, not
function code, as the [overlay map correction](../ps1/docs/overlays.md#counts)
records. All swept functions being exact does not make an entire module
source-complete while its data remains raw.

## Image byte ownership

All images listed here compare exactly, including their retained ranges.
Only the source-owned columns represent reconstruction progress.

| Image | C code bytes | Assembly bytes | Owned data bytes | Retained payload bytes | Payload bytes |
| --- | ---: | ---: | ---: | ---: | ---: |
| Resident | 288,624 | 1,040 | 8,252 | 316,484 | 614,400 |
| `slot00` | 12,240 | 0 | 16 | 4,964 | 17,220 |
| `slot08`, linked again | 12,240 | 0 | 16 | 4,964 | 17,220 |
| `slot0b` | 5,652 | 0 | 0 | 25,784 | 31,436 |
| `slot12` | 26,224 | 0 | 24 | 93,944 | 120,192 |
| `slot16` | 984 | 0 | 0 | 10,406 | 11,390 |
| `slot17`, linked again | 984 | 0 | 0 | 10,406 | 11,390 |
| `slot2a` | 5,356 | 0 | 0 | 15,840 | 21,196 |

Resident owned data is 2,352 read-only bytes plus 5,900 initialized writable
bytes. Module owned data in this table is read-only. Uninitialized data has
no payload bytes and adds no byte coverage. The resident's separate
2,048-byte executable header is retained and excluded from the payload table.
Resident source ownership covers **48.5% of its payload**, including data;
the other **51.5% is raw**. Raw includes data and unrecovered code, so it is
not a count of remaining gameplay code. Linked-again rows do not earn new C
source credit. [Ownership rules](../ps1/docs/matching-build.md#ownership-model).

## Route to completion

| Workstream | State | Completion condition |
| --- | --- | --- |
| Baseline and mapping | Established, with open loader questions | Preserve pinned inputs; resolve remaining destinations and ambiguous boundaries. |
| Matching build | Working for declared images | Keep complete-range comparisons, explicit owners and fail-closed controls as coverage expands. |
| Resident gameplay | In progress | Resolve the 69 parked functions, document types and connected behavior. |
| Sony library | In progress | Resolve 29 unowned functions and finish named SDK/assembly/provenance exceptions. |
| Overlay source | Main coverage gap | Declare remaining contents, reconstruct readable code and validate every address-specific link. |
| Game data | Partial | Replace relevant retained tables and state data with explicit, validated source owners. |
| Source quality and exceptions | In progress | Audit compiler-specific forms, ABI/type consistency and inferred meanings; retain explicit exceptions. |
| Reproducible delivery | Not complete | Provide fresh-clone input import/build instructions, dependency manifests and published-source licensing. |

The active direction is more overlay source, with compiler-pattern work to
unblock parked functions. The unresolved loader/data-symbol questions and
library decisions are tracked in [goals](goals.md) and
[project memory](project-memory.md). No new priority or port commitment is
implied by this map.

Completion requires reviewed source or documented assembly/SDK exceptions
for all scoped code-bearing payloads, readable gameplay logic, data ownership
and reproducible delivery. A byte-identical copy-through build is not enough.
No game boot, runtime injection, rebuilt disc or gameplay replay is an
additional mandatory gate for already exact mapped code. Nonmatching source
needs its own scoped behavioral evidence.

## Keeping the map current

Update this snapshot when ownership changes. Derive exact function counts and
range sizes from [`build.toml`](../ps1/src/build.toml), byte categories from a
passing matching-build report, and denominators from the documented inventory.
Record sweep corrections explicitly. Do not count second links as new source
or infer subsystem completion from a high function count. Historical pilot
reports remain historical evidence; update this map rather than rewriting them.

The picture needs no step of its own: the workflow in
`.github/workflows/coverage-map.yml` draws it from
[`ps1/inventory/`](../ps1/inventory/README.md) and `build.toml` on every
push to `main`, publishes it to GitHub Pages, and checks every pull request
the same way. The inventory is the one thing to regenerate, when the symbol
file or the configuration moves a boundary; it needs the executable and the
extracted archives:

```sh
.venv/bin/python ps1/tools/coveragemap.py sweep EXECUTABLE PAC_DIRECTORY
python3 ps1/tools/coveragemap.py render --svg out/completion-map.svg \
    --json out/completion-map.json --html out/index.html
```

`render` refuses an inventory that the build has outgrown: a declared
function that touches no swept function fails the check, and the sweep must
run again. It prints the counts of the two panels and every place where the
sweep took in bytes that no unit owns. `ps1/tools/test_coveragemap.py` holds
its control cases, on synthetic inputs; the workflow runs them first.
