# Work package: recover the intro's background assets

## Objective
Make the first Saturn intro state's background-map decoding reproducible,
then narrow the existing arcade/PS1 pixel comparison to those dependencies.
This is the asset-recovery increment, not a playable intro scene.

## Pinned contract
`port/intro/README.md` defines the selected revision, decoding, outputs and
limits. Decode the twelve map uploads referenced by the first state's setup.
Export separate pages, not a guessed screen composition. Keep game data and
analysis private. Reject other revisions, malformed references and overwrites.
Tests use made-up pixels and maps, not copyrighted fixtures.

## Scope
Own `port/intro/`, its control in `port/tools/`, the control-selection entry
and this change's documentation. No changes anywhere under `ps1/`, to game
source, build tables, runtime behavior or shared settings. A later runtime
increment must use the normal port and a default-off experimental setting.

## Acceptance commands
`python port/tools/test_intro_assets.py`: exit 0, subsecond synthetic tests,
including quadrant order, nibble order, flips, alpha, pointer bounds, revision
checks and existing-output refusal. No game, compiler or hardware needed.
Run the decoder once on the already extracted, hash-pinned supplied inputs.
Inspect representative output pages. Reuse the existing cross-version tile
catalogs rather than scanning unchanged discs again. No port or PS1 build:
no compiled source or build input changes in this package.

## Evidence
Private `port/local/intro-asset-probe/`, with hashes of inputs and outputs.
Record the exact source commit and commands. Public change record summarizes
findings and limitations without extracted tables, pictures or pseudocode.

## Decisions and escalation
Do not turn background-page coordinates into screen coordinates without the
display mapping. Do not equate Saturn timing or colors with arcade behavior.
Sprite composition, sound, complete scene playback and attract mode are later
work, not silently approximated here. On a material format ambiguity, report
the evidence and options instead of guessing.

## Completion
Scoped evidence, publication/provenance review and a small PR. The original
request for a port-only intro remains incomplete until scene playback exists.
