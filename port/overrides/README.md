# Overrides of the port

An override in C stands in the PC program for a function of the game whose
exact C cannot run there as written. Each function of this folder is the
exact C of its unit with one change, written beside the exact C and not in
it. There are four kinds of change.

The argument: the argument that the console's code leaves in the register
`a0` is passed in the call. On the console the callee reads that register
and finds in it what the caller's code left there: its own first
parameter, or, when a call lies before, what that earlier call left in it.
Compiled for a PC, the callee reads something else.

The result: the function returns the result of its last library call. The
unit defines it `void`, and the original's callers read the result
register, which on the console still holds that result. Compiled for a PC,
a function defined `void` hands its caller nothing.

The local without a value: a local gets a value at its declaration. The
unit declares it without one, and on one path the original uses the
register that holds it before anything was written to it, so that it has
what the caller's code left there. Compiled for a PC, such a local is
whatever the compiler's code happens to hold.

The shift count: the count of a shift is masked with 31. The unit shifts
by a count that can be 32 or more, as the original does, and the console's
shift instruction takes the count modulo 32. In C a shift by 32 or more has
no defined result, so compiled for a PC nothing ties the result to the
console's.

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

The one that gives a local a value:

| Function | Unit of its exact C | What the override adds | The path | What the original uses there |
| --- | --- | --- | --- | --- |
| `func_801347b4` | `ps1/src/d347b4_r1.c` | `col` is 0 at its declaration | the map has no rows and the kind is 2 or 3: the loops do not run, and `func_801350c0` is called with `col` | the low byte of `s7`, which the function writes first inside the loop over the rows (`andi a1,s7,0xff` at 80134e5c) |

Its test cannot compare that path: the original's value there depends on
the caller, so the setup makes at least one row when the kind is 2 or 3
(the header of the `.c` lists it under "Excluded"). On every other input
the override does what the original does, and that is what the test shows:
the callees `func_80134e94` and `func_801350c0` run as the original code,
`func_8015bf34` is a recorder. The control alters the build of the
override: the constant 0x7807 of the page word becomes 0x7809. The built
code is 4 bytes longer than the original (the one instruction that sets
`col`). Its contract was the contract of the readable C that stood for
this function before its unit was exact; that C also passed 0 on that
path.

The ones that mask a shift count:

| Function | Unit of its exact C | What the override adds | The inputs | What the original does there |
| --- | --- | --- | --- | --- |
| `func_80141534` | `ps1/src/e41534_r1.c` | `& 31` on the count of the shift in the return statement | the count is the first byte of `dir` halved, plus 6 when the second byte is not 0, plus 1 when flag 0x8000 is clear: 32 or more whenever the first byte is 64 or more | `srav` at 801415ac; the console takes the count modulo 32 (read from the listing) |
| `func_8013bfa4` | `ps1/src/d3bfa4_r1.c` | `& 31` on the count of the shift in the test before the state bytes (`0x800000 >> field_02`) | the count is the table object's `field_02`, a byte: 32 or more whenever that byte is | `srav` at 8013c4ec; the console takes the count modulo 32 (read from the listing) |

Its test compares those inputs too: the setup draws the first byte of
`dir` from 0 to 255, so most cases that reach the shift have a count of 32
or more, and on them the override gives what the original code gives (the
first command's lines below). The control alters the build of the
override: the final `xori 1` becomes `xori 0`. The built code is 4 bytes
longer than the original (the one instruction that masks). Its contract
was the contract of the readable C that stood for this function before its
unit was exact; that C also wrote the mask.

The setup of `func_8013bfa4` draws `field_02` as 0x17 in one case of four
and from 0 to 40 otherwise (its contract, `func_8013bfa4.py`, says so), so
counts of 32 and more are among the cases that reach the shift, and on
every case the override gives what the original code gives (the first
command's lines below). Its control alters the build of the override: the
constant 0x17 becomes 0x16. The built code is 4 bytes longer than the
original (the one instruction that masks). Its contract was the contract
of the readable C that stood for this function before its unit was exact;
that C also wrote the mask.

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

## Fixtures

A function that has fixtures has a file `FUNC.fixtures.json` beside its
`FUNC.c`. Replay runs the C of the function on the recorded inputs and never
runs the original code. It still needs the private inputs of the build
configuration and the PS1 compiler. From the tool's folder
(`ps1/src/slot06_nonmatching/`), for this folder and for one function:

    python -B difftest.py --config ../build.toml --folder ../../../port/overrides --replay
    python -B difftest.py --config ../build.toml --folder ../../../port/overrides --replay FUNC

For a new or changed function, record once (the wide comparison, then the
fixtures it keeps), then replay:

    python -B difftest.py --config ../build.toml --folder ../../../port/overrides --cases 2000 --jobs 4 --record FUNC

A pass is evidence for the recorded inputs. It is not the wide comparison and
it is not equivalence. Details of the files are on the page of the tool's folder.

Functions of this folder without a fixtures file, by reason:

- The file is larger than 100,000 bytes, also with `--max 6` (or with its one case). It is not kept. `func_800e12cc_slot0f`, `func_801347b4` (so listed on the page of `ps1/src/resident_nonmatching/` while the function stood there; not recorded again for this folder).
- The cases, or a callee they run, read bytes that the setup did not make. Those bytes come from the game's memory image, so a file for the function would publish them and none is kept. `func_8001385c_slot01`, `func_8001714c_slot28`.

What the replay command of the folder printed on 2026-10-10, one line per function:

```
func_800205b4_slot28 replay: fixtures 2, passed 2, failed 0
func_800e11e4_slot0f replay: fixtures 6, passed 6, failed 0
func_800e1250_slot0f replay: fixtures 12, passed 12, failed 0
func_8012126c replay: fixtures 2, passed 2, failed 0
func_801212c8 replay: fixtures 1, passed 1, failed 0
func_801212e8 replay: fixtures 1, passed 1, failed 0
func_80141534 replay: fixtures 4, passed 4, failed 0
func_80146794 replay: fixtures 3, passed 3, failed 0
func_8014dcc0 replay: fixtures 12, passed 12, failed 0
func_801b12bc_slot04_0f replay: fixtures 2, passed 2, failed 0
```

The replay of `func_8013bfa4` alone, on 2026-10-11, printed
`func_8013bfa4 replay: fixtures 10, passed 10, failed 0`.

## The recorded lines

The three blocks are a record, not an instruction. They are what the
commands printed for each function when it was last run. For the nine
functions of the argument kind that is one run of the whole folder on
2026-10-10 (`--seed 1 --all` in place of FUNC); the page keeps those lines
as printed. The lines of the three result overrides are from their own
runs on 2026-10-10 (seed 1; their seed 7 runs also printed `different 0`
and `outside 0`). The lines of `func_801347b4` are from its own runs on
2026-10-11 (seed 1; its seed 7 runs also printed `different 0` and
`outside 0`), and so are the lines of `func_80141534` and of
`func_8013bfa4`. From now on a function's lines change only when that
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
func_801347b4: built 1764 bytes, original 1760 bytes; cases 2000, discarded 0, equal 2000, different 0
func_801347b4 coverage: 436 of 440 instruction slots of the original executed
func_8013bfa4: built 1804 bytes, original 1800 bytes; cases 2000, discarded 0, equal 2000, different 0
func_8013bfa4 coverage: 450 of 450 instruction slots of the original executed
func_80141534: built 144 bytes, original 140 bytes; cases 2000, discarded 0, equal 2000, different 0
func_80141534 coverage: 35 of 35 instruction slots of the original executed
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
func_801347b4 control: different 711 of 2000 (expected more than 0)
  altered: page word constant 0x7807 changed to 0x7809, instruction slot 124
func_8013bfa4 control: different 147 of 2000 (expected more than 0)
  altered: kind constant 0x17 changed to 0x16, instruction slot 297
func_80141534 control: different 1514 of 2000 (expected more than 0)
  altered: final inversion xori 1 replaced by xori 0, instruction slot 32
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
func_801347b4 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8013bfa4 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80141534 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_80146794 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_8014dcc0 writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
func_801b12bc_slot04_0f writes: cases 2000, discarded 0, outside 0 (largest 0 bytes)
```
