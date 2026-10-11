/* The disc layer's interface. Host routines for the CD library functions the
 * game calls, driven by the disc image of disc.c.
 *
 * The seams with the rest of the runtime:
 *  - main.c calls port_cd_init once with the open image, and the frame clock
 *    (kernel.c) calls port_cd_tick from the vblank, after the vblank handlers
 *    (sectors are delivered only there).
 *  - this layer calls port_tick() in the routines in which the game waits
 *    or polls (CdSync, CdReady, and the wait inside CdControl).
 *  - library.c takes port_cd_library[] and writes the jumps. */
#ifndef PORT_CD_H
#define PORT_CD_H

#include "port.h"

/* port.h declares struct port_library and defines PORT_LIBRARY_DECLARED; the
 * fallback below is for a build whose port.h does not. */
#ifndef PORT_LIBRARY_DECLARED
#define PORT_LIBRARY_DECLARED
struct port_library {
    const char *name;   /* the name the build's tables give the library function */
    void *host;         /* the host routine (cdecl), or null at the end of an array */
    const char *note;   /* why a routine that does little is right on a PC; or null */
};
#endif

/* kernel.c: one step of the clock; may call port_cd_tick. */
extern void port_tick(void);

/* The rate of delivery (the rule is in the header of cd.c): the console's drive,
 * counted by the frame. A carry in fifths of a tick's drive time grows by 5 at every
 * port_cd_tick. A sector costs 2 with the double-speed bit
 * (0x80) of the mode (2, 3, 2, 3 ... sectors a tick: 150 a second at 60
 * ticks) and 4 without it (1, 1, 1, 2 ...: 75 a second). The carry is
 * cleared when a read starts and when the speed bit changes; when a sector
 * could not be delivered the carry is cut to its remainder (carry modulo the
 * cost), so no tick brings more than 3 sectors (2 at single speed). */

/* The library's own variables that game code reads or writes directly
 * (addresses in the game's RAM, this build of SLPS_004.15). */
#define PORT_CD_VAR_SYNC_CB   0x80181b34u  /* CdSyncCallback's handler */
#define PORT_CD_VAR_READY_CB  0x80181b38u  /* CdReadyCallback's handler (set by the game's C) */
#define PORT_CD_VAR_READ_CB   0x80181b3cu
#define PORT_CD_VAR_STATUS    0x80181b44u  /* word: the status byte of the last result */
#define PORT_CD_VAR_LASTCOM   0x80181b54u  /* byte: the last command (CdLastCom reads it) */

extern const struct port_library port_cd_library[];

/* Give the layer the open image and reset it. 0, or -1 with a line in err. */
int  port_cd_init(struct port_disc *disc);
/* Deliver: complete pending commands, then the sectors of a read in progress
 * that the rate gives for this tick (above). Re-entry (from a ready handler) completes
 * commands and delivers nothing. */
void port_cd_tick(void);
/* The sector of the image that the word of RAM containing `address` (a PS1
 * address) came from: the one CdGetSector last wrote all four bytes of it
 * from; -1 if none, if the word holds bytes of more than one origin, if they
 * are the zeros of an empty data FIFO, or if the address is outside RAM. */
int  port_cd_source_at(unsigned address);

/* The host routines (the table points at these). Pointers are host
 * pointers (the PS1's addresses in the port). */
int            port_CdInit(void);
int            port_CdSync(int mode, unsigned char *result);
int            port_CdReady(int mode, unsigned char *result);
int            port_CdControl(int com, unsigned char *param, unsigned char *result);
int            port_CdControlF(int com, unsigned char *param);
int            port_CdControlB(int com, unsigned char *param, unsigned char *result);
int            port_CdMix(void *vol);
int            port_CdGetSector(void *madr, int words);
unsigned char *port_CdIntToPos(int i, unsigned char *p);
int            port_CdPosToInt(const unsigned char *p);

/* The disc's streamed audio (the queue is in cd.c, see its header). port_xa_set_sink(1): something takes frames out (iopage.c calls
 * it when the device opened or the dump's file is written); without it audio sectors are consumed in silence. port_xa_pull: up to `num_frames` stereo
 * frames from the queue through the last CdMix's volumes, the count given; the caller holds the audio lock (PsyZ's pull does).
 * port_xa_counts_get: the counts so far, under the lock. port_audio_lock/unlock (iopage.c): PsyZ's audio lock, nothing without
 * PsyZ. port_frames and port_trace_line: kernel.c and library.c. */
struct port_xa_counts {
    unsigned played, held, refused, skipped, silent;   /* sectors */
    unsigned frames, pulled, queued;                    /* frames at 44,100 Hz: queued in all, pulled in all, in the queue now */
};
void   port_xa_set_sink(int on);
size_t port_xa_pull(short *out, size_t num_frames);
void   port_xa_counts_get(struct port_xa_counts *out);
void   port_audio_lock(void);
void   port_audio_unlock(void);
/* The sound chip's registers for port_CdInit, which stands for library code that writes them itself (iopage.c has the two and says
 * what they do). `offset` is the register's, counted from 0x1F801C00. The return is 1, or 0 in a build with no model of the chip,
 * where nothing is read or written. */
int    port_iopage_chip_read(unsigned offset, unsigned *value);
int    port_iopage_chip_write(unsigned offset, unsigned value);

/* Test hooks; null in the port. port_cd_ram: where the 2 MB of the PS1's RAM
 * lives, when it is not at its own addresses (a 64-bit test). port_cd_call:
 * how to call the ready handler whose value is stored in RAM, when that value
 * cannot be a host function address. */
extern unsigned char *port_cd_ram;
/* The modules package (modules.c) sets this: called after every CdGetSector
 * that wrote `bytes` bytes at PS1 address `address` in RAM. Null otherwise. */
extern void (*port_cd_written_hook)(unsigned address, unsigned bytes);
/* The open image the layer was given by port_cd_init (null before). */
struct port_disc *port_cd_disc(void);
extern void (*port_cd_call)(unsigned handler, int intr, unsigned char *result);

#endif
