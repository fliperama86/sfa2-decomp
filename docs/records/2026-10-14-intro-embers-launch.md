## Ember build launch and stale Windows image (2026-10-14)

The owner requested launch after b7dbfb7e's test-build handoff. The first launch
refused the version-2 pack at its header/length check. The executable on disk
contained both supported versions and the new ember code. A tiny isolated
Windows file-read probe confirmed the new pack's exact length, magic, EOF and
error-free read. No game source or loader change was necessary.

An earlier owner-test process was still running the same worker executable
path with the old version-1 pack. Its identity and path were checked; only
that prior test instance was closed. Relaunching the unchanged new executable
with the unchanged version-2 pack then passed startup validation and opened
the normal PSY-Z window. This supports a stale Windows loaded-image diagnosis,
not a bad pack or source-code fix. Do not rebuild unchanged source to work
around this; close the identified earlier test copy before the relaunch.

The new launch is windowed and muted, uses the supplied PS1 disc plus the new
private ember pack, and leaves the game running for the owner. Logs are retained
as `embers-owner-test-stdout.log` and `embers-owner-test-stderr.log` in the
existing private intro folder. User-visible appearance/approval remains the
owner's test; no PR is opened by launching. Earlier build/evidence hashes and
the first refusal remain recorded rather than rewritten as an initial success.
