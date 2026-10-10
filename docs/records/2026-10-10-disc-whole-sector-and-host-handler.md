## The disc layer: the drive's whole-sector mode and a ready handler of the runtime's own (2026-10-10)

What was added to `port/src/cd.c`, for a movie's stream; nothing uses
either yet:

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

Why: the logo's program reads its movie with the mode byte 0xe8
(double speed, the compressed audio on, whole sectors, the audio
filter) and with Sony's stream routines as the handler of each sector.
On the PC those routines will be routines of the runtime, not code in
the game's memory, so the game's handler variable cannot name them.

What ran: `python port/tools/test_hostcd.py`, `all cases behaved as
required`, 202 lines that begin `ok` (162 before). Two old cases that
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
and after, `port_CdInit`, the ring guard. The two other control files
that build `cd.c` (`test_hostlaunch.py`, `test_hostaudio.py`) ended as
before, with the same counts of cases.

Mutants in bounded runs by a private script, by kind: 43 runs; 41
failed a named case; 1 ended the build with an error and no named
case (a copy of the old length from the new place, which the compiler
refuses as a warning turned error; the same fault written so that it
builds fails a named case and is among the 41); 1 was the change of
nothing, which passed; none hit the time limit.

Limits, stated: nothing calls either addition yet, so no run of the
game shows them. The bytes of a whole sector are the image's raw
bytes: nothing checks a sector's error codes, as before. What a drive
does with a FIFO that was not emptied when the next sector comes is
the layer's old rule (the next sector replaces it), not looked at
here.
