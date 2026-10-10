# C for the logo's program

C for functions of the logo's program, the stand-alone executable `PAC/LOGO.EXE`
of the disc that the game starts with `Exec`. It is not the resident program
and not a module of an archive. The program is declared in
[`../programs.toml`](../programs.toml) (its name, the hash of the user's own
copy, where its body loads, the global pointer its entry code sets, the
functions that have C here, the names of its data);
[`../programs.py`](../programs.py) reads that file and
[`../discfile.py`](../discfile.py) takes the program's file out of the user's
disc image (the file is private and is never committed). The PS1 build and the
PC build do not see this folder. How the test works and what a pass does not
show are on the page of the tool's folder,
[`../../slot06_nonmatching/`](../../slot06_nonmatching/README.md): a pass is
evidence for the tested inputs of a function's contract, not equivalence.

Each function is a pair: `FUNC.c` with its contract in the header comment,
and `FUNC.py`, the contract as code. A function's name is
`func_ADDRESS_logo`, the address in eight hex digits.

## The program's file

Once, with the user's own disc image (the `.bin`, or the `.cue` that names it),
from `ps1/src/programs/`; `ps1/extract/` is a private folder that Git ignores:

    mkdir -p ../../extract/programs
    python discfile.py DISC PAC/LOGO.EXE ../../extract/programs/LOGO.EXE

It prints the size and the SHA-256, which must be the `sha256` in
`programs.toml`. The tool makes a new file and never writes over one: an
output path at which anything is (the disc image itself under any name
included) is refused, so to take the file out again remove the old copy first. The comparison, the write audit and the control need the file
(the original runs from it, over the resident executable); `--replay` does not.

## Testing one function

A function is tested alone. When one is added or changed, run its own five
runs, from the tool's folder, and at the same time (see
[efficient checks](../../../../docs/efficiency.md)):

    python difftest.py --config ../build.toml --folder ../programs/logo --cases 2000 FUNC
    python difftest.py --config ../build.toml --folder ../programs/logo --cases 2000 --seed 7 FUNC
    python difftest.py --config ../build.toml --folder ../programs/logo --cases 2000 --control FUNC
    python difftest.py --config ../build.toml --folder ../programs/logo --cases 2000 --writes FUNC
    python difftest.py --config ../build.toml --folder ../programs/logo --cases 2000 --writes --seed 7 FUNC

Then put that function's lines into the blocks below, in place of its old ones
or at its place in the order of names. No other function is run again for it.

The program's code reaches its small data through the global pointer
register. The test tool begins every run of a function of this program with
the register at the declared value, `gp` in `programs.toml` (0x8002eafc; the
reader checks it against the two instructions of the program's entry code when
it reads the file). The C here names such data by address, through
`[program.symbols]`, and does not use the register.

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

    python -B difftest.py --config ../build.toml --folder ../programs/logo --replay
    python -B difftest.py --config ../build.toml --folder ../programs/logo --replay FUNC

For a new or changed function, record once (the wide comparison, then the
fixtures it keeps), then replay. A record that prints a warning about bytes
that the setup did not make is not kept in the repository: the file would hold
bytes of the program.

    python -B difftest.py --config ../build.toml --folder ../programs/logo --cases 2000 --jobs 4 --record FUNC

A pass is evidence for the recorded inputs. It is not the wide comparison and
it is not equivalence. Details of the files are on the page of the tool's
folder.

The record of each function below printed no warning about bytes that the setup
did not make; the case counts of the fixtures files are in the record block.
The declarations of the program's data and of the library routines the C calls
are in [`logo.h`](logo.h).

## The recorded lines

The blocks are a record, not an instruction: what the commands printed for each
function when it was added or last changed. They were printed on 2026-10-10.

The test (`--cases 2000 FUNC`, seed 1, then `--seed 7`):

```
func_80010a38_logo: built 84 bytes, original 72 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010a38_logo coverage: 18 of 18 instruction slots of the original executed
func_80010a38_logo: built 84 bytes, original 72 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010a38_logo coverage: 18 of 18 instruction slots of the original executed
func_80010f20_logo: built 28 bytes, original 24 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010f20_logo coverage: 6 of 6 instruction slots of the original executed
func_80010f20_logo: built 28 bytes, original 24 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010f20_logo coverage: 6 of 6 instruction slots of the original executed
func_80010f38_logo: built 164 bytes, original 140 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010f38_logo coverage: 35 of 35 instruction slots of the original executed
func_80010f38_logo: built 164 bytes, original 140 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010f38_logo coverage: 35 of 35 instruction slots of the original executed
func_80011758_logo: built 60 bytes, original 60 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011758_logo coverage: 15 of 15 instruction slots of the original executed
func_80011758_logo: built 60 bytes, original 60 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011758_logo coverage: 15 of 15 instruction slots of the original executed
func_80011838_logo: built 8 bytes, original 8 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011838_logo coverage: 2 of 2 instruction slots of the original executed
func_80011838_logo: built 8 bytes, original 8 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011838_logo coverage: 2 of 2 instruction slots of the original executed
```

The negative control (`--control`):

```
func_80010a38_logo control: different 2000 of 2000 (expected more than 0)
func_80010f20_logo control: different 2000 of 2000 (expected more than 0)
func_80010f38_logo control: different 2000 of 2000 (expected more than 0)
func_80011758_logo control: different 2000 of 2000 (expected more than 0)
func_80011838_logo control: different 2000 of 2000 (expected more than 0)
```

The write audit (`--writes`, seed 1, then `--seed 7`):

```
func_80010a38_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010a38_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010f20_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010f20_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010f38_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010f38_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011758_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011758_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011838_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011838_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
```

The record (`--record`) and the replay (`--replay`):

```
func_80010a38_logo fixtures: kept 1 of 2000 cases (slots 1, edges 0, results 0)
func_80010a38_logo replay: fixtures 1, passed 1, failed 0
func_80010f20_logo fixtures: kept 1 of 2000 cases (slots 1, edges 0, results 0)
func_80010f20_logo replay: fixtures 1, passed 1, failed 0
func_80010f38_logo fixtures: kept 4 of 2000 cases (slots 2, edges 2, results 0)
func_80010f38_logo replay: fixtures 4, passed 4, failed 0
func_80011758_logo fixtures: kept 12 of 2000 cases (slots 1, edges 0, results 11)
func_80011758_logo replay: fixtures 12, passed 12, failed 0
func_80011838_logo fixtures: kept 1 of 2000 cases (slots 1, edges 0, results 0)
func_80011838_logo replay: fixtures 1, passed 1, failed 0
```

## The functions

- `func_80011758_logo`: the lookup in the program's table of the disc's files.
  Returns the first word of the entry at `0x8001a678 + 12 * index` and stores
  the second word through the pointer. The table is named `data_8001a678_logo`
  in `programs.toml`.
- `func_80011838_logo`: an empty function (a return and its delay slot).
  Its test also compares `v0`, so that the C is shown to leave it alone.
- `func_80010f20_logo`: waits while the flag word at `0x8002eb3c` is not 0. The
  flag is `volatile` in `logo.h`, so that the C reads it at every turn. The
  test sets the flag to 0 in every case (with another value the function ends
  only through an interrupt, which the tool does not model), so it shows the
  exit and the read, not the waiting.
- `func_80010a38_logo`: hands the display environment named by the index at
  `0x8002eb50` (20 bytes each, from `0x8002eb8c`) to the library call
  `func_80158470` (the program's copy), then flips the index. The call is a
  recorder in the test; the index is 0 or 1 in four cases of five.
- `func_80010f38_logo`: the movie decoder's output routine (inferred). Clears
  the word at `0x8002edb8` after `StCdInterrupt` when it is set, copies the
  strip to the picture (`func_80157fc4`, the program's `LoadImage`), moves the
  rectangle 24 pixels right and, while its x is below 480, asks for the next
  strip (`DecDCTout`) and sets the word at `0x8002eb3c` to 1, else to 0. The
  three callees are recorders in the test.
