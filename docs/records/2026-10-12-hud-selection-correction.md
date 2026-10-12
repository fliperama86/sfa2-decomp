# HUD selection correction, 2026-10-12

Corrected source commit: `19548e1ca6ea3c6334cdcecbedbe06220a0201b8`.
This supersedes the runtime-selection claims in the earlier optional-graphics
and optional-life-bar records, preserving those records and their actual tests.

## Mistake and effect

`data_801987c8` is a pointer variable. The original initialization and frame swap
put one of two frame-table addresses into it. The HUD producer passes that
value plus the list offset. PR341 and the first PR342 head instead selected
storage-address offsets. Their synthetic tests repeated that mistaken premise.
The backend's generated pixel tests were valid, but they did not establish that
real game packets received markers. The published super-meter bridge would not
select its intended runtime packets, and the first life-bar head had the same
problem. No claim that those heads fixed real gameplay sampling is retained.

The source audit caught this before PR342 merged; the owner independently
confirmed it in a request-changes comment. No merge was performed on that head.
Validate declarations/initialization and the producer-to-bridge path, not just
an isolated backend or a symbolic-looking address. Active memory records this
lesson without removing the 60-second polling/review rules.

## Correction and scoped evidence

The shared policy now derives list indices from the two initialized 30-entry
frame tables. It restricts the textured HUD pool to the two initialized blocks,
not a speculative negative-selector block. Super index8 and life index14 are
selected on either table; yellow/damage index14/15 must also agree with the exact
health-buffer record. Pointer-storage offsets and unrelated/misaligned tables
are rejected. Disabled selection remains byte-preserving.

Three updated `test_video.py` controls pass with exit0. A separate generated-RAM
fixture links **real gpu.c**, stores each original frame-pointer value, reads
that pointer back as the producer does, calls real AddPrim through its library
table, and checks super/life/fill selection, link/length/geometry preservation,
both disabled/enabled settings and deliberate old-address negative controls.
Its compile/link and Windows child both exit0. It starts no GPU/window/audio,
reads no game/disc and needs no gameplay. The workflow table's own isolated
validation passes and includes this fixture in the video subject.

Only the changed backend/header and GPU bridge were compiled, and normal cached
game objects relinked with a passing PE check. The new bridge fixture is built
separately. Existing settings/build-tool and life/backend pixel proofs are
reused; the windowed pixel fixture was not repeated for this selector fix.
The render algorithms and patch from the life-bar commit are unchanged.
Private manifests/logs/binaries stay in stable `port/build/`.

Privacy/provenance: the fixture is project-authored, with made-up RAM/pixels and
published-source layout constants. No original asset bytes, source snapshots of
dependencies, private paths or third-party binaries enter Git. No PS1 code
changed. Full-game/all-UI equivalence and UI layout remain unproven/out of scope.
