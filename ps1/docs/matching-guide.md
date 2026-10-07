# Matching guide

How to reconstruct one unit of the PS1 executable as C that compiles to the
exact original bytes. Contract of the tools: [matching build](matching-build.md).
This guide holds no game data. Addresses, names and sources are in `ps1/src/`.

## Goal

A unit is done only when `matchbuild.py` exits 0 with every function of every
unit exact and the executable hash equal to the baseline. A partial match is
progress to report, not a result.

## Workspace

Each unit is worked on in its own configuration directory, a working copy of
the main one that is not committed, with its own build tag. Nothing else is edited. The
directory holds:

- `build.toml`: units and their functions (name, address, size).
- `types.fields`: field table for shared structs. The build generates
  `types.gen.h` from it. Never write a shared struct by hand in C. The one
  exception is `PrimTag` in `game.h`: it has bit-fields, which the field
  table cannot express.
- `symbols.ld`: address of every symbol not defined by a unit.
- `object.h`, `game.h`: includes, externs and prototypes only, and that
  one type.
- one `.c` file per unit.

## Loop

```sh
PY=<python>; T=<tools directory>; C=<config directory>/build.toml; TAG=<tag>
$PY $T/matchbuild.py --config $C --tag $TAG                      # once, and again at the end
$PY $T/fndiff.py --rebuild --config $C --tag $TAG <unit> [function]   # every try
```

Build the whole image once: the loop reads the other units' objects from
that build. Then each try is `fndiff.py --rebuild`, which compiles your unit
alone and shows an aligned instruction diff, even when sizes are wrong.
`--all` also prints matching lines. It decides nothing and it removes the
tag's report, so finish with the whole build: only that says the unit is
exact, and only that shows what a changed header does to other units.

## Sources of truth

- The disassembly listing, one instruction per line, address first,
  delay-slot instructions marked with a leading underscore.
- The function table with start addresses and sizes.
- Decompiler pseudocode, where it exists, is a candidate only. Check argument
  registers, field widths and signedness against the disassembly.

## Conventions

- Function not understood yet: `func_<address>`. Data: `data_<address>`.
  Struct field: `field_<offset>`. Do not invent meanings. A name states a role
  only when the code shows it.
- To add a field, add a line to `types.fields`: offset, type, name. Keep every
  existing line. Gaps become padding automatically.
- To reference something outside the unit, add `name = 0x...;` to
  `symbols.ld`. Never list a function that a unit defines.
- Prototypes of functions from other units come from the shared header
  `protos.h` in the config directory. A unit includes it right after
  `game.h`. It holds one prototype per function: the one that keeps every
  unit exact. Look there before declaring anything. Put a prototype at the
  top of your own `.c` file only for a function the header does not have.
  If your code needs another parameter list or return type for a function
  than the header gives, leave `protos.h` out of that unit, declare what the
  unit needs, and say so in the report. The disagreement is a finding.
- Data externs that every unit declares the same way come from the shared
  header `externs.h`, included after `game.h` like `protos.h`. Declare in
  your own `.c` file only what it does not have. A symbol that you need
  with another type or shape than the header gives (an array where it has a
  scalar, a struct where it has a pointer) is a reason to leave `externs.h`
  out of that unit and declare your own, and to say so in the report: the
  two forms compile differently, and which units need which is a finding.
  `externs.h` also defines five function pointer typedefs (`ObjectFn`,
  `HandlerFn`, `FrameFn`, `ObjectFnInt`, `UnitFn`) and declares the callback
  tables that use them. Do not define one of those typedefs in a unit that
  includes the header: this compiler rejects a repeated typedef, even an
  identical one.
- Plain C89. Hex for values that are identifiers or masks.

## Units of a module image

Code of an overlay module is matched the same way. What differs:

- The unit carries `image = "<image>"` in `build.toml`, and its source sits
  in the folder `<image>/` of the config directory.
- It includes the shared headers with their path: `"../game.h"`,
  `"../protos.h"`, `"../externs.h"`. Without the `../` the preprocessor finds
  a file of the same name in the SDK headers and the unit compiles against
  the wrong declarations.
- Modules overlap in memory, and names are unique over all images. A module
  function that is not understood yet is `func_<address>_<image>`, its data
  `data_<address>_<image>`. A name of the resident image at the same address
  in `symbols.ld` is another symbol and stays.
- Functions of the resident image are called by their names. The build gives
  every link the declared functions of the other images.
- `matchbuild.py --image <image>` builds the module alone in a few seconds.
  Use it as the whole build of the loop. The build of every image is still
  what a merge has to pass.
- `slot17` is declared as like `slot16`: every unit of `slot16` is linked a
  second time at the other address and compared with the chunk of slot
  `0x17`. A new unit of `slot16` has to be exact there too.
  `fndiff.py --image slot17 <unit>` shows the unit as the second link has
  it. A name of `symbols.ld` outside the module that lies elsewhere on the
  second side gets its address in the table of that image in `build.toml`.
- The function table of a module comes from a sweep and is an estimate. A
  function that clearly ends elsewhere than its row says is a finding to
  report, not to work around.

## What is allowed

Reordering statements, changing expression shape, types and signedness,
splitting or merging conditions, `if/else` versus early assignment, loop
forms, locals versus repeated member access, and struct versus separate
externs for globals.

Not allowed: inline assembly, embedded bytes, `volatile`, `register`, dummy
operations, or anything whose only purpose is to steer the compiler and that
a reader could not take for ordinary code. If nothing else works, stop and
report.

One dummy construct is accepted, and only this one: an unused local array
that makes the function reserve the stack space that the original reserves
and never uses. It is the last resort for a function whose only difference
is the size of its frame, after the ordinary reasons for a larger frame
have been looked for and not found: a local whose address is taken, a
struct held on the stack, the argument area of a call with more than four
arguments. It is named `unused`, and this comment stands above it:

```c
/* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
```

The comment means what it says. An exact build with the array shows that
this source is compatible with the original's bytes, not what the original
source had there. `grep -rn "unused\[" src` lists the uses.

## What this compiler does

GCC 2.6.3, `-O2 -G0`, assembler behaviour of ASPSX 2.21 or older.

- Memory is reloaded after every store through a pointer. Repeating
  `object->field` in the source is normal and usually required.
- A table lookup compiles from `table[object->index]`. The four-instruction
  form through `$at` with a preceding `nop` is the assembler's expansion.
- Byte fields load with `lbu` when unsigned and `lb` when signed. Sixteen-bit
  fields load with `lhu` or `lh`. Pick the field type from the load.
- A block of globals addressed from one base register is one struct. Separate
  externs for its members give different register use.
- Identical tails before a common jump are merged. If the original keeps two
  identical blocks apart, the source reached them differently.
- A zero stored from a register rather than `$zero` came from a variable.
- Expression form, type and surrounding code all change what a decrement
  compiles to. One pattern is confirmed on a small fixture, for an unsigned
  byte field only:

  ```c
  struct S { unsigned char x; };
  void a(struct S *p) { p->x--; }          /* addu $2,$2,-1  */
  void b(struct S *p) { p->x -= 1; }       /* addu $2,$2,255 */
  void c(struct S *p) { p->x = p->x - 1; } /* addu $2,$2,255 */
  ```

  The same three forms on an unsigned 16-bit field all add minus one in that
  fixture, and so does the explicit form on a signed byte. Inside larger
  functions other forms have been needed. Do not treat this as a general
  mapping: read the constant in the listing and try the forms.
- A shifted value in two registers (`sll $a1,$v1,3` and, in the delay slot
  of the next branch, `move $v1,$a1`; a compare reads the first, a store
  and the code after a join read the second) was rebuilt in one function
  from one signed 16-bit local scaled in place: `t = table[i]; t <<= 3;`.
  `t = t << 3` gave the same code there; `t *= 8`, `t = t * 8`, an
  unsigned 16-bit local, and separate locals for the product and its copy
  did not. One function is thin evidence: try the spellings, do not treat
  this as a mapping.
- The first word of a drawing primitive and of an ordering-table entry
  holds the address of the next entry in its low 24 bits and a length in
  its high 8. Where the listing masks such a word with `0xffffff` and
  `0xff000000` and ORs the halves together, write a bit-field store through
  `PrimTag` of `game.h`, not the masks:

  ```c
  ((PrimTag *)p)->addr = (u32)next;              /* link p to next */
  ((PrimTag *)p)->addr = ((PrimTag *)ot)->addr;  /* add p to the entry ot: */
  ((PrimTag *)ot)->addr = (u32)p;                /* two statements, this order */
  ```

  Masks written by hand gave the same instructions with the two constants
  in each other's registers, or with a load on the other side of a store,
  in the functions of the module of slot `0x12` that were tried both ways.
  The SDK's header, as the reference project has it, declares this word
  with bit-fields and stores through them in its macros. `PrimTag` is this
  project's own declaration of the layout; that the original used the
  SDK's macros is an inference from the code they give.
- Two variables that hold one pointer are two registers to the compiler
  while it orders instructions (inferred from what it emits). A store
  through one and a load through the other stay in source order; through
  one name the load moves ahead of the store when that fills a load delay.
  One function was rebuilt only from

  ```c
  other->sequence = obj->sequence;   /* other is a global: store through it */
  p = other;                         /* then read it into a local */
  if (p->side != 0) { ... }
  f(p, ...);
  ```

  where `p = other; p->sequence = ...;` and the global at every use both
  differed. The signs in a listing: a load that stays behind a store to
  another offset of the same register, with a `nop` before it that it
  could have filled; the pointer in a register that is not the argument
  register, passed with a `move` in the delay slot of the call; one load
  of the pointer where reading the global at every use gives two. The
  same form through a parameter and a local copy of it, made after the
  first use, decided one more function. A local copy made before the
  first use is merged away and changes nothing.
- The order of independent loads and stores inside one block of the
  listing is not evidence of the order of the statements in the source:
  the compiler moves loads up and interleaves the statements of a block
  to fill load delays. So statement order is something to try, like a
  spelling, and not something to read off the listing. Two observations,
  each from one function, neither of them a rule:

  - A candidate loaded a field again (`lh`) where the original tests the
    value from the register that the decrement left it in. The candidate
    had the statements in the listing's order, with two updates of other
    fields between the decrement and the test. With the decrement
    directly before its test, the function is exact:

    ```c
    obj->field_24 += 0x10;
    obj->field_20 += 0xffff;
    obj->field_22 += 0xffff;
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) { ... }
    ```

    What this shows is narrow. The source reads the field a second time,
    and whether the compiler loads it again there depends on whether a
    store that it takes as able to touch that field stands between the
    two reads. A value copied into a local is another case: the compiler
    keeps a local across a store, and in this function a local gave other
    registers.
  - In another function a constant store moved from the top of a block
    to directly before an increment, and the two values got the registers
    the original has.

  Trying another order is a heuristic that worked twice. An exact build
  with one order shows that this order is compatible with the original's
  bytes, not that the original source had it.
- Two stores to one field. One fixture, and what it does and does not
  show:

  ```c
  struct S { int a; int b; };
  void adjacent(struct S *p) { p->a = 0; p->a = 0x20; }              /* one store: the second */
  void between(struct S *p) { p->a = 0; p->b = 0xfff; p->a = 0x20; }  /* three stores */
  ```

  In `between` the compiler emits the store to `b` first and the two
  stores to `a` after it, next to each other. So a pair of stores to one
  field in a listing is compatible with a source in which another store
  stood between them. It does not prove that one did: with a `volatile`
  struct the compiler keeps both stores of `adjacent` as well, and other
  types or forms may do the same. Moving a store to another field between
  the two is a heuristic to try when a build has one store where the
  original has two. It made two functions exact, one of the module of
  slot `0x0` and one of the module of slot `0x2a`; what the original
  source had there is not known.
- A narrow parameter whose callers pass the argument as it is, without the
  mask or the extension that a prototype with the narrow type makes them
  emit, is an `int` parameter copied into a narrow local:

  ```c
  void func_801307e0(Object *object, int arg) {
      u16 index = arg;
      ...
  ```

  The definition masks once at entry, as with a `u16` parameter, and a
  caller that has the prototype in scope emits nothing. A caller that does
  mask casts at the call: `func_801307e0(object, (u16)index);`. The same
  holds for a result: a function that returns a narrow local as `int` lets
  the callers that mask the result write `(u8)f()` and the others not.
  Two functions were given this form for a parameter and one for a result
  when the declarations of the module units were checked, and with it no
  unit of a module image declares a function without a prototype. Before
  it was found, one declaration was left without a prototype with a note
  that both a narrow and an `int` parameter had been tried; the `int`
  parameter had been tried without the local.
- One local that takes every step of a computation in place keeps
  instructions and registers that one expression, or a fresh local per
  value, does not. Two cases from the stage modules:

  ```c
  /* a quotient in the listing's register: `n = (a - b) / 144;` is not */
  n = layer->field_60;
  n -= *(s32 *)&other->field_08;
  layer->field_68 = 0;
  layer->field_6c = 0;
  n /= 144;

  /* a sum whose operands land in the listing's registers only this way */
  s16 d;
  d = l2->field_22;
  d -= l2->field_0a;
  d -= d / 4;
  d += layer->field_0a;
  d += layer->field_36;
  layer->field_22 = d;
  ```

  In the second the local is narrow where the listing extends the value
  after computing it (`sll 0x10`, `sra 0x10` after the subtraction). The
  same holds for a byte: a `u8` local that takes a field where the
  listing has its `lbu`, and is stored back later, keeps a load in place
  that the compiler otherwise moves to the top of the function. These are
  heuristics that worked: the second on 16 second attempts in nine stage
  modules and on later first attempts, the `u8` local on three functions.
  What the original source had is not known. A fourth case is measured
  and not yet part of an exact function: `pos &= 0xffff; pos |= t << 16;
  y = pos >> 16;` keeps an `or` that the one expression
  `((x & 0xffff) | (t << 16)) >> 16` loses.
- Whether a constant or an address is formed inside a loop or kept in a
  saved register across it depends on the size of the loop when the
  compiler's loop pass looks at it. Measured on one function with the
  pass's dump (`cc1 -dL`): two pointer assignments moved from before a
  loop into it took the loop from 21 instructions to 23, and a constant
  that had been moved out of the loop stayed inside, as in the listing.
  As a heuristic: where a build has a constant outside a loop and the
  listing has it inside, write the loop larger, and the other way round.
  It made eight functions of the stage modules exact.
- The operand order of an `addu` that forms an address followed the order
  of the terms only when the arithmetic went through an integer type:
  `(Tx *)(i * 0xfc0 + (u32)(t + j))`. With pointer arithmetic, pointer
  casts or variables for the offsets the order did not move. 19 units of
  the stage modules have this form.
- The order of stores in a listing is the scheduler's, not the source's.
  A function that sets many byte fields shows the stores grouped by
  register: the stores of one repeated constant first, then loads, then
  the other constants, then the zero stores. Written in that order, with
  or without a local for the repeated constant, the registers of the
  character blocks' state setters came out swapped. Written as plain
  statements with literal constants in ascending field order, as the
  sibling functions set a state (`field_04 = 1; field_05 = 0; field_06 =
  7; field_07 = 0; ...`), they were exact: the repeated constant then
  lives across the others and takes the second register. Where that
  order is still off, the zero stores are the ones to move: they bind
  no register, so only their place among the others matters. Measured
  first on `func_801b217c_slot04_10`.
- When every path of a function ends in the same call, the call is
  written in each arm, and the arm that the listing has last, the one
  that falls into the `jal`, is the last arm in the source. A listing
  that seems to keep a second pointer to the object in `a0` through the
  last block (`move a0,s0` in a delay slot, then loads through `a0`) had
  no second pointer in `func_801b0ef4_slot04_10`: the copy is the
  argument of the last arm's call, moved up. A shared call after the
  `if`, a pointer copy, or the arms the other way round give the right
  size with other registers or another block order.
- In the small functions that add speeds to a position, a field that the
  listing loads again before a store was the compare written the other
  way round: `if (obj->field_70 <= obj->pos_y)` was exact where `pos_y >=
  field_70` was not. Try the operand order of the listing's `slt` before
  anything else. Some of these functions are exact only when they return
  their last update, `return obj->field_50 += obj->field_58;`, and their
  callers test the result.
- The types of the locals come before the statements. Where the
  instructions are right and a register or the order of two of them is
  not, sweep each local over `int`, `u16`, `s16` and `u8` in both
  declaration orders before trying other spellings. An `s16` local that
  takes every step in place, a `u8` local for a byte used four ways, and
  two `u16` locals each made a function exact that other
  spellings had not moved.
- A zero that the listing keeps in a register over two clearing loops
  (`move a2,zero` at entry, `move a0,a2` before the second loop) needs a
  local for the zero and a copy of it taken in each loop's
  initialisation: `int z = 0; u8 c; u8 d; for (c = z, i = 0x3f; i >= 0;
  i--) *p++ = c; ... for (d = z, i = 0x17; ...) *p++ = d;`. A literal
  zero in either loop folds the copy away.
- Blocks that exist when registers are allocated and are gone in the
  listing leave a trace in the allocation. `func_801b460c_slot04_0c` has
  one store of `pos_x` to itself, keeps the parent pointer in `a1` and
  every temporary in `v0` and `v1`. As one statement `obj->pos_x =
  obj->pos_x;` the pointer is a one-block value and the temporaries
  spread over `a0` and `a1`. As the facing-dependent offset that the
  sibling functions have, with zero in both arms, it is exact: the arms
  are separate blocks until the late jump pass merges them. This is an
  inference from the allocation, and the comment at the statement says
  so. `grep -rn "The listing has one store of pos_x" src` lists the uses.
- Branch order in the listing follows source order of `if / else if` chains.
- The value in a delay slot belongs to the instruction before it in program
  order, not after.

## Before reporting

Run the build one last time after your final edit and report from that
output. A function is exact only if that last build says so. Earlier results
do not count.

## When stuck

Keep a short log per stubborn block: what was tried and what changed. After
about eight materially different attempts on the same block, stop and report
the best candidate with its `fndiff` output and the log. Do not permute
blindly.

Stop at once and report options instead of choosing when: a struct layout
contradicts an existing field, the evidence points to different compiler flags
or version, the build tool would need a change, or matching seems to need a
second symbol for one address.

## Report

Per function: exact or not. The final build summary lines. Fields and symbols
added. For any residual: the `fndiff` excerpt and the attempt log. Judgment
calls, flagged.
