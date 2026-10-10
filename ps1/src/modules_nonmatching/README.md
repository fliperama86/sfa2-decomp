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

What these commands printed on 2026-10-10, run from the tool's folder:

    python difftest.py --config ../build.toml --folder ../modules_nonmatching --cases 2000 --all

```
func_800108d4_slot01: built 704 bytes, original 796 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800108d4_slot01 coverage: 199 of 199 instruction slots of the original executed
func_80011688_slot12: built 260 bytes, original 260 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011688_slot12 coverage: 65 of 65 instruction slots of the original executed
func_8001188c_slot27: built 176 bytes, original 236 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8001188c_slot27 coverage: 59 of 59 instruction slots of the original executed
func_80011a14_slot01: built 760 bytes, original 748 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80011a14_slot01 coverage: 187 of 187 instruction slots of the original executed
func_80012714_slot01: built 568 bytes, original 636 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80012714_slot01 coverage: 159 of 159 instruction slots of the original executed
func_80012990_slot01: built 312 bytes, original 448 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80012990_slot01 coverage: 112 of 112 instruction slots of the original executed
func_80012a0c_slot12: built 316 bytes, original 336 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80012a0c_slot12 coverage: 84 of 84 instruction slots of the original executed
func_80012dec_slot01: built 320 bytes, original 372 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80012dec_slot01 coverage: 93 of 93 instruction slots of the original executed
func_80013224_slot12: built 548 bytes, original 556 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80013224_slot12 coverage: 139 of 139 instruction slots of the original executed
func_8001389c_slot28: built 140 bytes, original 144 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8001389c_slot28 coverage: 36 of 36 instruction slots of the original executed
func_800145f4_slot28: built 132 bytes, original 132 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800145f4_slot28 coverage: 33 of 33 instruction slots of the original executed
func_8001606c_slot12: built 352 bytes, original 384 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8001606c_slot12 coverage: 96 of 96 instruction slots of the original executed
func_80016af0_slot12: built 308 bytes, original 352 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80016af0_slot12 coverage: 88 of 88 instruction slots of the original executed
func_8001791c_slot27: built 612 bytes, original 680 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8001791c_slot27 coverage: 169 of 170 instruction slots of the original executed
func_8001a69c_slot28: built 476 bytes, original 476 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8001a69c_slot28 coverage: 119 of 119 instruction slots of the original executed
func_80022944_slot28: built 440 bytes, original 440 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80022944_slot28 coverage: 110 of 110 instruction slots of the original executed
func_80077868_slot2b: built 156 bytes, original 160 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80077868_slot2b coverage: 40 of 40 instruction slots of the original executed
func_80078628_slot00: built 136 bytes, original 132 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80078628_slot00 coverage: 33 of 33 instruction slots of the original executed
func_80079144_slot2b: built 172 bytes, original 196 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80079144_slot2b coverage: 49 of 49 instruction slots of the original executed
func_800793d4_slot2b: built 200 bytes, original 204 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800793d4_slot2b coverage: 51 of 51 instruction slots of the original executed
func_800796e4_slot2b: built 288 bytes, original 288 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800796e4_slot2b coverage: 72 of 72 instruction slots of the original executed
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
func_801b03e4_slot04_0a: built 180 bytes, original 188 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b03e4_slot04_0a coverage: 47 of 47 instruction slots of the original executed
func_801b0904_slot04_04: built 164 bytes, original 168 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b0904_slot04_04 coverage: 42 of 42 instruction slots of the original executed
func_801b0ea0_slot04_sel: built 552 bytes, original 552 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b0ea0_slot04_sel coverage: 138 of 138 instruction slots of the original executed
func_801b1934_slot04_02: built 260 bytes, original 264 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b1934_slot04_02 coverage: 66 of 66 instruction slots of the original executed
func_801b20b0_slot04_06: built 188 bytes, original 184 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b20b0_slot04_06 coverage: 46 of 46 instruction slots of the original executed
func_801b2560_slot04_00: built 144 bytes, original 144 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b2560_slot04_00 coverage: 36 of 36 instruction slots of the original executed
func_801b2f1c_slot04_0a: built 304 bytes, original 312 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b2f1c_slot04_0a coverage: 78 of 78 instruction slots of the original executed
func_801b3564_slot04_06: built 724 bytes, original 876 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b3564_slot04_06 coverage: 215 of 219 instruction slots of the original executed
func_801b36f8_slot04_09: built 288 bytes, original 312 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b36f8_slot04_09 coverage: 78 of 78 instruction slots of the original executed
func_801b37b0_slot04_03: built 492 bytes, original 552 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b37b0_slot04_03 coverage: 138 of 138 instruction slots of the original executed
func_801b3f38_slot04_sel: built 328 bytes, original 340 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b3f38_slot04_sel coverage: 85 of 85 instruction slots of the original executed
func_801b7bfc_slot04_02: built 344 bytes, original 348 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b7bfc_slot04_02 coverage: 87 of 87 instruction slots of the original executed
func_801ca0b0_slot05_06: built 188 bytes, original 184 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801ca0b0_slot05_06 coverage: 46 of 46 instruction slots of the original executed
func_801cb564_slot05_06: built 756 bytes, original 872 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801cb564_slot05_06 coverage: 214 of 218 instruction slots of the original executed
func_801e0a78_slot0b: built 556 bytes, original 624 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e0a78_slot0b coverage: 156 of 156 instruction slots of the original executed
func_801e0ce8_slot0b: built 304 bytes, original 448 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e0ce8_slot0b coverage: 112 of 112 instruction slots of the original executed
func_801e0ea8_slot0b: built 320 bytes, original 372 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e0ea8_slot0b coverage: 93 of 93 instruction slots of the original executed
func_801e1d6c_slot0b: built 540 bytes, original 624 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801e1d6c_slot0b coverage: 154 of 156 instruction slots of the original executed
```

    python difftest.py --config ../build.toml --folder ../modules_nonmatching --cases 2000 --control --all

```
func_800108d4_slot01 control: different 809 of 2000 (expected more than 0)
  altered: constant 0xe0 changed to 0xe1, instruction slot 16
func_80011688_slot12 control: different 1906 of 2000 (expected more than 0)
  altered: last channel step 2 changed to 3, instruction slot 24
func_8001188c_slot27 control: different 2000 of 2000 (expected more than 0)
  altered: last halfword store of the loop moved by two bytes, instruction slot 33
func_80011a14_slot01 control: different 649 of 2000 (expected more than 0)
  altered: constant 0xa0 changed to 0xa1, instruction slot 99
func_80012714_slot01 control: different 1323 of 2000 (expected more than 0)
  altered: a shift right by 4 changed to 3, instruction slot 40
func_80012990_slot01 control: different 2000 of 2000 (expected more than 0)
  altered: a byte store of the primitive moved by one byte, instruction slot 24
func_80012a0c_slot12 control: different 2000 of 2000 (expected more than 0)
  altered: width constant 0xc0 changed to 0xc1, instruction slot 18
func_80012dec_slot01 control: different 2000 of 2000 (expected more than 0)
  altered: a shift left by 4 changed to 3, instruction slot 37
func_80013224_slot12 control: different 1313 of 2000 (expected more than 0)
  altered: grey level 0x80 changed to 0x81, instruction slot 49
func_8001389c_slot28 control: different 2000 of 2000 (expected more than 0)
  altered: store of field_ad moved by one byte, instruction slot 29
func_800145f4_slot28 control: different 1193 of 2000 (expected more than 0)
  altered: store of field_60 moved by two bytes, instruction slot 11
func_8001606c_slot12 control: different 1012 of 2000 (expected more than 0)
  altered: address of the second list moved by one byte, instruction slot 49
func_80016af0_slot12 control: different 2000 of 2000 (expected more than 0)
  altered: texture page constant 0x7f07 changed to 0x7f08, instruction slot 51
func_8001791c_slot27 control: different 1192 of 2000 (expected more than 0)
  altered: texture page constant changed, instruction slot 121
func_8001a69c_slot28 control: different 1376 of 2000 (expected more than 0)
  altered: store of field_60 moved by two bytes, instruction slot 18
func_80022944_slot28 control: different 1607 of 2000 (expected more than 0)
  altered: store of field_60 moved by two bytes, instruction slot 19
func_80077868_slot2b control: different 824 of 2000 (expected more than 0)
  altered: base 0xc changed to 0xd, instruction slot 24
func_80078628_slot00 control: different 1348 of 2000 (expected more than 0)
  altered: field_06 increment changed from 1 to 2, instruction slot 26
func_80079144_slot2b control: different 1020 of 2000 (expected more than 0)
  altered: negation turned into a doubling, instruction slot 31
func_800793d4_slot2b control: different 865 of 2000 (expected more than 0)
  altered: third argument 0x10 changed to 0x11, instruction slot 40
func_800796e4_slot2b control: different 1479 of 2000 (expected more than 0)
  altered: size word 0x1e0 changed to 0x1e1, instruction slot 25
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
func_801b03e4_slot04_0a control: different 1718 of 2000 (expected more than 0)
  altered: constant stored in field_46 changed by one, instruction slot 11
func_801b0904_slot04_04 control: different 563 of 2000 (expected more than 0)
  altered: sequence constant 0x1d changed to 0x1e, instruction slot 26
func_801b0ea0_slot04_sel control: different 863 of 2000 (expected more than 0)
  altered: field_50 set to 2 instead of 1, instruction slot 129
func_801b1934_slot04_02 control: different 2000 of 2000 (expected more than 0)
  altered: store of the second table word moved from field_50 to field_5c, instruction slot 39
func_801b20b0_slot04_06 control: different 2000 of 2000 (expected more than 0)
  altered: sequence base 0x1b changed to 0x1c, instruction slot 38
func_801b2560_slot04_00 control: different 705 of 2000 (expected more than 0)
  altered: sequence base 0x32 changed to 0x33, instruction slot 24
func_801b2f1c_slot04_0a control: different 1003 of 2000 (expected more than 0)
  altered: field_50 constant 0x38000 changed to 0x48000, instruction slot 51
func_801b3564_slot04_06 control: different 85 of 2000 (expected more than 0)
  altered: knock-back distance 0x60 changed to 0x61, instruction slot 173
func_801b36f8_slot04_09 control: different 1020 of 2000 (expected more than 0)
  altered: sequence 0x33 changed to 0x34, instruction slot 42
func_801b37b0_slot04_03 control: different 1445 of 2000 (expected more than 0)
  altered: sound number 0x30d changed to 0x30e, instruction slot 14
func_801b3f38_slot04_sel control: different 1101 of 2000 (expected more than 0)
  altered: increment of field_4e changed to 2, instruction slot 56
func_801b7bfc_slot04_02 control: different 1489 of 2000 (expected more than 0)
  altered: constant stored in field_46 changed from 0x15 to 0x16, instruction slot 42
func_801ca0b0_slot05_06 control: different 2000 of 2000 (expected more than 0)
  altered: sequence base 0x1b changed to 0x1c, instruction slot 38
func_801cb564_slot05_06 control: different 419 of 2000 (expected more than 0)
  altered: field_4c constant 0xa0000 changed to 0xb0000, instruction slot 86
func_801e0a78_slot0b control: different 2000 of 2000 (expected more than 0)
  altered: first +16 changed to +17, instruction slot 93
func_801e0ce8_slot0b control: different 1997 of 2000 (expected more than 0)
  altered: id shift changed from 6 to 5, instruction slot 54
func_801e0ea8_slot0b control: different 2000 of 2000 (expected more than 0)
  altered: first corner store moved by 0x10 bytes, instruction slot 37
func_801e1d6c_slot0b control: different 1847 of 2000 (expected more than 0)
  altered: command word constant changed, instruction slot 75
```

    python difftest.py --config ../build.toml --folder ../modules_nonmatching --cases 2000 --writes --all

```
func_800108d4_slot01 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011688_slot12 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8001188c_slot27 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80011a14_slot01 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80012714_slot01 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80012990_slot01 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80012a0c_slot12 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80012dec_slot01 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80013224_slot12 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8001389c_slot28 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800145f4_slot28 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8001606c_slot12 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80016af0_slot12 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8001791c_slot27 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8001a69c_slot28 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80022944_slot28 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80077868_slot2b writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80078628_slot00 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80079144_slot2b writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800793d4_slot2b writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800796e4_slot2b writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e0b48_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e0f2c_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e3478_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e4ab0_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e68e8_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e80ec_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b03e4_slot04_0a writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b0904_slot04_04 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b0ea0_slot04_sel writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b1934_slot04_02 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b20b0_slot04_06 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b2560_slot04_00 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b2f1c_slot04_0a writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b3564_slot04_06 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b36f8_slot04_09 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b37b0_slot04_03 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b3f38_slot04_sel writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b7bfc_slot04_02 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801ca0b0_slot05_06 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801cb564_slot05_06 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e0a78_slot0b writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e0ce8_slot0b writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e0ea8_slot0b writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801e1d6c_slot0b writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
```

A function with fewer slots executed than it has names the others in its
header comment, with the reason why no input reaches them. The third
block is the write audit: in how many cases the original changed memory
that the function's setup did not make.
