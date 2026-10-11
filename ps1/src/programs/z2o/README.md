# C for the opening movie's program

C for functions of the opening movie's program, the stand-alone executable
`PAC/Z2O.EXE` of the disc that the game starts with `Exec` (inferred: the
program that plays the opening movie; the names of its functions in the
private study are inferred, not original). It is not the resident program
and not a module of an archive. The program is declared in
[`../programs.toml`](../programs.toml) (its name, the hash of the user's own
copy, where its body loads, the functions that have C here, the names of its
data and of its copy of a library routine);
[`../programs.py`](../programs.py) reads that file and
[`../discfile.py`](../discfile.py) takes the program's file out of the user's
disc image (the file is private and is never committed). The PS1 build and the
PC build do not see this folder. How the test works and what a pass does not
show are on the page of the tool's folder,
[`../../slot06_nonmatching/`](../../slot06_nonmatching/README.md): a pass is
evidence for the tested inputs of a function's contract, not equivalence. The
form of this folder is that of [`../logo/`](../logo/README.md).

Each function is a pair: `FUNC.c` with its contract in the header comment,
and `FUNC.py`, the contract as code. A function's name is
`func_ADDRESS_z2o`, the address in eight hex digits.

## The program's file

Once, with the user's own disc image (the `.bin`, or the `.cue` that names it),
from `ps1/src/programs/`; `ps1/extract/` is a private folder that Git ignores:

    mkdir -p ../../extract/programs
    python discfile.py DISC PAC/Z2O.EXE ../../extract/programs/Z2O.EXE

It prints the size and the SHA-256, which must be the `sha256` in
`programs.toml`. The tool makes a new file and never writes over one: to take
the file out again remove the old copy first. The comparison, the write audit
and the control need the file (the original runs from it, over the resident
executable); `--replay` does not.

## Testing one function

A function is tested alone. When one is added or changed, run its own five
runs, from the tool's folder, and at the same time (see
[efficient checks](../../../../docs/efficiency.md)):

    python difftest.py --config ../build.toml --folder ../programs/z2o --cases 2000 FUNC
    python difftest.py --config ../build.toml --folder ../programs/z2o --cases 2000 --seed 7 FUNC
    python difftest.py --config ../build.toml --folder ../programs/z2o --cases 2000 --control FUNC
    python difftest.py --config ../build.toml --folder ../programs/z2o --cases 2000 --writes FUNC
    python difftest.py --config ../build.toml --folder ../programs/z2o --cases 2000 --writes --seed 7 FUNC

Then put that function's lines into the blocks below. No other function is run
again for it.

The global pointer register: the program's entry code first clears the words
from `0x8002f7f0` up to `0x8008a400` and only then sets the register to
`0x8002f7f0` before it jumps to the main. The reader checks a declared value
against the entry code's first two instructions only, so `programs.toml`
declares none for this program, and the test tool begins each run with the
register at 0. That is right only for a function that does not use the
register. The listing of each of the twenty-seven functions here was read for that: no
instruction of them reads or writes the register or reaches data through it
plus an offset (they build each address with `lui`), and each function's header
says so. A function of this program that does use the register needs the
reader extended first.

A contract for a function that reads the program's data makes the entry it
reads (the setup writes values of its own at the entry's address), so that no
case reads a byte of the program's data: the record then names no byte that the
setup did not make, and the fixtures file holds none of the program.

## Fixtures

A function that has fixtures has a file `FUNC.fixtures.json` beside its
`FUNC.c`. Replay runs the C of the function on the recorded inputs and never
runs the original code; it needs the private inputs of the build configuration
and the PS1 compiler, and not the program's file. From the tool's folder
(`ps1/src/slot06_nonmatching/`):

    python -B difftest.py --config ../build.toml --folder ../programs/z2o --replay
    python -B difftest.py --config ../build.toml --folder ../programs/z2o --replay FUNC

For a new or changed function, record once (the wide comparison, then the
fixtures it keeps), then replay. A record that prints a warning about bytes
that the setup did not make is not kept in the repository: the file would hold
bytes of the program.

    python -B difftest.py --config ../build.toml --folder ../programs/z2o --cases 2000 --jobs 4 --record FUNC

A pass is evidence for the recorded inputs. It is not the wide comparison and
it is not equivalence.

The record of each function below printed no warning about bytes that the setup
did not make; the case counts of the fixtures files are in the record block.
The declarations of the program's data and of the library routine the C calls
are in [`z2o.h`](z2o.h).

## The recorded lines

The blocks are a record, not an instruction: what the commands printed for each
function when it was added. They were printed on 2026-10-11.

The test (`--cases 2000 FUNC`, seed 1, then `--seed 7`):

```
func_80010be4_z2o: built 84 bytes, original 84 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010be4_z2o coverage: 21 of 21 instruction slots of the original executed
func_80010be4_z2o: built 84 bytes, original 84 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010be4_z2o coverage: 21 of 21 instruction slots of the original executed
func_80011428_z2o: built 48 bytes, original 48 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011428_z2o coverage: 12 of 12 instruction slots of the original executed
func_80011428_z2o: built 48 bytes, original 48 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011428_z2o coverage: 12 of 12 instruction slots of the original executed
func_800116c8_z2o: built 16 bytes, original 16 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800116c8_z2o coverage: 4 of 4 instruction slots of the original executed
func_800116c8_z2o: built 16 bytes, original 16 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800116c8_z2o coverage: 4 of 4 instruction slots of the original executed
func_80011810_z2o: built 32 bytes, original 32 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011810_z2o coverage: 8 of 8 instruction slots of the original executed
func_80011810_z2o: built 32 bytes, original 32 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011810_z2o coverage: 8 of 8 instruction slots of the original executed
func_800119b8_z2o: built 16 bytes, original 16 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800119b8_z2o coverage: 4 of 4 instruction slots of the original executed
func_800119b8_z2o: built 16 bytes, original 16 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800119b8_z2o coverage: 4 of 4 instruction slots of the original executed
func_800119e8_z2o: built 60 bytes, original 60 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800119e8_z2o coverage: 15 of 15 instruction slots of the original executed
func_800119e8_z2o: built 60 bytes, original 60 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800119e8_z2o coverage: 15 of 15 instruction slots of the original executed
func_800115a8_z2o: built 136 bytes, original 136 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800115a8_z2o coverage: 34 of 34 instruction slots of the original executed
func_800115a8_z2o: built 136 bytes, original 136 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800115a8_z2o coverage: 34 of 34 instruction slots of the original executed
func_80011630_z2o: built 152 bytes, original 152 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011630_z2o coverage: 38 of 38 instruction slots of the original executed
func_80011630_z2o: built 152 bytes, original 152 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011630_z2o coverage: 38 of 38 instruction slots of the original executed
func_80011830_z2o: built 192 bytes, original 192 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011830_z2o coverage: 48 of 48 instruction slots of the original executed
func_80011830_z2o: built 192 bytes, original 192 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011830_z2o coverage: 48 of 48 instruction slots of the original executed
func_800118f0_z2o: built 200 bytes, original 200 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800118f0_z2o coverage: 50 of 50 instruction slots of the original executed
func_800118f0_z2o: built 200 bytes, original 200 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800118f0_z2o coverage: 50 of 50 instruction slots of the original executed
func_80011a24_z2o: built 164 bytes, original 164 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011a24_z2o coverage: 41 of 41 instruction slots of the original executed
func_80011a24_z2o: built 164 bytes, original 164 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011a24_z2o coverage: 41 of 41 instruction slots of the original executed
func_8001139c_z2o: built 140 bytes, original 140 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8001139c_z2o coverage: 35 of 35 instruction slots of the original executed
func_8001139c_z2o: built 140 bytes, original 140 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8001139c_z2o coverage: 35 of 35 instruction slots of the original executed
func_80010ae8_z2o: built 252 bytes, original 252 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010ae8_z2o coverage: 63 of 63 instruction slots of the original executed
func_80010ae8_z2o: built 252 bytes, original 252 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010ae8_z2o coverage: 63 of 63 instruction slots of the original executed
func_80011708_z2o: built 292 bytes, original 264 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011708_z2o coverage: 66 of 66 instruction slots of the original executed
func_80011708_z2o: built 292 bytes, original 264 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011708_z2o coverage: 66 of 66 instruction slots of the original executed
func_80011018_z2o: built 184 bytes, original 184 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011018_z2o coverage: 46 of 46 instruction slots of the original executed
func_80011018_z2o: built 184 bytes, original 184 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011018_z2o coverage: 46 of 46 instruction slots of the original executed
func_800108e8_z2o: built 512 bytes, original 512 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800108e8_z2o coverage: 128 of 128 instruction slots of the original executed
func_800108e8_z2o: built 512 bytes, original 512 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800108e8_z2o coverage: 128 of 128 instruction slots of the original executed
func_80010c38_z2o: built 776 bytes, original 776 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010c38_z2o coverage: 192 of 194 instruction slots of the original executed
func_80010c38_z2o: built 776 bytes, original 776 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010c38_z2o coverage: 192 of 194 instruction slots of the original executed
func_800110d0_z2o: built 720 bytes, original 716 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800110d0_z2o coverage: 172 of 179 instruction slots of the original executed
func_800110d0_z2o: built 720 bytes, original 716 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800110d0_z2o coverage: 172 of 179 instruction slots of the original executed
func_80011afc_z2o: built 8 bytes, original 8 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011afc_z2o coverage: 2 of 2 instruction slots of the original executed
func_80011afc_z2o: built 8 bytes, original 8 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011afc_z2o coverage: 2 of 2 instruction slots of the original executed
func_800116d8_z2o: built 16 bytes, original 16 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800116d8_z2o coverage: 4 of 4 instruction slots of the original executed
func_800116d8_z2o: built 16 bytes, original 16 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800116d8_z2o coverage: 4 of 4 instruction slots of the original executed
func_800116e8_z2o: built 16 bytes, original 16 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800116e8_z2o coverage: 4 of 4 instruction slots of the original executed
func_800116e8_z2o: built 16 bytes, original 16 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800116e8_z2o coverage: 4 of 4 instruction slots of the original executed
func_800116f8_z2o: built 16 bytes, original 16 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800116f8_z2o coverage: 4 of 4 instruction slots of the original executed
func_800116f8_z2o: built 16 bytes, original 16 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800116f8_z2o coverage: 4 of 4 instruction slots of the original executed
func_800119c8_z2o: built 16 bytes, original 16 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800119c8_z2o coverage: 4 of 4 instruction slots of the original executed
func_800119c8_z2o: built 16 bytes, original 16 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800119c8_z2o coverage: 4 of 4 instruction slots of the original executed
func_800119d8_z2o: built 16 bytes, original 16 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800119d8_z2o coverage: 4 of 4 instruction slots of the original executed
func_800119d8_z2o: built 16 bytes, original 16 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800119d8_z2o coverage: 4 of 4 instruction slots of the original executed
func_80011458_z2o: built 32 bytes, original 32 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011458_z2o coverage: 8 of 8 instruction slots of the original executed
func_80011458_z2o: built 32 bytes, original 32 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011458_z2o coverage: 8 of 8 instruction slots of the original executed
func_80011530_z2o: built 120 bytes, original 120 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011530_z2o coverage: 30 of 30 instruction slots of the original executed
func_80011530_z2o: built 120 bytes, original 120 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011530_z2o coverage: 30 of 30 instruction slots of the original executed
func_80011478_z2o: built 176 bytes, original 184 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011478_z2o coverage: 46 of 46 instruction slots of the original executed
func_80011478_z2o: built 176 bytes, original 184 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011478_z2o coverage: 46 of 46 instruction slots of the original executed
```

The negative control (`--control`):

```
func_80010be4_z2o control: different 2000 of 2000 (expected more than 0)
func_80011428_z2o control: different 2000 of 2000 (expected more than 0)
func_800116c8_z2o control: different 2000 of 2000 (expected more than 0)
func_80011810_z2o control: different 2000 of 2000 (expected more than 0)
func_800119b8_z2o control: different 2000 of 2000 (expected more than 0)
func_800119e8_z2o control: different 2000 of 2000 (expected more than 0)
func_800115a8_z2o control: different 992 of 2000 (expected more than 0)
func_80011630_z2o control: different 1039 of 2000 (expected more than 0)
func_80011830_z2o control: different 952 of 2000 (expected more than 0)
func_800118f0_z2o control: different 858 of 2000 (expected more than 0)
func_80011a24_z2o control: different 827 of 2000 (expected more than 0)
func_8001139c_z2o control: different 2000 of 2000 (expected more than 0)
func_80010ae8_z2o control: different 2000 of 2000 (expected more than 0)
func_80011708_z2o control: different 384 of 2000 (expected more than 0)
func_80011018_z2o control: different 2000 of 2000 (expected more than 0)
func_800108e8_z2o control: different 181 of 2000 (expected more than 0)
func_80010c38_z2o control: different 2000 of 2000 (expected more than 0)
func_800110d0_z2o control: different 955 of 2000 (expected more than 0)
func_80011afc_z2o control: different 2000 of 2000 (expected more than 0)
func_800116d8_z2o control: different 2000 of 2000 (expected more than 0)
func_800116e8_z2o control: different 2000 of 2000 (expected more than 0)
func_800116f8_z2o control: different 2000 of 2000 (expected more than 0)
func_800119c8_z2o control: different 2000 of 2000 (expected more than 0)
func_800119d8_z2o control: different 2000 of 2000 (expected more than 0)
func_80011458_z2o control: different 614 of 2000 (expected more than 0)
func_80011530_z2o control: different 676 of 2000 (expected more than 0)
func_80011478_z2o control: different 1895 of 2000 (expected more than 0)
```

The write audit (`--writes`, seed 1, then `--seed 7`):

```
func_80010be4_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010be4_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011428_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011428_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800116c8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800116c8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011810_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011810_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800119b8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800119b8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800119e8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800119e8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800115a8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800115a8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011630_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011630_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011830_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011830_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800118f0_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800118f0_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011a24_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011a24_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8001139c_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8001139c_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010ae8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010ae8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011708_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011708_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011018_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011018_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800108e8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800108e8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010c38_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010c38_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800110d0_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800110d0_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011afc_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011afc_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800116d8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800116d8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800116e8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800116e8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800116f8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800116f8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800119c8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800119c8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800119d8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800119d8_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011458_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011458_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011530_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011530_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011478_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011478_z2o writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
```

The record (`--record`) and the replay (`--replay`):

```
func_80010be4_z2o fixtures: kept 1 of 2000 cases (slots 1, edges 0, results 0)
func_80011428_z2o fixtures: kept 1 of 2000 cases (slots 1, edges 0, results 0)
func_800116c8_z2o fixtures: kept 1 of 2000 cases (slots 1, edges 0, results 0)
func_80011810_z2o fixtures: kept 1 of 2000 cases (slots 1, edges 0, results 0)
func_800119b8_z2o fixtures: kept 1 of 2000 cases (slots 1, edges 0, results 0)
func_800119e8_z2o fixtures: kept 12 of 2000 cases (slots 1, edges 0, results 11)
func_800115a8_z2o fixtures: kept 2 of 2000 cases (slots 2, edges 0, results 0)
func_80011630_z2o fixtures: kept 2 of 2000 cases (slots 2, edges 0, results 0)
func_80011830_z2o fixtures: kept 3 of 2000 cases (slots 2, edges 1, results 0)
func_800118f0_z2o fixtures: kept 3 of 2000 cases (slots 3, edges 0, results 0)
func_80011a24_z2o fixtures: kept 2 of 2000 cases (slots 2, edges 0, results 0)
func_8001139c_z2o fixtures: kept 1 of 2000 cases (slots 1, edges 0, results 0)
func_80010ae8_z2o fixtures: kept 1 of 2000 cases (slots 1, edges 0, results 0)
func_80011708_z2o fixtures: kept 3 of 2000 cases (slots 2, edges 1, results 0)
func_80011018_z2o fixtures: kept 12 of 2000 cases (slots 1, edges 0, results 11)
func_800108e8_z2o fixtures: kept 2 of 2000 cases (slots 1, edges 1, results 0)
func_80010c38_z2o fixtures: kept 5 of 2000 cases (slots 1, edges 4, results 0)
func_800110d0_z2o fixtures: kept 7 of 2000 cases (slots 4, edges 3, results 0)
func_80011afc_z2o fixtures: kept 1 of 2000 cases (slots 1, edges 0, results 0)
func_800116d8_z2o fixtures: kept 12 of 2000 cases (slots 1, edges 0, results 11)
func_800116e8_z2o fixtures: kept 12 of 2000 cases (slots 1, edges 0, results 11)
func_800116f8_z2o fixtures: kept 12 of 2000 cases (slots 1, edges 0, results 11)
func_800119c8_z2o fixtures: kept 1 of 2000 cases (slots 1, edges 0, results 0)
func_800119d8_z2o fixtures: kept 1 of 2000 cases (slots 1, edges 0, results 0)
func_80011458_z2o fixtures: kept 2 of 2000 cases (slots 1, edges 0, results 1)
func_80011530_z2o fixtures: kept 3 of 2000 cases (slots 3, edges 0, results 0)
func_80011478_z2o fixtures: kept 2 of 2000 cases (slots 2, edges 0, results 0)
func_80010be4_z2o replay: fixtures 1, passed 1, failed 0
func_80011428_z2o replay: fixtures 1, passed 1, failed 0
func_800116c8_z2o replay: fixtures 1, passed 1, failed 0
func_80011810_z2o replay: fixtures 1, passed 1, failed 0
func_800119b8_z2o replay: fixtures 1, passed 1, failed 0
func_800119e8_z2o replay: fixtures 12, passed 12, failed 0
func_800115a8_z2o replay: fixtures 2, passed 2, failed 0
func_80011630_z2o replay: fixtures 2, passed 2, failed 0
func_80011830_z2o replay: fixtures 3, passed 3, failed 0
func_800118f0_z2o replay: fixtures 3, passed 3, failed 0
func_80011a24_z2o replay: fixtures 2, passed 2, failed 0
func_8001139c_z2o replay: fixtures 1, passed 1, failed 0
func_80010ae8_z2o replay: fixtures 1, passed 1, failed 0
func_80011708_z2o replay: fixtures 3, passed 3, failed 0
func_80011018_z2o replay: fixtures 12, passed 12, failed 0
func_800108e8_z2o replay: fixtures 2, passed 2, failed 0
func_80010c38_z2o replay: fixtures 5, passed 5, failed 0
func_800110d0_z2o replay: fixtures 7, passed 7, failed 0
func_80011afc_z2o replay: fixtures 1, passed 1, failed 0
func_800116d8_z2o replay: fixtures 12, passed 12, failed 0
func_800116e8_z2o replay: fixtures 12, passed 12, failed 0
func_800116f8_z2o replay: fixtures 12, passed 12, failed 0
func_800119c8_z2o replay: fixtures 1, passed 1, failed 0
func_800119d8_z2o replay: fixtures 1, passed 1, failed 0
func_80011458_z2o replay: fixtures 2, passed 2, failed 0
func_80011530_z2o replay: fixtures 3, passed 3, failed 0
func_80011478_z2o replay: fixtures 2, passed 2, failed 0
```

## The functions

All twenty-seven are inferred roles, not original names. None uses the global pointer
register (see above).

- `func_800116c8_z2o`: stores the low byte of its argument at the byte
  `0x8002f8a9` (inferred: a channel number).
- `func_800119b8_z2o`: stores its argument at the word `0x8002f870` (inferred:
  a parameter word).
- `func_80011810_z2o`: adds 1 to the word `0x8002f86c` (inferred: the count of
  completed frames, from the routine's use as the stream's frame routine).
- `func_80011428_z2o`: waits while the flag word at `0x8002f890` is not 0, then
  copies the word at `0x8002f81c` to the word at `0x8002f818` (inferred: waits
  until the decoder's strips are done). The flag is `volatile` in `z2o.h`. As
  with the logo's wait, the test sets the flag to 0 in every case (with another
  value the function ends only through an interrupt, which the tool does not
  model), so it shows the exit, the read and the copy, not the waiting.
- `func_800119e8_z2o`: the lookup in the program's table of 12 byte entries at
  `0x8001b354` (inferred: the disc's files). Returns the first word of the
  entry and stores the second word through the pointer. The table is named
  `data_8001b354_z2o` in `programs.toml`.
- `func_80010be4_z2o`: hands the display environment named by the index at
  `0x80031e58` (20 bytes each, from `0x80049eac`) to the library call
  `func_80158470` (the program's copy, at `0x80013654`), then flips the index.
  The call is a recorder in the test; the index is 0 or 1 in four cases of
  five.
- `func_800115a8_z2o`: sets the volume (inferred: the disc's). Stores its argument in the word `0x8001b34c`, reads the flag word at `0x8001b350` once, and sets the four bytes at `0x8002f8a4` to (arg, 0, arg, 0) when the flag is not 0, else all four to the argument shifted right by one (logical), then calls CdMix with them. CdMix is a recorder in the test.
- `func_80011630_z2o`: sets stereo or mono (inferred). Stores its argument in the flag word at `0x8001b350`; when it is not 0 the four bytes at `0x8002f8a4` become (v, 0, v, 0) with v the low byte of the volume word at `0x8001b34c` (a byte load), else all four become the volume word shifted right by one (logical); then calls CdMix with them. CdMix is a recorder in the test.
- `func_80011830_z2o`: brings the disc's volume up in steps (inferred). Reads the flag word at `0x8001b350` once. For i = 0, 2, 4, ... (the first step always runs) while i is not above the volume word at `0x8001b34c` (an unsigned comparison): sets the four bytes at `0x8002f8a4` to (i, 0, i, 0) when the flag is not 0, else all four to i / 2 (signed), then calls CdMix with them. CdMix is a recorder in the test. The test keeps the volume word small (up to 300), since a large one would run for billions of steps.
- `func_800118f0_z2o`: brings the disc's volume down in steps (inferred). Reads the flag word at `0x8001b350` once and the volume word at `0x8001b34c` once into a counter v. While v is 0 or more (signed; nothing runs for a negative v): sets the four bytes at `0x8002f8a4` to (v, 0, v, 0) when the flag is not 0, else all four to v / 2 (signed), calls CdMix with them, then subtracts 2 from v. CdMix is a recorder in the test. The volume word is not written.
- `func_80011a24_z2o`: starts the disc (inferred). Calls CdInit up to 4 times until one call returns a value that is not 0 and returns 0 if none does. Otherwise calls CdControlB(0x0e, &mode, 0) (a local mode byte of 0x80) until it returns a value that is not 0, CdIntToPos(0x10, pos) (a local position), CdControlB(0x15, pos, 0) until it returns a value that is not 0, and returns 1. The three library routines are recorders in the test with scripted results; the recorder of CdIntToPos fills the position with a word of the setup. It reads and writes no memory of the program.
- `func_8001139c_z2o`: ends the movie (inferred). Calls `func_800118f0_z2o` (the volume down), StUnSetRing(), CdReady(0, 0), CdGetSector(p, 0x200) with p the pointer word at `0x8002f804`, stores 0x80 in the byte at `0x8002f800`, calls CdControlB(0x0e, that byte, 0) until it returns a value that is not 0, then CdControlB(9, 0, 0) until it returns a value that is not 0, then DecDCToutCallback(0). All of them are recorders in the test; CdControlB has scripted results.
- `func_80010ae8_z2o`: sets up the graphics and the two display environments (inferred; 20 bytes each from `0x80049eac`). Calls ResetGraph(0), SetDispMask(0), CdReadCallback(0), SetDefDispEnv for the first environment (0, 0, 320, 240) and the second (0, 240, 320, 240), sets isrgb24 of both to 1 and the screen width of both to 0xfa, stores 0 in the index word at `0x80031e58`, makes the rectangle (0, 0, 0x3c0, 0x1e0) in a local for ClearImage, then DrawSync(0), VSync(0), `func_80158470` (the program's copy, with the second environment) and SetDispMask(1). All the library routines are recorders in the test; the pointer to the local rectangle is masked and the two words behind it logged.

- `func_80011708_z2o`: the decoder's completion routine (inferred). Adds 1 to the count word at `0x8002f888`; if the flag word at `0x80031e68` is not 0 calls StCdInterrupt and clears it; if the word at `0x8002f870` is 0 calls LoadImage (the game's name `func_80157fc4`, the program's copy at `0x800131a8`) with the rectangle at `0x8002f874` and the buffer pointer at `0x8002f87c`. While the count is below the limit word at `0x8002f88c` (unsigned) it adds 0x18 (when the word at `0x8002f818` is not 0, else 0x10) to the rectangle's x, adds the word at `0x8002f880` to the buffer pointer (in words), calls DecDCTout with it and the word at `0x8002f884`, and sets the word at `0x8002f890` to 1; otherwise sets that word to 0. StCdInterrupt, LoadImage and DecDCTout are recorders in the test; the rectangle and the words up to `0x8002f890`, and the flag word, are watched.

- `func_80011018_z2o`: opens a movie by its index (inferred). Calls `func_800119e8_z2o(index, &size)` (size is a local the callee fills, never read) and passes its result as the first argument of `func_80010c38_z2o` (the open of a movie; inferred) together with its own other nine arguments unchanged (a1 to a3 and six words from the caller's stack), and returns what that returns. Both callees are recorders in the test; the second takes ten arguments, the setup writes the six stack words. The prototype of `func_80010c38_z2o` in `z2o.h` is the author's, from the use of the argument registers in its listing; the symbol is in `[program.symbols]` until the branch that has its C declares it as a function.

- `func_800108e8_z2o`: the program's main (inferred). Calls the empty library function, ResetCallback, ResetGraph(0), the disc start, PadInit(0), InitGeom and the graphics set-up; stores 0 in the word at `0x80031e6c`; sets the parameter word, the volume (0x80), stereo (1) and the channel (1); opens the movie with `func_80011018_z2o` (ten arguments). Then a loop: the frame routine `func_800110d0_z2o(0, y)` (y 0xf0 when the index word at `0x80031e58` is not 0), the wait, VSync(0), the flip, PadRead(1); stores the pad word at `0x80031e70`, the buttons newly pressed (against the word at `0x80031e6c`) at `0x80031e74`, their low half at `0x8003de88` and their high half at `0x80049e8c`; when a bit of 0x820 is set in either half it stores 1 at the scratchpad word `0x1f80000c` and leaves the loop; otherwise it goes on while the frame routine returned 0. Then `func_8001139c_z2o`, twice the rectangle at `0x80049ea4` set to (0, y, 0x1e0, 0xf0), ClearImage, DrawSync(0), VSync(0) and the flip, then PadStop and StopCallback. All callees are recorders in the test, with the results of the frame routine and PadRead scripted per call; the flip's recorder stores the opposite truth value into the index word at each call (a model by the contract's author). The pad words, the index word, the two halves, the rectangle and the scratchpad word are watched. The prototypes of `func_800110d0_z2o` and `func_80010c38_z2o` in `z2o.h` are the author's, from the use of the argument registers in their listings; their symbols are in `[program.symbols]` until the branch that has their C declares them as functions.
- `func_80010c38_z2o`: opens the movie (inferred). Ten arguments (sector, stream, frames, ring, ring size, two bitstream buffers, the strip buffer, a mode word, a word count; the last six on the caller's stack). Stores them in the program's words (`0x8002f804` to `0x8002f81c`, `0x8001b348`) and clears four more, then DecDCTReset(0), DecDCToutCallback(`func_80011708_z2o`), StSetRing, StSetStream(1, stream, 0x0fffffff, `func_80011810_z2o`, 0), `func_800118f0_z2o`, CdIntToPos(sector, `0x8002f8ac`), CdControl(0x0d, filter) and CdControl(2, position) each until it returns a value that is not 0, CdControlB(0x0e, &mode) until it returns a value that is not 0 (mode is 0x1e8 or 0x168 by the word at `0x8001b348`), VSync(0) 51 times, CdRead2(mode) until it returns a value that is not 0 (CdControl(2, position) after each 0), CdControlB(1, 0, `0x8002f8c4`) until bit 0x20 of the first result byte is set, `func_80011830_z2o`, StGetNext until it returns 0, copies the 32 byte header to `0x8002f84c` with its two halfwords at +0x10 and +0x12 rounded to a multiple of 16, DecDCTvlc and StFreeRing. Returns 0. All library routines and the program's functions it calls are recorders in the test; `func_80011708_z2o` is only named (its address is in `programs.toml`, its C is written elsewhere). The recorders of CdControl and StGetNext also store words that an interrupt could (the word at `0x8001b348`, the index and count words) so that the function's re-reads show. Two instruction slots are unreached (the corrections for a negative sum in the rounding, which `lhu` values cannot make); the .c names them.
- `func_800110d0_z2o`: one frame (inferred); the listing has no wait and no flip. Arguments x and y (a0, a1). Calls StGetNext until it returns 0 (after 0x100000 returns that are not 0 it returns 1; the test does not reach that, see the .c). Then stores x and y in the halfwords at `0x8002f874` and `0x8002f876` (the first two fields of a rectangle, inferred), the previous frame number (word 2 of the header copy at `0x8002f84c`) in the word at `0x8002f894`, the two halfwords at `0x8002f85c` and `0x8002f85e` in the words at `0x8002f898` and `0x8002f89c`, subtracts 1 from the word at `0x8002f86c`, and fills the rest of the rectangle and four more words (width 0x18 or 0x10 by the word at `0x8002f818`, sizes from the second halfword times 12 or 8, the pointer from the word at `0x8002f80c`). Then DecDCTReset(1), DecDCTin(buffer i, the word at `0x8002f818`), DecDCTout(strip, size), flips the index word at `0x8002f820`, copies the 32 byte header to `0x8002f84c` with the two halfwords rounded to a multiple of 16, DecDCTvlc(data, buffer i), StFreeRing. Returns 1 when the previous frame number is not below the word at `0x8002f814` (unsigned), else 0. All the library routines are recorders in the test; the recorders of DecDCTReset and DecDCTin store words that an interrupt could change (the flag, the index, the strip pointer, the size) so that the function's re-reads show. Seven instruction slots are unreached (the time-out's two and five corrections for negative values that `lhu` values cannot make); the .c names them.
- `func_80011afc_z2o`: does nothing (inferred: the program's own copy of a library routine; the listing is a return and its delay slot). The main calls it first. The test gives the four argument registers random values, with the result register 0 at the start of every run as the tool sets it, and compares the result register and memory after the call (the bytes around a data word are random, so that a store would show); it does not compare the argument registers after the call.
- `func_800116d8_z2o`: returns the word at `0x8002f894` (inferred: the latest frame number that the frame routine stores). No call of it was found in the program. The test makes the word and its neighbours random and compares the result register and memory after the call.
- `func_800116e8_z2o`: returns the word at `0x8002f86c` (inferred: the count of completed frames that `func_80011810_z2o` adds 1 to). No call of it was found in the program. The test makes the word and its neighbours random and compares the result register and memory after the call.
- `func_800116f8_z2o`: returns the word at `0x8002f8cc`, just after the 8 result bytes at `0x8002f8c4` (inferred: a word of the program's data; its meaning is not settled). No call of it was found in the program. The test makes the word and its neighbours random and compares the result register and memory after the call.
- `func_800119c8_z2o`: stores its argument in the word at `0x8002f80c` (inferred: the pointer that the decoder's output goes to). No call of it was found in the program. The test gives the argument a random 32 bit word and the bytes around the word random values, and compares memory after the call (the function has no result, and the result register is not compared).
- `func_800119d8_z2o`: stores its argument in the word at `0x8002f81c` (inferred: the count of strips that `func_80011428_z2o` copies). No call of it was found in the program. The test gives the argument a random 32 bit word and the bytes around the word random values, and compares memory after the call (the function has no result, and the result register is not compared).
- `func_80011458_z2o`: a test of the movie's end (inferred). Returns 1 when the word at `0x8002f894` (the latest frame number) is not below the word at `0x8002f814` (the number of frames), compared as unsigned numbers, else 0. No call of it was found in the program. The test sets the two words equal, one apart either way, at 0 or 0xffffffff, or random, with random bytes around them, and compares the result register and memory after the call; the control (signed instead of unsigned) differs only where the two words have different top bits.
- `func_80011530_z2o`: pauses or resumes the disc (inferred). With the argument 1 it calls `func_800118f0_z2o` (the volume down), then `CdControlB(9, 0, 0)` until that returns a value that is not 0; with any other argument it takes the mode word 0x1e8 when the word at `0x8001b348` is not 0, else 0x168, and calls `CdRead2(mode)` until that returns a value that is not 0. It returns 0 in both cases. No call of it was found in the program. The test scripts the results of both library recorders (0 up to three times, then not 0), gives the argument 1 in half of the cases and the flag word 0 in a third of them, and compares the result register, the log of the recorders and memory after the call.
- `func_80011478_z2o`: draws bands (inferred). Arguments: a source pointer, x, y, a total, and on the stack a height and a wide flag. It builds a rectangle (x, y, a width of 0x18 when the flag is not 0 else 0x10, the height), takes the stride ((width times height) >> 1) << 2 and the count of bands total >> 4, and for each band calls `func_80157fc4(&rect, src)` (the game's name for LoadImage) and `DrawSync(0)`, then adds the width to the rectangle's x and the stride to the source pointer. No call of it was found in the program. The test keeps the count of bands to 8 or fewer, so the loop's unsigned comparison is not told from a signed one; the two library calls are recorders (the rectangle is logged), and the result is not compared (the function has none).
