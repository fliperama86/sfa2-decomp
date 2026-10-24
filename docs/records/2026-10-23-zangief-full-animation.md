## Zangief plays his full animation set (2026-10-23)

By the owner's go of 2026-10-23. Against any other fighter the PS1 loads
Zangief's PL10 and PL10X, which hold a cut set (890 steps, 272 cell
lists); his mirror module PL16 holds 993 steps and 376 lists with the
arcade's art. With `arcade_assets` the loader now writes PL16's set into
PL10 and PL10X with data only (`port/arcade/full_sets.c`, its table from
`full_sets.py --c-table`; the design is in `port/arcade/README.md`, "Full
animation sets"). PL16's cell lists and sprite chunk (732 KB) go to the
port's new extra memory, 1 MB after the RAM at `0x80200000`, filled at the
start and then read-only; the game reaches them through its own 32-bit
offsets. These ranges are served to the game's disc reads only, so the
module checks of `modules.c` still pin the disc's chunks.

Checked on 2026-10-23: the unit test (30 cases); `full_sets.py
--compare-c` against the disc (all 226 sequences of both files, the 11
steps outside the step block and all 9,648 sprite tiles resolve to PL16's);
`--arcade-check` status 0 with coverage unchanged; a fight from the pack,
Zangief on the left against Dhalsim, two rounds with no stop, sprites
drawn whole. Not run: Zangief on the right side (an input script drives
only the first pad), and the Mac and Windows builds (the Mac has no extra
memory yet and plays the cut set with a line).

`full_sets.py` also reports that Sakura (PL0D against PL15) and Gen
(PL11, PL13 against PL17) do not follow Zangief's pattern: their tables
map, but the data after the frame records differs, so they need work of
their own.
