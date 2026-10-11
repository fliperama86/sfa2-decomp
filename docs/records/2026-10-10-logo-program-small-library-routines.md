## The logo program's small library routines for the PC program (2026-10-10)

What was added: `port/src/progcalls.c` and `progcalls.h` with six
routines of Sony's library that the logo's program calls and the
runtime did not have, apart from the decoder's and the stream's:
`port_CdRead` and `port_CdReadSync` (the plain read of sectors into
memory, for the file of the stills), `port_PadInit`, `port_PadRead`
and `port_PadStop` (Sony's simple pad calls), and
`port_SetVideoMode`; with their test
`port/tools/test_hostprogcalls.py`. No program calls them yet.

What each does:
- `CdRead(sectors, buf, mode)` starts a read of that many sectors of
  2,048 bytes into the game's memory, on the disc layer's host ready
  handler (pull request 197): a Pause first if the drive is reading,
  the Setmode with the mode's low byte, the read; each delivered sector
  goes to the buffer, and after the last the handler removes itself
  and pauses the drive. It returns 1 when the read was started. The
  buffer's whole span is checked to lie in the mapped RAM; another
  sector form than 2,048 bytes stops the program with a line.
- `CdReadSync(mode, result)`: with mode 0 it waits, ticking, until the
  read is done (0) or has failed (-1); with another mode it returns at
  once the sectors still to come. The result bytes are those of the
  last sector's interrupt.
- `PadRead` gives the word Sony's routine gives, a pressed button as a
  1 bit, for the port's pad state at this moment: port 1 from the
  runtime's pad frame with the input script applied (the two routines
  the vblank's filling of the game's buffers uses), port 2 as no
  controller. Start is bit 11, circle bit 5, as the program's main
  expects. `PadInit` and `PadStop` keep a flag only: the runtime's pads
  are started already.
- `SetVideoMode(mode)` keeps the word and returns the one before. The
  port's display is not changed by it.

What Sony's routines do was read from the logo program's own copy (a
private listing): its `PadRead` returns the complement of the BIOS's
word of both pads; the routine the private table of that program had
named as a callback setter is `SetVideoMode`.

What ran: `python port/tools/test_hostprogcalls.py --cc CC`, `all 45
cases behaved as required` in about 5 seconds (one-off): the read on a
made-up disc image through a driver that wraps what the file asks of
the disc layer (the buffer's bytes against the image's, for one sector
and for several; the commands in order; the handler removed and the
drive paused after the last; `CdReadSync` in both modes before, during
and after; a read past the image's end; a second read; each stop), the
pads (each button bit from a made-up frame, no controller, the input
script's press, the two calls that change nothing), `SetVideoMode`.
The PC program built with the library and without it (`linked: ...,
verified` both times; the file needs nothing of the library), and one
start without a window for 60 seconds ended by the time limit with no
stop line. Mutants in bounded runs by a private script, on the final
test file, by kind: 83; 80 failed a named case; 3 passed; none ended
the tester without a name, none hit the time limit. The three that
passed change nothing an input can show: the done mark set before the
Pause (nothing looks between the two); the pads' flag (nothing reads
it); the input script applied before the check of the pad's kind (the
script changes buttons, not the kind).

Limits, stated: nothing is compared with a console. Sony's retries of
a read (four attempts, a budget of eight restarts, a restart after 60
vblanks without a sector) are not modelled: a disc error ends the read
as failed. Sony's `CdRead` sends the last position again; this one
starts where the drive is, which is the same after a seek (the logo
seeks before every read) and differs for a read that follows a read
without a new seek. `PadRead`'s upper half (the second pad) is always
0. `PadInit`'s result is 1; the program does not read it.
