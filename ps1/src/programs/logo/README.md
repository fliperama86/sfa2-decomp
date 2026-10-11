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
func_80010928_logo: built 284 bytes, original 272 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010928_logo coverage: 68 of 68 instruction slots of the original executed
func_80010928_logo: built 284 bytes, original 272 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010928_logo coverage: 68 of 68 instruction slots of the original executed
func_80010a38_logo: built 84 bytes, original 72 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010a38_logo coverage: 18 of 18 instruction slots of the original executed
func_80010a38_logo: built 84 bytes, original 72 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010a38_logo coverage: 18 of 18 instruction slots of the original executed
func_80010e28_logo: built 260 bytes, original 248 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010e28_logo coverage: 62 of 62 instruction slots of the original executed
func_80010e28_logo: built 260 bytes, original 248 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010e28_logo coverage: 62 of 62 instruction slots of the original executed
func_80010f20_logo: built 28 bytes, original 24 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010f20_logo coverage: 6 of 6 instruction slots of the original executed
func_80010f20_logo: built 28 bytes, original 24 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010f20_logo coverage: 6 of 6 instruction slots of the original executed
func_80010f38_logo: built 164 bytes, original 140 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010f38_logo coverage: 35 of 35 instruction slots of the original executed
func_80010f38_logo: built 164 bytes, original 140 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010f38_logo coverage: 35 of 35 instruction slots of the original executed
func_80010fc4_logo: built 176 bytes, original 156 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010fc4_logo coverage: 39 of 39 instruction slots of the original executed
func_80010fc4_logo: built 176 bytes, original 156 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80010fc4_logo coverage: 39 of 39 instruction slots of the original executed
func_80011060_logo: built 168 bytes, original 148 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011060_logo coverage: 37 of 37 instruction slots of the original executed
func_80011060_logo: built 168 bytes, original 148 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011060_logo coverage: 37 of 37 instruction slots of the original executed
func_800110f4_logo: built 220 bytes, original 220 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800110f4_logo coverage: 55 of 55 instruction slots of the original executed
func_800110f4_logo: built 220 bytes, original 220 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800110f4_logo coverage: 55 of 55 instruction slots of the original executed
func_800111d0_logo: built 280 bytes, original 240 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800111d0_logo coverage: 60 of 60 instruction slots of the original executed
func_800111d0_logo: built 280 bytes, original 240 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800111d0_logo coverage: 60 of 60 instruction slots of the original executed
func_80011758_logo: built 60 bytes, original 60 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011758_logo coverage: 15 of 15 instruction slots of the original executed
func_80011758_logo: built 60 bytes, original 60 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011758_logo coverage: 15 of 15 instruction slots of the original executed
func_80011794_logo: built 164 bytes, original 164 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011794_logo coverage: 41 of 41 instruction slots of the original executed
func_80011794_logo: built 164 bytes, original 164 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011794_logo coverage: 41 of 41 instruction slots of the original executed
func_80011838_logo: built 8 bytes, original 8 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011838_logo coverage: 2 of 2 instruction slots of the original executed
func_80011838_logo: built 8 bytes, original 8 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011838_logo coverage: 2 of 2 instruction slots of the original executed
```

The negative control (`--control`):

```
func_80010928_logo control: different 2000 of 2000 (expected more than 0)
func_80010a38_logo control: different 2000 of 2000 (expected more than 0)
func_80010e28_logo control: different 2000 of 2000 (expected more than 0)
func_80010f20_logo control: different 2000 of 2000 (expected more than 0)
func_80010f38_logo control: different 2000 of 2000 (expected more than 0)
func_80010fc4_logo control: different 991 of 2000 (expected more than 0)
func_80011060_logo control: different 1001 of 2000 (expected more than 0)
func_800110f4_logo control: different 1463 of 2000 (expected more than 0)
func_800111d0_logo control: different 2000 of 2000 (expected more than 0)
func_80011758_logo control: different 2000 of 2000 (expected more than 0)
func_80011794_logo control: different 745 of 2000 (expected more than 0)
func_80011838_logo control: different 2000 of 2000 (expected more than 0)
```

The write audit (`--writes`, seed 1, then `--seed 7`):

```
func_80010928_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010928_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010a38_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010a38_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010e28_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010e28_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010f20_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010f20_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010f38_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010f38_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010fc4_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80010fc4_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011060_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011060_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800110f4_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800110f4_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800111d0_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800111d0_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011758_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011758_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011794_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011794_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011838_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011838_logo writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
```

The record (`--record`) and the replay (`--replay`):

```
func_80010928_logo fixtures: kept 1 of 2000 cases (slots 1, edges 0, results 0)
func_80010928_logo replay: fixtures 1, passed 1, failed 0
func_80010a38_logo fixtures: kept 1 of 2000 cases (slots 1, edges 0, results 0)
func_80010a38_logo replay: fixtures 1, passed 1, failed 0
func_80010e28_logo fixtures: kept 1 of 2000 cases (slots 1, edges 0, results 0)
func_80010e28_logo replay: fixtures 1, passed 1, failed 0
func_80010f20_logo fixtures: kept 1 of 2000 cases (slots 1, edges 0, results 0)
func_80010f20_logo replay: fixtures 1, passed 1, failed 0
func_80010f38_logo fixtures: kept 4 of 2000 cases (slots 2, edges 2, results 0)
func_80010f38_logo replay: fixtures 4, passed 4, failed 0
func_80010fc4_logo fixtures: kept 2 of 2000 cases (slots 2, edges 0, results 0)
func_80010fc4_logo replay: fixtures 2, passed 2, failed 0
func_80011060_logo fixtures: kept 2 of 2000 cases (slots 2, edges 0, results 0)
func_80011060_logo replay: fixtures 2, passed 2, failed 0
func_800110f4_logo fixtures: kept 3 of 2000 cases (slots 1, edges 2, results 0)
func_800110f4_logo replay: fixtures 3, passed 3, failed 0
func_800111d0_logo fixtures: kept 2 of 2000 cases (slots 1, edges 1, results 0)
func_800111d0_logo replay: fixtures 2, passed 2, failed 0
func_80011758_logo fixtures: kept 12 of 2000 cases (slots 1, edges 0, results 11)
func_80011758_logo replay: fixtures 12, passed 12, failed 0
func_80011794_logo fixtures: kept 2 of 2000 cases (slots 2, edges 0, results 0)
func_80011794_logo replay: fixtures 2, passed 2, failed 0
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
  strip to the picture (`func_80157fc4`, the program's `LoadImage`), adds 0x18
  to the rectangle's x (24 words of video memory) and, while that x is below
  0x1e0, asks for the next
  strip (`DecDCTout`) and sets the word at `0x8002eb3c` to 1, else to 0. The
  three callees are recorders in the test.
- `func_800110f4_logo`: expands run-length data (the format is in the header of
  the `.c`, read from the listing): a table of three-byte entries (a0) and a
  stream of 16-bit codes ended by `0xffff` (a1); a code with bit 15 clear adds
  a table entry to the running color and writes it, a code with bit 15 set
  writes the running color that many times. The output pointer is the word at
  `0x8002eb10`. The test's streams have at most 12 codes of at most 6 repeats.
- `func_80010fc4_logo`: brings the disc's volume up (inferred). Reads the word
  at `0x8002eb34` once; for i = 0, 2, ... 128 sets the four volume bytes at
  `0x8002eb40` to i, 0, i, 0 when the word is not 0, else all four to i / 2,
  and calls `CdMix` (the program's copy) on them at every step. `CdMix` is a
  recorder in the test; the log holds the four bytes at each call.
- `func_80011060_logo`: brings the disc's volume down (inferred), the mirror of
  `func_80010fc4_logo`: the same word and the same four bytes, for
  i = 128, 126, ... 0, and `CdMix` at every step (a recorder in the test).
- `func_800111d0_logo`: draws a still picture at a place (x, y) (inferred).
  Fills 0x500 * 12 bytes at the pointer in the word at `0x8002eb14` with three
  words read from the pointer in `0x8002eb10` (offsets 0, 0x10, 0x20), then
  sets the rectangle at `0x8002eb24` to (x, y, 0x1e0, 0x10) and calls
  `func_80157fc4` (the program's `LoadImage`) and `DrawSync` fifteen times,
  moving the rectangle's y by 0x10 each time, then sets it to
  (x + 0x48, y + 0x60, 0x150, 0x30) and calls `func_80157fc4` once more with
  the first pointer. The two callees are recorders in the test; the
  fixtures file is the largest of the folder because it holds the filled
  block.
- `func_80010928_logo`: sets up the graphics and the two display environments
  (inferred). Calls `ResetGraph(0)`, `SetDispMask(0)`, `SetVideoMode` with
  the video mode word at `0x8002eb38`, `SetDefDispEnv` for the two environments at
  `0x8002eb8c` (y 0 and y 240), sets their `isrgb24` bytes to 1 and the word at
  `0x8002eb50` to 0, and, when the video mode word is not 0, the screen x
  and y of both to 0 and 0x18. Then `ClearImage` of a local rectangle,
  `DrawSync(0)`, `VSync(0)`, `func_80158470` on the second environment and
  `SetDispMask(1)`. The eight callees are recorders in the test; the local
  rectangle's address is part of the log, so the test also holds the local
  at the address the original places it at.
- `func_80011794_logo`: starts the disc (inferred). Calls `CdInit` up to four
  times until it returns a value that is not 0 (after four returns of 0 it
  returns 0 and does nothing else); then `CdControlB(0x0e, &mode, 0)` with a
  local mode byte of 0x80 until it returns a value that is not 0,
  `CdIntToPos(0x10, pos)` into a local and `CdControlB(0x15, pos, 0)` until it
  returns a value that is not 0, and returns 1. The three callees are
  recorders in the test, their results scripted per call (failures, then a
  success); `CdIntToPos` has a fill that stores a word of the setup's through
  its second argument, the model of what the library writes to the position,
  and the words behind the two local pointers of `CdControlB` are logged.
- `func_80010e28_logo`: ends the movie (inferred). Calls `func_80011060_logo`
  (the volume down), `StUnSetRing`, `CdReady(0, 0)` and `CdGetSector` with the
  pointer in the word at `0x8002eb08` and 0x200; stores the byte 0x80 at
  `0x8002eafc` and sends `CdControlB(0x0e, 0x8002eafc, 0)` until it returns a
  value that is not 0, then `CdControlB(9, 0, 0)` the same way, and
  `DecDCToutCallback(0)`. Then twice: a local rectangle (0, y, 0x1e0, 0xf0)
  with y 0xf0 when the index word at `0x8002eb50` is not 0 and else 0, for
  `ClearImage`, then `DrawSync(0)`, `VSync(0)` and `func_80010a38_logo` (the
  flip). All callees are recorders in the test, `CdControlB` with its results
  scripted for both loops; the recorder of the flip stores a new value in the
  index word, so that the second turn reads another value than the first.
