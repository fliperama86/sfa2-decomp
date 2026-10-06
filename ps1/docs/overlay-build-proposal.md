# Proposal: overlay modules in the matching build

Parents: [overlay map](overlays.md), [matching build](matching-build.md).

Status: a proposal. Nothing here is built. It asks the owner for the
decisions listed at the end. Numbers come from the overlay map.

## What the design has to respect

1. A module's baseline is a chunk of an archive, not an executable. It has no
   header. Its size need not be a multiple of four: the largest chunk of slot
   `0x16` is `0x2c7e` bytes.
2. Modules overlap in memory. Slots `0x1`, `0x12`, `0x27` and `0x28` all load
   at `0x80010000`, slots `0xb` and `0x2a` at `0x801e0000`, slots `0x8`,
   `0x17` and `0x2c` at `0x8008bf00`. Two such modules cannot be in one link,
   and an address alone does not say which function is meant.
3. One content sits in many files. The one content of slot `0x2a` is in 63
   chunks.
4. The resident source already names 38 addresses inside module ranges. One
   of them is a function start in the block of `PL17.PAC` and the middle of
   another function in two other character blocks.
5. Inferred, not established: a second-side block is its first-side block
   linked at another address. 57 words of the comparison are unexplained and
   one pair differs in size.

## Proposed design

### Images

Today the build has one payload: the resident executable. The proposal makes
that one case of an **image**: a payload, a load address and a pinned
baseline. A module is a second kind of image whose baseline is a chunk.

```toml
[[image]]
name = "slot2a"
archive = "../extract/PAC/BOSS00.PAC"  # one file that carries the content
slot = 0x2a                            # its chunk of table 0 with this slot
sha256 = "<hex of the chunk's bytes>"
address = 0x801e0000

[[unit]]
name = "example"
image = "slot2a"                       # the resident image when absent
source = "slot2a/example.c"
functions = [ { name = "example", address = 0x801e0020, size = 248 } ]
```

- Each image is linked alone, with its own linker script. Its raw ranges are
  the complement of its units inside its own payload. Its image hash, its
  per-function comparison and its comparator controls are its own.
- The ownership kinds stay as they are: c, asm, rodata, data, bss, raw. The
  rule that a bss range lies outside the payload means the image's own
  payload.
- The declared address is checked against the loader's destination table,
  read from the pinned resident executable at the address of
  `data_8017eb1c`. A difference is a configuration error. So is a chunk whose
  hash differs, an archive that `pac.py` would reject, and two chunks of that
  slot in the named file.
- A payload that ends one to three bytes after a word keeps those bytes raw.
- The image is the content. The report lists how many chunks of the archive
  directory carry the same bytes; other contents of the slot are not an
  error.
- `--image NAME` builds one image. Without it every image is built and the
  run is exact only if all are.

### Names across images

- Function and unit names are unique across all images, as they are inside
  one today.
- A link sees its own units, `symbols.ld`, and the declared functions of
  every other image that is not a second link (next section), as absolute
  addresses. Those come from the configuration, so no other image has to be
  built first.
- No such source of absolute addresses ever supplies a name that the
  objects of the current link define. That covers every global or weak
  definition of its units: functions and the symbols in read-only data,
  data and bss, whether the units are the link's own or taken over from the
  image it is like. An assignment under such a name would override the
  definition, which is the owned-symbol fault the build rejects today.
- The check of the linked file stays and runs per image: every such symbol
  must be bound inside its unit's section of that link, at the range that
  link gives the unit plus the symbol's offset.
- Data symbols of another image are reached through `symbols.ld` only, for
  now. Placing them needs that image's objects, as `fndiff.py` does for
  sibling units. That can follow when a unit needs it.
- The existing rule stays: a name that a unit defines must leave
  `symbols.ld`. When a module unit takes over one of the 38 addresses, the
  resident link then gets the name from that unit's declaration.

### The two sides

A second-side image is declared as **like** its first-side image:

```toml
[[image]]
name = "slot17"
like = "slot16"
archive = "../extract/PAC/PL11X.PAC"
slot = 0x17
sha256 = "<hex>"
address = 0x8008bf00
```

- It has no units of its own. It takes every unit of the image it is like,
  with each range moved by the difference of the two addresses, links the
  same objects a second time and compares the result with its own baseline.
- In that link the units define their names at the moved ranges. The
  functions of the image it is like are therefore not given to it as
  absolute addresses: the rule above excludes them, because its own objects
  define them. A call from one such unit to another binds to the second
  placement, never to the first.
- The moved ranges include bss. Whether a block's uninitialised data moves
  with it is not known. If it does not, the code that refers to it differs
  and the unit fails.
- A unit that is exact in the first image and differs in the second fails
  the build and names the image. That is the test of the inference, unit by
  unit. It stays an inference for every unit that has not passed it.
- A unit can be left out of the second link by name, in the configuration.
  Its range is then raw in the second image and reported as left out. This is
  for a difference under investigation, not a default.
- A second link exports no names. The resident source keeps reaching
  second-side addresses through `symbols.ld`.

The alternative is two ordinary images that name the same source files, with
every unit declared twice. It needs no new mechanism. It doubles the
declarations and nothing ties the two sets together. Not recommended.

Open inside this section: a second-side block that refers to another
second-side module needs that module's second-side addresses. Some of the 57
unexplained words are such references. The mechanism would be that a second
link sees the second links of its side in place of their first images. It is
not designed further until a unit needs it.

### Counting

- The report has one coverage record per image, with the same kinds.
- Published counts name their image. Resident and module figures are not
  added into one percentage.
- Functions that pass a second link are reported as such. They are the same
  source and do not count again as functions from C.

### Layout

- Module sources live in one folder per image under `ps1/src/`.
- Declarations stay in the one `build.toml` for the pilot. Splitting it per
  image is a later step if its size gets in the way.
- `types.fields` and the shared headers serve every image.

## What it costs

Tool changes, each with control cases, as separate pull requests:

1. Images with a chunk baseline. The resident build and its report stay
   byte for byte what they are. The archive parser moves where both
   `pac.py` and the build can use it.
2. Names across images.
3. Second links.
4. The pilot units.

`fndiff.py` and `mergeunits.py` learn the `image` key. No change to the
compiler pipeline, the object cache or the checks of a unit object.

Control cases to require, beyond the existing ones: a module image that
builds exact from a synthetic archive; a wrong chunk hash; a declared
address that differs from the table; a payload that is not a multiple of
four; two images that overlap in memory, both exact; a name used in two
images; a second link that is exact; a second link with one word changed,
which must fail in the second image only; a unit left out of a second link,
reported as such.

For the binding inside a second link: two units taken over from the first
image, one calling the other. The second link must be exact, with the call
bound to the second placement of the callee. The same fixture with the
callee's name also supplied at its first-side address must fail and name
the symbol, so that a binding to the first side cannot pass. A third case
does the same for a data symbol that one unit defines and the other reads.

## Pilot

Two small targets, in this order:

1. The module of slot `0x2a`: one content in 63 chunks, 28 functions, 6,052
   bytes, one link. It tests the chunk baseline and references to the
   resident image. Slot `0xb` is the alternative: 61 functions, 7,960 bytes.
2. The modules of slots `0x16` and `0x17`: 11 functions, 984 bytes, one
   content each, two links. It is the smallest test of one source and two
   links. Their comparison has 8 words that the distance does not explain;
   the pilot has to say what they are.

The function counts are the sweep's estimates.

## Decisions asked of the owner

1. A module's baseline is a chunk's content, pinned by hash, with its
   address checked against the loader's table.
2. One configuration with several images, each linked alone.
3. The second side as a second link of the same units, not as a second set
   of declarations.
4. The naming rule: unique across images, declared functions visible to
   every link, data symbols of other images through `symbols.ld` for now.
5. Counting per image, second links not counted again.
6. The two pilot targets and their order.
7. Whether module source under `ps1/src/` is published like the resident
   source. When the decision of 2026-10-05 was made, all source there was
   resident source.

## Not covered

- Chunks without code, slot `0xffff`, table 5 and the four stand-alone
  executables.
- Building an archive or a disc from images.
- The character pair whose two blocks differ in size by four bytes.
- Which modules call which. The design does not depend on it.
