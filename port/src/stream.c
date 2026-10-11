/* Sony's movie stream calls for the PC program: see stream.h for what each call does, for the busy rule and for the readings taken
 * where the library's text left a case open. */
#include "port.h"
#include "cd.h"
#include "press.h"
#include "stream.h"
#include <stddef.h>
#include <string.h>

#define RAM_END (PORT_RAM_BASE + PORT_RAM_SIZE)
#define HDR 32u         /* a slot's header */
#define BLK 2016u       /* a slot's data block */
#define SECT 2340u      /* a sector's bytes in the drive's whole-sector mode */
#define QMAX 16

/* the library's own variables */
static unsigned char *ringp;
static unsigned n_slots;
static unsigned w, s, r;
static unsigned mask, start, end;
static unsigned curframe, nextchunk;
static unsigned mode;
static void (*f1)(void);
static void (*f2)(void);
static unsigned dmabusy;
static int active;      /* the handler is running */

/* the runtime's own */
static unsigned flag_address;
static struct {
    unsigned char b[SECT];
    int sector;
} queue[QMAX];
static unsigned qhead, qcount;

/* The span [p, p + n) must lie inside the mapped RAM, else the program stops with the call's name. */
static void need(const char *call, const char *what, const void *p, unsigned long long n)
{
    unsigned long long a = (unsigned)(size_t)p;
    if (a < PORT_RAM_BASE || a + n > RAM_END)
        port_halt(PORT_EXIT_OTHER, "%s: %s at 0x%08x, %llu bytes, is not inside the game's RAM", call, what, (unsigned)a, n);
}

static unsigned g16(const unsigned char *p) { return (unsigned)p[0] | (unsigned)p[1] << 8; }
static void s16(unsigned char *p, unsigned v) { p[0] = (unsigned char)v; p[1] = (unsigned char)(v >> 8); }

static void need_ring(const char *call)
{
    if (!ringp) port_halt(PORT_EXIT_OTHER, "%s: no ring has been set (StSetRing)", call);
}

static unsigned char *hdrp(const char *call, unsigned i)
{
    unsigned char *p;
    need_ring(call);
    if (i >= n_slots) port_halt(PORT_EXIT_OTHER, "%s: slot %u is not in the ring of %u slots", call, i, n_slots);
    p = ringp + (unsigned long long)HDR * i;
    need(call, "a slot's header", p, HDR);
    return p;
}

static unsigned char *blkp(const char *call, unsigned i)
{
    unsigned char *p;
    need_ring(call);
    if (i >= n_slots) port_halt(PORT_EXIT_OTHER, "%s: slot %u is not in the ring of %u slots", call, i, n_slots);
    p = ringp + (unsigned long long)HDR * n_slots + (unsigned long long)BLK * i;
    need(call, "a slot's data block", p, BLK);
    return p;
}

static void put_state(const char *call, unsigned i, unsigned v) { s16(hdrp(call, i), v); }

static void callback(const char *path, void (*f)(void))
{
    if (!f) return;
    port_target_check(path, (const void *)f);
    f();
}

static void write_flag(unsigned v)
{
    unsigned char *p = (unsigned char *)(size_t)flag_address;
    if (!flag_address) return;
    need("StCdInterrupt", "the deferred flag", p, 4);
    p[0] = (unsigned char)v;
    p[1] = (unsigned char)(v >> 8);
    p[2] = (unsigned char)(v >> 16);
    p[3] = (unsigned char)(v >> 24);
}

void port_stream_bind(unsigned deferred_flag_address)
{
    if (deferred_flag_address) need("port_stream_bind", "the deferred flag", (const void *)(size_t)deferred_flag_address, 4);
    flag_address = deferred_flag_address;
}

void port_StSetRing(unsigned long *ring, int n)
{
    unsigned i;
    if (n < 2) port_halt(PORT_EXIT_OTHER, "StSetRing: a ring of %d slots (at least 2 are needed)", n);
    need("StSetRing", "the ring", ring, (unsigned long long)n * (HDR + BLK));
    ringp = (unsigned char *)ring;
    n_slots = (unsigned)n;
    r = s = w = 0;
    dmabusy = 0;
    for (i = 0; i < n_slots; i++) put_state("StSetRing", i, 0);
    if (flag_address) {
        unsigned char *p = (unsigned char *)(size_t)flag_address;
        need("StSetRing", "the deferred flag", p, 4);
        memset(p, 0, 4);
    }
    nextchunk = curframe = 0;
    qhead = qcount = 0;   /* this layer's own rule: the library has no queue */
}

void port_StSetStream(unsigned long m, unsigned long st, unsigned long en, void (*a)(void), void (*b)(void))
{
    mask = 1;
    start = (unsigned)st;
    end = (unsigned)en;
    f1 = a;
    f2 = b;
    mode = (unsigned)m & 1u;
    nextchunk = curframe = 0;
}

int port_StGetNext(unsigned long **addr, unsigned long **header)
{
    unsigned char *h;
    need("StGetNext", "the address pointer", addr, sizeof *addr);
    need("StGetNext", "the header pointer", header, sizeof *header);
    h = hdrp("StGetNext", r);
    if (g16(h) == 1) {
        if (end != 0) s16(h, 0);
        r = 0;
        h = hdrp("StGetNext", r);
    }
    if (g16(h) == 2) {
        s16(h, 4);
        *addr = (unsigned long *)blkp("StGetNext", r);
        *header = (unsigned long *)h;
        return 0;
    }
    return 1;
}

int port_StFreeRing(unsigned long *addr)
{
    unsigned char *h, *base;
    unsigned long long off;
    unsigned i, count, k;
    need("StFreeRing", "the block", addr, 1);
    need_ring("StFreeRing");
    base = ringp + (unsigned long long)HDR * n_slots;
    if ((unsigned char *)addr < base || (unsigned long long)((unsigned char *)addr - base) >= (unsigned long long)BLK * n_slots)
        port_halt(PORT_EXIT_OTHER, "StFreeRing: the address 0x%08x is not in the ring's data blocks", (unsigned)(size_t)addr);
    off = (unsigned long long)((unsigned char *)addr - base);
    i = (unsigned)(off / BLK);
    h = hdrp("StFreeRing", i);
    if (g16(h) != 4) return 1;
    count = g16(h + 6);
    if ((unsigned long long)i + count > n_slots)
        port_halt(PORT_EXIT_OTHER, "StFreeRing: a frame of %u slots from slot %u reaches past the ring of %u slots", count, i, n_slots);
    for (k = 0; k < count; k++) put_state("StFreeRing", i + k, 0);
    r = i + count;
    return 0;
}

/* The sector handler's steps 4 to 10 for one sector: the one at hand (qb null: the bytes are in the drive's FIFO) or a queued one
 * (qb: its 2,340 bytes, qsector: its number). */
static void work(const unsigned char *qb, int qsector)
{
    const char *C = "StCdInterrupt";
    unsigned char first[12], *h, *d;
    unsigned fr, chunk, count, i, last;
    int sector = qsector;

    h = hdrp(C, w);
    if (g16(h) != 0) return;                                  /* 4: the slot is not free; the sector is not taken */
    if (qb) {                                                 /* 5 */
        memcpy(first, qb, 12);
        port_cd_store(h, qb + 12, HDR, sector);
    } else {
        port_cd_fifo_read(first, 12, &sector);
        port_CdGetSector(h, (int)(HDR / 4));
    }
    port_cd_store(h + 28, first, 4, sector);                  /* the 4 kept bytes over header bytes 28 to 31 */
    fr = g16(h + 8);
    if (mask == 1 && start != 0) {                            /* 6 */
        if (fr != start) { s16(h, 0); return; }
    }
    if (mask == 1) mask = 0;
    if (g16(h) != 0x160 || ((g16(h + 2) >> 10) & 0x1f) != 0) { s16(h, 0); return; }   /* 7: type 0 */
    chunk = g16(h + 4);
    if (chunk != nextchunk || (curframe != 0 && curframe != fr)) {                       /* 8 */
        for (i = s; i <= w; i++) put_state(C, i, 0);   /* the partial frame's slots and the sector's own */
        w = s;
        curframe = nextchunk = 0;
        return;
    }
    if (chunk == 0) {                                         /* 9 */
        curframe = fr;
        if (end != 0 && curframe >= end) {
            curframe = nextchunk = 0;
            s16(h, 0);
            mask = 1;
            callback("stream end-frame callback", f2);
            return;
        }
        count = g16(h + 6);
        if ((int)n_slots - (int)w - 1 < (int)count) {
            if (end == 0) {
                s16(h, 1);
                mask = 1;
                callback("stream end-frame callback", f2);
                return;
            }
            if (g16(hdrp(C, 0)) != 0) { s16(h, 0); return; }
            memcpy(hdrp(C, 0), h, HDR);
            s16(h, 1);
            w = 0;
            h = hdrp(C, 0);
        }
        s = w;
    }
    nextchunk = (nextchunk + 1) & 0xffff;                     /* 10 */
    d = blkp(C, w);
    if (qb) port_cd_store(d, qb + 12 + HDR, BLK, sector);
    else port_CdGetSector(d, (int)(BLK / 4));
    last = (int)g16(h + 6) - 1 == (int)g16(h + 4);
    if (last) {
        dmabusy = 1;
        nextchunk = curframe = 0;
    }
    s16(h, 3);
    w++;
    if (last) {   /* the transfer-end routine, at once */
        put_state(C, s, 2);
        s = w;
        callback("stream frame callback", f1);
        dmabusy = 0;
    }
}

static void work_off_queue(void)
{
    while (qcount) {
        unsigned k = qhead;
        qhead = (qhead + 1) % QMAX;
        qcount--;
        work(queue[k].b, queue[k].sector);
    }
}

void port_StCdInterrupt(void)
{
    unsigned char res[8];
    int rc, busy;

    if (dmabusy == 1) return;                                                 /* 1 */
    if (active) port_halt(PORT_EXIT_OTHER, "StCdInterrupt: called from inside itself");
    need_ring("StCdInterrupt");
    active = 1;
    busy = mode && port_press_out_busy();                                     /* 2 */
    if (!busy) work_off_queue();
    memset(res, 0, sizeof res);
    rc = port_CdReady(1, res);                                                /* 3 */
    if (rc == 1 && !(res[0] & 4)) {
        if (busy) {
            unsigned k;
            if (!flag_address) port_halt(PORT_EXIT_OTHER, "StCdInterrupt: a sector would be put aside (the decoder is busy) and no deferred flag is bound");
            if (qcount == QMAX) port_halt(PORT_EXIT_OTHER, "StCdInterrupt: the queue of %d sectors put aside is full", QMAX);
            k = (qhead + qcount) % QMAX;
            port_cd_fifo_read(queue[k].b, SECT, &queue[k].sector);
            qcount++;
            write_flag(1);
        } else {
            work(0, 0);
        }
    }
    active = 0;
}

void port_StUnSetRing(void)
{
    port_cd_host_ready(0);
    qhead = qcount = 0;
}

static void host_ready(int intr, unsigned char *result)
{
    (void)intr;
    (void)result;
    port_StCdInterrupt();
}

int port_CdRead2(long m)
{
    unsigned char param[4];
    memset(param, 0, sizeof param);
    param[0] = (unsigned char)(m & 0xff);
    port_CdControl(0xe, param, 0);
    if (m & 0x100) {
        if (!(m & 0x20)) port_halt(PORT_EXIT_OTHER, "CdRead2: mode 0x%lx has bit 0x100 and no bit 0x20; the \"old type\" stream is not modelled", (unsigned long)m);
        port_cd_host_ready(host_ready);
    }
    return port_CdControl(0x1b, 0, 0);
}
