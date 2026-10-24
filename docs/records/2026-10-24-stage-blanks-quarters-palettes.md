## Stages: blank object tiles, quarter tiles in the pages, and palettes by design (2026-10-24)

Toward no disc, by the owner's rule "rebuild arcade-style".

- Object tiles. 938 tiles of the stage files' object tile chunk have no
  arcade source (937 in stage 04: its bicycle riders, which the arcade draws
  as layer 2 animation blocks, already built; 1 in stage 01). They are blank
  by design, from an explicit counted table in `stages.py`; the chunks of
  stages 01 and 04 are then built whole from the arcade and served to every
  read (`owned`).
- Pages. 200 page slots of stages 12, 01 and 0F are quarters of the
  arcade's 32 x 32 tiles (the rule is now shared, `port/arcade/quarter.h`).
  Stage 0D's 16 slots with no source are declared and stay unbuilt for now.
- Palettes. The shared second palette chunk's 18 PS1-made palettes and 63
  first-chunk rows take the nearest arcade row (at most 6 of 15 entries
  differ) or are blank, from explicit tables; these bytes differ from the
  disc's by design and are served owned.

Checked on 2026-10-24: `stages.py --compare-c` (the C equals the model for
all 20 stages), the unit tests (stage 24, stages 46, sprite tiles 25),
`--arcade-check` status 0, a pack made again (18 MB, from 20 MB, together
with the screens change of the same day), its start, and a fight from it.
