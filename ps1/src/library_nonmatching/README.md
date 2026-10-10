# Nonmatching C: the sound library

C for functions of the PS1 game's sound library (the Sony library part of the
resident executable, `SLPS_004.15`) that have no exact C: the four functions
below. The PS1 build does not use any file of this folder; the raw bytes stay
what the matching build owns. What nonmatching means here, how the test
works and what a pass does not show are on the page of the tool's folder,
[`../slot06_nonmatching/`](../slot06_nonmatching/README.md): a pass is
evidence for the tested inputs of a function's contract, not equivalence.

The rest of the library's functions that are C are exact C under
`../sdk/libsnd` and `../sdk/libspu`. A function that is later rebuilt
exactly becomes a unit of the build and leaves this folder.

Each function is a pair: `FUNC.c` with its contract in the header comment,
and `FUNC.py`, the contract as code. A file includes only `../game.h`,
`../protos.h` and `../externs.h`, and declares the library's variables and
records itself (local views, named as views in a comment), so that it
compiles in the repository's public builds. The function's name is its
address, as in the other nonmatching folders; the library's name for it is
in the header. The rows of `ps1/inventory/library.tsv` give the sizes, which
the tool reads (it looks a resident function up in `game.tsv` and in
`library.tsv` and wants it in exactly one).

## The functions and where they come from

- `func_8016b788` (`_spu_init`, the sound chip's reset): written by this
  project from the listing of the original. The reference that the SDK files
  come from has an `_spu_init` of another version of the library; it was read
  for the names of the chip's registers and none of its text is used. No
  license line. Its contract has the limits of a test without the chip; they
  are stated in the header (the test gives both runs plain memory in place of
  the chip's registers and models no register that answers by itself). It
  names two variables of the library that `../symbols.ld` did not name,
  `D_80033514` (a pointer; inferred to point at the DMA control register) and
  `D_80033540` (a 16-byte table),
  and the two lines that name them are added there. It is declared `void`, as
  `../protos.h` declares it.
- `func_8015ffc0` (`SpuVmAlloc`, the voice search): adapted from the function
  `SpuVmAlloc` of `src/main/psxsdk/libsnd/vmanager.c` of sotn-decomp, with the
  offsets, widths and compares changed to follow the listing. MIT, by the
  `SPDX-License-Identifier: MIT` line that the file keeps as its first line;
  commit and license text as [`../sdk/README.md`](../sdk/README.md) and
  [`../sdk/LICENSE`](../sdk/LICENSE) state them.
- `func_80163794` (`SpuVmSetVol`, the volume of the voices of one sequence):
  adapted in outline from the function `SpuVmSetVol` of the same file of
  sotn-decomp (the call, the loop over matching voices, the two output cells);
  the computation was rewritten from the listing. MIT, same line, commit and
  license as above.
- `func_80167388` (`_SsContDataEntry`, a data entry of the sequencer): written
  by this project from the listing. The reference only declares a function of
  this name and has no definition. No license line.

Adapted files, as `../sdk/README.md` lists adapted files: `func_8015ffc0.c`
and `func_80163794.c`.

## What the commands printed

On 2026-10-10, run from the tool's folder (`ps1/src/slot06_nonmatching/`), 2,000 cases, seed 1:

    python difftest.py --config ../build.toml --folder ../library_nonmatching --cases 2000 --all

```
func_8015ffc0: built 628 bytes, original 636 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8015ffc0 coverage: 159 of 159 instruction slots of the original executed
func_80163794: built 960 bytes, original 964 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80163794 coverage: 241 of 241 instruction slots of the original executed
func_80167388: built 1124 bytes, original 1504 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80167388 coverage: 301 of 376 instruction slots of the original executed
func_8016b788: built 1328 bytes, original 1464 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8016b788 coverage: 366 of 366 instruction slots of the original executed
```

    python difftest.py --config ../build.toml --folder ../library_nonmatching --cases 2000 --control --all

```
func_8015ffc0 control: different 240 of 2000 (expected more than 0)
  altered: noise call argument 0xfeffff, instruction slot 147
func_80163794 control: different 113 of 2000 (expected more than 0)
  altered: first of 3 pan tests compares with 0x41, instruction slot 140
func_80167388 control: different 266 of 2000 (expected more than 0)
  altered: constant 0x1e becomes 0x1f, instruction slot 53
func_8016b788 control: different 2000 of 2000 (expected more than 0)
  altered: control register receives 0xc001, instruction slot 320
```

    python difftest.py --config ../build.toml --folder ../library_nonmatching --cases 2000 --writes --all

```
func_8015ffc0 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80163794 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80167388 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8016b788 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
```

`func_80167388` has fewer slots executed than it has: the others are a loop
over the tones for one value of a byte that an earlier test has already
excluded, and one arm for a negative product of an unsigned byte; its header
comment names them and says that no input reaches them. The third block is the
write audit: in how many cases the original changed memory that the function's
setup did not make.
