# Nonmatching C: the resident program

C for functions of the resident program (`SLPS_004.15` itself) that compiles
with the pinned toolchain and does not reproduce the original's bytes. The
PS1 build does not use any file of this folder; the raw bytes stay what the
matching build owns. What nonmatching means here, how the test works and
what a pass does not show are on the page of the tool's folder,
[`../slot06_nonmatching/`](../slot06_nonmatching/README.md): a pass is
evidence for the tested inputs of a function's contract, not equivalence.

Each function is a pair: `FUNC.c` with its contract in the header comment,
and `FUNC.py`, the contract as code. A function that is later rebuilt
exactly becomes a unit of the build and leaves this folder.

## Testing one function

A function is tested alone. When one is added or changed, run its own
five runs, from the tool's folder, and at the same time (see
[efficient checks](../../../docs/efficiency.md)):

    python difftest.py --config ../build.toml --folder ../resident_nonmatching --cases 2000 FUNC
    python difftest.py --config ../build.toml --folder ../resident_nonmatching --cases 2000 --seed 7 FUNC
    python difftest.py --config ../build.toml --folder ../resident_nonmatching --cases 2000 --control FUNC
    python difftest.py --config ../build.toml --folder ../resident_nonmatching --cases 2000 --writes FUNC
    python difftest.py --config ../build.toml --folder ../resident_nonmatching --cases 2000 --writes --seed 7 FUNC

Then put that function's lines into the three blocks below, in place of
its old ones or at its place in the order of names. No other function is
run again for it, and the folder is not run as a whole.

## Fixtures

A function that has fixtures has a file `FUNC.fixtures.json` beside its
`FUNC.c`. Replay runs the C of the function on the recorded inputs and never
runs the original code. It still needs the private inputs of the build
configuration and the PS1 compiler. From the tool's folder
(`ps1/src/slot06_nonmatching/`), for this folder and for one function:

    python -B difftest.py --config ../build.toml --folder ../resident_nonmatching --replay
    python -B difftest.py --config ../build.toml --folder ../resident_nonmatching --replay FUNC

For a new or changed function, record once (the wide comparison, then the
fixtures it keeps), then replay:

    python -B difftest.py --config ../build.toml --folder ../resident_nonmatching --cases 2000 --jobs 4 --record FUNC

A pass is evidence for the recorded inputs. It is not the wide comparison and
it is not equivalence. Details of the files are on the page of the tool's folder.

Functions of this folder without a fixtures file, by reason:

- The cases, or a callee they run, read bytes that the setup did not make. Those bytes come from the game's memory image, so a file for the function would publish them and none is kept. `func_80120604`, `func_80120cf0`, `func_8013a3a8`, `func_80140b5c`, `func_80140cd8`, `func_80142030`.
- The file is larger than 100,000 bytes, also with `--max 6` (or with its one case). It is not kept. `func_8011c724`, `func_8013245c`, `func_8014e890`.

What the replay command of the folder printed on 2026-10-10, one line per function:

```
func_8011acbc replay: fixtures 6, passed 6, failed 0
func_8011bf70 replay: fixtures 12, passed 12, failed 0
func_8011cf98 replay: fixtures 12, passed 12, failed 0
func_8011eb4c replay: fixtures 1, passed 1, failed 0
func_80124304 replay: fixtures 6, passed 6, failed 0
func_801254f4 replay: fixtures 10, passed 10, failed 0
func_8012f9b0 replay: fixtures 8, passed 8, failed 0
func_8013172c replay: fixtures 3, passed 3, failed 0
func_80131ab4 replay: fixtures 6, passed 6, failed 0
func_80135a10 replay: fixtures 8, passed 8, failed 0
func_801397d0 replay: fixtures 4, passed 4, failed 0
func_8013b0c4 replay: fixtures 10, passed 10, failed 0
func_8013bfa4 replay: fixtures 10, passed 10, failed 0
func_8013c6ac replay: fixtures 8, passed 8, failed 0
func_8013db48 replay: fixtures 12, passed 12, failed 0
func_8013f3e8 replay: fixtures 3, passed 3, failed 0
func_8013f474 replay: fixtures 12, passed 12, failed 0
func_80141cec replay: fixtures 9, passed 9, failed 0
func_801452ec replay: fixtures 5, passed 5, failed 0
func_8014c4a8 replay: fixtures 3, passed 3, failed 0
func_80151324 replay: fixtures 5, passed 5, failed 0
func_80152124 replay: fixtures 2, passed 2, failed 0
```

## The recorded lines

The three blocks are a record, not an instruction. They are what the
commands printed for each function when it was last run. As it stands
that is one run of the whole folder on 2026-10-10, made when the last
functions were added, with `--all` in place of FUNC; the page keeps
those lines as printed. From now on a function's lines change only when
that function is run again.

The test (`--cases 2000 FUNC`, seed 1):

```
func_8011acbc: built 1080 bytes, original 2264 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8011acbc coverage: 566 of 566 instruction slots of the original executed
func_8011bf70: built 1480 bytes, original 1528 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8011bf70 coverage: 381 of 382 instruction slots of the original executed
func_8011c724: built 1120 bytes, original 1172 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8011c724 coverage: 291 of 293 instruction slots of the original executed
func_8011cf98: built 1520 bytes, original 1568 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8011cf98 coverage: 391 of 392 instruction slots of the original executed
func_8011eb4c: built 716 bytes, original 792 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8011eb4c coverage: 198 of 198 instruction slots of the original executed
func_80120604: built 928 bytes, original 952 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80120604 coverage: 238 of 238 instruction slots of the original executed
func_80120cf0: built 664 bytes, original 592 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80120cf0 coverage: 148 of 148 instruction slots of the original executed
func_80124304: built 904 bytes, original 928 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80124304 coverage: 232 of 232 instruction slots of the original executed
func_801254f4: built 412 bytes, original 400 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801254f4 coverage: 100 of 100 instruction slots of the original executed
func_8012f9b0: built 300 bytes, original 328 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8012f9b0 coverage: 82 of 82 instruction slots of the original executed
func_8013172c: built 280 bytes, original 296 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8013172c coverage: 74 of 74 instruction slots of the original executed
func_80131ab4: built 368 bytes, original 456 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80131ab4 coverage: 114 of 114 instruction slots of the original executed
func_8013245c: built 1736 bytes, original 1748 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8013245c coverage: 437 of 437 instruction slots of the original executed
func_80135a10: built 524 bytes, original 508 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80135a10 coverage: 127 of 127 instruction slots of the original executed
func_801397d0: built 344 bytes, original 344 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801397d0 coverage: 86 of 86 instruction slots of the original executed
func_8013a3a8: built 2136 bytes, original 2436 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8013a3a8 coverage: 605 of 609 instruction slots of the original executed
func_8013b0c4: built 884 bytes, original 1172 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8013b0c4 coverage: 293 of 293 instruction slots of the original executed
func_8013bfa4: built 1388 bytes, original 1800 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8013bfa4 coverage: 450 of 450 instruction slots of the original executed
func_8013c6ac: built 704 bytes, original 708 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8013c6ac coverage: 177 of 177 instruction slots of the original executed
func_8013db48: built 408 bytes, original 408 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8013db48 coverage: 102 of 102 instruction slots of the original executed
func_8013f3e8: built 84 bytes, original 140 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8013f3e8 coverage: 35 of 35 instruction slots of the original executed
func_8013f474: built 376 bytes, original 388 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8013f474 coverage: 97 of 97 instruction slots of the original executed
func_80140b5c: built 320 bytes, original 380 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80140b5c coverage: 94 of 95 instruction slots of the original executed
func_80140cd8: built 440 bytes, original 488 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80140cd8 coverage: 122 of 122 instruction slots of the original executed
func_80141cec: built 336 bytes, original 328 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80141cec coverage: 82 of 82 instruction slots of the original executed
func_80142030: built 248 bytes, original 248 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80142030 coverage: 62 of 62 instruction slots of the original executed
func_801452ec: built 860 bytes, original 1632 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801452ec coverage: 406 of 408 instruction slots of the original executed
func_8014c4a8: built 136 bytes, original 128 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8014c4a8 coverage: 32 of 32 instruction slots of the original executed
func_8014e890: built 780 bytes, original 780 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8014e890 coverage: 195 of 195 instruction slots of the original executed
func_80151324: built 708 bytes, original 1404 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80151324 coverage: 351 of 351 instruction slots of the original executed
func_80152124: built 124 bytes, original 124 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80152124 coverage: 31 of 31 instruction slots of the original executed
```

The control (`--cases 2000 --control FUNC`):

```
func_8011acbc control: different 2000 of 2000 (expected more than 0)
  altered: loop limit 41, instruction slot 261
func_8011bf70 control: different 1598 of 2000 (expected more than 0)
  altered: store of the object's entry index moved by two bytes, instruction slot 338
func_8011c724 control: different 1355 of 2000 (expected more than 0)
  altered: store of the callee's result into the quad moved by two bytes, instruction slot 185
func_8011cf98 control: different 1595 of 2000 (expected more than 0)
  altered: store of the object's entry index moved by two bytes, instruction slot 345
func_8011eb4c control: different 2000 of 2000 (expected more than 0)
  altered: type byte of the third array is 0x11, instruction slot 73
func_80120604 control: different 254 of 2000 (expected more than 0)
  altered: volume shifted by 5 instead of 6, instruction slot 102
func_80120cf0 control: different 1012 of 2000 (expected more than 0)
  altered: random byte masked with 0x70, instruction slot 111
func_80124304 control: different 307 of 2000 (expected more than 0)
  altered: field_49 value 0x63 changed to 0x62, instruction slot 162
func_801254f4 control: different 118 of 2000 (expected more than 0)
  altered: failure value 0x3c changed to 0x3d, instruction slot 32
func_8012f9b0 control: different 1859 of 2000 (expected more than 0)
  altered: cursor stride 0xc0 changed to 0xc4, instruction slot 64
func_8013172c control: different 2000 of 2000 (expected more than 0)
  altered: store of field_134 moved by four bytes, instruction slot 59
func_80131ab4 control: different 258 of 2000 (expected more than 0)
  altered: flag value 1 changed to 2, instruction slot 21
func_8013245c control: different 2000 of 2000 (expected more than 0)
  altered: constant 0x7807 changed to 0x7806, instruction slot 410
func_80135a10 control: different 749 of 2000 (expected more than 0)
  altered: mode 2 code 0x20 changed to 0x21, instruction slot 94
func_801397d0 control: different 63 of 2000 (expected more than 0)
  altered: last slt changed to sltu, instruction slot 83
func_8013a3a8 control: different 418 of 2000 (expected more than 0)
  altered: store to field_260 moved by one byte, instruction slot 327
func_8013b0c4 control: different 1445 of 2000 (expected more than 0)
  altered: store to field_17f moved by one byte, instruction slot 49
func_8013bfa4 control: different 147 of 2000 (expected more than 0)
  altered: kind constant 0x17 changed to 0x16, instruction slot 244
func_8013c6ac control: different 1546 of 2000 (expected more than 0)
  altered: kind constant 9 changed to 10, instruction slot 33
func_8013db48 control: different 85 of 2000 (expected more than 0)
  altered: first branch on v0 == 0 removed, instruction slot 46
func_8013f3e8 control: different 2000 of 2000 (expected more than 0)
  altered: head store moved by two bytes, instruction slot 17
func_8013f474 control: different 323 of 2000 (expected more than 0)
  altered: first branch on v0 == 0 removed, instruction slot 4
func_80140b5c control: different 186 of 2000 (expected more than 0)
  altered: clearing store moved by one byte, instruction slot 41
func_80140cd8 control: different 1520 of 2000 (expected more than 0)
  altered: store of field_5c moved by two bytes, instruction slot 91
func_80141cec control: different 596 of 2000 (expected more than 0)
  altered: store of field_157 moved by one byte, instruction slot 74
func_80142030 control: different 784 of 2000 (expected more than 0)
  altered: store of field_219 moved by one byte, instruction slot 58
func_801452ec control: different 1860 of 2000 (expected more than 0)
  altered: texture byte store moved by one byte, instruction slot 184
func_8014c4a8 control: different 2000 of 2000 (expected more than 0)
  altered: store of data_80189464 moved by two bytes, instruction slot 31
func_8014e890 control: different 2000 of 2000 (expected more than 0)
  altered: flag byte store moved by one byte, instruction slot 14
func_80151324 control: different 1360 of 2000 (expected more than 0)
  altered: a byte store at record offset 0x25 moves to 0x26, instruction slot 136
func_80152124 control: different 1603 of 2000 (expected more than 0)
  altered: store of the first constant moved to offset 0xe, instruction slot 19
```

The write audit (`--cases 2000 --writes FUNC`, seed 1):

```
func_8011acbc writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8011bf70 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8011c724 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8011cf98 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8011eb4c writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80120604 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80120cf0 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80124304 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801254f4 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8012f9b0 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8013172c writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80131ab4 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8013245c writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80135a10 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801397d0 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8013a3a8 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8013b0c4 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8013bfa4 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8013c6ac writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8013db48 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8013f3e8 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8013f474 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80140b5c writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80140cd8 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80141cec writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80142030 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801452ec writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8014c4a8 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8014e890 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80151324 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80152124 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
```

A function with fewer slots executed than it has names the others in its
header comment, with the reason why no input reaches them. The third
block is the write audit: in how many cases the original changed memory
that the function's setup did not make.
