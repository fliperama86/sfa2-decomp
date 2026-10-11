
## Batch 11: five functions of the graphics library exact (2026-10-11)

- `get_mode` (`0x80158d28`, 92 bytes), `get_cs` (`0x80158d84`, 204),
  `get_ce` (`0x80158e50`, 204), `get_ofs` (`0x80158f1c`, 72) and `get_dx`
  (`0x80158fe8`, 188): functions of the library's `libgpu/sys.c` that
  the importer had left out, between and around `get_tw`, which is
  exact already. The names are the reference's, by the functions' shape
  and place. The tree is made from main at `965702c`.
- What held them: in each, the original forms the address of a field of
  the library's state `gs` in a register and loads through it
  (`lui v0; addiu v0,v0,0x1994; lbu v0,0(v0)` in `get_mode`), where a
  plain read builds one load with the offset in it. This compiler makes
  the original's form for a read through a `volatile` type.
- The form: each function reads the field through a cast to `volatile`
  at the one use, under the guide's general label (order of
  instructions) with its figure. The shared declaration of `gs` is the
  one the file's other parts have; nothing shared changes. Measured with
  `fndiff.py --rebuild` on the final text with the function's casts
  taken out, the unit put back: `get_mode` 6 differing instruction
  slots, `get_cs` 15, `get_ce` 14, `get_ofs` 3, `get_dx` 9. In `get_cs`
  and `get_ce` the cast stands at three reads (the width, the height,
  the type): one stand-in at three places, one figure for the three
  together.
- Besides the casts the text follows the reference's functions, with two
  differences that are ordinary code: the type is read once into a local
  before the reference's test of it (`type == 1 || type == 2`), and
  `get_cs` and `get_ce` read their limit once into a local inside the
  conditional, each axis in a local of its own, where the reference's
  `CLAMP` macro names the limit twice. One local for both limits:
  14 slots and 8 bytes more (measured on `get_cs` in an earlier text of
  the unit, before the labels were written).
- The units: `sdk/libgpu/sys_p8.c` (the first four, one run of 572
  bytes) and `sdk/libgpu/sys_p9.c` (`get_dx`), made by hand in the form
  of the importer's parts from `sys_p3.c`. They were exact at the first
  build in that form.
- Where the text comes from: a worker of this lane found the forms with
  the game's own names, after reading the same five functions in the
  PSY-Q decompilation that the PsyZ project carries
  (`decomp/src/libgpu/sys.c`, commit `4e4b3e8`, MIT); the top level
  wrote the parts from that file's text and the worker's forms. The
  library page names the file, the commit and the license; `LICENSE`
  has that project's lines since the sound function of pull request
  271. This follows the owner's answer of this day for that function
  ("publish" with the notice).
- Other files: `symbols.ld` loses the four names that the first unit
  defines (`get_dx` had none); the inventory carries the five names;
  the library page's counts (90 C files, 36 parts) and a paragraph.
- What ran: `fndiff.py --rebuild`:
  - `sdk_libgpu_sys_p8`: `baseline 572 bytes at 0x80158d28, built 572 bytes at 0x80158d28, 0 differing instruction slots: IDENTICAL`
  - `sdk_libgpu_sys_p9`: `baseline 188 bytes at 0x80158fe8, built 188 bytes at 0x80158fe8, 0 differing instruction slots: IDENTICAL`
  `matchbuild.py --image resident`: `RESULT: PASS`. The five figures:
  the same command on the unit with that function's casts taken out.
  The map's render:
  `overall: 5538/5600 distinct functions exact (98.9%); 12990/13072 placements (99.4%); 1766332/1809836 code bytes (97.6%)`.
  No whole build, no PC build, no differential test.
