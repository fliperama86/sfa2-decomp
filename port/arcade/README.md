# Arcade assets

The loader that builds what the game reads from the user's arcade set
(MAME's `sfa2u`), so that in the end the port needs no PS1 disc. Setting
`[experimental] arcade_assets`, default off; `--arcade-assets` and
`--ps1-assets` override it, and `--arcade-rom FILE` names the set
(`sfa2u.zip` by default). Nothing under `ps1/` changes, and no byte of
either game enters Git: the source holds addresses, sizes and layouts.

**State, 2026-10-23: the loader opens and checks the arcade set and builds
72 tables of the PS1 program's data from it (25,784 of 90,088 bytes). The
disc gives everything else. Without a disc the program stops with what it
cannot build yet.**

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
```

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
- With a disc, the first read of each disc file prints
  `arcade: NAME from the disc: no arcade source yet`. Played through, those
  lines are the list of files the loader has still to build, in the order
  the game wants them.
- `--arcade-check` builds every piece and compares it with what the disc
  gave at its place, prints each one that differs, and ends (status 1 if
  any differs).

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
  files). Ryu's and Dan's lists rebuild exactly, but for one tile of Ryu's
  that the PS1 stores flipped. Positions are byte-equal in 16 of 23
  characters. The rest differ where the PS1 changed things: tiles stored
  flipped (a few in most fighters), a palette or flip bit here and there,
  and in Zangief (PL10) and PL11/PL13 art of the PS1's own (poses turned
  half round, tiles the arcade lacks). Some fighters' tile codes are in
  another bank of the arcade graphics (+0x20000); some lists come from
  arcade records of mode 0 (one code a cell). Where a record's blocks step
  back in x, the record is in parts: the PS1 lists the parts last first,
  each sorted by (y, x) (Vega's 297 lists all rebuild with it; PL0E also
  starts a part at a change of palette). The arcade build does not copy the
  PS1's changes: it converts the arcade's records.
- **Room and flipped tiles.** The PS1 code reads a module's data at fixed
  addresses, and only 0 to 2 bytes follow the lists, so the lists must come
  out at the PS1's size. The PS1 stores each tile with the flip of its first
  use in the file, and a list's flag holds a use's flip relative to that
  (exact for all 106,434 tile codes of the 20 fighters whose art is the
  arcade's). With that rule the lists of 16 fighters rebuild byte for byte
  and fit by construction; PL02/PL14 and PL0B differ only in the PS1's
  palette changes; PL08 and PL0E (Sakura) number some body-part tiles
  together, and PL0E's lists come out 28 bytes over.
- **Stages** (`stages.py`, all 20). The two layers of 16- and 32-pixel
  cells are the arcade's scroll maps cell for cell (all cells in 17 and 19
  stages); their palettes are the arcade's (4-bit channels scaled
  `n*17*43/45`); the shared and object tiles and the object palettes are
  arcade tiles and palettes, flips stored as separate tiles. Open: the third
  layer is arcade art (stage 00 entirely) but the arcade map that places it
  is not found, its palettes mostly differ, stage 04's object tiles are the
  PS1's own, and the order in which the PS1 packs tiles into pages is not
  fully derived.
- **Animation steps** (`sequences.py`). A fighter's steps follow its
  lists: 12 bytes each, from the arcade's sequence records (countdown,
  flags, art pointer, 16 frame bytes, and a 4-byte jump target when flags
  bit 15 is set): duration and flags as they are, the art pointer becoming
  the list's offset, a jump becoming the distance in steps, and the frame
  bytes, each 4 turned, a frame record numbered by first use. 39,628 of the
  46,483 steps of the 45 files and 13,007 of 13,869 frame records rebuild
  byte for byte. The PS1 dropped arcade records (Ryu's last 196; Zangief's
  PL10 245, with countdowns lengthened to cover them, while PL16 has nearly
  all) and added steps of its own (Ryu 89, Dan's PL0E 1,138). Which arcade
  records the PS1 kept is not in the arcade bytes.
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
  portrait pages are arcade tiles and arcade palettes; which tile goes in
  which cell is not derived yet. The sound command lists and the `pQES`
  sequences of the fighter files are the PS1's own.
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
  2026-10-23: 72 pieces, 25,784 bytes, none differs, in 0.2 seconds with
  the set's reading and digests; with one piece's layout changed by hand the
  check names that piece and ends with status 1.

## Next

The rest of the program's data (pointers, the PS1's own tables), then the
files in the order the game reads them: the sprite files first, whose art
the arcade has entire, with their animation records rebuilt from the
arcade's.
