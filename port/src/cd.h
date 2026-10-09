/* The disc layer's interface (package L2). Host routines for the CD library
 * functions the game calls, driven by the disc image of disc.c.
 *
 * The seams with the driver (L1):
 *  - L1 calls port_cd_init once with the open image, and port_cd_tick from the
 *    vblank, after the vblank handlers (sectors are delivered only there).
 *  - this layer calls port_tick() in the routines in which the game waits
 *    or polls (CdSync, CdReady, and the wait inside CdControl).
 *  - L1's table code takes port_cd_library[] and writes the jumps. */
#ifndef PORT_CD_H
#define PORT_CD_H

#include "port.h"

/* L1: when port.h declares struct port_library, define PORT_LIBRARY_DECLARED
 * there (or delete this block). */
#ifndef PORT_LIBRARY_DECLARED
#define PORT_LIBRARY_DECLARED
struct port_library {
    const char *name;   /* the name the build's tables give the library function */
    void *host;         /* the host routine (cdecl), or null at the end of an array */
    const char *note;   /* why a routine that does little is right on a PC; or null */
};
#endif

/* L1's: one step of the clock; may call port_cd_tick. */
extern void port_tick(void);

/* How many sectors one port_cd_tick delivers while a read is on (double
 * speed is 150 a second, so 3 per 60 Hz frame is the real rate halved
 * with a margin; the value is a named constant to be tuned). */
#define PORT_CD_SECTORS_PER_TICK 3

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
/* Deliver: complete pending commands, then up to PORT_CD_SECTORS_PER_TICK
 * sectors of a read in progress. Re-entry (from a ready handler) completes
 * commands and delivers nothing. */
void port_cd_tick(void);
/* The sector of the image that CdGetSector last wrote into the 4 KB page of
 * RAM containing `address` (a PS1 address); -1 if none or outside RAM. */
int  port_cd_page_source(unsigned address);

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
