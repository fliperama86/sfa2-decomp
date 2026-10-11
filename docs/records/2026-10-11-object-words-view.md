## The words of `Object` that hold something else: a view by offset (2026-10-11)

The change before this one named four words of `Object` `vel_x`, `vel_y`,
`accel_x` and `accel_y`, and listed 37 lines of 22 units where one of them
holds a pointer or a copy of `pos_x`. This change takes the wrong name out
of those functions.

`ps1/src/types.fields` gets one struct, `ObjectWords`, of the size of the
record, with the four words by their offsets: `word_4c`, `word_50`,
`word_54`, `word_58`, each a 32-bit number. It claims nothing about what
they hold.

In each function that one of the 37 lines is in, every use of that word on
that object now goes through the view, for example
`ref_first.p = (Object *)((ObjectWords *)obj)->word_54;` where it was
`ref_first.p = (Object *)obj->accel_x;`. That is 40 lines in the 22 units
(`grep -rn ObjectWords ps1/src --include='*.c' | wc -l` prints 40): the 37,
and three more uses of the same word in the same functions. The units are
of the modules `slot27` (6), `slot06_0a` (3), `slot01`, `slot12`, `slot28`
(2 each), `slot06_12`, `slot0b`, `slot0f` and of the character files
`slot04_04`, `slot04_11`, `slot04_12`, `slot04_17` (1 each). The command of
the names page that printed the 37 lines prints nothing now.

Why a view of its own and not the modules' views: the views that the
modules declare do not agree with a 32-bit word at these offsets
(`Slot27Obj` and `Slot06Obj` declare 8-bit or 16-bit fields at `0x4c`,
`Slot04aObj` 16-bit ones at all four), and the field table refuses two
fields that overlap. What each module's records hold in these words is not
read yet; when it is, the words get names in a struct of that module and
its units leave this view.

What ran, on this change at its first commit:

- The matching build of every image, one run per image and eight at a
  time: 78 of 78 end with `RESULT: PASS`, in about two minutes. The new
  struct changes the header, so every unit that includes it was compiled
  again; the 22 units build to the same bytes with the cast as without.
- The replays of the recorded fixtures of the eight folders, at the same
  time: 123 functions, every one with `failed 0`.

The build ran again at `9b336ee`, after the branch took the first change's
merge of main's batch 5: 78 of 78 `RESULT: PASS`. The six units of that
batch have no line that the names page's command prints.

What did not run: the differential tests against the original code,
`workflow.py review` as a whole, the port's build and its control files,
the inventory sweep (no boundary moved).

Left open, as the names page says: a unit that keeps a plain number of
another meaning in one of these words has no cast to find it by.
