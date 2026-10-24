## Gen: the second block of steps from the arcade (2026-10-24)

Toward no disc. PL17 (the second Gen of a mirror match) holds a second
block of animation steps (1,146 steps, a 272-entry table and 293 frame
records of its own), the copy of the arcade's second sequence group. It is
now a second row of the sequence table (`SECOND_BLOCKS` in `sequences.py`,
`arcade_sequence_find_nth`) and built equal to the disc: 1,145 of 1,146
steps, 292 of 293 frames and all 272 entries; the two that differ stay the
disc's. The arcade keeps copies of some art records, and the PS1 gave each
copy the original's list; a table of aliases (numbers only) maps them, which
also builds a few more steps of other fighters.

Checked on 2026-10-24: the unit tests (sequence, sequences, module data,
fighter), `sequences.py --compare-c` and `module_data.py --compare-c`,
`--arcade-check` status 0, a pack made and started, and a Gen fight from it.
PL17 itself loads only in a Gen mirror match, which the scripted runs cannot
pick; it is checked by the tools and the pack's digest.
