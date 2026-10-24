## The endings' text from the arcade (2026-10-24)

By the owner's go: the endings as the arcade has them. The PS1's ending
text is the US arcade's English text, kept as plain characters ending in
"@@"; the arcade keeps it as a 16-bit word a character (credits as plain
bytes). `port/arcade/texts.c` makes the PS1's strings from the arcade's,
with a table from `texts.py --c-table`, for every screen module whose
strings the arcade has (87 chunks, 9,814 strings). Strings found in fewer
than 3 per chunk are left out (characters met by chance in tile data).

Checked on 2026-10-24: `texts.py --compare-c` (every string of the table
equal to the disc's), the unit test (39 cases), `--arcade-check` status 0
(269 KB of text served, coverage 45,719,932 bytes), and a start from a
pack made again with this build (the pack's digests now include the text,
so a pack made before is refused with the line that says to make it
again).

Not from the arcade: 1,284 of the endings' strings (lines the PS1
rewrote, its own staff names and grade words), the four PS1
illustrations and most of the logo screen. They stay the pack's.
