
## Batch 14: two game functions that nobody had written (2026-10-11)

- `func_80157270` (240 bytes) and `func_80157380` (588 bytes), the last
  two functions of the game's own code before the library's first
  function (`0x801575cc`). The tree is made from main at `45ed73d`.
- Why they were open: the static sweep lists them in the library's
  table, as "unidentified", because they stand above the address where
  its library area begins; the lists of open functions that this lane
  worked from were made from the game's table and by name. The list
  made by image and address this afternoon showed them. They are game
  code: both take an object, read the game's state, and one calls the
  game's `func_80120028`. No candidate, note or test of either existed.
  The sweep's table still lists them where it did; moving its boundary
  is not part of this change.
- Each was written from the original's instructions by a worker of this
  lane, in a tree of its own with the unit declared and the build's own
  comparison as judge, and was identical within two minutes.
- `func_80157270` (inferred): counts the object's halfword at 0x46 down
  while a byte of the game's state is 0, and on its wrap looks at the
  word at offset 4 of the object that its word at 0x3c points to; depending
  on that word, on a byte of that object and on another byte of the
  game's state it calls `func_80120028` or sets two of its own bytes.
  Two points of form, both ordinary C: the decrement is written with a
  local (`t = obj->field_46 - 1; obj->field_46 = t; if (t & 0x8000)`;
  with `obj->field_46 -= 1` the unit differs in 2 slots, the worker's
  figure, not measured again at the top level), and the test before the
  call reads the halfword as signed. No stand-in.
- `func_80157380`: declared in `protos.h` and called by three units of
  the character modules. When a byte of data is at least 3 and the
  object's byte at 0x49 is 0, it has `func_801460ec` fill three
  pointers and sets up the three objects they point to. No stand-in.
  Two notes:
  - A local, `s3`, gets no value in the function, and its value is
    stored into each of the three objects: the original stores a
    register that it saves and never sets. The unit has a comment that
    says so and claims nothing about the value. `func_80142c70`, exact
    since earlier, has the same local. The compiler's warning for it
    is the only one in the two units.
  - The call passes the array of pointers through a cast to the
    callee's declared parameter type; the cast was added at the top
    level and the unit built again.
- Other files: `symbols.ld` loses `func_80157380`, which the unit
  defines now. The inventory's two rows get their names (the sweep's
  output). One wording correction on the library page, from the review
  of pull request 292: `_addque2` has two stores and a read through the
  cast, not "a store and two reads".
- What ran: `matchbuild.py --image resident`: `RESULT: PASS`.
  `fndiff.py --rebuild`:
  - `s157270x_r1`: `baseline 240 bytes at 0x80157270, built 240 bytes at 0x80157270, 0 differing instruction slots: IDENTICAL`
  - `s157380x_r1`: `baseline 588 bytes at 0x80157380, built 588 bytes at 0x80157380, 0 differing instruction slots: IDENTICAL`
  The byte-offset scan on the two units: none. The map's render:
  `overall: 5550/5600 distinct functions exact (99.1%); 13002/13072 placements (99.5%); 1773440/1809836 code bytes (98.0%)`.
  No whole build, no PC build, no differential test.
