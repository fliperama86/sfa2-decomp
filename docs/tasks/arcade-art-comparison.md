# Work package: offline character artwork comparison

## Objective
A standard-library Python CLI compares full contiguous character artwork and
animation-record catalogs, not a two-animation sample. Validate Ryu and Zangief
from the owner's supplied CPS2 archive and PS1 extracted PAC files.

## Pinned contract
Read-only inputs: PS1 Japan SLPS_004.15 PAC files and reconstructed character
initialization C; CPS2 sfa2ur1 program layout and the supplied graphics bytes.
Pin actual hashes, do not reject graphics because reference hashes differ.
Do not modify or integrate assets, execute original code, or touch ps1 source.

PS1 catalogs: derive coordinate, drawing and dispatch addresses from existing C;
dispatch entry 4 locates base animation pointers and the first sequence. Walk
composition records up to that first sequence (allow zero alignment padding),
then walk 12-byte sequence records to the base pointer table. Unsupported formats,
bounds, codecs and unmatched discovery are explicitly unresolved, never missing.

CPS2: assemble four word lanes and permute banks; decode 16x16 planes. Find the
character's first drawing by exact normalized indexed-pixel comparison against
PS1 drawing zero. Ambiguous/missing seeds are unresolved. Walk the contiguous
mode-2 block-list catalog. Recover preceding 24-byte sequence records by artwork
pointer references, validate the entire range and appended loop pointers. This
is catalog coverage, not proof that every runtime-dependent asset was found.

Compare cropped artwork with palette indices normalized, preserve anchor and
palette-bank metadata separately. Pixel equality is not palette-color equality.
Deduplicate drawings, distinguish exact/canvas-anchor differences and unmatched
pictures. Near pixel differences are candidates, never authoritative move pairs.
Compare complete ordered visual/timer records by content, not list position or
assumed move IDs; retain ambiguity, terminal holds and external transitions.
An artwork/timeline difference is not proof of a gameplay change or omitted move.

## Scope
Own research/asset_compare/, this task, and one new docs/records/ entry.
Private generated evidence under research/local/asset-compare/. No assets,
extracted tables, original/decompiler bytes or personal paths enter Git.

## Acceptance commands
python -m unittest discover -s research/asset_compare -p 'test_*.py' -v
Synthetic bounds, packing/flips, padding, ambiguous discovery, unmatched/partial
coverage, deduplication, anchor, timer and malformed-loop negative controls.
Expected cost under one second, no private inputs.
python research/asset_compare/compare.py --ps1-pac-dir PAC_DIR \
  --arcade-zip ARCADE_ZIP --characters 00,10 --output PRIVATE_DIR
Expected cost seconds to a minute; report actual elapsed time and exit status.
No whole build, unrelated test suite or live game.

## Evidence
Baseline aa6b0a0421acb25d2fee2f4d2fcdd3b04c7d62a9. Pin final source commit and
input digests. Keep earlier exploratory evidence unchanged and name its scope.
New private JSON/CSV and optional previews report discovery/decoding coverage.

## Decisions and escalation
No unresolved architecture decision. Generalize from the tested formats with
explicit limitations, not guessed formats. STOP and report a material design
fork rather than silently importing another donor or altering gameplay.

## Completion
Isolated tests and scoped real-input comparison recorded, provenance/privacy
review performed, branch and PR opened. Do not claim full-game equivalence.
