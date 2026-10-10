## The disc's streamed audio (2026-10-10)

- What: the sectors of compressed audio that the game streams from the
  disc are decoded, resampled to 44,100 Hz, queued one second deep and
  offered to PsyZ's model of the chip through `Psyz_CdPullSamples`, which
  the port now defines. Before, `cd.c` consumed them and dropped them.
  They are played only when something takes sound out (`--audio`,
  `--audio-dump`); otherwise they are consumed in silence as before.
- Changed: `port/src/xa.c` and `xa.h` (new, the pure decoder and
  resampler), `cd.c` and `cd.h` (the filter, the queue, the hold, the
  volumes of `CdMix`, the lock calls, one line on the output at each
  stream's start), `iopage.c` (`Psyz_CdPullSamples`, the lock helpers and
  the sink), `port/tools/test_hostxa.py` (new), `test_hostcd.py` and
  `test_hostaudio.py` (cases), `test_hostlaunch.py` (its list of runtime
  files gets `xa`), `port/README.md`. `kernel.c`, `interrupt.c`,
  `mirror.c`, PsyZ, `review.py` and everything under `ps1/` are
  unchanged: `review.py` hands every `test_*.py` of `port/tools` the
  arguments it takes, and `test_hostxa.py` and `test_hostcd.py` need none.
- The hold and the carry. A played sector that does not fit the queue is
  not consumed: `deliver_one` returns 0 before the position moves, so the
  row ends and the carry is cut to its remainder like any row that ends
  for want of a sector. The cost of the sector was taken before the
  handler would have run and is not given back. `cd.c`'s header says so.
  The disc's controls (rate, carry, pause, new read) pass unchanged.
- Where the decoder differs. From the private trial's decoder
  (`ed1096e`): the rounding is written as a floor division and not as a
  shift of a negative value (the same values); the header and the page
  name the document and say what is this project's reading. The brief of
  this change said that the trial's decoder had no `+ 32`; the trial's
  `xa.c` at `ed1096e` has it, so no change of values follows. From
  PsyZ's (`libcd.c`): its history is the unclamped value, its division
  truncates toward zero, and it decodes 18,900 Hz and 8-bit sectors as
  4-bit at 37,800 Hz; its resampler is a 4-point Hermite. Its volume
  matrix is the same sum, and the same passthrough at 127 or 128.
- Controls (2026-10-10, all printed by the commands):
  `test_hostxa.py` 54 cases (`54 cases, 0.4 seconds`, `all 54 cases
  behaved as required`); `test_hostcd.py` 88 cases before, 123 after;
  `test_hostaudio.py` 14 before, 19 after (group `stream`: three volume
  settings through the dump, equal frame for frame with a tolerance of 0,
  the no-sink case, and the link); `test_hostiopage.py` 110 cases,
  `test_hostlaunch.py` 68, `test_hostmodules.py` 55 and
  `test_hostmirror.py` 103, all passing (counts after; before counts were
  not taken). The first run of `test_hostlaunch.py` had 67 passing and 1
  failing trace case, because the layer printed an exit line of counts
  into every trace; it now prints it only when an audio sector was met,
  and the file passes.
- The tolerance of the dump case is 0. The made-up game sets the chip's
  CD input on, both CD volumes and the main volume to values that make
  the mixer's sums integers that the control computes from
  `psyz_spu.c`'s formula, `(x * 0x7fff) >> 15` and then `* 0x3fff >> 14`.
  A different setting could need a tolerance of a step or two.
- Negative controls of the decoder (a copy of `xa.c` with one change, run
  with `--xa FILE` under `timeout`):

  | change | cases failed | first case that failed |
  | --- | --- | --- |
  | a coefficient (filter 2: 115 -> 114) | 15 | filter 2 at range 0, stereo |
  | a coefficient (filter 3 second: -55 -> -54) | 14 | filter 3 at range 0, stereo |
  | the + 32 taken out | 26 | filter 1 at range 0, stereo |
  | rounding: a division that truncates | 25 | filter 1 at range 0, stereo |
  | the clamp: upper limit missing | 11 | filter 1 at range 0, stereo |
  | the clamp: lower limit missing | 10 | filter 1 at range 0, stereo |
  | the history kept unclamped | 24 | filter 1 at range 0, stereo |
  | a range above 12 taken as 12 instead of 9 | 4 | range 13 (above 12), filter 3 |
  | a range above 12 not capped | 4 | range 13 (above 12), filter 3 |
  | the subheader copies not compared | 5 | the subheader's two copies differ |
  | 8 bit not refused | 1 | an 8-bit sector is refused, history untouched |
  | the history not carried across sectors | 9 | clamp at both ends, filter 3, up then down |
  | the nibble order swapped | 29 | filter 0 at range 0, stereo |
  | mono put on the left only | 4 | mono 37800 Hz, headers of every kind |
  | the resampler's truncation (round down instead) | 15 | resample 37800 Hz, 9000 frames in calls of 1 |
  | the resampler's phase not carried | 8 | resample 37800 Hz, 9000 frames in calls of 1 |

- Negative controls of the disc layer (a copy of the runtime's folder
  with one change in `cd.c`, `test_hostcd.py --src DIR`, under
  `timeout`):

  | change | cases failed | first case that failed |
  | --- | --- | --- |
  | the hold removed (a sector is queued whatever the room) | 4 | the queue is full |
  | an empty queue no longer always takes a sector | 0 | - |
  | the hold does not stop the row (returns 1, sector lost) | 3 | the read position stays at the held sector however many ticks pass |
  | the filter bit ignored | 3 | filter on, file differs |
  | only the file compared, not the channel | 1 | filter on, channel differs |
  | only the channel compared, not the file | 1 | filter on, file differs |
  | Setfilter bytes swapped | 26 | filter on, file and channel equal |
  | no reset at a read's start | 3 | the volumes stay after a new read |
  | the decoder alone not reset at a read's start | 2 | the volumes stay after a new read |
  | the queue alone not emptied at a read's start | 3 | the volumes stay after a new read |
  | the no-sink rule removed (queues without a consumer) | 2 | no sink |
  | an 8-bit or bad sector not consumed (held back) | 2 | an 8-bit sector and a sector with differing subheader copies are refused and counted; the others ... |
  | refusals not counted | 1 | an 8-bit sector and a sector with differing subheader copies are refused and counted; the others ... |
  | volumes: the two cross terms swapped | 1 | CdMix mixed 100, 30, 90, 20 |
  | volumes: the two straight terms swapped | 1 | CdMix mixed 100, 30, 90, 20 |
  | volumes: no clamp | 1 | CdMix twice 255, 0, 255, 0 (clamped) |
  | volumes: 127 not taken as unity | 1 | CdMix 127, 0, 127, 0 goes through unchanged |
  | CdMix ignores its volumes | 6 | CdMix half 64, 0, 64, 0 |
  | the lock not taken to add to the queue | 3 | the lock calls are balanced (depth 0, none unlocked without a lock), and a pull takes no lock |
  | the lock not released after the room check | 1 | a held sector leaves the lock released (depth 0, nothing unlocked without a lock), after 40 and a... |
  | the pull takes the lock itself (PsyZ already holds it) | 1 | the lock calls are balanced (depth 0, none unlocked without a lock), and a pull takes no lock |
  | a played sector also goes to the handler | 8 | with XA on, audio sectors are consumed (they use the tick's budget) and never reach the handler |

  One row has no failing case on purpose: that an empty queue always takes
  a sector cannot decide anything, because no sector makes more than a
  small part of the queue; the mutant is equivalent.
- Real sectors, one-off and private: the first 300 audio sectors of the
  stream at sector 109855 (file 1, channel 7), read in place from the
  image and held in memory, decoded by the published C and by the
  reference of `test_hostxa.py`: `every sample equal`, 1,209,600 samples.
  Without the `+ 32` 602,840 of 604,800 frames differ.
- One-off starts with the disc are in `port/README.md`, "What runs, and
  what is not there". The build lines, from the commands: the default
  build with `--psyz` and the build with `--sound-library --psyz` each
  printed `linked: ..., verified` (12,686 and 12,850 functions with C),
  and a build without PsyZ linked too.
- Not here: the console's resampling filter, 8-bit sectors, a check
  against hardware, the loss of the queued frames when a new read starts
  (by the rule), and the known fault of the lapped ring.
- After review: a consumer that is gone. A failing dump (`wav_stop`)
  left the sink on, the queue filled and a held audio sector stopped the
  data sectors behind it for ever. Now `wav_stop` switches the sink off
  and empties the queue, and the layer counts pulls: a sector held for
  120 vblanks with no pull in that time takes the sink as gone (a line,
  the queue emptied, audio consumed in silence), and a later pull puts it
  on again with the decoder reset (a line). The hold while pulls happen is
  unchanged and unbounded. Cases in `test_hostcd.py` (a switched-off sink
  in a hold; no cut-off after 100 vblanks; cut-off at the 121st attempt
  with held 120, played 18, silent 57; no cut-off in 250 vblanks with
  pulls; revival with the frames equal to a fresh decode) and one in
  `test_hostaudio.py` (a failing write with a stream playing: played 7,
  held 0, silent 33). Negative controls, each under `timeout` on a copy:
  the queue not emptied, the sink left on in `set_sink(0)` and in
  `wav_stop`, the 120-vblank rule removed, the rule cutting off while pulls
  happen, 60 vblanks instead of 120, no revival, no decoder reset: each
  fails the case that names it (1 to 3 cases).
