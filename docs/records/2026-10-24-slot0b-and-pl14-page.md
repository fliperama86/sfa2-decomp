## The fight's shared screen module and Shin Gouki's portrait page from the arcade (2026-10-24)

Toward no disc.

- The fight's shared screen-object module (table 0 slot 0xb, the same in
  24 fighter files): its rotation table (a new layout letter `j`, 4 PS1
  bytes from 8 arcade bytes, from the arcade's table), the fighter names
  (a recipe from the arcade's name list in the PS1's order and centring;
  the one byte where the PS1 writes "CHUN-LI" stays the disc's) and the
  19 portrait shapes (each an arcade portrait record cut into 16 x 16
  cells: counts, palette and coordinate arrays) are built equal to the
  disc (`module_data`). The wipe maps and the PS1's own palette rows stay
  the disc's.
- PL14's picture page (Shin Gouki) is Gouki's arcade portrait plus the
  shared VS records' cells; its portrait palettes are Gouki's arcade
  ones, which differ from the disc's by design. A picture page is no
  module chunk, so it is served owned (`fighter_parts`).

Checked on 2026-10-24: the unit tests (module data 37, fighter parts 25
and 13), `module_data.py --compare-c` and `fighter_parts.py --compare-c`,
`--arcade-check` status 0, a pack made and started, a Gouki fight that
loads slot 0xb, and a Zangief session through select, fight, win and
continue. PL14 itself was not loaded in a run.
