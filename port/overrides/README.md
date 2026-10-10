# Overrides of the port

An override in C stands in the PC program for a function of the game whose
exact C cannot run there as written. Each function of this folder is the
exact C of its unit with one change, written beside the exact C and not in
it: the argument that the console's code leaves in the register `a0` is
passed in the call. On the console the callee reads that register and finds
the caller's own first parameter in it. Compiled for a PC, the callee reads
something else.

The PS1 build uses no file of this folder, and the exact C stays in its
unit. Each function is a pair, `NAME.c` with its contract in the header
comment and `NAME.py`, the contract as code, tested with the tool of
[`../../ps1/src/slot06_nonmatching/`](../../ps1/src/slot06_nonmatching/README.md).
A pass is evidence for the tested inputs of a contract, not equivalence. The
test replaces each callee by a recorder that logs its first argument, so a
pass shows that the override hands the callee the same value as the original
code does. Each control alters the original code so that the callee gets
that value plus 4, and the test must then differ.

| Function | Unit of its exact C | What the override adds | Callee |
| --- | --- | --- | --- |
| `func_8014dcc0` | `ps1/src/s14d8a4_r2.c` | passes `object` | the function in `scr_d4_left` or `scr_184_right` |
| `func_8012126c` | `ps1/src/s120f40_r2.c` | passes `state` in the second arm | `func_80013834` (the module `slot28` has it) |
| `func_801212c8` | `ps1/src/s120f40_r2.c` | passes its parameter | `func_8001365c` (the module `slot12` has it) |
| `func_801212e8` | `ps1/src/s120f40_r2.c` | passes its parameter | `func_800136b0` (the module `slot12` has it) |

What these commands printed on 2026-10-10, run from the tool's folder (`ps1/src/slot06_nonmatching/`):

    python difftest.py --config ../build.toml --folder ../../../port/overrides --cases 2000 --seed 1 --all

```
func_8012126c: built 92 bytes, original 92 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8012126c coverage: 23 of 23 instruction slots of the original executed
func_801212c8: built 32 bytes, original 32 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801212c8 coverage: 8 of 8 instruction slots of the original executed
func_801212e8: built 32 bytes, original 32 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801212e8 coverage: 8 of 8 instruction slots of the original executed
func_8014dcc0: built 84 bytes, original 84 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8014dcc0 coverage: 21 of 21 instruction slots of the original executed
```

    python difftest.py --config ../build.toml --folder ../../../port/overrides --cases 2000 --seed 1 --control --all

```
func_8012126c control: different 1013 of 2000 (expected more than 0)
  altered: the callee gets a0 + 4, instruction slot 18
func_801212c8 control: different 2000 of 2000 (expected more than 0)
  altered: the callee gets a0 + 4, instruction slot 3
func_801212e8 control: different 2000 of 2000 (expected more than 0)
  altered: the callee gets a0 + 4, instruction slot 3
func_8014dcc0 control: different 2000 of 2000 (expected more than 0)
  altered: the callee gets a0 + 4, instruction slot 14
```

    python difftest.py --config ../build.toml --folder ../../../port/overrides --cases 2000 --seed 1 --writes --all

```
func_8012126c writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801212c8 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801212e8 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8014dcc0 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
```
