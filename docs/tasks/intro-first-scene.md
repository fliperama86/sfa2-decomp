# Work package: owner-testable intro scene slice

## Deliverable and boundary
Normal port, same settings and executable, default-off `arcade_intro`. Replace
only the opening movie with a 192-update Gouki/fire scene slice, then return to
the normal title flow. Owner must receive a runnable build before a new PR.
No ps1/ changes, emulator/interpreter, external preview executable or FMV.
The whole-port CPS2 input pivot is not part of this work.

## Contract
Read the supplied arcade ZIP offline, verify the already investigated revision,
and extract only private graphics/palettes and a relocated animation table.
No decrypted program required by importer or runtime. Six foreground poses are
selected by the existing tested C sequence reader (four updates each). The
background map advances every six updates. The portrait palette cycles through
three banks in a forward/backward four-update cycle. Native C composes the
layers at 384x224; foreground anchor is (192,224), derived from object/camera
code, not visual imitation. Background map Y starts at 16 (hardware viewport).
The warm-up and flash states, random particles, sound and subsequent scenes
are not implemented. This bounded segment starts fully lit; row-scroll and
palette cycle phase are normalized to zero rather than claiming the exact
warm-up's carried state or original pipeline latency. The native port's 60 Hz
clock is used; no claim of cycle-exact CPS2 refresh timing.

## Input format and safety
Private pack SFA2I001: 8-byte magic, SHA-256 of payload, then fixed-size payload:
102 bytes of relocated BE sequence data; 16 sprite, 512 scroll2 and 48 scroll3
BE RGB12 palette entries; six 384x144 indexed foregrounds; four 256x256 BE
palette-index scroll2 maps; one 512x256 indexed portrait. Pen 15 transparent.
Runtime rejects bad length/hash, bad palette indices and malformed sequences.
Disabled behavior does not open the pack. An enabled missing/bad pack is an
early explicit error, not silent substitution. Pack and inputs never enter Git.

## Integration
Feature source in port/intro/, startup and opening dispatch in port/src/.
Compile feature C into both existing host builds. Narrow GPU API uploads a
host RGB555 frame, explicitly enables 384-wide display and clears that mode
on exit. Use the existing clock and input; Start skips, Escape closes.

## Checks and cost
Synthetic importer/pack/scene fixtures, settings parser roundtrip, opening hook
and graphics command stubs, each seconds and no running game. Existing unchanged
sequence oracle evidence is reused. One normal Windows build reuses the existing
PsyZ library; no PS1 matching build. Optional bounded visual run only answers
new framebuffer/display integration uncertainty. No PR or merge before owner test.

## Importer checks caught during authoring
Do not extrapolate all pose bounds or flip attributes from pose zero. The six
real records include whole-block X/Y flips and 144-pixel-tall flames. All six
are inspected and supported, with explicit bounds and synthetic flip controls.
