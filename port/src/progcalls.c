/* The small library routines of the logo's program for the PC program: see progcalls.h for what each call does. */
#include "port.h"
#include "cd.h"
#include "progcalls.h"
#include <stdint.h>
#include <string.h>

#define RAM_END (PORT_RAM_BASE + PORT_RAM_SIZE)
#define WORDS 0x200
#define WAIT_LIMIT 600   /* vblanks, as the disc layer's own limit for a wait (cd.c); see progcalls.h */

/* ---- the plain read ---- */

enum { IDLE = 0, READING = 1, DONE = 2, FAILED = 3 };
static volatile int state;
static volatile int remaining;             /* sectors still to come */
static volatile unsigned char *dest;       /* where the next sector goes */
static unsigned char last_result[8];       /* the result bytes of the last interrupt */

/* This file's handler for the disc layer's host ready hook: a sector, or a disc error. */
static void on_ready(int intr, unsigned char *result)
{
    memcpy(last_result, result, 8);
    if (intr != 1) {   /* 5, a disc error, or anything else: the read has failed */
        port_cd_host_ready(0);
        state = FAILED;
        return;
    }
    port_CdGetSector((void *)dest, WORDS);
    dest += 4 * WORDS;
    if (--remaining == 0) {
        port_cd_host_ready(0);
        port_CdControlB(9, 0, 0);   /* Pause */
        state = DONE;
    }
}

int port_CdRead(int sectors, void *buf, int mode)
{
    unsigned long long a = (unsigned)(size_t)buf, n;
    unsigned char param[1];
    if (mode & 0x30) port_halt(PORT_EXIT_OTHER, "CdRead: mode 0x%x asks for a sector form other than 2,048 bytes", (unsigned)mode & 0xffff);
    if (sectors < 1) return 0;
    n = (unsigned long long)sectors * PORT_DATA;
    if (a < PORT_RAM_BASE || a + n > RAM_END)
        port_halt(PORT_EXIT_OTHER, "CdRead: the buffer at 0x%08x, %llu bytes, is not inside the game's RAM", (unsigned)a, n);
    port_cd_host_ready(0);
    state = IDLE;
    memset(last_result, 0, sizeof last_result);
    if (*(volatile unsigned char *)port_g((const void *)(size_t)PORT_CD_VAR_STATUS) & 0xe0u) {   /* the drive is reading */
        if (!port_CdControlB(9, 0, 0)) return 0;
    }
    param[0] = (unsigned char)mode;
    if (!port_CdControlB(0xe, param, 0)) return 0;
    dest = (unsigned char *)buf;
    remaining = sectors;
    state = READING;
    port_cd_host_ready(on_ready);
    if (!port_CdControl(6, 0, 0)) {
        port_cd_host_ready(0);
        state = IDLE;
        return 0;
    }
    return 1;
}

int port_CdReadSync(int mode, unsigned char *result)
{
    int seen = remaining, rc;
    if (mode == 0) {
        unsigned since = port_frames();   /* the vblank count at the call, then at the last sector */
        while (state == READING && port_frames() - since < WAIT_LIMIT) {
            port_tick();
            if (remaining != seen) {
                seen = remaining;
                since = port_frames();
            }
        }
        if (state == READING) {
            printf("cd: CdReadSync timed out\n");
            rc = -1;
        } else {
            rc = state == FAILED ? -1 : 0;
        }
    } else {
        rc = state == FAILED ? -1 : state == READING ? remaining : 0;
    }
    if (result) memcpy(port_gs(result, 8), last_result, 8);
    return rc;
}

/* ---- the pads ---- */

static int pads_started;

int port_PadInit(int mode)
{
    (void)mode;
    pads_started = 1;
    return 1;
}

void port_PadStop(void)
{
    pads_started = 0;
}

unsigned port_PadRead(int id)
{
    unsigned char frame[PORT_PAD_FRAME];
    (void)id;
    port_pad_host_read(frame, PORT_PAD_FRAME);
    if (frame[0] != 0 || frame[1] != 0x41) return 0;
    port_input_apply(frame, port_frames());
    return (~(unsigned)(frame[3] | (frame[2] << 8))) & 0xffffu;
}

/* ---- the video mode ---- */

static long video_mode;

long port_SetVideoMode(long mode)
{
    long before = video_mode;
    video_mode = mode;
    return before;
}

/* ---- the geometry coprocessor ---- */

void port_InitGeom(void)
{
}
