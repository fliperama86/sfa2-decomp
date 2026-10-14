# Work package: search PS1 intro animation tables

## Objective
Test the owner's PS1-first preference against animation data, not just shared
portrait pixels. Search for the first Saturn opening scene's five referenced
sprite schedules in PS1 data, then inspect candidates' artwork and consumers.

## Pinned contract
Interpret the two platforms' 12-byte sequence records using their respective
sequence readers. Saturn stores a 16-bit artwork offset and branch at byte 6;
PS1 stores a 32-bit artwork offset and branch at byte 8. Compare durations,
flags and active relative branches, not platform-specific artwork addresses or
unused branch/collision fields. Record artwork-alias equality separately.

Search every halfword-aligned offset of every non-STR/non-XA file listed in the
supplied PS1 baseline manifest. Check every selected file's size and digest.
Never infer scene identity from timing alone. Preserve all candidates; a hit
limit, malformed reference, missing file or hash mismatch fails explicitly.
This is not a search of compressed, sector-slack or streamed data.

## Scope
Own `port/intro/animation.py`, its synthetic control, control-selection entry
and documentation. Nothing under `ps1/` changes. Import the existing disc
reader read-only. No runtime hook, Saturn dependency or player is introduced.

## Acceptance commands
`python port/tools/test_intro_animation.py`: subsecond synthetic cases for
endianness, field relocation, ignored fields, active branches, alignment,
adjacent matches, truncation, limits, mutations and output restrictions.
Run the scoped scanner once on supplied inputs, expected seconds to a minute.
Reuse the earlier portrait evidence. No game launch or build.

## Evidence
Private reports under `port/local/intro-asset-probe/`, naming source commit,
input hashes, searched/excluded files and candidate offsets. Original tables,
artwork, pseudocode and extracted bytes stay private.

## Decisions and escalation
An equal schedule is a candidate, never a recovered intro by itself. Follow
its artwork and pointer references. Distinguish the sprite sequences from
background-page animation and palette cycling, which have separate controls.
If no candidate is confirmed, report the exact boundary, not whole-disc absence.

## Completion
Scoped tests and a bounded candidate analysis, publication review and a small
PR. PS1-only full-intro feasibility remains open until all dependencies match.
