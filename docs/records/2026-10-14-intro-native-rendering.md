## Intro reconstruction: all observed actors and native display lists (2026-10-14)

This checkpoint adds native map/palette/logo/duel actors, final tile lettering
and blinking UI, three palette-fade tasks, and display-list assembly. The old
first-scene executable route is not replaced yet. Complete import, independent
startup, host cadence and sound still need integration. No partial demo, owner
readiness notification or PR was issued. No `ps1/` changes.

## Evidence

Private original-routine construction, not a game boot or trace player:

- Core controller/actor/fade traversal: 2,948 logical ticks through all eight
  phases, first without title/UI, then with native title/UI. Both passed after
  the documented headshot register-residue projection was corrected.
- With native display-list assembly: two RNG seeds, 2,948 ticks each, game RAM,
  complete video memory and emitted objects through their terminator compared.
  Presentation still has an explicit display-boundary stand-in. Initial state
  comes from original setup, not a proven native initializer. CPU scratch,
  scheduler task storage and only the documented unused headshot high bytes
  are excluded. No original state or objects become runtime playback input.
- Map actors: 1,440 isolated calls per seed. Palette actor: 4,800 per seed.
  Both compare RAM/video; original writes to ROM fail. These construction
  comparisons are not a tight per-field original write audit.
- Map consumers: 2,000 randomized synthetic cases per seed, all three planes,
  strict PC/read/write audits. Original writes: 81,170 / 82,186.
- Fade consumers: 192 cases per seed, 393,216 palette words compared per seed,
  strict PC/write audits. Original writes: 223,863 / 222,842.
- Native fixtures: 53 original-computed synthetic cases, no extracted resource
  bytes; two controls with UBSan and four mutants passed in 9.372 seconds.
- Map controls: two tests, three mutants, 1.574 seconds. Fade controls: native
  color/timing/bank controls and three mutants, retained passing evidence.
- Presentation controls: two tests, four mutants, 2.866 seconds, UBSan.

The normal four-job Windows build linked successfully and passed address
verification. Its opening route is still the old slice; it was not launched.

Private logs and hashes live in the existing intro analysis folder. Original
program/data identity remains pinned by the previous records. New C and tests
are project-authored; no game assets, decompiler files, original instructions,
third-party source, credentials or personal paths enter Git.

## Corrections and limits

An isolated director test with cleared CPU registers had hidden a stale
caller-register upper word in the headshot completion initializer. Native C
keeps zero there, while the low word is two and actors later set explicit
completion markers. That is a deliberate nonmatching semantic projection, not
raw-state equivalence. The project memory records this lesson.

The first renderer construction failed at a scene clear: the original clears
its first object even without a normal presentation call. A native pending
blank-list event fixes that omission. The first presentation control run did
not detect two proposed mutants; one mutation was replaced with a meaningful
coordinate error and an assertion was added for the omitted blank-record
pointer. Both now fail as intended. A compile caught a hexadecimal-literal
spacing issue. UI block capacity was corrected to its actual in-slot bound
(47), rather than allowing a write into the following slot. Failed attempts
remain preserved, never presented as passing evidence.

Sound, real-time scheduling and complete runtime startup remain outstanding.
The experimental feature stays default off and is not ready for owner testing.
