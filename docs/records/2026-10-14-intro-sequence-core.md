## Original CPS2 intro sequence reader in C (2026-10-14)

The owner clarified why Saturn matters: its C implementation is a potential
byte-matching reconstruction target, not merely a visual reference. They
also authorized proceeding from CPS2 if its controller/tables offer a practical
route. The first intro is the immediate milestone; a whole-port CPS2-only pivot
is not approved or implemented. Nothing under `ps1/` changed.

### Code and data findings

The first intro sprite loop now has a traced CPS2 consumer. The actor at
0x73cb0 selects through the relative-word table at 0x229120 and the shared
sequence selector at 0x18cce, then updates via the reader at 0x18cb6. Its first
table entry resolves to the previously examined six-frame loop. The actor
contains actual movement and acceleration logic, and a bounds check; the
whole intro cannot be supplied by frame tables alone. The corresponding Saturn
actor structure is useful evidence, not a whole-scene equivalence claim.

`port/intro/sequence.c` implements the selector and countdown/branch reader as
project-authored behavioral C. It preserves the byte-masked selector, signed
relative displacement, wrapping 16-bit counter, full flags and absolute branch
trailer. Invalid input gets an explicit, atomic refusal, not original bus-error
behavior. The contract is in `port/intro/sequence-contract.md`.

This is tracked, inactive player groundwork, not visible playback. It has no
runtime call site, introduces no second user input or enabled setting, and
changes no existing behavior. Scene spawning/composition, movement, palettes,
transitions, sound and normal-port hookup remain. Activation will use the
normal program and shared default-off experimental setting, not a separate
preview player. No Saturn compiler or byte-matching result is claimed.

### Validation

On source identical to implementation commit `4c442615`, six initial native
controls passed in 0.634 seconds; the subsequently added verifier-input/output
guard control passed alone in 0.170 seconds. The unchanged six were not rerun.
The controls build only this C, use synthetic fixtures, check state canaries
and immutable ROM, and reject invalid/truncated/misaligned/out-of-range inputs.

At `4c442615`, the offline verifier compared the C against the original two
routines for 2,000 synthetic cases on seed 0 and 2,000 on seed 7. Both runs also
verified the ten named synthetic fixtures. Each took about 0.6 seconds. Original
writes were audited (3,393 and 3,439 respectively), as were read ranges and the
execution boundary. A deliberately wrong native record stride was rejected
in both runs. No game boot, full build or unchanged test suite was run.

The verifier's Unicorn dependency is development-only. Runtime C neither
embeds nor executes original instructions. Private reports and their manifest
pin the opcode input, source digests, compiler and evidence files under the
existing `port/local/intro-asset-probe/` location. The report includes a static
consumer route separately from the behavioral checks; those checks do not
execute the scene controller or establish full intro fidelity.

A separate data-integration probe at `6902b58a` ran the native C reader on the
hash-verified original first-loop data. It selected all six records in order
and returned to the initial state after 24 animation updates. This used no
original CPU execution and did not render the scene. Its private trace and
separate manifest preserve source/report hashes without altering earlier evidence.

### Tool limitations and lessons

REA refused this ELF architecture. The existing private Ghidra project was
reused, with Capstone instruction decoding for the focused trace. A first
linear pseudo-disassembly attempt began in data and stopped; it is not credited
as recovered code. Function entries and call targets were then selected from
actual instructions and references.

Do not apply a blanket "all tables use the original data plane" rule to CPS2.
PC-relative table reads use the program/decrypted plane in the examined core;
ordinary indirect data reads use the original plane. This was checked against
MAME's pinned [PC-relative access implementation](https://github.com/mamedev/mame/blob/32f48ebb75f4fefcc7bcf0482289c0afa57d9973/src/devices/cpu/m68000/m68kcpu.h#L478-L510).
The selector covered here uses ordinary indirect reads into unencrypted data.
The distinction matters for the actor's nearby velocity and dispatch tables.

Publication review: only authored C, format/verification tools, synthetic data
and documentation. No game instruction bytes, extracted tables/assets, raw
pseudocode, third-party implementation, credentials or personal paths enter Git.
