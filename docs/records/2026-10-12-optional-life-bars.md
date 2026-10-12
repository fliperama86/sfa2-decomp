# Optional life-bar pixel grid, 2026-10-12

Source commit: `cdbbd4f6e863884c11f5e72feea2af4b3ce31735`, based on the approved
optional-graphics merge `ae17f06b`. Subsequent record-only changes need no retest.

The existing default-off `pixel_perfect_ui` setting now includes the life-frame
16x16 tiles. Source artwork already has the needed detail. The selected PS1
producer uses mixed UV spans for some life tiles; the shared decoded correction
preserves a full-span axis and extends only the short one. No imported assets,
health logic, original game source or canvas behavior changed.

The one-to-one frame has a six-row interior. Original initialization gives
both yellow and delayed-damage fills y18 and height5; the source frame interior
is y17..22. Only eight exact health-buffer TILE addresses on their correct HUD
lists can be marked. The backend renders those valid fills at y17, height6,
keeping X, widths and colors. Packet geometry in game RAM is untouched. This
is interior alignment, not the next HUD-centering step. Zero-width fills stay
empty. Bit0 of untextured TILE is unused by the legacy conversion; the backend
also checks the setting and geometry, so stale markers retain original output
when disabled. SDK protocol2 rejects older libraries before game compilation.

## Scoped evidence

- Three `test_video.py` controls pass in under one second: synthetic geometry/
  exact-record policy, complete patch/header installation, and rejection of
  missing/version1 SDK metadata. All eight fills, widths0..144, mirrored/mixed
  UV tiles, damage endpoints, malformed packets and default-off byte identity.
- Changed PsyZ build-tool controls pass in about five seconds. All ten patch
  hunks also apply to the exact dependency pin. The staged header is identical
  to the project header; the compiled backend remains identical after a
  patch-header prose edit. Dependency source/submodule stayed untouched.
- Only the changed SDK backend and GPU bridge were compiled, plus the updated
  generated fixture. Cached normal game/library objects were relinked; the
  normal Windows port's PE layout check passes. Main/settings evidence from
  PR341 is reused, not repeated. No whole-game build or game replay.
- The silent, short-lived Windows Direct3D12 fixture exits0. All4,096 prior
  corrected checker pixels and1,024 mixed-span/mirrored pixels pass. Full,
  partial, delayed-damage and empty life fills match their synthetic six-row
  expectations. Enabled-unmarked and disabled-marked fills are pixel-identical
  and keep five rows. The previous unmarked checker control still differs at
  256 pixels; marked-off sampling still equals legacy. Original/optional/other
  canvas-size checks pass.

Private source/log/SDK/executable hashes and actual exit records remain in
stable `port/build/`. An initial status-collection shell wrapper failed on a
reserved variable after the policy assertions reported success; its evidence
was preserved. The expanded final fixture then passed with exit0 recorded.
No failed/incomplete wrapper is counted as passing evidence.

Publication review: new policy/fixtures/docs are project-authored, addresses
and geometry come from already published reconstructed source. No game assets,
raw binary data, dependency/compiler snapshots, private machine paths or
credentials enter Git. The existing dependency patch retains its MPL notice.

Not established: full-game/all-UI/color equivalence, arcade HUD placement or
true16:9. The next reviewed increment is wider-canvas UI centering/anchoring
behind a separate settings flag. Poll owner requests/approval every60 seconds.
