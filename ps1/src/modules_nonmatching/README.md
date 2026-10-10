# Nonmatching C: the other modules

C for functions of the overlay modules that have no folder of their own
here (every module but the stage modules of
[`../slot06_nonmatching/`](../slot06_nonmatching/README.md) and the
character modules of [`../slot04b_nonmatching/`](../slot04b_nonmatching/README.md))
that compiles with the pinned toolchain and does not reproduce the
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

What these commands printed on 2026-10-10, run from the tool's folder:

    python difftest.py --config ../build.toml --folder ../modules_nonmatching --cases 2000 --all

```
func_800108d4_slot01: built 704 bytes, original 796 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800108d4_slot01 coverage: 199 of 199 instruction slots of the original executed
func_8001188c_slot27: built 176 bytes, original 236 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8001188c_slot27 coverage: 59 of 59 instruction slots of the original executed
func_80011a14_slot01: built 760 bytes, original 748 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011a14_slot01 coverage: 187 of 187 instruction slots of the original executed
func_80012714_slot01: built 568 bytes, original 636 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80012714_slot01 coverage: 159 of 159 instruction slots of the original executed
func_80012990_slot01: built 312 bytes, original 448 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80012990_slot01 coverage: 112 of 112 instruction slots of the original executed
func_80012dec_slot01: built 320 bytes, original 372 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80012dec_slot01 coverage: 93 of 93 instruction slots of the original executed
func_8001791c_slot27: built 612 bytes, original 680 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8001791c_slot27 coverage: 169 of 170 instruction slots of the original executed
func_80078628_slot00: built 136 bytes, original 132 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80078628_slot00 coverage: 33 of 33 instruction slots of the original executed
func_800e0b48_slot0f: built 168 bytes, original 172 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800e0b48_slot0f coverage: 43 of 43 instruction slots of the original executed
func_800e0f2c_slot0f: built 148 bytes, original 232 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800e0f2c_slot0f coverage: 58 of 58 instruction slots of the original executed
func_800e3478_slot0f: built 748 bytes, original 796 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800e3478_slot0f coverage: 199 of 199 instruction slots of the original executed
func_800e4ab0_slot0f: built 540 bytes, original 608 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800e4ab0_slot0f coverage: 152 of 152 instruction slots of the original executed
func_800e68e8_slot0f: built 308 bytes, original 352 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800e68e8_slot0f coverage: 88 of 88 instruction slots of the original executed
func_800e80ec_slot0f: built 368 bytes, original 384 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800e80ec_slot0f coverage: 96 of 96 instruction slots of the original executed
```

    python difftest.py --config ../build.toml --folder ../modules_nonmatching --cases 2000 --control --all

```
func_800108d4_slot01 control: different 809 of 2000 (expected more than 0)
  altered: constant 0xe0 changed to 0xe1, instruction slot 16
func_8001188c_slot27 control: different 2000 of 2000 (expected more than 0)
  altered: last halfword store of the loop moved by two bytes, instruction slot 33
func_80011a14_slot01 control: different 649 of 2000 (expected more than 0)
  altered: constant 0xa0 changed to 0xa1, instruction slot 99
func_80012714_slot01 control: different 1323 of 2000 (expected more than 0)
  altered: a shift right by 4 changed to 3, instruction slot 40
func_80012990_slot01 control: different 2000 of 2000 (expected more than 0)
  altered: a byte store of the primitive moved by one byte, instruction slot 24
func_80012dec_slot01 control: different 2000 of 2000 (expected more than 0)
  altered: a shift left by 4 changed to 3, instruction slot 37
func_8001791c_slot27 control: different 1192 of 2000 (expected more than 0)
  altered: texture page constant changed, instruction slot 121
func_80078628_slot00 control: different 1348 of 2000 (expected more than 0)
  altered: field_06 increment changed from 1 to 2, instruction slot 26
func_800e0b48_slot0f control: different 678 of 2000 (expected more than 0)
  altered: mode 0x10 becomes 0x11, instruction slot 15
func_800e0f2c_slot0f control: different 1703 of 2000 (expected more than 0)
  altered: slot step 0x28 becomes 0x2c, instruction slot 25
func_800e3478_slot0f control: different 1429 of 2000 (expected more than 0)
  altered: cell value 0x16 changed to 0x17, instruction slot 80
func_800e4ab0_slot0f control: different 649 of 2000 (expected more than 0)
  altered: y store moved to the x offset, instruction slot 83
func_800e68e8_slot0f control: different 2000 of 2000 (expected more than 0)
  altered: texture page constant changed, instruction slot 51
func_800e80ec_slot0f control: different 2000 of 2000 (expected more than 0)
  altered: increment by 8 changed to 9, instruction slot 81
```

    python difftest.py --config ../build.toml --folder ../modules_nonmatching --cases 2000 --writes --all

```
func_800108d4_slot01 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8001188c_slot27 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011a14_slot01 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80012714_slot01 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80012990_slot01 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80012dec_slot01 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8001791c_slot27 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80078628_slot00 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e0b48_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e0f2c_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e3478_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e4ab0_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e68e8_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e80ec_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
```

A function with fewer slots executed than it has names the others in its
header comment, with the reason why no input reaches them. The third
block is the write audit: in how many cases the original changed memory
that the function's setup did not make.
