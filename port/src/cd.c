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
 * SeekL) completes in the next port_cd_tick (or when a blocking wait asks).
 * Sectors are delivered in port_cd_tick, at the rate of the console's drive,
 * counted BY THE FRAME AND NOT BY THE CLOCK: a vblank is a fixed amount of
 * drive time here, whatever the host's clock says. The drive reads 75 sectors
 * a second at single speed (the CD's own rate) and twice that, 150, with the
 * double-speed bit (0x80) of the mode set. At 60 vblanks a second that is 2.5
 * and 1.25 sectors a vblank. To lose no fraction, the layer keeps a carry in
 * fifths of a vblank's drive time: every vblank's delivery adds 5 to it, and a sector costs
 * 2 with the double-speed bit (2, 3, 2, 3, ... sectors a vblank) and 4
 * without it (1, 1, 1, 2, ...). The sectors of one vblank are delivered in a
 * row. The carry is cleared when a read starts (ReadN, ReadS) and when the
 * mode's speed bit changes. The drive is modelled as waiting: when the row
 * of a vblank ends because a sector could not be delivered (the read ended,
 * the handler paused it, or a polling reader has not taken the last one), the
 * carry left is cut to its remainder, the carry modulo the cost of a sector.
 * What the drive could not hand over at a vblank is not handed over later, so
 * the carry is below the cost at the start of every vblank and no vblank
 * brings more than 3 sectors at double speed or 2 at single speed (a blocked
 * reader never gets a burst when it starts taking again).
 *
 * The ready handler runs inside the row, and what it may do to the carry:
 *  - The cost of a sector is taken from the carry BEFORE deliver_one calls the
 *    handler (the drive's time for the sector is spent before the handler
 *    runs). Taking it first leaves the same remainder when the sector could
 *    not be delivered: (c - cost) modulo cost is c modulo cost.
 *  - A handler may start a read (ReadN, ReadS) or change the speed bit; both
 *    clear the carry. With the cost already taken the carry is then 0, below
 *    any cost, so the row ends by itself and the reset stands for the next
 *    vblank: no burst, no wrap of the unsigned carry, and the cost, which
 *    belongs to the old speed, is not used again. A handler that starts a read
 *    at every sector therefore brings exactly one sector a vblank.
 *  - Pause: reading is 0, the next deliver_one returns 0, the row ends with
 *    the remainder.
 *  - A Setmode that leaves the speed bit as it is changes nothing the row uses.
 *    CdInit from a handler stops the read (reading 0), so the row ends at the
 *    next round, before the changed mode could be used.
 *  - CdReady(0) from a handler: it does not call port_tick while a delivery is
 *    on, and its call of port_cd_tick is guarded by in_delivery; port_cd_tick
 *    itself returns before it touches the carry when a delivery is on. It
 *    takes the ready flag and leaves the carry as it is.
 *
 * The blocking wait of the game (CdReady with mode 0) is unchanged: when no
 * sector is ready and a read is on, it calls port_cd_tick itself, as before.
 * That call adds to the carry and delivers like a vblank's, so a wait can
 * bring more sectors in a second than the rate gives. That is the game
 * waiting on the drive, not the drive running ahead.
 *
 * The disc's streamed audio (see "The streamed audio" below): a sector of compressed audio that is played can be HELD. Then
 * deliver_one returns 0 before the position moves, the row ends, and the carry is cut to its remainder like every row that ends
 * for want of a sector. The cost of the held sector was taken before the handler would have run, as for every sector, and is not
 * given back: what the drive could not hand over now is not handed over later, so a held vblank is lost time, and a stream whose
 * queue stays full runs at the sound's pace (the sound empties the queue 44,100 frames a second; a played stereo sector at
 * 37,800 Hz is 2,352 of them). Nothing else of the sequence changes: no handler is called for an audio sector, a command that the
 * game sends meanwhile (Pause, a new read) is served at once, and the held sector is read again at the next row from the same
 * position (a new read or a seek moves the position, and the hold ends with the old one).
 *
 * THE DRIVE WAITS FOR THE GAME'S RING (a game-specific exception: this layer reads the game's memory).
 * The rate above is the console's and nothing more. The game reads a streamed load into a ring of seven places that its main loop
 * empties once per pass. On the console a pass fits in a frame or two. Here a pass can take several frames of the wall clock (a
 * slow host routine, a full trace, a busy machine) while the vblank, and with it the drive, keeps the frame count's pace. The ring
 * is then lapped. When the lost sectors were a sound bank's, the game's sound library never recovers and the game restarts the load
 * for ever. So the drive does not deliver a sector of a streamed load while the ring holds 4 sectors or more that the game has
 * not read.
 *  - Where it applies: a read is on, the mode's XA bit (0x40) is clear, and the ready handler stored in the library variable is
 *    the game's stream handler, func_8014f918 at 0x8014f918 (ps1/src/n14a648_r9.c; the game's function v10_r1.c stores it before a
 *    streamed load and the loaders store 0). With another handler, with none (the game polls) or with the XA bit set, there is no
 *    guard, whatever the ring holds.
 *  - The ring: bytes 0x16 and 0x17 of the game's stream object, ctl_80190948 at 0x80190948. The source shows that byte 0x16 is the
 *    place a sector is written to (the handler's work, 0x801e0000 plus the place times 2048; it is stepped 1 to 7, wrapping from 8 to
 *    1, l14de94_r3.c and s14f9f0_r3.c) and byte 0x17 the place the main loop reads next (l14de94_r4.c, wrapping the same way);
 *    both are 0 before a stream starts (s14ec80_r4.c, v10_r1.c). That the two bytes ARE a ring of seven that holds the written
 *    but unread sectors, and that the fill is (w + 7 - r) % 7, is INFERRED from that source and from traces of good loads.
 *    With either byte outside 1 to 7 there is no guard.
 *  - The rule: before each sector of a vblank's row, with the fill 4 or more, the sector is not delivered and the row ends, the
 *    carry cut to its remainder like every row that ends for want of a sector (the drive waits and does not catch up).
 *  - The bound: when 30 vblanks in a row delivered nothing because of the guard, the next vblank delivers its row regardless,
 *    with one line on the output and a count, so that a reader this guard misjudges cannot hang. A vblank that the guard did
 *    not hold, or one that delivered a sector, begins the count again.
 *  - Not guarded: the blocking wait of the game (CdReady with mode 0): that is the game asking for a sector. Its own delivery
 *    does not count as a held vblank and does not end a run of held ones.
 *  - The counts (checks, vblanks held, forced rows, the largest fill seen) go to the trace's last line. A run ended from outside
 *    prints no last line, so the trace also gets one line each time a run of held vblanks ends.
 * Why (one-off private runs of a trial, not a test): every lost lap fell where the ring's fill had reached 6; with every vblank's
 * sectors read in time the largest fill was 3; with this guard, a full trace and four busy processes, 8 loads of 294 sectors each
 * completed, the largest fill was 4, the guard held 2 vblanks and forced none.
 * Not designed: the general answer, the port's clock following the game's progress, so that no game-specific knowledge is
 * needed here. This is the game's memory in the disc layer, listed as an exception in port/README.md. */
#include "cd.h"
#include "xa.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

unsigned char *port_cd_ram;
void (*port_cd_written_hook)(unsigned address, unsigned bytes);
void (*port_cd_call)(unsigned handler, int intr, unsigned char *result);

#define NWORDS (PORT_RAM_SIZE / 4u)
#define ST_IDLE 0x02u   /* motor on */
#define ST_READ 0x22u   /* motor on, reading */
#define ST_ERR  0x03u   /* motor on, error */
#define WAIT_LIMIT 600  /* polls of a wait with nothing to wait for (about ten seconds) */
#define GUARD_STREAM  0x80190948u   /* ctl_80190948: the game's stream object */
#define GUARD_HANDLER 0x8014f918u   /* func_8014f918: the ready handler of a streamed load */
#define GUARD_FILL    4u            /* the fill at which the drive waits */
#define GUARD_HOLD    30u           /* vblanks held in a row before the next row is delivered regardless */

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
    int unguarded;                  /* the blocking wait of the game is delivering: the ring guard is off */
    unsigned guard_run;             /* vblanks in a row that delivered nothing because of the guard */
    unsigned guard_checked, guard_held, guard_forced, guard_max;   /* for the trace's last line */
    unsigned carry;                 /* the drive's time not yet turned into sectors, in fifths of a vblank (see the header) */
    int *word_source;               /* for each word of RAM: the sector of the image its bytes came from, -1 if none */
    unsigned char filter_file, filter_channel;   /* the last Setfilter */
} cd;

/* ---- the streamed audio ------------------------------------------------------------------------------------------------
 * A sector of compressed audio that the game plays (the mode's XA bit 0x40 and the sector's audio bit; with the mode's filter
 * bit 0x08 only the file and channel of the last Setfilter) is decoded by xa.c, resampled to 44,100 Hz and put in a queue one
 * second deep. The sound chip's model takes frames from the queue through Psyz_CdPullSamples (iopage.c defines it, in a build with
 * PsyZ, and calls port_xa_pull). When the queue lacks room for a sector the sector is not consumed (see the header). Without a
 * consumer (no --audio and no --audio-dump, or a build without PsyZ: port_xa_set_sink was not called) nothing would empty the
 * queue, so the sector is consumed in silence, as before.
 * A consumer that is gone. The drive waits for sound that is being played, without a bound. It must not wait for a consumer that has
 * stopped: the layer counts pulls (port_xa_pull). When an audio sector has been held for XA_GONE_VBLANKS vblanks in a row (by
 * port_frames) and no pull happened in that time, the sink is taken as gone: one line on the output, the queue emptied, and audio
 * sectors consumed in silence. A pull that arrives later puts the sink on again from the next audio sector, with the decoder reset
 * (one line). port_xa_set_sink(0) (the dump stopped, in iopage.c) switches the sink off for good and empties the queue.
 * The lock: the pull is called by PsyZ under its audio lock (the audio thread's callback; the dump's pull in iopage.c takes it
 * itself), so it does not take it. The game's thread takes it (port_audio_lock) for every access to the queue and its counts. */
#define XA_QUEUE_FRAMES 44100u
#define XA_GONE_VBLANKS 120u
static struct {
    int16_t q[XA_QUEUE_FRAMES * 2];
    unsigned head, count;               /* under the lock */
    unsigned char vol[4];               /* the last CdMix: left to left, left to right, right to right, right to left; 128 is unity */
    int passthrough;                    /* the volumes are (127 or 128, 0, 127 or 128, 0): the frames go out as they are */
    struct port_xa_counts n;            /* under the lock */
    int sink;                           /* something empties the queue */
    int started;                        /* a sector has been played since the read began */
    unsigned pulls;                     /* calls of the pull (under the lock) */
    int holding;                        /* a sector is being held: since frame hold_from, with hold_pulls pulls counted then */
    unsigned hold_from, hold_pulls;
    int gone, revived;                  /* the sink is taken as gone; a pull has come since (under the lock) */
    struct xa_state st;
} xa = { .vol = { 128, 0, 128, 0 }, .passthrough = 1 };
static int16_t xa_src[XA_MAX_FRAMES * 2];
static int16_t xa_out[XA_MAX_FRAMES * 3 * 2];   /* 4,032 frames at 18,900 Hz make 9,410 at 44,100 */

void port_xa_set_sink(int on)
{
    port_audio_lock();
    xa.sink = on;
    if (!on) {   /* nothing takes the frames any more: they are dropped, and the audio sectors are consumed in silence */
        xa.head = xa.count = 0;
        xa.holding = 0;
    }
    port_audio_unlock();
}

static int clamp16(int v)
{
    return v < -32768 ? -32768 : v > 32767 ? 32767 : v;
}

/* Frames out of the queue through the volumes of the last CdMix; the count given (0 with no stream). The caller holds the lock.
 * PsyZ's own pull does the same sum: out_left = (left * v0 + right * v3) >> 7, out_right = (left * v1 + right * v2) >> 7,
 * clamped to 16 bits, and no sum when the volumes are 127 or 128 straight through. */
size_t port_xa_pull(short *out, size_t num_frames)
{
    size_t give = num_frames < xa.count ? num_frames : xa.count, i;
    int v0 = xa.vol[0], v1 = xa.vol[1], v2 = xa.vol[2], v3 = xa.vol[3];

    for (i = 0; i < give; i++) {
        int l = xa.q[xa.head * 2], r = xa.q[xa.head * 2 + 1];
        if (xa.passthrough) {
            out[i * 2] = (short)l;
            out[i * 2 + 1] = (short)r;
        } else {
            out[i * 2] = (short)clamp16((l * v0 + r * v3) >> 7);
            out[i * 2 + 1] = (short)clamp16((l * v1 + r * v2) >> 7);
        }
        xa.head = (xa.head + 1) % XA_QUEUE_FRAMES;
    }
    xa.count -= (unsigned)give;
    xa.n.pulled += (unsigned)give;
    xa.pulls++;
    if (xa.gone) xa.revived = 1;
    return give;
}

void port_xa_counts_get(struct port_xa_counts *out)
{
    port_audio_lock();
    *out = xa.n;
    out->queued = xa.count;
    port_audio_unlock();
}

/* A read starts: the decoder and the queue begin again. */
static void xa_begin(void)
{
    port_audio_lock();
    xa_reset(&xa.st);
    xa.head = xa.count = 0;
    xa.started = 0;
    port_audio_unlock();
}

/* 1: the sector is consumed (played, skipped, refused or silent); 0: the queue lacks room and it is not consumed. */
static int xa_take(const unsigned char *raw, unsigned sector)
{
    struct xa_info info;
    int r;
    unsigned n, bound, i, tail, room;

    if ((cd.mode & 0x08) && (raw[16] != cd.filter_file || raw[17] != cd.filter_channel)) {
        port_audio_lock();
        xa.n.skipped++;
        port_audio_unlock();
        return 1;
    }
    if (!xa.sink) {
        xa.n.silent++;
        return 1;
    }
    port_audio_lock();
    if (xa.gone) {
        int back = xa.revived;
        if (back) {   /* a pull has come: the sink is on again, the decoder begins again */
            xa.gone = xa.revived = 0;
            xa_reset(&xa.st);
            xa.started = 0;
            xa.holding = 0;
        } else {
            xa.n.silent++;
        }
        port_audio_unlock();
        if (!back) return 1;
        printf("audio: something takes sound again; the disc's audio is played from this sector, with the decoder begun again\n");
        fflush(stdout);
    } else {
        port_audio_unlock();
    }
    r = xa_parse(raw, &info);
    if (r != XA_OK) {
        port_audio_lock();
        xa.n.refused++;
        port_audio_unlock();
        if (xa.n.refused <= 8)
            port_trace_line("xa: frame %u sector %u refused: %s (file %u channel %u submode 0x%02x coding 0x%02x)", port_frames(), sector,
                            r == XA_BAD_SUBHEADER ? "the two copies of the subheader differ" : r == XA_EIGHT_BIT ? "8 bit samples are not decoded" : "not audio",
                            raw[16], raw[17], raw[18], raw[19]);
        return 1;
    }
    bound = xa_resampled_bound(info.frames, (unsigned)info.rate);
    port_audio_lock();
    room = XA_QUEUE_FRAMES - xa.count;
    if (xa.count > 0 && room < bound) {   /* an empty queue always takes a sector */
        if (!xa.holding || xa.hold_pulls != xa.pulls) {   /* a new hold, or a pull came: the count of vblanks begins here */
            xa.holding = 1;
            xa.hold_pulls = xa.pulls;
            xa.hold_from = port_frames();
        } else if (port_frames() - xa.hold_from >= XA_GONE_VBLANKS) {   /* held for 120 vblanks and nothing pulled: the sink is gone */
            xa.gone = 1;
            xa.revived = 0;
            xa.holding = 0;
            xa.head = xa.count = 0;
            xa.n.silent++;
            port_audio_unlock();
            printf("audio: nothing has taken sound for %u vblanks; the disc's audio is consumed in silence until something does\n", XA_GONE_VBLANKS);
            fflush(stdout);
            return 1;
        }
        xa.n.held++;
        port_audio_unlock();
        return 0;
    }
    xa.holding = 0;
    port_audio_unlock();
    if (!xa.started) {   /* the first sector played since a read began: one line on the output, and one in the trace */
        printf("audio: a stream starts at sector %u: file %u channel %u, %s, %d Hz\n", sector, info.file, info.channel, info.stereo ? "stereo" : "mono", info.rate);
        fflush(stdout);
        port_trace_line("xa: frame %u stream starts: sector %u file %u channel %u, %s, %d Hz, %d bits", port_frames(), sector, info.file, info.channel,
                        info.stereo ? "stereo" : "mono", info.rate, info.bits);
    }
    xa_decode(&xa.st, raw, xa_src, &info);   /* parsed above: it cannot refuse */
    n = xa_resample(&xa.st, xa_src, info.frames, (unsigned)info.rate, xa_out);
    port_audio_lock();   /* only this thread adds, so the room found above is still there */
    tail = (xa.head + xa.count) % XA_QUEUE_FRAMES;
    for (i = 0; i < n; i++) {
        xa.q[tail * 2] = xa_out[i * 2];
        xa.q[tail * 2 + 1] = xa_out[i * 2 + 1];
        tail = (tail + 1) % XA_QUEUE_FRAMES;
    }
    xa.count += n;
    xa.n.played++;
    xa.n.frames += n;
    port_audio_unlock();
    xa.started = 1;
    return 1;
}

static void guard_report(void)
{
    if (!cd.guard_checked) return;   /* the guard never applied: a trace is what it was */
    port_trace_line("cd: the ring guard looked %u times, held %u vblanks, forced %u rows, largest fill %u", cd.guard_checked, cd.guard_held, cd.guard_forced, cd.guard_max);
}

static void xa_report(void)
{
    struct port_xa_counts c;
    port_xa_counts_get(&c);
    if (!(c.played || c.held || c.refused || c.skipped || c.silent)) return;   /* no audio sector was met: a trace is what it was */
    port_trace_line("xa: %u sectors played (%u frames at 44100 Hz queued, %u pulled), %u held, %u refused, %u skipped by the filter, %u consumed in silence",
                    c.played, c.frames, c.pulled, c.held, c.refused, c.skipped, c.silent);
}

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

/* The carry is cleared (a read starts, the speed bit changes). A delivery row in progress then ends: 0 is below any cost. */
static void clear_carry(void)
{
    cd.carry = 0;
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
        clear_carry();
        xa_begin();
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
    case 0xd:   /* the file and channel that the mode's filter bit lets through */
        cd.filter_file = param[0];
        cd.filter_channel = param[1];
        respond(one, 1, 2);
        break;
    case 0xe:
        if (param[0] & 0x20) cd_stop("disc mode (whole sector)", param[0]);
        if ((cd.mode ^ param[0]) & 0x80) clear_carry();
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
        if (cd.ready == 0 && !mode && cd.reading && !cd.in_delivery) {
            cd.unguarded = 1;   /* the game asks for a sector: the ring guard does not apply */
            port_cd_tick();
            cd.unguarded = 0;
        }
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
    /* CdlATV: four bytes, left to left, left to right, right to right, right to left (the order of PsyZ's CD_vol and of the game's
     * call); 128 is unity. A null pointer changes nothing. */
    if (vol) {
        const unsigned char *v = (const unsigned char *)vol;
        port_audio_lock();
        memcpy(xa.vol, v, 4);
        xa.passthrough = (v[0] == 127 || v[0] == 128) && (v[2] == 127 || v[2] == 128) && v[1] == 0 && v[3] == 0;
        port_audio_unlock();
    }
    return 1;
}

/* Record that the bytes [off, off + n) of the RAM now come from `sector` (-1: not from the disc). A word is
 * attributed only when all four of its bytes were written; a word that is only partly covered holds bytes of two
 * origins and is -1. */
static void record(unsigned off, unsigned n, int sector)
{
    unsigned first, last, w;
    if (!n || !cd.word_source) return;
    first = off / 4;
    last = (unsigned)(((unsigned long long)off + n - 1) / 4);
    for (w = first; w <= last && w < NWORDS; w++)
        cd.word_source[w] = (unsigned long long)w * 4 >= off && (unsigned long long)w * 4 + 4 <= (unsigned long long)off + n ? sector : -1;
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
    if (off >= 0) {
        record((unsigned)off, give, (int)cd.cur_sector);   /* the sector's bytes ... */
        record((unsigned)off + give, bytes - give, -1);    /* ... and the zeros of a dry FIFO, which are from nowhere */
        if (port_cd_written_hook) port_cd_written_hook(PORT_RAM_BASE + (unsigned)off, bytes);
    }
    return 1;
}

struct port_disc *port_cd_disc(void)
{
    return cd.disc;
}

int port_cd_source_at(unsigned address)
{
    if (!cd.word_source || address < PORT_RAM_BASE || address - PORT_RAM_BASE >= PORT_RAM_SIZE) return -1;
    return cd.word_source[(address - PORT_RAM_BASE) / 4u];
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
    if ((cd.mode & 0x40) && (raw[18] & 0x04)) {   /* XA audio: played, skipped or refused; never handed to the handler */
        if (!xa_take(raw, sector)) return 0;      /* no room in the queue: the sector stays on the disc, the position does not move */
        cd.pos = sector + 1;
        return 1;
    }
    cd.pos = sector + 1;
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

/* The ring guard (see the header): 1 when the sector must not be delivered now. `regardless` is set for a row that is delivered
 * regardless; the first sector it would have held is counted and reported. */
static int ring_full(int *regardless)
{
    const unsigned char *b;
    unsigned w, r, fill;

    if (cd.unguarded || !cd.reading || (cd.mode & 0x40) || lib_get32(PORT_CD_VAR_READY_CB) != GUARD_HANDLER) return 0;
    b = lib_ptr(GUARD_STREAM);
    w = b[0x16];
    r = b[0x17];
    if (w < 1 || w > 7 || r < 1 || r > 7) return 0;   /* no stream has run, or these are not the places */
    fill = (w + 7u - r) % 7u;
    cd.guard_checked++;
    if (fill > cd.guard_max) cd.guard_max = fill;
    if (fill < GUARD_FILL) return 0;
    if (*regardless == 1) {   /* the 31st vblank: delivered regardless, counted and reported once */
        *regardless = 2;
        cd.guard_forced++;
        printf("cd: the game's ring looked full for %u vblanks; delivering\n", GUARD_HOLD);
        fflush(stdout);
    }
    return *regardless == 0;
}

void port_cd_tick(void)
{
    unsigned cost;
    int regardless, delivered = 0, held = 0;
    if (!cd.started) return;
    settle_pending();
    if (cd.in_delivery) return;
    cd.in_delivery = 1;
    regardless = !cd.unguarded && cd.guard_run >= GUARD_HOLD;   /* 1: this row is delivered regardless (2 once it was needed) */
    cost = (cd.mode & 0x80) ? 2u : 4u;   /* read here: after the pending commands are settled, before any handler runs */
    cd.carry += 5;
    while (cd.carry >= cost) {
        if (ring_full(&regardless)) {   /* the sector waits: the drive waits and does not catch up */
            held = 1;
            cd.carry %= cost;
            break;
        }
        cd.carry -= cost;   /* the drive's time for this sector is spent before the handler runs; a reset by the handler leaves 0 and ends the row */
        if (!deliver_one()) {   /* the row ends: what the drive could not hand over now is not handed over later */
            cd.carry %= cost;   /* the same remainder as before the cost was taken */
            break;
        }
        delivered = 1;
    }
    if (!cd.unguarded) {
        if (held && !delivered) {
            cd.guard_run++;
            cd.guard_held++;
        } else {
            if (cd.guard_run)   /* the run ends: a trace shows each one, so that a run ended from outside (no last line) still has its counts */
                port_trace_line("cd: the ring guard held %u vblanks in a row, then a row was delivered (frame %u, largest fill so far %u)", cd.guard_run, port_frames(), cd.guard_max);
            cd.guard_run = 0;
        }
    }
    cd.in_delivery = 0;
}

int port_cd_init(struct port_disc *disc)
{
    unsigned i;
    free(cd.word_source);
    memset(&cd, 0, sizeof cd);
    cd.disc = disc;
    cd.total = disc->length / PORT_SECTOR;
    cd.word_source = malloc(NWORDS * sizeof *cd.word_source);
    if (!cd.word_source) return -1;
    for (i = 0; i < NWORDS; i++) cd.word_source[i] = -1;
    cd.status = ST_IDLE;
    cd.sync = 2;
    cd.started = 1;
    {
        static int registered;
        if (!registered) {
            registered = 1;
            atexit(xa_report);
            atexit(guard_report);
        }
    }
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
    { "CdMix", (void *)port_CdMix, "the four volume bytes are kept for the pull of the streamed audio" },
    { "CdGetSector", (void *)port_CdGetSector, 0 },
    { "CdIntToPos", (void *)port_CdIntToPos, 0 },
    { "CdPosToInt", (void *)port_CdPosToInt, 0 },
    { 0, 0, 0 }
};
