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
arguments, a signed 16-bit local that is compared and used again (see
"What this compiler does"). It is named `unused`, and this comment stands
above it:

```c
/* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
```

The comment means what it says. An exact build with the array shows that
this source is compatible with the original's bytes, not what the original
source had there. `grep -rn "unused\[" src` lists the uses.

One construct is accepted for one case: a call that passes another number
of arguments than the callee's definition has. Every function of the tree
is declared with a prototype, so such a call cannot be written plainly. It
is written through a cast of the callee, and the function that holds it
has a comment directly above it whose first sentence is fixed and whose
second gives what was measured:

```c
/* The call of func_80131094 passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 1 instruction slots. */
void func_800205b4_slot28(Object *obj) {
    func_80020608_slot28(obj);
    if (obj->pos_y >= 0xd0) {
        obj->field_04++;
    }
    ((void (*)(void))func_80131094)();
}
```

Two functions of the module of slot `0x28` have it, both at a call of
`func_80131094`, which uses its parameter: what it receives there is
whatever the code before the call left in the register. The comment says
what the original does at that call. It does not say how the original
source was written; a file that declares the callee without a prototype
compiles such a call from plain C. Use it only after two checks: the plain
call was measured (the cast stays only if the function is exact with it
and not without it), and the callee does not simply take another
parameter list. In the same module three helpers that their callers pass
the object to, although they do not use it, got an unused parameter in
their definition instead, and every caller is written plainly.
`grep -rn "The call of" src` lists the uses.

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
  - In a function of the module of slot `0x27`, `func_80015838_slot27`,
    one store of an address constant (`b->field_90 = (void *)0x80038000;`)
    was put at each of the ten places among the nine other statements of
    its block. Two places give the exact function, the two directly after
    the store of a small constant to `field_09`; the other eight leave the
    constant and a loaded byte in each other's registers or change more.

  - In `func_8001bd48_slot28`, 908 bytes with five blocks that each set
    up a new object, one store of the constant 2 (`p->field_03 = 2;`) was
    moved from directly after the store to `field_02` to directly before
    the other store of 2 in its block: 107 instruction slots differ, the
    register of the object pointer in all five blocks among them. Two
    agents had credited the match of this function and of a like one to
    narrow locals that held the constants; with literals in their place
    both functions build the same bytes, and the locals were taken out
    again.

  Trying another order is a heuristic that worked in these four
  functions; the second attempts on the module of slot `0x27` report it
  for four more, and the batches of the module of slot `0x28` report one
  order for most blocks that set up a new object: the store of 1 to the
  first byte first in the block, and the store that the original has in
  the delay slot of the block's call last before that call. An exact
  build
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
  parameter had been tried without the local. A third function,
  `func_80130768`, got the form for its index in the round of the module
  of slot `0x27`: two functions of that module pass an `int` without
  extending it, and their candidates were exact as they stood once the
  parameter was an `int`. One exact caller in the module of slot `0x12`
  does extend, and got `(s16)(...)` at the call.
- A global that the original loads again after a store. Two measurements,
  neither a rule:

  ```c
  struct O { short x; short y; };
  extern int scalar_word;
  extern int array_word[];
  void use_scalar(struct O *o) { scalar_word = scalar_word * 2; o->x = scalar_word; o->y = scalar_word; }
  void use_array(struct O *o)  { array_word[0] = array_word[0] * 2; o->x = array_word[0]; o->y = array_word[0]; }
  ```

  `use_scalar` loads the word once after its own store and uses the
  register for both stores to `o`. `use_array` loads it again after the
  store to `o->x`; so do `*(int *)array_word` in place of `array_word[0]`
  and a member of a global struct. The second measurement is on
  `func_80015070_slot27`: after a store through an `int *` local that
  points to another global word, `data_80190464[0]` is loaded again and
  the unit is exact, 216 bytes; with `*(int *)data_80190464`, or with the
  local pointer, at the same two places it is 204 bytes. Four words of the
  game state that the module of slot `0x27` uses as scratch
  (`data_8019045c`, `data_80190460`, `data_80190464`, `data_8019046c`) are
  declared as arrays of `int` for that reason, and each use picks the
  element or the cast. By the reports of the agents that wrote them, the
  element decided three second attempts of that module
  (`func_800139a8_slot27`, `func_8001276c_slot27`,
  `func_80013198_slot27`). What the original declared there is not known;
  a member of the game state's struct would behave like the element in
  the fixture.
- A signed 16-bit local that is compared and then used again costs 8 bytes
  of frame that the code never uses, and a second register that holds a
  copy of the value. Fixture, frames of leaf functions:

  ```c
  void f_int(struct O *o)   { int x = o->pos_x; if (x < gi + 5) o->pos_x = x + 8; }   /* frame 0 */
  void e_arith(struct O *o) { s16 x = o->pos_x; if (x < gi + 5) o->pos_x = x + 8; }   /* frame 8 */
  void c_cmp(struct O *o)   { s16 x = o->pos_x; if (x < 5) g = 1; }                   /* frame 0: not used after the compare */
  void a_copy(struct O *o)  { s16 x = o->pos_x; g = x; }                              /* frame 0: no compare */
  void h_u16(struct O *o)   { u16 x = o->w; if (x < gi + 5) o->w = x + 8; }           /* frame 0 */
  void k_param(struct O *o, s16 x) { if (x < gi + 5) o->pos_x = x + 8; }              /* frame 0: a parameter */
  ```

  A `u8` local in the place of the `u16` gave frame 0 as well. In the
  tree, `func_800169c4_slot27` has two such locals and the original's
  frame of 0x30; with `int` in their place it builds with a frame of 0x20
  and 12 bytes shorter. This is one ordinary reason for a frame that looks
  unused, to check before the stand-in. It is not the reason for every
  such frame: the eight functions of the module of slot `0x27` that carry
  the stand-in were tried with it, about ten forms each, and none became
  exact without the array. Four of the eight contain no branch at all.
  In the module of slot `0x28` one function has its frame from such a
  local and no stand-in: `func_800279d4_slot28` is exact with `s16 h`;
  with `int h` it builds 304 bytes against 324, with a frame of 0x30
  against 0x38.
- One pointer local, assigned again right before each use, against a
  store written through the global each time. In `func_8001e85c_slot28`

  ```c
  p = data_80051a98_slot28[0];
  p->field_48 = 0xff;
  p = data_80051a98_slot28[1];
  p->field_48 = 0xff;
  ```

  is exact, and `data_80051a98_slot28[0]->field_48 = 0xff;` with the
  second store written alike differs in 10 instruction slots: the pointers
  and the constant are in each other's registers. By the agents' reports
  the same form, one local for every pointer of the function, decided
  eleven second attempts on that module. It is a heuristic for a function
  whose only difference is which register holds a loaded pointer.
- A helper that is passed the object although it does not use it. In
  `func_80018c70_slot28` the first call is `func_80019090_slot28(obj);`
  and the callee, which reads only globals, has an unused parameter. With
  the call written without the argument the function differs in 8 slots:
  the argument register is then free where the original keeps the object
  in it, and a pointer that the function loads later lands in it. The sign
  in a listing: a value in the second or third argument register where
  nothing seems to occupy the first. Three helpers of that module got the
  parameter for this reason; what their original declarations were is not
  known.
- A pointer that is stepped between two calls. In `func_80012668_slot01`

  ```c
  func_80157fc4(&r, data_80015440_slot01[o->field_d4]);
  p = data_801a2964;
  func_80158028(&r, p);
  p += 0x1400;
  func_80158028(&r, p);
  ```

  is exact. With `p = data_801a2964;` before the first call and
  `p + 0x1400` as the argument of the last, 5 instruction slots differ:
  the address of the buffer and the address of `r` are loaded at other
  places. By the agents' reports two neighbours of the same shape,
  `func_8001257c_slot01` and `func_800125f4_slot01`, became exact the same
  way. It is a heuristic for a function that passes a buffer and the same
  buffer at an offset to two calls.
- The places of a few statements, tried exhaustively. In
  `func_800121e0_slot01` a run of thirteen statements sets up an object;
  four of them store constants to `field_4c`, `field_50`, `field_54` and
  `field_58`. A candidate with three of the four written first differed
  in 5 slots; with the four written near the end in the order of the
  fields, in 12. It is exact with the four near the end in the order
  `field_4c`, `field_54`, `field_50`, `field_58`. That was found by
  building every order of the four, put before or after a run of eight of
  the other statements at each of the five ways to split them, at most
  120 arrangements, and stopping at the first exact one. About a dozen
  hand-written forms and six minutes of random search had not found it.
  The search moves statements and writes nothing that the source did not
  have. It is a heuristic for a function whose instructions are right and
  whose constants sit in other registers or other places; on three other
  functions of that module, with 600 to 950 arrangements each, it found
  nothing.
- One local for two values that follow each other in one register, and a
  store that the compiler takes to reach a global. `func_80014b28_slot01`
  begins

  ```c
  t = obj->field_3c;
  data_80033bb0_slot01 = t;
  t = (void *)0x800767c0;
  *(u32 *)&obj->field_90 = (u32)t;
  data_80033bb0_slot01->kind = obj->field_48;
  ```

  The original builds the constant in the register that held the loaded
  pointer, after the store to the global, and loads the global again
  before the store to `kind`. Without the local (`data_80033bb0_slot01 =
  obj->field_3c;` and the constant stored as a literal) the build loads
  the constant first and is 4 bytes short. Of 32 spellings of the store
  of the constant and of the three places that use the global, 5 are
  exact, all with the global declared as a plain pointer; with the global
  declared `ObjectRef` and used as `.p` at all three, the build keeps the
  global's address in a register and 5 slots differ. Inferred from these
  builds, not known otherwise: this compiler takes a store through a
  `u32 *` to reach a plain global, and a store to a field of a struct
  through a pointer not to. The store through the cast is in the form
  the rules ask for: the field is named and its address is cast.
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
- Where an instruction stands in its block depends on the type of the
  local it computes. This is read from the compiler's source (GCC 2.6.3,
  `sched.c`, `adjust_priority` and `birthing_insn_p`), not inferred from
  listings: the first scheduling pass moves an instruction as late as its
  users allow when its destination is a plain register that is assigned
  exactly once in the function. An instruction whose destination is a
  16-bit local written from a word operation, or a local assigned twice,
  is not moved and keeps its place in source order. Priority otherwise is
  the longest chain of load latencies before the instruction, and ties go
  by original order. Three consequences decided the stage drawing
  functions:
  - A value that the original computes earlier in its block than the
    build does is a `s16` or `u16` local where the candidate has `int` or
    `u32`. `func_801e82c8_slot06_00` was exact when the tile mask, the
    column counter, the pixel offset and the difference of the scroll
    were 16-bit, with the statements in the listing's order; its comment
    gives what each costs as a word. With 16-bit locals the listing's
    order is the source's order, so write statements as the listing has
    them and do not reorder to steer registers.
  - A value that the listing sign-extends (`sll 16`, `sra 16`) or masks
    (`andi 0xffff`) after computing it is a 16-bit local. A local that
    gets no register has an 8-byte stack slot, and a 16-bit one is
    stored there with `sh`; slots are handed out in ascending number of
    the pseudo-registers, which for locals is the order of their
    declarations (`reload1.c`, `alter_reg` and its caller).
  - Local allocation takes values in the order of `floor(log2(refs)) *
    refs / length of life`, the lowest free register first
    (`local-alloc.c`, `qty_compare`); a destination can share the
    register of an operand that dies in the instruction, and the
    operands are tried in order. In the drawing functions a difference
    written `t = a - t` landed in the register of `t`; written into a
    local of its own it landed in the register of `a`.
- Branch order in the listing follows source order of `if / else if` chains.
- The value in a delay slot belongs to the instruction before it in program
  order, not after.

## Before reporting

Run the build one last time after your final edit and report from that
output. A function is exact only if that last build says so. Earlier results
do not count.

Reduce an exact function before reporting it. Take out, one at a time,
each thing that the plain source would not have: a local for a constant,
a field read into a local before the stores, a cast, a statement away
from its natural place. Build again each time and keep the plainer form
when it is still exact. At each thing that has to stay, write a comment
with the measured effect, for example "with the literal two instruction
slots differ". A form that an earlier attempt needed is often not needed
by the attempt that is exact.

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
