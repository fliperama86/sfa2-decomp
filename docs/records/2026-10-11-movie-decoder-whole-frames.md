## The movie decoder gives whole frames to the logo's program (2026-10-11)

What was wrong: in the PC program the logo's program did not get all of
many of its frames out of the movie decoder. Counted in one start of 150
seconds on this change's first commit `aaf2a5c`, which only adds the
counting: `decoder: program logo: 196 frames, 3709 transfers, 3667
completions taken, 42 dropped by a reset`. A frame of the logo's is 20
output transfers; 42 times the program's next `DecDCTReset` came while a
transfer of the frame was pending, and 211 of the 3,920 transfers of
those 196 frames were never made. (What that looks like in the window was
not seen.)

Two causes, both in `port/src/press.c`, both a matter of time on a PC:

- The program starts a frame's first transfer, calls `DecDCTvlc` for the
  next frame and then waits while a flag is not 0, which the transfers'
  routine sets to 1 when it starts a further transfer and to 0 after the
  last. On the console the call lasts long enough for the first
  transfer's routine to have run (inferred from the sizes; not measured
  on a console). On a PC it lasts microseconds: the flag was still 0, the
  program did not wait, and the frame had only the time until the next
  `DecDCTReset`.
- One completion was taken per poll or per round of the timer. The poll
  comes after a sleep of up to 2 ms and the timer's round waits 1 ms,
  while the transfers of a frame are due 0.8 ms apart, so 20 transfers
  were not done within a frame's time.

What changed:

- A call that takes a completion goes on to the next one when the
  routine has started a transfer whose due time has passed too, and so
  on until a due time is still ahead or the routine starts nothing. No
  completion is taken before its due time. The state, the due time, the
  clock and the registered routine are read again after every call of
  the routine.
- `port_DecDCTvlc`, once its output is written, ticks until one
  completion has been taken, when one is pending and an interruption is
  allowed. It does not wait for the frame's other transfers, nor where
  no interruption is allowed, nor when its result is -1. If none comes
  in 100,000 ticks the program stops with a line that names the call.
- When a built-in program has returned, the output has one line of the
  decoder's counts, and the counts begin again (`port_press_report`,
  called by `programs.c`).

Not changed: `interrupt.c` and the tick of `kernel.c`. The owner had been
asked whether the timer's round might be changed for this, with the
sound measured before and after, and said to change it now; reading the
code again showed that the two changes above are enough, so the timer
and the vblank's clock are as they were. Nothing under `ps1/` changed.

What was measured: 14 starts with the user's disc, without a window, with
`--skip-programs --audio-dump FILE`, one at a time and never beside
another game program, each ended by its time limit (status 124), none
with a line beginning `stop` or `refused`. Seven of a build of `aaf2a5c`
("before"), seven of a build of `804ebf7`, the change merged with main
`293de95` ("after"): one of 150 seconds, two of 16 and four of 7 of each.
The page ("Sony's calls") has the lines. In short:
- the logo, starts that reached its end: before 196, 198 and 194 frames
  with 42, 47 and 56 completions dropped by a reset; after 195, 198 and
  198 frames, 20 transfers for each and none dropped;
- the opening movie, twice in a start of 150 seconds: before 16,399 and
  16,391 transfers for 820 frames, after 16,400 both times;
- the sound in the dumps, by a private script: the first sample that is
  not 0 in vblank 42 to 49 of the seven dumps before and 42 to 50 of the
  seven after; from that sample on every dump equals the first one sample
  for sample for 9.26 seconds or more, or to within 0.03 seconds of its
  end when it is shorter (the logo's sound lasts about 9.28 seconds).

The controls. `python port/tools/test_hostpress.py --cc CC --psyz-build
DIR`: `all 90 cases behaved as required` (66 before this change; one of
the 66 was replaced, since it required that a poll takes one completion
only). `python port/tools/test_hostprograms.py --cc CC`: `all 72 cases
behaved as required` (one new: the line of counts is asked for once,
after everything else, with the program's name, and not after a failed
check).

Found by the review tool's first run: the rig of `test_hostlaunch.py`,
which other test files use too, links `programs.c` without `press.c`, with stand-ins
for the decoder's calls, and had none for the new one, so the first of the
tool's checks (`test_hostaudio.py`) did not link. The rig has the stand-in
now, a routine that does nothing.

Seen and not explained: now and then a run of `test_hostprograms.py` has
one case whose driver ends with status 1 and no line of output, ten
seconds late (`FAIL NAME: status 1, lines []`, the run 14 seconds long
and not 4). Here that was 4 of the first 16 runs, a different case each
time (the first one's name was not kept; the others were
`ram_body_first_difference_is_named`,
`decoder_report_once_with_the_programs_name_after_everything_else` and
`disc_changed_file_stops_before_the_header_is_compared`), and none of 48
runs after them, which were of a copy of the test that would have kept
the driver's error output. Status 1 is not one that the driver's own code
gives (0, 2, 3, 64, or the status of a stop, with its line), which points
at the start of the Windows program from the Linux side; that is not
confirmed. The mutant that ran into the time limit above may be the same
thing. Whether main has it was not tried.

Changes made on purpose, by a private script: 53 of `press.c` and
`programs.c`, one at a time on a scratch copy of `port/src`, each run of
a test under a time limit of 150 seconds and a memory limit. In the last
run of all 53: 52 failed a named case; 1 (the line of counts asked for
with the first row's name instead of the program's) ran into the time
limit with no case named. That one, run alone three more times, failed
the named case `decoder_report_once_with_the_programs_name_after_
everything_else` each time. A first run of 46 had left 5 that no case
noticed (a call that takes three completions and no more; a claimed
completion taken by the call; the wait's limit ten times as long; the
wait's count set to 0 by the line of counts; frames counted at the first
transfer) and 4 that did not build; a case was written for each of the 5,
and the 4 were repaired.

Not right, or not known:

- A start does not always hand every frame of the logo to the decoder,
  with or without this change (198 would be every one, by the program's
  C). This change does not look at why.
- The time of `DecDCTvlc` is modelled only as far as "past one transfer
  that was pending". A program that relied on more of the call's length
  would not get it.
- The window's own picture was not seen: every start here had no
  window. Nobody listened. Nothing was compared with a console.
