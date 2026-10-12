# Nonmatching C: character modules

C for functions of the character modules `PL0C` to `PL17` that compiles with
the pinned toolchain and does not reproduce the original's bytes. The PS1
build does not use any file of this folder; the raw bytes stay what the
matching build owns. What nonmatching means here, how the test works and
what a pass does not show are on the page of the tool's folder,
[`../slot06_nonmatching/`](../slot06_nonmatching/README.md): a pass is
evidence for the tested inputs of a function's contract, not equivalence.

Each function is a pair: `FUNC.c` with its contract in the header comment,
and `FUNC.py`, the contract as code. A function that is later rebuilt
exactly moves to the folder of its image and leaves this one.

What these commands printed on 2026-10-10, run from the tool's folder:

    python difftest.py --config ../build.toml --folder ../slot04b_nonmatching --cases 2000 --all

```
func_801b1a00_slot04_14: built 260 bytes, original 264 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b1a00_slot04_14 coverage: 66 of 66 instruction slots of the original executed
func_801b1b40_slot04_0e: built 168 bytes, original 188 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b1b40_slot04_0e coverage: 47 of 47 instruction slots of the original executed
func_801b21f8_slot04_0f: built 88 bytes, original 120 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b21f8_slot04_0f coverage: 30 of 30 instruction slots of the original executed
func_801b4374_slot04_17: built 444 bytes, original 448 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b4374_slot04_17 coverage: 112 of 112 instruction slots of the original executed
func_801b43e8_slot04_11: built 444 bytes, original 448 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b43e8_slot04_11 coverage: 112 of 112 instruction slots of the original executed
```

    python difftest.py --config ../build.toml --folder ../slot04b_nonmatching --cases 2000 --control --all

```
func_801b1a00_slot04_14 control: different 2000 of 2000 (expected more than 0)
  altered: store of the second table word moved from field_50 to field_5c, instruction slot 39
func_801b1b40_slot04_0e control: different 2000 of 2000 (expected more than 0)
  altered: state byte 3 made 4, instruction slot 3
func_801b21f8_slot04_0f control: different 1993 of 2000 (expected more than 0)
  altered: final shift 4 changed to 3, instruction slot 19
func_801b4374_slot04_17 control: different 56 of 2000 (expected more than 0)
  altered: distance cutoff 0x100 changed to 0x101, instruction slot 44
func_801b43e8_slot04_11 control: different 70 of 2000 (expected more than 0)
  altered: distance cutoff 0x100 changed to 0x101, instruction slot 44
```

    python difftest.py --config ../build.toml --folder ../slot04b_nonmatching --cases 2000 --writes --all

```
func_801b1a00_slot04_14 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b1b40_slot04_0e writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b21f8_slot04_0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b4374_slot04_17 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b43e8_slot04_11 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
```

A function with fewer slots executed than it has names the others in its
header comment, with the reason why no input reaches them. The third
block is the write audit: in how many cases the original changed memory
that the function's setup did not make.

## Fixtures

A function that has fixtures has a file `FUNC.fixtures.json` beside its
`FUNC.c`. Replay runs the C of the function on the recorded inputs and never
runs the original code. It still needs the private inputs of the build
configuration and the PS1 compiler. From the tool's folder
(`ps1/src/slot06_nonmatching/`), for this folder and for one function:

    python -B difftest.py --config ../build.toml --folder ../slot04b_nonmatching --replay
    python -B difftest.py --config ../build.toml --folder ../slot04b_nonmatching --replay FUNC

For a new or changed function, record once (the wide comparison, then the
fixtures it keeps), then replay:

    python -B difftest.py --config ../build.toml --folder ../slot04b_nonmatching --cases 2000 --jobs 4 --record FUNC

A pass is evidence for the recorded inputs. It is not the wide comparison and
it is not equivalence. Details of the files are on the page of the tool's folder.

Functions of this folder without a fixtures file, by reason:

- The cases, or a callee they run, read bytes that the setup did not make. Those bytes come from the game's memory image, so a file for the function would publish them and none is kept. `func_801b1a00_slot04_14`.
- A callee that runs as original code takes the address of its own stack (it hands a local of its own to a function it calls). What it then makes of that address is another value under a build, and the stand-in could not check it; `--record` refuses the function. A limit of the tool. `func_801b1b40_slot04_0e`.

What the replay command of the folder printed on 2026-10-10, one line per function:

```
func_801b21f8_slot04_0f replay: fixtures 1, passed 1, failed 0
func_801b4374_slot04_17 replay: fixtures 9, passed 9, failed 0
func_801b43e8_slot04_11 replay: fixtures 8, passed 8, failed 0
```
