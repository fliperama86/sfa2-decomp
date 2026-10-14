## Port-only intro background recovery (2026-10-14)

The owner authorized probing the supplied CPS2 ROM, Japanese Saturn version
and PS1 disc for arcade-style intro restoration. Their explicit boundary is
permanent: nothing under `ps1/` changes for this feature. Future playback uses
the normal port, behind a default-off experimental setting.

The first asset-recovery increment is `port/intro/assets.py`. It decodes the
twelve background rectangles referenced by the Saturn opening's initial setup,
with their initial palettes, into private PNGs and a dependency manifest. It
accepts the examined file hashes only, checks bounds and rejects overwrites.
The source is project-authored format interpretation, not decompiler output,
retained executable bytes or copied emulator code. No game data is published.

The narrowed private comparison found all 211 distinct nonuniform 8x8 cells
of the Gouki portrait in the PS1 ending container `END00.PAC`. All 450 distinct
nonuniform flame cells match the supplied CPS2 graphics after the observed
index conversion. Newly selected portrait matches were checked by decoding
the actual PS1 container, with its baseline hash verified. Flame matches were
checked independently with the existing planar decoder; each one-pixel
mutation was rejected. These are indexed-pixel identities, not proof of equal
RGB palettes or an entire hidden PS1 intro. The supplied arcade revision is
US; the console discs are Japanese.

Validation: `python port/tools/test_intro_assets.py`, nine synthetic controls
passed in 0.023 seconds. The pinned-input decoder ran successfully and wrote
twelve rectangles; representative images were inspected. The new targeted
catalog comparison took 1.032 seconds. Existing disc inventories and decoder
controls were reused, not rerun. Private reports and their manifest are under
`port/local/intro-asset-probe/`; the manifest records the exact source commit.
No game, emulator, port build, PS1 build or hardware was started.

This is **not a scene player**. Map placement, sprites, palette effects,
sequencing, audio and arcade timing remain unresolved. The tool takes extracted
opening files, not disc images directly. Default runtime behavior, build
inputs and settings are unchanged. See `port/intro/README.md` and the bounded
task contract for limitations and the next runtime boundary.

Investigation lesson: compare Saturn dependencies directly against PS1 data,
not only the subset that already matched a particular CPS2 index conversion.
The latter filter missed the portrait even though all its nonuniform cells
are present on PS1. A cross-platform filter is not an exhaustive asset search.
