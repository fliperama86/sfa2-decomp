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

- The cases, or a callee they run, read bytes that the setup did not make. Those bytes come from the game's memory image, so a file for the function would publish them and none is kept. `func_80012714_slot01`, `func_800e0a1c_slot0f`, `func_800e80ec_slot0f`, `func_801b1934_slot04_02`, `func_801b0904_slot04_04`.
- An original callee takes a pointer to a local of the function (the record refuses it: this is a limit of the tool, because the frames of the original and of the build differ). `func_801b3564_slot04_06`, `func_801cb564_slot05_06`.

What the replay command of the folder printed on 2026-10-10, one line per function:

```
func_80011688_slot12 replay: fixtures 2, passed 2, failed 0
func_8001188c_slot27 replay: fixtures 1, passed 1, failed 0
func_80012990_slot01 replay: fixtures 2, passed 2, failed 0
func_80012dec_slot01 replay: fixtures 2, passed 2, failed 0
func_8001389c_slot28 replay: fixtures 1, passed 1, failed 0
func_800145f4_slot28 replay: fixtures 2, passed 2, failed 0
func_8001606c_slot12 replay: fixtures 2, passed 2, failed 0
func_80077868_slot2b replay: fixtures 3, passed 3, failed 0
func_80078628_slot00 replay: fixtures 2, passed 2, failed 0
func_80079144_slot2b replay: fixtures 3, passed 3, failed 0
func_800e0850_slot0f replay: fixtures 9, passed 9, failed 0
func_800e0b48_slot0f replay: fixtures 4, passed 4, failed 0
func_800e4ab0_slot0f replay: fixtures 3, passed 3, failed 0
func_801b2f1c_slot04_0a replay: fixtures 5, passed 5, failed 0
func_801b36f8_slot04_09 replay: fixtures 8, passed 8, failed 0
func_801b3f38_slot04_sel replay: fixtures 3, passed 3, failed 0
func_801e0a78_slot0b replay: fixtures 2, passed 2, failed 0
func_801e0ce8_slot0b replay: fixtures 2, passed 2, failed 0
func_801e0ea8_slot0b replay: fixtures 2, passed 2, failed 0
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
func_80011688_slot12: built 260 bytes, original 260 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011688_slot12 coverage: 65 of 65 instruction slots of the original executed
func_8001188c_slot27: built 176 bytes, original 236 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8001188c_slot27 coverage: 59 of 59 instruction slots of the original executed
func_80012714_slot01: built 568 bytes, original 636 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80012714_slot01 coverage: 159 of 159 instruction slots of the original executed
func_80012990_slot01: built 312 bytes, original 448 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80012990_slot01 coverage: 112 of 112 instruction slots of the original executed
func_80012dec_slot01: built 320 bytes, original 372 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80012dec_slot01 coverage: 93 of 93 instruction slots of the original executed
func_8001389c_slot28: built 140 bytes, original 144 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8001389c_slot28 coverage: 36 of 36 instruction slots of the original executed
func_800145f4_slot28: built 132 bytes, original 132 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800145f4_slot28 coverage: 33 of 33 instruction slots of the original executed
func_8001606c_slot12: built 352 bytes, original 384 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8001606c_slot12 coverage: 96 of 96 instruction slots of the original executed
func_80077868_slot2b: built 156 bytes, original 160 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80077868_slot2b coverage: 40 of 40 instruction slots of the original executed
func_80078628_slot00: built 136 bytes, original 132 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80078628_slot00 coverage: 33 of 33 instruction slots of the original executed
func_80079144_slot2b: built 172 bytes, original 196 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80079144_slot2b coverage: 49 of 49 instruction slots of the original executed
func_800e0850_slot0f: built 412 bytes, original 460 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800e0850_slot0f coverage: 115 of 115 instruction slots of the original executed
func_800e0a1c_slot0f: built 276 bytes, original 300 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800e0a1c_slot0f coverage: 75 of 75 instruction slots of the original executed
func_800e0b48_slot0f: built 168 bytes, original 172 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800e0b48_slot0f coverage: 43 of 43 instruction slots of the original executed
func_800e4ab0_slot0f: built 540 bytes, original 608 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800e4ab0_slot0f coverage: 152 of 152 instruction slots of the original executed
func_800e80ec_slot0f: built 368 bytes, original 384 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800e80ec_slot0f coverage: 96 of 96 instruction slots of the original executed
func_801b0904_slot04_04: built 164 bytes, original 168 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b0904_slot04_04 coverage: 42 of 42 instruction slots of the original executed
func_801b1934_slot04_02: built 260 bytes, original 264 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b1934_slot04_02 coverage: 66 of 66 instruction slots of the original executed
func_801b2f1c_slot04_0a: built 304 bytes, original 312 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b2f1c_slot04_0a coverage: 78 of 78 instruction slots of the original executed
func_801b3564_slot04_06: built 728 bytes, original 876 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b3564_slot04_06 coverage: 215 of 219 instruction slots of the original executed
func_801b36f8_slot04_09: built 288 bytes, original 312 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b36f8_slot04_09 coverage: 78 of 78 instruction slots of the original executed
func_801b3f38_slot04_sel: built 328 bytes, original 340 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b3f38_slot04_sel coverage: 85 of 85 instruction slots of the original executed
func_801cb564_slot05_06: built 756 bytes, original 872 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801cb564_slot05_06 coverage: 214 of 218 instruction slots of the original executed
func_801e0a78_slot0b: built 556 bytes, original 624 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e0a78_slot0b coverage: 156 of 156 instruction slots of the original executed
func_801e0ce8_slot0b: built 304 bytes, original 448 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e0ce8_slot0b coverage: 112 of 112 instruction slots of the original executed
func_801e0ea8_slot0b: built 320 bytes, original 372 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e0ea8_slot0b coverage: 93 of 93 instruction slots of the original executed
```

The control (`--cases 2000 --control FUNC`):

```
func_80011688_slot12 control: different 1906 of 2000 (expected more than 0)
  altered: last channel step 2 changed to 3, instruction slot 24
func_8001188c_slot27 control: different 2000 of 2000 (expected more than 0)
  altered: last halfword store of the loop moved by two bytes, instruction slot 33
func_80012714_slot01 control: different 1323 of 2000 (expected more than 0)
  altered: a shift right by 4 changed to 3, instruction slot 40
func_80012990_slot01 control: different 2000 of 2000 (expected more than 0)
  altered: a byte store of the primitive moved by one byte, instruction slot 24
func_80012dec_slot01 control: different 2000 of 2000 (expected more than 0)
  altered: a shift left by 4 changed to 3, instruction slot 37
func_8001389c_slot28 control: different 2000 of 2000 (expected more than 0)
  altered: store of field_ad moved by one byte, instruction slot 29
func_800145f4_slot28 control: different 1193 of 2000 (expected more than 0)
  altered: store of field_60 moved by two bytes, instruction slot 11
func_8001606c_slot12 control: different 1012 of 2000 (expected more than 0)
  altered: address of the second list moved by one byte, instruction slot 49
func_80077868_slot2b control: different 824 of 2000 (expected more than 0)
  altered: base 0xc changed to 0xd, instruction slot 24
func_80078628_slot00 control: different 1348 of 2000 (expected more than 0)
  altered: field_06 increment changed from 1 to 2, instruction slot 26
func_80079144_slot2b control: different 1020 of 2000 (expected more than 0)
  altered: negation turned into a doubling, instruction slot 31
func_800e0850_slot0f control: different 585 of 2000 (expected more than 0)
  altered: write length changed to 0x2001, instruction slot 84
func_800e0a1c_slot0f control: different 314 of 2000 (expected more than 0)
  altered: a returned 2 becomes 3, instruction slot 61
func_800e0b48_slot0f control: different 678 of 2000 (expected more than 0)
  altered: mode 0x10 becomes 0x11, instruction slot 15
func_800e4ab0_slot0f control: different 649 of 2000 (expected more than 0)
  altered: y store moved to the x offset, instruction slot 83
func_800e80ec_slot0f control: different 2000 of 2000 (expected more than 0)
  altered: increment by 8 changed to 9, instruction slot 81
func_801b0904_slot04_04 control: different 563 of 2000 (expected more than 0)
  altered: sequence constant 0x1d changed to 0x1e, instruction slot 26
func_801b1934_slot04_02 control: different 2000 of 2000 (expected more than 0)
  altered: store of the second table word moved from field_50 to field_5c, instruction slot 39
func_801b2f1c_slot04_0a control: different 1003 of 2000 (expected more than 0)
  altered: field_50 constant 0x38000 changed to 0x48000, instruction slot 51
func_801b3564_slot04_06 control: different 80 of 2000 (expected more than 0)
  altered: knock-back distance 0x60 changed to 0x61, instruction slot 174
func_801b36f8_slot04_09 control: different 1020 of 2000 (expected more than 0)
  altered: sequence 0x33 changed to 0x34, instruction slot 42
func_801b3f38_slot04_sel control: different 1101 of 2000 (expected more than 0)
  altered: increment of field_4e changed to 2, instruction slot 56
func_801cb564_slot05_06 control: different 419 of 2000 (expected more than 0)
  altered: field_4c constant 0xa0000 changed to 0xb0000, instruction slot 86
func_801e0a78_slot0b control: different 2000 of 2000 (expected more than 0)
  altered: first +16 changed to +17, instruction slot 93
func_801e0ce8_slot0b control: different 1997 of 2000 (expected more than 0)
  altered: id shift changed from 6 to 5, instruction slot 54
func_801e0ea8_slot0b control: different 2000 of 2000 (expected more than 0)
  altered: first corner store moved by 0x10 bytes, instruction slot 37
```

The write audit (`--cases 2000 --writes FUNC`, seed 1):

```
func_80011688_slot12 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8001188c_slot27 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80012714_slot01 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80012990_slot01 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80012dec_slot01 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8001389c_slot28 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800145f4_slot28 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8001606c_slot12 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80077868_slot2b writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80078628_slot00 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80079144_slot2b writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e0850_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e0a1c_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e0b48_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e4ab0_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e80ec_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b0904_slot04_04 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b1934_slot04_02 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b2f1c_slot04_0a writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b3564_slot04_06 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b36f8_slot04_09 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b3f38_slot04_sel writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801cb564_slot05_06 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e0a78_slot0b writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e0ce8_slot0b writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e0ea8_slot0b writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
```

A function with fewer slots executed than it has names the others in its
header comment, with the reason why no input reaches them. The third
block is the write audit: in how many cases the original changed memory
that the function's setup did not make.
