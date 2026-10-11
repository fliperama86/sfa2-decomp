## Batch 1: six functions exact with labelled stand-ins, in one pull request (2026-10-11)

- Six functions that were exact only with a stand-in are published
  together: `func_800e0f2c_slot0f`, `func_80135a10`,
  `func_801b37b0_slot04_03`, `func_8013172c`, `func_80013224_slot12`,
  `func_8012f9b0`. Each was prepared on a branch of its own (unit,
  build entry, removal of its readable C, its page lines, its record)
  and the six branches are merged here onto main at `7195a20`. One pull
  request for six, by the owner's word of the same day that publishing
  one at a time takes too long: with six separate ones each merge
  would send the others back for a merge of main and a new approval.
- The stand-ins are those of the guide ("What is allowed": the named
  ones and "Other stand-ins"), each under its fixed comment with the
  measured figure; each function's record beside this one says what
  they are and what was tried in their place.
- The conflicts between the six branches were all of one kind and
  resolved by hand: two branches each took its own lines out of the
  same place of a page of nonmatching functions (`resident_nonmatching/
  README.md` at four places, `modules_nonmatching/README.md` at one):
  both removals stand. One unit (`d2f9b0_r1.c`) was re-indented to the
  tree's four spaces after the merge: text only.
- Two units were renamed after their addresses before the merge
  (`d35a10_r1`, `d3172c_r1`); their branches' records say the names.
- What ran on the merged tree: `matchbuild.py --image resident`,
  `--image slot0f`, `--image slot04_03`, `--image slot05_03`,
  `--image slot12`: `RESULT: PASS` each. `fndiff.py --rebuild`:
  - `slot0f_0f2cx_r1`: `baseline 232 bytes at 0x800e0f2c, built 232 bytes at 0x800e0f2c, 0 differing instruction slots: IDENTICAL`
  - `d35a10_r1`: `baseline 508 bytes at 0x80135a10, built 508 bytes at 0x80135a10, 0 differing instruction slots: IDENTICAL`
  - `slot04_03_37b0x_r1`: `baseline 552 bytes at 0x801b37b0, built 552 bytes at 0x801b37b0, 0 differing instruction slots: IDENTICAL`
  - `d3172c_r1`: `baseline 296 bytes at 0x8013172c, built 296 bytes at 0x8013172c, 0 differing instruction slots: IDENTICAL`
  - `slot12_3224x_r1`: `baseline 556 bytes at 0x80013224, built 556 bytes at 0x80013224, 0 differing instruction slots: IDENTICAL`
  - `d2f9b0_r1`: `baseline 328 bytes at 0x8012f9b0, built 328 bytes at 0x8012f9b0, 0 differing instruction slots: IDENTICAL`
  The map's render:
  `overall: 5487/5600 distinct functions exact (98.0%); 12922/13072 placements (98.9%); 1733100/1809836 code bytes (95.8%)`. The figures in the six functions' own records are those of each
  branch alone. No whole build, no PC build, no differential test.
- After the review of the pull request (comments and the guide only;
  no code changed: the three touched units rebuilt IDENTICAL, the
  images `resident` and `slot12` PASS): four general stand-in comments
  end with the guide's fixed sentence again, their further figures in a
  comment beside each (`d2f9b0_r1.c` twice, `d3172c_r1.c`,
  `slot12_3224x_r1.c`). And the string constant of
  `func_80013224_slot12` is not a stand-in for registers, order or
  frame: it gives the unit's owned read-only words the original's
  three bytes behind a nine-byte table (the build's tool takes a
  unit's read-only data in whole words, so the ownership cannot end at
  nine). The guide gets a short paragraph for that one case under
  "Other stand-ins", with a comment of its own that counts bytes of
  read-only data and says that no instruction reads them; it sets this
  apart from embedded code or assets. A proposal with this pull
  request: if it is declined, that function leaves the batch.
