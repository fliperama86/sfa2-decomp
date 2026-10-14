# Work package: CPS2 intro sequence core

## Objective
Implement the first reusable C part of the intro, its original animation-table
selector and countdown/branch reader. Trace the first actor's consumer before
choosing it. No visual approximation, game launch or whole-port asset pivot.

## Pinned contract
Supplied arcade program: the selector at 0x18cce and tick at 0x18cb6, ending at
0x18ce6. CPU data is big-endian. Records have a 16-byte stride. Selection masks
the index to eight bits, reads a signed word offset relative to the table base,
and loads the selected record's countdown and flags. Tick decrements a wrapping
16-bit countdown. Only zero after decrement advances; bit 15 of the old flags
selects an absolute pointer after the current record instead of the next record.
The new record's complete countdown/flags word replaces the old word.

Host API uses ROM offsets, not executable pointers. A byte span maps CPU address
zero. Inputs are separate, nonaliasing objects. Valid offsets are even and within
a maximum 16 MiB span, without 24-bit bus wrapping. Invalid pointers, truncated
records/table entries and out-of-range displacements return -1 without changing
state. This refusal behavior is port-authored, not a claim about invalid original
inputs. Full 16-byte records are required although the original reader loads only
the first four bytes. Tick does not inspect ROM before countdown expires.

## Scope
Own port/intro/sequence.c/.h, the focused fixture/control and original-code
comparison tool, documentation and test-selector metadata. No ps1/ changes.
No movie interception, new runtime input, renderer, audio or settings change.
Playback remains disabled/unconnected until the scene contract is complete.
Saturn remains a possible byte-matching C target, not just a visual reference.

## Acceptance commands
- python port/tools/test_intro_sequence.py: native C, synthetic fixtures and
  invalid-input controls; under one second, no private input or running game.
- python port/intro/verify_sequence.py --opcodes INPUT --seed SEED --cases 2000:
  compare native C with only the two original routines, on synthetic data, for
  seeds 0 and 7. Audit all writes; check a deliberately wrong result is rejected.
  Expected seconds per seed, no game boot. Save private reports, source/input
  digests and fixture verification. The emulator exists only in this offline
  verifier, never in runtime C. Do not rerun unchanged controls.

## Evidence
Private reports use port/local/intro-asset-probe/. Publish only project-authored
C, tooling and synthetic fixtures, never opcode bytes, original tables or
pseudocode. Pin the input digest and fail on unexpected execution or timeout.

## Decisions and escalation
This increment proves the sequence primitive, not intro playback. Particle
movement, spawn schedules, map placement, palette effects and audio remain.
No whole-intro parity follows from one shared actor. STOP and report options
on a spec contradiction or material interface change; minor choices are recorded.

## Completion
Scoped evidence, provenance review and a small PR, with no claim that the
intro is already visible. Later activation uses the normal port/settings path.
