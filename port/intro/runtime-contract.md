# Complete intro: player, resources and sound

This is a port-only, tested-nonmatching native implementation. It does not
alter the PS1 decompilation, execute original instructions, replay recorded
frames/state, or implement the rest of arcade attract mode. The examined
profile is the supplied US 960306 archive with the recovered Zero 2 intro
variant. Unknown revisions fail rather than borrowing nearby evidence.

## Native lifetime and display boundary

`intro_player` owns its game-data RAM, video memory and object presentation.
Initialization clears them, establishes the explicit display-register shadows,
white/black flash banks and blank object list, binds native callbacks and sets
the caller's RNG seed. No original RAM snapshot initializes the player.
Import and normal playback both use seed `0x1234`; alternate seeds are research
controls, not a promise that the sparse default-seed pack covers every seed.

One update runs the recovered director and actor work, publishes pending
clears, then advances palette tasks and RNG unless the director has exited.
The presentation callback consumes its preceding busy flag at the display
boundary. This substitutes a deterministic native boundary for the original
cooperative scheduler and interrupt machinery. The host advances one logical
update per normal 60 Hz frame. Exact arcade cycle timing is not claimed.
The [director](director-contract.md) and
[presentation](presentation-contract.md) contracts state the scoped semantic
projection and unsupported paths. Missing data or unsupported behavior fails
by name; there is no original-code fallback.

With the experimental flag off, configuration frees any pack and the original
PS1 movie route is unchanged. With it on, only the `z2o` program is replaced.
Each invocation constructs a new player, restarts the cached soundtrack,
switches the existing GPU backend to the intro canvas, and restores that
backend on completion or Start skip. The loop has a 6,000-update failure bound,
not the old first-scene duration. The observed complete run takes 2,948 updates.
Repeat/skip glue has synthetic controls; the real executable observation covers
one uninterrupted completion and return, not a live repeated/skip session.

## Sparse resource file, version 3

The file begins with eight bytes `SFA2I003`, then SHA-256 of the remaining
payload. Legacy slice versions are rejected explicitly. All integer fields in
the payload are big-endian:

- Two 32-bit counts, data spans then graphics tiles.
- Each 16-byte span record: plane, address, length, payload-relative offset.
- Each 12-byte tile record: key, length, payload-relative offset.
- Bodies follow the tables contiguously in record order, without padding or
  trailing bytes.

Planes 0 and 1 distinguish original data-bus values from inline constants
recovered from the decrypted carrier. They are data lookups, not an opcode
decoder or instruction-dispatch interface. Spans are sorted, nonempty and
nonoverlapping within the 3 MiB address domain. There are at most 32,768 spans.

A tile key is `(side << 24) | (code << 1) | half`. Supported sides are 8, 16
and 32, with `half` permitted only for 8-pixel tiles. Keys strictly increase;
each tile has exactly `side * side` byte-valued pens in 0..15, within the
examined 20 MiB graphics-address domain. There are at most 65,536 tiles.
Payload size is bounded at 64 MiB. Offsets, arithmetic bounds, pen values and
complete file consumption are checked before publication. Reads use binary
search and return null on missing resources. The caller owns the immutable
payload for the entire resource object's lifetime.

The importer pins archive, data-plane, decrypted-plane and graphics digests.
Its import-only [BSD-licensed cipher helper](tools/cps2crypt/README.md) uses the
key from the user's archive. It is not linked into the game. Native C walks
the full sequence to collect requested data ranges and tiles, then walks it
again with only the sparse pack and compares every rendered frame hash.
These hashes are import evidence, not runtime animation input. No original
CPU executes in the importer. Full carrier images do not enter the pack.
Outputs stay private, and existing outputs/reports are never overwritten.

## PS1 soundtrack adaptation

Sound deliberately reuses the supplied Japan PS1 disc's opening XA recording,
not Saturn audio, emulator-captured audio or a new QSound implementation.
The real disc finder takes the basename `Z2O.STR`, locating it under `PAC`.
Its expected logical length is 16,896,000 bytes. Read 956 raw 2,352-byte sectors
at file-relative offsets `558 + 8 * i`, with a separate read-only file handle
so the active drive cursor is untouched. The gathered stream's SHA-256 is
`f952bd2ca0b1b5bfda31605ab80dd58c4b92120b3c836e4042f4f88a8b67d41c`.
This skips the preceding Capcom-logo section. A mismatch fails explicitly.

The existing native XA decoder must report file/channel 1, stereo, 37,800 Hz,
4-bit samples and 2,016 stereo frames per sector. Its existing resampler
produces 2,248,512 stereo frames at 44,100 Hz, cached once for this process's
disc. Decode failure leaves the preceding audio object unchanged. Pulls are
bounded by remaining samples, with signed sample scaling at gain 0..65,536.

The opening cue starts the stream through the normal audio backend after the
same CD/SPU initialization used by the replaced movie. A callback override is
installed, adjusted and removed under the existing audio lock. It defaults to
null; ordinary XA pull/queue behavior is untouched in that state. The final
native visual fade scales music gain and completion/skip removes the override.
No game audio queue is replaced by a separate executable or playback process.

The original finale queue command is not established as a stop request. The
director retains its opaque event; the host does not truncate music there.
The PS1 recording already includes its title cue. The source stream lasts
about 50.99 seconds, while this native presentation lasts about 49.13 seconds;
the native exit fade and completion control its cutoff. This adaptation is
not sample-exact arcade audio or proof of exact cross-platform audiovisual
synchronization. Owner listening/acceptance is still pending.

## Evidence boundary

See the [runtime record](../../docs/records/2026-10-14-intro-full-runtime.md).
Original-routine comparisons are offline construction evidence with explicit
scheduler/peripheral substitutions. Full-input versus sparse-pack comparison
establishes resource closure, not independent original-image equivalence.
Runtime capture comparison establishes native frame upload, not a full arcade
recording comparison. Audio was dumped silently and inspected numerically,
not listened to. None of these claims whole-game completion or owner approval.
