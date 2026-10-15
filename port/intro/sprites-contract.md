# Intro metasprite consumers

Native, readable C reconstructs the six object data formats consumed by the
supplied program at 0x129ce through 0x13120. No original instructions, assets,
captured frames or decompiler output are compiled into this module. The
importer must normalize the artwork's coordinate pointers into bounded blocks.
This module does not implement scene actors or claim full-intro completion.

## Format and state

`sprites.h` describes the normalized inputs and result codes. Formats 0 and 2
use a shared coordinate array, format 4 has inline coordinates, and formats 6
and 8 use a directly addressed coordinate array. Format 10 intentionally draws
nothing. Format 0 has one attribute word for all cells, format 6 has one for
all blocks; normalized blocks repeat it. Formats 2, 4 and 8 have per-block
attributes. `cells` is the original cell budget, not the number of records.

The caller resolves the hardware anchor and supplies X priority/Y bank bits.
Formats 4, 6 and 8 add 0xa400 to tile codes and force Y bank 0x2000. Other
formats retain the actor's Y bank. X/Y arithmetic wraps at 16 bits before the
ten-bit output coordinate mask. The complete block is rejected without budget
consumption when its cell budget is unavailable. Capacity failures are errors,
not partial object lists. Bad inputs leave list and destination untouched.

Actor bypass disables palette overrides, geometry flips and transforms. Normal
format 0 combines actor and cell flips with OR and reflects 16-pixel cells;
formats 2, 4 and 8 XOR actor/cell flip bits but reflect geometry only by actor
flips and the original block dimensions. Format 6 combines flips with OR and
also reflects geometry only by actor flips. Palette mode zero preserves the
original palette; positive mode replaces it, negative mode adds it modulo 32.
The original mode-8 override clears the block-size bits as well. This behavior
is retained rather than silently corrected.

Formats 6 and 8 optionally expand coordinates, then rotate them using imported
signed coefficients. Expansion specifically truncates the signed product to
16 bits **before** the arithmetic right shift by four. Rotation uses full
signed 32-bit products and arithmetic right shifts by eight. C helpers express
these operations without signed overflow or implementation-defined shifts.
After anchoring, these formats clamp signed X outside [-576,448] to 448 and
signed Y outside [-784,240] to 240, then apply ten-bit wrapping.

Mode 8 with actor bypass is not reached in the complete exploratory intro
trace and is explicitly rejected. Its original consumer has a distinct
register-dependent path; no guessed reconstruction is provided. All observed
format/bypass pairs are supported, including format 10's no-draw path.

## Scoped validation

`verify_sprites.py` runs only the bounded original object consumers using the
pinned private opcode derivative. Artwork, coordinates, matrices, actor state
and budgets are synthetic. Reads, writes and control-flow are audited; source
tables and actor bytes remain unchanged. Native outputs, record counts and
cell budgets are compared. A bounded two-seed run and fixtures exercise every
format, both observed bypass variants, palette modes, flipping, overflow,
scaling, rotation, clipping and budget rejection.

`sprites-fixtures.json` contains synthetic numeric inputs and isolated-original
outputs. It contains no game artwork, tables or instructions. The normal unit
test replays those fixtures without original inputs or an emulator. Four actual
C mutations must be rejected: wrong expansion width, wrong flip composition,
wrong clipping destination, and charging record count instead of cell count.
Bounds checks also test rejection atomicity.
