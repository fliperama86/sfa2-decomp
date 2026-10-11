## Sony's movie stream calls for the PC program, on the port's disc layer (2026-10-10)

What was added: `port/src/stream.c` and `stream.h` with the calls of
Sony's library with which the logo's and the opening movie's programs
read a movie from the disc: `port_StSetRing`, `port_StSetStream`,
`port_StGetNext`, `port_StFreeRing`, `port_StUnSetRing`,
`port_StCdInterrupt` and `port_CdRead2`, with their test
`port/tools/test_hoststream.py`; and one routine in `press.c`,
`port_press_out_busy`. No program calls them yet.

Why the port's own: the library the port borrows from (PsyZ, at the
pin of 2026-10-10) has decompiled C of the ring's routines, but a
stand-in for the sector handler and no delivery of single sectors in
its disc emulation. The port's disc layer has the paced delivery, the
ready handler and the disc's compressed audio. So these calls stand on
`cd.c`, through the additions of pull request 197.

What they do was read from the two programs' own copies of Sony's
routines (a private listing; the two copies are the same code): the
ring is the game's memory in Sony's layout (n headers of 32 bytes,
then n blocks of 2,016 bytes; a slot's state in the first halfword of
its header: free, sector received, frame complete, handed to the
reader, wrap marker); the handler's tests come in the library's order
with its effects (a slot that is not free, the mask before the start
frame, the id and the type, a chunk out of order or a changed frame
number, the end frame, no room with and without an end frame and the
wrap to slot 0); `StGetNext` and `StFreeRing` give the library's
results. The library's own variables are the runtime's.

Three things are the PC's own way to the same end:
- The last data transfer of a frame ends at once: the frame's first
  slot becomes complete and the frame routine is called at the end of
  the handler. On the console that is a second interrupt a little
  later.
- The decoder's busy rule. While the movie decoder's output transfer
  is busy, Sony's handler leaves the sector in the drive's FIFO, sets
  a flag, and the program's own output routine calls the handler again
  when it sees the flag. On the console that costs at most one strip's
  time and no sector, because sectors come 6.67 ms apart. The port's
  drive hands over two or three sectors in a row at a vblank, so a
  sector left in the FIFO would be lost to the next. Here the handler
  takes such a sector into a queue of the runtime (16 sectors; a
  seventeenth stops the program) and sets the flag; whenever it runs
  with the decoder free, from the disc layer or from the program's
  call, it first works off the queue in order. The drive never stalls,
  order is kept, nothing is lost. The flag is a word of the program's
  own memory that the program's C reads and clears, so the layer is
  told its address (`port_stream_bind`); with none bound, a sector
  that would be put aside stops the program.
- Stops in place of what is not modelled, each with a line that names
  the call: Sony's "old type" stream (the whole-sector bit clear), a
  ring of fewer than two slots or outside the RAM, pointers outside
  the RAM, a call of the handler from inside itself other than from
  the frame routine.

What ran: `python port/tools/test_hoststream.py --cc CC`, `all 61
cases behaved as required` in about 7 seconds (one-off), on a disc
image the test makes (a made-up stream with sectors of compressed
audio between and sectors that break each rule), the ring's bytes
against the image's and the states and results against the header's
text; `python port/tools/test_hostpress.py --cc CC --psyz-build DIR`,
`all 66 cases behaved as required` (2 new for the busy state). The PC
program built with the library and without it (`linked: ...,
verified` both times), and one start without a window for 60 seconds
ended by the time limit with no stop line. Mutants of `stream.c` in
bounded runs by a private script, on the final test file, by kind:
248; 230 failed a named case; 18 passed; none ended the tester
without a name, none hit the time limit. Of the 18, 14 change
nothing an input can show (a check that an earlier check already
made, a position or a variable that nothing reads back, a cast that
hides the change); 2 concern the state after a stall with no end
frame, which never ends and so shows nothing; 1 is covered by another
check on the same input; and 1 is left open on purpose: what
`StFreeRing` does with an address inside a data block is not pinned
(the programs pass a block's start). One step was brought to the
listing's letter after the worker's run (the mask is cleared only
with a start frame); no case could tell the difference, and the 61
cases pass with it.

A correction of the header of the small library routines (the owner's
review of that change, not blocking): `progcalls.h` said that a buffer
a call writes is checked to lie in the mapped RAM; that holds for
`CdRead`'s sector buffer, and `CdReadSync`'s eight result bytes go to
the pointer it is given without that check. The header says so now.

Limits, stated: nothing is compared with a console; the comparison is
with the made-up image and with the header's text, which follows the
reading of the listing. The handler's two ways in (from the disc
layer's rows and one sector at a time through the test's own handler)
are compared with each other in one case; most cases use the second.
Re-entry from two interrupt sources at once, which the listing leaves
open, cannot happen here: both ways run on the game's thread.
