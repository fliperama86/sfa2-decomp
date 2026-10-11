# Offline character artwork comparison

A small, standard-library Python tool compares character drawing catalogs from
the user's PS1 Japan disc and CPS2 Alpha 2 ROM archive. It does not run the game,
change game code, or install an emulator. Inputs and reports stay private.

From the repository root:

```sh
python research/asset_compare/compare.py \
  --ps1-pac-dir PS1_EXTRACT/PAC --arcade-zip ARCADE.zip \
  --characters all --output research/local/art-comparison
```

`--characters 00,10` selects Ryu and Zangief. Other selectors are hexadecimal
PAC IDs, `00` through `17`, rather than assumed character/move names.
`--previews 8` optionally writes up to eight unmatched grayscale pictures per
platform/character. These are private artwork, not files to publish. `--output`
must be a new directory; existing inputs and earlier reports are not overwritten.
The comparison typically costs seconds per character, not a build or game boot.

## Reports

- `summary.csv`: decoded picture counts and content-paired animation totals.
- `report.json`: input/source hashes, discovered catalog boundaries, picture
  dimensions/origins, exact matches, flip-equivalent coverage, unmatched pictures,
  candidate small pixel differences and individual animation comparison results.
- Optional PNGs: neutral indexed-pixel previews, not recovered palette colors.

The tool walks every composition and animation record in each discovered
contiguous catalog. It is not limited to idle and one punch. Duplicate drawing
references are counted separately from distinct pixels. Horizontal/vertical
storage flips are a separate equivalence class, not extra artwork. Transparent
padding is cropped, but the painted origin is retained and compared separately.

Animation pairing uses the complete ordered pictures, timer fields, loop index
and held terminal picture. It never assumes that the same ordinal identifies the
same move. Multiple possible matches remain explicit. External transitions and
undecoded pictures are unresolved, not equal. Timer fields are not a runtime
measurement of displayed frames, speed settings or hitstop; gameplay fields and
collision records are not compared.

An unmatched arcade picture is not automatically a missing PS1 frame. The
cross-catalog check reports exact matches in other decoded PS1 PAC files. This
matters for alternate character archives. Even pictures not found in the decoded
catalogs cannot be called absent from the whole disc while catalogs are unresolved.
Near pixel matches are candidates for inspection, not verified move counterparts.

## Supported inputs and limits

- PS1 Japan `SLPS_004.15` extracted first-side `PLxx.PAC` files and the project's
  reconstructed initialization C. The tool derives coordinates, drawing base
  and dispatch pointers from that C, pins its hashes, and bounds-checks the
  corresponding PAC data. No hand-maintained list of character addresses.
- CPS2 `sfa2ur1` program layout, identified by all six program member SHA-1 hashes.
  The graphics members are accepted as supplied, not rejected because their
  hashes differ from a current MAME reference. Different program layouts need a
  separately verified coordinate-table mapping, not a guessed conversion.
- The currently implemented PS1 literal, sparse and input-reference tile codecs,
  composition modes 0/2/4/6, and CPS2 mode-2 rectangular block compositions.
  Unsupported codecs/modes, ambiguous/missing seeds, incomplete catalogs and
  unexpected bounds are reported as unresolved.

CPS2 discovery requires an exact first-picture seed with the same painted origin.
A modified seed is unresolved even if other pictures might match. Thus `all` is
an automated attempt across all PAC IDs, not a promise of complete game coverage.
Catalogs are structurally recovered and content-identified, not runtime-traced;
reachability, graphics outside these catalogs, stage art, effects outside the
character files, original colors and character/move semantics remain unchecked.

Exit 0 means every requested catalog was discovered and its pictures decoded,
not that the versions are equal. Exit 2 means a partial report includes unresolved
characters or pictures. Fatal input/layout errors fail before comparison.

## Validation and provenance

```sh
python -m unittest discover -s research/asset_compare -p 'test_*.py' -v
```

Synthetic tests need no disc, ROM, compiler or running game. They include bounds,
packing, block flips, row addressing, alignment padding, discovery ambiguity,
malformed loops, partial decoding, duplicate artwork, anchors, timer differences,
cross-archive matches and output preservation.

The parsers and tests were authored for this project. PS1 format knowledge comes
from its reconstructed C (`d1bf70_r1.c`, `d1db78_r1.c`, `d1df3c_r1.c` and character
initialization). The consulted primary CPS format definitions are
[MAME's ROM lanes/bank permutation and sprite drawing](https://github.com/mamedev/mame/blob/master/src/mame/capcom/cps2.cpp),
[graphics layouts](https://github.com/mamedev/mame/blob/master/src/mame/capcom/cps1.cpp),
and [plane significance](https://github.com/mamedev/mame/blob/master/src/emu/drawgfx.cpp).
No third-party implementation, extracted game table, graphics or original
executable bytes are shipped with this tool.
