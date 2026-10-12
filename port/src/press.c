/* Sony's movie decoder library for the PC program: see press.h for what each call does and for the completion. */
#include "port.h"
#include "press.h"
#include "mdec.h"
#include <stddef.h>
#include <string.h>

/* PsyZ's decoder chip core (its src/psyz/mdec.c; the header there is the library's internal one). hostbuild.py defines
 * PORT_HAVE_PSYZ for this file when the program is built with the library (--psyz). Without it there is no chip: the three calls
 * that need one stop the program with a line that names the call, and the program links nothing of PsyZ. */
#ifdef PORT_HAVE_PSYZ
void Psyz_MdecReset(void);
int Psyz_MdecCommand(unsigned int command, const void *data, size_t words);
int Psyz_MdecRead(void *data, size_t words);
#define NEED_CHIP(call) ((void)0)
#else
#define NEED_CHIP(call) port_halt(PORT_EXIT_OTHER, "%s: this program was built without the picture library, which has the decoder's chip", call)
static void Psyz_MdecReset(void) {}
static int Psyz_MdecCommand(unsigned int command, const void *data, size_t words) { (void)command; (void)data; (void)words; return -1; }
static int Psyz_MdecRead(void *data, size_t words) { (void)data; (void)words; return -1; }
#endif

void *port_h_ResetCallback(void);

#define RAM_END (PORT_RAM_BASE + PORT_RAM_SIZE)

/* 0 none, 1 pending, 2 claimed by the timer thread, 3 taken and about to run (as iopage.c). */
static volatile int state;
static unsigned long long due_us;
static unsigned long long in_us;        /* the time of the frame's DecDCTin */
static unsigned long long handed;       /* bytes of the frame handed out so far */
static unsigned mb_bytes;               /* a macroblock's bytes at the depth of the frame's DecDCTin */
static unsigned dropped;                /* completions dropped for want of a routine */
/* The counts of port_press_report, since the report before: frames handed in, transfers out, completions taken, and completions
 * that a DecDCTReset dropped while they were pending. */
static unsigned n_frames, n_transfers, n_taken, n_reset;
static unsigned takes;                  /* completions taken since the start: DecDCTvlc's wait watches it */

/* The span [p, p + n) must lie inside the mapped RAM, else the program stops with the call's name. */
static void need(const char *call, const char *what, const void *p, unsigned long long n)
{
    unsigned long long a = (unsigned)(size_t)p;
    if (a < PORT_RAM_BASE || a + n > RAM_END)
        port_halt(PORT_EXIT_OTHER, "%s: %s at 0x%08x, %llu bytes, is not inside the game's RAM", call, what, (unsigned)a, n);
}

static void run_completion(void)
{
    port_fn f = port_iopage_dma_callback(1);
    state = 0;
    n_taken++;
    takes++;
    if (f) {
        port_target_check("decoder output callback", PORT_FN_ADDR(f));   /* its address checked now, as iopage.c does for its own */
        port_call0(f);
    } else {
        dropped++;
    }
}

/* Takes the completion that the caller has marked as taken or that the timer claimed, and after it every completion that the
 * routine's own DecDCTout made pending with a due time that has passed already (press.h, SEVERAL IN ONE CALL). The state, the due
 * time and the clock are read after each call of the routine, and run_completion reads the registered routine each time. A
 * completion whose due time is still ahead stays pending, and one that the timer claimed is left to the delivery. */
static void take_here(void)
{
    port_handler_depth++;
    do run_completion();
    while (state == 1 && port_now_us() >= due_us && __sync_bool_compare_and_swap(&state, 1, 3));
    port_handler_depth--;
}

/* Sony's routine waits for the transfer before (its MDEC_out_sync), and on the console the interrupt of that transfer's end comes
 * in the wait. Here the wait ticks, as the runtime's other waits do: the tick's poll (port_press_poll) takes the completion when its
 * due time has come and never before, so a completion is not delivered early because a second DecDCTout was called. If the routine
 * starts a further transfer the wait goes on for that one too, as Sony's does while the transfer is busy. Where no interruption is
 * allowed (inside a handler, interrupts off) the completion could never come: the program stops. */
#define PRESS_WAIT_LIMIT 100000u   /* ticks of a wait in which no completion comes (the clock does not move) */
static void wait_for_output(void)
{
    unsigned n;
    for (n = 0; state != 0; n++) {
        if (!port_interrupt_allowed())
            port_halt(PORT_EXIT_OTHER, "DecDCTout: the completion of the transfer before is pending and no interruption is allowed");
        if (n >= PRESS_WAIT_LIMIT) port_halt(PORT_EXIT_OTHER, "DecDCTout: the transfer before did not complete in %u ticks", PRESS_WAIT_LIMIT);
        port_tick();
    }
}

/* The time a decode of a frame's bitstream takes, as far as a program can tell: see press.h at DecDCTvlc. */
static void decode_lasts_past_a_pending_transfer(void)
{
    unsigned before = takes, n;
    if (!port_interrupt_allowed()) return;
    for (n = 0; state != 0 && takes == before; n++) {
        if (n >= PRESS_WAIT_LIMIT)
            port_halt(PORT_EXIT_OTHER, "DecDCTvlc: the completion of the output transfer that was pending did not come in %u ticks", PRESS_WAIT_LIMIT);
        port_tick();
    }
}

void port_DecDCTReset(int mode)
{
    if (mode != 0 && mode != 1) port_halt(PORT_EXIT_OTHER, "DecDCTReset: mode %d is not 0 or 1", mode);
    NEED_CHIP("DecDCTReset");
    if (mode == 0) port_h_ResetCallback();
    Psyz_MdecReset();
    if (state != 0) n_reset++;
    state = 0;
    if (mode == 0) {
        struct port_mdec t;
        unsigned char quant[128], scale[128];
        unsigned i;
        port_mdec_reset(&t);
        memcpy(quant, t.iq_y, 64);
        memcpy(quant + 64, t.iq_uv, 64);
        for (i = 0; i < 64; i++) {
            scale[2 * i] = (unsigned char)((unsigned short)t.scale[i] & 0xFF);
            scale[2 * i + 1] = (unsigned char)((unsigned short)t.scale[i] >> 8);
        }
        if (Psyz_MdecCommand(0x40000001u, quant, 32) != 0) port_halt(PORT_EXIT_OTHER, "DecDCTReset: the chip refused the quantisation tables");
        if (Psyz_MdecCommand(0x60000000u, scale, 32) != 0) port_halt(PORT_EXIT_OTHER, "DecDCTReset: the chip refused the scale table");
    }
}

int port_DecDCTvlc(unsigned long *bs, unsigned long *buf)
{
    unsigned a = (unsigned)(size_t)buf, b = (unsigned)(size_t)bs, head, used = 0;
    unsigned long long room;
    unsigned char *hbs, *hbuf;
    int rc;
    if (!bs) port_halt(PORT_EXIT_OTHER, "DecDCTvlc: a null bitstream (the continuation of a decode in parts)");
    need("DecDCTvlc", "the bitstream's header", bs, 8);
    need("DecDCTvlc", "the output (the command word, and the 64 end codes after the codes)", buf, 4 + 64 * 2);
    room = (RAM_END - a - 4 - 64 * 2) / 2;
    hbs = port_gs(bs, RAM_END - b);
    hbuf = port_gs(buf, RAM_END - a);
    memcpy(&head, hbs, 4);
    rc = port_mdec_vlc(hbs, RAM_END - b, (unsigned short *)hbuf + 2, (unsigned)room, &used);
    if (rc == PORT_MDEC_E_VERSION) {
        memcpy(hbuf, &head, 4);
        return -1;
    }
    if (rc != PORT_MDEC_OK) port_halt(PORT_EXIT_OTHER, "DecDCTvlc: the bitstream reader's result is %d", rc);
    memcpy(hbuf, &head, 4);
    {
        unsigned short *end = (unsigned short *)hbuf + 2 + used;
        unsigned i;
        for (i = 0; i < 64; i++) end[i] = 0xFE00;
    }
    decode_lasts_past_a_pending_transfer();
    return 0;
}

void port_DecDCTin(unsigned long *buf, int mode)
{
    unsigned w, count, depth;
    uint32_t *word;
    NEED_CHIP("DecDCTin");
    need("DecDCTin", "the command word", buf, 4);
    word = port_gs(buf, 4);
    w = *word;
    w = (mode & 1) ? (w & ~(1u << 27)) : (w | (1u << 27));
    w = (mode & 2) ? (w | (1u << 25)) : (w & ~(1u << 25));
    *word = w;
    if ((w >> 29) != 1) port_halt(PORT_EXIT_OTHER, "DecDCTin: the command word 0x%08x is not a decode", w);
    depth = (w >> 27) & 3;
    if (depth < 2) port_halt(PORT_EXIT_OTHER, "DecDCTin: the command word 0x%08x has a monochrome depth, which is not modelled", w);
    count = w & 0xFFFFu;
    if (count & 31) port_halt(PORT_EXIT_OTHER, "DecDCTin: %u words of codes is not a multiple of 32, which is not modelled", count);
    need("DecDCTin", "the codes", (const char *)buf + 4, (unsigned long long)count * 4);
    if (state != 0) port_halt(PORT_EXIT_OTHER, "DecDCTin: an output transfer is pending");
    if (Psyz_MdecCommand(w, port_gs((const char *)buf + 4, (size_t)count * 4), count) != 0) port_halt(PORT_EXIT_OTHER, "DecDCTin: the chip refused the command 0x%08x", w);
    mb_bytes = depth == 2 ? 768 : 512;
    handed = 0;
    in_us = port_now_us();
    n_frames++;
}

void port_DecDCTout(unsigned long *buf, int words)
{
    unsigned long long n;
    unsigned long long blocks;
    NEED_CHIP("DecDCTout");
    if (words < 0) port_halt(PORT_EXIT_OTHER, "DecDCTout: %d words", words);
    n = 32ull * ((unsigned)words >> 5);
    need("DecDCTout", "the strip", buf, n * 4);
    wait_for_output();
    if (Psyz_MdecRead(port_gs(buf, (size_t)(n * 4)), (size_t)n) != 0)
        port_halt(PORT_EXIT_OTHER, "DecDCTout: the chip cannot give %llu words (more than the frame holds, or broken codes)", n);
    handed += n * 4;
    blocks = mb_bytes ? (handed + mb_bytes - 1) / mb_bytes : 0;
    due_us = in_us + blocks * PORT_PRESS_US_PER_MACROBLOCK;
    n_transfers++;
    state = 1;
}

port_fn port_DecDCToutCallback(port_fn f)
{
    return (port_fn)(uintptr_t)port_h_DMACallback(1, f);
}

void port_press_poll(void)
{
    if (state == 1 && port_now_us() >= due_us && __sync_bool_compare_and_swap(&state, 1, 3)) take_here();
}

int port_press_claim_due(void)
{
    return state == 1 && port_now_us() >= due_us && __sync_bool_compare_and_swap(&state, 1, 2);
}

void port_press_deliver_claimed(void)
{
    if (state == 2) take_here();
}

int port_press_out_busy(void)
{
    return state != 0;
}

void port_press_report(const char *program)
{
    printf("decoder: program %s: %u frames, %u transfers, %u completions taken, %u dropped by a reset\n", program, n_frames, n_transfers, n_taken,
           n_reset);
    fflush(stdout);
    n_frames = n_transfers = n_taken = n_reset = 0;
}
