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
register. The listing of each of the six functions here was read for that: no
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
func_80010be4_z2o replay: fixtures 1, passed 1, failed 0
func_80011428_z2o replay: fixtures 1, passed 1, failed 0
func_800116c8_z2o replay: fixtures 1, passed 1, failed 0
func_80011810_z2o replay: fixtures 1, passed 1, failed 0
func_800119b8_z2o replay: fixtures 1, passed 1, failed 0
func_800119e8_z2o replay: fixtures 12, passed 12, failed 0
func_800115a8_z2o replay: fixtures 2, passed 2, failed 0
```

## The functions

All six are inferred roles, not original names. None uses the global pointer
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
