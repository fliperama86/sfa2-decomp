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

What these commands printed on 2026-10-09, run from the tool's folder:

    python difftest.py --config ../build.toml --folder ../slot04b_nonmatching --cases 2000 --all

```
func_801b0904_slot04_12: built 164 bytes, original 168 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b0904_slot04_12 coverage: 42 of 42 instruction slots of the original executed
func_801b1a00_slot04_14: built 260 bytes, original 264 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b1a00_slot04_14 coverage: 66 of 66 instruction slots of the original executed
func_801b1b40_slot04_0e: built 168 bytes, original 188 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b1b40_slot04_0e coverage: 47 of 47 instruction slots of the original executed
func_801b21f8_slot04_0f: built 96 bytes, original 120 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b21f8_slot04_0f coverage: 30 of 30 instruction slots of the original executed
func_801b2270_slot04_0f: built 124 bytes, original 124 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b2270_slot04_0f coverage: 31 of 31 instruction slots of the original executed
func_801b248c_slot04_0f: built 140 bytes, original 120 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b248c_slot04_0f coverage: 30 of 30 instruction slots of the original executed
func_801b24e0_slot04_17: built 580 bytes, original 592 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b24e0_slot04_17 coverage: 148 of 148 instruction slots of the original executed
func_801b24f0_slot04_11: built 580 bytes, original 592 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b24f0_slot04_11 coverage: 148 of 148 instruction slots of the original executed
func_801b27a0_slot04_17: built 504 bytes, original 528 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b27a0_slot04_17 coverage: 132 of 132 instruction slots of the original executed
func_801b27b0_slot04_11: built 504 bytes, original 528 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b27b0_slot04_11 coverage: 132 of 132 instruction slots of the original executed
func_801b2e8c_slot04_0e: built 200 bytes, original 204 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b2e8c_slot04_0e coverage: 51 of 51 instruction slots of the original executed
func_801b339c_slot04_0e: built 196 bytes, original 180 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b339c_slot04_0e coverage: 45 of 45 instruction slots of the original executed
func_801b34b8_slot04_17: built 552 bytes, original 556 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b34b8_slot04_17 coverage: 139 of 139 instruction slots of the original executed
func_801b34e4_slot04_11: built 628 bytes, original 628 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b34e4_slot04_11 coverage: 157 of 157 instruction slots of the original executed
func_801b4100_slot04_0e: built 264 bytes, original 268 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b4100_slot04_0e coverage: 67 of 67 instruction slots of the original executed
func_801b420c_slot04_0e: built 248 bytes, original 256 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b420c_slot04_0e coverage: 64 of 64 instruction slots of the original executed
func_801b4374_slot04_17: built 444 bytes, original 448 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b4374_slot04_17 coverage: 112 of 112 instruction slots of the original executed
func_801b43e8_slot04_11: built 444 bytes, original 448 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b43e8_slot04_11 coverage: 112 of 112 instruction slots of the original executed
func_801b4a3c_slot04_0e: built 252 bytes, original 260 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b4a3c_slot04_0e coverage: 65 of 65 instruction slots of the original executed
func_801b5e34_slot04_0e: built 252 bytes, original 244 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b5e34_slot04_0e coverage: 61 of 61 instruction slots of the original executed
func_801b81f8_slot04_14: built 340 bytes, original 348 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b81f8_slot04_14 coverage: 87 of 87 instruction slots of the original executed
```

    python difftest.py --config ../build.toml --folder ../slot04b_nonmatching --cases 2000 --control --all

```
func_801b0904_slot04_12 control: different 518 of 2000 (expected more than 0)
  altered: sequence constant 0x1d changed to 0x1e, instruction slot 26
func_801b1a00_slot04_14 control: different 2000 of 2000 (expected more than 0)
  altered: store of the second table word moved from field_50 to field_5c, instruction slot 39
func_801b1b40_slot04_0e control: different 2000 of 2000 (expected more than 0)
  altered: state byte 3 made 4, instruction slot 3
func_801b21f8_slot04_0f control: different 2000 of 2000 (expected more than 0)
  altered: final shift 4 changed to 3, instruction slot 21
func_801b2270_slot04_0f control: different 756 of 2000 (expected more than 0)
  altered: bit test 0x80 changed to 0x40, instruction slot 10
func_801b248c_slot04_0f control: different 2000 of 2000 (expected more than 0)
  altered: field_07 increment changed to 2, instruction slot 8
func_801b24e0_slot04_17 control: different 633 of 2000 (expected more than 0)
  altered: stop-case field_50 0x10000 changed to 0x20000, instruction slot 81
func_801b24f0_slot04_11 control: different 558 of 2000 (expected more than 0)
  altered: stop-case field_50 0x10000 changed to 0x20000, instruction slot 81
func_801b27a0_slot04_17 control: different 840 of 2000 (expected more than 0)
  altered: sequence base 0x2e changed to 0x2f, instruction slot 116
func_801b27b0_slot04_11 control: different 796 of 2000 (expected more than 0)
  altered: sequence base 0x2e changed to 0x2f, instruction slot 116
func_801b2e8c_slot04_0e control: different 328 of 2000 (expected more than 0)
  altered: store of zero to field_50 moved to field_54, instruction slot 35
func_801b339c_slot04_0e control: different 581 of 2000 (expected more than 0)
  altered: sound argument 0x1b made 0x1c, instruction slot 10
func_801b34b8_slot04_17 control: different 2000 of 2000 (expected more than 0)
  altered: bgez changed to bltz, instruction slot 17
func_801b34e4_slot04_11 control: different 2000 of 2000 (expected more than 0)
  altered: bgez changed to bltz, instruction slot 18
func_801b4100_slot04_0e control: different 1615 of 2000 (expected more than 0)
  altered: constant stored to field_7c changed from 0x1e0 to 0x1e1, instruction slot 40
func_801b420c_slot04_0e control: different 1579 of 2000 (expected more than 0)
  altered: constant stored to field_48 changed from 0x4b to 0x4c, instruction slot 29
func_801b4374_slot04_17 control: different 56 of 2000 (expected more than 0)
  altered: distance cutoff 0x100 changed to 0x101, instruction slot 44
func_801b43e8_slot04_11 control: different 70 of 2000 (expected more than 0)
  altered: distance cutoff 0x100 changed to 0x101, instruction slot 44
func_801b4a3c_slot04_0e control: different 937 of 2000 (expected more than 0)
  altered: constant stored to field_48 changed from 0x4e to 0x4f, instruction slot 33
func_801b5e34_slot04_0e control: different 2000 of 2000 (expected more than 0)
  altered: store of the state byte moved by one byte, instruction slot 21
func_801b81f8_slot04_14 control: different 1484 of 2000 (expected more than 0)
  altered: constant stored in field_46 changed from 0x15 to 0x16, instruction slot 43
```
