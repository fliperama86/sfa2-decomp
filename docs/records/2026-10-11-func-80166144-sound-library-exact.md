## func_80166144 of the sound library exact (2026-10-11)

- `func_80166144` (304 bytes, the sound library part of the resident
  executable; inferred to be the library's `SsSepOpen`: it opens a group
  of sequences): unit `sdk_libsnd_sepopen`, file
  `ps1/src/sdk/libsnd/sepopen.c`, `baseline 304 bytes at 0x80166144,
  built 304 bytes at 0x80166144, 0 differing instruction slots:
  IDENTICAL`. No wrapper, no helper, no stand-in. It is the first of
  the six functions of `library_nonmatching` to become exact. "Slots"
  are instruction slots by `fndiff.py --rebuild` on the unit, each
  variant built in the tree and taken out again.
- What made it exact against the published readable C: the address
  walks in a local pointer of its own, set from the parameter at its
  declaration. Walked in the parameter itself: 4 slots, the order of
  two instruction pairs at the function's entry. Commented. Measured
  besides: the loop counter as `int`: 44 slots; the loop test without
  the cast of the count to 16 bits: 24.
- Inferred from the compiler's source and the private search's dumps,
  not shown by a dump here: the two parameter copies at the entry tie
  in the scheduler and go by their order of creation; with the local,
  the copy of the first parameter is made later than the second's.
- The unit owns the 36 bytes of its message string (`rodata = { address
  = 0x8016e22c, size = 36 }`), which the build compares with the image;
  the library's units under `sdk/` own their strings the same way.
- The file is written by this project from the listing and carries no
  license line; it has a file of its own under `sdk/libsnd/` because the
  reference has no file for it, and the page of `sdk/` says so. It keeps
  the name `func_80166144`, as `protos.h` declares it and its one
  caller (`x07_r2.c`) calls it, includes the game's three headers and
  declares what it needs of the library itself, as the readable C did.
  It is built with the library's compiler flags.
- It leaves `ps1/src/library_nonmatching/` (two files, its five lines
  of recorded output, its entry in the list of the functions and three
  mentions; the page says five functions now), its line leaves
  `symbols.ld`, and the sweep gives its row of
  `ps1/inventory/library.tsv` its name (the row of its caller
  `func_8014ff20` in `game.tsv` loses the label "unidentified" with
  it; both are what the sweep printed, not edits by hand).
- The PC program compiles the unit as part of the sound library. One
  build of it from this branch: `sound library: 46 units of sdk/libsnd,
  22 of sdk/libspu, 5 of library_nonmatching`, `units: 3980 compiled,
  123 of them nonmatching, 0 failed`, `functions overridden in C: 15`,
  linked and verified. One start of it without a window, 45 seconds,
  the sound written to a file: no `stop` or `refused` line, no `sound
  output open` line, and the file holds sound (peak 28388 of 32767).
- What ran besides: `fndiff.py --rebuild` on the unit (the line
  above); `matchbuild.py --image resident`: `RESULT: PASS` (the 36
  bytes included); the map's render on this branch, which does not hold
  pull request 248: `overall: 5469/5600 distinct functions exact (97.7%); 12903/13072 placements (98.7%); 1720168/1809836 code bytes (95.0%)`. No whole PS1 build, no differential test.
