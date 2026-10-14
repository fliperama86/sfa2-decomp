# CPS2 intro sequence core

Status: project-authored behavioral C, not a byte-matching reconstruction.
The two original routines are the scoped offline oracle. This is the first
player primitive, not a visible intro, renderer, movie replacement or ROM importer.
It is tracked WIP with no runtime call site and therefore no changed default
behavior. Activation belongs in the normal build behind its shared setting.

## Original behavior and evidence boundary

For the supplied arcade revision, the first intro actor selects sequences via
the helper at `0x18cce`, using a relative-word table at `0x229120`. It advances
via `0x18cb6`. Their shared tail ends at `0x18ce6`. The first table entry resolves
to the already examined six-record intro loop. This connects the previously
candidate table to a real actor consumer, rather than relying on equal timing.
Names are inferred; original symbol names are unknown.

The actor initializer selects by its subtype. Its update includes movement,
horizontal acceleration reversal, sequence advancement and a bounds check.
Thus the scene is not just an animation table. Those routines are traced but
not implemented by this increment. The scene-level spawn scheduler, background
scroll, palette effects, transitions and sound remain separate obligations.

### Host API

`intro_sequence_select` masks its index to eight bits, reads a signed 16-bit
big-endian displacement from `table + index * 2`, and adds it to the table base.
It loads the selected record's full countdown and flags into the state.

`intro_sequence_tick` decrements a wrapping 16-bit countdown. If the result is
nonzero, nothing else changes and no ROM read occurs. If it becomes zero, the
next record is 16 bytes after the current one, unless bit 15 of the *state's*
old flags is set. Then the four bytes immediately after the current record
are an absolute big-endian ROM pointer to the next record. Load both new words.
A zero initial countdown wraps to 65535; 4095 is a long hold, not a magic stop.
No artwork, unused record fields or collision data are interpreted here.

The original object holds its current pointer at byte 0x1c and countdown/flags
at 0x32/0x34. The host owns a typed state with ROM offsets instead; it never
calls a ROM pointer. Object register values and CPU condition flags are not
part of this C API. The offline comparison audits the complete synthetic
object and every original memory write, not only the three returned fields.

### Port-authored safety boundary

ROM maps address zero and is at most 16 MiB. Referenced offsets must be even,
in range and have a full 16-byte record. Branch trailers need four bytes.
State, ROM and their storage must not alias. No 24-bit bus wrapping is supported.
All errors return -1 with state unchanged. This differs deliberately from
original undefined/out-of-range memory behavior; only valid inputs receive a
behavioral-equivalence claim. The no-read countdown path accepts an absent ROM.
The importer must separately verify revision, table ownership and artwork.

## Validation

`python port/tools/test_intro_sequence.py` builds only this C and runs synthetic
fixtures, refusal/atomicity checks, host-state canaries, immutable-ROM checks and
a deliberately wrong-stride mutation. No game inputs, original CPU or game boot.

`verify_sequence.py` accepts only the private pinned opcode image used in this
investigation. It runs only the two routines with Unicorn, against made-up data,
then compares the native C. The ten published fixtures contain no game values
or instruction bytes. They include signed relative offsets, masked indices,
zero wrapping, all flags, sequential/absolute/self branches and an address-zero
record. Random cases supplement them, not replace the named boundary cases.
Every original read/write is range-audited. Unexpected PCs, writes, nonreturns
or mismatches fail. A mutated native stride must disagree with the oracle.

The tool writes exclusive-created private evidence with input and source
hashes. See the dated record for actual commands, commits and results; these
paragraphs specify the check rather than claiming it already passed.
