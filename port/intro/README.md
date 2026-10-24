# Native arcade intro

The complete eight-phase intro now runs as native C in the normal port behind
`[experimental] arcade_intro`, on by default since 2026-10-24 (the owner: "turn
the arcade intro and pc settings default on"); without a pack at the default path
the PS1 opening plays, with a line. It includes the opening, character
strips and portraits, montage, Ryu/Ken sequence, animated title, fades and sound.
Nothing under `ps1/` changes. This does not implement the rest of arcade attract
mode or change the game's asset source to CPS2.

## Import and run

Build the normal port, then import the supplied, examined arcade archive once:

```sh
python port/intro/import_full.py INPUT/arcade.zip port/local/intro-asset-probe/intro-full.dat
```

The importer needs Python, a native C compiler and a C++ compiler (`--cc` and
`--cxx` select them). It obtains the key from the supplied archive, decrypts the
inline-data carrier transiently with the licensed import-only helper, and runs
project-authored native C to collect the data/tile closure. No emulator, second
analysis ROM or separate player is required. It verifies native output again
using only the resulting sparse asset pack. Original program images, frame
recordings and game assets are never distributed with this source.

Use the same normal executable and the user's normal PS1 Japan disc:

```text
sfa2.exe --skip-programs --arcade-intro --intro-data PATH/intro-full.dat PATH/PS1.cue
```

Paths above are placeholders in the host's native syntax. Alternatively set
`arcade_intro = true` under `[experimental]` in the shared settings file.
`--ps1-intro` restores the original movie route and does not read an intro pack.
Start skips and returns to the normal game; Return is keyboard Start. Escape
closes the port window (with the settings menu on, Escape opens the menu).

**Old first-scene packs are rejected with an explicit re-import message.** They
cannot silently launch the old Akuma/flames-only slice again. Historical slice
source and evidence remain preserved, but are not the current runtime route.
The importer refuses unknown archive revisions and never overwrites existing
outputs. Prepared packs and their reports stay private under `port/local/`.

## Sound and fidelity boundary

Pictures and animation use recovered CPS2 C behavior and original imported
artwork/tables, not video playback or visual-mimic timing. No Saturn code is
used by this implementation. The current profile uses the examined US 960306
archive and its recovered Zero 2 intro variant; other configurations are not
claimed. Startup RNG is explicitly normalized for reproducible import/playback.

Sound comes from the user's PS1 disc's original opening XA stream, excluding
its preceding Capcom-logo portion. It needs no additional audio file or Saturn
disc. Native C decodes/mixes that soundtrack through the normal audio backend;
it already includes its recorded title cue. The native exit fade controls its
cutoff. This is an intentional soundtrack adaptation, **not a reconstructed
QSound driver or bit-perfect arcade audio**. The normal port's 60 Hz clock is
used; exact CPS2 scheduler/cycle timing is not claimed.

The final original sound queue command is kept opaque in the director. It was
initially mislabeled as a stop request; the recovered code only establishes
its queue command and parameter, not that interpretation. The host does not
silently cut music at that cue.

## Evidence and testing

The [integration record](../../docs/records/2026-10-14-intro-full-runtime.md)
separates original-routine comparisons, native resource closure, controls and
normal executable observation. All eight phases reach their native exit.
Controller/actor state, fade/map consumers and generated display lists have
scoped original evidence; independent native startup is compared separately.
The normal Windows program completed all 2,948 updates, with a silent audio
dump and captures compared to native RGB555 output, then returned to the game.
This is readiness for owner testing, not owner acceptance or a merged PR.

Focused controls are the `port/tools/test_intro_{native,maps,fade,presentation,
player,resources,import,runtime,audio,audio_host}.py` files. They use synthetic
inputs and do not boot the game. Prior unchanged sprite/video/sequence evidence
is reused. The [director](director-contract.md),
[presentation](presentation-contract.md) and [resource/audio](runtime-contract.md)
contracts state supported paths and explicit approximations.

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

## Historical asset-recovery boundary

The original asset-only increment required map placement, sprite lists,
palette effects, timing contracts and a dependency manifest before playback.
Arcade fidelity stays distinct from Saturn-reference behavior; original-code
interpreters and imported decompiler output are excluded. Intro playback and
the attract-mode controller are separate pieces. Neither was claimed by that
asset-recovery increment. The current full intro is documented above; the
broader attract-mode controller is not implemented by it.
