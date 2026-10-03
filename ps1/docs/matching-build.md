# PS1 matching build layout

Parent: [delivery plan](../../PLAN.md), [requirements](../../docs/requirements.md).
Follow-up to the [matching pilot](matching-pilot.md). This document is the
normative contract for `ps1/tools/matchbuild.py` and `ps1/tools/baseline.py`.

## Purpose

Rebuild the complete resident executable `SLPS_004.15` from declared owners and
compare it with the pinned baseline. Recovered C is compiled and linked at its
original addresses. Everything not yet recovered is retained from the baseline
and counted separately. A byte-identical whole image therefore proves the C
ranges and the layout, and says nothing about understanding of retained bytes.

## Layout

| Path | Visibility | Content |
| --- | --- | --- |
| `ps1/tools/matchbuild.py` | public | Build, link, compare, report |
| `ps1/tools/fndiff.py` | public | Instruction diff of one unit against the baseline |
| `ps1/tools/test_matchbuild.py` | public | Controls for the build tool and the diff |
| `ps1/tools/structgen.py` | public | Struct-layout generator, layout check, field-file merge |
| `ps1/tools/test_structgen.py` | public | Controls for the struct generator |
| `ps1/tools/mergeunits.py` | public | Merge per-unit work directories |
| `ps1/tools/test_mergeunits.py` | public | Controls for the merge tool |
| `ps1/tools/baseline.py` | public | Disc/file baseline manifest and verification |
| `ps1/tools/verify_cc1_golden.py` | public | Compare a compiler with saved reference output |
| `ps1/tools/test_verify_cc1_golden.py` | public | Controls for the compiler checker |
| `ps1/local/src/build.toml` | private | Baseline pin, toolchain pins, unit declarations |
| `ps1/local/src/symbols.ld` | private | Addresses of symbols not defined by C units |
| `ps1/local/src/*.c` | private | Reconstructed source, one file per unit |
| `ps1/local/build/<tag>/` | generated | Objects, linker script, ELF, image, report |
| `ps1/local/baseline/manifest.json` | private | Per-file hashes derived from the disc image |
| `ps1/local/toolchain/` | private | Compiler binaries, SSH control socket |

The public tools contain no game data, addresses, names, hosts or credentials.
All of those come from the private configuration.

## Ownership model

The resident payload is the byte range `[load, load + size)` taken from the
PS-X EXE header. Every payload byte has exactly one owner.

- **c**: a unit, compiled from one C source file. A unit declares its functions
  as `name`, `address`, `size`. Its text range is the run of those functions in
  address order. Inside a unit, each function must end where the next begins.
- **rodata**: the read-only data of a unit, for example the jump table of a
  `switch`. A unit may declare one range for it with `rodata = { address, size }`.
  The range need not be near the text. The unit owns it and it is compared with
  the baseline like a function. Read-only data is the only data a unit can own.
- **raw**: retained baseline bytes. Raw ranges are never declared. They are
  computed as the complement of all text ranges and all rodata ranges, so a gap
  cannot silently count as recovered source.
- **asm**: reserved for reviewed assembly owners. Not implemented yet.

The 2,048-byte PS-X EXE header is retained from the baseline and reported as
raw.

Rejected before any compilation: overlapping units, ranges outside the payload,
addresses or sizes that are not multiples of four, zero sizes, a rodata range
that overlaps any text range (including its own unit's) or any other rodata
range, duplicate function or unit names, a gap between functions of one unit, and any name in
`symbols.ld` that is also a declared unit function. The last rule matters
because a linker-script assignment would silently override the compiled symbol.

## Configuration

`build.toml`, paths relative to the file itself, `~` expanded:

```toml
[baseline]
executable = "../audit/files/SLPS_004.15"
sha256 = "<hex>"

[toolchain]
cpp = "/opt/homebrew/opt/llvm/bin/clang"
maspsx = "~/Projects/references/maspsx/maspsx.py"
maspsx_commit = "<git commit>"
aspsx_version = "2.34"
binutils_prefix = "mipsel-linux-gnu-"

[toolchain.cc1]            # default compiler
kind = "local"
path = "../toolchain/gcc-2.6.3-psx/cc1"
sha256 = "<hex of the cc1 binary>"
no_float = true            # optional: reject floating-point source

[toolchain.cc1_reference]  # optional, selected with --reference
kind = "remote"
sha256 = "<hex of the cc1 binary>"
host = "user@host"
socket = "../toolchain/ssh.sock"
scp_dir = "Tools/example"              # remote path as scp sees it
exec_dir = "/mnt/c/Users/x/Tools/example"  # same directory as the compiler sees it
exec_prefix = "wsl.exe -d Ubuntu --"   # command prefix on the remote host
binary = "cc1-psx-26"

[selftest]
unit = "metrics"
find = "<exact source text occurring once>"
replace = "<text that changes generated code>"

[[unit]]
name = "metrics"
source = "metrics.c"
flags = ["-O2", "-G0"]
functions = [
  { name = "example", address = 0x80000000, size = 4 },
]
# Optional. The one range that holds the unit's read-only data, for example a
# jump table. Address and size are multiples of four.
rodata = { address = 0x80001000, size = 24 }
```

`symbols.ld` sits next to `build.toml`, holds lines of the form
`name = 0x80000000;` and is included by the generated linker script.

## Shared types

Hand-written structs with padding arrays do not merge when several people add
fields. A field table does: one line per field, sorted by offset. The header is
generated from it and never edited.

Field file, one declaration per line, `#` starts a comment:

```
type OpaqueFn size=4 align=4      # defined elsewhere in C
struct Small size=6
0x00 s16 origin
0x04 u8  extent
struct Big size=0x40
0x01 u8      flag
0x20 Big*    next
0x28 Small   inline_part
0x30 u8      block[8]
```

- `struct NAME size=N` starts a struct. Following `OFFSET TYPE NAME` lines are
  its fields until the next `struct` or `type` line. Numbers are hex or decimal.
  Field order in the file is free.
- Types: `u8 s8 u16 s16 u32 s32 int`, pointers written `T*` (4 bytes), a struct
  declared earlier, and `type` declarations with explicit size and align.
  `NAME[N]` makes an array.
- The header is C89. Gaps become `u8 unknown_<offset>[n];` and the tail is padded
  to the declared size. The scalar typedefs are emitted once at the top.
- Errors, all reported with line numbers: overlapping fields, an offset that is
  not a multiple of the type alignment, a field past the declared size, a struct
  size that is not a multiple of its alignment, duplicate field, struct or type
  names, unknown types, a struct used by value before its declaration, and
  malformed lines.

Commands of `ps1/tools/structgen.py`:

- `generate FIELDS -o HEADER` writes the header.
- `check FIELDS --cpp CLANG` generates the header and compiles a file of
  `_Static_assert` offset and size checks with
  `CLANG -target mipsel-none-elf -nostdinc -fsyntax-only`. This confirms
  independently that a real compiler lays the struct out as declared.
- `merge A B [C ...] -o OUT` writes the union. Structs and fields present in
  several inputs must agree exactly. Any disagreement, including two names at
  one offset, is listed with both sources and nothing is written. A name used
  as a `type` in one input and a `struct` in another is also a conflict. The
  merged text is parsed and validated again before it is written, so an
  invalid result is an error, not a file. Output is deterministic: structs in first-seen order (a struct used by value moves after
  the struct it embeds), fields sorted by offset.

Build integration. The optional table

```toml
[types]
fields = "types.fields"
header = "types.gen.h"
```

(`fields` relative to `build.toml`, `header` a bare file name) makes
`matchbuild.py` generate `<build>/gen/<header>`, run the layout check with the
configured `cpp`, and pass `-I <build>/gen` when preprocessing every unit. A
generator or check error fails the build with exit 1. The fields file hash is
recorded as `inputs.types_fields`. A file named like the header in a source
directory is a configuration error, because quoted includes resolve next to the
including file first and it would shadow the generated one. Without `[types]`
nothing changes.

## Compilers

Both compiler tables take `kind = "local"` or `kind = "remote"`. The remote kind
runs over an already authenticated SSH control master. The tool never
authenticates and no credential is stored anywhere.

The pinned reference is the Linux GCC 2.6.3 binary named in the
[pilot report](matching-pilot.md). The default compiler is a native macOS build
of the same source, made by the private script
`ps1/local/toolchain/build-gcc-2.6.3-psx-macos.sh`. It emitted identical
assembly on 121 of 131 saved reference outputs. All ten differences are
floating-point constants, which that build gets wrong because of the 64-bit
host. `no_float = true` makes the tool reject any unit whose preprocessed text
contains `float`, `double` or a floating literal, so that defect cannot reach a
build. `ps1/tools/verify_cc1_golden.py` repeats the comparison.

Which compiler produced a build does not weaken an exact result: identical
bytes are identical bytes. `--reference` exists to show that the pinned binary
reproduces the same image, and for units the native build must not compile.

## Command line

```sh
.venv/bin/python ps1/tools/matchbuild.py [--config PATH] [--tag NAME] [--reference] [--cache DIR | --no-cache]
.venv/bin/python ps1/tools/test_matchbuild.py [--config PATH]
.venv/bin/python ps1/tools/fndiff.py [--config PATH] [--tag NAME] [--all] UNIT [FUNCTION]
```

`fndiff.py` is a diagnostic for a unit that does not match yet. It links the
unit object left by the last build alone at the unit's start address, with its
read-only data at the declared rodata address so that jump-table addresses in
the code come out right, and prints
an aligned instruction diff against the baseline range. It works when sizes are
wrong and the whole-image link was never reached. It decides nothing:
`matchbuild.py` remains the only authority on whether a build is exact.

Exit status: 0 all checks passed, 1 failed check or build step, 2 invalid
configuration, 3 unusable environment such as a missing SSH master.

## Pipeline

Each run deletes and recreates `build/<tag>/`. The only thing reused between
runs is the unit object cache, described after the steps.

1. Read and validate the configuration. Check the baseline hash, the `PS-X EXE`
   magic, and that the file size equals header size plus payload size. Extract
   the payload to the build directory.
2. Check toolchain pins. The `cc1` SHA-256 is hashed on the host where it
   runs. For maspsx, the pinned commit is exported from the checkout's object
   database into the build directory and that copy is what runs. The
   checkout's working tree, its HEAD and any stale bytecode therefore cannot
   change the code that executes. A pinned commit missing from the checkout
   fails the build. The report records whether the checkout was dirty. Record
   tool versions.
3. Per unit: preprocess with
   `clang -E -P -x c -target mipsel-none-elf -nostdinc`, compile with
   `cc1 -quiet <flags>`, convert with `maspsx --aspsx-version=<v>`, assemble
   with `as -EL -G0 -march=r3000 -mabi=32 -no-pad-sections`. Every subprocess
   exit status is enforced. Without `-no-pad-sections` the assembler pads each
   section to 16 bytes and a unit would spill past its declared range.
4. Check the unit object's allocated sections. `.text` must equal the declared
   text range. Read-only data (`.rodata` and `.rdata`, added together) must
   equal the declared `rodata` size exactly; a unit that declares none must have
   none, and a unit that declares a range must have data for it. Any other
   non-empty allocated section (`.data`, `.bss`, `.sdata`) and any common symbol
   is rejected, because that data would have no owner. This compiler puts a
   jump table in `.rodata`.
5. Generate `raw.s` with one section per raw range using `.incbin` on the
   extracted payload, and `link.ld` placing every range at its explicit address
   in address order. A unit's read-only data is its own output section at the
   declared rodata address, placed the same way as text. Raw ranges exclude it. Link with undefined symbols as errors.
6. Convert the ELF to a flat image. Rebuilt executable is baseline header plus
   image.

### Object cache

What is cached: per unit, the output of compile, maspsx and assemble. An entry
holds the unit object, the compiler output `.s`, the converted `.gnu.s` and a
JSON file with the object's SHA-256 and the key inputs in readable form. A hit
skips those three steps for that unit and copies the three files into the build
directory under their usual names.

The declared rodata range is not in the key. It does not change the object: the
assembler leaves the table's address to the linker, so a different range gives
the same bytes. The object checks that use the range (step 4) run on hits too.

The key is the SHA-256 of a canonical encoding of:

- a cache format version string;
- the preprocessed text (as its hash in the JSON);
- the unit name, because the compiler may record the input file name;
- the unit's cc1 flags;
- the verified cc1 SHA-256 and which table is in use (`cc1` or
  `cc1_reference`);
- the full pinned maspsx commit hash and the script path inside it;
- the aspsx version;
- the assembler flags;
- the assembler's first banner line and the SHA-256 of the executable file
  that `<prefix>as` resolves to on `PATH`, after resolving symlinks. The banner
  is self-reported and can match across different builds, so the content hash
  is what identifies the tool. The build runs that resolved file;
- the Python interpreter that runs maspsx: its full version string and the
  SHA-256 of the resolved executable. Its standard library is not hashed.

The compiler is covered by its pinned SHA-256 and maspsx by the pinned commit
that is exported and run. The preprocessor runs before the key, so its effect
is already in the preprocessed text. The linker and objcopy run after the
cache and do not touch cached objects. The report records the path and SHA-256
of the assembler, linker, objcopy and interpreter.

Preprocessing and the float guard run before the lookup, so the key covers the
source and every header it includes, and a `no_float` compiler still rejects
float tokens on a hit. The pin checks also run first. A wrong cc1 hash fails
before any lookup.

The JSON records the key and the SHA-256 of each of the three files. A reader
reads the JSON once, copies the three files into the build directory, and
hashes each copy against what it read. A wrong key, a missing file, a read
error or any mismatch is a miss and removes the partial copies. An entry that
disappears or changes during the read is therefore a miss, never an error.
After a miss the unit is rebuilt and published.

Entries are written in a temporary sibling directory and then renamed.
Publication re-checks the existing entry first. A valid entry, including one
that another run published after this run missed, is kept and never replaced.
Only an invalid entry is moved aside and replaced; the moved copy is checked
once more and put back if it turns out to be valid. A run that loses the
rename race to a concurrent run carries on. The format version is part of the
key, so entries written under an older key layout are never hit.

What always runs, on hits and misses alike: the unit object checks, raw and
linker script generation, the link, the image conversion, every function and
image comparison, the executable hash check and every comparator control. The
cache cannot turn a failing build into a passing one. A stale object cannot be
used because any change to the source, a header, the flags or a pinned tool
changes the key.

Location: `<config dir>/../build/.objcache/`, shared by all tags. `--cache DIR`
overrides it and `--no-cache` neither reads nor writes it. The build directory
is deleted on every run; the cache directory is not. Delete the cache directory
to start cold. The remote compiler path uses the same cache with its own keys.

## Checks

All fail closed with a non-zero exit status.

- Each declared function exists in the ELF with the declared address and size,
  and its bytes equal the baseline range. Report the first differing offset and
  the count of equal instruction words.
- Each unit's `.text` size equals its declared range.
- Each unit's read-only data size equals its declared `rodata` size, and the
  rodata range of the image equals the baseline range. A difference fails the
  build with a message naming the unit's rodata and the first differing offset.
- Image size and SHA-256 equal the baseline payload. Rebuilt executable SHA-256
  equals the baseline executable.
- Comparator controls on every successful build: flipping one byte inside each
  C function of the image must make exactly that function fail, and flipping
  one raw byte must make the image check fail. A control that does not trip
  fails the build. When units own the whole payload there is no raw byte to
  flip: the raw control is recorded as not applicable and the function controls
  stay mandatory. Flipping one byte inside each rodata range must make exactly
  that unit's rodata fail and no function fail.

`test_matchbuild.py` runs the tool against temporary copies of the private
configuration and requires failure for: the `[selftest]` source mutation, a
wrong declared size, two overlapping units, a unit with read-only data and no
`rodata`, a rodata range of the wrong size, address or alignment, a rodata range
that overlaps text or another rodata range or lies outside the payload, a symbol removed from `symbols.ld`,
a unit function duplicated in `symbols.ld`, a wrong `cc1` hash pin, a wrong
baseline hash, a maspsx commit that does not exist, and floating-point source
when the default compiler is marked `no_float`. It requires success for: the
unmodified build; a maspsx checkout whose script is edited to abort, which
passes only because the pinned commit runs instead; and a synthetic fixture
whose whole payload is one C function, and a synthetic fixture whose function
has a jump table that the baseline places away from the text, with raw filler
between them.

`test_verify_cc1_golden.py` covers the compiler checker with a stand-in
compiler: a selected case with a missing input or missing expected output must
fail, as must a differing output and an empty selection.

## Report

`build/<tag>/report.json` and a short text summary:

- inputs with SHA-256: configuration, symbols, baseline, each source, and each
  unit's preprocessed text, which covers included headers;
- tools: which compiler was used, paths, versions, pinned hashes and commits;
- per unit and function: addresses, sizes, exact or not, hashes;
- per unit rodata: address, size, exact or not, first differing offset, hashes;
- per unit cache state (`hit`, `miss` or `off`) and key, with totals in the
  text summary, for example `cache: 118 hits, 2 misses`;
- coverage in bytes by owner kind (`c_bytes` for code, `rodata_bytes`, raw),
  plus the function count owned by C. Retained raw bytes exclude rodata. The
  summary line shows the rodata bytes;
- overall `exact` flag, image and executable hashes, control results.

## Baseline manifest

`baseline.py pin` reads the user's disc image and the audit inventory and writes
the private manifest: image size and SHA-256, then per file its path, LBA, size
and SHA-256, plus PS-X EXE header fields where the magic is present. File
content is the first `size` bytes of the concatenated 2,048-byte user-data
fields starting at the file's LBA. `baseline.py verify` recomputes and compares,
against the image, against a directory of extracted files, or both.

`baseline.py extract` copies files out of the image and refuses any whose hash
differs from the manifest. A manifest is rejected as a whole if any path is
absolute, has an empty, `.` or `..` component, contains a backslash, or
repeats. Output stays inside `--out`: a symbolic link on the way to a
destination is refused, and each file is written to a new temporary file that
then replaces the destination name, so an existing symbolic or hard link there
is replaced and never written through. A destination that is the image or the
manifest is refused.

The published root pins remain the disc and executable hashes in the
[pilot report](matching-pilot.md). The manifest is a derived index.

## Merging unit work directories

Several units can be reconstructed at once, each in its own sibling copy of
the configuration directory. See the [matching guide](matching-guide.md).

```sh
.venv/bin/python ps1/tools/mergeunits.py --base DIR --out NEWDIR UNITDIR [UNITDIR ...]
```

The merge is mechanical and refuses to guess. `types.fields` goes through the
field-file merge. A change to the declarations counts, not only new fields: a
new `type` or a struct with no fields is kept. A unit that drops a base type,
struct or field is a conflict. `symbols.ld` keeps the base text and appends each unit's new
statements; the same name with two values is a conflict, and two names for one
value is a warning. A symbol that a merged unit now defines is dropped. New
`[[unit]]` tables are appended, including a `rodata` key, which is written back as
`rodata = { address = 0x..., size = N }`; a changed base table or a repeated unit name
is a conflict. New files are copied. A base file that a unit changed is a
conflict unless named with `--take`. A path that is a file in one place and a
directory in another (for example a new file `support` in one unit and
`support/helper.h` in another, or in the base) is a conflict.

On any conflict nothing is written. Otherwise the result is built in a hidden
staging directory next to `--out` and published with a single rename after
everything was written. If anything fails, the staging directory is removed
and `--out` does not exist. The merged directory then has to pass
`matchbuild.py` like any other.

## Limits

No data ownership other than one read-only data range per unit, no assembly
owners, no overlay images, no
incremental builds. Compiler provenance is unchanged from the pilot: a
compatible toolchain, not a uniquely identified original.
