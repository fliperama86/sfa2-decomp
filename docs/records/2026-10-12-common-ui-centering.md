# Optional common-UI centering, 2026-10-12

Source commit: `cf44d359f5783e5cf3dcd37c5c732827209879c8`, based on approved
life-bar/selection merge `c45b07d8`. Record-only changes require no retest.

The shared model/config adds `center_ui`, default false. It only takes effect
in the active wider game canvas and is independent of UI sampling. Original
368-wide UI coordinates are centered in384 with an eight-pixel X translation.
No stretching, Y change, scene/camera translation or true16:9 implementation.

This is an integrated WIP increment: resident HUD tile/fill/strip buffers and
common text, including shared menu text. Screen-specific menu/overlay artwork
is excluded until separately identified/tested. It does not complete all UI.
Selection follows validated frame-table chains and exact producer buffers,
not draw depth alone or only AddPrim calls. World graphics may share a depth,
and some producers write links directly. Compound draw-mode/sprite packets
are supported. Original game RAM/geometry, widths, UVs, colors and command
lengths stay unchanged; only validated SDK copies are translated. Marked life
fills are corrected before translating X, composing both enabled features
without breaking the backend's original-position guard. Out-of-range copied
geometry refuses submission rather than silently wrapping it.

## Scoped evidence

- Native policy fixtures pass:72 supported geometry forms, malformed lengths/
  coordinate edges, transactional failure, life-fix composition, exact buffer
  boundaries and canvas/settings gates. The real no-SDK module refuses enabling
  the new option while accepting disabled behavior.
- A verbatim real list-walker/copy-seam fixture passes with a mock SDK sink,
  generated mapped RAM, both initialized tables and all eight flag states.
  Selected HUD/font/compound-sprite copies move correctly; world packets sharing
  their depth, single environment packets, source RAM and UV/control words stay
  unchanged. No SDK/GPU/game/disc is used for these subsecond checks.
- Typed settings API controls pass on Linux and Windows, including eight-state
  atomic save/load, new-key defaults/errors and transactional preservation.
  The real native startup parser cases pass. The normal linked Windows port
  also passes all eight settings states, CLI/last-override precedence and
  malformed-key-value refusal with actual exits0/2, before hardware/game/audio
  initialization.
- Only changed main/settings/GPU runtime units and fixtures were compiled.
  Cached approved SDK and normal game/library objects were reused. The normal
  PE layout check passes. Updated launch stand-in compiles/links; the no-SDK
  symbol check matches both optional setters, with no imports. No legacy case
  suite or whole-game build was run.
- Whole-module, silent windowed Windows Direct3D12 readback passes1,376,256
  generated pixels across both tables/all eight states: HUD quad, common font,
  compound sprite, life-fill composition, unchanged world/source RAM and an
  other-display-mode gate. This verifies placement and color classes, not exact
  texture RGB quantization or original-game palette equivalence.
- The workflow table's isolated validation passes and registers the new policy,
  seam and GPU fixtures. Existing SDK patch/build-tool and backend sampling
  evidence from PR342 is reused; those unchanged suites were not repeated.

Initial fixture failures are preserved privately: an extra test argument after
expanding the settings model, missing seam-fixture declarations, a Windows/SDK
header-name collision, and an overstrict packed-white texel expectation on the
unchanged baseline renderer. Fixed only those fixture issues and reran their
changed parts. Readback now uses explicit color classes, not calibrated source
art or a claimed exact-RGB image. No failed run is counted as passing evidence.
Stable `port/build/` holds source/artifact/log hashes and actual child exits.

Publication review: project-authored code, generated fixtures and docs only.
Layout constants come from already published producer declarations/init. No
PS1 edits, original assets, third-party source/binaries, SDK snapshots, private
machine paths or credentials enter Git. No new experiment-specific directories
or binary modes; the normal settings file controls the normal port.

Next: extend the same WIP flag to independently identified screen-specific UI
in small reviewed increments. Poll owner requests/approval every60 seconds,
merge only the approved head after checks pass.
