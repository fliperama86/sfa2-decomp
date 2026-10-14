# Work package: first intro scene effects

## Objective
Extend the owner-tested Gouki/fire slice with its code-derived fade/flash
transitions and moving embers, in the normal port behind the existing
arcade_intro setting. Base: PR370, merged as 23ed5b41 after owner approval of
416577f2. Sound and later scenes remain separate increments. Provide another
runnable owner-test build before opening this increment's PR.

## Pinned contract
Use the same investigated US 960306 arcade input and separately identified
CPU-data/opcode planes as the first slice. Preserve the existing input pins,
privacy safeguards, disabled path and no-interpreter rule. No PS1 changes.

Recovered entry points define the scope: first-scene dispatcher and phases
0x27ae through 0x2d30, palette helpers around 0x43ba, row-scroll setup/update
0x44e0/0x4510, spawn helpers 0x451c/0x4560/0x45bc, and actor lifecycle beginning
at 0x73cb0. PC-relative control tables use the decrypted program plane during
analysis; indirect ROM asset reads use the original CPU data plane.

Do not implement an effect by guessing its duration, easing, palette or
trajectory from a picture. Preserve the phase timer/substate ordering, sprite
initialization versus update, unsigned bounds, wrapping fixed-point motion,
sequence holds/branches, and object priority where established by the source.
Use explicit callbacks/scripted results for random/allocation calls in scoped
behavioral fixtures, then recover the original RNG separately before claiming
its runtime particle sequence. A successful frame comparison alone cannot
establish controller equivalence.

## Scope
Feature source and importer under port/intro/, shared normal runtime hook only
as required, focused synthetic controls and this contract/evidence. Keep the
normal build/settings model, Start skip and return-to-title behavior. Do not
replace sound, other game programs, later scenes, or port-wide assets.

## Acceptance commands
- New controller/particle unit controls in port/tools/, synthetic data only,
  expected seconds. Pin each function's contract before writing those controls.
- Scoped original-routine comparisons, two seeds and negative controls, for
  reconstructed behavior that is not byte-identical; seconds per bounded unit.
  Record fixtures and read/write/control-flow bounds; no whole-game emulator.
- Importer/pack controls only for formats that change. Reuse unchanged sequence,
  settings, GPU and original asset decode evidence.
- One normal port build for changed runtime C, reusing the current library and
  canonical outputs. No PS1 matching build. Owner gets the runnable artifact.

## Evidence
Private analysis, tables, asset packs, captures and comparison reports reuse
port/local/intro-asset-probe/. Published records contain authored source,
synthetic fixtures, contracts and scoped conclusions, never original tables,
bytes or decompiler output. Distinguish recovered behavior from display or
clock approximations inherited from the port.

## Decisions and escalation
Resolve before implementation: the fade helper's exact palette-bank/latch
semantics, frame-pipeline phase carried into the steady scene, RNG/state and
allocation behavior, all particle layout modes and clipping bounds, and the
first scene's final handoff. Record code-derived answers rather than silently
carrying the first slice's normalized effect phase into a fidelity claim.
Pin any expanded private-pack version and old-pack handling explicitly.
For a delegated mechanical package, STOP on a missing contract, interface
change or material tradeoff; report options instead of choosing.

## Completion
Owner-tested normal-port effects, scoped evidence and publication inspection,
then a small PR. Monitor owner comments every 60 seconds until handled;
merge only after explicit approval of the current head.

## Investigation started
Reused the retained first-scene and effect disassembly instead of rerunning
analysis. The three spawning phases do not map their random interval to the
same sprite subtype: the steady helper sets its interval first, then maps it
to subtype 1, 3 or 4. The flash-phase helper changes both subtype and render
priority/layer. Preserve that distinction when writing the particle contract.
No new controller C or behavioral proof is claimed by this task document.
