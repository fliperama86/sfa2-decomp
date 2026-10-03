# Matching guide

How to reconstruct one unit of the PS1 executable as C that compiles to the
exact original bytes. Contract of the tools: [matching build](matching-build.md).
This guide holds no game data. Addresses, names and sources stay private.

## Goal

A unit is done only when `matchbuild.py` exits 0 with every function of every
unit exact and the executable hash equal to the baseline. A partial match is
progress to report, not a result.

## Workspace

Each unit is worked on in its own private configuration directory, a sibling
copy of the main one, with its own build tag. Nothing else is edited. The
directory holds:

- `build.toml`: units and their functions (name, address, size).
- `types.fields`: field table for shared structs. The build generates
  `types.gen.h` from it. Never write a shared struct by hand in C.
- `symbols.ld`: address of every symbol not defined by a unit.
- `object.h`, `game.h`: includes, externs and prototypes only.
- one `.c` file per unit.

## Loop

```sh
PY=<python>; T=<tools directory>; C=<config directory>/build.toml; TAG=<tag>
$PY $T/matchbuild.py --config $C --tag $TAG
$PY $T/fndiff.py --config $C --tag $TAG <unit> [function]
```

`fndiff.py` reads the object left by the last build, so build first. It shows
an aligned instruction diff even when sizes are wrong. `--all` also prints
matching lines.

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
- Prototypes of functions from other units go at the top of the unit's own
  `.c` file. Shared headers are consolidated later at the top level.
- Plain C89. Hex for values that are identifiers or masks.

## What is allowed

Reordering statements, changing expression shape, types and signedness,
splitting or merging conditions, `if/else` versus early assignment, loop
forms, locals versus repeated member access, and struct versus separate
externs for globals.

Not allowed: inline assembly, embedded bytes, `volatile`, `register`, dummy
operations, or anything whose only purpose is to steer the compiler and that
a reader could not take for ordinary code. If nothing else works, stop and
report.

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
- On an 8-bit or 16-bit field, `x--` and `x -= 1` add minus one. `x = x - 1`
  adds 255 on a byte, or loads 0xffff for a halfword. Pick the form from the
  constant in the listing.
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
