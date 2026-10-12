# Work package: optional life-bar pixel grid

## Objective
Extend the existing default-off `pixel_perfect_ui` setting to life bars,
without changing game C, health logic, assets or HUD placement.

## Pinned contract
The known HUD tile pool on life list index 14 receives the same 16x16 decoded
UV correction as super list index 8. Mixed 16/15 UV spans are supported.
List indices come from the two actual initialized frame tables, not the storage
address of the pointer selecting them. Limit tiles to the two initialized blocks.
Only the eight original yellow/damage TILE records in the fixed health-bar
buffer, on their respective lists 14/15, can receive a fill marker. Valid
five-row fills at y18 render as six rows starting y17, matching the interior
of the one-to-one source frame. X, width, color and delayed-health logic stay
unchanged. Zero-width fills remain empty. Invalid/unselected packets and all
flag-off rendering retain the old path, including previously marked packets.
No recentering, new setting, asset import, color equivalence or true16:9 claim.

## Scope
`port/src/video.h`, GPU bridge, existing SDL_GPU patch, SDK cache protocol
version and their synthetic controls/documentation. No files under `ps1/`,
no SDK source snapshot and no experiment-specific folders or executable modes.
The marker uses bit0 of untextured variable TILE's command, ignored by legacy
rendering. The backend gates it on the setting and validates geometry again.
SDK metadata version 2 rejects stale version-1 libraries before compilation.

## Acceptance commands
`python port/tools/test_video.py` and changed build-tool controls take seconds.
Compile only the changed SDK backend and GPU bridge, then relink cached normal
game objects and check the PE. The generated silent windowed GPU fixture must
check mixed-span/mirrored sampling, six-row full/partial/damage fills, empty
fills, unmarked controls and flag-off pixel identity. No game/disc input or
whole-game build/replay. Reuse unchanged settings/startup evidence from PR341.
The generated-RAM bridge fixture links real gpu.c, exercises both original
frame-pointer values and rejects the former mistaken storage-address offsets,
without initializing a GPU/window/audio.

## Evidence
Stable private `port/build/` for hashes/logs and artifacts, public dated record
for results and limits. Original source geometry and existing private raw-HUD
comparison establish the six-row interior, not a new running-game capture.

## Decisions and escalation
No unresolved decision. Keep UI centering for its own subsequent flagged PR.
Poll owner comments every 60 seconds, apply requests and merge only the
approved head once its checks pass.

## Completion
Scoped tests and privacy/provenance review, reviewed PR and approved merge.
