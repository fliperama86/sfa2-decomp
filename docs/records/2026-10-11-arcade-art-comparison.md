## Automated character artwork comparison (2026-10-11)

The owner requested a small Python tool after the one-character probe: Ryu's
idle and one punch cannot represent all animations or characters, particularly
Zangief. The new offline tool is in `research/asset_compare/`. It changes no game
source and implements no arcade-assets option.

It reads user-supplied first-side PS1 PAC files, derives character mappings from
reconstructed initialization C, and compares their complete contiguous drawing
and animation catalogs against the supplied CPS2 archive. Exact pixels, storage
flips, anchors, near-difference candidates, unpaired artwork and visual timer
records are separate. It does not pair moves by ordinal or claim gameplay
identity from matching art. A cross-catalog check distinguishes one PAC's absent
pictures from artwork stored in another PS1 PAC.

All 24 PAC IDs were attempted in about 24 seconds. Thirteen catalogs decoded;
eleven remained explicitly unresolved. The partial run returned 2 and retained
its JSON/CSV and selected previews privately. It is not a completed full-game
asset audit. Thirty-three isolated synthetic tests passed in under a tenth of a
second, including malformed/ambiguous inputs and partial-report controls. The
cross-catalog postprocessor was applied separately to the existing report; the
unchanged decoding run was not repeated. Exact commits, source/input hashes,
commands, actual exits and logs are recorded in private evidence.

Zangief demonstrates why an archive-level difference is not a whole-disc absence
claim: arcade pictures unmatched in its first PS1 catalog also appear in another
PS1 character catalog. Their availability does not show which gameplay version
uses them. No extra-frame gameplay restoration or blanket replacement is chosen.

Workflow corrections/preferences: the owner confirmed their arcade ROM is good
and instructed its use. Differences from current MAME graphics hashes are input
characteristics, not proof of corruption or a reason to repeatedly request a
replacement. The earlier hash-audit regex lost reference records by allowing
newlines inside names; verify record coverage before interpreting mismatches.
Do not generalize a successful small sample to all characters or moves.

First-publication review: authored Python/docs/tests only; no game binary,
extracted assets/tables, decompiler output, third-party source, credentials or
personal/machine paths. Primary references are linked in the tool README.
