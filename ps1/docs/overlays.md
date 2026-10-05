# PS1 overlay map

Parent: [delivery plan](../../PLAN.md). Tool: `ps1/tools/pac.py`.

No game was run for anything on this page. Two kinds of evidence are kept
apart:

- **Exact code.** Functions of the resident executable that the
  [matching build](matching-build.md) rebuilds byte for byte from the C in
  `ps1/src/`. What such a function does is read from its source.
- **Static measurement.** Counts and comparisons over the disc files and the
  executable's data, made with `pac.py`. They say what the bytes are, not when
  or whether the game uses them.

Where a statement is an inference, it says so.

## Archive format

The 239 `*.PAC` files share one container layout:

- offset 0: little-endian u32 chunk count;
- offset 0x20: one 32-byte entry per chunk: a u16 slot, a u16 table number, a
  u32 size in bytes, and a copy of the chunk's first four bytes;
- offset 0x800 onward: chunk data in entry order, each chunk padded to a
  2,048-byte boundary.

`pac.py` rejects a file unless every entry's first-word copy matches the data
and the last chunk ends exactly at the file size. All 239 files pass. They
hold 1,572 chunks.

The split of the first entry word into slot and table number comes from exact
code, see the next section. An earlier version of this page read the word as
one u32 "type". For table 0 the two readings give the same number.

## What the resident loader does

From exact functions in `ps1/src/s14f9f0_r3.c` and `ps1/src/y14e890_r2.c`:

- `func_801501a0` steps to the next entry. It takes the size from offset 4,
  the table number from the u16 at offset 2 and the first-word copy from
  offset 8. Unless the table number is 5, it sets the chunk's destination to
  `data_8017eb1c[table][slot]`, the slot being the u16 at offset 0.
- `func_8015003c` transfers a chunk to its destination one sector at a time
  with `CdGetSector`. Of the last sector it transfers only the words the
  chunk needs to the destination; the rest of that sector goes to the bytes
  that end at `0x801e8000`.
- `func_80150180` compares the first word at the destination with the entry's
  copy and sets a state byte to 3 when they differ.
- `func_80150100` transfers a sector into a buffer at `0x801e0000` plus a
  multiple of 2,048 instead. `ps1/src/s14f9f0_r4.c` hands that buffer to
  `SsVabTransBodyPartly`.
- `func_8014f194` loads a stand-alone executable: it transfers the header
  sector to `0x801e0000`, takes the load address from header offset 0x18 and
  the size from offset 0x1c, and transfers that many sectors there.

The library names are those of the public SDK reconstruction that the
matching library functions were identified against.

`func_8014ff20`, which also addresses `data_8017eb1c`, is not exact yet and
is not relied on here.

## Destination tables

`data_8017eb1c` is a block of six addresses of tables inside the resident
image. `pac.py loadmap` reads them. A table is taken to end where the next
one starts, which bounds its length from above.

| Table | Entries | Holds | Chunks on the disc |
| ---: | ---: | --- | ---: |
| 0 | at most 51 | addresses in main memory | 760 |
| 1 | at most 1 | `0x80010000` | 91 |
| 2 | at most 1 | `0x80010000` | 239 |
| 3 | at most 8 | addresses inside the resident image's own range | 240 |
| 4 | at most 4 | four values below `0x80000`, not main memory addresses | 120 |
| 5 | not bounded | not used by `func_801501a0` | 122 |

What tables 3, 4 and 5 address is not established. Inferred from the sound
library calls in the same units: sound headers, sample data and sequences.

The executable holds a second, identical copy of the tables and of the
address block, 0x694 bytes further on. A scan of the code for instruction
pairs that form an address found none that points at the copy. That does not
prove it unused.

## The static estimates agree with table 0

`pac.py` counts a chunk as code when it holds at least eight `jr ra` words,
and estimates its link address as the base under which the most absolute
call targets inside the chunk land on function prologues inside the same
chunk. 247 chunks count as code. All of them are in table 0, in 16 slots.
For every one of the 16 the estimate equals the table's entry:

| Slot | Destination | Chunks | Distinct contents | Largest | Found in |
| ---: | --- | ---: | ---: | ---: | --- |
| `0x4` | `0x801b0000` | 25 | 24 | `0x1ec9c` | character files, select screen |
| `0x5` | `0x801c8000` | 21 | 20 | `0x17ff4` | second-side character files |
| `0xb` | `0x801e0000` | 24 | 1 | `0x7acc` | character files |
| `0x2a` | `0x801e0000` | 63 | 1 | `0x52cc` | boss, demo and related files |
| `0x6` | `0x801e8000` | 20 | 20 | `0x13d1c` | stage files |
| `0x1` | `0x80010000` | 46 | 24 | `0xb0dcc` | 21 of the 46 chunks are code |
| `0x12` | `0x80010000` | 42 | 1 | `0x1d580` | |
| `0x28` | `0x80010000` | 21 | 1 | `0x41ec0` | |
| `0x27` | `0x80010000` | 1 | 1 | `0x1f15c` | `SELECTA.PAC` |
| `0xf` | `0x800df000` | 1 | 1 | `0x195d0` | `DEMO.PAC` |
| `0x0` | `0x80075e00` | 1 | 1 | `0x4344` | `PL09.PAC` |
| `0x8` | `0x8008bf00` | 1 | 1 | `0x4344` | `PL09X.PAC` |
| `0x2b` | `0x80077000` | 1 | 1 | `0x7f38` | `PL0E.PAC` |
| `0x2c` | `0x8008bf00` | 1 | 1 | `0x7f38` | `PL0EX.PAC` |
| `0x16` | `0x8007bc00` | 2 | 1 | `0x2c7e` | `PL11.PAC`, `PL13.PAC` |
| `0x17` | `0x8008bf00` | 2 | 1 | `0x2c7e` | `PL11X.PAC`, `PL13X.PAC` |

This confirms the addresses that the earlier version of this page gave as
estimates, including the ones it called weak. It is agreement between two
static sources: the code inside the chunks and a table in the executable
that an exact function indexes. It is not an observed load.

195 chunks have no entry: the 122 of table 5, and 73 chunks of table 0 with
slot `0xffff`, each 0x4000 bytes. How the loader treats those is not read yet.

Four slots hold one content each across many files: `0xb`, `0x2a`, `0x12`
and `0x28`. Reconstructing each once covers 150 chunks.

## Above the resident image

The resident image ends at `0x801ae900`. Seven entries of table 0 lie above
it. Six of them have chunks on the disc:

| Slot | From | Largest chunk ends at |
| ---: | --- | --- |
| `0x4` | `0x801b0000` | `0x801cec9c` |
| `0x26` | `0x801b0000` | `0x801ba000` (one chunk, no code, `SELECTA.PAC`) |
| `0x5` | `0x801c8000` | `0x801dfff4` |
| `0xb`, `0x2a` | `0x801e0000` | `0x801e7acc` |
| `0x6` | `0x801e8000` | `0x801fbd1c` |

Slot `0x32` has the entry `0x801e6000` and no chunk on the disc.

The first-side and second-side blocks are `0x18000` apart. Two first-side
blocks are larger than that, those of `PL16.PAC` and `PL17.PAC`. Neither
file has a second-side twin.

The loader's sector buffer at `0x801e0000` and the discarded tail that ends
at `0x801e8000` lie in the range of slots `0xb` and `0x2a`. The order in
which the loader uses that range is not analysed.

## The two sides

`PL##.PAC` carries a character's first-side block (slot `0x4`), `PL##X.PAC`
the second-side block (slot `0x5`). `pac.py sides` compares each pair word by
word and names how a differing word relates to the distance `0x18000`:

| Words | Relation |
| ---: | --- |
| 391,211 | identical |
| 12,981 | a word that points into the first block, moved by the distance |
| 3,702 | a jump or call target moved by the distance |
| 1,430 | upper half of an address (`lui`), one or two higher |
| 1,429 | lower half of an address, differing by `0x8000` |
| 57 | none of these |

20 pairs were compared. The pair of `PL06` differs in size by four bytes and
was not compared. The classes say what a difference is consistent with; a
word is not proven to be an address by falling into one.

Read as: the second-side block is the first-side block linked `0x18000`
higher. Most of the 57 other words follow from the next point.

Four characters carry one more module per side, and its second-side copy is
linked at a different distance:

| Files | First side | Second side | Words identical | Explained by the distance | Other |
| --- | --- | --- | ---: | ---: | ---: |
| `PL09`, `PL09X` | slot `0x0` at `0x80075e00` | slot `0x8` at `0x8008bf00` | 3,918 | 387 | 0 |
| `PL0E`, `PL0EX` | slot `0x2b` at `0x80077000` | slot `0x2c` at `0x8008bf00` | 7,679 | 463 | 0 |
| `PL11`, `PL11X` and `PL13`, `PL13X` | slot `0x16` at `0x8007bc00` | slot `0x17` at `0x8008bf00` | 5,580 | 106 | 8 |

`PL15`, `PL16` and `PL17` have no `X` twin, and neither has `SELE.PAC`. The
resident image has two tables of 24 addresses, `handlers_left` and
`handlers_right`. Every entry of the first lies in the first-side block.
Entries 0 to 20 of the second lie in the second-side block and entries 21
to 23, the numbers of those three files, in the first-side block. Inferred:
when one of those three characters is the second player, its code runs from
the first-side block.

A third table, `table_801725a0`, has 20 entries, all `0x801e8000`, the
destination of the 20 stage blocks.

## What the resident code refers to outside itself

`pac.py loadmap --symbols ps1/src/symbols.ld` lists every symbol that the
reconstructed source assigns outside the image, and the table 0 chunks whose
destination range covers it. 38 symbols are covered:

- Six at `0x80010000` to `0x80013834`. Two of them open a stack frame in the
  one content of slot `0x12`, one in the one content of slot `0x28`.
- `0x80078e44` opens a frame in the slot `0x0` module and `0x8008ef44` in
  the slot `0x8` module: the two placements of one extra character module.
- `data_800fb100`, the destination of slots 2 and 3.
- Six in the character blocks. `0x801b0000` opens a frame in 17 of the 25
  first-side blocks, so the block's start is code there, not data as the
  earlier version of this page said for all of them.
- Four at `0x801e0020` to `0x801e1d6c`. Slots `0xb` and `0x2a` have one
  content each, so each address means one thing per module. Two of the four
  open a frame in the slot `0xb` module, none in the slot `0x2a` module.
- 19 in the stage range, `0x801e8dc8` to `0x801ea640`. Each opens a frame in
  only one or two of the 20 stage blocks. Inferred: these calls belong to
  particular stages.

76 symbols are covered by no chunk: 65 in the scratchpad at `0x1f800000`,
and 11 from `0x801fc0a2` to `0x801fc248`, above the end of the largest stage
block. What owns those 11 is not established.

The matched `swap_kind` routine calls three of the character-block
addresses. They resolve to one routine in three placements:

| Called address | Placement |
| --- | --- |
| `0x801b5ab8` | first-side block of the two character files that the routine swaps between |
| `0x801cdab8` | the same routine in their second-side blocks |
| `0x801b5a34` | the same routine in the shared block of `PL17` |

The first instructions are identical in all three. In the two ordinary
character files, `0x801b5a34` falls in the middle of another function, so that
call is only valid when the shared file is loaded.

## Reproducing

With the executable and the archives extracted as the
[PS1 instructions](../README.md) describe:

```sh
.venv/bin/python ps1/tools/pac.py loadmap EXECUTABLE PAC_DIRECTORY \
    --pointers 0x8017eb1c --symbols ps1/src/symbols.ld
.venv/bin/python ps1/tools/pac.py sides EXECUTABLE PAC_DIRECTORY --pointers 0x8017eb1c
.venv/bin/python ps1/tools/pac.py sides EXECUTABLE PAC_DIRECTORY --pointers 0x8017eb1c \
    --first 0x16 --second 0x17
```

`0x8017eb1c` is the address of `data_8017eb1c` in `ps1/src/symbols.ld`.
`loadmap` exits with an error if any estimate differs from the table.
`ps1/tools/test_disc_tools.py` holds control cases for both commands on
synthetic inputs.

## Open

- Decide which overlay blocks enter the matching build, and how a build
  expresses one source linked at two addresses for the two sides. The
  measurements above say the second link differs by one distance per module.
- Function boundaries inside the blocks. Nothing here inventories them.
- How the loader treats slot `0xffff` and table 5, and the order in which it
  uses the range at `0x801e0000`.
- What owns the 11 data symbols above the stage blocks.
- The four stand-alone `PS-X EXE` files under `PAC/` are not analysed beyond
  the function that loads them.
