# Complete-intro graphics adapter

This is project-authored port glue, not an emulation of original instructions
or a claim that the full intro is implemented. The controller supplies maps,
palettes and an object list. No game data is compiled into this unit. The old
first-scene runtime is unchanged until complete-sequence integration.

## Boundary

`video.h` describes the input. Map words, palette words and eight-byte object
records are big endian. Tile callbacks provide validated indexed pixels and
must return null on missing resources. Tiles have 8, 16 or 32 pixels per side.
Pen 15 is transparent. The output is the port's normal 384 by 224 RGB555 image,
cropped from hardware coordinates x=64..447, y=16..239. All arithmetic wraps
only where the hardware specifies a wrap; unsigned C avoids signed overflow.

The three tile planes have 64 by 64 cells. Their column-major layouts split
rows into 32-, 16- and 8-cell sections. The graphics banks add 0x20000, 0x10000
and 0x4000 to 8-, 16- and 32-pixel tile codes respectively. Eight-pixel tiles
alternate physical left/right halves by map column. Scroll registers are the
already-transformed hardware values, not original world-space camera values.
Layer two can add a 1024-word rowscroll table, indexed by screen Y plus the
row offset. Plane enables and the four-position layer permutation are explicit.

Palette pages are sprites, then the three tile planes. For each channel:
`rgb8 = nibble * 17 * (15 + 2 * brightness) / 45`, then discard the low three
bits for the normal upload format. Brightness zero is not black unless the
color components are zero. The transparent background is always black.

Object blocks are up to 16 by 16 cells. X/Y wrap at 1024; tile bank bits are
in the Y word. Tile attributes select palette, flips, block dimensions and
offset bypass. Unflipped blocks wrap the tile-code low nibble; flipped blocks
use ordinary cell addition. Later records are in front of earlier records.
Priority zero is hidden. Other priorities must exceed that of the topmost
nontransparent background at the pixel. A nontransparent object pixel claims
its position even if hidden by the background, preventing an earlier object
from leaking through. Transparent pixels do not claim positions.

The adapter rejects null/bounded-size failures, misaligned map/row/palette
addresses, invalid layer permutations, excess objects and global screen flip
(not used in this intro) before output writes. Missing tiles or invalid pen
values fail explicitly during composition; incomplete output must not be
displayed. The resource owner must keep all input pointers alive for the call.

## Evidence and provenance

The supplied program's renderer constructs these records at 0x127a4 and its
consumers. Offline discovery reaches all eight intro phases and the actual
object renderer. That discovery does not establish exact real-time playback:
cooperative yields and fade scheduling still have an explicit analysis shim.

Hardware layout and priority interpretation were checked against the owner's
local MAME research checkout, pinned at
`32f48ebb75f4fefcc7bcf0482289c0afa57d9973`: `cps1.cpp` graphics layouts,
`cps1_v.cpp` tile mapping, palette and scroll handling, and `cps2.cpp` object
composition. MAME is a reference, not linked or copied into this implementation.
The reference files retain their own BSD-3-Clause notices in the private
research area. This unit contains neither extracted assets nor decompiler output.

`port/tools/test_intro_video.py` compiles only this adapter and its synthetic
C fixture. It needs no ROM, window, game, audio device or console and takes
seconds. It covers map permutation, brightness, scroll/wrap/halves/flips,
rowscroll, object blocks/banks/offsets, priority/occlusion and failure bounds.
Private original object-list snapshots can additionally resolve integration
questions, but are not runtime input or substitutes for native controllers.
