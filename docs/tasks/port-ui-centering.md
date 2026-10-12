# Work package: optional common-UI centering

## Objective
Start the wider-canvas UI layout work in a small integrated increment: center
all resident HUD pieces and common text, without translating the fight scene.
Keep screen-specific artwork as explicit WIP, not a claim that every UI screen
is already handled.

## Pinned contract
Add `[experimental] center_ui = false` to the shared settings model. It only
takes effect when `arcade_video` has activated the wider game canvas, independently
of `pixel_perfect_ui`. Center original 368-wide UI coordinates in the 384-wide
canvas with an eight-pixel horizontal translation, not stretching or changing
Y. Default/off/other video modes retain original positions. No true16:9 claim.

Select the two initialized resident HUD tile blocks, exact health/super/custom
fill records, resident HUD strip buffers and common text SPRT records, only
within an actual initialized frame-table chain. Derive membership from packet
addresses and producer declarations/initialization. Do not classify by draw-list
depth alone: world graphics can share HUD depths. Screen-specific overlay
artwork is excluded until separately identified and tested behind this flag.

Translate copies in the existing validated packet-to-SDK seam, never game RAM.
Decode every accepted geometry command in a selected packet, preserving all
UV/color/width/control words and command lengths. Unselected world packets and
single environment packets are unchanged. Correct marked life fill geometry
before translating X so both enabled features compose without failing the
backend's original-position guard. Malformed/out-of-range geometry fails closed
before submission. Original `ps1/` source and assets stay untouched.

## Scope
Shared settings/startup, GPU bridge, new project-authored pure layout policy and
isolated fixtures. No SDK API or patch change, per-experiment folders, bespoke
build modes or game-source rewrite. SDK protocol2 and backend proofs carry over.

## Acceptance commands
Fast native settings/startup and synthetic layout controls, including all flag
combinations, every supported geometry form, mixed command packets, exact buffer
edges, original/default/world identity and composition with the life fix.
A real-bridge copied-packet fixture and one generated-pixel layout readback use
made-up RAM/graphics, no game/disc/audio. Compile only changed runtime/fixture
units, relink cached normal game objects and verify PE. No whole-game build or
unchanged settings/backend test rerun after source freeze. Expected seconds.

## Evidence
Stable private `port/build/` for exact source/artifact/log hashes and exits,
public dated record for tested scope, WIP exclusions, provenance and reuse.

## Decisions and escalation
Initial increment is resident HUD/common text only; complete screen-specific
UI coverage follows in subsequent small PRs behind the same default-off flag.
Keep scene/camera geometry unchanged. Poll owner requests/approval every60
seconds; merge only the approved head after scoped checks pass.

## Completion
Tracked WIP integrated behind its settings flag, scoped evidence and approved
merge. No assertion that all menu/overlay artwork or true16:9 is complete.
