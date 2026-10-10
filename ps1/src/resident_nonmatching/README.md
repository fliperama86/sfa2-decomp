# Nonmatching C: the resident program

C for functions of the resident program (`SLPS_004.15` itself) that compiles
with the pinned toolchain and does not reproduce the original's bytes. The
PS1 build does not use any file of this folder; the raw bytes stay what the
matching build owns. What nonmatching means here, how the test works and
what a pass does not show are on the page of the tool's folder,
[`../slot06_nonmatching/`](../slot06_nonmatching/README.md): a pass is
evidence for the tested inputs of a function's contract, not equivalence.

Each function is a pair: `FUNC.c` with its contract in the header comment,
and `FUNC.py`, the contract as code. A function that is later rebuilt
exactly becomes a unit of the build and leaves this folder.

What these commands printed on 2026-10-10, run from the tool's folder:

    python difftest.py --config ../build.toml --folder ../resident_nonmatching --cases 2000 --all

```
func_80119694: built 132 bytes, original 132 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80119694 coverage: 33 of 33 instruction slots of the original executed
func_8011a018: built 788 bytes, original 1016 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8011a018 coverage: 254 of 254 instruction slots of the original executed
func_8011a880: built 536 bytes, original 868 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8011a880 coverage: 217 of 217 instruction slots of the original executed
func_8011acbc: built 1080 bytes, original 2264 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8011acbc coverage: 566 of 566 instruction slots of the original executed
func_8011bf70: built 1480 bytes, original 1528 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8011bf70 coverage: 381 of 382 instruction slots of the original executed
func_8011c724: built 1120 bytes, original 1172 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8011c724 coverage: 291 of 293 instruction slots of the original executed
func_8011cf98: built 1520 bytes, original 1568 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8011cf98 coverage: 391 of 392 instruction slots of the original executed
func_8011d74c: built 1080 bytes, original 1068 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8011d74c coverage: 266 of 267 instruction slots of the original executed
func_8011db78: built 768 bytes, original 964 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8011db78 coverage: 239 of 241 instruction slots of the original executed
func_8011df3c: built 1048 bytes, original 1676 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8011df3c coverage: 419 of 419 instruction slots of the original executed
func_8011eb4c: built 716 bytes, original 792 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8011eb4c coverage: 198 of 198 instruction slots of the original executed
func_8011f06c: built 44 bytes, original 48 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8011f06c coverage: 12 of 12 instruction slots of the original executed
func_8011fa50: built 128 bytes, original 144 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8011fa50 coverage: 35 of 36 instruction slots of the original executed
func_80120cf0: built 664 bytes, original 592 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80120cf0 coverage: 148 of 148 instruction slots of the original executed
func_80124304: built 904 bytes, original 928 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80124304 coverage: 232 of 232 instruction slots of the original executed
func_801254f4: built 412 bytes, original 400 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801254f4 coverage: 100 of 100 instruction slots of the original executed
func_8012f9b0: built 300 bytes, original 328 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8012f9b0 coverage: 82 of 82 instruction slots of the original executed
func_8012fd80: built 196 bytes, original 224 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8012fd80 coverage: 56 of 56 instruction slots of the original executed
func_8013172c: built 280 bytes, original 296 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8013172c coverage: 74 of 74 instruction slots of the original executed
func_80131ab4: built 368 bytes, original 456 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80131ab4 coverage: 114 of 114 instruction slots of the original executed
func_8013245c: built 1736 bytes, original 1748 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8013245c coverage: 437 of 437 instruction slots of the original executed
func_80134234: built 988 bytes, original 1008 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80134234 coverage: 252 of 252 instruction slots of the original executed
func_801347b4: built 940 bytes, original 1760 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801347b4 coverage: 436 of 440 instruction slots of the original executed
func_80135a10: built 524 bytes, original 508 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80135a10 coverage: 127 of 127 instruction slots of the original executed
func_801364a0: built 460 bytes, original 456 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801364a0 coverage: 114 of 114 instruction slots of the original executed
func_80136744: built 344 bytes, original 340 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80136744 coverage: 85 of 85 instruction slots of the original executed
func_80138d70: built 320 bytes, original 340 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80138d70 coverage: 85 of 85 instruction slots of the original executed
func_8013902c: built 740 bytes, original 728 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8013902c coverage: 182 of 182 instruction slots of the original executed
```

    python difftest.py --config ../build.toml --folder ../resident_nonmatching --cases 2000 --control --all

```
func_80119694 control: different 141 of 2000 (expected more than 0)
  altered: counter starts at 28, instruction slot 0
func_8011a018 control: different 2000 of 2000 (expected more than 0)
  altered: data_80184700 receives 0x70, instruction slot 93
func_8011a880 control: different 1996 of 2000 (expected more than 0)
  altered: part field_09 offset 4 becomes 5, instruction slot 9
func_8011acbc control: different 2000 of 2000 (expected more than 0)
  altered: loop limit 41, instruction slot 261
func_8011bf70 control: different 1598 of 2000 (expected more than 0)
  altered: store of the object's entry index moved by two bytes, instruction slot 338
func_8011c724 control: different 1355 of 2000 (expected more than 0)
  altered: store of the callee's result into the quad moved by two bytes, instruction slot 185
func_8011cf98 control: different 1595 of 2000 (expected more than 0)
  altered: store of the object's entry index moved by two bytes, instruction slot 345
func_8011d74c control: different 614 of 2000 (expected more than 0)
  altered: cell position byte stored one byte lower, instruction slot 193
func_8011db78 control: different 939 of 2000 (expected more than 0)
  altered: first halfword store of a y value moved by two bytes, instruction slot 149
func_8011df3c control: different 1141 of 2000 (expected more than 0)
  altered: address mask combined with or instead of xor, instruction slot 14
func_8011eb4c control: different 2000 of 2000 (expected more than 0)
  altered: type byte of the third array is 0x11, instruction slot 73
func_8011f06c control: different 2000 of 2000 (expected more than 0)
  altered: store of field_226 moved by one byte, instruction slot 8
func_8011fa50 control: different 2000 of 2000 (expected more than 0)
  altered: store of field_3a moved by two bytes, instruction slot 10
func_80120cf0 control: different 1012 of 2000 (expected more than 0)
  altered: random byte masked with 0x70, instruction slot 111
func_80124304 control: different 307 of 2000 (expected more than 0)
  altered: field_49 value 0x63 changed to 0x62, instruction slot 162
func_801254f4 control: different 118 of 2000 (expected more than 0)
  altered: failure value 0x3c changed to 0x3d, instruction slot 32
func_8012f9b0 control: different 1859 of 2000 (expected more than 0)
  altered: cursor stride 0xc0 changed to 0xc4, instruction slot 64
func_8012fd80 control: different 584 of 2000 (expected more than 0)
  altered: return value 2 changed to 3, instruction slot 41
func_8013172c control: different 2000 of 2000 (expected more than 0)
  altered: store of field_134 moved by four bytes, instruction slot 59
func_80131ab4 control: different 258 of 2000 (expected more than 0)
  altered: flag value 1 changed to 2, instruction slot 21
func_8013245c control: different 2000 of 2000 (expected more than 0)
  altered: constant 0x7807 changed to 0x7806, instruction slot 410
func_80134234 control: different 1645 of 2000 (expected more than 0)
  altered: record offset 0x124 changed to 0x128, instruction slot 46
func_801347b4 control: different 711 of 2000 (expected more than 0)
  altered: page word constant 0x7807 changed to 0x7809, instruction slot 84
func_80135a10 control: different 749 of 2000 (expected more than 0)
  altered: mode 2 code 0x20 changed to 0x21, instruction slot 94
func_801364a0 control: different 78 of 2000 (expected more than 0)
  altered: span threshold 0xc0 changed to 0xc1, instruction slot 73
func_80136744 control: different 929 of 2000 (expected more than 0)
  altered: first field_26 store moved by two bytes, instruction slot 61
func_80138d70 control: different 262 of 2000 (expected more than 0)
  altered: fill byte 0x20 changed to 0x21, instruction slot 50
func_8013902c control: different 715 of 2000 (expected more than 0)
  altered: box width load moved to the height byte, instruction slot 82
```

    python difftest.py --config ../build.toml --folder ../resident_nonmatching --cases 2000 --writes --all

```
func_80119694 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8011a018 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8011a880 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8011acbc writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8011bf70 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8011c724 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8011cf98 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8011d74c writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8011db78 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8011df3c writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8011eb4c writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8011f06c writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8011fa50 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80120cf0 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80124304 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801254f4 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8012f9b0 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8012fd80 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8013172c writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80131ab4 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8013245c writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80134234 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801347b4 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80135a10 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801364a0 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80136744 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80138d70 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8013902c writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
```

A function with fewer slots executed than it has names the others in its
header comment, with the reason why no input reaches them. The third
block is the write audit: in how many cases the original changed memory
that the function's setup did not make.
