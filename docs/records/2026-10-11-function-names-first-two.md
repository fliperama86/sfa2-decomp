## Function names begin: a rename tool and the first two names (2026-10-11)

After the fields, the functions. This change adds the tool that renames a
function, says on the names page how a function gets its name, and names
two functions of the resident program. No name is an original symbol.

The tool, `ps1/tools/renamefuncs.py`. A function has its name and its
address together in its entry of `ps1/src/build.toml`. The tool compares
the configuration before and after: a function that kept its image and
address and changed its name is a rename. It replaces the old name by the
new one, as a whole word, in every text file under the paths it is given.
For a field that would be wrong, because one field name means different
things in different structs; a function's name is one word with one meaning
in the whole tree, so the replacement is exact, and it reaches comments,
the inventory and the contracts and fixtures that name a callee as well.
`ps1/tools/test_renamefuncs.py` has 14 cases on made-up files and needs
only Python.

What was looked at before it was written: where a name is taken for an
address. The test tool of nonmatching C, the review command and the port's
build read the address of a nonmatching function out of its `func_` name
and out of its files' names. So a nonmatching function is not renamed, and
cannot be by this tool, which renames only what the configuration declares.
Functions with a name of their own were in the build before this change
(`build_metrics` and others), declared the same way.

The two names:

| Address | Old | New | Places |
| --- | --- | --- | --- |
| `0x80130efc` | `func_80130efc` | `update_sequence` | 1799 |
| `0x80130f2c` | `func_80130f2c` | `advance_sequence` | 7 |

The tool's last lines, on main at `f37da0b` with this change's
configuration: `by kind of file: .c 1747, .h 2, .json 42, .md 2, .py 11,
.tsv 2` and `2 renames, 1806 places rewritten in 832 of 4475 files`. The
command:

    git show f37da0b:ps1/src/build.toml > old.toml
    python ps1/tools/renamefuncs.py apply --fresh --old old.toml --new ps1/src/build.toml \
        ps1/src ps1/inventory port ps1/docs/names.md

By hand: the two names in `build.toml`, the new parts of the names page
and of the build guide, and this record. `advance_sequence`
counts the step timer down and takes the next step of the object's
sequence; `update_sequence` calls it and then `func_80131020`, which calls
it once more under a condition that is not understood yet, and is the call
that 819 C files make
(`grep -rlw update_sequence ps1/src --include='*.c' | wc -l`). The other
routines of the two units (the ones that set a sequence from one of the
tables, and the ones that look at the byte at `0x08` before they take a
frame) keep their first names until the tables and that byte are read.

What ran, on the tree of this change:

- The matching build of every image, one run per image and eight at a
  time: 78 of 78 end with `RESULT: PASS`, in under two minutes.
- The inventory: `python tools/coveragemap.py sweep` in about three seconds; its
  four tables are the same as the ones in `ps1/inventory/` after the tool
  rewrote two lines of `game.tsv`.
- The replays of the recorded fixtures of the eight folders, at the same
  time: 117 functions, every one with `failed 0`. Five fixtures files and
  five contracts name `update_sequence` as a callee now.
- `python ps1/tools/test_renamefuncs.py`: all 14 cases behaved as required.

What did not run: the differential tests against the original code,
`workflow.py review` as a whole, the port's build and its control files (no
file of the port names either function).

For work in flight in other checkouts: a branch from before this change
that calls one of the two by its first name does not link after merging
main. The tool repairs it, without `--fresh`, with the merge base's
configuration as `--old` and the branch's own files or folders as paths.
