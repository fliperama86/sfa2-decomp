## Native intro controllers and actor reconstruction checkpoint (2026-10-14)

The normal tracked port tree now contains C for all eight scene controllers,
the shared actor pools/map/palette operations, eleven primary actor kinds and
three secondary kinds. This is an implementation checkpoint, not completion:
logo/duel actors, remaining background/palette actors, full fade scheduling,
lettering, resource import, render-list integration and sound remain unfinished.
The existing executable still enters the earlier first-scene slice. It was not
launched or offered for owner testing. No PR was opened, and `ps1/` was untouched.

The state buffer preserves the original game's big-endian byte/word aliases,
not CPU state. Native functions implement the state transitions. Read-only
callbacks will receive typed data resources, never original instructions.
Histories used by trailing sprites are produced by native movement during the
run, not extracted original frame recordings. The contract is in
`port/intro/director-contract.md`.

### Scoped author evidence

Private, bounded construction comparisons, with seeds 0 and 7:

- Director/world: 577 cases per seed, all eight controller families, initialized
  and dirty reset state, game RAM and video equality. Actor/presentation/title
  dependencies are stand-ins, not an assertion that those dependencies work.
- Basic actors: 7,605 / 7,503 isolated calls, RAM equality for kinds 80, 82, 84.
- Portrait/strip/fighter actors: 11,019 calls per seed, RAM equality for kinds
  83, 85, 86; the seed-7 run also compared video writes.
- Montage actors: 7,414 / 7,423 calls, RAM and video equality for kinds 87, 88,
  89, 8a, 8e.
- Secondary effects: 1,352 calls per seed, RAM and video equality for kinds
  20, 21, 22, with the original parent routines providing their dependency data.

These are isolated original-routine calls, not a game boot or complete native
playback. Comparison excludes the original machine call stack and scheduler
job storage; the oracle rejects original writes to ROM. The construction
scripts and original-resource inputs remain private. Their limitations remain
explicit; this is not yet the final comprehensive resource or write-boundary
validation for the integrated intro.

`python port/tools/test_intro_native.py`: two controls passed in 7.109 seconds,
34 original-computed synthetic fixtures, undefined-behavior sanitization, and
four real source mutations rejected (row wrap, omitted double actor update,
signed exit boundary, aliased completion value). Fixtures need no private input
and contain no extracted game resource bytes. The recording oracle supplied
synthetic zero data and checked its resource-read set. Publication review:
project-authored C/test source, synthetic computed results, no game binary,
third-party source, personal path/name or credential.

Normal Windows build, four jobs: 3,982 units compiled, zero failed, executable
linked and address verification passed. This proves the new unused native
units integrate into the ordinary build, not that the complete intro runs.
No unrelated PS1, existing sprite/video, sequence, particle, settings or GPU
suite was rerun. Existing adapter evidence remains at 689c1cae.

### Corrections preserved during construction

The first compile caught misleading indentation, fixed without changing the
intended branches. Director comparisons exposed the map-clear buffer flags and
its returned scalar being one, which the original strip controller retains as
its initial scroll values. Those were corrected from original execution rather
than visual adjustment. An actor harness initially failed because it did not
supply the draw-list reset routine's zero argument; the test harness was fixed.
Failed construction logs remain beside the passing logs. Fixture recording's
first output path was wrong and created no file; the corrected destination is
the normal port tree. No failed attempt is presented as a passing result.

Source/evidence hashes are retained in the private
`native-controller-evidence-manifest.json`. The owner still wants notification
only for a complete intro, not this intermediate checkpoint.
