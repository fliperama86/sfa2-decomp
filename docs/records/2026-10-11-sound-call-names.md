## Two names for the calls that start a sound, and a refusal the tool lacked (2026-10-11)

Character code calls two small resident functions several hundred times,
each with an object, the object's side and a number. Both end in a call of
`func_80120604`. That function reads commands from a table; a command keys
a voice on through the function that the build names `SsUtKeyOnV`, with a
left and a right volume that it makes from a position, or plays or stops a
sequence through `SsSepPlay` and `SsSepStop`, or sets a volume through
`SsUtSetVVol` or `SsSetMVol`. The two callers make the position from the
object's `pos_x`. So they start a sound placed by where the object is. The
names are inferred from that reading, not recovered.

| Address | Old | New | Places |
| --- | --- | --- | --- |
| `0x80120554` | `func_80120554` | `play_sound_at` | 487 |
| `0x801204f4` | `func_801204f4` | `play_side_sound_at` | 398 |

The tool's last lines, on main at `158b6e0` with this change's
configuration: `by kind of file: .c 813, .h 2, .json 52, .py 16, .tsv 2`
and `2 renames, 885 places rewritten in 422 of 4454 files`. The command is
the one of the names page, with `--fresh`.

What the tool lacked. The first try named `func_80120604` too
(`play_sound`). The tool rewrote the name inside
`port/overrides/func_80120604.c`, the port's own C for that function, and
left the file's name, from which the port's build and the test tool read
the function's address. Nothing had run yet; the tree was put back. The
tool now refuses to rename a function that has a file named after it under
the paths, the names page and the build guide say so, and the test has a
case for it (15 cases now). `func_80120604` keeps its first name. The two
functions that were named before this change have no such file.

What ran, as `docs/efficiency.md` says for a function's rename, on the tree
of this change:

- The matching build of every image, one run per image and eight at a
  time: 78 of 78 end with `RESULT: PASS`, in under two minutes.
- The replays of the fixtures of the eight folders that have some, at the
  same time: 108 functions, every one with `failed 0`.
- The inventory sweep: its four tables equal `ps1/inventory/` after the
  tool rewrote two lines of `game.tsv`.
- `python ps1/tools/test_renamefuncs.py`: all 15 cases behaved as required.

What did not run: any differential test, the port's build, the whole
review.

After the review. Approved at `008f4b2`; main had moved by then
(`fd64ea0`). Merged at `9ff22fad` without a conflict; both rename tools,
run on the merged tree against the tables of before the first rename,
found no old name. On that head: 78 of 78 images `RESULT: PASS`; the
replays, 107 functions (one more left its folder on main), every one with
`failed 0`.
