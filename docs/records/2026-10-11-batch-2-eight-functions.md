## Batch 2: eight functions exact with labelled stand-ins, in one pull request (2026-10-11)

- Eight functions that were exact only with a stand-in are published
  together: `func_801e0a78_slot0b`, `func_801b1934_slot04_02`,
  `func_801b3564_slot04_06`, `func_801cb564_slot05_06`,
  `func_8011eb4c`, `func_800e0b48_slot0f`, `func_8001188c_slot27`,
  `func_80142030`. Each was prepared on a branch of its own (unit,
  build entry, removal of its readable C, its page lines, its record)
  and the eight branches are merged here onto main at `52faaf3` (which
  holds batch 1, pull request 261). The stand-ins are those of the
  guide, each under its fixed comment with the measured figure; each
  function's record beside this one says what they are and what was
  tried in their place.
- The conflicts between the branches, and with main, were of one kind
  and resolved by taking both sides' removals: two branches each took
  its own lines, or its own name in a list, out of the same place of a
  page of nonmatching functions (`modules_nonmatching/README.md`), or
  its own alias line out of the same place of `symbols.ld`. One list
  entry of that page was left without a name and is removed. Two units
  were renamed after their addresses before the merge (`d1eb4c_r1`,
  `d42030_r1`).
- What ran on the merged tree: `matchbuild.py --image` for `slot0b`,
  `slot04_02`, `slot05_02`, `slot04_06`, `slot05_06`, `slot0f`,
  `slot27`, `resident`: `RESULT: PASS` each. `fndiff.py --rebuild`:
  - `slot0b_0a78x_r1`: `baseline 624 bytes at 0x801e0a78, built 624 bytes at 0x801e0a78, 0 differing instruction slots: IDENTICAL`
  - `slot04_02_1934x_r1`: `baseline 264 bytes at 0x801b1934, built 264 bytes at 0x801b1934, 0 differing instruction slots: IDENTICAL`
  - `slot04_06_3564x_r1`: `baseline 876 bytes at 0x801b3564, built 876 bytes at 0x801b3564, 0 differing instruction slots: IDENTICAL`
  - `slot05_06_b564x_r1`: `baseline 872 bytes at 0x801cb564, built 872 bytes at 0x801cb564, 0 differing instruction slots: IDENTICAL`
  - `slot0f_0b48x_r1`: `baseline 172 bytes at 0x800e0b48, built 172 bytes at 0x800e0b48, 0 differing instruction slots: IDENTICAL`
  - `slot27_188cx_r1`: `baseline 236 bytes at 0x8001188c, built 236 bytes at 0x8001188c, 0 differing instruction slots: IDENTICAL`
  - `d1eb4c_r1`: `baseline 792 bytes at 0x8011eb4c, built 792 bytes at 0x8011eb4c, 0 differing instruction slots: IDENTICAL`
  - `d42030_r1`: `baseline 248 bytes at 0x80142030, built 248 bytes at 0x80142030, 0 differing instruction slots: IDENTICAL`
  The map's render:
  `overall: 5495/5600 distinct functions exact (98.1%); 12931/13072 placements (98.9%); 1737448/1809836 code bytes (96.0%)`. The figures in the eight functions' own records are those of each
  branch alone. No whole build, no PC build, no differential test.
