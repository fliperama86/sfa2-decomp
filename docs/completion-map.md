# SFA2 completion map

Every function that the static sweep finds in the Japanese PS1 program is
exact: the build owns its bytes from C (or, for 55 functions of the resident
executable, from assembly). **Code is complete; data is not.** The build
still copies tables, text and similar data from the original files, and the
function boundaries are estimates from a sweep, so this is not a claim that
the whole game is rebuilt from source.

Snapshot: 2026-10-13, at revision bc79760c. Target and completion contract:
[plan](../PLAN.md), [requirements](requirements.md).

## Map

[![Coverage map: one square per function of the static sweep, green where the build owns it](https://fliperama86.github.io/sfa2-decomp/completion-map.svg)](https://fliperama86.github.io/sfa2-decomp/)

One square per function that the static sweep lists, in address order: green
where a C unit of the matching build owns its bytes, blue where an assembly
unit does, grey where no unit does. The bar at the top is the one
whole-program share: distinct functions, each function of the resident
executable once and each function of the modules once per address-blind
form, the hash of the [module inventory](../ps1/docs/overlays.md#counts),
so that code the character modules share is counted once; a form is exact
when any placement of it is. The share of all placements and of all code
bytes stand beside it. The left panel is the resident
executable, the game code in one block and the Sony library by
[family](../ps1/docs/library-families.md). The right panel is the overlay
modules, one block per distinct content of a slot, named by the archives
that carry the content: the archive family where many do (`END`, `CONT`,
`CDEMO`, `BOSS+2` where three families share one content), the archive
stems where three or fewer do (`PL09`, `PL11+PL13`); the slot and the
archive count are in the block's tooltip. Its unit is the
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
  +-- Resident executable: 1,852 / 1,852 functions exact
  |     +-- Game: 1,436 / 1,436
  |     +-- Library: 416 / 416
  |     +-- Data: partial ownership, about 42% of the payload still raw
  +-- Overlays: 77 / 77 slot/content pairs in the build
  |     +-- 11,220 / 11,220 function placements exact
  |     +-- 3,748 distinct functions, 3,822 placements are second links
  +-- Names: 98% of functions, 68% of fields, 8% of data symbols
  +-- Delivery: source published, fresh-clone build pipeline unfinished

Windows research: tested nonmatching reference, outside PS1 completion
Port: runs on Windows and Apple Silicon Macs, see the port page
```

These are separate measures. The one whole-program share is the picture's
bar, distinct functions: 5,600 of 5,600. Function boundaries and the
game/library split are static estimates. Overlay counts include placements
of shared code at different addresses, not that many unique source
functions. See the [inventory method](../ps1/docs/overlays.md) and
[family analysis](../ps1/docs/library-families.md).

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

| Measure | Exact | Estimated total |
| --- | ---: | ---: |
| Game functions | 1,436 | 1,436 |
| Library functions | 416 | 416 |
| All functions | 1,852 | 1,852 |

The functions comprise **1,797 C functions** and **55 assembly functions**
(system call stubs, host-debugging stubs and the program entry routine). The
library count includes C functions reconstructed from the SDK reference; it is
not all project-authored gameplay C.

Declared ownership of the resident payload (614,400 bytes), from
[`build.toml`](../ps1/src/build.toml):

| Owned by | Bytes |
| --- | ---: |
| C code | 347,024 |
| Assembly code | 1,140 |
| Read-only data | 2,616 |
| Initialised data | 5,900 |
| Retained raw (data) | about 257,700 |

Resident source ownership covers about **58% of the payload**; the rest is
data copied from the original. Uninitialised data has no payload bytes. The
resident's separate 2,048-byte executable header is retained and excluded.
[Ownership rules](../ps1/docs/matching-build.md#ownership-model).

## Overlay map

Each content is counted once per slot, regardless of how many archives carry
it. Exact counts mean declared build owners, not matching bytes found
incidentally elsewhere. Totals come from the static sweep.

- 77 images of overlay modules are in the build, all exact: every function
  placement (11,220 of 11,220) is owned by C.
- 3,748 of these are distinct functions; 3,822 placements are the same objects
  linked a second time at another address, for the second side of a
  character, for example. They add no new source.
- The units own 876 bytes of overlay read-only data; the rest of the overlay
  data is retained raw.
- The [published page](https://fliperama86.github.io/sfa2-decomp/) lists
  every block (slot, archives carrying it, counts) and links the counts as
  JSON. Which folder holds which character, stage or screen is in the
  [source README](../ps1/src/README.md).

## Route to completion

| Workstream | State | Completion condition |
| --- | --- | --- |
| Baseline and mapping | Established, with open loader questions | Preserve pinned inputs; resolve remaining destinations and ambiguous boundaries. |
| Matching build | Working for all declared images | Keep complete-range comparisons, explicit owners and fail-closed controls. |
| Code | Every swept function exact | Keep the sweep's boundaries honest; review the forms found by search. |
| Game data | Partial | Replace retained tables and text with explicit, validated source owners. |
| Names and types | 98% of functions, 68% of fields, 8% of data symbols named | Name the rest; all names stay inferred unless originals are found. |
| Source quality and exceptions | In progress | Audit compiler-specific forms, ABI/type consistency and inferred meanings; retain explicit exceptions. |
| Reproducible delivery | Not complete | Provide fresh-clone input import/build instructions, dependency manifests and published-source licensing. |

The active direction is data ownership, naming and delivery. The unresolved
loader/data-symbol questions and library decisions are tracked in
[goals](goals.md) and [project memory](project-memory.md). The port is a
separate scope and runs beside this plan (see the
[port page](../port/README.md)).

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

`render` has one guard on the inventory: a declared function that touches
no swept row fails the check, and the sweep must run again. That is all it
establishes. A row stale within its range, shorter or longer than the
function it holds, passes, and the sweep's boundaries stay estimates beside
the declared ranges; the JSON gives the declared bytes next to the bytes
exact within rows, and they differ where the two disagree. `render` prints
the counts of the two panels and every place where the sweep took in bytes
that no unit owns. `ps1/tools/test_coveragemap.py` holds its control cases,
on synthetic inputs, the guard's limit among them; the workflow runs them
first.
