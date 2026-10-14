# Intro asset recovery

Port-only restoration of arcade-style intro behavior. **A bounded first scene
slice now runs in the normal port behind `arcade_intro`, default off. Nothing
under `ps1/` is changed. This is not a complete intro or attract mode.**

## Test the first scene

The existing Windows executable can replace the PS1 opening movie with
192 updates (about 3.2 seconds) of Gouki and animated flames, with moving
embers when a version-2 pack is supplied, then return to
the title flow. Leave the title idle to reach the opening. Start skips the
slice; Escape closes the normal port window.

Build with the existing `./build.sh win`. Import your examined arcade archive
once, without distributing the output:

```sh
python port/intro/import_scene.py INPUT/arcade.zip port/local/intro-asset-probe/intro.dat
```

Then use the **same** executable (paths here are placeholders, in the host's
native syntax):

```text
sfa2.exe --skip-programs --arcade-intro --intro-data PATH/intro.dat PATH/PS1.cue
```

Or set `arcade_intro = true` under `[experimental]` in the shared settings
file and pass `--settings FILE`. The default pack path is `intro.dat` in the
working directory. `--ps1-intro` overrides the setting off and restores the
original opening route without reading any arcade pack. No separate player,
interpreter or captured video is used. Invalid/missing enabled input stops
with an explicit message. The importer currently accepts only the supplied,
examined US 960306 archive, including its observed graphics variants.

The owner tested version 2 and rejected its ember-only scope as insufficient.
It is not a complete-intro test candidate or an approved feature. Its
[particle contract](particles-contract.md) reconstructs the original
RNG, spawning and fixed-point movement. The old version-1 pack still works and
retains the earlier no-particle scene. Do not overwrite it. To prepare the new
pack during development, use the pinned decrypted analysis image of the same
arcade archive (not another ROM):

```sh
python port/intro/import_scene.py INPUT/arcade.zip port/local/intro-asset-probe/intro-embers.dat \
  --opcodes INPUT/arcade-opcodes.bin
```

Select that pack with the existing `--intro-data` option. The runtime never
reads decrypted program bytes. Automating this analysis-only decryption step
in the final importer remains unfinished; this is not the final import UX.
The prepared private owner-test pack needs no further extraction.

This test slice uses both the normal PS1 disc and the privately imported CPS2
scene data. It does not settle the future single-input user experience or
adopt a whole-port CPS2 asset pivot. Saturn is not a runtime input.

## Implemented and missing

The [C sequence core](sequence-contract.md) now drives six foreground poses
from relocated original records. The native compositor uses extracted tiles,
metasprite layouts and palettes, with code-derived anchor placement, four-update
foreground durations, six-update background pages, palette cycling and row
scroll. The [bounded contract](../../docs/tasks/intro-first-scene.md) states the
normalization and sources. This is project-authored C, not byte-matching Saturn
C or a claim of whole-scene differential equivalence.

Version-2 packs add the steady scene's moving embers from original artwork,
sequence records and privately imported velocity/spawn tables. Forty original
actor slots are modeled, including the one occupied by the main flames.
Spawning, two random selections at initialization, acceleration reversal,
unsigned bounds and deferred deletion follow the recovered code. The initial
seed and interval are explicitly normalized, not inherited from omitted scenes.

Not implemented: warm-up, fades/flashes, sound, the remaining intro
scenes and attract-mode flow. Full brightness and initial effect phase are
normalized in this slice. The existing port's 60 Hz clock is used, not an exact
CPS2 refresh clock. The owner must be able to test this build before a new PR.

The active [complete-intro task](../../docs/tasks/intro-complete-sequence.md)
requires the ENTIRE intro before another readiness notification or owner test.
Native [graphics composition](video-contract.md) and
[metasprite expansion](sprites-contract.md) now cover the shared formats needed
by all scenes. These are intermediate source components, not a new runnable
intro. Offline original-state captures are analysis evidence only, never
runtime playback data. Native controllers, actors, complete import and sound
remain required before integration into the normal opening route.

The owner authorized intro-first CPS2 implementation on 2026-10-14. Saturn
remains valuable as a possible byte-matching C target. A match still needs
compiler/build evidence; C ancestry alone does not establish it.

Focused synthetic checks (seconds, no game or private input):

```sh
python port/tools/test_intro_scene.py
python port/tools/test_intro_particles.py
```

## Code-first recovery

Recover behavior from the supplied CPS2 program, using Saturn decompilation
to clarify it. Do not reconstruct timings or transitions by visually mimicking
a recording. User testing validates the resulting implementation, not its spec.
The earlier probes investigated the owner's hypothesis that the PS1 disc
retains the original in-engine intro code.

The first [PS1 code probe](../../docs/records/2026-10-14-intro-code-probe.md)
found existing ending/scroll-scene C and sprite layouts referencing the same
Gouki portrait. The two referenced layouts collectively use all 211 distinct
nonuniform cells previously matched to the Saturn portrait. This is more than
unreferenced artwork, but **not evidence of the full arcade intro controller**.
The normal PS1 opening route selects the movie player. Dormant scene code and
equivalent controllers elsewhere remain an open question.

The earlier PS1-only investigation included any surviving animation tables. `animation.py` searches the manifest-listed, nonstream PS1 disc files
for the first Saturn scene's five referenced sprite schedules:

```sh
python port/intro/animation.py --opening-z INPUT/OPENING.Z --disc INPUT/PS1.bin \
  --manifest INPUT/manifest.json --output port/local/intro-asset-probe/animation-search.json
python port/tools/test_intro_animation.py
```

The manifest is the private PS1 baseline file inventory (`files` entries with
`path`, `size`, `sha256`). Every searched file is verified. Output is private,
never overwrites existing evidence, and lists the scope and exclusions. The
tool normalizes endianness and relocated branch fields; differing artwork
offsets and unused fields are deliberately ignored. Equal timings alone are
**candidates**, not recovered intro tables. Their artwork and consumers need
inspection. Background-page animation and palette cycling are separate from
these sprite records. Saturn is an analysis reference, not a new port input.

The [first table search](../../docs/records/2026-10-14-intro-animation-tables.md)
checked 248 nonstream files. It found no native-format match for the main
six-step sprite loop, and no matching RGB tables for the three cycling
palettes. Smaller timing matches lead to larger character/stage drawings,
not confirmed intro particles. This does not exclude retimed, differently
encoded or compressed tables. The earlier PS1 portrait sequences are real,
but they do not supply the missing intro schedule. No Saturn port is adopted.

## Background decoder

`assets.py` reads the two opening files extracted from a user-supplied Japanese
Saturn disc (T-1212G V1.001). It accepts only the file sizes and SHA-256 hashes
of the examined revision. It does not acquire or distribute game files.

```sh
python port/intro/assets.py --opening-bin INPUT/OPENING.BIN --opening-z INPUT/OPENING.Z
python port/tools/test_intro_assets.py
```

The first command writes twelve PNG rectangles and a JSON manifest under the
ignored `port/local/intro-asset-probe/`. Existing files are never overwritten;
`--prefix NAME` distinguishes an intentionally new evidence revision in the
same folder. `--output` may select another folder only within this checkout's
`port/local/`. The manifest's image names take the recorded `output_prefix`.
Both input files, all pictures, extracted tables and analysis remain private.
The tool currently takes **extracted files**, not a disc image directly.

### What is established

Static inspection of the supplied opening module identifies an initial-state
setup which uploads twelve background rectangles. Decoding those references
produces the title logo, flame artwork and Gouki's portrait. The module contains
palette-copy routines and their initial palette references; these colors are
decoded, not guessed or generated. Labels are ours, not original symbols.

The profile follows the setup at `0x0607010c`, the rectangle-copy routine at
`0x06072b84` and palette-copy routine at `0x06072c5c`. Its interpretation maps
the module at `0x06070000` and opening pattern data at VDP2 VRAM `0x20000`.
Internal pointers and coherent decoded artwork support these mappings; a
runtime loading trace has not independently verified them.

The format is two-word, big-endian pattern names; palette selection in groups
of 16 colors; 8x8 characters or four cells forming a 16x16 character; packed
high-nibble-first indices; RGB555 colors; transparent pen zero. The decoder
handles both flips and refuses unsupported pattern-name attributes. It does
not copy a console/emulator implementation. Hardware interpretation is based
on Sega's [pattern-name specification](https://www.infochunk.com/saturn/segahtml_en/hard/vdp2/hon/p04_61.htm)
and [character-pattern specification](https://www.infochunk.com/saturn/segahtml_en/hard/vdp2/hon/p04_40.htm).

### What this does not establish

These 256x256 rectangles are upload units, **not screen positions or whole
scenes**. They do not include map-register selection, scroll, enable/priority,
animated palette changes, color-offset flashes or sprites. The output uses
the initial palettes, not every palette seen during playback. The source
contains timing counters, but complete scene duration and arcade parity are
not established. No original CPU instructions execute in this tool or the port.

Earlier private comparisons found many exact indexed-pixel matches between
Saturn and CPS2, and some also on the PS1 disc. That is not proof of equal RGB
palettes, complete intro assets on PS1, or equivalent sequencing. The supplied
arcade program is a US revision; both console discs are Japanese. A failed
bounded tile search does not prove whole-disc absence.

## Next runtime boundary

Before introducing a scene player, finish its map placement, sprite list,
palette effects and timing contract. Build a per-scene dependency manifest:
derive intro data from the supplied CPS2 set, recording the exact revision
and any behavior reconstructed from Saturn. Keep arcade fidelity distinct from Saturn-reference behavior.
Do not insert an original-code interpreter or import decompiler output as
project source. Intro playback and the attract-mode controller are separate
pieces; neither is claimed by this asset-recovery increment.
