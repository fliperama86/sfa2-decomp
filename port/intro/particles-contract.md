# Bounded intro particle reconstruction

Project-authored, nonmatching C for the supplied US 960306 CPS2 program. No
original instructions execute in the port. Names are inferred. Nothing in
`ps1/` changes. Original tables and graphics stay in the private asset pack.

## Functions and evidence boundary

- `intro_particle_random`: 0x1975a..0x19774. Multiply the 16-bit seed by three,
  wrap, take its high byte, add it to the old low byte (wrap), and store the
  two resulting bytes. Return the new low byte. Zero is a fixed point.
- `intro_particle_spawn`: 0x451c, 0x4560 and 0x45bc through their returns.
  Decrement the 16-bit interval, including zero underflow. Only on zero call
  the RNG and set interval to `(random & 3)+1`. Do so even if allocation is
  suppressed. More than five free slots permits allocation. Phase 0 uses the
  interval as subtype, layer 10, priority 0xa000. Phase 1 changes intervals
  3/4 to subtypes 5/6, layers 8/4, priority 0xe000. Phase 2 maps intervals
  1/2 to subtypes 3/4 and 3/4 to subtype 1, layer 0, priority 0xe000.
  The output descriptor is unchanged when no allocation is requested.
  The caller owns allocation; free counts are 0..40 and phases 0..2.
- `intro_particle_init`: nonzero-subtype branch of 0x73cc4..0x73d2c, including
  0x73d80 and 0x73dfa. Subtypes 1..6. Two RNG calls select a velocity pair
  (bit 3, within that subtype's two pairs), then a spawn coordinate (low
  seven bits). Big-endian supplied tables contain 12 pairs of 32-bit
  velocities and 128 pairs of 16-bit coordinates. Fractional position and
  sway start at zero; acceleration is -0x4000 and the sway limit is 0x24000.
  Invalid subtype returns -1 atomically. Sequence selection and render-list
  insertion are outside this function and are checked at the integration.
- `intro_particle_move`: nonzero-subtype path 0x73d32..0x73d6a plus bounds
  0x74010..0x74034. First add velocity and OLD sway to x, then velocity to y.
  Compare unsigned modular absolute sway with the limit, reversing acceleration
  on equality or above, then add acceleration to sway. All arithmetic wraps
  at 32 bits, including negation of 0x80000000. Mark for removal if the
  unsigned 16-bit integer coordinate plus 32 exceeds 448 (x) or 288 (y).
  Sequence tick between motion and bounds has its separately verified contract.

The isolated offline oracle executes only these original routines, hooks the
sequence/render/allocator boundary, uses synthetic table values, and audits
reads, writes and PCs. Two seeds, boundary fixtures and actual wrong-result
mutants are required. This is not a whole-scene equivalence claim.

## First testable integration

Implement moving embers in the existing 192-update steady slice FIRST. Keep
fades/flashes, warm-up, phase carry-in and the later handoff as the next bounded
increment rather than presenting guessed transitions as recovered behavior.
The original 40-slot LIFO pool is modeled with the main flame actor occupying
slot 39; 39 particle slots remain. Ascending slot updates, deferred deletion
on the next update, and the reserve of five free slots follow the original.
Newly allocated particles initialize, select their first pose and render on
that update without moving. Existing particles move, tick and render; a
marked particle is released next update. The frame compositor draws particles
after the main flames, in ascending slot order. The emitted layer-0 list is
after layer 6 and both use priority 7. Pen 15 is transparent.

The two-byte seed is explicitly normalized to 0x1234 and the initial spawn
interval to four. There is no claim to inherit the original preceding phases'
RNG, particles or clock phase. The original RNG itself is reconstructed, not
replaced with host randomness. Existing brightness/60 Hz limitations remain.

## Private pack versioning

Version 1 remains accepted and displays exactly the earlier no-particle slice.
Version 2 appends 96 velocity bytes, 512 coordinate bytes, 612 relocated
sequence bytes (six offsets and six 100-byte slots), then 36 16x16 indexed
pictures. Subtype 1 has five poses; the other five have six. Early records
last eight updates; each final record holds for 4095 and branches to itself.
Unused bytes are zero (unused image is transparent). Tables are data only.

The importer accepts an optional pinned, privately decrypted opcode image to
extract the PC-relative tables. This is an analysis derivative of the SAME
arcade archive, not another game/ROM requirement. No cipher or interpreter is
added to runtime, no decrypted image is loaded by runtime, and no original
table is published. Without that option, the existing version-1 import stays
unchanged. Automating this offline decryption in a distributable importer is
still required for final user ergonomics; the owner-test build uses a prepared
private version-2 pack. Never replace the old pack/evidence in place.
