# Nonmatching C: the other modules

C for functions of the overlay modules that have no folder of their own
here (the stage modules are in
[`../slot06_nonmatching/`](../slot06_nonmatching/README.md), and
[`../slot04b_nonmatching/`](../slot04b_nonmatching/README.md) holds the
character modules its page names; every other module's functions come
here) that compiles with the pinned toolchain and does not reproduce the
original's bytes. A file's name ends with the name of its image;
`../build.toml` says which file of the disc's archives each image is.
The PS1 build does not use any file of this folder; the raw bytes stay
what the matching build owns. What nonmatching means here, how the test
works and what a pass does not show are on the page of the tool's
folder, [`../slot06_nonmatching/`](../slot06_nonmatching/README.md): a
pass is evidence for the tested inputs of a function's contract, not
equivalence.

Each function is a pair: `FUNC.c` with its contract in the header
comment, and `FUNC.py`, the contract as code. A function that is later
rebuilt exactly becomes a unit of the build and leaves this folder.

## Testing one function

A function is tested alone. When one is added or changed, run its own
five runs, from the tool's folder, and at the same time (see
[efficient checks](../../../docs/efficiency.md)):

    python difftest.py --config ../build.toml --folder ../modules_nonmatching --cases 2000 FUNC
    python difftest.py --config ../build.toml --folder ../modules_nonmatching --cases 2000 --seed 7 FUNC
    python difftest.py --config ../build.toml --folder ../modules_nonmatching --cases 2000 --control FUNC
    python difftest.py --config ../build.toml --folder ../modules_nonmatching --cases 2000 --writes FUNC
    python difftest.py --config ../build.toml --folder ../modules_nonmatching --cases 2000 --writes --seed 7 FUNC

Then put that function's lines into the three blocks below, in place of
its old ones or at its place in the order of names. No other function is
run again for it, and the folder is not run as a whole.

## Fixtures

A function that has fixtures has a file `FUNC.fixtures.json` beside its
`FUNC.c`. Replay runs the C of the function on the recorded inputs and never
runs the original code. It still needs the private inputs of the build
configuration and the PS1 compiler. From the tool's folder
(`ps1/src/slot06_nonmatching/`), for this folder and for one function:

    python -B difftest.py --config ../build.toml --folder ../modules_nonmatching --replay
    python -B difftest.py --config ../build.toml --folder ../modules_nonmatching --replay FUNC

For a new or changed function, record once (the wide comparison, then the
fixtures it keeps), then replay:

    python -B difftest.py --config ../build.toml --folder ../modules_nonmatching --cases 2000 --jobs 4 --record FUNC

A pass is evidence for the recorded inputs. It is not the wide comparison and
it is not equivalence. Details of the files are on the page of the tool's folder.

Functions of this folder without a fixtures file, by reason:

- The cases, or a callee they run, read bytes that the setup did not make. Those bytes come from the game's memory image, so a file for the function would publish them and none is kept. `func_80012714_slot01`, `func_800e0a1c_slot0f`.

What the replay command of the folder printed on 2026-10-10, one line per function:

```
func_80012990_slot01 replay: fixtures 2, passed 2, failed 0
func_80079144_slot2b replay: fixtures 3, passed 3, failed 0
func_800e0850_slot0f replay: fixtures 9, passed 9, failed 0
func_800e4ab0_slot0f replay: fixtures 3, passed 3, failed 0
func_801b2f1c_slot04_0a replay: fixtures 5, passed 5, failed 0
func_801e0ce8_slot0b replay: fixtures 2, passed 2, failed 0
```

## The recorded lines

The three blocks are a record, not an instruction. They are what the
commands printed for each function when it was last run. For all functions
but `func_800e0850_slot0f` and `func_800e0a1c_slot0f` that is one run of
the whole folder on 2026-10-10, with `--all` in place of FUNC; the page
keeps those lines as printed. The lines of the two functions named are from
their own runs on 2026-10-10 (seed 1 in the test and the write audit; their
seed 7 runs also printed `different 0` and `outside 0`). From now on a
function's lines change only when that function is run again.

The test (`--cases 2000 FUNC`, seed 1):

```
func_80012714_slot01: built 568 bytes, original 636 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80012714_slot01 coverage: 159 of 159 instruction slots of the original executed
func_80012990_slot01: built 312 bytes, original 448 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80012990_slot01 coverage: 112 of 112 instruction slots of the original executed
func_80079144_slot2b: built 172 bytes, original 196 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80079144_slot2b coverage: 49 of 49 instruction slots of the original executed
func_800e0850_slot0f: built 412 bytes, original 460 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800e0850_slot0f coverage: 115 of 115 instruction slots of the original executed
func_800e0a1c_slot0f: built 276 bytes, original 300 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800e0a1c_slot0f coverage: 75 of 75 instruction slots of the original executed
func_800e4ab0_slot0f: built 540 bytes, original 608 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800e4ab0_slot0f coverage: 152 of 152 instruction slots of the original executed
func_801b2f1c_slot04_0a: built 304 bytes, original 312 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b2f1c_slot04_0a coverage: 78 of 78 instruction slots of the original executed
func_801e0ce8_slot0b: built 304 bytes, original 448 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e0ce8_slot0b coverage: 112 of 112 instruction slots of the original executed
```

The control (`--cases 2000 --control FUNC`):

```
func_80012714_slot01 control: different 1323 of 2000 (expected more than 0)
  altered: a shift right by 4 changed to 3, instruction slot 40
func_80012990_slot01 control: different 2000 of 2000 (expected more than 0)
  altered: a byte store of the primitive moved by one byte, instruction slot 24
func_80079144_slot2b control: different 1020 of 2000 (expected more than 0)
  altered: negation turned into a doubling, instruction slot 31
func_800e0850_slot0f control: different 585 of 2000 (expected more than 0)
  altered: write length changed to 0x2001, instruction slot 84
func_800e0a1c_slot0f control: different 314 of 2000 (expected more than 0)
  altered: a returned 2 becomes 3, instruction slot 61
func_800e4ab0_slot0f control: different 649 of 2000 (expected more than 0)
  altered: y store moved to the x offset, instruction slot 83
func_801b2f1c_slot04_0a control: different 1003 of 2000 (expected more than 0)
  altered: field_50 constant 0x38000 changed to 0x48000, instruction slot 51
func_801e0ce8_slot0b control: different 1997 of 2000 (expected more than 0)
  altered: id shift changed from 6 to 5, instruction slot 54
```

The write audit (`--cases 2000 --writes FUNC`, seed 1):

```
func_80012714_slot01 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80012990_slot01 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80079144_slot2b writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e0850_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e0a1c_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e4ab0_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b2f1c_slot04_0a writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e0ce8_slot0b writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
```

A function with fewer slots executed than it has names the others in its
header comment, with the reason why no input reaches them. The third
block is the write audit: in how many cases the original changed memory
that the function's setup did not make.
