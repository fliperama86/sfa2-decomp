# Names and what they are inferred from

No name on this page is an original symbol. Each one is inferred from what
the reconstructed code does with the thing it names, and the page says where
to read that, so that a reader can check it. A name says what the uses show
and no more. A field whose uses do not show a meaning keeps `field_` and its
offset.

## How many have a name

    python ps1/tools/namestats.py

prints, from the build configuration, the field table and the symbol table,
how many functions, fields and data symbols have a name instead of a
placeholder (`func_`, `data_`, `table_` or `jtbl_` and an address, `field_`
and an offset). The tool's header says what is counted: Sony's library is
left out of the functions, because its names were found and not given, and a
function that has only nonmatching C is not counted, because it keeps its
placeholder by rule. The count says how many things were given a name, not
that the names are right.

The workflow that draws the coverage map runs the command on every pull
request and on every push to `main`, and publishes its three percentages as
bars under the map in the README (a full bar is every thing of its kind; a
bar is drawn at least two units wide once anything has a name), with the
counts beside them as `names.json`. Nothing is written into the repository,
so a rename needs no second step to keep the bars current.

## How a field gets its name

A field is named in one place, its line of the field table
`ps1/src/types.fields` (see "Shared types" in the
[build guide](matching-build.md)). The uses follow from that line:

    python ps1/tools/renamefields.py apply --old OLD_TABLE --new ps1/src/types.fields \
        -I ps1/src/sdk/include -I ps1/src/sdk/include/psxsdk ps1/src port

`OLD_TABLE` is the table before the change, for example the output of
`git show origin/main:ps1/src/types.fields`. The tool has clang point at
every use of an old name and rewrites that token; it does no text search.
A rename cannot change the compiled code, and the matching build of every
image is what shows that it did not.

Three rules keep the names honest:

- A name is given per struct. The record of 0x394 bytes that the game keeps
  for a fighter and for the other things on the screen has fifteen structs
  of that size in the table: `Object`, and views of the same bytes that
  modules declare with other types. A view's field gets a name when that
  view's own uses show it, not because `Object` has one at the offset.
- A comment follows the code of its file. Where a file's uses were renamed
  and no other struct's field of the old name is used in it, its comments
  were rewritten too.
- The contracts of nonmatching functions (the `.py` files) address memory
  by offset and keep `field_` and the offset for "the field at this
  offset". They are not rewritten.

## `Object`

| Offset | Name | What the uses show | Where to read it |
| --- | --- | --- | --- |
| `0x04` to `0x07` | `state_0` to `state_3` | Four bytes, each used as an index into tables of functions, stepped by one or set to a constant. They are numbered by offset; which of them is the outer one is not claimed. | `func_801312b8` sets the four; `func_80131020` reads them as one word |
| `0x10`, `0x14` | `pos_x_frac`, `pos_y_frac` | The low half of the 32-bit word whose high half is `pos_x` or `pos_y`. The code takes the address of this half to add a velocity to the whole word. | any character unit that moves its object |
| `0x38` | `step_timer` | Set from the `duration` of the current step of the object's `sequence` and counted down; when it reaches zero the next step is taken. | `advance_sequence` |
| `0x3a` | `step_flags` | A copy of the current step's `flags`. A negative value makes the sequence continue at the step's `loop_offset`; other bits are tested and cleared by the units. | `advance_sequence`, `func_801307e0` |
| `0x4c`, `0x50` | `vel_x`, `vel_y` | A 32-bit word that is added to, or taken from, the position word of its axis. | any character unit that moves its object |
| `0x54`, `0x58` | `accel_x`, `accel_y` | A 32-bit word that is added to the velocity of its axis. | the same units |

## `ObjectWords`: the same words when they hold something else

`Object` is also the type of records that are not fighters, and some units
keep another value in a word that is named above for what most units keep
there: a pointer, or a copy of `pos_x`. In those functions the name would
be wrong. They read and write the word through `ObjectWords`, a view of the
same record that gives the four words by their offsets, `word_4c`,
`word_50`, `word_54` and `word_58`, and claims nothing about what they
hold.

A function was moved to the view when a cast in it showed the other use:
the word cast to a pointer, a pointer stored into it with a cast, or
`pos_x` stored into it. Every use of that word on that object in the
function was moved with it. This command prints such a line where one is
left, and prints nothing now:

    grep -rnE '\*\) ?[a-z]+->(vel|accel)_[xy]\b|accel_[xy] = [a-z]+->pos_x;|->(vel|accel)_[xy] = \((s32|int|u32)\) ?[a-z]' ps1/src --include='*.c'

A unit that keeps a plain number of another meaning in such a word has no
cast to find it by; it is moved when it is read. When the records of a
module are understood, their words get a struct of their own with names,
and its units leave this view.

## How a function gets its name

A function has its name and its address together in one place, its entry in
`ps1/src/build.toml`. The name is changed there, and

    python ps1/tools/renamefuncs.py apply --old OLD_CONFIGURATION --new ps1/src/build.toml \
        ps1/src ps1/inventory port

replaces the old name, as a whole word, everywhere it is written: code,
comments, prototypes, the inventory, and the contracts and fixtures of
nonmatching functions, which name their callees. `OLD_CONFIGURATION` is the
file before the change. A function's first name is `func_` and its address,
with the image after it for a module's function, so the address of a named
function is in its entry and its first name follows from that.

- Only a function of an exact unit gets a name; the tool renames what the
  configuration declares and nothing else. A nonmatching function keeps
  `func_` and its address: its files are named after it, and the tool that
  tests it finds the original code by that name.
- An exact function that the port overrides keeps its first name for the
  same reason: the override is a file named after it
  (`port/overrides/func_80120604.c` is one). The tool refuses to rename a
  function that has a file of its name under the paths.
- Markdown is rewritten only where a page describes the source as it is
  (this page, the guides), and such a page is given to the tool by its own
  path. A page that records a run, such as the page of a folder of
  nonmatching functions or a record under `docs/records/`, keeps the names
  of the day of the run.

## Functions

| Address | Name | What the code shows |
| --- | --- | --- |
| `0x80130f2c` | `advance_sequence` | Counts `step_timer` down. At zero it takes the next step of the object's `sequence`, the one at `loop_offset` when `step_flags` is negative, and loads `step_timer`, `step_flags` and `frame` from that step. |
| `0x80130efc` | `update_sequence` | Calls `advance_sequence`, then `func_80131020`, which calls it a second time under a condition on the state bytes that is not understood yet. It is the routine that the units call to move an object's sequence on. |
| `0x80120554` | `play_sound_at` | Takes an object or none, a side and a word. From the object's `pos_x`, less the first word of `box_margin` and held between -192 and 575, it makes a position (192 without an object) and calls `func_80120604` with the word's high byte, the side, the word's low byte and the position. `func_80120604` reads commands from the table that its first number selects, from the command that its third number selects; a command keys a voice on through the function that the build names `SsUtKeyOnV`, with a left and a right volume made from the position, or plays or stops a sequence of the sound library, or sets a volume. So the call starts a sound, chosen by table and number, placed between left and right by where the object is. |
| `0x801204f4` | `play_side_sound_at` | The same for an object that must be there, with the side as the table's number too: the sound is taken from the side's own table. |

`func_80120604` itself keeps its first name: the port overrides it.

## A note on every function

`ps1/docs/function-notes.tsv` has one line per function that was read for naming: its address, its image
(empty for the resident program), its name where it has one, and one or two sentences on what its code shows.
The sentences and the names were written by a model that read each function with the sentences of the
functions it calls, callees first. They are inferred, like every name on this page. A name was taken only
when two separate readings of the function gave the same name, neither gave that name to another function,
the function has no look-alike in the resident program and is not inside Sony's library, and the name
passed fixed rules: no empty verb such
as `handle_`, no placeholder in it, and no word that the tree already uses. A function without a name there
keeps `func_` and its address; its sentence is still what the readers of its callers were given.

## A note on every field that was read

`ps1/docs/field-notes.tsv` has one line per field whose uses were read for naming: its struct, its offset, its
name where it has one, and a sentence on what its uses show. The sentences and the names were written by a
model that was given, for each field, lines of code that use it (found by the compiler, as the rename tool
finds them) and the sentences of `function-notes.tsv` for the functions of those lines. They are inferred,
like every name on this page. A name was taken only when two separate readings gave the same name, neither
gave that name to another field of the struct, and the name passed fixed rules: no placeholder word, no
offset in it, not a keyword and not a macro of the tree. A name is per struct, as the rules above say.
