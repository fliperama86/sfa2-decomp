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
| `ps1/tools/test_matchbuild.py` | public | Negative controls for the build tool |
| `ps1/tools/baseline.py` | public | Disc/file baseline manifest and verification |
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
- **raw**: retained baseline bytes. Raw ranges are never declared. They are
  computed as the complement of all unit ranges, so a gap cannot silently count
  as recovered source.
- **asm**: reserved for reviewed assembly owners. Not implemented yet.

The 2,048-byte PS-X EXE header is retained from the baseline and reported as
raw.

Rejected before any compilation: overlapping units, ranges outside the payload,
addresses or sizes that are not multiples of four, zero sizes, duplicate
function or unit names, a gap between functions of one unit, and any name in
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
```

`symbols.ld` sits next to `build.toml`, holds lines of the form
`name = 0x80000000;` and is included by the generated linker script.

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
.venv/bin/python ps1/tools/matchbuild.py [--config PATH] [--tag NAME] [--reference]
.venv/bin/python ps1/tools/test_matchbuild.py [--config PATH]
```

Exit status: 0 all checks passed, 1 failed check or build step, 2 invalid
configuration, 3 unusable environment such as a missing SSH master.

## Pipeline

Each run deletes and recreates `build/<tag>/`. Nothing is reused.

1. Read and validate the configuration. Check the baseline hash, the `PS-X EXE`
   magic, and that the file size equals header size plus payload size. Extract
   the payload to the build directory.
2. Check toolchain pins: `cc1` SHA-256 (hashed on the host where it runs) and
   the maspsx git commit. Record tool versions.
3. Per unit: preprocess with
   `clang -E -P -x c -target mipsel-none-elf -nostdinc`, compile with
   `cc1 -quiet <flags>`, convert with `maspsx --aspsx-version=<v>`, assemble
   with `as -EL -G0 -march=r3000 -mabi=32 -no-pad-sections`. Every subprocess
   exit status is enforced. Without `-no-pad-sections` the assembler pads each
   section to 16 bytes and a unit would spill past its declared range.
4. Reject a unit object whose allocated sections other than `.text` are
   non-empty. Data and read-only data ownership is not implemented yet and must
   not be discarded or placed silently.
5. Generate `raw.s` with one section per raw range using `.incbin` on the
   extracted payload, and `link.ld` placing every range at its explicit address
   in address order. Link with undefined symbols as errors.
6. Convert the ELF to a flat image. Rebuilt executable is baseline header plus
   image.

## Checks

All fail closed with a non-zero exit status.

- Each declared function exists in the ELF with the declared address and size,
  and its bytes equal the baseline range. Report the first differing offset and
  the count of equal instruction words.
- Each unit's `.text` size equals its declared range.
- Image size and SHA-256 equal the baseline payload. Rebuilt executable SHA-256
  equals the baseline executable.
- Comparator controls on every successful build: flipping one byte inside each
  C function of the image must make exactly that function fail, and flipping
  one raw byte must make the image check fail. A control that does not trip
  fails the build.

`test_matchbuild.py` runs the tool against temporary copies of the private
configuration and requires failure for: the `[selftest]` source mutation, a
wrong declared size, two overlapping units, a symbol removed from `symbols.ld`,
a unit function duplicated in `symbols.ld`, a wrong `cc1` hash pin, a wrong
baseline hash, and floating-point source when the default compiler is marked
`no_float`. It also requires the unmodified build to pass.

## Report

`build/<tag>/report.json` and a short text summary:

- inputs with SHA-256: configuration, symbols, baseline, each source, and each
  unit's preprocessed text, which covers included headers;
- tools: which compiler was used, paths, versions, pinned hashes and commits;
- per unit and function: addresses, sizes, exact or not, hashes;
- coverage in bytes by owner kind, plus the function count owned by C;
- overall `exact` flag, image and executable hashes, control results.

## Baseline manifest

`baseline.py pin` reads the user's disc image and the audit inventory and writes
the private manifest: image size and SHA-256, then per file its path, LBA, size
and SHA-256, plus PS-X EXE header fields where the magic is present. File
content is the first `size` bytes of the concatenated 2,048-byte user-data
fields starting at the file's LBA. `baseline.py verify` recomputes and compares,
against the image, against a directory of extracted files, or both.

The published root pins remain the disc and executable hashes in the
[pilot report](matching-pilot.md). The manifest is a derived index.

## Limits

No data or read-only data ownership, no assembly owners, no overlay images, no
incremental builds. Compiler provenance is unchanged from the pilot: a
compatible toolchain, not a uniquely identified original.
