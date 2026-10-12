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

- The cases, or a callee they run, read bytes that the setup did not make. Those bytes come from the game's memory image, so a file for the function would publish them and none is kept. `func_8013a3a8`.
- The file is larger than 100,000 bytes, also with `--max 6` (or with its one case). It is not kept. `func_8011c724`, `func_8013245c`, `func_8014e890`.

What the replay command of the folder printed on 2026-10-10, one line per function:

```
func_8013b0c4 replay: fixtures 10, passed 10, failed 0
func_8013c6ac replay: fixtures 8, passed 8, failed 0
func_801452ec replay: fixtures 5, passed 5, failed 0
func_80151324 replay: fixtures 5, passed 5, failed 0
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
func_8011c724: built 1120 bytes, original 1172 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8011c724 coverage: 291 of 293 instruction slots of the original executed
func_8013245c: built 1736 bytes, original 1748 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8013245c coverage: 437 of 437 instruction slots of the original executed
func_8013a3a8: built 2136 bytes, original 2436 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8013a3a8 coverage: 605 of 609 instruction slots of the original executed
func_8013b0c4: built 884 bytes, original 1172 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8013b0c4 coverage: 293 of 293 instruction slots of the original executed
func_8013c6ac: built 704 bytes, original 708 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8013c6ac coverage: 177 of 177 instruction slots of the original executed
func_801452ec: built 860 bytes, original 1632 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801452ec coverage: 406 of 408 instruction slots of the original executed
func_8014e890: built 780 bytes, original 780 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8014e890 coverage: 195 of 195 instruction slots of the original executed
func_80151324: built 708 bytes, original 1404 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80151324 coverage: 351 of 351 instruction slots of the original executed
```

The control (`--cases 2000 --control FUNC`):

```
func_8011c724 control: different 1355 of 2000 (expected more than 0)
  altered: store of the callee's result into the quad moved by two bytes, instruction slot 185
func_8013245c control: different 2000 of 2000 (expected more than 0)
  altered: constant 0x7807 changed to 0x7806, instruction slot 410
func_8013a3a8 control: different 418 of 2000 (expected more than 0)
  altered: store to field_260 moved by one byte, instruction slot 327
func_8013b0c4 control: different 1445 of 2000 (expected more than 0)
  altered: store to field_17f moved by one byte, instruction slot 49
func_8013c6ac control: different 1546 of 2000 (expected more than 0)
  altered: kind constant 9 changed to 10, instruction slot 33
func_801452ec control: different 1860 of 2000 (expected more than 0)
  altered: texture byte store moved by one byte, instruction slot 184
func_8014e890 control: different 2000 of 2000 (expected more than 0)
  altered: flag byte store moved by one byte, instruction slot 14
func_80151324 control: different 1360 of 2000 (expected more than 0)
  altered: a byte store at record offset 0x25 moves to 0x26, instruction slot 136
```

The write audit (`--cases 2000 --writes FUNC`, seed 1):

```
func_8011c724 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8013245c writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8013a3a8 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8013b0c4 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8013c6ac writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801452ec writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8014e890 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80151324 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
```

A function with fewer slots executed than it has names the others in its
header comment, with the reason why no input reaches them. The third
block is the write audit: in how many cases the original changed memory
that the function's setup did not make.
