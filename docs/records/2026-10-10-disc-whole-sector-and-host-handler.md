## The disc layer: the drive's whole-sector mode, a ready handler of the runtime's own, and two routines for its FIFO (2026-10-10)

What was added to `port/src/cd.c`, for a movie's stream; nothing uses
any of it yet:

- The whole-sector mode of the drive (Setmode bit 0x20). The layer
  stopped on it. With the bit, the FIFO of a delivered data sector
  holds the sector's raw bytes 12 to 2351 (2,340 bytes: the 4 of the
  sector's header, the 8 of its subheader, then the rest), and
  `port_CdGetSector` serves successive pieces of that, zeros when it
  has run dry. Without the bit the layer does as before: the 2,048
  bytes from byte 24. The bit is looked at when a sector is delivered.
  The mode bit 0x10 (the form of 2,328 bytes), which nothing uses,
  stops the program at the Setmode. Sectors of the disc's compressed
  audio go their own way as before, in either mode.
- `port_cd_host_ready(handler)`: a ready handler of the runtime's own.
  While one is set, each delivered data sector calls it with (1, the
  result bytes) and a disc error with (5, the result bytes), directly
  and with the handler depth raised, in place of the game's handler
  variable, which is then not called whatever it holds. The rule for a
  polling game (the next sector waits until the last was taken) and
  the ring guard of the game's own stream do not apply while it is
  set. It may call the layer's routines as a game's handler may.
  `port_CdInit` removes it; a null pointer removes it.
- `port_cd_fifo_read(dst, bytes, &sector)` and `port_cd_store(madr,
  src, bytes, sector)`. The first takes the next bytes of the current
  sector's FIFO into memory of the runtime (not the game's), by the
  FIFO's own rules, and says how many were real and which sector they
  are of; it changes nothing of the game's memory. The second puts
  bytes of the runtime's memory into the game's memory as bytes of a
  given sector: the same check of the destination, the same record of
  the source and the same call of the written hook as
  `port_CdGetSector`, with which both now share their code. A stream
  routine that must put a sector aside for a moment (while the movie
  decoder is busy) needs the pair: `port_CdGetSector` rightly refuses
  a destination outside the game's memory, and a later plain copy into
  the game's memory would not be seen by the layer that watches what
  the disc wrote.

Why: the logo's program reads its movie with the mode byte 0xe8
(double speed, the compressed audio on, whole sectors, the audio
filter) and with Sony's stream routines as the handler of each sector.
On the PC those routines will be routines of the runtime, not code in
the game's memory, so the game's handler variable cannot name them.

What ran: `python port/tools/test_hostcd.py`, `all cases behaved as
required`, 224 lines that begin `ok` (162 before). Two old cases that
said a Setmode with the bit stops are replaced by cases for the bit
that still stops (0x10, 0x30, 0xf8) and by cases of the accepted mode:
one sector taken whole, as pieces of 12, 32 and 2,016 bytes and a
rest, a piece past the end, the mode set and cleared between two
sectors of one read, on an image whose raw sectors are filled with a
pattern so that a wrong offset shows; and for the host handler:
called once per data sector in order, the game's handler not called
beside it and called again after removal, an audio sector never
reaching it, a read past the image's end, the bytes taken inside the
call, a pause and a new read from inside, the handler depth inside
and after, `port_CdInit`, the ring guard; and for the two FIFO routines: the read
as one piece and as pieces in both modes, past the end, the sector's
number, no sector in the FIFO, pieces taken in turn by the read and by
`port_CdGetSector` continuing each other, the read leaving the game's
memory, the record of sources and the hook alone (also with a
destination inside the game's memory), the store landing its bytes with
the record and one call of the hook, a sector of -1, a length of 0, and
destinations outside the memory. The two other control files
that build `cd.c` (`test_hostlaunch.py`, `test_hostaudio.py`) ended as
before, with the same counts of cases.

Mutants in bounded runs by a private script, the whole list on the
final test file, by kind: 64 runs; 62 failed a named case; 1 ended the build with an error and no named
case (a copy of the old length from the new place, which the compiler
refuses as a warning turned error; the same fault written so that it
builds fails a named case and is among the 62); 1 was the change of
nothing, which passed; none hit the time limit.

Limits, stated: nothing calls either addition yet, so no run of the
game shows them. The bytes of a whole sector are the image's raw
bytes: nothing checks a sector's error codes, as before. What a drive
does with a FIFO that was not emptied when the next sector comes is
the layer's old rule (the next sector replaces it), not looked at
here.
