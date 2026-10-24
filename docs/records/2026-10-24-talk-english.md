## The talk before a fight in the arcade's English (2026-10-24)

With the owner's ask for the English text. The PS1's talks (`RDMnn`,
`BOSSnn`, `GDEMOnn`) are a script in the shared boss_demo module and a
Japanese text page a file, which the module draws box by box as sprites.
With `arcade_assets` the loader now draws the US arcade's English talk into
each page in the arcade's 8 x 16 font and writes the English line counts
into the script (`port/arcade/talk.c`, table from `talk.py --c-table`; the
design is in `port/arcade/README.md`, "Talk before a fight"). The talk with
three English boxes ends after its third; BOSS0D's fifth English line is
not shown (four boxes a page).

Checked on 2026-10-24: `talk.py --compare-c` (84 of 84 pages equal the
model and the arcade's text, glyph by glyph; the scripts change only as
the rules say), the unit test (34 cases), `--arcade-check` status 0 and a
start from the pack. Not seen on screen: no input script reaches a rival
or boss fight yet. The talk-to-file mapping is inferred from content.
