# Work package: optional port graphics

## Objective
Publish the accepted super-meter texel mapping and full arcade canvas in the
normal port, behind independent, default-off settings flags.

## Pinned contract
`settings.ini` uses `[experimental]`, with boolean `pixel_perfect_ui` and
`arcade_video`. Both default false. The first flag corrects only super-meter
16x16 textured HUD tiles; it does not generalize all graphics or life borders.
The second selects the full 384x224 native canvas with 4:3 presentation, not
true 16:9. Disabled paths preserve existing PS1 canvas and sampling. Movie and
other display modes remain unchanged. A typed C loader/saver is shared with
the future settings UI; CLI overrides address the same values.

## Scope
Tracked `port/src`, `port/tools`, existing PsyZ patch/build integration and
documentation. No PS1 source changes, game assets, binary/source snapshots of
dependencies, per-experiment source folders or bespoke runtime variants.
Tag only the known super-meter tile-buffer/ordering-table combination in the
port graphics bridge; retain original producer C. Apply the decoded-corner
correction in the pinned SDL_GPU backend, preserving every unmarked primitive.
The optional canvas uses the same presentation behavior tested in the private
probe. Keep all newly written geometry/policy code in one shared header.

## Acceptance commands
Fast settings/geometry/packet-policy tests use synthetic fixtures only; test
all flag combinations, default preservation, mirrors and UV256 boundaries.
Patch tests use invented trees and the exact pinned dependency source. Compile
only changed runtime/backend units, reuse cached game/library objects, and
perform one bounded windowed generated-pixel test (silent, no disc/game).
Do not repeat unchanged test suites or build all game units. Expected seconds
per isolated test and minutes at most for targeted compile/link.

## Evidence
Private build/log outputs in existing `port/build/`, a public dated record with
head/scope/results. Review provenance/privacy before first publication. No raw
assets, machine paths, credentials or compiler/dependency binaries in Git.

## Decisions and escalation
Initial PR is the two flags and accepted graphics behavior, not later UI work.
Poll owner comments every60 seconds, act only on owner requests/approval, merge
after approval and checks. Then separate small flagged PRs: life bars first,
UI centering/anchoring for the wider canvas second. No long-lived feature branch.

## Completion
Integrated source, scoped evidence and reviewed PR; no universal arcade/UI claim.
