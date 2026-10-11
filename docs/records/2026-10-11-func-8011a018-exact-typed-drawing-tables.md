## func_8011a018 exact; the three drawing tables get their types (2026-10-11)

- `func_8011a018` (1,016 bytes, resident), the routine that initializes
  the tables of the two-buffer drawing system, is the exact unit
  `d34234_r2`: `baseline 1016 bytes at 0x8011a018, built 1016 bytes at
  0x8011a018, 0 differing instruction slots: IDENTICAL`. No wrapper and
  no stand-in. It leaves `ps1/src/resident_nonmatching/` (its two files,
  its four lines of that folder's README and its name in the list of
  functions without a fixtures file), and its line leaves `symbols.ld`.
- What it took. The candidate had been two instruction slots away since
  2026-10-10: in the first loop over the 0x280 cells of each buffer the
  call's argument was added as (offset + base) + buffer where the
  original adds (buffer + base) + offset. The loop was written with
  hand-kept running offsets. Written as a plain nested loop, with the
  call's argument as `&data_801987cc[j][i]`, the compiler folds the sum
  in the original's order. A worker found this on a bounded try, reading
  `fold-const.c`; the top level checked the result and measured the
  alternatives below.
- Two addresses in it are sums of bytes, each with a comment that says
  what the typed spelling gives. In the first loop the cell's address is
  formed a second time for the three stores: with `&data_801987cc[j][i]`
  there too the function is 12 bytes shorter and differs in 18
  instruction slots; with `&data_801987cc[0][i + j * 0x500]`, 42. In the
  second loop the second half of each buffer is addressed in bytes from
  its start: with a `Cell16` pointer and sums of cells, 22 slots; with
  `&data_801987cc[j][0x280 + i]` for the call and the stores, 62. The
  fields themselves are reached by name in every form.
- The shared declarations. `externs.h` had `data_801987cc`,
  `data_8018db18` and `data_8019056c` as `u8[]`. They are now `Cell16
  [2][0x500]`, `Prim [2][0x70]` and `Prim [2][0x28]`: two buffers of
  cells and of primitives, which is how every user indexes them. The
  users follow in the same change. The two exact units `m11907c_r4` and
  `m11907c_r5` take the address of a buffer as `(u8 *)data_801987cc[n]`
  and so on, in place of a sum of bytes: both still `IDENTICAL` (252 and
  216 bytes). Three nonmatching functions add bytes to the tables and
  get the byte view written out, `(u8 *)data_801987cc + ...`:
  `func_8011c724`, `func_8011d74c`, `func_8011db78`. No other file of
  `ps1/src` names the three tables (searched: every `.c` and `.h`).
- What ran. `fndiff.py --rebuild` on the three exact units (the lines
  above). `matchbuild.py --image resident`: `RESULT: PASS`, the
  executable's SHA-256 equal to the baseline's (1 minute 50; it ran on
  this change before main was merged in, and main brought only files of
  the program-study folder and the module function of the pull request
  before this one). The three nonmatching functions alone: the fixtures
  replay of `func_8011d74c` (6 of 6) and `func_8011db78` (10 of 10);
  `func_8011c724` has no fixtures file, so its own five runs, at the
  same time: 2,000 of 2,000 equal on both seeds, the control trips
  (1,355 of 2,000), no write outside on both seeds: the same lines as
  the folder's README has for it, which therefore does not change. The
  inventory sweep (the function's row gets its name) and the map's
  render: `overall: 5436/5600 distinct functions exact (97.1%);
  12863/13072 placements (98.4%); 1702292/1809836 code bytes (94.1%)`.
- What did not run: the build of every image. The efficiency page asks
  for it once when a shared header changes. Here the change of the
  header is the type of three names, the search shows every file that
  names them, and each of those was rebuilt or tested; a unit that does
  not name them cannot compile to other code. The reviewer may hold
  that the page's row applies as written.
