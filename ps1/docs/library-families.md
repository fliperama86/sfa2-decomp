# Library families of the game functions

Parent: [delivery plan](../../PLAN.md). Which game functions of the resident
executable call the Sony library, and which part of it: graphics, sound,
disc, pads and so on. It tells code that touches the machine from game logic
before the functions have proper names.

Everything here is a static estimate over function boundaries from a sweep.
Nothing was observed in a running game. The functions of the overlay
modules are labelled the same way, each content of a module taken alone.

## Method

`ps1/tools/families.py` sweeps the code of the executable for functions, as
the [overlay map](overlays.md) describes. A function that starts below
`--library` is a game function, any other a library function. The address
used, `0x80157000`, is the project's working boundary and approximate.

A library function has one family, the first of these that applies:

1. Its name. The build configuration declares functions with names;
   `[names]` of the family table gives some names a family. With
   `--symbols`, another name that the symbol file assigns the function's
   start counts too. That is how a library function gets a family before
   any unit declares it: see "Names by reference" below.
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
    --symbols ps1/src/symbols.ld --out GAME_TSV --library-out LIBRARY_TSV
```

The sweep has 1,852 functions, 1,436 game and 416 library, and every
function of the build is a start of it.

| Family | Library functions | Game functions that call it directly | Game functions that reach it |
| --- | ---: | ---: | ---: |
| C library | 9 | 0 | 0 |
| disc | 39 | 18 | 88 |
| files | 7 | 0 | 0 |
| graphics | 85 | 47 | 141 |
| memory card | 7 | 1 | 4 |
| pads | 3 | 1 | 3 |
| sound | 160 | 14 | 179 |
| system | 47 | 12 | 47 |
| threads | 3 | 6 | 51 |
| unidentified | 56 | 23 | 193 |

- 91 game functions call a library function directly.
- 1,117 game functions reach no library function. 830 of them are closed:
  the tool finds no call edge from them that it could not follow. That is
  not proof that they are free of the machine. A function can read or
  write a hardware register itself, and control can pass in ways the tool
  does not decode. 287 are open.
- 133 game functions have a call through a register and 15 a call
  elsewhere.

## Names by reference

A unit under `sdk/` is built from the reference reconstruction of the
library. Where its source calls a function, the unit's object refers to
that function by name, and the link gives the name an address. The build
is exact, so the original calls the same address at that place.
`ps1/tools/librefs.py` reads those references from the unit objects of a
finished build:

```sh
.venv/bin/python ps1/tools/librefs.py EXECUTABLE --config ps1/src/build.toml \
    --build ps1/build/default --symbols ps1/src/symbols.ld \
    --library 0x80157000 --end 0x8016d920
```

Only a reference to the start of a function counts. A reference with an
offset, such as a call to a name plus four, is to another place and is
left out, and so is one whose offset the tool cannot establish.

Of the 416 library functions, a unit under `sdk/` declares 327 itself. 28
others are referred to by such units, each under one name, and no function
under two. 61 have no name. The command lists the 28 with the number of
units that refer to each.

What such a name shows: at an exact place, the reference's source refers
to the function under that name. It does not show that the name is the
original symbol. A reference that names the wrong function at its only
place of use would go unnoticed.

The family table lists the 28 names with the family of the reference file
that defines each. One of them, `SpuInitHot`, has no such file in the
reference; its family is taken from its name.

## The overlay modules

With `--modules PAC_DIRECTORY --pointers 0x8017eb1c --symbols
ps1/src/symbols.ld` the tool also sweeps every distinct content of a
code-bearing chunk, as `pac.py functions` does, and labels its functions.
A function of a module calls a function of its own content, or a function
of the executable when the address lies outside its chunk. Any other
target is a call elsewhere: into another module, whose content at that
moment is not known, or to no function start. Its reached families add what
the game functions of the executable that it calls reach, and it is open
when one of those is.

The 77 contents hold 11,220 functions. The counts are over all contents, so
code that several contents share is counted once in each.

| Family | Module functions that call it directly | Module functions that reach it |
| --- | ---: | ---: |
| C library | 9 | 18 |
| disc | 0 | 89 |
| files | 7 | 13 |
| graphics | 97 | 597 |
| memory card | 1 | 13 |
| pads | 0 | 0 |
| sound | 0 | 2,593 |
| system | 8 | 201 |
| threads | 0 | 194 |
| unidentified | 7 | 2,723 |

- 121 module functions call a library function directly, and 7,008 call a
  game function of the executable.
- 8,105 reach no library function: 4,814 closed and 3,291 open.
- 1,941 have a call through a register and 14 a call elsewhere.

## What it shows and what it does not

- Few game functions talk to the library themselves. The rest of the
  resident game code reaches it through them or not at all.
- The game uses the thread calls of the BIOS: 6 game functions call them
  directly and 51 reach them.
- 56 library functions have no family. By the rows of `--library-out`,
  40 are declared by the build outside `sdk/` under a placeholder name and
  16 are not in the build.
  193 game functions reach one of them, more than reach any family.
  Identifying those library functions is the largest gap of this table.
  They were 116 when the table was first made. 32 of them have since been
  replaced by the reference's source for the same function, which builds
  to the same bytes, and carry its name and its library's family. 28 more
  have a name by reference.
- Inferred, not established: an unidentified function probably belongs to
  the library whose functions surround it. The table does not use that.
- A family says which library a function calls, not what the function is
  for. Names still need the code to be read.
- By the direct calls that the tool decodes, the modules reach the library
  almost only through the executable. No module function has a direct call
  to a sound, disc, pad or thread function.
- 14 module functions have a direct call whose target the tool cannot
  resolve: an address in another module, or one that is no function start.
  Among direct calls, taking each content alone therefore loses little.
  That is not a count of all calls between modules. Calls through a
  register are in 1,941 module functions, and where those go, another
  module included, is unknown here.
- By direct calls, no function reaches the pad calls except three of the
  executable. How the game reads the pads after starting them is not
  visible in these calls.
