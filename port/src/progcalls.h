/* The small library routines that the logo's stand-alone program calls and the runtime had no host routine for yet, apart from the
 * movie decoder's (press.h) and the movie stream's: a plain read of sectors into memory (CdRead, CdReadSync), Sony's simple pad
 * calls (PadInit, PadRead, PadStop) and SetVideoMode. The port's list of programs (port/programs.toml) binds the calls for the logo's program. Written from
 * what the program's own copies of the routines do (read in a private listing, in words here) and from the disc layer's header
 * (cd.c, cd.h) and input.c; nothing here is compared with a console's library or its output. The checks are in
 * port/tools/test_hostprogcalls.py.
 *
 * Every pointer is a game address, which is a real address in the PC program. CdRead's sector buffer is checked, whole, to lie
 * inside the mapped RAM (PORT_RAM_BASE to PORT_RAM_BASE + PORT_RAM_SIZE); a buffer outside it, and every case below that is not
 * modelled, ends the program with a `stop:` line that names the call (port_halt, status PORT_EXIT_OTHER). CdReadSync's eight
 * result bytes go to the pointer it is given, as it is, when that is not null: that pointer is not checked against the RAM.
 *
 * CdRead(sectors, buf, mode). In the program's own copy: a Pause first if the drive's status byte shows reading (the bits 0xe0),
 * Setmode with the mode's low byte, Setloc with the last position the program sent, its own ready handler, then ReadN. Per
 * delivered sector the handler takes the sector's 0x200 words into the buffer; after the last it restores the handlers and
 * Pauses. Here: the same steps through the disc layer (cd.c) with this file's handler set as the host ready handler
 * (port_cd_host_ready). Returns 1 when the read was started and 0 when the disc layer refused a step (the host ready handler is
 * then removed) or when `sectors` is below 1. The mode's bits 0x30 must be 0 (a sector of 2,048 bytes, the only form the programs
 * use); another form stops the program. The whole span (sectors * 2,048 bytes) must lie in the mapped RAM or the program stops.
 *   - The position: the read starts where the disc layer's drive is, which is the position of the last Setloc the program sent
 *     when it was followed by a seek and no read (the program sends it with CdControl(0x15, ...) before every CdRead). Sony's
 *     routine would send its last Setloc again; this routine does not, because the disc layer does not tell what it was. The two
 *     differ only for a CdRead after a read, or after a Pause that cut one, with no new position in between.
 *   - NOT modelled, and not tried: Sony's retries (up to four attempts in CdRead, a budget of eight restarts at an interrupt that
 *     is not a sector, a restart after 60 vblanks without a sector). A disc error (the handler called with 5) ends the read as
 *     failed, once, and so does any other interrupt code.
 *   - After the last sector the handler removes itself and Pauses the drive (a blocking Pause, as the rig of the disc layer's own
 *     checks does from a handler); the read counts as done only after the Pause.
 *
 * CdReadSync(mode, result). Mode 0 waits until the read is done and returns 0, or -1 when it failed (or the wait ran out, with a
 * line `cd: CdReadSync timed out`). The wait is the disc layer's wait: port_tick in a loop, with the disc layer's own limit of 600,
 * counted in vblanks (port_frames), not in calls of port_tick. The wait goes on while the read is on and fewer than 600 vblanks have
 * passed since the last sector came (since the call, before the first); a sector starts the count again, so a long read is not cut
 * off while it makes progress. A count of calls is no clock: the runtime's tick sleeps only while the next vblank is more than a
 * millisecond away and returns at once otherwise, so many hundreds of calls can pass inside one vblank, and a read that needs some
 * 30 vblanks was given up at once when the limit counted calls. A mode
 * that is not 0 returns at once the number of sectors still to come (0 when done, -1 when failed). `result`, when not null, gets
 * the 8 result bytes of the last interrupt the handler saw (zeros before any). With no read ever started both return 0.
 *
 * PadInit(mode), PadRead(id), PadStop(). In the program's own copies: PadInit stores its argument, sets a word to -1, calls
 * ResetCallback, then the BIOS's B0 15h (pad init and start) with 0x20000001 and that word's address, then B0 5Bh
 * (ChangeClearPAD) with 0, and returns the result of the first BIOS call; PadRead calls B0 16h and returns the complement of that
 * word (its argument is not used by the routine itself); PadStop calls B0 14h (StopPAD). The word is 32 bits: the BIOS keeps the
 * two ports' buttons in it with a 0 bit for pressed, so the complement has a 1 bit for pressed. The program's main reads it with
 * id 1 and tests the mask 0x820 with an `andi`, so only the low half (port 1) is looked at there; the bits are Start (bit 11) and
 * circle (bit 5), as the layout of input.c has them.
 * Here: PadInit and PadStop keep a flag (and return 1 and nothing); the runtime's pads are already started (InitPAD, StartPAD),
 * so neither changes anything, and the flag changes nothing of PadRead's word. PadRead builds the word from the frame of port 1
 * that the runtime's own pad code gives at this moment (port_pad_host_read, then the input script's buttons for this vertical
 * blank, port_input_apply, as kernel.c does for the game's own buffers; the devices are not read another way) as input.c's header
 * describes it: ~(byte3 | byte2 << 8) & 0xffff for a digital pad (status 0, kind 0x41), in the low half, and 0 for a frame that
 * is not a digital pad's; the high half is port 2, which has no controller, so no bit is pressed (0). The id is not used.
 * The return value of PadInit (1) is this file's choice: the listing returns what the BIOS call returns, which is not known here
 * and which the program's main does not read.
 *
 * SetVideoMode(mode). In the program's own copy a word is exchanged: the call returns the one before and stores the new one (GetVideoMode
 * returns it). Here the same, 0 at the start. The port's display is NOT changed by it: the picture layer shows the picture as the
 * game draws it, whatever mode the game says. */
#ifndef PORT_PROGCALLS_H
#define PORT_PROGCALLS_H

int           port_CdRead(int sectors, void *buf, int mode);
int           port_CdReadSync(int mode, unsigned char *result);
int           port_PadInit(int mode);
unsigned      port_PadRead(int id);
void          port_PadStop(void);
long          port_SetVideoMode(long mode);

/* InitGeom(). Does nothing. Sony's routine sets up the console's geometry coprocessor; the PC program has none, the port's picture
 * calls use none, and the opening movie's program holds no coprocessor instruction in its own functions (a private scan of the
 * eighteen functions of the program that have a caller counted 0). The port's list of programs binds the program's call to this
 * routine. */
void          port_InitGeom(void);

#endif
