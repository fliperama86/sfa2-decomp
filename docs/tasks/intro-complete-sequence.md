# Work package: complete native intro

## Objective
The owner's latest instruction is to finish the entire intro before requesting
another test or announcing readiness. The ember-only steady slice was rejected
as insufficient. Do not substitute partial scenes or cosmetic changes for this
acceptance boundary. Keep intermediate source tracked, buildable and default-off,
but do not open a PR before the owner can test the complete sequence.

## Scope and pinned evidence
Native, readable C derived from the supplied CPS2 program and its data. Saturn
remains a code reference when useful. The original first-scene-only contracts
and evidence remain historical and scoped, not proof of the whole intro.
No `ps1/` modifications, executable-byte retention, runtime original-code
interpreter, visual-mimic timing, or distributed game assets. Reuse the normal
program/settings/renderer and Start skip/return behavior.

The intro root at 0x2792 dispatches eight major phases, via the PC-relative
table at 0x279e: 0x27ae, 0x2d32, 0x302e, 0x32bc, 0x3468, 0x3606, 0x3d3a,
0x4190. The first of these alone supplied the previous test slice. Recover
all phase entry/update/exit paths, their actor consumers and visual/audio
resources. Private opcode/data-plane identity and mapping remain pinned.

## Implementation packages
1. Pin the complete phase/actor/asset/audio dependency graph. Treat unsupported
   layouts and unknown transitions as unresolved, never silently omit them.
2. Implement the shared bounded map/palette/sprite resource adapter required
   across all scenes. Import only needed data, never original instructions.
3. Reconstruct controllers and actor lifecycles in readable C, with scoped
   original-routine comparisons and synthetic fixtures. Integrate each buildable
   increment behind the existing flag without requesting owner tests.
4. Integrate the full sequence and sound, authored skip/return glue, validated
   private input and explicit importer limits. Do not use captured video.
5. Build the normal executable, verify complete sequence progression and
   resource coverage, then notify the owner. No partial-build notification.

## Acceptance and cost
New tools and C units have isolated, seconds-long controls. Reconstructed
nonmatching behavior gets scoped differential evidence on two seeds and actual
negative controls; reuse unchanged sequence/particle evidence. No PS1 matching
build or whole-game replay gate. One normal build per runtime increment when
needed, using the existing four-job limit and canonical outputs. Runtime
observation may answer a specific integration/clock/display question, not
substitute for original-code evidence. Automated launches remain muted or dump
audio. Final owner test must cover the complete intro, not just Akuma/flames.

## Evidence and delivery
Keep private tables, extracted images, disassembly, analysis and reports in the
existing ignored intro folder. Publish project-authored source, contracts,
synthetic fixtures and scoped records only. Complete importer/runtime behavior,
limitations and provenance must be documented. Owner testing precedes the PR;
then monitor only owner's comments every 60 seconds and merge only on approval.

## Implementation checkpoint

Shared graphics and metasprite adapters are tracked at 689c1cae, with scoped
evidence in [the adapter record](../records/2026-10-14-intro-complete-adapters.md).
Offline discovery reaches all eight original phases, including secondary actor
pools, but substitutes cooperative scheduling and is not a native intro.
Original-state captures are never runtime playback input. Native controllers,
actor lifecycles, typed resource import and sound remain outstanding. Do not
launch or announce the rebuilt executable: its opening route is still the old
bounded first-scene slice.

Native controller/actor work now exists in the normal `port/intro/` source tree.
See `port/intro/director-contract.md` for the implemented subset and remaining
dependencies. It is not connected to the opening runtime yet and is not a
complete-intro test build.

The next checkpoint implements all observed actor families, fades, maps,
lettering and native display-list assembly. Two scoped traversals compare all
eight phases through exit, including generated object records. Resource import,
independent startup, host cadence and sound are still required. No new owner
notification or PR is authorized by this intermediate evidence.
