# Overrides of the port

An override in C stands in the PC program for a function of the game whose
exact C cannot run there as written. Each function of this folder is the
exact C of its unit with one change, written beside the exact C and not in
it: the argument that the console's code leaves in the register `a0` is
passed in the call. On the console the callee reads that register and finds
in it what the caller's code left there: its own first parameter, or, when
a call lies before, what that earlier call left in it. Compiled for a PC,
the callee reads something else.

The PS1 build uses no file of this folder, and the exact C stays in its
unit. Each function is a pair, `NAME.c` with its contract in the header
comment and `NAME.py`, the contract as code, tested with the tool of
[`../../ps1/src/slot06_nonmatching/`](../../ps1/src/slot06_nonmatching/README.md).
A pass is evidence for the tested inputs of a contract, not equivalence. The
test replaces the callee of the call by a recorder that logs its first
argument (the earlier callees of the five later functions run as original code,
see below), so a pass shows that the override hands the callee the same value as the original
code does. Each control alters the build of the override, not the original
code: it adds 4 to the argument of the call, so that the built C hands the
callee another value, and the test must then differ from the original. In
the four first functions it puts `addiu a0,a0,4` into the empty delay slot
of the call. In the five later ones the build has no empty delay slot there
(the compiler puts the move of the argument into it), so the control
changes that move into `addiu a0,s0,4`.

The build of the PC program (`port/tools/hostbuild.py`, whose header is
the contract) compiles every `NAME.c` of this folder and lets it run in
place of the unit's C of NAME; it refuses a file whose NAME is not a
function that a unit has C for, and a file without its `NAME.py`. How it
does that is on [the port's page](../README.md), under "Overrides in C".

| Function | Unit of its exact C | What the override adds | Callee | What lies before the call, and what it leaves in `a0` |
| --- | --- | --- | --- | --- |
| `func_8014dcc0` | `ps1/src/s14d8a4_r2.c` | passes `object` | the function in `scr_d4_left` or `scr_184_right` | nothing: the parameter |
| `func_8012126c` | `ps1/src/s120f40_r2.c` | passes `state` in the second arm | `func_80013834` (the module `slot28` has it) | nothing: the parameter |
| `func_801212c8` | `ps1/src/s120f40_r2.c` | passes its parameter | `func_8001365c` (the module `slot12` has it) | nothing: the parameter |
| `func_801212e8` | `ps1/src/s120f40_r2.c` | passes its parameter | `func_800136b0` (the module `slot12` has it) | nothing: the parameter |
| `func_80146794` | `ps1/src/s1460ec_r3.c` | passes `object`, or the word at offset 0x18 of the object as it was when `func_80131094` was entered | `func_80120028` | `func_80131094`, which leaves `a0` alone on two paths and loads that word into it on the third (when the halfword at 0x38, lowered by one, is 0 and the signed halfword at 0x3a is negative) |
| `func_8001385c_slot01` | `ps1/src/slot01/slot01_3440_r3.c` | passes `obj` | `func_8011ffdc` | a call through the table `data_80015a64_slot01`, three entries, none writes `a0` |
| `func_801b12bc_slot04_0f` | `ps1/src/slot04_0f/slot04_0f_09e4_r1_b.c` | passes `obj` | `func_80141c4c` | nothing: the parameter |
| `func_800205b4_slot28` | `ps1/src/slot28/slot28_05b4x_r1.c` | passes `obj` | `func_80131094` | `func_80020608_slot28`, a leaf that never writes `a0` |
| `func_8001714c_slot28` | `ps1/src/slot28/slot28_714cx_r1.c` | passes `obj` | `func_80131094` | a call through the table `data_800306cc_slot28`, two entries, both leaves that never write `a0` |

For the last five, a call lies before the call that has no argument, in all
but `func_801b12bc_slot04_0f`, where none lies and the register holds the
parameter. In `func_80146794`, `func_8001385c_slot01`, `func_800205b4_slot28`
and `func_8001714c_slot28` the earlier callee runs as the original code of the
console in the test, and only the callee of the argument-less call is a
recorder. Reason: what the earlier callee leaves in `a0` is then what the
console's code leaves, in every case of the setup. With a recorder in its place
the test would only show that the override passes what the contract's author
believed was in the register. The tables and their entries are the image's own,
which the setup does not write. The header of each `.c` gives, for the callee,
one instruction that reads `a0`, and for every earlier callee (every entry of a
table) what it does to `a0` on each return path, with the addresses read in the
listing; the tables' lengths are inferred from the word after the last entry.

What these commands printed on 2026-10-10, run from the tool's folder (`ps1/src/slot06_nonmatching/`):

    python difftest.py --config ../build.toml --folder ../../../port/overrides --cases 2000 --seed 1 --all

```
func_8001385c_slot01: built 84 bytes, original 72 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8001385c_slot01 coverage: 18 of 18 instruction slots of the original executed
func_8001714c_slot28: built 92 bytes, original 92 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8001714c_slot28 coverage: 23 of 23 instruction slots of the original executed
func_800205b4_slot28: built 84 bytes, original 84 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800205b4_slot28 coverage: 21 of 21 instruction slots of the original executed
func_8012126c: built 92 bytes, original 92 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8012126c coverage: 23 of 23 instruction slots of the original executed
func_801212c8: built 32 bytes, original 32 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801212c8 coverage: 8 of 8 instruction slots of the original executed
func_801212e8: built 32 bytes, original 32 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801212e8 coverage: 8 of 8 instruction slots of the original executed
func_80146794: built 184 bytes, original 140 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80146794 coverage: 35 of 35 instruction slots of the original executed
func_8014dcc0: built 84 bytes, original 84 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8014dcc0 coverage: 21 of 21 instruction slots of the original executed
func_801b12bc_slot04_0f: built 112 bytes, original 112 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801b12bc_slot04_0f coverage: 28 of 28 instruction slots of the original executed
```

    python difftest.py --config ../build.toml --folder ../../../port/overrides --cases 2000 --seed 1 --control --all

```
func_8001385c_slot01 control: different 2000 of 2000 (expected more than 0)
  altered: the callee gets its argument + 4, instruction slot 15
func_8001714c_slot28 control: different 2000 of 2000 (expected more than 0)
  altered: the callee gets its argument + 4, instruction slot 15
func_800205b4_slot28 control: different 2000 of 2000 (expected more than 0)
  altered: the callee gets its argument + 4, instruction slot 15
func_8012126c control: different 1013 of 2000 (expected more than 0)
  altered: the callee gets a0 + 4, instruction slot 18
func_801212c8 control: different 2000 of 2000 (expected more than 0)
  altered: the callee gets a0 + 4, instruction slot 3
func_801212e8 control: different 2000 of 2000 (expected more than 0)
  altered: the callee gets a0 + 4, instruction slot 3
func_80146794 control: different 2000 of 2000 (expected more than 0)
  altered: the callee gets its argument + 4, instruction slot 40
func_8014dcc0 control: different 2000 of 2000 (expected more than 0)
  altered: the callee gets a0 + 4, instruction slot 14
func_801b12bc_slot04_0f control: different 1592 of 2000 (expected more than 0)
  altered: the callee gets its argument + 4, instruction slot 9
```

    python difftest.py --config ../build.toml --folder ../../../port/overrides --cases 2000 --seed 1 --writes --all

```
func_8001385c_slot01 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8001714c_slot28 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800205b4_slot28 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8012126c writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801212c8 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801212e8 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80146794 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8014dcc0 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b12bc_slot04_0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
```
