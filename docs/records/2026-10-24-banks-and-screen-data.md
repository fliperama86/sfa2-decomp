## Silent sample banks and the screen modules' data from the arcade (2026-10-24)

Toward no disc. Two steps:

- With every sound request the arcade's effect or silent, the PS1's
  sound banks' samples (the 120 VAB bodies, 13 MB) are never heard. The
  loader builds them as zeros (their first word stays: the disc stream
  checks it); the banks' headers and sequences stay, since the sound
  library parses them.
- `port/arcade/module_data.py` now covers the screen and stage modules
  (END, CDEMO, CONT, DEMO, SELECT, SELECTA, the talk module and the 20
  stage modules): their pointer words, the tables equal to runs of the
  arcade program and the arcade's palette rows (new layout letters `k`,
  `z`, `K`, `Z`: a PS1 palette row from an arcade row, with the screens'
  black and bit 15 rules), about 118 KB once per image, all equal to the
  disc's.

Checked on 2026-10-24: `module_data.py --compare-c` (every row equal to
the model and the disc), the unit tests (module data 33 cases),
`--arcade-check` status 0, a pack made again (25 MB, from 38 MB), its
start, and a fight from it: 393 requests played as the arcade's effects,
6 silent, none as the PS1's.
