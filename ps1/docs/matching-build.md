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
| `ps1/src/build.toml` | public | Baseline pin, toolchain pins, unit declarations |
| `ps1/src/symbols.ld` | public | Addresses of symbols not defined by C units |
| `ps1/src/*.c` | public | Reconstructed source, one file per unit |
| `ps1/src/sdk/include/` | private | SDK headers of another project, under its license |
| `ps1/build/<tag>/` | generated | Objects, linker script, ELF, image, report |
| `ps1/local/baseline/manifest.json` | private | Per-file hashes derived from the disc image |
| `ps1/local/toolchain/` | private | Compiler binaries, SSH control socket |

The public tools contain no game data, addresses, names, hosts or credentials.
Addresses and names come from the configuration in `ps1/src/`; binaries and
hosts come from local inputs that are not in the repository.

## Ownership model

The resident payload is the byte range `[load, load + size)` taken from the
PS-X EXE header. Every payload byte has exactly one owner.

- **c**: a unit, compiled from one C source file. A unit declares its functions
  as `name`, `address`, `size`. Its text range is the run of those functions in
  address order. Inside a unit, each function must end where the next begins.
- **rodata**: the read-only data of a unit, for example the jump table of a
  `switch`. A unit may declare one range for it with `rodata = { address, size }`.
  The range need not be near the text. The unit owns it and it is compared with
  the baseline like a function.
  The declared size is a whole number of words. An object whose read-only data
  ends one to three bytes short of a word is accepted: the link fills the range
  to its declared size with zero bytes, the unit owns that padding, and it is
  compared with the baseline like the rest of the range.
- **data**: the initialised writable data of a unit, its `.data` section. A
  unit may declare one range for it with `data = { address, size }`. The rules
  are those of rodata: the unit owns the range, it is compared with the
  baseline, the declared size is a whole number of words, and an object that
  ends one to three bytes short is filled with zero bytes that the unit owns.
- **bss**: the uninitialised data of a unit, its `.bss` and `.sbss` sections. A
  unit may declare one range for it with `bss = { address, size }`. The range
  lies outside the payload, so there are no bytes to compare and it adds
  nothing to coverage. Declaring it fixes where the unit's uninitialised
  symbols live, which the code that refers to them depends on.
- **asm**: a unit written in assembly, `kind = "asm"`. It is for code whose
  original source was assembly, such as the system-call stubs of the SDK. It is
  assembled directly and checked like any unit, and its functions are reported
  as assembly, never as C. See "Assembly units".
- **raw**: retained baseline bytes. Raw ranges are never declared. They are
  computed as the complement of all text, rodata and data ranges, so a gap
  cannot silently count as recovered source.

The 2,048-byte PS-X EXE header is retained from the baseline and reported as
raw.

Rejected before any compilation: overlapping units, ranges outside the payload,
addresses or sizes that are not multiples of four, zero sizes, a rodata or data
range that overlaps any text range (including its own unit's) or any other
rodata or data range, a bss range that touches the payload or overlaps another
bss range, an unknown `kind`, an assembly unit with `flags`,
duplicate function or unit names, a gap between functions of one unit, and any name in
`symbols.ld` that is also a declared unit function. The last rule matters
because a linker-script assignment would silently override the compiled symbol.

### Symbols a unit defines

A unit owns every symbol its object defines with global or weak binding: its
functions, and the variables and tables in its read-only data, data and bss.
Such a symbol lives at the unit's declared range of that kind plus the
symbol's offset in the object. Where one kind is made of two object sections
(`.rodata` then `.rdata`, `.bss` then `.sbss`), the second follows the first
directly, as the link places them.

`symbols.ld` must not assign any of those names. The assignment would win at
the link: the symbol would become an absolute address and the declared range
would no longer be where the code's variable lives, while the build could still
compare equal. The rule for declared functions above is the part of this that
can be checked before compilation. The rest needs the objects, so it is checked
after every unit object passed its own checks and before the link, and a
collision fails the build with exit status 1 and a message naming the symbol,
the unit and the object section.

A different name at the same address stays allowed. `symbols.ld` may give an
address that a unit owns a second name, as long as the name itself is not one
the unit defines.

Local symbols (`static`) are outside the rule: a linker-script assignment
cannot rebind them.

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
expand_div = true          # optional, default false: run maspsx with --expand-div
include_dirs = ["include"] # optional: extra include directories for every unit

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
# Optional. The unit's initialised data (.data) and where its uninitialised
# data (.bss, .sbss) lives. Same form and alignment rules as rodata.
data = { address = 0x80002000, size = 16 }
bss = { address = 0x80300000, size = 8 }

[[unit]]
name = "stubs"
kind = "asm"               # optional, "c" by default
source = "stubs.s"
functions = [
  { name = "stub", address = 0x80000004, size = 16 },
]
```

`symbols.ld` sits next to `build.toml`, holds lines of the form
`name = 0x80000000;` and is included by the generated linker script.

`expand_div` selects how a division is assembled. The original assembler
expands a division into the divide instruction plus checks for a zero divisor
and for overflow. With `expand_div = true` maspsx writes that expansion; without
it the GNU assembler emits the bare instruction and a function that divides
cannot match. The option applies to every unit and is part of the cache key.

`include_dirs` lists directories, relative to `build.toml`, that are passed to
the preprocessor with `-I` for every unit, after the generated types directory
and in the listed order. A listed directory that does not exist, or one listed
twice, is a configuration error. So is a file in one of them named like the
generated types header. Their content needs no separate hash: the
preprocessed text of each unit already covers every header it includes.

## Assembly units

A unit with `kind = "asm"` names an assembly file in GNU assembler syntax. It
is not preprocessed, compiled or passed through maspsx: it is assembled with
the same assembler and flags as every other unit. It has no `flags` key. Each
declared function must be a symbol of the object with its size set, as for a C
unit, so the source marks it with `.type name, @function` and `.size`.
Everything after assembly is the same: the object checks, the link at the
declared address, the comparison with the baseline and the controls. An
assembly unit may declare `rodata`, `data` and `bss` like a C unit.

Assembly units are not cached. They take one assembler run.

Accounting keeps the two kinds apart. The functions and bytes of assembly
units are reported as `asm_functions` and `asm_bytes` and are not part of
`c_functions` and `c_bytes`. The summary names them separately.

A C unit must not hide assembly. After string and character literals are
blanked, a preprocessed C unit that contains the token `asm`, `__asm` or
`__asm__` fails the build with a message naming the unit. Code that was
assembly goes into an assembly unit.

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
build. String and character literals are blanked before that search: their text
is data, and a version string such as `"1.71"` is not a floating constant.
`ps1/tools/verify_cc1_golden.py` repeats the comparison.

Which compiler produced a build does not weaken an exact result: identical
bytes are identical bytes. `--reference` exists to show that the pinned binary
reproduces the same image, and for units the native build must not compile.

## Command line

```sh
.venv/bin/python ps1/tools/matchbuild.py [--config PATH] [--tag NAME] [--reference] [--cache DIR | --no-cache]
.venv/bin/python ps1/tools/test_matchbuild.py [--config PATH] [--only TEXT] [--jobs N]
.venv/bin/python ps1/tools/fndiff.py [--config PATH] [--tag NAME] [--all] UNIT [FUNCTION]
```

`test_matchbuild.py --only TEXT` runs the cases whose name contains the text
and says so in its last lines, with the number of cases that ran. A text that
matches no case is an error: nothing runs and the exit status is 2. It is for
working on one area; only a run without it is the full control set.

`test_matchbuild.py --jobs N` runs up to N cases at the same time; the
default is the number of processors, at most 8, and `--jobs 1` runs them one
after another. Every case builds its own copy of the configuration under its
own build tag with its own, empty object cache, all named after the case, so
running cases side by side changes nothing in what a case builds or checks.
One thing is shared: the cases of a synthetic fixture (read-only data, data,
bss, division, assembly, sibling units) take their baseline from throwaway
seed builds under names fixed per fixture. Those seed builds run one at a
time per fixture; a control case checks that for every fixture class.
Results are printed in the fixed order of the cases. A case that raises an
error is reported as a failed case and the others still run. The same holds
for a case whose setup stops with `SystemExit` ("test setup: ..."), for one
case at a time as well as side by side. An interrupt still ends the run. Two runs of the
whole suite on one checkout at the same time are still not supported: they
would use the same names.

`fndiff.py` is a diagnostic for a unit that does not match yet. It links the
unit object left by the last build alone at the unit's start address, with its
read-only data, data and bss at their declared addresses so that the addresses
in the code come out right, and prints
an aligned instruction diff against the baseline range. It works when sizes are
wrong and the whole-image link was never reached. It decides nothing:
`matchbuild.py` remains the only authority on whether a build is exact.

Symbols of the other units are given to that link as addresses. Their declared
functions come from the configuration, as before. Every other global or weak
symbol a sibling unit defines is read from the sibling's object in the same
build directory and placed by the rule in "Symbols a unit defines": declared
range of its kind plus offset. This needs no linked image and no entry in
`symbols.ld`. A sibling without an object there, or a symbol whose kind the
sibling does not declare, contributes nothing, and a reference to it fails the
link with the linker's message.

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
   `clang -E -P -x c -target mipsel-none-elf -nostdinc` plus one `-I` per
   include directory, compile with
   `cc1 -quiet <flags>`, convert with `maspsx [--expand-div] --aspsx-version=<v>`, assemble
   with `as -EL -G0 -march=r3000 -mabi=32 -no-pad-sections`. Every subprocess
   exit status is enforced. Without `-no-pad-sections` the assembler pads each
   section to 16 bytes and a unit would spill past its declared range.
4. Check the unit object's allocated sections. `.text` must equal the declared
   text range. Read-only data (`.rodata` and `.rdata`, added together), rounded
   up to a multiple of four, must equal the declared `rodata` size; a unit that
   declares none must have none, and a unit that declares a range must have data
   for it. The same holds for `.data` against `data` and for `.bss` plus `.sbss`
   against `bss`. Any other non-empty allocated section (for example `.sdata`)
   and any common symbol is rejected, because that data would have no owner.
   This compiler puts a jump table in `.rodata`. maspsx turns the compiler's
   `.comm` and `.lcomm` into definitions in `.bss`, as the original assembler
   did, so an uninitialised global of a unit is part of its bss range.
5. Generate `raw.s` with one section per raw range using `.incbin` on the
   extracted payload, and `link.ld` placing every range at its explicit address
   in address order. A unit's read-only data is its own output section at the
   declared rodata address, placed the same way as text and filled with zero
   bytes up to the declared size. Its `.data` is placed the same way at the
   declared data address. Its bss is an output section at the declared bss
   address that occupies no bytes of the image. Raw ranges exclude it. Link with undefined symbols as errors.
6. Convert the ELF to a flat image. Rebuilt executable is baseline header plus
   image.

### Object cache

What is cached: per unit, the output of compile, maspsx and assemble. An entry
holds the unit object, the compiler output `.s`, the converted `.gnu.s` and a
JSON file with the object's SHA-256 and the key inputs in readable form. A hit
skips those three steps for that unit and copies the three files into the build
directory under their usual names.

The declared rodata, data and bss ranges are not in the key. It does not change the object: the
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
- the aspsx version and the other maspsx options (`--expand-div` or none);
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
- Each unit's read-only data size, rounded up to a multiple of four, equals its
  declared `rodata` size, and the rodata range of the image, padding included,
  equals the baseline range. A difference fails the
  build with a message naming the unit's rodata and the first differing offset.
- The same two checks for `.data` against the declared `data` range, with
  messages that say `data`.
- Each unit's bss size, rounded up to a multiple of four, equals its declared
  `bss` size. In the linked ELF the unit's bss section starts at the declared
  address.
- No name assigned in `symbols.ld` is a global or weak symbol defined by a
  unit object. Checked before the link.
- In the linked ELF, every global or weak symbol that a unit object defines in
  its text, read-only data, data or bss is bound inside that unit's output
  section of that kind, at the declared address of the kind plus the symbol's
  offset in the object. A symbol that is absolute, missing, in another section
  or at another address fails the build with a message naming the unit, the
  symbol and both locations. This repeats the rule above on the result of the
  link, so the rule does not depend on how an override was written.
- Image size and SHA-256 equal the baseline payload. Rebuilt executable SHA-256
  equals the baseline executable.
- Comparator controls on every successful build: flipping one byte inside each
  C function of the image must make exactly that function fail, and flipping
  one raw byte must make the image check fail. A control that does not trip
  fails the build. When units own the whole payload there is no raw byte to
  flip: the raw control is recorded as not applicable and the function controls
  stay mandatory. Flipping one byte inside each rodata range must make exactly
  that unit's rodata fail and no function fail. Flipping one byte inside each
  data range must make exactly that unit's data fail, and nothing else.

For data, bss and assembly units `test_matchbuild.py` requires failure for: a
unit with `.data` and no `data`; a `data` range of the wrong size, alignment or
address, one whose baseline byte differs, one that overlaps a text, rodata or
other data range or lies outside the payload, and one declared by a unit that
has no `.data`; a unit with bss and no `bss`; a `bss` range of the wrong size,
one that touches the payload, one that overlaps another bss range, one
declared by a unit that has none, and one at the wrong address, which shows as
differing code; an unknown `kind`; an assembly unit with `flags`; a C unit
that contains inline assembly; and an assembly unit with one changed
instruction. It requires success, with the report checked, for: a unit with
declared data, with coverage, the data record and a tripped data control; data
that ends short of a word; a unit with declared bss that its code refers to;
and an assembly unit next to a C unit, whose functions count as assembly and
not as C and whose control trips. `test_mergeunits.py` requires that `kind`,
`data` and `bss` of a new unit table survive a merge unchanged.

For symbol ownership it requires failure for: a bss range declared at another
address while `symbols.ld` assigns the unit's two bss variables their old
addresses, which built and passed before the rule existed; and a data variable
of a unit assigned in `symbols.ld` at its correct address. It requires success
for a second name in `symbols.ld` at the address of a unit's bss variable. A
unit case assembles an authored object with a symbol in every owned kind,
including the second section of read-only data and of bss, and requires the
addresses computed for them. It then links the object twice with a
hand-written script, once clean and once with an assignment that overrides
its bss symbol, and requires the check of the linked ELF to accept the first,
which confirms the computed addresses against the linker, and to reject the
second.

For `fndiff.py` it uses a fixture of two units, where one owns a data variable
and a bss variable and the other's function reads the first and updates the
second. It requires the whole build to pass and the diff of the using unit to
report identical code, and, with that unit's size declared wrong so that the
build stops before the link, the diff to still link and report different code.

`test_matchbuild.py` runs the tool against temporary copies of the
configuration and requires failure for: the `[selftest]` source mutation, a
wrong declared size, two overlapping units, a unit with read-only data and no
`rodata`, a rodata range of the wrong size, address or alignment, a rodata range
that overlaps text or another rodata range or lies outside the payload, a symbol removed from `symbols.ld`,
a unit function duplicated in `symbols.ld`, a wrong `cc1` hash pin, a wrong
baseline hash, a maspsx commit that does not exist, floating-point source
when the default compiler is marked `no_float`, a padded rodata range whose
padding byte differs from the baseline or whose declared size is a word off, a
dividing function built without `expand_div`, an `expand_div` that is not a
boolean, a header that is only reachable through an include directory that is
not listed, a listed include directory that is missing or that shadows the
generated types header, and an included header that changes the generated
code. It requires success for: the
unmodified build; float-looking text inside string and character literals; a
header reached through a listed include directory; read-only data that ends
two bytes short of a word; a dividing function with `expand_div`; a maspsx checkout whose script is edited to abort, which
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
- per unit `kind` (`c` or `asm`);
- per unit rodata: address, size, exact or not, first differing offset, hashes;
- per unit data: the same record as rodata; per unit bss: address and size;
- `inputs.include_dirs` and `inputs.maspsx_flags`: the include directories and
  the maspsx options the build used;
- per unit cache state (`hit`, `miss` or `off`) and key, with totals in the
  text summary, for example `cache: 118 hits, 2 misses`;
- coverage in bytes by owner kind (`c_bytes` and `asm_bytes` for code,
  `rodata_bytes`, `data_bytes`, raw), plus the function counts `c_functions`
  and `asm_functions`. Retained raw bytes exclude rodata and data. `bss_bytes`
  is recorded next to coverage and is not part of it. The summary line shows
  C, assembly, rodata, data and raw;
- overall `exact` flag, image and executable hashes, control results.

## Module images

The build knows two kinds of image. The **resident image** is the payload of
the baseline executable; everything above describes it. A **module image** is
one chunk of an archive, which the resident loader copies to a fixed address.
The [overlay map](overlays.md) describes the archives and the loader's
tables. Module images are declared. A configuration that declares none
builds exactly as before, and its report and summary are unchanged.

### Declaration

```toml
[overlays]
table_pointers = 0x80001000   # address, inside the resident payload, of the block of table addresses

[[image]]
name = "example"
archive = "../extract/EXAMPLE.PAC"
slot = 0x2a
sha256 = "<hex of the chunk's bytes>"
address = 0x80200000

[[unit]]
name = "unit"
image = "example"             # the resident image when absent
source = "example/unit.c"
functions = [ { name = "function", address = 0x80200000, size = 4 } ]
```

- `name` follows the rule for a build tag. `resident` is reserved: it names
  the resident image on the command line and may not be declared.
- `archive` is a path relative to `build.toml`. The file has the layout that
  `pac.py` documents and is read with the same reader.
- The image is the one chunk of that file with table number 0 and the given
  `slot`. Its payload is the chunk's bytes and covers
  `[address, address + size of the chunk)`. `sha256` pins those bytes.
- `address` must equal the entry of the loader's table 0 for that slot. The
  tables are read from the baseline executable through the block at
  `table_pointers`, as `pac.py loadmap` reads them.

Every payload byte of a module image has exactly one owner, with the kinds of
the ownership model: c, asm, rodata, data and raw, and bss outside the
payload. There is no header. A payload whose size is not a multiple of four
keeps its last one to three bytes raw.

Rejected before any compilation, with exit status 2:

- an `[[image]]` when `[overlays]` or its `table_pointers` is missing; a
  `table_pointers` that is not an address of a whole block inside the
  resident payload, or a block that the table reader rejects;
- an image name that is not a valid tag, is repeated or is `resident`;
- an archive that is missing or that the archive reader rejects;
- no chunk, or more than one chunk, with table number 0 and the slot;
- a chunk whose SHA-256 differs from `sha256`;
- an `address` that is not a multiple of four, a slot beyond table 0, or an
  `address` that differs from the table's entry for the slot;
- a unit whose `image` names no declared image.

The range rules of the ownership model apply to each image by itself. A
unit's text, rodata and data ranges must lie inside the payload of its own
image. Overlap is judged among the units of one image only: units of
different images may cover the same addresses, as the modules do in memory.
A bss range must not touch the payload of its own image and must not overlap
another bss range of the same image. Unit and function names stay unique
across all images, and no name in `symbols.ld` may be a declared function of
any image.

### Build

Units are preprocessed, compiled, assembled and checked as objects exactly as
before, whatever their image. Then each image is linked alone: the resident
image first, in `build/<tag>/` as before, then each module image in the order
of its declaration, in `build/<tag>/image-<name>/` with its own `payload.bin`,
`raw.s`, `link.ld`, `image.elf` and `image.bin`. The unit objects stay in
`build/<tag>/`.

- The raw ranges of a module image are the complement of its units inside
  its payload.
- Its link takes the objects of its own units and its raw ranges, includes
  `symbols.ld`, and treats undefined symbols as errors. It does not see the
  units of any other image. The resident link does not see module units.
  Names across images are a later change; until then a module unit can only
  refer to itself, to the other units of its image and to `symbols.ld`.
- Every check of the resident image runs for a module image against its own
  payload: the function, text, rodata, data, bss and symbol checks, image
  size and SHA-256 against the chunk, and the comparator controls. No
  executable is rebuilt for a module image, so there is no executable hash.
- A failure in a module image names it: the message starts with
  `image '<name>': `.
- The run is exact only if every image it built is exact.

`--image NAME` builds one image: `resident` or a declared name. Any other
name is a configuration error with exit status 2. Only the units of that
image are compiled, and only that image is linked and checked. The baselines
of all declared images are still validated.

### Report

- `images` is a list with one record per module image that was built, in
  declaration order: `name`, `archive`, `slot`, `address`, `size`,
  `baseline_sha256`, `image_sha256`, `exact`, `carriers`, `units`,
  `coverage`, `bss_bytes` and `controls`. `units`, `bss_bytes` and
  `controls` have the form of the resident keys of the same name.
  `coverage` has the resident's keys without `raw_header_bytes`.
- `carriers` is the number of chunks with table number 0, that slot and the
  same bytes in the files of the archive's directory that the archive reader
  accepts. It counts the declared chunk, so it is at least 1.
- The keys `units`, `coverage`, `bss_bytes`, `image_sha256`,
  `executable_sha256`, `baseline_executable_sha256` and `controls` at the top
  of the report describe the resident image and hold only its units. A unit
  of a module image appears in its image's record and nowhere else.
- `inputs.sources`, `inputs.preprocessed` and `cache.units` cover every unit
  that was compiled. `inputs.images` maps each declared image to the SHA-256
  of its archive file and of its chunk.
- `selected_image` is present when `--image` was given. With a module image
  selected the resident keys listed above are absent; with `resident`
  selected `images` is absent.
- Without any declared image the report has no `images`, `inputs.images` or
  `selected_image` key.

The summary gains, per module image, after the resident's lines: one line
per function and per rodata or data range as for the resident, then lines
that start with `image <name>` for the count of exact functions, the
coverage, the image and baseline hashes, the carriers and the comparator
controls. Without any declared image the summary is unchanged.

### Controls

`test_matchbuild.py` uses synthetic fixtures only: a resident executable
whose payload holds the block of table addresses and the tables, and
archives written by the test. It requires failure, with a message that names
the fault, for: an image without `[overlays]`; a `table_pointers` outside the
payload; an image named `resident`; a repeated image name; a missing archive;
an archive with one changed byte in an entry's first-word copy; no chunk with
the slot; two chunks with the slot; a wrong chunk hash; an address that
differs from the table; an address that is not a multiple of four; a slot
beyond the table; a unit with an unknown image; a module unit whose range
lies outside its image; two overlapping units of one module image; a module
unit whose bss touches its image's payload; a function name used in the
resident image and in a module image; a module function with one changed
instruction, whose failure must name the image and leave the resident image
exact; a wrong declared size in a module unit; and `--image` with an unknown
name.

It requires success, with the report checked, for: a module image whose
whole payload is one C function, with its record, its coverage and a tripped
function control; a module image with raw bytes around its function and a
size that is not a multiple of four, with the raw count and a tripped raw
control; two module images at the same address with different contents, both
exact; a module unit and a resident unit that cover the same addresses;
`--image` with a module name, where the resident keys are absent, and with
`resident`, where `images` is absent; the carriers count with a second
archive that holds the same bytes and a third that holds others; and the
unmodified configuration, whose report has no `images` key.

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
`[[unit]]` tables are appended, including `kind` and the `rodata`, `data` and `bss` keys, which are written back as
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

One range of each data kind per unit, no incremental builds. Module images
are linked alone: a unit cannot yet refer by name to a unit of another
image, the second link of the two sides does not exist yet, and `fndiff.py`
and `mergeunits.py` do not know the `image` key. The
[proposal](overlay-build-proposal.md) describes those steps. Compiler
provenance is unchanged from the pilot: a compatible toolchain, not a
uniquely identified original.
