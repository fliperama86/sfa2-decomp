## Field names begin: a rename tool and the first twelve names of `Object` (2026-10-11)

The owner asked to start naming, with the types first. This change adds the
tool that renames a field where it is used, a page for the names and their
evidence, and the first twelve names. No name is an original symbol; each is
inferred from the uses, and `ps1/docs/names.md` says where to read them.

What was checked before any name was given. The record of 0x394 bytes has
fifteen structs of that size in the field table
(`grep -c "size=0x394" ps1/src/types.fields` prints 15): `Object` and
fourteen views that modules declare. They do not agree on what an offset
holds: at `0x18`, `0x40` and `0x88`, for three, `Object` declares a pointer
and a view declares a 16-bit number. So a field is named per struct, from
that struct's own uses.

The tool, `ps1/tools/renamefields.py`. It compares the table before and
after, takes a field that kept its struct and offset and changed its name
for a rename, generates the header of the new table and has clang check
every C file against it. clang then reports each use of an old name with
its line and column, and the tool replaces that token. Nothing is found by
text search. `ps1/tools/test_renamefields.py` has 12 cases on made-up
tables and files (a second struct with the same field name, a chain of two
renamed fields, a use in a header, a use in a macro, a file that cannot be
read to the end, a dry run); it needs clang only and runs in about a
second.

The names, all in `Object`, with the uses the tool rewrote on main at
`29ba77c` (its own output):

| Offset | Old | New | Uses |
| --- | --- | --- | --- |
| `0x04` | `field_04` | `state_0` | 1517 |
| `0x05` | `field_05` | `state_1` | 1205 |
| `0x06` | `field_06` | `state_2` | 1182 |
| `0x07` | `field_07` | `state_3` | 3575 |
| `0x10` | `field_10` | `pos_x_frac` | 627 |
| `0x14` | `field_14` | `pos_y_frac` | 471 |
| `0x38` | `field_38` | `step_timer` | 95 |
| `0x3a` | `field_3a` | `step_flags` | 1883 |
| `0x4c` | `field_4c` | `vel_x` | 1567 |
| `0x50` | `field_50` | `vel_y` | 1008 |
| `0x54` | `field_54` | `accel_x` | 735 |
| `0x58` | `field_58` | `accel_y` | 603 |

Its last line: `12 renames, 14468 uses rewritten in 4089 files, 0 left, 30
files not read to the end`. The 30 are C files of the port's own runtime,
which include the host's headers; none has one of the twelve old names as
text. Run again after the merge of main, the tool rewrote nothing. The
command:

    git show 29ba77c:ps1/src/types.fields > old.fields
    python ps1/tools/renamefields.py apply --old old.fields --new ps1/src/types.fields \
        -I ps1/src/sdk/include -I ps1/src/sdk/include/psxsdk ps1/src port

Run on main at `29ba77c` with this change's table, it writes the code lines
of this change. By hand on top of it:

- Comments. In a C file whose uses of a name were rewritten and which uses
  no other struct's field of that old name, the comments' mentions of the
  name were rewritten too, by a script. Two files use both and were read:
  `port/overrides/func_8013bfa4.c` (one mention is the object's, three are
  a frame record's and stay) and
  `ps1/src/slot06_nonmatching/func_801e8dc4_slot06_00.c` (three mentions,
  all the object's).
- The contracts of nonmatching functions (`.py`) are not changed: they
  address memory by offset, and the outputs that the folders' pages record
  quote their texts.
- Six passages of `ps1/docs/matching-guide.md` quote lines of functions
  whose fields were renamed; they quote the new lines now.

What ran. The build ran twice: on the change as made on main at `29ba77c`,
and again after the merge of main at `ae125bc` (head `a5e3de0`), with the
same result. The comparison of the nonmatching C ran before that merge:

- The matching build of every image, one run per image and eight at a time
  (`python ps1/tools/matchbuild.py --config ps1/src/build.toml --image NAME`
  for `resident` and the 77 module images): 78 of 78 end with
  `RESULT: PASS`. The first round compiled every unit that includes the
  header again and took under three minutes for all 78; the second, after
  the merge, under two.
- Each of the 105 C files that `ps1/src/*_nonmatching/` and
  `port/overrides/` had at `29ba77c` (the merge of main takes one of them
  out, a library function that became exact) was preprocessed and compiled as the build does it (`clang -E -P -x c
  -target mipsel-none-elf -nostdinc`, then the pinned compiler with
  `-quiet -O2 -G0`) in the tree before and in the tree after. The
  compiler's output is the same for all 105 but for the `.file` line, which
  carries the path of the input. Their differential tests and replays were
  therefore not run again: what they test did not change.
- The replays of the recorded fixtures, one run per folder and the eight
  folders at the same time, at `3669d38`
  (`python ps1/src/slot06_nonmatching/difftest.py --config ps1/src/build.toml --folder FOLDER --replay FUNC...`
  for the five nonmatching folders, the two programs and the port's
  overrides, with the functions the review command lists for each): 123
  functions, every one with `failed 0`, in under half a minute.
- `python ps1/tools/test_renamefields.py`: all 12 cases behaved as required.

What did not run: the differential tests against the original code (also
for the functions that have no fixtures; the comparison of the compiler's
output covers those), `python tools/ai_workflow/workflow.py review` as a
whole (its plan for this change is the whole build, every tool's tests and
every folder's differential runs), the port's build and its control files,
the inventory sweep (no boundary moved).

After the review. The pull request was approved at `d57b426`; before it
could be merged, main gained batch 5 (`7a1d6ab`): six functions left
`slot04b_nonmatching` and became units of `slot04_11` and `slot04_17`,
written with the old names. The merge of main had six conflicts of one
kind, a file that this change modified and main deleted, and each was
settled by taking the deletion. The tool, run on the merged tree with the
same command, rewrote 86 uses in the six new units and left nothing; their
comments name none of the twelve fields. On that head, `3c49872`: 78 of 78
images `RESULT: PASS`, and the replays of the eight folders, 117 functions
(the six are gone from their folder), every one with `failed 0`.

Known and left for the next change. `Object` is also the type of records
that are not fighters. In 37 lines of 22 units, found by their casts with
the command on the names page, one of the four words `vel_x`, `vel_y`,
`accel_x`, `accel_y` holds a pointer or a copy of `pos_x`, and the name is
wrong there. Those uses move to the module's own view next, where the word
keeps its offset name. A unit that keeps a plain number of another meaning
in such a word has no cast to find it by.

For work in flight in other checkouts: a branch from before this change
that adds or changes units uses the old names in them and does not compile
after merging main. The tool repairs it, with the table of the branch's
merge base as `--old` and the paths of the branch's own files.
