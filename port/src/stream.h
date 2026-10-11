/* Sony's movie stream calls, as the PC program has them: the routines with which the logo's and the opening movie's programs take a
 * movie's sectors from the disc into a ring of frames (StSetRing, StSetStream, StGetNext, StFreeRing, StUnSetRing, StCdInterrupt,
 * CdRead2), on top of the port's disc layer (cd.c) and the decoder calls' busy rule (press.c). Nothing binds them to a program yet; a
 * later change does. Written from what the programs' own copies of the library do, as a private note of the project states it;
 * nothing here is compared with a console's library or with its output. What stream.c adds is tested in port/tools/test_hoststream.py
 * by what the calls do on a made-up disc image, against values worked out from that image.
 *
 * THE RING is the GAME's memory in Sony's layout: n headers of 32 bytes, then n blocks of 2,016 bytes. A slot's state is the
 * halfword at the start of its header: 0 free, 3 sector received, 2 frame complete (on the frame's first slot only), 4 handed to the
 * reader, 1 wrap marker. A frame's slots are consecutive and the last slot of the ring is never used as a frame's first. The header's
 * fields are halfwords at: +0 the id (0x160), +2 the type (bits 10 to 14), +4 the chunk number, +6 the chunks of the frame, +8 the
 * frame number. Every read and write of the ring is checked to lie inside the mapped RAM; a ring outside it, or fewer than 2 slots,
 * stops the program with a line that names the call (port_halt, status PORT_EXIT_OTHER), as press.c does.
 *
 * THE LIBRARY'S OWN VARIABLES (w, s, r, mask, start, end, curframe, nextchunk, mode, f1, f2, dmabusy) are variables of stream.c.
 * The type tested is always 0. Header fields are taken as halfwords. The frame number is compared with `start` and `end` as the
 * halfword it is.
 *
 * port_StCdInterrupt() is the sector handler. The disc layer calls it for each delivered data sector (port_cd_host_ready, set by
 * CdRead2 with bit 0x100); the program's own C may call it too, as the logo's output routine does when it sees the deferred flag.
 * Its steps, in the order of the library: (1) dmabusy set: return; (2) the busy rule below; (3) CdReady(1): a code other than 1, or
 * a first result byte with bit 2 (value 4), ends it; (4) the slot w is not free: the sector is not taken; (5) 12 bytes from the sector's FIFO
 * (the first 4 kept, the next 8 dropped), the 32 header bytes into the slot, the 4 kept bytes over header bytes 28 to 31; (6) with
 * mask 1 and start not 0 a frame number other than start frees the slot, else mask becomes 0; (7) an id other than 0x160 or a type
 * other than 0 frees the slot; (8) a chunk number other than nextchunk, or a frame number other than curframe when that is not 0,
 * drops the partial frame (slots s to w, the sector's own included, freed; w = s; curframe = nextchunk = 0); (9) at chunk 0 the
 * frame number becomes curframe; with end not 0 and curframe at or past it the partial state is cleared, mask becomes 1, f2 is called
 * and the sector is dropped; with no room ((n - w - 1) below the frame's chunks) and end 0 the slot gets state 1, mask becomes 1, f2
 * is called (a stall); with end not 0 slot 0 is used when it is free (the old slot gets state 1, the header is copied to slot 0, w =
 * 0) and the sector is dropped when it is not; then s = w; (10) nextchunk is raised, the 2,016 bytes go into data block w, the slot
 * gets state 3, w is raised; at the frame's last chunk the library's transfer-end routine is run at once, at the end of the handler:
 * state 2 on slot s, s = w, f1 called when set; dmabusy is never left set. The 12 bytes of step 5 go to a local buffer (on the
 * console they never were in the game's memory either); the 32 and 2,016 bytes come from the disc layer's FIFO routine
 * (port_CdGetSector) straight into the ring; the 4 kept ones are written with port_cd_store. A callback (f1, f2) is called through
 * the runtime's check of a call target.
 * Readings of the library's text that left a case open, as taken here: in step 8 and step 9's end-frame case the dropped sector's own
 * slot is also set to state 0 (otherwise the slot would keep the header's id and the ring would stall); the frame number held in
 * curframe is not cleared by a stall or a refused wrap; StGetNext follows a wrap marker and then looks at slot 0 in the same call; the
 * mask is cleared at step 6 also when start is 0 (it cannot be seen).
 *
 * THE BUSY RULE. With StSetStream's mode bit 0 set and the decoder's output transfer busy (port_press_out_busy), the library leaves a
 * sector in the drive's FIFO and sets `deferred`, to be worked on when the program's output routine calls StCdInterrupt again. The
 * port's drive hands over the sectors of a vblank in a row and would lose that one, so here the handler takes the sector's 2,340
 * bytes from the FIFO into a queue of the runtime (16 sectors, each with its sector's number; a seventeenth stops the program with a
 * line), writes 1 in the program's deferred flag (a word of the game's memory: port_stream_bind), and returns. Whenever the handler
 * runs with the decoder not busy, from the disc layer or from the program's call, it first works off the queue in order, each
 * entry through steps 4 to 10 with its own bytes (its first 12 as the hand-read bytes, the next 32 into the header and the next
 * 2,016 into the data block, by port_cd_store with the entry's sector number), then the sector at hand if the disc layer brought
 * one. A program's call with nothing queued and no sector at hand does nothing; a busy call with no sector at hand queues nothing and
 * writes no flag. The program's own C clears the flag. With no address bound, a sector that would be queued stops the program.
 * The queue is also emptied by StSetRing and StUnSetRing (this layer's own rules; the library has no queue); CdRead2 does not.
 *
 * NOT MODELLED, each a stop with a line that names the call: the "old type" stream (CdRead2 with bit 0x100 and without the mode's
 * 0x20 bit); a ring of fewer than 2 slots or outside the RAM; StGetNext's and StFreeRing's pointers outside the RAM; an address given
 * to StFreeRing that is not inside the ring's data blocks (or a frame that reaches past the ring). The emulation mode of Sony's
 * routine is not reachable from the programs and is left out. A call of the handler from inside itself returns at step 1 while the
 * transfer-end routine runs (f1) and stops the program anywhere else. */
#ifndef PORT_STREAM_H
#define PORT_STREAM_H

/* StSetRing(ring, n): the ring at `ring` of n slots; then it is cleared as the library's StClearRing does: r = s = w = 0, dmabusy = 0,
 * every slot's state 0, nextchunk = curframe = 0, the bound deferred flag written 0, the queue emptied. */
void port_StSetRing(unsigned long *ring, int n);

/* StSetStream(mode, start, end, f1, f2): mask = 1; start and end frames (0: from the first / no end); f1 called when a frame is
 * complete, f2 at the end frame and at a stall; mode & 1 is the busy rule's switch; nextchunk = curframe = 0. Touches no slot, no
 * w, s or r. */
void port_StSetStream(unsigned long mode, unsigned long start, unsigned long end, void (*f1)(void), void (*f2)(void));

/* StGetNext(&addr, &header): looks at slot r. State 1 (a wrap marker): r = 0 (the marker is cleared only when end is not 0), then
 * the slot is looked at again. State 2: it becomes 4, *addr is the data block, *header the header, the result 0. Otherwise 1.
 * r is moved only by the marker. */
int port_StGetNext(unsigned long **addr, unsigned long **header);

/* StFreeRing(addr): the slot of the data block at `addr`. 1 when its state is not 4. Otherwise the states of as many slots as the
 * header's chunk count says, from that slot, are 0, r is that slot plus the count, and the result is 0. */
int port_StFreeRing(unsigned long *addr);

/* StUnSetRing(): the host ready handler removed (port_cd_host_ready(0)); the ring, the variables and the flag unchanged; no Pause; the
 * queue emptied. */
void port_StUnSetRing(void);

/* StCdInterrupt(): the sector handler (see above). */
void port_StCdInterrupt(void);

/* CdRead2(mode): Setmode with the low byte of mode through port_CdControl (its result ignored, as the library's); with bit 0x100: the
 * mode's 0x20 bit must be set (else the program stops: the "old type" stream) and the host handler is set; then ReadS (0x1b, no
 * parameter) through port_CdControl, whose result is returned. */
int port_CdRead2(long mode);

/* The address, in the game's memory, of the word that the program's copy of the library calls `deferred` (0: none). The handler
 * writes 1 there when it queues a sector. The address must lie inside the RAM, else the program stops. */
void port_stream_bind(unsigned deferred_flag_address);

#endif
