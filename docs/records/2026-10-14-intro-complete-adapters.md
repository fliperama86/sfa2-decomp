## Complete-intro dependency discovery and native graphics adapters (2026-10-14)

The owner's acceptance boundary is now the **entire intro**, not another
Akuma/fire slice or cosmetic addition. No owner test, launch or PR is requested
for this increment. The normal opening still runs the earlier bounded slice;
the full native controllers, actors, import and sound are not implemented.

## Original-program discovery, not native playback

A private, bounded offline probe followed all eight major intro phases through
the original final return. It executes the supplied program only as a development
oracle. It is not linked into the port or used to generate runtime playback.
The first successful controller-only discovery ended at iteration 2947. A later
probe also executed the original object renderer, producing 104 private state
snapshots. This exposed 443 artwork descriptors across formats 0, 2, 4, 6, 8 and
10, and 115 sequence-table/index selections. Both primary and secondary actor
pools are required; omitting the latter would drop intro effects and title UI.

The snapshots were composed with the new native graphics adapter to check the
shared layout interpretation. Representative pictures from every phase were
inspected privately. These are **original-state visualizations**, not proof that
the native port can play those scenes. No captured object lists, frames or
original instructions are runtime assets.

The probe still substitutes a bounded cooperative-yield/fade scheduler and does
not run the game's boot or interrupt pipeline. Its iteration count is not an
exact video-frame timing claim. Two sound-command boundaries were observed;
their soundtrack implementation remains unresolved. The native full-scene
controller must be reconstructed and tested, not replaced with a trace replay.

Earlier failed discovery attempts remain private and preserved. Important
corrections were initializing the original map-base registers before map writes,
respecting opcode-versus-data reads, reading descriptor metadata from the data
plane rather than a temporarily patched instruction page, and initializing the
white/black palette banks. The final discovery includes those initial palettes.
The inspected contact sheet predates that last palette initialization correction;
it must not be cited as validation of flashes or complete palette timing.

## Native source at 689c1cae

- `port/intro/video.c`: all three scrolling planes, 8/16/32-pixel graphics,
  alternating 8-pixel halves, rowscroll, brightness, sprite blocks/flips/banks,
  layer ordering and object occlusion, using bounded private indexed resources.
- `port/intro/sprites.c`: the observed original metasprite consumers, including
  palette overrides, signed expansion/rotation, wrapping/clipping and cell
  budget behavior. Unsupported mode-8 bypass is rejected explicitly, not guessed.
- Contracts, synthetic fixtures, isolated controls and an offline differential
  verifier. The normal build picks up both C units; runtime routing is unchanged.

Scoped checks and existing-evidence reuse:

- Native compositor controls and three real C mutations passed in 1.289 seconds,
  with undefined-behavior sanitization of the unmodified unit. An earlier
  rowscroll mutation survived a periodic fixture; the failure is preserved and
  the fixture now distinguishes high row bits instead of repeating every tile.
- Four metasprite unit controls passed in 0.993 seconds: 59 original-computed
  synthetic fixtures, atomic rejection, oracle audit guards and four rejected
  mutations. The first expansion-width mutation survived the initial fixtures;
  a focused signed-product truncation fixture corrected that evidence gap.
- Two isolated original-versus-C runs, seeds 0 and 7, each passed 24,000 cases
  plus the 59 fixtures. Each seed includes 4,000 cases per format, split into
  2,000 per bypass variant where applicable. Original memory/control-flow audits
  recorded 151,402 and 150,408 writes respectively. All four mutations rejected.
- The normal four-job Windows build compiled 3,982 units with zero failures,
  linked the ordinary executable and verified addresses. No game was launched.
- Unchanged sequence, particle, settings, GPU and PS1 matching evidence was not
  rerun. No macOS runtime claim is made.

Private logs, discovery failures/successes, source hashes and the evidence
manifest remain under the existing `port/local/intro-asset-probe/` purpose folder.
The normal executable must not be announced as a complete-intro candidate:
it still selects the old first-scene runtime. The owner receives no further
partial test builds. The next work is native scene/actor reconstruction and
typed resource import, then sound and complete normal-route integration.

Publication review: additions are project-authored C, tooling, contracts and
synthetic numeric fixtures. No game assets, binary instructions, private paths,
third-party source or decompiler output are included. Hardware reference files
remain in the private research checkout with their original notices. Nothing
under `ps1/`, another worker or the integration checkout was modified.
