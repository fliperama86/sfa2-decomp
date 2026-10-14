## Intro review control repairs and active monitoring (2026-10-14)

Owner review of PR370 at 52e6153c requested two control-only corrections:
main/program dispatch now reference intro APIs, but the shared synthetic launch
rig lacked their definitions; the no-PsyZ GPU export assertion also omitted
three new public functions. The inherited pad and module rigs use the same
launch stub text. Keep their scope narrow: supply typed disabled-feature
stand-ins, reject enabled requests, and update the GPU export expectation.
No scene, game, assets or runtime behavior changes.

Validation is restricted to one generated launch executable link and the GPU
control's first two compile/nm assertions, without running any generated
executable, GPU, game, full suite or rebuild of the owner-tested port. Results
will be recorded separately against the exact repair commit.

The owner also corrected PR monitoring: ending a turn with "awaiting review"
stops polling. Do not claim an ongoing watch while ending the turn. Keep the
60-second comment watch active, handle requested changes, and merge only after
explicit approval from the owner account on the current PR head.

Lesson: when adding a normal-runtime API, inspect the explicit source lists,
shared stand-ins and exact export assertions of existing controls, not only the
new feature's controls. This is dependency maintenance, not a reason to rerun
unrelated suites or the game.
