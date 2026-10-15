# Native intro state and scene controllers

## Scope and status

`world.c`, `director.c` and the actor units are native C reconstruction work in
progress. They do not yet replace the first-scene runtime in `port/src/intro.c`.
All eight scene controllers, primary kinds 01 and 80 through 8e, secondary
kinds 20 through 22, and UI kind 8 are implemented for the observed supplied
profile. Native map consumers, palette fades, title lettering and display-list
assembly are present. Full resource packaging, host scheduling and sound remain
unfinished. This is not yet a complete-intro owner-test build.

## State, resources and dependencies

`intro_world` owns 64 KiB of game state and 256 KiB of video data. Big-endian
word/long accessors preserve fields that the original code aliases at byte,
word and long widths. They do not represent CPU registers or executable state.
There is no instruction decoder, original-code interpreter or recorded-frame
playback. The position-history buffers are populated by native actor movement
and consumed by native trails during the same run.

The resource callback distinguishes indirect data reads from inline data
operands. The finished importer must whitelist actual data ranges and reject
missing/invalid ranges. It must not package executable regions merely because
some data tables share those address ranges. The private original-program
oracle is development-only and never part of the native executable.

The director explicitly calls actor update, presentation and title-lettering
dependencies. Missing dependencies stop with an error. Actor update order is
ascending slot address, with allocation from the original LIFO free lists.
Links are validated against their pool and stride before native access. The
primary, secondary and UI pools are distinct. Allocation exhaustion preserves
the original branch behavior on reachable intro paths; malformed free lists
are rejected rather than dereferenced.

`intro_director_step` returns -1 for an error, 0 while the controller is active,
and 1 after the exit flag. It is a controller tick, not a claim about scheduler
or host display cadence. Cooperative yields and host fade/audio scheduling
still need integration. Palette bank shadows, timer underflow, fractional
coordinates, ordered group allocation and aliased completion signals remain
explicit. The strip-exit substate intentionally ticks and queues twice; the
headshot controller's initial completion low word is two, not zero. The
original long write also retains an unused caller-register upper word; native
C uses zero there instead of preserving CPU register residue. That is a stated
nonmatching projection until the actors replace those bytes with completion
markers, not full raw-state identity. The strip
scene's initial scroll coordinates are one, the map-clear helper's returned
scalar, not visually guessed zeros.

Supported actor input is initialized intro state with the subtype/state and
resource graph used by the supplied variant. The functions are not a general
CPS2 runtime. Unsupported kinds and malformed state fail closed. Secondary
actors are attached effects, not independently collected primary sprites.
The native renderer traverses their parent links in original order.

## Evidence boundary

The 53 fixtures in `native-fixtures.json` use project-constructed state and
synthetic zero data, with results computed by the isolated original routines.
They contain no extracted artwork, palette tables, opcode bytes or captured
frames. The private recording run verified that their only resource reads
were the zero-valued camera origin. The fixture baseline is the original's
computed actor-pool initialization for a zeroed input state.

`python port/tools/test_intro_native.py` runs these fixtures with undefined-
behavior sanitization and four actual source mutants, without private inputs.
It covers representative controller transitions, actor movement, word/byte
aliases, signed bounds, history writes and secondary timing. It is not an
exhaustive rejection test or a complete-intro proof.

Private construction comparisons also exercise all controller phases and the
implemented actor families on two seeds. They compare game RAM (excluding the
original call stack and scheduler task storage) and, for director/montage/
secondary runs, video memory. See the change record for exact scope and the
preserved failed attempts. These do not replace the remaining dependency,
clock, presentation and sound work.

## Additional reconstruction evidence

The [second reconstruction record](../../docs/records/2026-10-14-intro-native-rendering.md)
covers all-phase controller/actor/fade/UI traversal, native display lists and
isolated map/fade tests. The complete state traversal is not a game boot, a
proof of real-time audio synchronization, or final owner acceptance. Startup
still uses oracle-prepared dependency state in that comparison.

`fade.c` supports only the three intro-used tasks: brightness down, brightness
up and RGB-to-white. Bank-wide brightness changes are gated by the first color;
the task completes on the later invocation that observes its target. Timing
and completion flags preserve the scoped scheduler shim, not a new claim of
console display timing. `maps.c` parses bounded tile-pair rectangles with the
original plane-specific row and column wraps. `text.c` parses bounded strings,
with explicit UI slot capacity (47 blocks after the in-slot header).

The supplied logo profile implements regional variants 1 and 4, not other
region choices. Unsupported kinds, formats and state transitions remain errors.
