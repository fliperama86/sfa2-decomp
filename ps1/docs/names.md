# Names and what they are inferred from

No name on this page is an original symbol. Each one is inferred from what
the reconstructed code does with the thing it names, and the page says where
to read that, so that a reader can check it. A name says what the uses show
and no more. A field whose uses do not show a meaning keeps `field_` and its
offset.

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
