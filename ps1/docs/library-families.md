# Library families of the game functions

Parent: [delivery plan](../../PLAN.md). Which game functions of the resident
executable call the Sony library, and which part of it: graphics, sound,
disc, pads and so on. It tells code that touches the machine from game logic
before the functions have proper names.

Everything here is a static estimate over function boundaries from a sweep.
Nothing was observed in a running game. The overlay modules are not covered
yet.

## Method

`ps1/tools/families.py` sweeps the code of the executable for functions, as
the [overlay map](overlays.md) describes. A function that starts below
`--library` is a game function, any other a library function. The address
used, `0x80157000`, is the project's working boundary and approximate.

A library function has one family, the first of these that applies:

1. Its name. The build configuration declares functions with names;
   `[names]` of the family table gives some names a family.
2. Its BIOS call. A stub loads a table (`0xa0`, `0xb0` or `0xc0`) and a
   number and jumps into the BIOS. `[bios]` gives a family to a table and
   number, written as in `b0:12`.
3. Its folder. If the source of the unit that declares it is
   `sdk/FOLDER/...`, `[folders]` gives the folder a family.
4. `unidentified`: none of the three applies.

The family table is [`ps1/src/library-families.toml`](../src/library-families.toml).
Which family a name, a call or a folder gets is this project's reading of
the library, written there once and open to correction. The names of the
five BIOS calls in it come from the public PSX-SPX documentation.

A game function gets two sets of families:

- direct: the families of the library functions it calls;
- reached: its direct families and those of every game function that it
  reaches through calls, itself included.

A call is a `jal` or a `j` to the start of another function of the sweep.
Two things are counted and not followed:

- a call through a register (`jalr`);
- a `jal` or `j` to an address that is no start of the sweep and lies
  outside the function: a call into a module, or into the middle of a
  function.

A game function is `open` when it, or a game function it reaches, has one
of the two, and `closed` otherwise. The reached set of an open function is a
lower bound. The library is not entered: what a library function calls does
not count for its caller.

## Result

```sh
.venv/bin/python ps1/tools/families.py EXECUTABLE --config ps1/src/build.toml \
    --families ps1/src/library-families.toml --library 0x80157000 --end 0x8016d920 \
    --out GAME_TSV --library-out LIBRARY_TSV
```

The sweep has 1,852 functions, 1,436 game and 416 library, and every
function of the build is a start of it.

| Family | Library functions | Game functions that call it directly | Game functions that reach it |
| --- | ---: | ---: | ---: |
| C library | 9 | 0 | 0 |
| disc | 34 | 17 | 88 |
| files | 7 | 0 | 0 |
| graphics | 77 | 47 | 141 |
| memory card | 7 | 1 | 4 |
| pads | 3 | 1 | 3 |
| sound | 117 | 11 | 167 |
| system | 43 | 12 | 47 |
| threads | 3 | 6 | 51 |
| unidentified | 116 | 31 | 199 |

- 91 game functions call a library function directly.
- 1,117 game functions reach no library function. 830 of them are closed:
  by this measure they are game logic and nothing else. 287 are open.
- 133 game functions have a call through a register and 15 a call
  elsewhere.

## What it shows and what it does not

- Few game functions talk to the library themselves. The rest of the
  resident game code reaches it through them or not at all.
- The game uses the thread calls of the BIOS: 6 game functions call them
  directly and 51 reach them.
- 116 library functions have no family. By the rows of `--library-out`,
  86 are declared by the build outside `sdk/` under a placeholder name and
  30 are not in the build.
  199 game functions reach one of them, more than reach any family.
  Identifying those library functions is the largest gap of this table.
- Inferred, not established: an unidentified function probably belongs to
  the library whose functions surround it. The table does not use that.
- A family says which library a function calls, not what the function is
  for. Names still need the code to be read.
- Most game code is in the overlay modules. They call resident functions
  and the library too, and are the next step for this tool.
