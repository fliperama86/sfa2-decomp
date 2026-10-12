# PS1 source layout

One file per unit of `build.toml`. A unit is a run of functions that is
compiled together, so a file holds one function or several.

## Where a file is

- `resident/<subsystem>/`: the code of the main executable, sorted by what a
  file mainly does. The subsystems are `fighters` (per-fighter states and
  combat), `objects` (the object pool, spawning, parent and child links),
  `script` (the move-script interpreter), `graphics`, `hud` (meters, scores,
  round and result text), `menus` (modes, game flow, select, continue, name
  entry), `input`, `disc` (CD access, streaming, loading), `sound` and
  `system` (startup, threads, memory, random numbers).
- `overlays/characters/<name>/`: a character's code module, loaded over the
  game for that character. `second_side/` and `extra_module/` below it hold
  the extra modules some characters carry. `*_mirror` is the combined block
  loaded when both players pick that character, and `*_alt` is the second
  style of a character that has two.
- `overlays/stages/<name>/`: the module of the stage whose number equals that
  character's number.
- `overlays/<screen>/`: the modules of the select screens, the attract demo,
  the continue screen, the ending and the others.
- `sdk/`: Sony's library, in the layout the library has. Not renamed.

## How a file is named

A file is named after the first function it holds, by address. Look for a
function with `grep -rn NAME ps1/src` or find its file in
`ps1/docs/source-map.tsv`, which also gives the file's earlier name
(`d189c4_r1.c`, `slot04_00_0008_r1.c`), the name the history and the older
records use.

`ps1/tools/renamefuncs.py` renames the file together with the function when
the function is the one the file is named after.

## What the names rest on

Nothing here is an original symbol. The character names come from the table
of names in the executable, whose order matches the numbers the code uses for
the characters. The stage names rest on the stage number equalling the
character number. The screens are named from how the code loads them, and
some of those names are guesses. The subsystem of a resident file was decided
from the names of its functions and, for part of the files, from the code.
Where it is wrong, move the file and its `source` line together (`git mv`),
and put the include paths right (`#include "../../game.h"`, one `../` per folder).
