## Sony's movie decoder calls for the PC program, on PsyZ's chip core (2026-10-10)

What was added: `port/src/press.c` and `press.h` with the five calls
of Sony's movie decoder library that the logo's program uses
(`DecDCTReset`, `DecDCTvlc`, `DecDCTin`, `DecDCTout`,
`DecDCToutCallback`, as `port_DecDCT...`), and their test
`port/tools/test_hostpress.py`. No program calls them yet.

Whose parts: the decoder chip is PsyZ's (`Psyz_MdecReset`,
`Psyz_MdecCommand`, `Psyz_MdecRead`, in the library since the pin of
2026-10-10, tested upstream against captures from a console). The
bitstream reader and the tables sent at a reset are the port's
(`port_mdec_vlc`, `port_mdec_reset` of `port/src/mdec.c`): the library
has a stand-in where Sony's bitstream decoding would be. The calls
themselves are the port's, small: PsyZ has decompiled C of Sony's
calls, but it registers the output routine in PsyZ's own kernel, which
the port replaces with its own. The port's own decode routine in
`mdec.c` is no longer needed by anything; it stays in this change and
its removal is a later one. This follows the owner's word of
2026-10-10 (borrow what the library has); a first version of these
calls on the port's own decode routine was stopped half way for it and
is not published.

What each call does was read from the logo program's own copy of
Sony's routines (a private listing) and is in `press.h`. The end of an
output transfer is delivered to the game's routine for DMA channel 1
as an interrupt, in the pattern of the sound chip's transfer: pending,
then taken exactly once, by a poll of a library routine that ticks or
by the timer thread's claim, after a due time. The due time is this
project's choice and not the chip's known pace: 55 microseconds a
macroblock, counted from the frame's `DecDCTin`, so that a frame of
320 by 240 is done about 16.5 ms after its input and the timer's
lateness does not add up from strip to strip. A completion must not
come before a chip could have made the data: the logo's program relies
on its routine not running inside `DecDCTout`. The routine's address
goes through the runtime's check of a call target before it is
called.

Added to files of other parts, a few lines each: `port/src/iopage.c`
(one function that returns the routine registered for a DMA channel),
`port/src/kernel.c` (three calls next to the sound transfer's), their
declarations in `port/src/port.h`, three stand-ins in the rig of
`port/tools/test_hostlaunch.py` (it builds `kernel.c` without
`press.c`), the test's name in the review tool's two lists.

A program built without the library has no chip. `hostbuild.py` gives
`press.c` the library's define with `--psyz` (as it does for three
other runtime files); without it the reset, the input and the output
calls stop with a line that names the call, and the program links
nothing of PsyZ. The first form of this change called the chip
unconditionally, and the review command's build of the PC program
without the library did not link; that run found it.

What ran: `python port/tools/test_hostpress.py --cc CC --psyz-build
DIR`, `all 61 cases behaved as required` in about 8 seconds (one-off).
Expected codes come from the driver's own call of the reader, expected
pixels from its own direct calls of the chip after the same reset and
tables; the completion's cases run on a clock the test sets. Mutants
of `press.c` in bounded runs by a private script, on the test as it
was with 59 cases, by kind: 74 in all; 71 failed a named case; 3
passed; none ended the tester without a name, none hit the time
limit. The three that passed: the two quantisation tables swapped at
the reset (the reset gives both the same matrix, so nothing can
show); the taking of a completion made not atomic (a race of two
threads, which a driver with one thread cannot show); the chip's
refusal of a decode command ignored (the checks before it leave no
input that reaches it). The sixtieth case, the check of the routine's
address, and one mutant by hand that takes the check out and fails
that case. The sixty-first builds `press.c` a second time without the
define and without the chip and shows the three stops; the build
tool's test got a case for the define, and with `press.c` taken out of
the tool's list that case fails. The review command was run on the branch's final head
before the pull request; its count of checks is in the pull request's
text.

A correction of the record of the pin's move (the owner's review of
it, not blocking): where that record and that pull request said that
each of the port's twenty control files ended with "the same last
line" before and after, the two files whose last line gives the time
they took differ in the seconds; what was the same is the count of
cases and the outcome. The record's sentence is brought to that here.

Limits, stated: nothing here is compared with a console or with Sony's
library; the comparison is with the chip core and the reader called
directly. Not modelled, and stopped with a line that names the call:
the decoding of a frame in parts (a null bitstream), the monochrome
depths, a count of words that is not a multiple of 32, a read of more
than the frame holds. The retries and time-outs of Sony's waits are
not modelled. Before the first `DecDCTReset(0)` the chip's tables are
whatever the library starts with. The calls not used by the logo's
program are left out (`DecDCTvlcSize`, the input's callback and the
two waits, the environment calls).
