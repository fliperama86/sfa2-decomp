# Intro asset recovery

Port-only research toward the arcade-style intro and attract mode. **No intro
player is implemented here yet. Nothing under `ps1/` is changed.** Any eventual
runtime feature belongs in the normal port behind a default-off experimental
setting, not in the matching PS1 reconstruction.

## Code-first recovery

Recover behavior from the supplied CPS2 program, using Saturn decompilation
to clarify it. Do not reconstruct timings or transitions by visually mimicking
a recording. User testing validates the resulting implementation, not its spec.
Before writing a replacement controller, investigate the owner's hypothesis
that the PS1 disc retains the original in-engine intro code.

The first [PS1 code probe](../../docs/records/2026-10-14-intro-code-probe.md)
found existing ending/scroll-scene C and sprite layouts referencing the same
Gouki portrait. The two referenced layouts collectively use all 211 distinct
nonuniform cells previously matched to the Saturn portrait. This is more than
unreferenced artwork, but **not evidence of the full arcade intro controller**.
The normal PS1 opening route selects the movie player. Dormant scene code and
equivalent controllers elsewhere remain an open question.

The owner prefers PS1-only code and data, including any surviving animation
tables. `animation.py` searches the manifest-listed, nonstream PS1 disc files
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
use PS1 data where equality is proven, and user-supplied supplemental assets
where it is not. Keep arcade fidelity distinct from Saturn-reference behavior.
Do not insert an original-code interpreter or import decompiler output as
project source. Intro playback and the attract-mode controller are separate
pieces; neither is claimed by this asset-recovery increment.
