## Analysis tools for the arcade assets (2026-10-23)

By the owner's direction the port is to need no PS1 disc in the end, and,
asked to work in parallel and to put the heavy work in reusable scripts,
the lead had helper agents write analysis tools under `port/arcade/`, each
with a fast unit test on synthetic data: `program_map.py` (every byte of the
program's data classed), `fighters.py` (cell lists, positions and tile
numbering of every fighter from the arcade's art records), `fighter_parts.py`
(palettes and portrait pages), `stages.py` (stage layers, tiles and
palettes), `samples.py` (sound samples) and `sequences.py` (animation steps
and frame records). They read the user's disc and arcade set, print a
compact report and write nothing; the runtime converter will be C. Their
findings are on [the arcade page](../../port/arcade/README.md).

What ran: each tool over all of the disc's files with the user's inputs
(between 6 and 223 seconds each) and each unit test (all pass). From them,
24 more tables of the program's data went into `program.c` (25,784 of
90,088 bytes, all equal to the disc's in `--arcade-check`).

The owner noted that the PS1 gives some fighters fewer animation frames in
some matchups (Zangief); the step analysis confirms it (PL10 lacks 245 of
the arcade's records that PL16 has). The arcade build is to use the
arcade's full set.
