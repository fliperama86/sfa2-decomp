## Owner-testable native intro scene slice (2026-10-14)

Implemented on the existing worker branch after the owner required a runnable
feature before another PR. No new PR opened; earlier PR369 stays closed.
Nothing under ps1/ changed. This does not adopt the whole-port CPS2-input pivot.

The normal executable now has a default-off shared `arcade_intro` setting,
`--arcade-intro` / `--ps1-intro` overrides and `--intro-data FILE`. The opening
program is still verified against the PS1 disc before replacement. Disabled
behavior never opens the supplemental pack and follows the existing movie
setup/main/teardown. An enabled missing or corrupt pack fails explicitly.

The importer reads only the previously investigated supplied US 960306 arcade
ZIP. It pins the archive, CPU data and decoded graphics digests, supports the
observed graphics variants, writes exclusively under ignored port/local/ and
never overwrites an existing file. It needs no decrypted opcodes, Saturn input,
emulator or downloaded assets. The private pack contains only extracted pixels,
palettes and relocated sequence data, no executable program bytes.

Native C displays 192 updates of the Gouki/fire scene, using the previously
tested sequence reader for the six foreground poses. Background pages, portrait
palette cycling, triangular row scroll and layer placement come from the code
and data formats, not visual imitation. Source routes are the scene setup and
steady phase at 0x2a56/0x2af4, palette helper 0x43ba, row helpers 0x44e0/0x4510,
actor initializer 0x73cc4, coordinate transform 0x13028, and map loaders
0x1cbd2/0x1cf9c. The decrypted opcode plane is required for PC-relative tables;
ordinary address-register data uses the CPU data plane. Prior REA could not
open this M68K ELF; focused Capstone listings and existing private Ghidra
analysis supplied the evidence. No decompiler text is published.

Graphics bank addressing, tilemap indexing, transparency and display origin
were checked against the locally retained official MAME CPS2/CPS1 video source
at commit 32f48ebb75f4fefcc7bcf0482289c0afa57d9973. No third-party source was
copied into this implementation. All newly tracked code and fixtures are
project-authored, with synthetic tests. Publication inspection found no binary,
extracted asset, original table, pseudocode, machine/person path or credential.

This is explicitly a partial reconstruction, not a whole-scene differential
match or byte-matching Saturn C. Missing: warm-up/flashes, particles, sound,
later scenes and attract flow. Full brightness and initial effect phase are
normalized. Timing uses the existing 60 Hz port clock rather than exact CPS2
refresh. Foreground bounds span 144 pixels, not the 128 pixels of pose zero;
later poses have whole-block flips. Initial importer assertions caught those
incorrect generalizations before any pack was written; all six now pass.

## Scoped evidence

- Five new synthetic importer/native scene/pack/runtime/GPU-command controls
  passed in 0.44 s. They cover planar decode, lane/bank permutation, whole-block
  flips including tile-row wrap, foreground/background/palette clocks, layer
  priority, bounds, input corruption, completion, skip and disabled behavior.
- A separate added importer publication/input safeguard control passed in 0.22 s.
- Settings C roundtrips (128 flag combinations) and the actual startup parser:
  two controls passed in 0.46 s, including default-off and CLI overrides.
- Existing normal program setup/main/teardown case passed with the cross
  compiler in 0.5 s. New hook case passed in 0.5 s: replacement only after
  verification, no movie setup/main/teardown, and corrupt original file refused.
- The Mac build's source-selection/flags case passed. No macOS build/run here.
- Existing sequence/original-code comparisons are reused from 4c442615 and
  6902b58a. The unchanged reader was not reverified against original code.
- Normal Windows hostbuild, four compiler workers, existing isolated PsyZ
  library: 3,982 units compiled, zero failed; normal executable linked and
  address-verified. No PS1 matching build or library rebuild.
- Actual imported private pack validated through native C. A bounded, silent,
  offscreen normal-executable observation confirmed the actual new upload:
  all 86,016 cropped GPU pixels equal native scene frame 1, zero mismatched
  color channels. A subsequent observation confirmed return after exactly
  192 updates and restoration of the original display mode/title setup.
  This establishes integration, not original-game scene equivalence.

The first observation was limited to 20 seconds and reached the scene but not
its return. A 28-second observation answered that remaining question. Both used
no audio and were stopped by their own process IDs, not image name. Logs, input
and executable digests, GPU captures and the build manifest remain private in
the stable intro-asset-probe folder. The Windows executable remains in the
canonical port/build/host folder; the owner gets its direct launch command.

Author command errors, not hidden results: the initial program-control attempt
used native cc and failed at windows.h, then passed with the proper cross
compiler. The build itself succeeded, but its shell trailer used zsh's reserved
`status` variable and returned 1 after completion; the compiler/link verification
log and executable are the evidence, not that wrapper status. No unchanged
build was repeated. Two malformed ad-hoc Mac-case invocations failed to parse
before the corrected, narrowly selected case passed.
