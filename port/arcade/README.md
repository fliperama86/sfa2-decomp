# Arcade assets

The loader that builds what the game reads from the user's arcade set
(MAME's `sfa2u`), so that in the end the port needs no PS1 disc. Setting
`[experimental] arcade_assets`, default off; `--arcade-assets` and
`--ps1-assets` override it, and `--arcade-rom FILE` names the set
(`sfa2u.zip` by default). Nothing under `ps1/` changes, and no byte of
either game enters Git: the source holds addresses, sizes and layouts.

**State, 2026-10-23: the loader opens and checks the arcade set and builds
from it 45,858 of the 90,088 bytes of the PS1 program's data (78 tables, the
2,125 pointer words of the port's layout, the effects' cell lists); for 27
of the 45 fighter files the cell lists and position runs (585 KB, equal to
the disc's) and the sprite tiles (every tile the disc's, packed by the
loader's own encoder into 9.8 MB of chunks, each smaller than the disc's);
for all 45 the palettes and portrait pages (2.4 MB, equal to the disc's);
for the 37 that the fighter tables cover the arcade-sourced animation steps
(34,771 steps equal to the disc's); and for the 20 stages 6.2 MB of layer
maps, pages, palettes and tiles. All of it is served in place of the
disc's. The disc gives everything
else. Without a disc the program stops with what it cannot build yet.**

## The owner's direction

On 2026-10-23, after the comparison below: "The ps1 code isn't going
anywhere. We need the ports to maps and use the cps2 assets and
reimplement what is missing", then "you may start by writing the loader
targeting the arcade rom and work your way through the files it expects".
The owner does not need the PS1's own menus, modes or movies for now.

So the game's C stays the PS1's. What it reads (its program's data and the
files of the disc) is built from the arcade set where the arcade has it,
and what the arcade lacks is written anew. No code of either game is run
or interpreted.

## How it works

```
port/build/linux/sfa2 --arcade-assets --arcade-rom sfa2u.zip DISC   # arcade pieces over the disc
port/build/linux/sfa2 --arcade-assets --arcade-rom sfa2u.zip        # stops: what the arcade set cannot give yet
port/build/linux/sfa2 --arcade-assets --arcade-rom sfa2u.zip --arcade-check DISC   # compare, then end
port/build/linux/sfa2 --arcade-assets --arcade-rom sfa2u.zip --arcade-import local/sfa2.pack DISC   # once
port/build/linux/sfa2 --arcade-assets --arcade-rom sfa2u.zip --arcade-pack local/sfa2.pack          # no disc
```

- The program is built from the loader files that `build.txt` lists, so
  work in progress in this folder does not break the build.
- `zip.c` reads the archive (its own inflate; no library). `rom.c` puts the
  members together into four views (the 68000 program, the graphics as the
  video chip reads them, the QSound samples and the Z80's program) and
  checks each view's SHA-256 against the examined set's. Nothing is
  decrypted: the loader reads data, never the program's encrypted opcodes.
- `program.c` has the pieces of the PS1 program's data that the arcade
  program holds: for each, the PS1 address, the size, the arcade address
  and the layout (bytes as they are, or 16-bit or 32-bit values whose bytes
  turn from big-endian to little-endian). After the program is loaded from the
  disc, the runtime (`port/src/arcade.c`) writes each piece over it.
- `fighter.c` builds a fighter module's position runs and cell lists and
  the PS1's tile numbering from the arcade's art records (the rules below;
  `fighters.py` is their specification, and `fighters.py --compare-c`
  checks the C against it), for the files in `fighters_table.c` (generated
  by `fighters.py --c-table`: addresses, counts and offsets). The runtime
  builds them at the start (13 ms for 37 files) and serves each file's
  blocks in place of the disc's where they are equal to them, to the game's
  reads (`cd.c`) and the runtime's own (`disc.c`), through one table of
  byte ranges. The other 10 differ where the PS1 changed the arcade's data
  and stay the disc's until the steps and tiles that point into them are
  built too. For the 27, `tilepack.c` packs the fighter's sprite chunk from
  the arcade tiles (the color index one higher, stored with the flip of
  their first use; tile 0 and the PS1's own last tiles from the disc), no
  larger than the disc's chunk: the encoder is the loader's own (fewer bytes
  than the PS1's), not the PS1's.
- `--arcade-check` ends with a coverage report: how many bytes of the
  disc's files (other than the program and the music and movie streams) the
  loader builds equal to the disc's, in all and by kind of file. On
  2026-10-23: 39% in all, built from the arcade alone (the fighter files 46%,
  the stages 56%, the screens from 3% (endings) to 90% (the HUD file)); the
  modules' code, which the port's C replaces, counts as not built.
- With a disc, the first read of each disc file prints
  `arcade: NAME from the disc: no arcade source yet`. Played through, those
  lines are the list of files the loader has still to build, in the order
  the game wants them.
- `--arcade-check` builds every piece and compares it with what the disc
  gave at its place, prints each one that differs, and ends (status 1 if
  any differs).

## Playing without the disc (the owner's decision of 2026-10-23)

What the arcade does not have (the PS1's own tables, text and menus, its
added steps and art, the sound banks' settings, the music tracks without an
arcade song) comes from a one-time import of the user's disc into a private
file, a pack, as the arcade intro's pack does: nothing of it enters Git.

- `--arcade-import PACK DISC` (with `--arcade-rom`): the program builds what
  it builds from the arcade, as for a start with the disc, and writes into
  PACK the disc's layout (each file's name, first sector and size, the
  sectors' form and XA subheaders) and every byte that it does not build,
  the program's file included. The movies (`*.STR`) are left out: the port
  skips them without the disc. The pack is checked by its SHA-256 and names
  the arcade set's and the build's tables it was made against; a pack of
  other tables is refused and must be made again.
- `--arcade-pack PACK` in place of the disc: the disc layer reads a virtual
  disc whose sectors are the loader's built bytes over the pack's. The pack
  shrinks as the loader builds more.
- Done on 2026-10-23: the pack is written in 3 seconds (203 MB: 93 MB of the
  files' bytes the loader does not build, the music streams and the logo and
  opening movies; the sectors of the music tracks the loader plays as arcade
  songs are kept as their 8-byte subheaders only and read back as silence,
  which halves the pack; `A2O.STR`, `BF3.STR` and `ZNULL.DAT` left out) to
  `local/`, which Git ignores. A pack that holds a track silent which a build
  plays as recorded is refused, as is a pack of the earlier format. A start from it with
  no disc went from the title into a fight, and the arcade intro ran with
  its soundtrack. The pack holds, per file, the ranges the loader served and
  their SHA-256: a build whose bytes differ refuses the pack. Without the
  disc the screens' own check against the disc is not made (the pack lacks
  those bytes); the digests check them. The fighters' palettes are served
  palette by palette, so that the PS1's own stay in the pack.
- Sound effects and voices: by the owner's word ("Just use the arcade. It
  should sound as good as the arcade version"), the game's sound requests
  play the arcade's effects through the port's own player (`music.c`), not
  the PS1's sound banks re-encoded. Done: the three wrappers of the game's
  sound requests are overrides with `arcade_assets` (`arcade.c`); a request
  is known by its bank and its run of commands (`sounds_table.c`, generated
  by `sounds.py`: 396 of 466 sounds), and plays as the arcade's effect with
  the arcade's pan, else as on the PS1. A bank in memory is matched to its
  disc header, as the library rewrites part of it (the samples' sizes and 8
  bytes of each program record). The player's songs and effects sound
  through a mix of their own after the sound chip's (a hook added to PsyZ by
  `port/psyz.patch`), so they do not wait in the disc audio's buffer; the
  recorded tracks it replaces are read and consumed in silence. In a fight
  344 requests played as the arcade's, 14 as the PS1's (sounds the arcade
  does not have). The game's own fades of the music do not reach the
  arcade's songs yet.

## What the game reads, in order

A run of 150 seconds on 2026-10-23 (title, menus, a fight of Ryu against
Dhalsim, the continue screen, the opening) printed these first reads, in
this order: the stand-alone logo program and its movie (`logo.exe`,
`capcom.bin`, `capcom.str`); `firstsnd.pac`, `ascii.pac`, `demo.pac`,
`selecta.pac` and a stream `zero203.xa` (title and menus); `game_snd.pac`,
the stage `stage0f.pac`, the two fighters `pl00.pac` and `pl0fx.pac`, the
streams `zero206.xa` and `zero201.xa` (the fight); `cdemo0f.pac`,
`ndemo0f.pac`, `cont00x.pac` and `zero204.xa` (after it); the opening
program and movie (`z2o.exe`, `z2o.str`). Other modes, stages and fighters
read other files of the same kinds. The movies and the stand-alone
programs are the PS1's own; the arcade's opening runs in real time behind
`arcade_intro`.

The data of the PS1 program is the first thing the game needs: 90,088 bytes
between the end of its last function (`0x8016d91c`) and its last byte that
is not zero (`0x80183903`); the rest of the image is zeros. Its code is not
needed: the C runs instead.

## What the two versions share

The fighter files are numbered as the PS1 code's modules (`slot04_NN` in
`ps1/src/overlays/characters/`): 00 Ryu, 01 Ken, 02 Gouki, 03 Nash, 04
Chun-Li, 05 Adon, 06 Sodom, 07 Guy, 08 Birdie, 09 Rose, 0A Vega, 0B Sagat,
0C Dan, 0D Sakura, 0E Rolento, 0F Dhalsim, 10 Zangief, 11 Gen; 12, 14, 15,
16 and 17 are the alternate modules of Chun-Li, Gouki, Sakura, Zangief and
Gen.

Measured on 2026-10-23 with the user's disc and arcade set, with throwaway
scripts (nothing of it is in the tree):

- **Sprites.** The PS1 keeps each character's art as compressed 16x16
  tiles (read by `decode_compressed_block`). Every tile of Ryu's (4,199)
  and Dhalsim's (5,562) files, decoded, is a tile of the arcade graphics,
  with the color index one higher (the arcade's index 15, transparent, is
  the PS1's 0).
- **Tables.** The attack boxes of a character's module are the arcade's
  records, the same values in the same layout, with 16-bit fields in the
  other byte order. In the program's data, 48 runs of 32 bytes or more agree
  with the arcade program in that way (the pieces of `program.c`). The
  animation records do not: the PS1's steps are 12 bytes where the arcade's
  are 16 (`port/intro/sequence-contract.md`), and its frames point at its
  own copy of the art.
- **Cell lists.** A fighter's poses are drawn from cell lists in its module
  (`unpack_cell_list_into_stream_cells` reads them: a count, a mode 0, 2, 4
  or 6, a palette, then tile words with flags and runs). Followed on
  2026-10-23 with a debugger in a fight: the second fighter's lists lie one
  after another in its module, and 104 of 108 of them, in order, hold
  exactly the tiles of the arcade's art records that lie one after another
  at `0x213396` (mode 2, blocks of a code and an attribute whose bits give
  the block's width and height), the block expanded tile by tile, with the
  same cell count and palette. The other 4 differ by one tile each, a tile
  whose pixels another tile repeats. The PS1 numbers the tiles in the order
  the lists first use them (1,048 of 1,082 new tiles are the previous
  number plus one) and gives a tile it has already the number it had.
- **All fighters** (`fighters.py`, the rules above run over the 45 fighter
  files). Ryu's and Sodom's (PL06) lists rebuild exactly, but for one tile of Ryu's
  that the PS1 stores flipped. Positions are byte-equal in 16 of 23
  characters. The rest differ where the PS1 changed things: tiles stored
  flipped (a few in most fighters), a palette or flip bit here and there,
  and in Zangief (PL10) and PL11/PL13 art of the PS1's own (poses turned
  half round, tiles the arcade lacks). Some fighters' tile codes are in
  another bank of the arcade graphics (+0x20000); some lists come from
  arcade records of mode 0 (one code a cell). Where a record's blocks step
  back in x, the record is in parts: the PS1 lists the parts last first,
  each sorted by (y, x) (Birdie's, PL08, 297 lists all rebuild with it; Rolento's, PL0E, also
  starts a part at a change of palette). The arcade build does not copy the
  PS1's changes: it converts the arcade's records.
- **Room and flipped tiles.** The PS1 code reads a module's data at fixed
  addresses, and only 0 to 2 bytes follow the lists, so the lists must come
  out at the PS1's size. The PS1 stores each tile with the flip of its first
  use in the file, and a list's flag holds a use's flip relative to that
  (exact for all 106,434 tile codes of the 20 fighters whose art is the
  arcade's). With that rule the lists of 16 fighters rebuild byte for byte
  and fit by construction; PL02/PL14 and PL0B differ only in the PS1's
  palette changes; PL08 and PL0E (Birdie, Rolento) number some body-part tiles
  together. For PL0E a rule fitted to its cases (parts numbered by size, in
  runs of records drawn in parts) brings it to 627 of 727 exact lists, 6
  bytes under its room; no exact rule was found. Every fighter's lists and
  positions now fit where the PS1 code reads them.
- **Sprite chunks** (`tilepack.py`). The PS1's tile compression: blocks in
  tile order after an offset table; a tile with at least 14 zero words takes
  the zero form, else back references (2 to 5 words, window 16,383 words,
  longest and farthest first, cut so a tile's last word is a literal) when
  that is shorter, else 64 literal words. The PS1's encoder also skips about
  8% of the references these rules make, by no rule found, so no chunk comes
  out byte for byte (43% to 71% of blocks do); the reader finds tiles by the
  offset table only, so a valid chunk decodes to the same tiles. The loader
  is to build valid chunks no larger than the disc's. The tiles themselves
  are exact from the arcade in 17 of 22 chunks.
- **Stages** (`stages.py`, all 20). The two layers of 16- and 32-pixel
  cells are the arcade's scroll maps cell for cell (all cells in 17 and 19
  stages); their palettes are the arcade's (4-bit channels scaled
  `n*17*43/45`); the shared and object tiles and the object palettes are
  arcade tiles and palettes, flips stored as separate tiles. The third
  layer is the arcade's scroll 1: block sets at `0x2ad300` of 4x4-cell
  groups at `0x2b1a80`, four sets a stage (all pixels equal in 4 stages,
  nearly all in 7 more). The arcade's block ids per stage are in three byte
  tables (`0x1cff4`, `0x1d0ec`, `0x1d2cc`); the PS1 packs a layer's tiles
  into page slots in the order its scan first uses them, the blank tile
  first. `stage.c` builds the layer maps, pages, layer palettes, object tiles
  and palettes and the shared tiles, with a mask of the bytes known to equal
  the disc's; the runtime serves 6.7 MB of the 8.4 MB it builds. The third
  layer's cells take the first of the stage's rows (by use) that holds their
  colors (93% of the cells); the PS1 dedupes its tiles after the colors are
  applied; the larger palette chunks' extra rows are arcade rows of the
  effects (374 of 512). Left to the disc: the rows the PS1 merged in the
  third layer by an order-dependent, lossy rule (no exact rule found; not
  run-time changes: the arcade's alternate tables serve only stages 20 and
  up), cells and tiles of the PS1's own (247 KB), and 63 mixed palettes. All of
  it is in the arcade's data plane: the loader needs no decryption.
- **Animation steps** (`sequences.py`). A fighter's steps follow its
  lists: 12 bytes each, from the arcade's sequence records (countdown,
  flags, art pointer, 16 frame bytes, and a 4-byte jump target when flags
  bit 15 is set): duration and flags as they are, the art pointer becoming
  the list's offset, a jump becoming the distance in steps, and the frame
  bytes, each 4 turned, a frame record numbered by first use. An arcade
  fighter's records can lie in several groups with tables between them. The
  PS1 dropped arcade records (Ryu's last 196; Zangief's PL10 245, with
  countdowns lengthened to cover them, while PL16 has nearly all) and added
  steps of its own: sequences for projectiles that the arcade animates in its
  code (Ryu's 11 fireball loops, Ken's, Dan's), and small runs in Rolento,
  Dhalsim, Sakura and others. `sequence.c` builds the arcade-sourced steps,
  frame records and step tables of the 37 files `fighter.c` covers; which
  arcade records make which PS1 steps is in its generated table (`sequences.py
  --c-table`). In the runtime 34,771 steps, 11,313 frame records and 9,075
  table entries equal the disc's and are served; 4,080 steps, the PS1's own or
  changed, stay the disc's.
- **The other files** (`other_files.py`, `screens.py`). The select,
  continue, ending, boss and demo screens are mostly arcade tiles and
  palettes (from 65% to 100% of their tiles), with modules that are code and
  cell lists of the art records. `screens.c` builds their tiles, palettes and
  pictures by recipes (generated `screens_table.c`: runs of arcade tiles,
  pieces of scroll 1, palette addresses), 379 chunks of 172 files; the
  loader serves the bytes equal to the disc's and the packed tile segments
  whose tiles decode to the disc's (in a segment, the PS1's own tiles come
  from the disc). The screen modules' cell lists (`screen_lists.c`) follow
  the fighters' rules but for the numbering, which looks each tile up in the
  chain's tile segment (the lowest number with the same pixels, either flip),
  a part per change of x by other than 16, and a palette map per run of
  records: 1.16 MB of their lists and positions are built and served. Not
  built: pictures the PS1 cut from scroll data and the text lines.
  The PS1's own: the movies (4 STR), the recorded music (8 XA files), the
  four full-color ending illustrations, the title picture, the layout of the
  dialogue text, the select screen's 27 voice samples, `CAPCOM.BIN`.
- **Effects and pointers** (`program_more.c`). The effects' cell lists and
  positions in the program's data are the fighters' format over arcade art
  records `0x217fd4` to `0x21b12e` (bank `0x20000`), numbering into a tile
  chunk that every stage file holds (slot 3); the PS1 left out 40 cells its
  art tool cleaned to blank (a table) and added 6 full-screen pictures of its
  own. 216 of 223 lists and their positions are built (11,334 bytes). The
  program's 2,125 pointer words are a table of the build's layout. Two more
  layouts for the pieces: `n` (a PS1 byte from a 16-bit arcade value) and
  `e` (a PS1 32-bit value from an arcade byte).
- **The rest of the fighter modules' data** (`module_data.py`, every byte
  of the 45 modules classed): after the code (1.07 MB) and the blocks of
  `fighter.c` and `sequence.c` (1.73 MB), 845 KB are left, of which 90% are
  built by `module_data.c` (generated table of pieces and pointer words):
  450 KB equal to arcade tables, 119 KB of records the PS1 narrowed (hurt
  boxes, 8 bytes to 6), 69 KB of pointer words, 125 KB of zeros. Left: the
  PS1's own tables and art (77 KB) and some near-arcade bytes.
- **The rest of the program's data** (`program_map.py`, every byte
  classed): 29% arcade tables (the 72 pieces), 19% cell lists and position
  runs of effects in the fighters' format, 12% the Sony library's data
  (PsyZ replaces the library), 9% pointers (fixed by the port's layout), 5%
  text (mostly the PS1's own), 12% zeros, 14% the PS1's own tables (among
  them the disc's file table, which a disc of the loader's must fill).
- **Palettes and pictures** (`fighter_parts.py`). The 64 palettes of each
  fighter file are the arcade's (each 4-bit channel doubled, arcade color 15
  becomes the transparent 0, a black of 0 is written 0x8000): 88% of their
  bytes exactly; the rest are filler ramps of the PS1's own and 62 palettes
  that differ from this US arcade set (perhaps the Japanese revision's). The
  portrait pages are the arcade's portrait sprites (a mode-4 art record
  through the pointer at `0x5e6c0 + 0x14*fighter`, cells at
  ((x+96)/16, (y+80)/16)), the shared "VS" page four records from
  `0x235658`; the PS1's own choices left are which of 12 shadow cells a page
  holds and one dropped cell. `fighter_parts.c` builds the palettes and pages
  of all 45 files (equal to the disc's but the 62 palettes and Gouki's
  alternate portrait, which stay the disc's). The sound command lists and
  the `pQES` sequences of the fighter files are the PS1's own.
- **Sound banks and music** (`samples.py`). The arcade's instrument table is
  in the Z80 program at `0x8000` (435 records: bank, start, loop, end, root
  key). The PS1's VAB headers rebuild byte for byte from rules, the arcade and
  a recipe of the PS1's own (per tone: key ranges, envelopes, the rate the PS1
  chose for each sample), which has no arcade source. `vag.c` encodes a
  sample body from the arcade's at the PS1's rate and size (295 of 409 built,
  sizes equal; not wired in yet). The fighters' `pQES` sequences are short
  echo and chime patterns, not songs. The fight and menu music is the XA
  streams: 64 recorded tracks, with no arcade source; the arcade's music is
  59 songs of up to 16 tracks for its Z80 sound program (command table at
  `0x9006`, interpreter at `0x0b07`). [`music.md`](music.md) specifies it
  (songs, effects, the track code, the 32 control commands, envelopes, the
  volume, pitch and rate formulas, what the chip must do), and `music.py` is
  its reference in Python: its register writes equal the sound program's for
  all 59 songs and 868 effects (compared, privately, with an emulation of the
  sound program that is not part of the project). The fighters' themes map to
  songs by the fighter index; the other screens' songs are not all settled.
  `music.c` is the player of the port's own (the driver's rules and a mixer
  of 16 voices of the arcade's samples, 44.1 kHz; the pan law and the echo
  are the guesses `music.md` section 13 names), about 1,000 times faster
  than real time. In the runtime, when the game starts one of the 35
  recorded tracks whose arcade song is known, the arcade's song plays in its
  place, the stream still read and consumed so the game's timing and its
  volume changes hold; other tracks keep the recording.
- **Samples** (`samples.py`, all banks). 355 of the 409 distinct PS1
  samples are arcade samples: the chip's sample taken as 24,000 Hz,
  resampled to a rate the bank's tone gives (44100 times 2 to the power of
  (72 minus the center key plus the shift) over 12, from a list of 11
  rates), times 256, SPU ADPCM encoded, with a zero block first and the
  PS1's one-shot flags. The PS1's encoder was coarse (17 dB against the
  arcade data; a careful encode gets 29), so the bytes will not be equal,
  but the sizes are. The 27 samples of the select screen and 27 others have
  no source found.
- **Sound.** There is no arcade sound driver in the port yet: the arcade
  intro plays the PS1 disc's recorded opening music (`port/intro/audio.c`).

## How the pieces were found, and their limits

The PS1 program's data was compared with the arcade program in 2-byte
units, each equal either as it is or with its two bytes turned. Runs of at
least 32 bytes, with at least 16 that are not zero, became pieces. Each
piece's layout is what its own bytes show; a unit whose two bytes are the
same either way takes the layout of the rest. So:

- A piece is equal to the disc's bytes by construction; `--arcade-check`
  shows that the table, the archive reader and the layouts agree, not that
  each piece is the arcade's table of the same meaning.
- Most pieces are whole tables or long runs at a steady stride; a few short
  ones were found by value alone. What each table means is not read yet.
- Data that holds addresses (pointers) does not agree in this way and is not
  a piece. Neither are the PS1's own tables.

## Checks

- `python3 port/tools/test_arcade_zip.py`: the archive reader and inflate,
  on synthetic archives (stored, every deflate kind, malformed ones).
- `python3 port/tools/test_arcade_program_map.py`,
  `test_arcade_fighter_parts.py`: the analysis tools' pure functions, on
  synthetic data. The tools themselves (`program_map.py`,
  `fighter_parts.py`) take the private inputs and print a report.
- `python3 port/tools/test_arcade_rom.py`: the four views from a made-up
  set with the real member names and sizes, against a reference built from
  `port/intro/import_scene.py`'s layout; the digests refuse it; the piece
  builder's layouts and bounds and the table's order.
- With the private inputs: `--arcade-check` with the disc and the set. On
  2026-10-23: 72 pieces, 25,784 bytes, none differs; 27 of 37 fighter files'
  blocks equal, the 10 others the PS1's known changes; the 27 sprite chunks
  decode to the disc's tiles, tile for tile; 0.7 seconds in all with the
  set's reading and digests. With the color conversion broken by hand the
  check fails on every tile. With one piece's layout, or one
  fighter segment's count, changed by hand the check names it and ends with
  status 1. A run with the blocks in place went from the title into a fight
  (Ryu against Dhalsim, both served from the arcade's blocks).

## Next

The rest of the program's data (pointers, the PS1's own tables), then the
files in the order the game reads them: the sprite files first, whose art
the arcade has entire, with their animation records rebuilt from the
arcade's.
