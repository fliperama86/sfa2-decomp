# Overrides of the port

An override in C stands in the PC program for a function of the game whose
exact C cannot run there as written. Each function of this folder is the
exact C of its unit with one change, written beside the exact C and not in
it. There are two kinds of change.

The argument: the argument that the console's code leaves in the register
`a0` is passed in the call. On the console the callee reads that register
and finds in it what the caller's code left there: its own first
parameter, or, when a call lies before, what that earlier call left in it.
Compiled for a PC, the callee reads something else.

The result: the function returns the result of its last library call. The
unit defines it `void`, and the original's callers read the result
register, which on the console still holds that result. Compiled for a PC,
a function defined `void` hands its caller nothing.

The PS1 build uses no file of this folder, and the exact C stays in its
unit. Each function is a pair, `NAME.c` with its contract in the header
comment and `NAME.py`, the contract as code, tested with the tool of
[`../../ps1/src/slot06_nonmatching/`](../../ps1/src/slot06_nonmatching/README.md).
A pass is evidence for the tested inputs of a contract, not equivalence. The
test of an argument override replaces the callee of the call by a recorder that logs its first
argument (the earlier callees of the five later functions run as original code,
see below), so a pass shows that the override hands the callee the same value as the original
code does. Each control of an argument override alters the build of the override, not the original
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

The three that return the result:

| Function | Unit of its exact C | What the override adds | Library call | Where the original's callers read the result |
| --- | --- | --- | --- | --- |
| `func_800e11e4_slot0f` | `ps1/src/slot0f/slot0f_119c_r1.c` | returns the result of `open` | `open` | `func_800e0850_slot0f` at 800e0924, 800e0944, 800e0968; `func_800e0a1c_slot0f` at 800e0adc (each `move s0,v0` after the call) |
| `func_800e1250_slot0f` | `ps1/src/slot0f/slot0f_119c_r1.c` | returns the result of `read` | `read` | `func_800e0a1c_slot0f`, `bltz v0` at 800e0b00 |
| `func_800e12cc_slot0f` | `ps1/src/slot0f/slot0f_119c_r2.c` | returns the result of `write` | `write` | `func_800e0850_slot0f`, `bne v0,v1` at 800e09d4 |

The test of a result override replaces the library call and
`func_8015fb30` by recorders. The recorder of the library call answers from
a script, one result per call: the loop ends at the first try, at a later
try, and never (0x78 tries). The setup declares that the result register is
compared (`returns_value`). The control alters the build so that the function
returns its result plus 1 (the empty delay slot of its `jr ra` becomes
`addiu v0,v0,1`), and the test must then differ. Each is the same size as the
original (the first command's lines below).

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

## Testing one function

A function is tested alone. When one is added or changed, run its own
five runs, from the tool's folder (`ps1/src/slot06_nonmatching/`), and at
the same time (see [efficient checks](../../docs/efficiency.md)):

    python difftest.py --config ../build.toml --folder ../../../port/overrides --cases 2000 FUNC
    python difftest.py --config ../build.toml --folder ../../../port/overrides --cases 2000 --seed 7 FUNC
    python difftest.py --config ../build.toml --folder ../../../port/overrides --cases 2000 --control FUNC
    python difftest.py --config ../build.toml --folder ../../../port/overrides --cases 2000 --writes FUNC
    python difftest.py --config ../build.toml --folder ../../../port/overrides --cases 2000 --writes --seed 7 FUNC

Then put that function's lines into the three blocks below, in place of
its old ones or at its place in the order of names. No other function is
run again for it, and the folder is not run as a whole.

## The recorded lines

The three blocks are a record, not an instruction. They are what the
commands printed for each function when it was last run. For the nine
functions of the argument kind that is one run of the whole folder on
2026-10-10 (`--seed 1 --all` in place of FUNC); the page keeps those lines
as printed. The lines of the three result overrides are from their own
runs on 2026-10-10 (seed 1; their seed 7 runs also printed `different 0`
and `outside 0`). From now on a function's lines change only when that
function is run again.

The test (`--cases 2000 FUNC`, seed 1):

```
func_8001385c_slot01: built 84 bytes, original 72 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8001385c_slot01 coverage: 18 of 18 instruction slots of the original executed
func_8001714c_slot28: built 92 bytes, original 92 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8001714c_slot28 coverage: 23 of 23 instruction slots of the original executed
func_800205b4_slot28: built 84 bytes, original 84 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800205b4_slot28 coverage: 21 of 21 instruction slots of the original executed
func_800e11e4_slot0f: built 108 bytes, original 108 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800e11e4_slot0f coverage: 27 of 27 instruction slots of the original executed
func_800e1250_slot0f: built 124 bytes, original 124 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800e1250_slot0f coverage: 31 of 31 instruction slots of the original executed
func_800e12cc_slot0f: built 124 bytes, original 124 bytes; cases 2000, discarded 0, equal 2000, different 0
func_800e12cc_slot0f coverage: 31 of 31 instruction slots of the original executed
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

The control (`--cases 2000 --control FUNC`):

```
func_8001385c_slot01 control: different 2000 of 2000 (expected more than 0)
  altered: the callee gets its argument + 4, instruction slot 15
func_8001714c_slot28 control: different 2000 of 2000 (expected more than 0)
  altered: the callee gets its argument + 4, instruction slot 15
func_800205b4_slot28 control: different 2000 of 2000 (expected more than 0)
  altered: the callee gets its argument + 4, instruction slot 15
func_800e11e4_slot0f control: different 2000 of 2000 (expected more than 0)
  altered: the function returns its result + 1, instruction slot 26
func_800e1250_slot0f control: different 2000 of 2000 (expected more than 0)
  altered: the function returns its result + 1, instruction slot 30
func_800e12cc_slot0f control: different 2000 of 2000 (expected more than 0)
  altered: the function returns its result + 1, instruction slot 30
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

The write audit (`--cases 2000 --writes FUNC`, seed 1):

```
func_8001385c_slot01 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8001714c_slot28 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800205b4_slot28 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e11e4_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e1250_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_800e12cc_slot0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8012126c writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801212c8 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801212e8 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80146794 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8014dcc0 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b12bc_slot04_0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
```
