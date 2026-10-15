## Complete native intro in the normal port (2026-10-14)

The normal Windows executable now completes all eight native intro phases,
including the montage, Ryu/Ken sequence, animated title and exit fade, with
the user's PS1 opening soundtrack, then returns to the game. This replaces
the prior Akuma/flames-only route. It is ready for the owner's complete-intro
test, not accepted by the owner, merged, or a perfect-arcade-audio claim.
No PR was opened. No `ps1/` file changed. The feature remains default off.

Implementation adds an independently initialized native player, sparse resource
import/parser, normal opening integration and a bounded soundtrack adapter.
The [runtime contract](../../port/intro/runtime-contract.md) describes format,
ownership, clock, sound and failure boundaries. Previous controller, actor,
map, fade, sprite and object-presentation evidence is reused from `dc8b8bb2`
and its [record](2026-10-14-intro-native-rendering.md), not rerun as a tree gate.

### Original behavior and resource closure

- Two independent-startup comparisons each completed 2,948 updates. Native
  startup used only its own initializer, not copied original RAM. All video
  memory and emitted object records through the terminator compared equal
  after every update. Original writes were audited against explicit game-RAM,
  stack and video/object regions: 2,006,457 and 2,012,535 writes. This is a
  region audit, not a tight per-field contract, and does not compare every
  normalized RAM byte. Prior actor-state evidence retains its documented
  unused headshot-register-residue projection. Original scheduler/peripheral
  substitutions remain explicit; this is not a game boot.
- Archive-only import automatically decrypted the inline-data carrier and
  produced the same pack as the earlier developer-carrier import. The pack
  has 489 data spans (108,072 bytes), 7,342 indexed tiles (1,915,456 bytes),
  and a total size of 2,119,504 bytes. SHA-256:
  `9a90a0e16b4e1540f68ea8b558d6ce1299fce0e7321d8a98fcfb0473b062ae02`.
  All 2,948 native RGB555 frame hashes agreed between full-input and sparse
  playback. No original CPU executes during import or normal playback.
- Eight native phase previews were inspected. They show subsequent scenes,
  not another variation of the first scene. These previews and every game
  asset, carrier, key, capture and analysis file remain private.

### Normal executable observation

The normal four-job Windows build compiled 3,982 units with zero failures,
linked and passed its address verification, using PsyZ
`4c68cab400cba341be3d2066993fcdf778301acf`. The tested executable has the full
runtime, not merely the previously built offline core. Mac/Linux execution
was not tested by this increment.

One silent run used the real user-supplied PS1 disc, version-3 pack and
`--audio-dump`, plus periodic GPU-memory captures. It printed completion
after 2,948 updates, restored normal graphics and returned to the game's
module route. Automation then closed only this worker's window. The final
`window closed` stop and unavailable post-close dump were intentional,
not a failed intro. This observation did not test an entire fight or game.

Twelve samples during the native intro, at four-second intervals, match the
native RGB555 display exactly. Captures are full VRAM; the comparison extracts
only the 384x224 display rectangle. Unused texture-bank contents are not a
screen defect. The late black sample has multiple equivalent native frames
and is not unique timing evidence. Cropped contact sheets were inspected.

The closed audio dump contains 68.65 seconds of stereo 44,100 Hz PCM for the
whole launch, including earlier boot material, with nonzero intro output and
its exit fade. It was inspected numerically, not listened to. Its SHA-256 is
`1a6e50cba70a0fe5521bd1b780e84d814617a8c696f5188e4fcbaed52fd04d67`.
Graphics/control are recovered native CPS2 behavior; soundtrack is explicitly
the PS1 XA adaptation, not native QSound or Saturn code. The normal 60 Hz
port clock and normalized display boundary are not exact CPS2 cycle timing.

### Scoped synthetic controls

All following controls passed without a running game or private inputs:

| Control | Cases | Time | Boundary |
| --- | ---: | ---: | --- |
| `test_intro_resources.py` | 1 | 0.474 s | Sparse ranges/tiles, bounds and rejection |
| `test_intro_player.py` | 1 | 2.271 s | Native initialization, failure and exit, UBSan |
| `test_intro_import.py` | 5 | 1.176 s | Encoding/refusals, synthetic cipher golden, actual probe bookkeeping and update budget |
| `test_intro_runtime.py` | 2 | 0.367 s | Normal glue with explicit player/audio/GPU stand-ins, full-length loop, skip and legacy rejection |
| `test_intro_audio.py` | 1 | 0.297 s | Native XA adaptation, capacity, failure and gain |
| `test_intro_audio_host.py` | 2 | 0.864 s | Real disc lookup on synthetic ISO, locked host routing and default XA preservation |
| `test_intro_scene.py` | 9 | 0.429 s | Preserved legacy scene/GPU controls, decoupled from replaced runtime |

Host audio unit tests stub the pinned digest; the real runtime separately
verified the actual selected stream. Import probe tests deliberately replace
the player with a declared stand-in; actual full-native resource closure is
separate private evidence above. The cipher golden uses a synthetic byte
ramp and synthetic key, independently calculated by the retained upstream
adapter, not game data. Control mapping was updated for these owned paths.

### Failed attempts and corrections

The first independent-startup comparison failed at a clear because the busy
flag was consumed before, rather than at, object publication. Moving that
consumption into the native presentation callback fixed the compared boundary.
The first audio fixture expected one too few resampled frames; its expectation
was corrected to the existing resampler's actual bounded output. The first
cipher adaptation compile exposed a removed closing brace in the UI-only
block; it was fixed before the successful golden/import checks.

The first normal launch stopped before the intro: the real disc finder takes
a basename, not `PAC/Z2O.STR`. A permissive lookup mock had hidden that error.
The host now passes `Z2O.STR`, and its test links real `disc.c` against a
synthetic ISO directory so this mismatch is exercised. The second build/run
is the successful evidence above. The opaque finale sound command was also
initially mislabeled as a stop without sufficient evidence; that claim and
behavior were corrected. Lessons were recorded in project memory.

### Publication and retained evidence

New player/importer/audio/runtime C, Python, contracts and synthetic tests
are project-authored. The sole new third-party code is the explicitly marked,
import-only MAME BSD-3-Clause cipher adaptation, with copyright, license,
pinned upstream and changes documented beside it. It contains no game key
or game program and is not linked into the port executable. No decompiler
output or supplied asset enters Git.

Private evidence uses the existing intro analysis folder: `player-seed0-audit`
and `player-seed7-audit` logs, `full-import-2.log`, `intro-full.dat.json`,
`full-runtime-build-2.log`, `full-runtime-2.log`,
`runtime-picture-native-comparison.json`, `full-runtime-audio-2-analysis.json`
and `full-runtime-evidence.json`. The final manifest records source, binary,
input and evidence digests. Failed logs remain preserved as failed attempts.
The whole-port CPS2-only asset pivot and the rest of attract mode are separate
future decisions. Owner testing precedes any PR for this full-intro change.
