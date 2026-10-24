## Stages: the extra animation maps and the shared palettes from the arcade (2026-10-24)

Toward no disc. Two steps for the stage files:

- Extra animation maps. Eight stages keep, besides their three layer maps,
  further maps of layer 1 or 2 in the stage module: the arcade's animation
  frames of the layer, as stacks of pages of two arcade blocks side by side.
  The stage descriptor now names them (`struct arcade_stage_xmap`: the map's
  place and its block numbers); their cells follow the layer's own in the
  packing scan, so their tiles take the PS1's page slots by the same rule.
  `stages.py` finds the maps itself and drops any map with a cell that
  differs from the disc. Stage 06's extra map (layer 0) is not covered.
- The second palette chunk, the same in every stage file: 110 of its 128
  palettes are arcade rows, now built; the other 18 stay the disc's.

Checked on 2026-10-24: `stages.py --compare-c` (the C equals the model for
all 20 stages; every known byte equals the disc's; the table equals the
tool's), the unit tests (stage 21, stages 41), `--arcade-check` status 0, a
pack made again (22 MB, from 23 MB) and its start. The built bytes equal the
disc's, so no fight was run for this change.
