# PS1 overlay map

Parent: [delivery plan](../../PLAN.md). Tools: `ps1/tools/pac.py` and
`ps1/tools/funcscan.py`.

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

The executable holds the tables a second time, 0x694 bytes further on. The
contents of the five bounded tables are identical there, and so are the
eight words from the start of table 5 to the address block. The second
address block is not identical to the first: each of its six words is the
first block's word plus 0x694, so it points at the second set of tables. A
scan of the code for instruction pairs that form an address found none that
points at the second set. That does not prove it unused.

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

Inferred from these counts, not established: a second-side block is its
first-side block linked `0x18000` higher, from the same source. The counts
support that reading strongly and do not prove it for every block. 57 words
fall outside the classes and were not explained one by one, and the pair of
`PL06` was not compared at all. Some of the 57 are addresses in the modules
of the next table, which sit at other distances.

Four characters carry one more module per side. Its two copies sit at
another distance than the blocks do:

| Files | First side | Second side | Words identical | Explained by the distance | Other |
| --- | --- | --- | ---: | ---: | ---: |
| `PL09`, `PL09X` | slot `0x0` at `0x80075e00` | slot `0x8` at `0x8008bf00` | 3,918 | 387 | 0 |
| `PL0E`, `PL0EX` | slot `0x2b` at `0x80077000` | slot `0x2c` at `0x8008bf00` | 7,679 | 463 | 0 |
| `PL11`, `PL11X` and `PL13`, `PL13X` | slot `0x16` at `0x8007bc00` | slot `0x17` at `0x8008bf00` | 5,580 | 106 | 8 |

The 8 other words of the last row have an explanation since. `sides --first
0x16 --second 0x17 --show 4` prints them: in each pair four `jal` words,
whose targets lie `0x18000` higher on the second side. That is the distance
of the character blocks, not of this module. The build gives the three
names behind them their second-side addresses in the image `slot17`, which
is exact with them. The count in the table is the measurement as it was
made and stays.

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
  the slot `0x8` module, at the same offset in both.
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

## Functions inside the modules

`pac.py functions` sweeps every code-bearing chunk of table 0 for function
boundaries with `funcscan.py`, once per distinct content of a slot, and
counts what it finds. Everything in this section is a static estimate. A
function listed here is established only when the matching build rebuilds
it.

### How a boundary is found

A function starts at the first word that is neither zero nor an impossible
instruction. It ends with the delay slot of the first `jr ra` that lies at
or beyond every forward branch target and jump table case seen so far. A run
is cut where a `jal` in the same chunk calls, or where a symbol of
`ps1/src/symbols.ld` points at a word at which a function with a stack
frame can start in that module. A run that never returns is set aside as
data, unless it was cut at a called address and holds a `jal` itself.
`funcscan.py` states the rules in full, `pac.py` what it takes as such a
start.

### The method against the resident executable

The resident executable has an inventory of 1,649 functions made with
another tool. `funcscan.py compare` sweeps the same range:

- All 1,649 starts are found, 1,643 of them with the same size.
- Five functions are 20 to 32 bytes longer in the sweep. The words in
  question are register restores, `jr ra` and its delay slot, which the
  inventory leaves out.
- The program entry routine is 16 bytes longer: the sweep takes in the four
  table words after it. It is the one function that ends without a return.
- The sweep reports 202 functions that the inventory does not list.
  `compare --show 202` names them with their sizes. The next section sorts
  them. The counts of inventoried functions elsewhere in this project do
  not include them.

### The functions the resident inventory does not list

`pac.py unlisted` sweeps the resident executable as `compare` does, takes
the functions whose start the inventory does not have, and says three
things about each:

- Its area. A function that starts below `--library` is game code, any
  other library code. The address used, `0x80157000`, is the project's
  working boundary. It is approximate: it lies inside a function that
  starts below it and is counted as game code.
- Whether the build has it: whether a unit of the resident image declares a
  function at that address in the build configuration. A unit is of the
  resident image when it has no `image` key or the key says `resident`.
- One way it is referred to, the first of these that applies:
  1. called by the executable: a `jal` or a `j` to its start inside a
     function that the sweep finds in the executable;
  2. called by modules only: the same inside a function that the sweep
     finds in a code-bearing chunk of table 0, each distinct content of a
     slot read once;
  3. address in a data word: a word equal to its start, in the executable
     or in such a chunk, outside every function that the sweep finds there;
  4. address formed in code: a `lui` and the first `addiu` or `ori` after
     it that reads the register it loaded, at most eight words later and
     before another `lui` into that register, which together form its
     start;
  5. no reference found.

A jump from inside the function itself is not counted. The classes say what
was counted. A word may equal an address by chance, the two halves of a
pair need not belong together, and a function without a counted reference
may be reached in a way the command does not look for.

The sweep has to reach the end of the code. The inventory ends one function
early: swept to the inventory's end, the command reports one function of
the build that is no start of the sweep, at `0x8016d910`. The build declares
it with 16 bytes, so the code ends at `0x8016d920`, and `--end` says so.

What the command prints for that range:

- 1,852 functions, 348,000 of the 348,192 bytes swept. The words outside
  them are 48 zero words and no other. Every word of the resident code is
  in a function of the sweep or is padding.
- All 1,649 starts of the inventory are found, and every function of the
  build is a start of the sweep.
- 203 functions are not in the inventory, 19,760 bytes: the 202 above and
  the one after the inventory's end.

| Area | In the build | Reference | Functions | Bytes |
| --- | --- | --- | ---: | ---: |
| game | no | called by modules only | 29 | 3,700 |
| game | no | address in a data word | 88 | 11,024 |
| game | no | address formed in code | 1 | 216 |
| game | no | no reference found | 15 | 596 |
| game | yes | address in a data word | 1 | 8 |
| library | no | called by modules only | 2 | 636 |
| library | no | address in a data word | 2 | 264 |
| library | no | no reference found | 3 | 168 |
| library | yes | called by the executable | 2 | 24 |
| library | yes | called by modules only | 13 | 156 |
| library | yes | address formed in code | 2 | 212 |
| library | yes | no reference found | 45 | 2,756 |

By area: 1,302 game functions in the inventory and 134 not, 1,436
together, of which the build has 1,231. 347 library functions in the
inventory and 69 not, 416 together, of which the build has 386.

The table is the state when the functions were sorted. A round on the 133
game functions that the build did not have followed. The same command now
prints 119 of the 134 as in the build, among them all 15 without a counted
reference, and 1,349 game functions in the build.

What this sorts out:

- All 134 in the game area are counted as game functions from here on, and
  all 69 in the library area as library functions. The count of functions
  to rebuild in the resident image is 1,852, not 1,649. This is accounting,
  not proof: each of the 203 is a candidate with a boundary from the sweep,
  the split by area rests on an approximate address, and none is shown to
  be a function of the original source. A candidate is established when
  the build rebuilds it. The inventory file itself is not changed; reports
  that say "of the 1,302 inventoried game functions" mean the inventory.
- Inferred, not established: the inventory was made by following calls
  inside the executable. Only 2 of the 203 are called from there. The others
  are called from modules, stand in tables, or have no counted reference.
- 119 of the 134 game functions have a counted reference. For the other
  15 the boundary from the sweep is the only evidence. Nine of those are 8
  bytes by the rows of `--out`, a `jr ra` and its delay slot. Such a pair
  is either an empty
  function or a last return of the function before it that nothing
  reaches. Which it is, is not decided here; the matching build decides it
  for each when the code around it is rebuilt.
- 45 library functions of the build have no counted reference. That fits
  library objects that are linked whole and used in part, and is not
  examined further.

### Counts

77 distinct contents in 16 slots hold 11,220 functions, 1,461,872 bytes.

| Slot | Destination | Contents | Functions | Bytes | Distinct by bytes | Distinct address-blind | In no other slot |
| ---: | --- | ---: | ---: | ---: | ---: | ---: | ---: |
| `0x0` | `0x80075e00` | 1 | 112 | 13,120 | 107 | 91 | 0 |
| `0x1` | `0x80010000` | 1 | 124 | 20,468 | 108 | 100 | 78 |
| `0x4` | `0x801b0000` | 24 | 4,540 | 570,636 | 3,794 | 2,211 | 77 |
| `0x5` | `0x801c8000` | 20 | 3,823 | 464,404 | 3,287 | 2,129 | 2 |
| `0x6` | `0x801e8000` | 20 | 874 | 159,724 | 770 | 532 | 518 |
| `0x8` | `0x8008bf00` | 1 | 112 | 13,120 | 107 | 91 | 0 |
| `0xb` | `0x801e0000` | 1 | 61 | 7,960 | 54 | 51 | 39 |
| `0xf` | `0x800df000` | 1 | 228 | 36,912 | 215 | 187 | 141 |
| `0x12` | `0x80010000` | 1 | 187 | 29,928 | 167 | 156 | 106 |
| `0x16` | `0x8007bc00` | 1 | 11 | 984 | 11 | 9 | 0 |
| `0x17` | `0x8008bf00` | 1 | 11 | 984 | 11 | 9 | 0 |
| `0x27` | `0x80010000` | 1 | 212 | 31,684 | 190 | 163 | 143 |
| `0x28` | `0x80010000` | 1 | 683 | 85,408 | 555 | 319 | 305 |
| `0x2a` | `0x801e0000` | 1 | 28 | 6,052 | 27 | 26 | 23 |
| `0x2b` | `0x80077000` | 1 | 107 | 10,244 | 98 | 73 | 0 |
| `0x2c` | `0x8008bf00` | 1 | 107 | 10,244 | 98 | 73 | 0 |

"Functions" and "Bytes" add up every content of the slot. "Distinct by
bytes" counts a function once per slot however many contents hold the same
bytes. Over all slots 8,822 functions are distinct by bytes.

"Address-blind" compares functions after setting to zero what depends on
where things are linked: the target of every `j` and `jal`, the 16-bit field
of every `lui`, and the 16-bit field of a later instruction that uses the
register so loaded as its base. `pac.py` has the exact rule. It is an
estimate of how much code is shared and nothing more: two functions that
agree this way may still address different things, and two that disagree
may be one source with other constants. Over all slots 3,749 functions,
677,624 bytes, are distinct this way.

### Which modules share code

All counts here are address-blind.

- The first-side and second-side character blocks, slots `0x4` and `0x5`,
  have 2,127 functions in common. Slot `0x4` has 2,211 and slot `0x5` 2,129.
- The three pairs of extra modules hold the same functions on both sides:
  91 in slots `0x0` and `0x8`, 9 in `0x16` and `0x17`, 73 in `0x2b` and
  `0x2c`.
- Slots `0x2b` and `0x2c` each have 23 functions in common with each of the
  two character block slots. Slots `0xf` and `0x12` have 46 in common. No
  other two slots have 20 or more.
- Within a slot that has several contents, most functions belong to one
  content. Of the 2,211 functions of slot `0x4`, 1,498 are in one of its 24
  contents only and none is in all of them. Of the 2,129 of slot `0x5`,
  1,781 are in one of its 20 contents only and 4 in all. Of the 532 of slot
  `0x6`, 481 are in one of its 20 contents only and 3 in all.

The four modules with one content across many files:

| Slot | Functions | Bytes | Distinct address-blind | In no other slot |
| ---: | ---: | ---: | ---: | ---: |
| `0xb` | 61 | 7,960 | 51 | 39 |
| `0x2a` | 28 | 6,052 | 26 | 23 |
| `0x12` | 187 | 29,928 | 156 | 106 |
| `0x28` | 683 | 85,408 | 319 | 305 |

Together 959 functions, 129,348 bytes. Slot `0x28` repeats itself: its 683
functions are 555 by bytes and 319 address-blind.

### What supports the boundaries, and what does not

Both commands end with four counts that a wrong boundary can disturb. For
the 77 contents, and for the resident range, they are the same:

- no `jr ra` word lies outside the functions found;
- no function holds more than one `jr ra`;
- no function opens more than one stack frame;
- one function ends without a return.

In the resident executable that one is the program entry routine. In the
archives it is at `0x801b6370` in the block of `PL17.PAC`: a `jal` at
`0x801b6fd8` in the same block targets `0x801b63fc`, a word inside it, so
the sweep cut it there and counts the rest as a second function at
`0x801b6400`. Why that call points there is not established.

These counts do not prove a boundary. A function cut in two, or two taken
as one, goes unnoticed when neither part opens a frame or holds a second
return. Where a function starts is weaker than where it ends: instruction
shaped data directly before a function that nothing calls becomes part of
it. 557 times a symbol's address lies in a module, and 59 times it was
taken as a function start there.

## Reproducing

With the executable and the archives extracted as the
[PS1 instructions](../README.md) describe:

```sh
.venv/bin/python ps1/tools/pac.py loadmap EXECUTABLE PAC_DIRECTORY \
    --pointers 0x8017eb1c --symbols ps1/src/symbols.ld
.venv/bin/python ps1/tools/pac.py sides EXECUTABLE PAC_DIRECTORY --pointers 0x8017eb1c
.venv/bin/python ps1/tools/pac.py sides EXECUTABLE PAC_DIRECTORY --pointers 0x8017eb1c \
    --first 0x16 --second 0x17
.venv/bin/python ps1/tools/pac.py functions EXECUTABLE PAC_DIRECTORY --pointers 0x8017eb1c \
    --symbols ps1/src/symbols.ld --out FUNCTIONS_TSV
.venv/bin/python ps1/tools/funcscan.py compare EXECUTABLE --base 0x80118900 --offset 0x800 \
    --inventory RESIDENT_INVENTORY_TSV
.venv/bin/python ps1/tools/pac.py unlisted EXECUTABLE PAC_DIRECTORY --pointers 0x8017eb1c \
    --inventory RESIDENT_INVENTORY_TSV --library 0x80157000 --end 0x8016d920 \
    --config ps1/src/build.toml --symbols ps1/src/symbols.ld --out UNLISTED_TSV
```

`0x8017eb1c` is the address of `data_8017eb1c` in `ps1/src/symbols.ld`.
`loadmap` exits with an error if any estimate differs from the table.
`functions --out` writes one line per function: the first archive with that
content, the slot, the address, the size and the address-blind hash.
`compare` exits with an error here, because the sweep and the inventory
differ as described above. `0x80118900` is the executable's load address
and `0x800` the size of its header.

`ps1/tools/test_disc_tools.py` holds control cases for the `pac.py` commands
and `ps1/tools/test_funcscan.py` for the sweep, all on synthetic inputs.

## Open

- Decide which overlay blocks enter the matching build, and how it treats
  the two sides. Proposed, on the strength of the comparison above: one
  source and a second link at the other address. Whether that holds for a
  block is only shown when both of its links rebuild exactly. It is shown
  for one part of one block: the 11 functions at the start of slots `0x16`
  and `0x17`, 984 bytes, are exact on both sides from the same objects. The
  rest of that block is retained raw on both sides, and no other block has
  been tried.
- The inventory of functions is an estimate from a sweep. Names, and which
  functions of different characters are one source, are not established.
- Of the 203 functions of the resident executable that the inventory does
  not list: the 15 in the game area without a counted reference, and the 7
  in the library area that the build does not have and no library function
  is identified for yet.
- The call into the middle of a function in the block of `PL17.PAC`.
- How the loader treats slot `0xffff` and table 5, and the order in which it
  uses the range at `0x801e0000`.
- What owns the 11 data symbols above the stage blocks.
- The four stand-alone `PS-X EXE` files under `PAC/` are not analysed beyond
  the function that loads them.
