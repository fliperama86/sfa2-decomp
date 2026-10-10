/* The disc layer: host routines for the CD library functions that the game
 * calls, reading the disc image of disc.c.
 *
 * THE GAME'S SEQUENCE (as the game's C drives the library; commands are the
 * controller's numbers):
 *
 * Boot, init: CdInit until it returns 1 (up to four tries); then Setmode
 *   0x80 (double speed) with CdControlB, then SeekL (0x15) with a position
 *   parameter (CdControl loops until it returns 1).
 * Position: every file is an absolute sector from the game's table, turned
 *   into a position by CdIntToPos (the sector plus 150, as BCD minute,
 *   second, frame). Setloc and the implied Setloc below go back to the
 *   sector as position minus 150, which is the image's sector number.
 * One-shot loader (stand-alone programs, not used by the port): Setmode
 *   0x80, Nop (1) until the status byte shows no 0x40, Pause (9); the ready
 *   callback is cleared (the game's own C stores 0 in the library variable);
 *   then CdControlB(Setloc), CdControlB(ReadN 6); poll CdReady(1) until it is
 *   not 0 (5 means start over); CdGetSector(buffer, 0x200 words) for the
 *   header; for each following sector CdReady(0) (5 means start over) and
 *   CdGetSector(0x200 words); then Pause.
 * Streamed loads (overlays, the normal case): if a read is on, CdControlB(
 *   Pause); the game's C stores its handler in the library variable; Nop
 *   until good; CdReady(1) polled until it returns 0 (drains a stale flag);
 *   CdControl(Setmode 0x80), CdControlB(Setloc), CdControl(ReadN). Per
 *   sector the handler is called with (1, result); it calls CdGetSector one
 *   to three times (the whole 0x200 words, or the words left and a rest into
 *   a dump place) and, when the stream is done, stores 0 into the handler
 *   variable and Pauses with CdControlB. Any code other than 1 (5) is a
 *   read error and the handler restarts the load.
 * Audio and state machine (several calls per frame): CdSync(1, status) as a
 *   poll (0 pending, 2 complete, 5 error); CdControl(ReadS 0x1b, position)
 *   (the position makes a Setloc first), Pause 9, SeekL 0x15 with position,
 *   Setfilter 0xd (two bytes), Setmode 0xe (0xc8: double speed, XA on,
 *   filter), CdControlF(GetlocP 0x11) then CdSync(1, buffer): bytes 5 to 7 of
 *   the result are the absolute position, read with CdPosToInt; the game
 *   reads the last command through CdLastCom (a library variable kept here).
 * Commands never sent, so not written (they stop): everything but 1, 2, 6,
 *   9, 0xd, 0xe, 0x11, 0x15, 0x1b; and a Setmode with the 0x20 bit (whole
 *   sector, 2340 bytes).
 *
 * What the original library does that the game relies on (read at the
 * listing's addresses):
 *  - 8015c9c0 CdControl: up to four tries; returns 1 when the command was
 *    accepted and its first response came, 0 otherwise. A position
 *    parameter on SeekL/ReadN/ReadS is sent first as Setloc (the same try).
 *    A command that needs a parameter (Setloc, Setfilter, Setmode) given a
 *    null one fails. Before sending it waits for the previous command's
 *    completion (the blocking sync). It stores the command in the
 *    library variable CdLastCom reads (8015dcb4). With a result pointer it
 *    receives the 8 bytes of the first response (status byte first).
 *  - 8015cb08 CdControlF: the same without waiting for the first response and
 *    without a result; returns 1.
 *  - 8015cc44 CdControlB: CdControl, then CdSync(0, result); returns 1 only if
 *    that returned 2 (complete). The result is then the final response.
 *  - 8015c938/8015d584 CdSync: returns 2 (complete) or 5 (error) when the
 *    last command is done, and copies the 8 response bytes; afterwards the
 *    state rests at 2; 0 if not done and mode 1; mode 0 waits.
 *  - 8015c958/8015d7f8 CdReady: returns 1 (a sector arrived), 5 (disc error),
 *    4 (data end) once per interrupt, clearing it; copies 8 bytes; 0 in
 *    mode 1 when none. The interrupt code of the ready handler is that same
 *    code, its second argument the response buffer (called at 8015d710).
 *  - 8015ec84/8015cdbc CdGetSector: moves `words` words from the data FIFO,
 *    in order, so successive calls give successive pieces of the sector;
 *    returns 1 (the inner routine returns 0 always).
 *  - 8015cd98 CdMix: returns 1. 8015c6ec CdInit: returns 1 after a reset and
 *    stores the library's default handlers in the three callback variables.
 *  - 8015cec4 CdIntToPos / 8015cfc8 CdPosToInt: BCD conversions with the 150
 *    sector offset; IntToPos returns its second argument.
 *
 * Timing: nothing here sleeps. A command that has a second response (Pause,
 * SeekL) completes in the next port_cd_tick (or when a blocking wait asks);
 * sectors are delivered only in port_cd_tick. */
#include "cd.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

unsigned char *port_cd_ram;
void (*port_cd_written_hook)(unsigned address, unsigned bytes);
void (*port_cd_call)(unsigned handler, int intr, unsigned char *result);

#define NPAGES (PORT_RAM_SIZE / 0x1000u)
#define ST_IDLE 0x02u   /* motor on */
#define ST_READ 0x22u   /* motor on, reading */
#define ST_ERR  0x03u   /* motor on, error */
#define WAIT_LIMIT 600  /* polls of a wait with nothing to wait for (about ten seconds) */

static struct {
    struct port_disc *disc;
    unsigned long long total;       /* sectors in the image */
    int started;
    /* the drive */
    int reading;
    unsigned pos;                   /* the next sector to read */
    unsigned loc;                   /* a Setloc not yet used */
    int loc_set;
    unsigned char mode, status;
    int pending;                    /* a second response is due */
    unsigned char pending_status;
    /* the interrupt flags the library keeps */
    unsigned char sync;             /* 0 none yet, 3 first response only, 2 complete, 5 error */
    unsigned char sync_result[8];
    unsigned char ready;            /* 0, 1 data, 5 error */
    unsigned char ready_result[8];
    /* the sector under the FIFO */
    int have_sector;
    unsigned cur_sector, offset;
    unsigned char data[PORT_DATA];
    int in_delivery;
    int page_source[NPAGES];
} cd;

/* ---- stops ---------------------------------------------------------- */

static void cd_stop(const char *line, unsigned value)
{
    printf("stop: %s 0x%x is not handled\n", line, value);
    fflush(stdout);
    exit(PORT_EXIT_DISC);
}

static void need_disc(void)
{
    if (!cd.started) {
        printf("stop: the disc layer was used before port_cd_init\n");
        fflush(stdout);
        exit(PORT_EXIT_DISC);
    }
}

/* ---- the PS1's RAM -------------------------------------------------- */

static uintptr_t ram_base(void)
{
    return port_cd_ram ? (uintptr_t)port_cd_ram : (uintptr_t)PORT_RAM_BASE;
}

static unsigned char *lib_ptr(unsigned address)
{
    return (unsigned char *)(ram_base() + (address - PORT_RAM_BASE));
}

static unsigned lib_get32(unsigned address)
{
    const unsigned char *p = lib_ptr(address);
    return (unsigned)p[0] | (unsigned)p[1] << 8 | (unsigned)p[2] << 16 | (unsigned)p[3] << 24;
}

static void lib_put32(unsigned address, unsigned v)
{
    unsigned char *p = lib_ptr(address);
    p[0] = (unsigned char)v;
    p[1] = (unsigned char)(v >> 8);
    p[2] = (unsigned char)(v >> 16);
    p[3] = (unsigned char)(v >> 24);
}

/* The offset in RAM of a host pointer, or -1. */
static long ram_offset(const void *p)
{
    uintptr_t b = ram_base(), a = (uintptr_t)p;
    if (a < b || a >= b + PORT_RAM_SIZE) return -1;
    return (long)(a - b);
}

/* The buffer that CdGetSector is given: `n` bytes at the host address p must lie inside the PS1's RAM (the data
 * channel writes only there). Anything else ends the program with one line, before a byte is copied. The small
 * buffers of the other routines (a result of 8 bytes, a position of 3) are the game's own host memory, often its
 * stack, and are not checked. */
static void need_buffer(const void *p, unsigned long long n, const char *what)
{
    long off = ram_offset(p);

    if (off >= 0 && (unsigned long long)off + n <= PORT_RAM_SIZE) return;
    printf("stop: %s buffer 0x%08x (%llu bytes) is outside the PS1's RAM\n", what, (unsigned)(uintptr_t)p, n);
    fflush(stdout);
    exit(PORT_EXIT_DISC);
}

/* ---- positions ------------------------------------------------------- */

static unsigned char bcd(unsigned v)
{
    return (unsigned char)(((v / 10) << 4) + v % 10);
}

static void frames_to_pos(int frames, unsigned char *p)
{
    int f = frames % 75, s = (frames / 75) % 60, m = (frames / 75) / 60;
    p[0] = bcd((unsigned)m);
    p[1] = bcd((unsigned)s);
    p[2] = bcd((unsigned)f);
}

unsigned char *port_CdIntToPos(int i, unsigned char *p)
{
    frames_to_pos(i + 150, p);
    return p;
}

static int from_bcd(unsigned char b)
{
    return (b >> 4) * 10 + (b & 0xf);
}

int port_CdPosToInt(const unsigned char *p)
{
    return (from_bcd(p[0]) * 60 + from_bcd(p[1])) * 75 + from_bcd(p[2]) - 150;
}

/* A Setloc parameter that the controller accepts: BCD digits, second < 60,
 * frame < 75, at or after 00:02:00. Gives the image's sector. */
static int loc_valid(const unsigned char *p, unsigned *sector)
{
    int i;
    for (i = 0; i < 3; i++)
        if ((p[i] & 0xf) > 9 || (p[i] >> 4) > 9) return 0;
    if (from_bcd(p[1]) >= 60 || from_bcd(p[2]) >= 75) return 0;
    if (port_CdPosToInt(p) < 0) return 0;
    *sector = (unsigned)port_CdPosToInt(p);
    return 1;
}

/* ---- the drive -------------------------------------------------------- */

static void set_status(unsigned char s)
{
    cd.status = s;
    lib_put32(PORT_CD_VAR_STATUS, s);
}

/* The second response of Pause and SeekL arrives. */
static void settle_pending(void)
{
    if (!cd.pending) return;
    cd.pending = 0;
    set_status(cd.pending_status);
    memset(cd.sync_result, 0, sizeof cd.sync_result);
    cd.sync_result[0] = cd.pending_status;
    cd.sync = 2;
}

/* The wait in a blocking call: one step of the clock when no handler is
 * running (a handler is called from inside port_cd_tick, which must not be
 * entered again through the clock), then the pending response if the clock
 * did not bring it. */
static void wait_step(void)
{
    if (!cd.in_delivery) port_tick();
    settle_pending();
}

/* The first response of a command. */
static void respond(const unsigned char *bytes, unsigned n, unsigned char sync)
{
    memset(cd.sync_result, 0, sizeof cd.sync_result);
    memcpy(cd.sync_result, bytes, n);
    cd.sync = sync;
}

static int needs_param(int com)
{
    return com == 2 || com == 0xd || com == 0xe;
}

/* Send one command, as the library's inner routine does. 0 when the first
 * response came without an error; -1 for an error response; -2 for a missing
 * parameter. `fast`: do not wait for the first response (CdControlF); then
 * `result` is not used. */
static int issue(int com, const unsigned char *param, unsigned char *result, int fast)
{
    unsigned char one[8];
    unsigned sector;

    need_disc();
    switch (com) {
    case 1: case 2: case 6: case 9: case 0xd: case 0xe: case 0x11: case 0x15: case 0x1b:
        break;
    default:
        cd_stop("disc command", (unsigned)com);
    }
    if (needs_param(com) && !param) return -2;
    if (cd.sync == 3) {   /* wait for the previous command's second response */
        int n;
        for (n = 0; cd.sync == 3 && n < WAIT_LIMIT; n++) wait_step();
        settle_pending();
    }
    cd.sync = 0;
    lib_ptr(PORT_CD_VAR_LASTCOM)[0] = (unsigned char)com;
    one[0] = cd.status;

    switch (com) {
    case 1:
        respond(one, 1, 2);
        break;
    case 2:
        if (!loc_valid(param, &sector)) {
            one[0] = (unsigned char)(cd.status | 1u);
            one[1] = 0x40;   /* invalid parameter */
            respond(one, 2, 5);
        } else {
            cd.loc = sector;
            cd.loc_set = 1;
            respond(one, 1, 2);
        }
        break;
    case 6: case 0x1b:
        if (cd.mode & 0x20) cd_stop("disc mode (whole sector)", cd.mode);
        if (cd.loc_set) { cd.pos = cd.loc; cd.loc_set = 0; }
        cd.reading = 1;
        cd.ready = 0;
        set_status(ST_READ);
        one[0] = cd.status;
        respond(one, 1, 2);
        break;
    case 9:   /* the first response shows the state before, the second the idle state */
        cd.reading = 0;
        cd.pending = 1;
        cd.pending_status = ST_IDLE;
        set_status(ST_IDLE);
        respond(one, 1, 3);
        break;
    case 0x15:
        if (cd.loc_set) { cd.pos = cd.loc; cd.loc_set = 0; }
        cd.reading = 0;
        cd.pending = 1;
        cd.pending_status = ST_IDLE;
        set_status(ST_IDLE);
        respond(one, 1, 3);
        break;
    case 0xd:   /* the file and channel only matter to XA audio, which is dropped */
        respond(one, 1, 2);
        break;
    case 0xe:
        if (param[0] & 0x20) cd_stop("disc mode (whole sector)", param[0]);
        cd.mode = param[0];
        respond(one, 1, 2);
        break;
    case 0x11:   /* track, index, position in the track, absolute position */
        one[0] = 0x01;
        one[1] = 0x01;
        frames_to_pos((int)cd.pos, one + 2);
        frames_to_pos((int)cd.pos + 150, one + 5);
        respond(one, 8, 2);
        break;
    }
    lib_put32(PORT_CD_VAR_STATUS, cd.status);
    if (fast) return 0;
    if (result) memcpy(result, cd.sync_result, 8);
    return cd.sync == 5 ? -1 : 0;
}

/* Setloc-first for the commands that take a position. */
static int issue_with_position(int com, const unsigned char *param, unsigned char *result, int fast)
{
    int r;
    if (param && (com == 6 || com == 0x15 || com == 0x1b)) {
        r = issue(2, param, result, fast);
        if (r != 0) return r;
        /* the first response of the real command replaces it */
        return issue(com, 0, result, fast);
    }
    return issue(com, param, result, fast);
}

/* ---- the library functions ------------------------------------------ */

int port_CdInit(void)
{
    need_disc();
    cd.reading = 0;
    cd.pos = cd.loc = 0;
    cd.loc_set = 0;
    cd.mode = 0;
    cd.pending = 0;
    cd.have_sector = 0;
    cd.ready = 0;
    set_status(ST_IDLE);
    memset(cd.sync_result, 0, sizeof cd.sync_result);
    memset(cd.ready_result, 0, sizeof cd.ready_result);
    cd.sync_result[0] = ST_IDLE;
    cd.sync = 2;
    /* the library's default handlers only post events that the game never
     * waits on; none is the same as them for this program */
    lib_put32(PORT_CD_VAR_SYNC_CB, 0);
    lib_put32(PORT_CD_VAR_READY_CB, 0);
    lib_put32(PORT_CD_VAR_READ_CB, 0);
    return 1;
}

int port_CdSync(int mode, unsigned char *result)
{
    int n;
    need_disc();
    for (n = 0; n < WAIT_LIMIT; n++) {
        if (!cd.in_delivery) port_tick();
        if (cd.sync == 2 || cd.sync == 5) {
            int r = cd.sync;
            cd.sync = 2;
            if (result) memcpy(result, cd.sync_result, 8);
            return r;
        }
        if (mode) return 0;
        settle_pending();
    }
    printf("cd: CdSync timed out\n");
    return -1;
}

int port_CdReady(int mode, unsigned char *result)
{
    int n;
    need_disc();
    for (n = 0; n < WAIT_LIMIT; n++) {
        if (!cd.in_delivery) port_tick();
        if (cd.ready == 0 && !mode && cd.reading && !cd.in_delivery) port_cd_tick();
        if (cd.ready) {
            int r = cd.ready;
            cd.ready = 0;
            if (result) memcpy(result, cd.ready_result, 8);
            return r;
        }
        if (mode) return 0;
    }
    printf("cd: CdReady timed out\n");
    return -1;
}

int port_CdControl(int com, unsigned char *param, unsigned char *result)
{
    return issue_with_position(com & 0xff, param, result, 0) == 0;
}

int port_CdControlF(int com, unsigned char *param)
{
    return issue_with_position(com & 0xff, param, 0, 1) == 0;
}

int port_CdControlB(int com, unsigned char *param, unsigned char *result)
{
    if (issue_with_position(com & 0xff, param, result, 0) != 0) return 0;
    return port_CdSync(0, result) == 2;
}

int port_CdMix(void *vol)
{
    (void)vol;   /* no sound yet: the volumes are accepted */
    return 1;
}

int port_CdGetSector(void *madr, int words)
{
    unsigned long long want = words > 0 ? (unsigned long long)words * 4u : 0u;
    unsigned bytes, give = 0;
    unsigned char *dst = (unsigned char *)madr;
    long off;
    need_disc();
    if (want == 0) return 1;
    need_buffer(madr, want, "CdGetSector");   /* the count and the address are the game's: both inside the RAM, or nothing is copied */
    bytes = (unsigned)want;
    if (cd.have_sector && cd.offset < PORT_DATA) {
        give = PORT_DATA - cd.offset;
        if (give > bytes) give = bytes;
        memcpy(dst, cd.data + cd.offset, give);
        cd.offset += give;
    }
    if (give < bytes) memset(dst + give, 0, bytes - give);   /* the FIFO ran dry */
    off = ram_offset(dst);
    if (cd.have_sector && off >= 0) {
        unsigned first = (unsigned)off / 0x1000u, last = (unsigned)(off + (long)bytes - 1) / 0x1000u;
        for (; first <= last && first < NPAGES; first++) cd.page_source[first] = (int)cd.cur_sector;
        if (port_cd_written_hook) port_cd_written_hook(PORT_RAM_BASE + (unsigned)off, bytes);
    }
    return 1;
}

struct port_disc *port_cd_disc(void)
{
    return cd.disc;
}

int port_cd_page_source(unsigned address)
{
    if (address < PORT_RAM_BASE || address - PORT_RAM_BASE >= PORT_RAM_SIZE) return -1;
    return cd.page_source[(address - PORT_RAM_BASE) / 0x1000u];
}

/* ---- delivery ---------------------------------------------------------- */

static void call_ready(unsigned handler, int intr)
{
    if (!port_cd_call) port_target_check("ready callback", (const void *)(uintptr_t)handler);
    port_handler_depth++;   /* the vblank must not interrupt the ready handler */
    if (port_cd_call)
        port_cd_call(handler, intr, cd.ready_result);
    else
        ((void (*)(int, unsigned char *))(uintptr_t)handler)(intr, cd.ready_result);
    port_handler_depth--;
}

/* One sector of the read in progress. 0 when nothing more can be delivered now. */
static int deliver_one(void)
{
    unsigned handler = lib_get32(PORT_CD_VAR_READY_CB);
    unsigned char raw[PORT_SECTOR];
    unsigned sector;

    if (!cd.reading) return 0;
    /* without a handler the game polls; the next sector waits for the poll
     * to take the last one (the controller's FIFO holds one) */
    if (!handler && cd.ready) return 0;
    sector = cd.pos;
    if ((unsigned long long)sector >= cd.total ||
        fseek(cd.disc->f, (long)((unsigned long long)sector * PORT_SECTOR), SEEK_SET) != 0 ||
        fread(raw, 1, PORT_SECTOR, cd.disc->f) != PORT_SECTOR) {
        /* past the end of the image (or unreadable): a disc error, and the read stops */
        cd.reading = 0;
        set_status(ST_ERR);
        memset(cd.ready_result, 0, sizeof cd.ready_result);
        cd.ready_result[0] = ST_ERR;
        cd.ready_result[1] = 0x40;
        cd.ready = 5;
        if (handler) call_ready(handler, 5);
        return 0;
    }
    cd.pos = sector + 1;
    if ((cd.mode & 0x40) && (raw[18] & 0x04)) return 1;   /* XA audio: consumed, nothing sounds */
    memcpy(cd.data, raw + PORT_DATA_OFFSET, PORT_DATA);
    cd.have_sector = 1;
    cd.cur_sector = sector;
    cd.offset = 0;
    memset(cd.ready_result, 0, sizeof cd.ready_result);
    cd.ready_result[0] = ST_READ;
    cd.ready = 1;
    if (handler) call_ready(handler, 1);
    return 1;
}

void port_cd_tick(void)
{
    int n;
    if (!cd.started) return;
    settle_pending();
    if (cd.in_delivery) return;
    cd.in_delivery = 1;
    for (n = 0; n < PORT_CD_SECTORS_PER_TICK; n++)
        if (!deliver_one()) break;
    cd.in_delivery = 0;
}

int port_cd_init(struct port_disc *disc)
{
    unsigned i;
    memset(&cd, 0, sizeof cd);
    cd.disc = disc;
    cd.total = disc->length / PORT_SECTOR;
    for (i = 0; i < NPAGES; i++) cd.page_source[i] = -1;
    cd.status = ST_IDLE;
    cd.sync = 2;
    cd.started = 1;
    return 0;
}

/* ---- the table -------------------------------------------------------- */

const struct port_library port_cd_library[] = {
    { "CdInit", (void *)port_CdInit, 0 },
    { "CdSync", (void *)port_CdSync, 0 },
    { "CdReady", (void *)port_CdReady, 0 },
    { "CdControl", (void *)port_CdControl, 0 },
    { "CdControlF", (void *)port_CdControlF, 0 },
    { "CdControlB", (void *)port_CdControlB, 0 },
    { "CdMix", (void *)port_CdMix, "no sound yet: the CD audio volumes are accepted and nothing sounds" },
    { "CdGetSector", (void *)port_CdGetSector, 0 },
    { "CdIntToPos", (void *)port_CdIntToPos, 0 },
    { "CdPosToInt", (void *)port_CdPosToInt, 0 },
    { 0, 0, 0 }
};
