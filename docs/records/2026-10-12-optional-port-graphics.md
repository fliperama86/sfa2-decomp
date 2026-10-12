## Opt-in port graphics (2026-10-12)

The owner requests publication of the accepted graphics changes as a reviewed
PR, not another private build. Integrate super-meter texel mapping and the full
arcade canvas in the normal port, independently controlled by default-off flags
in [experimental]. Preserve original sampling/canvas with both disabled.

Project-authored source now lives in port/src, with typed settings load/save,
startup/CLI selection and shared geometry/HUD policy. Mark only the known
super-meter tile pool/ordering-table combination in the library bridge. The
original PS1 producer/source and all assets remain unchanged. The existing
MPL-licensed PsyZ patch applies the tested decoded-corner mapping and optional
384x224/4:3 presentation; unsupported GL enhancements refuse explicitly.

Scoped evidence from the changed tree based on cee262bc:
- Synthetic settings fixtures pass on Linux and Windows, including independent
  flags, strict rejection, transactional loading and atomic save/preservation.
- Real startup-parser unit fixtures pass. The newly linked normal Windows port
  reports all four flag combinations and CLI overrides without disc/game/audio
  initialization; malformed values refuse with the expected status.
- Synthetic geometry/HUD selection tests pass, including mirrors, UV256,
  invalid geometry and byte-identical packets with the feature disabled.
- Complete published patch/header tests and psyzbuild controls pass. Hunks
  also apply to the exact pinned dependency source, which remains untouched.
  Updated SDK metadata advertises the new API; stale library builds refuse
  before any game-source compilation rather than wasting a whole build.
- Normal component build compiled changed startup/settings/graphics/backend units. Cached
  normal game/library objects reused, resulting PE verified. No whole-game build.
- Updated launch-test runtime list/video stand-in compile and link in one
  isolated smoke check; no legacy case suite was rerun.
- Windowed silent Windows Direct3D12 fixture passes all 4,096 corrected checker
  pixels; unmarked negative controls differ at 256. A marked quad with the flag
  off is pixel-identical to its unmarked legacy control, proving the backend
  cannot activate solely from the marker bit. Original 368x240 capture before/
  after opting in, optional 384x224 capture and other 320x240 mode pass.

First startup-parser fixture link failed because its test-only stand-in for
port_skip_programs was missing. Added it and reran only that changed fixture;
the already successful settings fixture was not repeated. Failed logs remain
private. Source input hashes and actual child exits are retained in port/build.

First-publication review: new C/Python/config/fixtures are written by this
project; no game binaries/assets, dependency/compiler binaries, private paths,
credentials or decompiler output enter Git. Dependency patch context retains
its existing MPL notice. No game boot/replay or unrelated suite was run.

No claim of all-UI/charge-state coverage, new artwork, completed life bars or
true16:9. After owner approval/merge, next small flagged PRs are life bars and
then centering/anchoring all UI for the wider canvas. Poll owner comments every
60 seconds for approvals and requested changes.
